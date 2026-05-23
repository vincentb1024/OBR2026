//calibration

int r1,g1,b1,r2,g2,b2;
int hue1, hue2;

task main()
{
	while(true)
	{
		getColorRGB(S1,r1,g1,b1);
		getColorRGB(S2,r2,g2,b2);

		hue1 = getColorHue(S1);
		hue2 = getColorHue(S2);

		displayTextLine(1, "Right: r1:%d g1:%d b1:%d", r1,g1,b1);
		displayTextLine(3, "Left: r2:%d g2:%d b2:%d", r2,g2,b2);
		displayTextLine(5, "Right: hue1:%d",hue1);
		displayTextLine(7, "Left: hue2:%d",hue2);
	}
}
