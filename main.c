// Global variables and calibration thresholds
int r1, g1, b1, r2, g2, b2, hue1, hue2;
float distance_front, distance_left;

int white1 = 98;
int black1 = 12;
int white2 = 96;
int black2 = 8;

float kp = 0.8;
float kd = 0.3;
float last_error;
int i1, i2, error;

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

int margin = 10;
int margin_hue = 15;

// Base movement functions
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

// Line following logic
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

// Sensor detection functions
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

// Obstacle avoidance logic
void avoid_obstacle()
{
    stop_motors();
    delay(30);
    move_forward(70, -8);
    turn(480, 40, -40);
    delay(30);
    
    while(distance_left < 20)
    {
        motor[motorA] = 15;
        motor[motorB] = 15;
        delay(5);
    }
    move_forward(180, 10);
    delay(30);
    turn(480, -40, 40);
    
    while(distance_left > 20)
    {
        motor[motorA] = 15;
        motor[motorB] = 15;
        delay(5);
    }
    while(distance_left < 20)
    {
        motor[motorA] = 15;
        motor[motorB] = 15;
        delay(5);
    }
    move_forward(180, 10);
    turn(480, -40, 40);
    move_forward(150, 10);
    
    while((detect_black_left() || detect_black_right()) == false)
    {
        motor[motorA] = 10;
        motor[motorB] = 10;
    }
    move_forward(80, 10);
    turn(480, 40, -40);
}

// Wall following steering algorithm
void follow_wall_left()
{
    int target = 16;
    int error_wall = target - distance_left;
    int correction = error_wall * 2; 

    if(correction > 15) correction = 15;
    if(correction < -15) correction = -15;

    int base_speed = 18; 

    motor[motorA] = base_speed + correction;
    motor[motorB] = base_speed - correction;
}

// Verification function to confirm rescue room exit door
bool check_exit()
{
    stop_motors();
    delay(50);

    // Move forward a bit to align with the gap opener
    move_forward(90, 12);
    delay(50);

    // Double check if it is still a large open area
    if(distance_left > 35)
    {
        stop_motors();
        delay(50);

        // Turn 90 degrees left entering the exit path
        turn(480, -25, 25);
        delay(50);

        clearTimer(T2);

        // Scan straight ahead for the black line for up to 3 seconds
        repeatUntil(detect_black_left() || detect_black_right() || time1[T2] > 8000)
        {
            if(distance_front < 7) 
            { 
                stop_motors(); 
                break; 
            }

            motor[motorA] = 15;
            motor[motorB] = 15;
            delay(5);
        }

        stop_motors();
        delay(50);

        // Success: Black line detected!
        if(detect_black_left() || detect_black_right())
        {
            move_forward(100, 15); // Fully exit the room
            return true;
        }
        // False Positive: Reset positions and return to the main wall tracking
        else
        {
            move_forward(90, -12); // Back up
            delay(50);
            turn(480, 25, -25);    // Turn back to face the corridor
            delay(50);
            return false;
        }
    }
    return false;
}

// Main Rescue Room sequence (Room 3)
void rescue_room()
{
    stop_motors();
    delay(100);

    // Move forward enough to clear the silver tape completely
    move_forward(280, 15);
    delay(100);

    // Check initial space. If wide open, rotate left to locate the wall
    if(distance_left > 35)
    {
        turn(480, -25, 25);
        move_forward(250, 15);
    }

    delay(100);
    
    clearTimer(T1);     // Global room timer
    long lock_exit = 0; // Prevent consecutive false readings from causing infinite turns

    repeatUntil(detect_black_left() || detect_black_right())
    {
        // Front wall crash prevention (Corners/Dead ends)
        if(distance_front < 11)
        {
            stop_motors();
            delay(50);
            turn(480, 25, -25); // Turn 90 degrees right to escape corner
            delay(50);
            lock_exit = time1[T1] + 1500; // Impose cooldown on exit reading right after turn
        }
        // Check for exit paths after initially exploring for 2 seconds
        else if(time1[T1] > 2000 && time1[T1] > lock_exit)
        {
            if(distance_left > 35)
            {
                if(check_exit())
                {
                    break; // Successfully broke out of the rescue room loop
                }
                else
                {
                    lock_exit = time1[T1] + 2500; // Lock exit checks for 2.5s to get away from the gap
                }
            }
            else
            {
                follow_wall_left();
            }
        }
        else
        {
            follow_wall_left();
        }

        delay(5);
    }

    stop_motors();
    delay(50);
    move_forward(150, 15); // Extra push to guarantee alignment back onto the external line track
}

// Background thread for non-blocking hardware sensor management
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
        delay(12);
    }
}

// Main execution process loop
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
                break;
            }
        }
        else if(detect_silver_left() || detect_silver_right())
        {
            rescue_room();
        }
        else if(distance_front <= 5)
        {
            avoid_obstacle();
        }
        else if(detect_green_left())
        {
            stop_motors();
            delay(10);
            move_forward(10, 5);
            if (detect_green_right())
            {
                move_forward(55, 8);
                if(detect_black_right() || detect_black_left())
                {
                    turn(960, 25, -25);
                    move_forward(25, 10);
                }
            }
            else
            {
                stop_motors();
                move_forward(55, 8);
                if(detect_black_left())
                {
                    move_forward(55, 8);
                    turn(480, -25, 25);
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
            move_forward(10, 5);
            delay(10);
            if (detect_green_left())
            {
                move_forward(55, 8);
                if(detect_black_right() || detect_black_left())
                {
                    turn(960, 25, -25);
                    move_forward(25, 10);
                }
            }
            else
            {
                stop_motors();
                move_forward(55, 8);
                if(detect_black_right())
                {
                    move_forward(55, 8);
                    turn(480, 25, -25);
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
    }
}


