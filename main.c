#include <reg52.h>

sfr AUXR = 0x8e;

typedef unsigned int uint;
typedef unsigned char uchar;

uchar urdat;

void SelectHC138(uchar channel)
{
	switch(channel)
	{
		case 4:    //LED
			P2 = (P2 & 0X1F) | 0X80;
		break;
		case 5:    //蜂鸣器和继电器
			P2 = (P2 & 0X1F) | 0Xa0;
		break;
		case 6:    //位码
			P2 = (P2 & 0X1F) | 0Xc0;
		break;
		case 7:    //段码
			P2 = (P2 & 0X1F) | 0Xe0;
		break;
	}
}

void InitUart()
{
	TMOD = 0X20;
	TH1 = 0XFD;
	TL1 = 0XFD;
	TR1 = 1;
	
	SCON = 0X50;
	AUXR = 0X00;
	
	ES = 1;
	EA = 1;
}



void SendByte(uchar dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
		urdat = SBUF;
		SendByte(urdat );
	}
}

//初始化系统
void Initsys()
{
	SelectHC138(5);
	P0 = 0X00;//关闭蜂鸣器和继电器
	SelectHC138(4);
	P0 = 0XFF;//关闭LED
}

void main()
{
	InitUart();
	Initsys();
	SendByte(0xa5);      
	SendByte(0x5a);
	while(1);
}
