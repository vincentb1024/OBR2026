int r1, g1, b1, r2, g2, b2, hue1, hue2;
float distance;
int black1 = 10;
int white1 = 99;
int black2 = 7;
int white2 = 95;
float kp = 0.9;
float kd = 0.3;
float lastError;
int i1,i2,error;

int r1_green = 11;
int g1_green = 20;
int hue1_green = 130;

int r2_green = 9;
int g2_green = 23;
int hue2_green = 133;

int r1_red = 75;
int g1_red = 14;
int hue1_red = 4;

int r2_red = 80;
int g2_red = 14;
int hue2_red = 5;


int margin_green = 15;
int margin_hue_green = 18;


void follow_line()
{
	i1 = 100*(r1- black1)/(white1-black1);
	i2 = 100*(r2 - black2)/(white2-black2);
	error = i1 - i2;
	if (abs(error) < 5) error = 0;
	motor[motorA] = 25 + error*kp + (lastError * kd);
	motor[motorB] = 25 - error*kp + (lastError * kd);
	lastError = error;

}

bool detect_green_right()
{
	return (r1 >= r1_green - margin_green && r1 <= r1_green + margin_green &&
	g1 >= g1_green - margin_green && g1 <= g1_green + margin_green &&
	hue1 >= hue1_green - margin_hue_green && hue1 <= hue1_green + margin_hue_green);
}

bool detect_green_left()
{
	return (r2 >= r2_green - margin_green && r2 <= r2_green + margin_green &&
	g2 >= g2_green - margin_green && g2 <= g2_green + margin_green &&
	hue2 >= hue2_green - margin_hue_green && hue2 <= hue2_green + margin_hue_green);
}

bool detect_red_right()
{
	return (r1 >= r1_red - margin_green && r1 <= r1_red + margin_green &&
	g1 >= g1_red - margin_green && g1 <= g1_red + margin_green &&
	hue1 >= hue1_red - margin_hue_green && hue1 <= hue1_red + margin_hue_green);
}

bool detect_red_left()
{
	return (r2 >= r2_red - margin_green && r2 <= r2_red + margin_green &&
	g2 >= g2_red - margin_green && g2 <= g2_red + margin_green &&
	hue2 >= hue2_red - margin_hue_green && hue2 <= hue2_red + margin_hue_green);
}


bool detect_black_right()
{
	return(r1<=black1 + 10);
}

bool detect_black_left()
{
	return(r2<=black2 + 10);
}

void turn(int degrees, int potA, int potB)
{
	moveMotorTarget(motorA,degrees,potA);
	moveMotorTarget(motorB,degrees,potB);
	waitUntilMotorStop(motorA);
	waitUntilMotorStop(motorB);
}

void move_forward(int degrees, int pot)
{
	moveMotorTarget(motorA,degrees,pot);
	moveMotorTarget(motorB,degrees,pot);
	waitUntilMotorStop(motorA);
	waitUntilMotorStop(motorB);
}

void avoid_obstacle()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
	delay(200);
	turn(350,30,-30);
	delay(200);
	move_forward(1000,40);
	delay(200);
	turn(350,-30,30);
	delay(200);
	move_forward(1000,40);
	turn(350,-30, 30);
	delay(200);
	move_forward(1000,40);
	delay(200);
	turn(350,30,-30);
}

task refresh_sensors()
{
	while(true)
	{
		getColorRGB(S1,r1,g1,b1);
		getColorRGB(S2,r2,g2,b2);
		hue1 = getColorHue(S1);
		hue2 = getColorHue(S2);
		distance = getUSDistance(S3);
	}
}

task main()
{
	startTask(refresh_sensors);

	while(true)
	{
		if (detect_red_left() && detect_red_right())
		{
			motor[motorA] = 0;
			motor[motorB] = 0;
			break;
		}
		else if ()
		{

		}
		else if(distance <= 10)
		{
			avoid_obstacle();
		}
		else if(detect_green_left() )
		{
			motor[motorA] = 0;
			motor[motorB] = 0;
			delay(400);

			if (detect_green_right())
			{
				move_forward(50,8);
				if(detect_black_right() || detect_black_left())
				{
					turn(900,25,-25);
				}
			}
			else
			{
				motor[motorA] = 0;
				motor[motorB] = 0;
				move_forward(50,8);
				if(detect_black_right() || detect_black_left())
				{
					move_forward(50,8);
					turn(350,15,-15);
				}
			}
		}

		else if(detect_green_right() )
		{
			motor[motorA] = 0;
			motor[motorB] = 0;
			delay(400);
			if (detect_green_left())
			{
				move_forward(50,8);
				if(detect_black_right() || detect_black_left())
				{
					turn(900,30,-30  );
				}
			}
			else
			{
				motor[motorA] = 0;
				motor[motorB] = 0;
				move_forward(55,10);
				if(detect_black_right() || detect_black_left())
				{
					move_forward(50,8);
					turn(350,-20,20);
				}
			}
		}
		else
		{
			follow_line();
		}
	}
}
