#include <reg52.h>

typedef unsigned int uint;
typedef unsigned char uchar;
//定义月份
uint yue = 1;
uchar code SMG_duanma[18] = 
	{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
	 0x88,0x80,0xc6,0xc0,0x86,0x8e,
	 0xbf,0x7f};//分别是0-9（对应下标），A-F，“-”，“.”


void Delay_SMG(uint n)
{
	while(n--);
}



//配置HC138
void SelectHC138(uchar channel)
{
	switch(channel)
	{
		case 4:
			P2 = (P2 & 0X1F) | 0X80;
		break;
		case 5:
			P2 = (P2 & 0X1F) | 0Xa0;
		break;
		case 6:
			P2 = (P2 & 0X1F) | 0Xc0;
		break;
		case 7:
			P2 = (P2 & 0X1F) | 0Xe0;
		break;
	}
}

//选择位码并显示
void DisplaySMG_Bit(uchar pos, uchar dat)
{
	SelectHC138(6);
	P0 = 0X01 << pos;
	SelectHC138(7);
	P0 = SMG_duanma[dat];
}

void Display_Dymaic()
{
	//uint i;
	DisplaySMG_Bit(0,2);
	Delay_SMG(500);
	DisplaySMG_Bit(1,0);
	Delay_SMG(500);
	DisplaySMG_Bit(2,2);
	Delay_SMG(500);
	DisplaySMG_Bit(3,2);
  Delay_SMG(500);
	DisplaySMG_Bit(4,16);
	Delay_SMG(500);
	DisplaySMG_Bit(5,16);
  Delay_SMG(500);
	
	DisplaySMG_Bit(6,yue / 10);
  Delay_SMG(500);
	DisplaySMG_Bit(7,yue % 10);
  Delay_SMG(500);
}

//初始化系统
void Initsys()
{
	SelectHC138(5);
	P0 = 0X00;//关闭蜂鸣器和继电器
	SelectHC138(4);
	P0 = 0XFF;//关闭LED
}

void Delay_yue(uint n)
{
	while(n--)
	{
		Display_Dymaic();
	}
}
void main()
{
	while(1)
	{
		Initsys();
		//DisplaySMG_Bit(2,6);
		Display_Dymaic();
		yue++;
		if(yue > 12)
		{
			yue = 1;
		}
		Delay_yue(200);
	}
}
