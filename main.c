#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

uchar code SMG_duanma[18] = 
	{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
	 0x88,0x80,0xc6,0xc0,0x86,0x8e,
	 0xbf,0x7f};//分别是0-9（对应下标），A-F，“-”，“.”

sfr AUXR=0x8e;
	 
sbit L7=P0^6;
sbit L8=P0^7;

sbit S5=P3^2;
sbit S4=P3^3;
	 
uchar t_005s=0;
uchar t_s= 00;
uchar t_min=20;
uchar t_h=05;

uchar stat_led=0xff;
void SelectHC138(uchar channel)
{
	switch(channel)
	{
		case 4:
			P2=(P2&0x1F)|0x80;
		break;
		case 5:
			P2=(P2&0x1F)|0xa0;
		break;
		case 6:
			P2=(P2&0x1F)|0xc0;
		break;
		case 7:
			P2=(P2&0x1F)|0xe0;
		break;
		case 0:
			P2=(P2&0x1F)|0x00;
		break;
	}
}

void DisplaySMG_Bit(uchar pos,uchar value)
{
	P0=0xff;
	SelectHC138(6);
	P0=0x01<<pos;
	SelectHC138(7);
	P0=value;
}
//----------------------定时器---------------------------
void InitTimer0()
{
	TMOD=0x21;
	TH0=(65535-50000)/256;//50ms
	TL0=(65535-50000)%256;
	
	ET0=1;
	EA=1;
	TR0=0;
}

void ServiceTimer0() interrupt 1
{
	TH0=(65535-50000)/256;//50ms
	TL0=(65535-50000)%256;
	
	t_005s++;
	if(t_005s==20)//1s
	{
		t_005s=0;
		t_s++;
		
		if(t_s==60)
		{
			t_min++;
			t_s=0;
			if(t_min==60)
			{
				t_h++;
				t_min=0;
				if(t_h==24)
				{
					t_h=0;
				}
			}
		}
	}                                                                                                           
}

//----------------------数码管---------------------------
void Delay_SMG(uint t)
{
	while(t--);
	
}
void DisplaySMG()
{
	DisplaySMG_Bit(0,SMG_duanma[t_h/10]);
	Delay_SMG(500);
	DisplaySMG_Bit(1,SMG_duanma[t_h%10]);
	Delay_SMG(500);
	DisplaySMG_Bit(2,SMG_duanma[16]);
	Delay_SMG(500);
	
	DisplaySMG_Bit(3,SMG_duanma[t_min/10]);
	Delay_SMG(500);
	DisplaySMG_Bit(4,SMG_duanma[t_min%10]);
	Delay_SMG(500);
	DisplaySMG_Bit(5,SMG_duanma[16]);
	Delay_SMG(500);
	
	DisplaySMG_Bit(6,SMG_duanma[t_s/10]);
	Delay_SMG(500);
	DisplaySMG_Bit(7,SMG_duanma[t_s%10]);
	Delay_SMG(500);
	P0=0xFF;
}
void Delay_Check(uint t)
{
	while(t--);
	while(t--);
}
//----------------------系统检查---------------------------
void SysCheck()
{
	uchar i;
	SelectHC138(4);
	for(i=1;i<=8;i++)
	{
		P0=0xFF<<i;
		Delay_Check(60000);
	}
	for(i=1;i<=8;i++)
	{
		P0=~(0xFF<<i);
		Delay_Check(60000);
	}
	SelectHC138(7);
	P0=0x00;
	for(i=0;i<8;i++)
	{
		SelectHC138(6);
		P0=~(0xFE<<i);
		Delay_Check(60000);
	}
	for(i=0;i<8;i++)
	{
		SelectHC138(6);
		P0=0xFE<<i;
		Delay_Check(60000);
		Delay_Check(60000);
	}
	SelectHC138(0);
	TR0=1;
}
//----------------------本地控制---------------------------
void Delay_K(uint t)
{
	while(t--);
}

void Localcheck()
{

	if(S4==0)
	{
		DisplaySMG();
		if(S4==0)
		{
			while(S4==0)
			{
				DisplaySMG();
			}
			SelectHC138(4);
			stat_led=(stat_led|0x80)&(~stat_led|0x7f);
			P0=stat_led;
			SelectHC138(0);
		}

	}
	if(S5==0)
	{
		Delay_K(100);
		{
			if(S5==0)
			{
				while(S5==0)
				{
					DisplaySMG();
				}
				SelectHC138(4);
				stat_led=(stat_led|0x40)&(~stat_led|0xbf);
				P0=stat_led;
				SelectHC138(0);
			}
			
		}
	}
}
//----------------------串口通信--------------------------
uchar command=0;

void InitUart()
{
	TMOD = 0X21;
	TH1 = 0XFD;//设置9600波特率
	TL1 = 0XFD;
	TR1 = 1;//启动定时器T1
	
	SCON = 0X50; //8位UART模式，允许接收
	AUXR = 0X00;//辅助寄存器
	
	ES = 1;
	EA = 1;
	
}

void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		command = SBUF;
		RI = 0;
	}
}

void SendByteUart(uchar dat)
{
	SBUF=dat;
	while(TI==0);
	TI=0;
}

void UartWorking()
{
	if(command!=0)
	{
		SelectHC138(4);
		switch(command&0xF0)
		{
			case 0xA0:
				stat_led=(P0|0x0F)&(~command|0xF0);
				P0=stat_led;
				SelectHC138(0);
				command=0x00;
			break;
			case 0xB0:
			SendByteUart((t_h/10<<4)|(t_h%10));
			SendByteUart((t_min/10<<4)|(t_min%10));
			SendByteUart((t_s/10<<4)|(t_s%10));
			command=0;
			break;
		}
	}
}

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
	InitUart();
	SysCheck();
	
	while(1)
	{
		Localcheck();
		DisplaySMG();
		UartWorking();
	}
}
