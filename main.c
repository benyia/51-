#include <reg52.H>
//#include <absacc.h>
#include <onewire.h>

//-------共阳数码管的段码编码表（无小数点）--------
unsigned char code SMG_NoDot[18]={0xc0,0xf9,//0-1
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,//2-9
    0x88,0x80,0xc6,0xc0,0x86,0x8e,//A-F
	0xbf,0x7f};//- .
//-------共阳数码管的段码编码表（带小数点）--------
//0.～9.
unsigned char code SMG_Dot[10]={0x40,0x79,
    0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

typedef unsigned char uchar;
typedef unsigned int uint;	

uint temp=0;
	
void SelectHC138(uchar channel,uchar dat)
{
	P0 = dat;                      //待设置数据
	switch(channel)
	{
		case 4:
			P2=(P2&0x1f)|0x80;
		break;
		case 5:
			P2=(P2&0x1f)|0xa0;
		break;
		case 6:
			P2=(P2&0x1f)|0xc0;
		break;
		case 7:
			P2=(P2&0x1f)|0xe0;
		break;
		case 0:
			P2=(P2&0x1f)|0x00;
		break;
	}
	P2 = (P2 & 0x1f) | 0x00;      //设置完成，关闭全部锁存器
}	

void Delay(uint t)
{
	while(t--);
}

void DisPlaySMG_Bit(uchar pos,uchar value)
{
	SelectHC138(6,0x01<<pos);
	SelectHC138(7,value);
	Delay(500);                //数码管点亮时长
	SelectHC138(6, 0x01 << pos);        
	SelectHC138(7, 0xff);            //消隐
}
void DisPlay_All(uchar dat)
{
	SelectHC138(6,0xff);
	SelectHC138(7,dat);
}
void DisPlaySMG_Temp()
{
	DisPlaySMG_Bit(7,SMG_NoDot[temp%10]);
	DisPlaySMG_Bit(6,SMG_Dot[(temp/10%10)]);
	DisPlaySMG_Bit(5,SMG_NoDot[temp/100]);
	
	DisPlaySMG_Bit(4,0xff);
	DisPlaySMG_Bit(3,0xff);
	DisPlaySMG_Bit(2,0xff);
	
	DisPlaySMG_Bit(1,0xff);
	DisPlaySMG_Bit(0,0xff);
	
	DisPlay_All(0xff);
	
}
void Delay_temp(uint t)
{
	while(t--)
	{
		DisPlaySMG_Temp();
	}
}
void Read_DS18B20_temp()
{
	uchar LSB,MSB;
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay_temp(200);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xBE);
	
	LSB=Read_DS18B20();
	MSB=Read_DS18B20();
//	init_ds18b20();
	
//	temp=MSB;
//	temp<<=8;
//	temp=temp|LSB;
	
	temp = MSB;                  //合成16位温度原始数据
	temp = ((temp << 8) | LSB);
	
	if((temp&0xf800)==0x0000)
	{
		temp>>=4;
		temp = temp*10;
		temp = temp+(LSB&0x0f)*0.625;
	}
	
}

void Initsys()
{
	SelectHC138(4,0xFF);
	SelectHC138(5,0x00);
	SelectHC138(0,0x00);
}
void main()
{
	Initsys();
	while(1)
	{
		Read_DS18B20_temp();
		DisPlaySMG_Temp();
	}
}
