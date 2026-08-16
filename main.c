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

// Margens específicas evitam que uma calibração mais ampla de uma cor
// diminua a precisão das outras.
int r1_green_margin = 10, g1_green_margin = 10, hue1_green_margin = 15;
int r2_green_margin = 10, g2_green_margin = 10, hue2_green_margin = 15;
int r1_red_margin = 10, g1_red_margin = 10, hue1_red_margin = 15;
int r2_red_margin = 10, g2_red_margin = 10, hue2_red_margin = 15;
int silver_threshold1 = 110;
int silver_threshold2 = 110;

#define CAL_WHITE  0
#define CAL_BLACK  1
#define CAL_GREEN  2
#define CAL_RED    3
#define CAL_SILVER 4

int cal_min_r1, cal_max_r1, cal_min_g1, cal_max_g1, cal_min_b1, cal_max_b1, cal_min_hue1, cal_max_hue1;
int cal_min_r2, cal_max_r2, cal_min_g2, cal_max_g2, cal_min_b2, cal_max_b2, cal_min_hue2, cal_max_hue2;
int cal_min_brightness1, cal_min_brightness2;

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

bool within_range(int value, int center, int range)
{
	return (value >= center - range && value <= center + range);
}

int calibration_center(int minimum, int maximum)
{
	return (minimum + maximum) / 2;
}

int calibration_margin(int minimum, int maximum, int safety_margin)
{
	int result = ((maximum - minimum) / 2) + safety_margin;
	if(result < safety_margin) result = safety_margin;
	return result;
}

void reset_calibration_extremes()
{
	cal_min_r1 = 255; cal_max_r1 = 0; cal_min_g1 = 255; cal_max_g1 = 0; cal_min_b1 = 255; cal_max_b1 = 0; cal_min_hue1 = 255; cal_max_hue1 = 0;
	cal_min_r2 = 255; cal_max_r2 = 0; cal_min_g2 = 255; cal_max_g2 = 0; cal_min_b2 = 255; cal_max_b2 = 0; cal_min_hue2 = 255; cal_max_hue2 = 0;
	cal_min_brightness1 = 255;
	cal_min_brightness2 = 255;
}

void save_calibration_sample()
{
	int brightness1 = (r1 + g1 + b1) / 3;
	int brightness2 = (r2 + g2 + b2) / 3;

	if(r1 < cal_min_r1) cal_min_r1 = r1; if(r1 > cal_max_r1) cal_max_r1 = r1;
	if(g1 < cal_min_g1) cal_min_g1 = g1; if(g1 > cal_max_g1) cal_max_g1 = g1;
	if(b1 < cal_min_b1) cal_min_b1 = b1; if(b1 > cal_max_b1) cal_max_b1 = b1;
	if(hue1 < cal_min_hue1) cal_min_hue1 = hue1; if(hue1 > cal_max_hue1) cal_max_hue1 = hue1;
	if(brightness1 < cal_min_brightness1) cal_min_brightness1 = brightness1;

	if(r2 < cal_min_r2) cal_min_r2 = r2; if(r2 > cal_max_r2) cal_max_r2 = r2;
	if(g2 < cal_min_g2) cal_min_g2 = g2; if(g2 > cal_max_g2) cal_max_g2 = g2;
	if(b2 < cal_min_b2) cal_min_b2 = b2; if(b2 > cal_max_b2) cal_max_b2 = b2;
	if(hue2 < cal_min_hue2) cal_min_hue2 = hue2; if(hue2 > cal_max_hue2) cal_max_hue2 = hue2;
	if(brightness2 < cal_min_brightness2) cal_min_brightness2 = brightness2;
}

void apply_calibration(int color)
{
	if(color == CAL_WHITE)
	{
		white1 = calibration_center(cal_min_r1, cal_max_r1);
		white2 = calibration_center(cal_min_r2, cal_max_r2);
	}
	else if(color == CAL_BLACK)
	{
		black1 = calibration_center(cal_min_r1, cal_max_r1);
		black2 = calibration_center(cal_min_r2, cal_max_r2);
	}
	else if(color == CAL_GREEN)
	{
		r1_green = calibration_center(cal_min_r1, cal_max_r1);
		g1_green = calibration_center(cal_min_g1, cal_max_g1);
		hue1_green = calibration_center(cal_min_hue1, cal_max_hue1);
		r2_green = calibration_center(cal_min_r2, cal_max_r2);
		g2_green = calibration_center(cal_min_g2, cal_max_g2);
		hue2_green = calibration_center(cal_min_hue2, cal_max_hue2);
		r1_green_margin = calibration_margin(cal_min_r1, cal_max_r1, 4);
		g1_green_margin = calibration_margin(cal_min_g1, cal_max_g1, 4);
		hue1_green_margin = calibration_margin(cal_min_hue1, cal_max_hue1, 6);
		r2_green_margin = calibration_margin(cal_min_r2, cal_max_r2, 4);
		g2_green_margin = calibration_margin(cal_min_g2, cal_max_g2, 4);
		hue2_green_margin = calibration_margin(cal_min_hue2, cal_max_hue2, 6);
	}
	else if(color == CAL_RED)
	{
		r1_red = calibration_center(cal_min_r1, cal_max_r1);
		g1_red = calibration_center(cal_min_g1, cal_max_g1);
		hue1_red = calibration_center(cal_min_hue1, cal_max_hue1);
		r2_red = calibration_center(cal_min_r2, cal_max_r2);
		g2_red = calibration_center(cal_min_g2, cal_max_g2);
		hue2_red = calibration_center(cal_min_hue2, cal_max_hue2);
		r1_red_margin = calibration_margin(cal_min_r1, cal_max_r1, 4);
		g1_red_margin = calibration_margin(cal_min_g1, cal_max_g1, 4);
		hue1_red_margin = calibration_margin(cal_min_hue1, cal_max_hue1, 6);
		r2_red_margin = calibration_margin(cal_min_r2, cal_max_r2, 4);
		g2_red_margin = calibration_margin(cal_min_g2, cal_max_g2, 4);
		hue2_red_margin = calibration_margin(cal_min_hue2, cal_max_hue2, 6);
	}
	else if(color == CAL_SILVER)
	{
		silver_threshold1 = cal_min_brightness1 - 5;
		silver_threshold2 = cal_min_brightness2 - 5;
		if(silver_threshold1 < 0) silver_threshold1 = 0;
		if(silver_threshold2 < 0) silver_threshold2 = 0;
	}
}

void calibrate_color(int color, string name)
{
	reset_calibration_extremes();
	clearTimer(T3);
	while(time1[T3] < 15000)
	{
		save_calibration_sample();
		displayTextLine(0, "CALIBRACAO");
		displayTextLine(1, "%s", name);
		displayTextLine(3, "Mova o robo");
		displayTextLine(4, "Tempo: %d s", (15000 - time1[T3]) / 1000);
		delay(20);
	}
	apply_calibration(color);
	playSound(soundBeepBeep);
}

void run_calibration()
{
	calibrate_color(CAL_WHITE, "BRANCO");
	calibrate_color(CAL_BLACK, "PRETO");
	calibrate_color(CAL_GREEN, "VERDE");
	calibrate_color(CAL_RED, "VERMELHO");
	calibrate_color(CAL_SILVER, "PRATA");
	displayTextLine(0, "CALIBRACAO OK");
	displayTextLine(2, "Iniciando...");
	delay(1000);
}

bool calibration_requested()
{
	clearTimer(T4);
	while(time1[T4] < 3000)
	{
		displayTextLine(0, "ESQUERDA: CALIBRAR");
		displayTextLine(1, "Aguarde para iniciar");
		if(getButtonPress(7) == 1)
		{
			while(getButtonPress(7) == 1) delay(10);
			return true;
		}
		delay(10);
	}
	return false;
}

void follow_line()
{
	if(white1 == black1 || white2 == black2)
	{
		stop_motors();
		return;
	}

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
	return (within_range(r1, r1_green, r1_green_margin) &&
	within_range(g1, g1_green, g1_green_margin) &&
	within_range(hue1, hue1_green, hue1_green_margin));
}

bool detect_green_left()
{
	return (within_range(r2, r2_green, r2_green_margin) &&
	within_range(g2, g2_green, g2_green_margin) &&
	within_range(hue2, hue2_green, hue2_green_margin));
}

bool detect_silver_right()
{
	return ((r1 + g1 + b1) / 3) >= silver_threshold1;
}

bool detect_silver_left()
{
	return ((r2 + g2 + b2) / 3) >= silver_threshold2;
}

bool detect_red_right()
{
	return (within_range(r1, r1_red, r1_red_margin) &&
	within_range(g1, g1_red, g1_red_margin) &&
	within_range(hue1, hue1_red, hue1_red_margin));
}

bool detect_red_left()
{
	return (within_range(r2, r2_red, r2_red_margin) &&
	within_range(g2, g2_red, g2_red_margin) &&
	within_range(hue2, hue2_red, hue2_red_margin));
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
	if(calibration_requested())
	{
		run_calibration();
	}
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
