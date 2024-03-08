#include "reg52.h"
//引入51头文件

typedef unsigned int uint;
typedef unsigned char uchar;
//重命名

sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
//sbit定义特殊功能寄存器的位变量
//定义好P25\P26\P27三个地址


//定义延时函数
void Delays(uint t)
{
	while(t--);
	while(t--);
}

void InitHC138(uint n)
{
	switch(n)
	{
		case 4://LED
			P2=(P2 & 0x1f) | 0x80;
		break;
		case 5://蜂鸣器
			P2=(P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2=(P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2=(P2 & 0x1f) | 0xe0;
		break;
	}
}

void OutPutP0(uint n, uchar dat)
{
	InitHC138(n);
	P0 = dat;
}

void LEDRuning()
{
	uint i;
	

	for(i = 0; i < 3; i++)
	{
		OutPutP0(4,0x00);
		Delays(60000);
		Delays(60000);
		OutPutP0(4,0xff);
		Delays(60000);
		Delays(60000);
	}
	
	for(i = 1; i <= 8; i++)
	{
		P0 = 0XFF << i;
		Delays(60000);
		Delays(60000);
	}
	
	OutPutP0(5,0x10);
	Delays(60000);
	Delays(60000);
	OutPutP0(5,0x00);
	
	
	for(i = 1; i <= 8; i++)
	{
		OutPutP0(4,~(0XFF << i));
		Delays(60000);
		Delays(60000);
	}
	OutPutP0(5,0x40);
	Delays(60000);
	Delays(60000);
	OutPutP0(5,0x00);
}

void Initsys()
{
	OutPutP0(5,0x00);
}

void main()
{
		while(1)
		{
			Initsys();
			LEDRuning();
		}
}
