int r1, g1, b1, r2, g2, b2, hue1, hue2;
float distance_front, distance_left;
bool black_detected = false;
int white1 = 114;
int black1 = 11;
int white2 = 109;
int black2 = 14;

float kp = 0.8;
float kd = 0.3;
float last_error;
int i1, i2, error;

int r1_green = 13;
int g1_green = 30;
int hue1_green = 115;

int r2_green = 11;
int g2_green = 40;
int hue2_green = 121;

int r1_red = 75;
int g1_red = 12;
int hue1_red = 11;

int r2_red = 98;
int g2_red = 11;
int hue2_red = 5;

int margin = 10;
int margin_hue = 15;

void stop_motors()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
}

void turn(int degrees, int speed_B, int speed_A)
{
	moveMotorTarget(motorA, degrees, speed_A);
	moveMotorTarget(motorB, degrees, speed_B);
	waitUntilMotorStop(motorA);
	waitUntilMotorStop(motorB);
}

void move_forward(int degrees, int speed)
{
	moveMotorTarget(motorA, degrees, speed);
	moveMotorTarget(motorB, degrees, speed);
	waitUntilMotorStop(motorA);
	waitUntilMotorStop(motorB);
}

void follow_line()
{
	i1 = 100 * (r1 - black1) / (white1 - black1);
	i2 = 100 * (r2 - black2) / (white2 - black2);

	error = i1 - i2;

	if(abs(error) < 5) error = 0;

	float correction = (error * kp) + ((error - last_error) * kd);
	int base_speed = 35 - abs(error) * 0.5;

	if(base_speed < 12) base_speed = 12;

	int power_A = base_speed + correction;
	int power_B = base_speed - correction;

	if(power_A > 100) power_A = 100;
	if(power_A < -100) power_A = -100;
	if(power_B > 100) power_B = 100;
	if(power_B < -100) power_B = -100;

	motor[motorA] = power_A;
	motor[motorB] = power_B;

	last_error = error;
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
	return ((r1 + g1 + b1) / 3) >= 110;
}

bool detect_silver_left()
{
	return ((r2 + g2 + b2) / 3) >= 110;
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
	return (r1 <= black1 + margin);
}

bool detect_black_left()
{
	return (r2 <= black2 + margin);
}

void avoid_obstacle()
{
	stop_motors();
	delay(10);
	move_forward(70, -8);
	delay(10);
	turn(480, 35, -35);
	delay(30);
	move_forward(105,10);
	while(distance_left < 20)
	{
		motor[motorA] = 18;
		motor[motorB] = 18;
		delay(5);
	}
	move_forward(185, 10);
	delay(30);
	turn(480, -35, 35);
	while(distance_left > 20)
	{
		motor[motorA] = 18;
		motor[motorB] = 18;
		delay(5);
	}
	while(distance_left < 20)
	{
		motor[motorA] = 18;
		motor[motorB] = 18;
		delay(5);
	}
	move_forward(190, 10);
	turn(480, -35, 35);
	move_forward(150, 10);
	while((detect_black_left() || detect_black_right()) == false)
	{
		motor[motorA] = 15;
		motor[motorB] = 15;
		delay(5);
	}
	move_forward(85, 10);
	turn(480, 35, -35);
}

task detect_black()
{
	while(true)
	{
		if(detect_black_left() || detect_black_right())
		{
			black_detected = true;
		}
		else
		{
			black_detected = false;
		}
		delay(10);
	}
}

void find_line()
{
	while(true)
	{
		clearTimer(T2);
		black_detected = false;
		startTask(detect_black);
		move_forward(85,10);
		if(black_detected == true)
		{
			stop_motors();
			playSound(soundBlip);
			stopTask(detect_black);
			break;
		}
		repeatUntil(black_detected == true || time1[T2] > 1500)
		{
			motor[motorB] = 10;
			motor[motorA] = -10;
		}
		repeatUntil(black_detected == true || time1[T2] > 1500)
		{
			motor[motorB] = -10;
			motor[motorA] = 10;
		}
	}
}

void rescue_room()
{
	int min_left = 4;
	move_forward(320,30);
	black_detected = false;
	startTask(detect_black);
	while(true)
	{
		if(black_detected == true)
		{
			stop_motors();
			playSound(soundBlip);
			stopTask(detect_black);
			find_line();
			break;
		}
		else if(distance_left <= 25 && distance_front < 25)
		{
			turn(250,30,-30);
		}
		else if(distance_left <= 25 && distance_front >= 25)
		{
			motor[motorA] = 20;
			motor[motorB] = 20;
		}
		else if(distance_front < 10 && distance_left > 15)
		{
			move_forward(70,-8);
			turn(480,35,-35);
		}
		else if(distance_left > 35)
		{
			move_forward(110,10);
			turn(480,-30,30);
			move_forward(110,8);
			clearTimer(T1);
			repeatUntil(black_detected == true || distance_front < 30 || time1[T1] > 3500)
			{
				motor[motorA] = 15;
				motor[motorB] = 15;
			}
		}
		else if(distance_left >= 25 && distance_front >= 25)
		{
			motor[motorA] = 20;
			motor[motorB] = 20;
		}
		else if(distance_left < min_left)
		{
			turn(35,20,-20);
			move_forward(80,10);
		}
		else
		{
			motor[motorA] = 20;
			motor[motorB] = 20;
		}
	}
}

task refresh_sensors()
{
	while(true)
	{
		getColorRGB(S1, r1, g1, b1);
		getColorRGB(S2, r2, g2, b2);
		hue1 = getColorHue(S1);
		hue2 = getColorHue(S2);
		distance_front = getUSDistance(S3);
		distance_left = getUSDistance(S4);
		delay(3);
	}
}

task main()
{
	startTask(refresh_sensors);
	while(true)
	{
		if (detect_red_left() || detect_red_right())
		{
			stop_motors();
			move_forward(70, -10);
			delay(5);
			move_forward(70, 10);
			delay(5);
			if (detect_red_left() || detect_red_right())
			{
				stop_motors();
				playSound(soundLowBuzz);
				break;
			}
		}
		else if(detect_silver_left() || detect_silver_right())
		{
			playSound(soundBeepBeep);
			rescue_room();
		}
		else if(distance_front <= 4)
		{
			avoid_obstacle();
		}
		else if(detect_green_left())
		{
			stop_motors();
			delay(5);
			move_forward(15,8);
			if (detect_green_right())
			{
				move_forward(55, 8);
				if(detect_black_right() || detect_black_left())
				{
					turn(960, 25, -25);
					delay(5);
					move_forward(60, 10);
					delay(5);
				}
			}
			else
			{
				move_forward(55, 8);
				if(detect_black_left() || detect_black_right())
				{
					move_forward(55, 8);
					turn(480, -25, 25);
					move_forward(55, 8);
				}
				else
				{
					move_forward(85, 10);
					if(r1 >= 88 && r2 >= 88)
					{
						move_forward(105, -10);
						turn(75, 10, -10);
					}
				}
			}
		}
		else if(detect_green_right())
		{
			stop_motors();
			delay(5);
			move_forward(15,8);
			if (detect_green_left())
			{
				move_forward(55, 8);
				if(detect_black_right() || detect_black_left())
				{
					turn(960, 25, -25);
					delay(5);
					move_forward(60, 8);
					delay(5);
				}
			}
			else
			{
				move_forward(55, 8);
				if(detect_black_right() || detect_black_left())
				{
					move_forward(55, 8);
					turn(480, 25, -25);
					move_forward(55, 8);
				}
				else
				{
					move_forward(85, 10);
					if(r1 >= 88 && r2 >= 88)
					{
						move_forward(105, -10);
						turn(75, -10, 10);
					}
				}
			}
		}
		else
		{
			follow_line();
		}
		delay(5);
	}
}

/*
void rescue_room()
{
float last_left1 = 255;
float last_left2 = 255;

move_forward(300,30);

black_detected = false;
startTask(detect_black);

while(true)
{
bool approaching_wall =
(
distance_left < last_left1 &&
last_left1 < last_left2 &&
distance_left < 8
);

last_left2 = last_left1;
last_left1 = distance_left;

if(black_detected == true)
{
stop_motors();
playSound(soundBlip);
stopTask(detect_black);
break;
}
else if(distance_left <= 25 && distance_front < 25)
{
turn(250,30,-30);
}
else if(distance_left <= 25 && distance_front >= 25)
{
motor[motorA] = 20;
motor[motorB] = 20;
}
else if(distance_front < 10 && distance_left > 15)
{
move_forward(70,-8);
turn(480,35,-35);
}
else if(distance_left >= 25 && distance_front >= 25)
{
motor[motorA] = 20;
motor[motorB] = 20;
}
else if(approaching_wall)
{
turn(35,20,-20);
move_forward(80,10);
}
else if(distance_left > 35)
{
turn(480,-30,30);
move_forward(50,8);
clearTimer(T1);

repeatUntil(
black_detected == true ||
distance_front < 30 ||
time1[T1] > 2500)
{
motor[motorA] = 15;
motor[motorB] = 15;
}
}
else
{
motor[motorA] = 20;
motor[motorB] = 20;
}
}
}

*/