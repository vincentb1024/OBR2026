//calibration

int r1,g1,b1,r2,g2,b2;
int hue1, hue2;
int maxr1=0,minr1=255,maxg1=0,ming1=255,maxb1=0,minb1=255,maxhue1=0,minhue1=255;
int maxr2=0,minr2=255,maxg2=0,ming2=255,maxb2=0,minb2=255,maxhue2=0,minhue2=255;
int distance, distancel;

task main()
{
	while(true)
	{
		if(getButtonPress(buttonAny) == 1)
	{
		int maxr1=0,minr1=255,maxg1=0,ming1=255,maxb1=0,minb1=255,maxhue1=0,minhue1=255;
		int maxr2=0,minr2=255,maxg2=0,ming2=255,maxb2=0,minb2=255,maxhue2=0,minhue2=255;
	}
		getColorRGB(S1,r1,g1,b1);
		getColorRGB(S2,r2,g2,b2);
		hue1 = getColorHue(S1);
		hue2 = getColorHue(S2);
		//TBA Ignore weird values
		if(r1<minr1)minr1=r1;	if(r2<minr2)minr2=r2;
		if(r1>maxr1)maxr1=r1;	if(r2>maxr2)maxr2=r2;
		if(g1<ming1)ming1=g1;	if(g2<ming2)ming2=g2;
		if(g1>maxg1)maxg1=g1;	if(g2>maxg2)maxg2=g2;
		if(b1<minb1)minb1=b1;	if(b2<minb2)minb2=b2;
		if(b1>maxb1)maxb1=b1;	if(b2>maxb2)maxb2=b2;
		if(hue1>maxhue1)maxhue1=hue1;	if(hue2>maxhue2)maxhue2=hue2;
		if(hue1<minhue1)minhue1=hue1;	if(hue2<minhue2)minhue2=hue2;
		displayTextLine(1,"SENSOR 1");
		displayTextLine(3,"	Red: %d~%d",minr1,maxr1);
		displayTextLine(5,"Green: %d~%d",ming1,maxg1);
		displayTextLine(7,"Blue: %d~%d",minb1,maxb1);
		displayTextLine(9,"Hue: %d+-%d",maxhue1-minhue1,((maxhue1+minhue1)/2));
		displayTextLine(11,"SENSOR 2");
		displayTextLine(13,"Red: %d~%d",minr2,maxr2);
		displayTextLine(15,"Green: %d~%d",ming2,maxg2);
		displayTextLine(17,"Blue: %d~%d",minb2,maxb2);
		displayTextLine(19,"Hue: %d+-%d",((maxhue2+minhue2)/2),((maxhue2-minhue2)/2));
		displayTextLine(21,"Distance: %d",distance);
		displayTextLine(23,"Distancel: %d",distancel);
	}
}
