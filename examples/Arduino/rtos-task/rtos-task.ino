#include <Arduino.h>

HWCDC USBSerial;

int counter = 0;

void outputTask(void *pvParameters)
{

  for (;;)
  {
    USBSerial.print("RTOS Counter: ");
    USBSerial.println(++counter);

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  USBSerial.begin(115200);

  xTaskCreatePinnedToCore(
      outputTask,          // Function name of the task
      "output task",       // Name of the task (e.g. for debugging)
      1024,                // Stack size (bytes)
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