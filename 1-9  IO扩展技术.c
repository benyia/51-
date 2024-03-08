#include <reg52.h>

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void SelectHC138(unsigned char channel)
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


void LEDRuning()
{
	SelectHC138(4);
	P0=0xF0;
	Delay(60000);
	Delay(60000);
	P0=0x0F;
	Delay(60000);
	Delay(60000);
	P0=0xFF;
	Delay(60000);
	Delay(60000);
}

void SMGRuning()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SelectHC138(6);
		P0=0x01<<i;
		SelectHC138(7);
		P0=0x00;
		Delay(60000);
		Delay(60000);
	}
	P0=0xFF;
	Delay(60000);
	Delay(60000);
}

void Initsys()
{
	SelectHC138(5);
	P0=0x00;
	SelectHC138(4);
	P0=0xFF;
}

void main()
{
	Initsys();
	while(1)
	{
		LEDRuning();
		SMGRuning();
	}
}
