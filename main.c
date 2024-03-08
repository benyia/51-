#include <REGX52.H>

#define Init_138(n) P2=(P2&0x1f)|n<<5;

sbit S7 = P3^0;        
sbit S6 = P3^1;        
sbit S5 = P3^2;        
sbit S4 = P3^3;        

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;
	
void DelayK(unsigned char t)
{
        while(t--);
}

unsigned char stat_k=0;
void ScanKeys()
{
	if(S7 == 0)
	{
		DelayK(100);
		if(S7 == 0)
		{
			if(stat_k == 0)
			{
				L1 = 0;
				stat_k = 1;
			}
			else if(stat_k == 1)
			{
				L1 = 1;
				stat_k = 0;
			}
			while(S7 == 0);
		}
	}
	if(S6 == 0)
	{
		DelayK(100);
		if(S6 == 0)
		{
			if(stat_k == 0)
			{
				L2 = 0;
				stat_k = 2;
			}
			else if(stat_k == 2)
			{
				L2 = 1;
				stat_k = 0;
			}
			while(S6 == 0);
		}
	}
	if(S5 == 0)
	{
		DelayK(100);
		if(S5 == 0)
		{
			if(stat_k == 1)
			{
				L3 = 0;
				while(S5 == 0);
				L3 = 1;
			}
			else if(stat_k == 2)
			{
				L5 = 0;
				while(S5 == 0);
				L5 = 1;
			}
		}
	}
	if(S4 == 0)
	{
		DelayK(100);
		if(S4 == 0)
		{
			if(stat_k == 1)
			{
				L4 = 0;
				while(S4 == 0);
				L4 = 1;
			}
			else if(stat_k == 2)
			{
				L6 = 0;
				while(S4 == 0);
				L6 = 1;
			}
		}
	}
}
                          
void main()
{
	Init_138(4);
        while(1)
        {
			ScanKeys();
        }
}