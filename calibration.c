//calibration

int r1,g1,b1,r2,g2,b2;
int hue1, hue2;
int maxr1=0,minr1=255,maxg1=0,ming1=255,maxb1=0,minb1=255,maxhue1=0,minhue1=255;
int maxr2=0,minr2=255,maxg2=0,ming2=255,maxb2=0,minb2=255,maxhue2=0,minhue2=255;


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
		if(r1<minr1)minr1=r1;	if(r2<minr2)minr2=r2;
		if(r1>maxr1)maxr1=r1;	if(r2>maxr1)maxr2=r2;
		if(g1<ming1)ming1=g1;	if(g2<ming2)ming2=g2;
		if(g1>maxg1)maxg1=g1;	if(g2>maxg2)maxg2=g2;
		if(b1<minb1)minb1=b1;	if(b2<minb2)minb2=b2;
		if(b1>maxb1)maxb1=b1;	if(b2>maxb2)maxb2=b2;
		if(hue1>maxhue1)maxhue1=hue1;	if(hue2>maxhue2)maxhue2=hue2;
		if(hue1<minhue1)minhue1=hue1;	if(hue2<minhue2)minhue2=hue2;
		displayTextLine(1,"SENSOR 1");
		displayTextLine(3,"red: %d~%d",minr1,maxr1);
		//WIP
	}
}
