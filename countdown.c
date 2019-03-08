#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit beep=P1^5;

void timer1(void)  interrupt 3     //???1????????????(?1s)
{
TH1=(65536-2000)/256;    //???T1?8????
TL1=(65536-2000)%256;   //???T1?8????
beep=~beep;
t1++;
if(t1==500)
{  
t1=0;
   TR1=0;}}
char bb[]="0123456789";
void main()
{
   P0=bb[s/10];   //µ¹¼ÆÊ±
		P2=tablewe[2];
		delay(2);
		P0=0;
		P2=0XFF;
		P0=tabledu[s%10];
		P2=tablewe[3];
		delay(2);
		P2=0XFF;
		P0=0;
}