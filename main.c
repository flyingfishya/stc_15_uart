#include <STC15F2K60S2.H>
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L          //????
#define BAUD 115200             //?????

#define NONE_PARITY     0       //???
#define ODD_PARITY      1       //???
#define EVEN_PARITY     2       //???
#define MARK_PARITY     3       //????
#define SPACE_PARITY    4       //????

#define PARITYBIT NONE_PARITY   //?????
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

bit busy;
char str;

void SendData(BYTE dat);
void SendString(char *s);

void main()
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
    
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
//  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8??????
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9??????,??????1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9??????,??????0
#endif

    AUXR = 0x40;                //???1?1T??
    TMOD = 0x00;                //???1???0(16?????)
    TL1 = (65536 - (FOSC/4/BAUD));   //????????
    TH1 = (65536 - (FOSC/4/BAUD))>>8;
    TR1 = 1;                    //???1????
    ES = 1;                     //??????
    EA = 1;
    while(1)
		{
				
		}
}

void SendData(BYTE dat)
{
    while (busy);               //???????????
    ACC = dat;                  //?????P (PSW.0)
    if (P)                      //??P??????
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //??????0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //??????1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //??????1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //??????0
#endif
    }
    busy = 1;
    SBUF = ACC;                 //????UART?????
}

void SendString(char *s)
{
    while (*s)                  //?????????
    {
        SendData(*s++);         //??????
    }
}

void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //??RI?
				str = SBUF;
				SendString(&str);
    }
    if (TI)
    {
        TI = 0;                 //??TI?
        busy = 0;               //????
    }
}

