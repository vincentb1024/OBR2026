#include "EV3Arduino.h"

int count=0;

task main()
{
	while(1){
		displayTextLine(0, "Sharp 1: %d", readArduino(S1, 0));
		displayTextLine(1, "count: %d", count);
		writeArduino(S1, 1, count);
		delay(1000);
		count++;
	}
}
