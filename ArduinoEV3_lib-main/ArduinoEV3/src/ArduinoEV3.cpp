#include "ArduinoEV3.h"


int32_t ArduinoEV3::data[MAX_I2C_DATA] = {0};
uint8_t ArduinoEV3::currentIndex = 0;
unsigned char ArduinoEV3::instruction[5]={5,0,0,0,0};;

void ArduinoEV3::begin(uint8_t address, TwoWire* wire) {
  wire->begin(address);
  wire->onRequest(onRequestHandler);
  wire->onReceive(onReceiveHandler);
}

int32_t ArduinoEV3::read(uint8_t index){
  return (data[index]);
}

void ArduinoEV3::onReceiveHandler(int numBytes) {
  byte read_byte = numBytes;
  int byte_count = 0;

  while(1 < Wire.available()) // loop through all but the last
  {
    read_byte = Wire.read(); 

    instruction[byte_count] = read_byte;

    byte_count++;
  }
  int lastByte = Wire.read(); // Read the last dummy byte (has no meaning, but must read it)


  if(instruction[0] == 0) currentIndex = instruction[1]; //define index to write
  else if(instruction[0] == 1){
    int32_t x = (instruction[4]) | (instruction[3]<<8) | (instruction[2]<<16); //decode 3 last bytes
    write(instruction[1], x); //write in data array index
  }
}

void ArduinoEV3::onRequestHandler() {
  if(instruction[0] == 0) Wire.write((uint8_t*)&data[currentIndex], sizeof(int32_t));
}