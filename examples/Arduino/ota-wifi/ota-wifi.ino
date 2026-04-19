/**
 * OTA Wi-Fi Example
 *
 * This example demonstrates how to use the OTA library to enable over-the-air updates via Wi-Fi.
 * It sets up a Wi-Fi access point and starts the OTA service, allowing you to upload.
 *
 * 1. Make sure to copy credentials_template.h to credentials.h and edit the AP_PSK as needed before running this example.
 * 2. To upload the firmware via USB cable, you can either use the "Upload" button in your IDE or run the following command in the terminal:
 *    pio run -t upload
 * 3. You can connect to your ESP32 as an Access Point with the SSID "ESP32-XX" (where XX is the MAC address) and the password defined in AP_PSK in credentials.h.
 * 4. After the initial upload, you can upload new firmwares over the air using the following command:
 *    pio run -t upload --upload-port <IP_ADDRESS>
 *    Make sure to replace <IP_ADDRESS> with the actual IP address of the device when it is in AP mode (default is 192.168.4.1)
 *    or you can specify an extra_configs/upload_config.ini and comment out the line under the section platformio.extra_configs in platformio.ini
 *
 *  Note: The OTA upload will use the same PSK as the AP for authentication,
 *  so ensure that the AP_PSK in credentials.h matches the --auth parameter in upload_flags of extra_configs/upload_config.ini.
 */
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <OTA.h>
#include "credentials.h" // use credentials_template.h as a template for credentials

#define APP_VERSION "Version: v0.1"
HWCDC USBSerial;

// Over the air update object
OTA ota;

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

void otaTask(void *pvParameters)
{
  ota.begin(AP_PSK); // start the OTA service with the specified passphrase and activate as an Access Point

  for (;;)
  {
    ota.handle();
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  USBSerial.begin(115200);
  USBSerial.println("OTA upload over WiFi AP example");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin())
  {
    USBSerial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(0xFFFF);   // WHITE
  gfx->setTextColor(0x001F); // BLUE in 565 RGB format

  // center the text
  gfx->setTextSize(4);
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t w = 0;
  uint16_t h = 0;
  gfx->getTextBounds(APP_VERSION, 0, 0, &x1, &y1, &w, &h);
  gfx->setCursor((gfx->width() - w) / 2, (gfx->height() - h) / 2);

  gfx->println(APP_VERSION);

  xTaskCreatePinnedToCore(
      otaTask,             // Function name of the task
      "OTA task",          // Name of the task (e.g. for debugging)
      3 * 1024,            // Stack size (bytes)
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