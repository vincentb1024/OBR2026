#ifndef ARDUINO_I2C_H
#define ARDUINO_I2C_H

// First, define the Arduino Address
// Address is 0x04 on the Arduino: 0100
// Bit shifted out with one 0, that becomes: 1000 (LEGO expects 8 bit address)
// Which is 0x08
byte ARDUINO_ADDRESS =	0x08;    // Arduino: 0x04

ubyte I2Cmessage[22];
unsigned char I2Creply[20];

void set_address(byte new_address){
	ARDUINO_ADDRESS = new_address << 1;
}

//messsage size max is 5
int i2c_msg(tSensors port, byte ard_address, int message_size, int return_size, ubyte byte0, ubyte byte1, ubyte byte2, ubyte byte3 ,ubyte byte4)
{
	memset(I2Creply, 0, sizeof(I2Creply));
	message_size = message_size+3;

	I2Cmessage[0] = message_size; // Messsage Size
	I2Cmessage[1] = ard_address;

	I2Cmessage[2] = byte0;
	I2Cmessage[3] = byte1;
	I2Cmessage[4] = byte2;
	I2Cmessage[5] = byte3;
	I2Cmessage[6] = byte4; // max is 99 only for I2Cmessage[6]	
	//// can't add more than 5 Bytes

	sendI2CMsg(port, &I2Cmessage[0], return_size);
	wait1Msec(20);

	readI2CReply(port, &I2Creply[0], return_size);

	int x = (I2Creply[0]) | (I2Creply[1]<<8) |
			(I2Creply[2]<<16) | (I2Creply[3]<<24);

	wait1Msec(35);
	return x;
}

int readArduino(tSensors port, int index){
	int value = i2c_msg(port, ARDUINO_ADDRESS, 1, 4, 0, index, 0 , 0, 0); //0 for read (parameter 5)
	return value;
}

void writeArduino(tSensors port, int index, int send){
	unsigned char* bytes = (unsigned char*)&send;
	int value = i2c_msg(port, ARDUINO_ADDRESS, 5, 0, 1, index, bytes[2] , bytes[1], bytes[0]);//1 for write (parameter 5)
	return;
}

#endif