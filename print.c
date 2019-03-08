#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_DATA_PORT	 P0
typedef unsigned char u8; 		// ����������u8�򻯴����д
typedef unsigned int u16;
void Lcd1602Init(void);
void Lcd1602SetCursor(u8 x,u8 y);
void Lcd1602ShowStr(u8 x, u8 y, u8 *pStr);
sbit gLcd1602_E	 = P2^7;			// LCD1602�������ߵ�ʹ���ź�
sbit gLcd1602_RW = P2^5;			// LCD1602�������ߵĶ�дѡ���ź�
sbit gLcd1602_RS = P2^6;			// LCD1602�������ߵ�����/����ѡ���ź�
sbit beep=P1^5;//������

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
 
static void Lcd1602WaitNoBusy(void)  //æ��⺯�����ж�bit7��0������ִ�У�1��ֹ
{
    u8 sta = 0;      //

    LCD1602_DATA_PORT = 0xff;
    gLcd1602_RS = 0;
    gLcd1602_RW = 1;
    do
    {
        gLcd1602_E = 1;
        sta = LCD1602_DATA_PORT;
        gLcd1602_E = 0;    //ʹ�ܣ���������ͣ��ͷ�����
    }while(sta & 0x80);
}

static void Lcd1602WriteCmd(u8 cmd)	  //����LCD1602�Ͳ�ʱ����LCD�ڲ�д��8λ������
{
	Lcd1602WaitNoBusy();		// �ȵȴ�LCD1602���ڲ�æ״̬

	gLcd1602_E = 0;     		// ��ֹLCD
	gLcd1602_RS = 0;	   		// ѡ��������ģʽ
	gLcd1602_RW = 0;	   		// ѡ��д��ģʽ	
	LCD1602_DATA_PORT = cmd;    // ��1�ֽ������ַ���8λ�������ݶ˿�
	gLcd1602_E = 1;	          	// ʹ��LED
	gLcd1602_E = 0;				// ��ֹLCD
}

static void Lcd1602WriteData(u8 dat)	//����LCD1602�Ͳ�ʱ����LCD�ڲ�д��8λ����		
{
	Lcd1602WaitNoBusy();		// �ȵȴ�LCD1602���ڲ�æ״̬

	gLcd1602_E = 0;     		// ��ֹLCD
	gLcd1602_RS = 1;	   		// ѡ��������ģʽ
	gLcd1602_RW = 0;	   		// ѡ��д��ģʽ	
	LCD1602_DATA_PORT = dat;    // ��1�ֽ������ַ���8λ�������ݶ˿�
	gLcd1602_E = 1;	          	// ʹ��LED
	gLcd1602_E = 0;				// ��ֹLCD
}

void Lcd1602Init(void)		//����LCD1602�Ͳ�ʱ����г�ʼ������				
{
 	Lcd1602WriteCmd(0x38);  	// ���������ֲ�ĳ�ʼ��ʱ���ȷ���38H
	delay5ms();					// ��ʱ5ms
	Lcd1602WriteCmd(0x38);  	// ���������ֲ�ĳ�ʼ��ʱ���ȷ���38H
	delay5ms();					// ��ʱ5ms
	Lcd1602WriteCmd(0x38);  	// ���������ֲ�ĳ�ʼ��ʱ���ȷ���38H
	delay5ms();					// ��ʱ5ms
	Lcd1602WriteCmd(0x38);		// ��ʾģʽ����
	Lcd1602WriteCmd(0x08);		// �ر���ʾ
	Lcd1602WriteCmd(0x01);		// ������ͬʱ������ָ�룩
	Lcd1602WriteCmd(0x06);		// ��д��ָ���Զ���1��д1���ַ���������ʾ���ƶ�
	Lcd1602WriteCmd(0x0c);		// ����ʾ������ʾ���
}

/*********************************************************************
* �� �� ��       : Lcd1602SetCursor
* ��������		 : �������������õ�ǰ���λ�ã���ʵ�������õ�ǰ���ڱ༭
*				   ��λ�ã���ʵ�����ڲ������ݵ�ַָ�룬��ʵ����RAM�Դ�
*				   ��ƫ����
* �����б�       : x - �������꣬��Χ��0-15
*				   y - �������꣬0��ʾ����һ�У�1��ʾ����һ��
* �������    	 : ��
*********************************************************************/
void Lcd1602SetCursor(u8 x, u8 y)  	// ������ʾ
{
    u8 addr = 0;

   	switch (y)
	{
		case 0:	 					// ����һ��
			addr = 0x00 + x;		break;	
		case 1:						// ����һ��
			addr = 0x40 + x; 		break;
		default:
									break;
	}
    Lcd1602WriteCmd(addr | 0x80);
}


/*********************************************************************
* �� �� ��       : Lcd1602ShowStr
* ��������		 : ������(x,y)��ʼ��ʾ�ַ���str��ע������������ܿ���
*				   ��ʾ����Ϊ�Դ��ַ�ǲ������ġ�
* �����б�       : x - �������꣬��Χ��0-15
*				   y - �������꣬0��ʾ����һ�У�1��ʾ����һ��
*				   pStr - ָ�����ʾ���ַ�����ָ��
* �������    	 : ��
*********************************************************************/
void Lcd1602ShowStr(u8 x, u8 y, u8 *pStr)     //��ʾ�ַ���
{
    Lcd1602SetCursor(x, y);      //��ǰ�ַ�������
    while (*pStr != '\0')
    {
        Lcd1602WriteData(*pStr++);
    }
}



/*********************************************************************
* �� �� ��       : delay5ms
* ��������		 : ��Ƭ��С����V1.3���ɵ���ʱ5ms�ľ�ȷ��ʱ����
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
static void delay5ms(void)   //��� 0us
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
//һ�¿�ʼΪ����ʱ******************************************************************
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
	//��ʱms
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