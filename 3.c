#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
char s;
uchar num=0; //抢答人编号，等于0代表没人抢答
char time=20;      //抢答时间
char datitime=30;      
uint tt,t1;      
bit flag, s_flag=1,b_flag, fall_flag;     //flag开始键按下，s_flag没有人抢答（1），b_flag蜂鸣器，fall_flag有人违规 
bit K_startcountflag, K_timecountflag;    //

sbit K0=P3^0;		//开始按钮    //P3^0
sbit beep=P1^5;   	//蜂鸣器    //P1^5
sbit rled=P2^0;     //发光二极管	//P2^0
sbit K1=P1^0;		//8个按钮		//
sbit K2=P1^1;
sbit K3=P1^2;
sbit K4=P1^3;
sbit K5=P1^4;
sbit K6=P3^5;		//P2^5
sbit K7=P1^6;
sbit K8=P1^7;
sbit K_Time=P3^1;   		//答题计时键        //P3^1
sbit K_startcount=P3^2;  	//开始抢答时间调整键//p3^2
sbit K_timecount=P3^3;   	//答题计时时间调整键//p3^3
void delay(uchar ms)
	//延时ms
{
	uchar y;
	for(; ms>0;ms--)
	for(y=120;y>0;y--);
}

//数码管-表
uchar code tabledu[ ]={0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71
};

//数码管-位码
uchar code tablewe[]={0x0C,0x08,0x04,0x00};   //{0x00,0x04,0x08,0x0C}
void reset_1();

//Timer0-初始化20ms
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

//主持人按键扫描
void Key_Scan( void)
{
	if(K0==0)
	{
		delay(10);//按键消抖
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

//8路按键扫描
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

//显示
void display (void)
{
	if(flag==1)
	{	
		if(num!=0)    //如果有人抢答，则显示相应的几号
		{
			P0=tabledu[num];  
			P2=tablewe[0];
			delay(2);
			P0=0;
			P2=0XFF;
		}
		else//否则没人抢答，则前面不显示几号
		{
			P0=0;
			P2=0XFF;
		}
		
		P0=tabledu[s/10];   //倒计时
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
else    //如果开始键没有按下，则显示F FF(若有违规者，则显示违规号码及FF)或时间调整
{
if(fall_flag==1)    //违规
{
if(num!=0)
{
P0=tabledu[num];      //显示几号违规
P2=tablewe[0];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[15];    //下面显示FF，表示违规
P2=tablewe[2];
delay(2);
P0=0;      ///消隐
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
else      //没有人违规才显示调整时间
{
if(K_startcountflag==1)
{
P0=0X6D; //第一位数码管显示’’5(s)’’(表示抢答事件调整)
P2=tablewe[0];
delay(2);
P0=0;
P2=0XFF;
P0=tabledu[time/10]; //下面显示调整的抢答时间  
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
P0=0X07;  //第一位与第二位数码管合起来显示’’T’’，表示答题时间调整
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
else     //否则显示 F FF
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
void Time_Scan(void)   //调整时间键扫描
{ 
	if(K_startcount==0)    //抢答时间调整
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
			K_startcountflag=1; //将抢答时间标志位置1
			K_timecountflag=0; //同时关闭答题时间标志位
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
		Key_Scan();    //开始键扫描
		if((flag==0)&&(s_flag==1))    //当开始键没按下及没有人违规时才可进行时间调整
		{
			Time_Scan( );
		}
		if((flag==1)&&(s_flag==0)) //当开始键按下及有人抢答才进行开始回答计时倒计时
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
					if(K_Time==0)//复位开关
					{delay(10);
					reset_1();}
				}
			}
		}
	
		if((flag==0)&&(s_flag==1))  ///违规
		{ 
			Scan();
			if(num!=0)    //开始键没有按下时，有人按下了抢答器，则置违规标志位
			{
				fall_flag=1;
				rled=0;
			}   
		}
		if((flag==1)&&(s_flag==1))    //如果开始键按下且抢答键没有人按下，则进行八路抢答键扫描
		{
			Scan();
		}
		display();  //显示到数码管上
	}
}
void timer0(void) interrupt 1
{
TH0=(65536-2000)/256;//2ms
TL0=(65536-2000)%256;
if(b_flag)   //开始（START）键按下，嘟一声（长1秒），表示开始抢答
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
b_flag=0; //关闭开始键按下响1s的嘟声
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


