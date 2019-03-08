#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
char s;
uchar num=0; //�����˱�ţ�����0����û������
char time=20;      //����ʱ��
char datitime=30;      
uint tt,t1;      
bit flag, s_flag=1,b_flag, fall_flag;     //flag��ʼ�����£�s_flagû��������1����b_flag��������fall_flag����Υ�� 
bit K_startcountflag, K_timecountflag;    //

sbit K0=P3^0;		//��ʼ��ť    //P3^0
sbit beep=P1^5;   	//������    //P1^5
sbit rled=P2^0;     //���������	//P2^0
sbit K1=P1^0;		//8����ť		//
sbit K2=P1^1;
sbit K3=P1^2;
sbit K4=P1^3;
sbit K5=P1^4;
sbit K6=P3^5;		//P2^5
sbit K7=P1^6;
sbit K8=P1^7;
sbit K_Time=P3^1;   		//�����ʱ��        //P3^1
sbit K_startcount=P3^2;  	//��ʼ����ʱ�������//p3^2
sbit K_timecount=P3^3;   	//�����ʱʱ�������//p3^3
void delay(uchar ms)
	//��ʱms
{
	uchar y;
	for(; ms>0;ms--)
	for(y=120;y>0;y--);
}

//�����-��
uchar code tabledu[ ]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71
};

//�����-λ��
uchar code tablewe[]={0x0C,0x08,0x04,0x00};   //{0x00,0x04,0x08,0x0C}
void reset_1();

//Timer0-��ʼ��20ms
void T0_Init(void)
{
	TMOD=0X01;    
	TH0=(65536-2000)/256;    
	TL0=(65536-2000)%256;  
	TH1=(65536-2000)/256;   
	TL1=(65536-2000)%256;   
	ET0=1;   
	ET1=1;  
	EA=1;  
	P0=0;
}

//�����˰���ɨ��
void Key_Scan( void)
{
	if(K0==0)
	{
		delay(10);//��������
		if(K0==0)
		{
			while(!K0);
			TR0=1;
			s=time;
			tt=0;
			flag=1;
			s_flag=1;
			b_flag=1;
			num=0;
			beep=1;
			rled=1;   
			fall_flag=0;   //???????
			K_startcountflag=0;
			K_timecountflag=0; 
		}
	}
}

//8·����ɨ��
void Scan(void)
{
	if(K1==0)
	{
		delay(10);
		if(K1==0)
		{
			while(!K1);
			num=1;
			TR0=0;
			TR1=1;
			s_flag=0;
		}
	}
	if(K2==0)
	{
		delay(10);
		if(K2==0)
		{
			while(!K2);
			num=2;
			TR0=0;
			TR1=1;
			s_flag=0;
		}
	}
if(K3==0)
{
delay(10);
if(K3==0)
{
while(!K3);
num=3;
TR0=0;
TR1=1;
s_flag=0;
}}
if(K4==0)
{
delay(10);
if(K4==0)
{
while(!K4);
num=4;
TR0=0;
TR1=1;
s_flag=0;
}}
if(K5==0)
{
delay(10);
if(K5==0)
{
while(!K5);
num=5;
TR0=0;
TR1=1;
s_flag=0;
}}
if(K6==0)
{
delay(10);
if(K6==0)
{
while(!K6);
num=6;
TR0=0;
TR1=1;
s_flag=0;
}}
if(K7==0)
{
delay(10);
if(K7==0)
{
while(!K7);
num=7;
TR0=0;
TR1=1;
s_flag=0;
}}
if(K8==0)
{
delay(10);
if(K8==0)
{
while(!K8);
num=8;
TR0=0;
TR1=1;
s_flag=0;
}    }}

//��ʾ
void display (void)
{
	if(flag==1)
	{	
		if(num!=0)    //���������������ʾ��Ӧ�ļ���
		{
			P0=tabledu[num];  
			P2=tablewe[0];
			delay(2);
			P0=0;
			P2=0XFF;
		}
		else//����û��������ǰ�治��ʾ����
		{
			P0=0;
			P2=0XFF;
		}
		
		P0=tabledu[s/10];   //����ʱ
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
else    //�����ʼ��û�а��£�����ʾF FF(����Υ���ߣ�����ʾΥ����뼰FF)��ʱ�����
{
if(fall_flag==1)    //Υ��
{
if(num!=0)
{
P0=tabledu[num];      //��ʾ����Υ��
P2=tablewe[0];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[15];    //������ʾFF����ʾΥ��
P2=tablewe[2];
delay(2);
P0=0;      ///����
P2=0XFF;
P0=tabledu[15];
P2=tablewe[3];
delay(2);
P0=0;
P2=0XFF;
}
else
{
P0=0;
P2=0XFF;
}}
else      //û����Υ�����ʾ����ʱ��
{
if(K_startcountflag==1)
{
P0=0X6D; //��һλ�������ʾ����5(s)����(��ʾ�����¼�����)
P2=tablewe[0];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[time/10]; //������ʾ����������ʱ��  
P2=tablewe[2];
delay(2);
P0=0;
P2=0XFF,
P0=tabledu[time%10];
P2=tablewe[3];
delay(2);
P0=0;
P2=0XFF;
}
else if(K_timecountflag==1)
{
P0=0X07;  //��һλ��ڶ�λ����ܺ�������ʾ����T��������ʾ����ʱ�����
P2=tablewe[0];
delay(2);
P0=0;
P2=0XFF;
P0=0X31;
P2=tablewe[1];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[datitime/10];
P2=tablewe[2];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[datitime%10];
P2=tablewe[3];
delay(2);
P0=0;
P2=0XEF;
}
else     //������ʾ F FF
{
P0=tabledu[15];
P2=tablewe[0];
delay(2);
P0=0;
P0=tabledu[15];
P2=tablewe[2];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[51];
P2=tablewe[3];
delay(2);
P0=0;
P2=0XFF;
}     }     }}
void Time_Scan(void)   //����ʱ���ɨ��
{ 
	if(K_startcount==0)    //����ʱ�����
	{
		delay(10);
		if(K_startcount==0)
		{
			while(K_startcount==0);
			time++;
			if(time==50)
			{
			time=20;
			}
			K_startcountflag=1; //������ʱ���־λ��1
			K_timecountflag=0; //ͬʱ�رմ���ʱ���־λ
		}
	}
	if(K_timecount==0)
	{
		delay(10);
		if(K_timecount==0)
		{
			while(!K_timecount);
			datitime++;
			if(datitime==60)
			{
			datitime=30;
			}
			K_timecountflag=1;
			K_startcountflag=0;
		}
	}
}
void main(void)
{
	T0_Init();
	while(1)
	{
		Key_Scan();    //��ʼ��ɨ��
		if((flag==0)&&(s_flag==1))    //����ʼ��û���¼�û����Υ��ʱ�ſɽ���ʱ�����
		{
			Time_Scan( );
		}
		if((flag==1)&&(s_flag==0)) //����ʼ�����¼���������Ž��п�ʼ�ش��ʱ����ʱ
		{
			if(K_Time==0)
			{
				delay(10);
				if(K_Time==0)
				{
					while (!K_Time);
					s=datitime;
					TR0=1;
					tt=0;
					TR1=1;
					if(K_Time==0)//��λ����
					{delay(10);
					reset_1();}
				}
			}
		}
	
		if((flag==0)&&(s_flag==1))  ///Υ��
		{ 
			Scan();
			if(num!=0)    //��ʼ��û�а���ʱ�����˰�����������������Υ���־λ
			{
				fall_flag=1;
				rled=0;
			}   
		}
		if((flag==1)&&(s_flag==1))    //�����ʼ�������������û���˰��£�����а�·�����ɨ��
		{
			Scan();
		}
		display();  //��ʾ���������
	}
}
void timer0(void) interrupt 1
{
TH0=(65536-2000)/256;//2ms
TL0=(65536-2000)%256;
if(b_flag)   //��ʼ��START�������£��һ������1�룩����ʾ��ʼ����
{
beep= ~beep;
}
else
beep=1;
if(s<5)
{
if(s%2==0)
{
b_flag=1;
rled=0;
}
else
{
b_flag=0;
rled=1;
}}
tt++;
if(tt==500)            //1s
{
tt=0;
s--;   
b_flag=0; //�رտ�ʼ��������1s�����
if(s==-1)
reset_1();
}}
void reset_1(void)
{
s=time;
TR0=0;
flag=0;   
s_flag=1;
num=0;
rled=1;  	
}
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


