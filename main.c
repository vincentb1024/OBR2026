typedef struct
{
	byte red;
	byte green;
	byte blue;
	short hue;
	byte value;
}targetc;
typedef struct
{
	byte red;
	byte green;
	byte blue;
	short hue;
	targetc twhite;
	targetc tblack;
	targetc tsilver;
	targetc tgreen;
	targetc tred;
} csensor;
csensor cs1, cs2;
float distance_front, distance_left;
bool black_detected = false;
int cs1.twhite.value = 114;
int cs1.tblack.value = 11;
int cs2.twhite.value = 109;
int cs2.tblack.value = 14;

float kp = 0.8;
float kd = 0.3;
float last_error;
int i1, i2, error;

int cs1.tgreen.red = 13;
int cs1.tgreen.green = 30;
int cs1.tgreen.hue = 115;

int cs2.tred.green   = 11;
int cs2.tgreen.green = 40;
int cs2.tgreen.hue = 121;

int cs1.tred.red = 75;
int cs1.tred.green = 12;
int cs1.tred.hue = 11;

int cs2.tred.red = 98;
int cs2.tred.green = 11;
int cs2.tred.hue = 5;

int margin = 10;
int margin_hue = 15;
void stop_motors()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
}
time delta;
int rot;
void checkmstatus(motor cmot)
{
	if(getMotorEncoder(cmot)!=rot)
	{
		rot = getMotorEncoder(cmot);
		delta=0;
	}
	if(delta>1000)
	{
		stuck();
	}
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
	i1 = 100 * (cs1.red - cs1.tblack.value) / (cs1.twhite.value - cs1.tblack.value);
	i2 = 100 * (cs2.red - cs2.tblack.value) / (cs2.twhite.value - cs2.tblack.value);

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
	return (cs1.red >= cs1.tgreen.red - margin && cs1.red <= cs1.tgreen.red + margin &&
	cs1.green >= cs1.tgreen.green - margin && cs1.green <= cs1.tgreen.green + margin &&
	cs1.hue >= cs1.tgreen.hue - margin_hue && cs1.hue <= cs1.tgreen.hue + margin_hue);
}

bool detect_green_left()
{
	return (cs2.red >= cs2.tgreen.red - margin && cs2.red <= cs2.tgreen.red + margin &&
	cs2.green >= cs2.tgreen.green - margin && cs2.green <= cs2.tgreen.green + margin &&
	cs2.hue >= cs2.tgreen.hue - margin_hue && cs2.hue <= cs2.tgreen.hue + margin_hue);
}

bool detect_silver_right()
{
	return ((cs2.blue + cs1.green + cs1.blue) / 3) >= 110;
}

bool detect_silver_left()
{
	return ((cs2.red + cs2.green + cs2.blue) / 3) >= 110;
}

bool detect_red_right()
{
	return (cs1.red >= cs1.tred.red - margin && cs1.red <= cs1.tred.red + margin &&
	cs1.green >= cs1.tred.green - margin && cs1.green <= cs1.tred.green + margin &&
	cs1.hue >= cs1.tred.hue - margin_hue && cs1.hue <= cs1.tred.hue + margin_hue);
}

bool detect_red_left()
{
	return (cs2.red >= cs2.tred.red - margin && cs2.red <= cs2.tred.red + margin &&
	cs2.green >= cs2.tred.green - margin && cs2.green <= cs2.tred.green + margin &&
	cs2.hue >= cs2.tred.hue - margin_hue && cs2.hue <= cs2.tred.hue + margin_hue);
}

bool detect_black_right()
{
	return (cs1.red <= cs1.tblack.value + margin);
}

bool detect_black_left()
{
	return (cs2.red <= cs2.tblack.value + margin);
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
		getColorRGB(S1, cs1.blue, cs1.green, cs1.blue);
		getColorRGB(S2, cs2.red, cs2.green, cs2.blue);
		cs1.hue = getColorHue(S1);
		cs2.hue = getColorHue(S2);
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
					if(cs1.red >= 88 && cs2.red >= 88)
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
					if(cs1.red >= 88 && cs2.red >= 88)
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