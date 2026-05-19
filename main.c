int r1, g1, b1, r2, g2, b2, hue1, hue2;
float distance;
float distancel;
int black1 = 12;
int white1 = 98;
int black2 = 8;
int white2 = 96;
float kp = 0.8;
float kd = 0.3;
float lastError;
int i1,i2,error;

int r1_green = 12;
int g1_green = 18;
int hue1_green = 121;

int r2_green = 8;
int g2_green = 21;
int hue2_green = 138;

int r1_red = 67;
int g1_red = 12;
int hue1_red = 4;

int r2_red = 65;
int g2_red = 15;
int hue2_red = 5;

int r1_silver = 110;
int g1_silver = 110;
int b1_silver = 110;

int r2_silver = 110;
int g2_silver = 110;
int b2_silver = 110;

int margin = 10;
int margin_hue = 15;

void stop()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
}

void follow_line()
{
	i1 = 100*(r1- black1)/(white1-black1);
	i2 = 100*(r2 - black2)/(white2-black2);
	error = i1 - i2;
	if (abs(error) < 5) error = 0;
	motor[motorA] = 30 + (error * kp) + ((error - lastError) * kd);
	motor[motorB] = 30 - (error * kp) + ((error - lastError) * kd);
	lastError = error;

}

bool detect_green_right()
{
	return (r1 >= r1_green - margin && r1 <= r1_green + margin &&
	g1 >= g1_green - margin && g1 <= g1_green + margin &&
	hue1 >= hue1_green - margin_hue && hue1 <= hue1_green + margin_hue);
}

bool detect_green_left()
{
	return (r2 >= r2_green - margin && r2 <= r2_green + margin &&
	g2 >= g2_green - margin && g2 <= g2_green + margin &&
	hue2 >= hue2_green - margin_hue && hue2 <= hue2_green + margin_hue);
}

bool detect_silver_right()
{
	return (r1 >= r1_silver - margin && r1 <= r1_silver + margin &&
	g1 >= g1_silver - margin && g1 <= g1_silver + margin &&
	b1 >= b1_silver - margin && b1 <= b1_silver+margin);
}

bool detect_silver_left()
{
	return (r2 >= r2_silver - margin && r2 <= r1_silver + margin &&
	g2 >= g2_silver - margin && g2 <= g2_silver + margin &&
	b2 >= b2_silver - margin && b2 <= b2_silver + margin);
}

bool detect_red_right()
{
	return (r1 >= r1_red - margin && r1 <= r1_red + margin &&
	g1 >= g1_red - margin && g1 <= g1_red + margin &&
	hue1 >= hue1_red - margin_hue && hue1 <= hue1_red + margin_hue);
}

bool detect_red_left()
{
	return (r2 >= r2_red - margin && r2 <= r2_red + margin &&
	g2 >= g2_red - margin && g2 <= g2_red + margin &&
	hue2 >= hue2_red - margin_hue && hue2 <= hue2_red + margin_hue);
}

bool detect_black_right()
{
	return(r1 <= black1 + 10);
}

bool detect_black_left()
{
	return(r2 <= black2 + 10);
}

void turn(int degrees, int potB, int potA)
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
	stop();
	delay(100);
	turn(480,40,-40);
	stop();
	delay(100);
	move_forward(410,40);
	stop();
	delay(100);
	turn(480,-40,40);
	stop();
	delay(100);
	move_forward(780,40);
	stop();
	delay(100);
	turn(480,-40,40);
	stop();
	delay(100);
	move_forward(425,40);
	stop();
	delay(100);
	turn(480,40,-40);
	stop();
	delay(100);
	move_forward(100,40);
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
		distancel = getUSDistance(S4);
		delay(10);
	}
}
void ball_room()
{
	float maxld=15, maxfd=5;
	move_forward(50,15);
	if(distancel>maxld)
	{
		turn(450,40,-40);
		move_forward(100,15);
	}

	while((detect_black_left() || detect_black_right()) == false)
	{
		if(distance<maxfd)
		{
			turn(50,-40,40);
		}
		else
		{
			move_forward(20,15);
		}
		if(distancel>maxld)
		{
			turn(450,40,-40);
			move_forward(50,15);
		}

	}
}

task main()
{
	startTask(refresh_sensors);

	while(true)
	{
		if (detect_red_left() || detect_red_right())
		{
			motor[motorA] = 0;
			motor[motorB] = 0;
			break;
		}
		else if(detect_silver_left() || detect_silver_right())
		{
			ball_room();
		}
		else if(distance <= 6)
		{
			avoid_obstacle();
		}
		else if(detect_green_left() )
		{
			delay(100);
			motor[motorA] = 0;
			motor[motorB] = 0;
			delay(100);

			if (detect_green_right())
			{
				move_forward(50,8);
				if(detect_black_right() || detect_black_left())
				{
					turn(960,25,-15);
				}
			}
			else
			{
				motor[motorA] = 0;
				motor[motorB] = 0;
				move_forward(55,8);
				if(detect_black_left())
				{
					move_forward(55,8);
					turn(450,-25,25);
				}
				else{
					move_forward(85,8);
					if(r1 >= 90 && r2 >= 90){
						move_forward(95,-8);
						turn(75,9,-9);
					}
				}
			}
		}

		else if(detect_green_right() )
		{
			delay(100);
			motor[motorA] = 0;
			motor[motorB] = 0;
			delay(100);
			if (detect_green_left())
			{
				move_forward(55,8);
				if(detect_black_right() || detect_black_left())
				{
					turn(960,25,-25);
				}
			}
			else
			{
				motor[motorA] = 0;
				motor[motorB] = 0;
				move_forward(55,8);
				if(detect_black_right())
				{
					move_forward(55,8);
					turn(450,25,-25);
				}
				else
				{
					move_forward(85,8);
					if(r1 >= 90 && r2 >= 90){
						move_forward(95,-8);
						turn(75,-9,9);
					}
				}
			}
		}
		else
		{
			follow_line();
		}
	}
}
