#include "ArduinoEV3.h"

ArduinoEV3 arduino;

void setup() {
  arduino.begin();
}

void loop() {
  arduino.read(0);
  delay(1000);
}
