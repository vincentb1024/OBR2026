#include "ArduinoEV3.h"

ArduinoEV3 arduino;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  arduino.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  count++;
  arduino.write(0, count);
  //delay(1000);
  //arduino.write(1, 1023);
  delay(1000);
}
