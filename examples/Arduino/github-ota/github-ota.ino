/**
 * Reference app with OTA with GitHub and Wi-Fi connectivity for ESP32-C6 AMOLED 2.06" SDK
 * source code and credits to https://github.com/ittipu/IoT_Bhai_Youtube_Channel/blob/main/Mastering%20ESP32(English)/1.%20ESP32%20OTA%20Firmware%20Update%20from%20GitHub%20%20Step-by-Step/firmware/firmware.ino
 *
 * This example demonstrates how to set up an over-the-air (OTA) update service on the ESP32-C6 AMOLED 2.06" using both GitHub and Wi-Fi connectivity.
 * It allows you to upload new firmware versions wirelessly, making it easier to maintain and update your device without needing a physical connection.
 * 
 * To use this example:
 * 1. To upload the firmware via USB cable, you can either use the "Upload" button in your IDE or run the following command in the terminal:
 *    pio run -t upload
 * 2. Connect to the device's Wi-Fi network (SSID: "ESP32-C6-OTA", Password: "YOUR_PASSWORD").
 * 
 */
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <OneButton.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Update.h>

// The CI will bump the version in  ./version.txt when creating a new tag, e.g. v1.0.0
#define FIRMWARE_VERSION "v0.1.3"
#define VERSION_URL "https://raw.githubusercontent.com/mcuw/esp32-c6-amoled-2.06-sdk/refs/heads/main/version.txt"
#define FIRMWARE_URL "https://github.com/mcuw/esp32-c6-amoled-2.06-sdk/releases/download/%s/firmware_esp32_c6_amoled_2_06_%s.bin"

#define MARGIN_LEFT 0

HWCDC USBSerial;
volatile bool updateInProgress = false;

// display setup
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */, LCD_SDIO1 /* SDIO1 */,
    LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);
Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RESET /* RST */,
                                      0 /* rotation */, LCD_WIDTH, LCD_HEIGHT,
                                      22 /* col_offset1 */,
                                      0 /* row_offset1 */,
                                      0 /* col_offset2 */,
                                      0 /* row_offset2 */);

// Setup a new OneButton on pin BUTTON_BOOT
OneButton bootButton(BUTTON_BOOT, BUTTON_ACTIVELOW);


// This function will be called when the button was pressed 1 time (and no 2. button press followed).
void click() {
  updateInProgress = true;
}

String fetchLatestVersion() {
  HTTPClient http;
  http.begin(VERSION_URL);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String latestVersion = http.getString();
    http.end();
    latestVersion.trim(); // Remove any extra whitespace
    return latestVersion;
  }
  
  http.end();
  USBSerial.printf("Failed to fetch version. HTTP code: %d\n", httpCode);
  return "";
}

bool startOTAUpdate(WiFiClient* client, int contentLength) {
  USBSerial.println("Initializing update...");
  if (!Update.begin(contentLength)) {
    USBSerial.printf("Update begin failed: %s\n", Update.errorString());
    return false;
  }

  USBSerial.println("Writing firmware...");
  size_t written = 0;
  int progress = 0;
  int lastProgress = 0;

  // Timeout variables
  const unsigned long timeoutDuration = 120*1000;  // 120 seconds timeout
  unsigned long lastDataTime = millis();

  while (written < contentLength) {
    if (client->available()) {
      uint8_t buffer[128];
      size_t len = client->read(buffer, sizeof(buffer));
      if (len > 0) {
        Update.write(buffer, len);
        written += len;

        // Calculate and print progress
        progress = (written * 100) / contentLength;
        if (progress != lastProgress) {
          USBSerial.printf("Writing Progress: %d%%\n", progress);
          lastProgress = progress;
        }
      }
    }
    // Check for timeout
    if (millis() - lastDataTime > timeoutDuration) {
      USBSerial.println("Timeout: No data received for too long. Aborting update...");
      Update.abort();
      return false;
    }

    yield();
  }
  USBSerial.println("\nWriting complete");

  if (written != contentLength) {
    USBSerial.printf("Error: Write incomplete. Expected %d but got %d bytes\n", contentLength, written);
    Update.abort();
    return false;
  }

  if (!Update.end()) {
    USBSerial.printf("Error: Update end failed: %s\n", Update.errorString());
    return false;
  }

  USBSerial.println("Update successfully completed");
  return true;
}

void downloadAndApplyFirmware(const String& latestVersion) {
  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  char url[256];
  snprintf(url, sizeof(url), FIRMWARE_URL, latestVersion.c_str(), latestVersion.c_str());
  USBSerial.printf("Download URL: %s\n", url);
  http.begin(url);

  int httpCode = http.GET();
  USBSerial.printf("HTTP GET code: %d\n", httpCode);

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    USBSerial.printf("Firmware size: %d bytes\n", contentLength);

    if (contentLength > 0) {
      WiFiClient* stream = http.getStreamPtr();
      if (startOTAUpdate(stream, contentLength)) {
        USBSerial.println("OTA update successful, restarting...");
        gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
        gfx->println("Update successful, restarting...");
        delay(2000);
        ESP.restart();
      } else {
        USBSerial.println("OTA update failed");
        gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
        gfx->println("Update failed");
      }
    } else {
      USBSerial.println("Invalid firmware size");
      gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
      gfx->println("Invalid firmware size");
    }
  } else {
    USBSerial.printf("Failed to fetch firmware. HTTP code: %d\n", httpCode);
    gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
    gfx->printf("Failed to fetch firmware. HTTP code: %d\n", httpCode);
  }
  http.end();
}


void checkForFirmwareUpdate() {
  USBSerial.println("Checking for firmware update...");
  gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
  gfx->println("Check updates...");

  if (WiFi.status() != WL_CONNECTED) {
    USBSerial.println("WiFi not connected");
    gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
    gfx->println("WiFi not connected");
    return;
  }

  // Step 1: Fetch the latest version from GitHub
  String latestVersion = fetchLatestVersion();
  if (latestVersion == "") {
    USBSerial.println("Failed to fetch latest version");
    gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
    gfx->println("Failed to fetch latest version");
    return;
  }

  USBSerial.println("Current Version: " + String(FIRMWARE_VERSION));
  USBSerial.println("Latest Version: " + latestVersion);
  gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
  gfx->print("Latest Version: ");
  gfx->println(latestVersion);


  // Step 2: Compare versions
  if (latestVersion != FIRMWARE_VERSION) {
    USBSerial.println("New firmware available. Starting OTA update...");
    gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
    gfx->println("Starting OTA update...");

    downloadAndApplyFirmware(latestVersion);
  } else {
    USBSerial.println("Device is up to date.");
    gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
    gfx->println("Device is up to date.");
  }
}

void buttonTask(void *pvParameters)
{

  for (;;)
  {
    bootButton.tick();
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

void otaTask(void *pvParameters)
{

  for (;;)
  {
    if (updateInProgress) {
      updateInProgress = false;
      checkForFirmwareUpdate();
    }
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  USBSerial.begin(115200);
  USBSerial.println("OTA update with GitHub over WiFi connectivity");
  
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin())
  {
    USBSerial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(0x0000);   // BLACK
  gfx->setTextColor(0x001F); // BLUE in 565 RGB format

  // center the version text
  gfx->setTextSize(6);
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t w = 0;
  uint16_t h = 0;
  gfx->getTextBounds(FIRMWARE_VERSION, 0, 0, &x1, &y1, &w, &h);
  gfx->setCursor((gfx->width() - w) / 2, 0);
  gfx->println(FIRMWARE_VERSION);
  gfx->println();

  gfx->setTextSize(3);
  gfx->setTextColor(0xFFFF); // WHITE in 565 RGB format

  // initialize button feature
  bootButton.attachClick(click);
  xTaskCreatePinnedToCore(
    buttonTask,          // Function name of the task
    "button task",       // Name of the task (e.g. for debugging)
    512,                 // Stack size (bytes)
    NULL,                // Parameter to pass
    0,                   // Task priority
    NULL,                // Assign task handle
    ARDUINO_RUNNING_CORE // use same single core
  );

  // connect to Wi-Fi
  WiFiManager wm;
  USBSerial.println("autoConnect to WiFi...");
  gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
  gfx->println("WiFi autoConnect...");
  bool result = wm.autoConnect("ESP32-C6-OTA", "YOUR_PASSWORD");
  if (!result) {
      USBSerial.println("Failed to connect. Restarting...");
      delay(5000);
      ESP.restart();
  }
  //if you get here you have connected to the WiFi    
  USBSerial.print("WiFi connected! IP address: ");
  USBSerial.println(WiFi.localIP());
  gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
  gfx->println("WiFi connected");

  gfx->setCursor(MARGIN_LEFT, gfx->getCursorY());
  gfx->print("IP: ");
  gfx->println(WiFi.localIP());


  // ready to check for updates when button is pressed
  xTaskCreatePinnedToCore(
    otaTask,             // Function name of the task
    "OTA task",          // Name of the task (e.g. for debugging)
    8 * 1024,            // Stack size (bytes)
    NULL,                // Parameter to pass
    0,                   // Task priority
    NULL,                // Assign task handle
    ARDUINO_RUNNING_CORE // use same single core
  );
}

void loop()
{
  vTaskDelete(NULL); // current task is not needed anymore or delete this line to keep the loop running

  // delay(10);
}
