#include <Arduino.h>
#include <OneButton.h>

HWCDC USBSerial;

// Setup a new OneButton on pin BUTTON_BOOT
OneButton bootButton(BUTTON_BOOT, BUTTON_ACTIVELOW);


int counter = 0;

// This function will be called when the button was pressed 1 time (and no 2. button press followed).
void click() {
  USBSerial.print("Button clicked: ");
  USBSerial.println(++counter);
}


// This function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
  USBSerial.println("Button doubleclick.");
}


// This function will be called once, when the button is pressed for a long time.
void longPressStart() {
  USBSerial.println("Button longPress start");
}

// This function will be called often, while the button is pressed for a long time.
void longPress() {
  USBSerial.println("Button longPress...");
}


// This function will be called once, when the button is released after being pressed for a long time.
void longPressStop() {
  USBSerial.println("Button longPress stop");
}


void setup() {
  USBSerial.begin(115200);

  // link the button functions
  bootButton.attachClick(click);
  bootButton.attachDoubleClick(doubleclick);
  bootButton.attachLongPressStart(longPressStart);
  bootButton.attachLongPressStop(longPressStop);
  bootButton.attachDuringLongPress(longPress);
}

void loop() {
  bootButton.tick();
  delay(2);
}