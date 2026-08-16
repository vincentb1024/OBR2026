#include "EV3Arduino.h"

task main()
{
	while(1){
		displayTextLine(0, "Sharp 1: %d", readArduino(S1, 0));
		displayTextLine(1, "Sharp 2: %d", readArduino(S1, 1));
		displayTextLine(2, "Sharp 3: %d", readArduino(S1, 2));
	}
}
