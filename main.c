#include "reg52.h"  

#define Init_138(n) P2=(P2&0x1f)|n<<5;

sbit HC138_A = P2^5;     
sbit HC138_B = P2^6; 
sbit HC138_C = P2^7;       

sbit S7 = P3^0;        
sbit S6 = P3^1;        
sbit S5 = P3^2;        
sbit S4 = P3^3;        

unsigned char code SMG_NoDot[18] = 
    {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
     0xbf,0x7f};
unsigned char code SMG_HaveDot[16]=
	{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,
	0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};
void Delay(unsigned char t)
{
        while(t--);
}



void ShowKeyNum(unsigned char value)
{
        Init_138(6);
        P0 = 0x01;
        Init_138(7);
        P0 = value;
}

void ScanKeys()
{
        if(S7 == 0)
        {
                Delay(200);
                if(S7 == 0)
                {
                        while(S7 == 0);
                        ShowKeyNum(SMG_HaveDot[1]);
                }
        }
        if(S6 == 0)
        {
                Delay(200);
                if(S6 == 0)
                {
                        while(S6 == 0);
                        ShowKeyNum(SMG_HaveDot[2]);
                }
        }
        if(S5 == 0)
        {
                Delay(200);
                if(S5 == 0)
                {
                        while(S5 == 0);
                        ShowKeyNum(SMG_HaveDot[3]);
                }
        }
        if(S4 == 0)
        {
                Delay(200);
                if(S4 == 0)
                {
                        while(S4 == 0);
                        ShowKeyNum(SMG_HaveDot[4]);
                }
        }
}
                          
void main()
{
        while(1)
        {
                ScanKeys();
        }
}
