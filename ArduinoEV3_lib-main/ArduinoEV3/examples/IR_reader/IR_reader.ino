#include <SharpIR.h>

#include "ArduinoEV3.h"

ArduinoEV3 arduino;

#define ir1 A0
#define ir2 A1
#define ir3 A2
#define model 1080

SharpIR SharpIR1(ir1, model);
SharpIR SharpIR2(ir2, model);
SharpIR SharpIR3(ir3, model);

void setup() {
  arduino.begin();
  Serial.begin(9600);
}

void loop() {
  arduino.write(0, SharpIR1.distance());
  arduino.write(1, SharpIR2.distance());
  arduino.write(2, SharpIR3.distance());
}
