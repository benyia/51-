#include <REGX52.H>


sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}
void LEDRuning()
{
	unsigned char i;
	HC138_A=1;
	HC138_B=0;
	HC138_A=0;
	for (i=0;i<4;i++)
	{
		P0=0x00;
		Delay(6000);
		Delay(6000);
		P0=0xff;
		Delay(6000);
		Delay(6000);
	}
	for (i=0;i<9;i++)
	{
		P0=0xff<<i;
		Delay(6000);
		Delay(6000);
	}
	for (i=0;i<9;i++)
	{
		P0=~(0xff<<i);
		Delay(6000);
		Delay(6000);
	}
}

void main()
{
	while(1)
	{
		LEDRuning();
	}
}
