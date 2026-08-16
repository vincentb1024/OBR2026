#ifndef ARDUINOEV3_H
#define ARDUINOEV3_H

#include <Arduino.h>
#include <Wire.h>

#define MAX_I2C_DATA 16
#define DEFAULT_I2C_ADDRESS 0x04

class ArduinoEV3 {
public:
  void begin(uint8_t address = DEFAULT_I2C_ADDRESS, TwoWire* wire  = &Wire);
  
  template<typename T>
  static bool write(uint8_t index, T value) {
    if (index >= MAX_I2C_DATA || sizeof(T) > sizeof(int32_t)) return false;
    memcpy(&data[index], &value, sizeof(T));
    return true;
  }

  static int32_t read(uint8_t index);

private:
  static void onRequestHandler();
  
  static void onReceiveHandler(int numBytes);

  static int32_t data[MAX_I2C_DATA];    
  static uint8_t currentIndex; 
  static unsigned char instruction[5];
};

#endif
