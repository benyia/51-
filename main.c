#include <REGX52.H>

typedef unsigned int uint;
typedef unsigned char uchar;

uchar code SMG_duanma[18]={
	0xc0,0xf9,0xa4,0xb0,0x99,//0-4
	0x92,0x82,0xf8,0x80,0x90,//5-9
	0x88,0x80,0xc6,0xc0,0x86,0x8e,//A-F
	0xbf,0x7f  //“-”，“.”
	};

//配置HC138
void InitHC138(uchar n)
{
	switch(n)
	{
		case 4:
			P2 = (P2 & 0X1F) | 0X80;//LED
		break;
		case 5:
			P2 = (P2 & 0X1F) | 0Xa0;//蜂鸣器
		break;
		case 6:
			P2 = (P2 & 0X1F) | 0Xc0;//数码管位码
		break;
		case 7:
			P2 = (P2 & 0X1F) | 0Xe0;//数码管断码
		break;
	}
}
void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}
void ShowSMG_Bit(unsigned pos, uchar dat)
{
	InitHC138(6); //数码管的位置
	P0=0x01<<pos;
	InitHC138(7);//数码管的内容
	P0 = SMG_duanma[dat];
	Delay(6000);
	Delay(6000);
	//P0=0xff;//消影
}
void SMG_Static()
{
	uchar i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<18;j++)
		{
			ShowSMG_Bit(i,j);
			Delay(6000);
		}
	}
	for(j=0;j<18;j++)
	{
		InitHC138(6);
		P0=0xff;
		InitHC138(7);
		P0=SMG_duanma[j];
		Delay(6000);
	}
}
void Initsys()
{
	InitHC138(5);
	P0 = 0X00;
	InitHC138(4);
	P0 = 0XFF;
}
void main()
{
	Initsys();
	while(1)
	{
		SMG_Static();
	}
}
