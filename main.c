#include <reg52.h>

typedef unsigned int uint;
typedef unsigned char uchar;

sbit L1 = P0^0;
sbit S7 = P3^0;

//配置HC138
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

//*********定时相关函数*********
uchar count = 0;
uchar pwm_duty = 0;
void InitTimer0()
{
	TMOD = 0x01;
	TH0 = (65535 - 100) / 256;
	TL0 = (65535 - 100) % 256;
	
	ET0 = 1;
	EA = 1;
}

void ServiceTimer0() interrupt 1 
{
	TH0 = (65535 - 100) / 256;
	TL0 = (65535 - 100) % 256;
	
	count++;
	if(count == pwm_duty)
	{
		L1 = 1;
	}
	else if(count == 100)
	{
		L1 = 0;
		count = 0;
	}
}
//*******按键相关函数********
uchar stat = 0;
void Delay(uint t)
{
	while(t--);
}


void ScanKeys()
{
	if(S7 == 0)
	{
		Delay(100);
		if(S7 == 0)
		{
			switch(stat)
			{
				case 0:
					L1 = 0;
					TR0 = 1;  //打开寄存器
					pwm_duty = 10;
					stat = 1;
				break;
				case 1:
					pwm_duty = 50;
					stat = 2;
				break;
				case 2:
					pwm_duty = 90;
					stat = 3;
				break;
				case 3:
					L1 = 1;
					TR0 = 0;
				  stat = 0;
				break;
			}
			while(S7 == 0);
		}
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
	InitTimer0();
	Initsys();
	SelectHC138(4);
	while(1)
	{
		ScanKeys();
	}
}


