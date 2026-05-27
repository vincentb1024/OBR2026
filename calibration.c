// Calibration Code

int r1, g1, b1, r2, g2, b2;
int hue1, hue2;
int max_r1 = 0, min_r1 = 255, max_g1 = 0, min_g1 = 255, max_b1 = 0, min_b1 = 255, max_hue1 = 0, min_hue1 = 255;
int max_r2 = 0, min_r2 = 255, max_g2 = 0, min_g2 = 255, max_b2 = 0, min_b2 = 255, max_hue2 = 0, min_hue2 = 255;
int distance_front, distance_left;

task main()
{
    while(true)
    {
        // Reset Button Debounce: Triggers only when pressed, 
        // and holds execution until the user RELEASES the button.
        if(getButtonPress(7) == 1) // Left arrow button
        {
            max_r1 = 0; min_r1 = 255; max_g1 = 0; min_g1 = 255; max_b1 = 0; min_b1 = 255; max_hue1 = 0; min_hue1 = 255;
            max_r2 = 0; min_r2 = 255; max_g2 = 0; min_g2 = 255; max_b2 = 0; min_b2 = 255; max_hue2 = 0; min_hue2 = 255;
            
            while(getButtonPress(7) == 1)
            {
                delay(10); // Wait for physical button release
            }
        }

        // Fetch current hardware sensor data
        getColorRGB(S1, r1, g1, b1);
        getColorRGB(S2, r2, g2, b2);
        hue1 = getColorHue(S1);
        hue2 = getColorHue(S2);
        distance_front = getUSDistance(S3);
        distance_left = getUSDistance(S4);

        // Update threshold extremes for Sensor 1
        if(r1 < min_r1) min_r1 = r1;   if(r1 > max_r1) max_r1 = r1;
        if(g1 < min_g1) min_g1 = g1;   if(g1 > max_g1) max_g1 = g1;
        if(b1 < min_b1) min_b1 = b1;   if(b1 > max_b1) max_b1 = b1;
        if(hue1 < min_hue1) min_hue1 = hue1; if(hue1 > max_hue1) max_hue1 = hue1;

        // Update threshold extremes for Sensor 2
        if(r2 < min_r2) min_r2 = r2;   if(r2 > max_r2) max_r2 = r2;
        if(g2 < min_g2) min_g2 = g2;   if(g2 > max_g2) max_g2 = g2;
        if(b2 < min_b2) min_b2 = b2;   if(b2 > max_b2) max_b2 = b2;
        if(hue2 < min_hue2) min_hue2 = hue2; if(hue2 > max_hue2) max_hue2 = hue2;

        // Render calibration profiles onto the EV3/NXT LCD Screen
        displayTextLine(0, "SENSOR 1 (RIGHT)");
        displayTextLine(1, "Red:   %d ~ %d", min_r1, max_r1);
        displayTextLine(2, "Green: %d ~ %d", min_g1, max_g1);
        displayTextLine(3, "Blue:  %d ~ %d", min_b1, max_b1);
        // FIXED MATH: (Max+Min)/2 computes the center, and (Max-Min)/2 computes the target range margin
        displayTextLine(4, "Hue:   %d +- %d", ((max_hue1 + min_hue1) / 2), ((max_hue1 - min_hue1) / 2));
        
        displayTextLine(6, "SENSOR 2 (LEFT)");
        displayTextLine(7, "Red:   %d ~ %d", min_r2, max_r2);
        displayTextLine(8, "Green: %d ~ %d", min_g2, max_g2);
        displayTextLine(9, "Blue:  %d ~ %d", min_b2, max_b2);
        displayTextLine(10, "Hue:  %d +- %d", ((max_hue2 + min_hue2) / 2), ((max_hue2 - min_hue2) / 2));
        
        displayTextLine(12, "Dist. Front: %d cm", distance_front);
        displayTextLine(13, "Dist. Left:  %d cm", distance_left);
        
        delay(30); // Prevent display flickering and optimize CPU utilization
    }
}

