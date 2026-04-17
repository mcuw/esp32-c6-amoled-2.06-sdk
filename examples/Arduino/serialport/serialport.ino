#include <Arduino.h>

HWCDC USBSerial;

int counter = 0;

void setup() {
  USBSerial.begin(115200);
}

void loop() {
  USBSerial.print("Wearable AMOLED Watch - Counter: ");
  USBSerial.println(++counter);
  delay(1000);
}