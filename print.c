#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_DATA_PORT	 P0
typedef unsigned char u8; 		// 重命名类型u8简化代码编写
typedef unsigned int u16;
void Lcd1602Init(void);
void Lcd1602SetCursor(u8 x,u8 y);
void Lcd1602ShowStr(u8 x, u8 y, u8 *pStr);
sbit gLcd1602_E	 = P2^7;			// LCD1602控制总线的使能信号
sbit gLcd1602_RW = P2^5;			// LCD1602控制总线的读写选择信号
sbit gLcd1602_RS = P2^6;			// LCD1602控制总线的数据/命令选择信号
sbit beep=P1^5;//蜂鸣器

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

/*
static void delay5ms(void);
static void Lcd1602WaitNoBusy(void);
static void Lcd1602WriteCmd(u8 cmd);
static void Lcd1602WriteData(u8 dat); */

 void delay5ms(void);
 void Lcd1602WaitNoBusy(void);
 void Lcd1602WriteCmd(u8 cmd);
 void Lcd1602WriteData(u8 dat);
 
static void Lcd1602WaitNoBusy(void)  //忙检测函数，判断bit7是0，允许执行；1禁止
{
    u8 sta = 0;      //

    LCD1602_DATA_PORT = 0xff;
    gLcd1602_RS = 0;
    gLcd1602_RW = 1;
    do
    {
        gLcd1602_E = 1;
        sta = LCD1602_DATA_PORT;
        gLcd1602_E = 0;    //使能，用完就拉低，释放总线
    }while(sta & 0x80);
}

static void Lcd1602WriteCmd(u8 cmd)	  //按照LCD1602低层时序向LCD内部写入8位命令字
{
	Lcd1602WaitNoBusy();		// 先等待LCD1602处于不忙状态

	gLcd1602_E = 0;     		// 禁止LCD
	gLcd1602_RS = 0;	   		// 选择发送命令模式
	gLcd1602_RW = 0;	   		// 选择写入模式	
	LCD1602_DATA_PORT = cmd;    // 将1字节命令字放入8位并行数据端口
	gLcd1602_E = 1;	          	// 使能LED
	gLcd1602_E = 0;				// 禁止LCD
}

static void Lcd1602WriteData(u8 dat)	//按照LCD1602低层时序向LCD内部写入8位数据		
{
	Lcd1602WaitNoBusy();		// 先等待LCD1602处于不忙状态

	gLcd1602_E = 0;     		// 禁止LCD
	gLcd1602_RS = 1;	   		// 选择发送数据模式
	gLcd1602_RW = 0;	   		// 选择写入模式	
	LCD1602_DATA_PORT = dat;    // 将1字节命令字放入8位并行数据端口
	gLcd1602_E = 1;	          	// 使能LED
	gLcd1602_E = 0;				// 禁止LCD
}

void Lcd1602Init(void)		//按照LCD1602低层时序进行初始化序列				
{
 	Lcd1602WriteCmd(0x38);  	// 按照数据手册的初始化时序，先发送38H
	delay5ms();					// 延时5ms
	Lcd1602WriteCmd(0x38);  	// 按照数据手册的初始化时序，先发送38H
	delay5ms();					// 延时5ms
	Lcd1602WriteCmd(0x38);  	// 按照数据手册的初始化时序，先发送38H
	delay5ms();					// 延时5ms
	Lcd1602WriteCmd(0x38);		// 显示模式设置
	Lcd1602WriteCmd(0x08);		// 关闭显示
	Lcd1602WriteCmd(0x01);		// 清屏（同时清数据指针）
	Lcd1602WriteCmd(0x06);		// 读写后指针自动加1，写1个字符后整屏显示不移动
	Lcd1602WriteCmd(0x0c);		// 开显示，不显示光标
}

/*********************************************************************
* 函 数 名       : Lcd1602SetCursor
* 函数功能		 : 本函数用来设置当前光标位置，其实就是设置当前正在编辑
*				   的位置，其实就是内部的数据地址指针，其实就是RAM显存
*				   的偏移量
* 参数列表       : x - 横向坐标，范围是0-15
*				   y - 纵向坐标，0表示上面一行，1表示下面一行
* 函数输出    	 : 无
*********************************************************************/
void Lcd1602SetCursor(u8 x, u8 y)  	// 坐标显示
{
    u8 addr = 0;

   	switch (y)
	{
		case 0:	 					// 上面一行
			addr = 0x00 + x;		break;	
		case 1:						// 下面一行
			addr = 0x40 + x; 		break;
		default:
									break;
	}
    Lcd1602WriteCmd(addr | 0x80);
}


/*********************************************************************
* 函 数 名       : Lcd1602ShowStr
* 函数功能		 : 从坐标(x,y)开始显示字符串str，注意这个函数不能跨行
*				   显示，因为显存地址是不连续的。
* 参数列表       : x - 横向坐标，范围是0-15
*				   y - 纵向坐标，0表示上面一行，1表示下面一行
*				   pStr - 指向待显示的字符串的指针
* 函数输出    	 : 无
*********************************************************************/
void Lcd1602ShowStr(u8 x, u8 y, u8 *pStr)     //显示字符串
{
    Lcd1602SetCursor(x, y);      //当前字符的坐标
    while (*pStr != '\0')
    {
        Lcd1602WriteData(*pStr++);
    }
}



/*********************************************************************
* 函 数 名       : delay5ms
* 函数功能		 : 单片机小精灵V1.3生成的延时5ms的精确延时函数
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
static void delay5ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=19;b>0;b--)
        for(a=130;a>0;a--);
}  
int score[8];
int i;
int shiwei=0;
int gewei=0;
char bb[]="0123456789";
void print(void)
{
	Lcd1602Init();
	Lcd1602ShowStr(0,0,"A:00B:00C:00D:00");
	Lcd1602ShowStr(0,1,"E:00F:00G:00H:00");
	score[1]=20;
	for(i=0;i<3;i++){
	shiwei=score[i]/10;
	gewei=score[i]%10;
	Lcd1602SetCursor(2+4*i,0);
	Lcd1602WriteData(bb[shiwei]);
	Lcd1602SetCursor(3+4*i,0);
	Lcd1602WriteData(bb[gewei]);
	}
	for(i=4;i<7;i++){
	shiwei=score[i]/10;
	gewei=score[i]%10;
	Lcd1602SetCursor(2+4*(i-4),1);
	Lcd1602WriteData(bb[shiwei]);
	Lcd1602SetCursor(3+4*(i-4),1);
	Lcd1602WriteData(bb[gewei]);
	}
}
//一下开始为倒计时******************************************************************
int t1=0;
int time=30;
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
void delay(uchar ms)
	//延时ms
{
	uchar y;
	for(; ms>0;ms--)
	for(y=120;y>0;y--);
}
 void main(void)
{
	T0_Init();
	Lcd1602Init();
	Lcd1602ShowStr(0,0,"20:00");
	shiwei=time/10;
	gewei=time%10;
	Lcd1602SetCursor(0,0);
	Lcd1602WriteData(bb[shiwei]);
	Lcd1602SetCursor(1,0);
	Lcd1602WriteData(bb[gewei]);
while(1)
{
	shiwei=time/10;
	gewei=time%10;
	Lcd1602SetCursor(0,0);
	Lcd1602WriteData(bb[shiwei]);
	Lcd1602SetCursor(1,0);
	Lcd1602WriteData(bb[gewei]);
	shiwei=t1/10;
	gewei=t1%10;
	Lcd1602SetCursor(3,0);
	Lcd1602WriteData(bb[shiwei]);
	Lcd1602SetCursor(4,0);
	Lcd1602WriteData(bb[gewei]);
	if(t1==500)
	{
	time--;
	}
}
}