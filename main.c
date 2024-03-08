#include "reg52.h"
#include "ds1302.h"

typedef unsigned int uint;
typedef unsigned char uchar;

uchar Write_DS1302_adrr[7] = {0X80, 0X82, 0X84, 0X86, 0X88, 0X8A, 0X8C};
uchar Read_DS1302_adrr[7] = {0X81, 0X83, 0X85, 0X87, 0X89, 0X8B, 0X8D};
uchar Timer[7] = {0X00, 0X00, 0X00, 0X18, 0X04, 0X06, 0X20};

uchar code SMG_duanma[18] = 
	{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
	 0x88,0x80,0xc6,0xc0,0x86,0x8e,
	 0xbf,0x7f};//分别是0-9（对应下标），A-F，“-”，“.”

void DS1302_Config()
{
	char i;
	
	Write_Ds1302_Byte(0X8E, 0X00);  //打开保护
	for(i = 0; i < 7; i++)
	{
		Write_Ds1302_Byte(Write_DS1302_adrr[i], Timer[i]);
	}

}

void Read_DS1302_Timer()
{
	uchar i;
	for(i = 0; i < 7; i++)
	{
		Timer[i] = Read_Ds1302_Byte(Read_DS1302_adrr[i]);
	}
}

//配置HC138
void SelectHC138(uchar channel,uchar dat)
{
	P0=dat;
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
		case 0:    //锁住所有寄存器
			P2 = (P2 & 0X1F) | 0X00;
		break;
	}
	P2 = (P2 & 0X1F) | 0X00;
}

void Initsys()
{
	SelectHC138(5,0X00);
	SelectHC138(4,0xff);
	SelectHC138(0,0x00);
	
}



void Delay_SMG(uint t)
{
	while(t--);
}

//在pos位码上，显示value段码
void DisplaySMG_Bit(uchar pos, uchar value)
{
	SelectHC138(6,0x01<<pos);
	SelectHC138(7,value);
	Delay_SMG(500);                //数码管点亮时长
	SelectHC138(6, 0x01 << pos);        
	SelectHC138(7, 0xff);            //消隐
}

void Display_DS1302()
{
	DisplaySMG_Bit(0, SMG_duanma[Timer[2]/16]);
	DisplaySMG_Bit(1, SMG_duanma[Timer[2]%16]);
	
	DisplaySMG_Bit(2, SMG_duanma[16]);
	
	DisplaySMG_Bit(3, SMG_duanma[Timer[1]/16]);
	DisplaySMG_Bit(4, SMG_duanma[Timer[1]%16]);
	
	DisplaySMG_Bit(5, SMG_duanma[16]);
	
	DisplaySMG_Bit(6, SMG_duanma[Timer[0]/16]);
	DisplaySMG_Bit(7, SMG_duanma[Timer[0]%16]);
}

void main()
{
	Initsys();
	DS1302_Config();
	while(1)
	{
		Read_DS1302_Timer();
		Display_DS1302();
	}
}
