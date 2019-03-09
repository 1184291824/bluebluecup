/*
**********************************************************************
*                         ͷ�ļ�����
**********************************************************************
*/
#include "keyScan.h"


/*
**********************************************************************
*                         ���غ궨��
**********************************************************************
*/


/*
**********************************************************************
*                         ����ȫ�ֱ���
**********************************************************************
*/
sbit key1 = P3^0;		//��������s2
sbit key2 = P3^1;		//��������s3
sbit key3 = P3^2;		//��������s4
sbit key4 = P3^3;		//��������s5

sbit hot1 = P1^0;		//�ȼ�1
sbit hot2 = P1^1;		//�ȼ�2
sbit hot3 = P1^2;		//�ȼ�3
sbit hot4 = P1^3;		//�ȼ�4
sbit hot5 = P1^4;		//�ȼ�5
sbit hot6 = P1^5;		//�ȼ�6
sbit hot7 = P1^6;		//�ȼ�7
sbit hot8 = P1^7;		//�ȼ�8


/*
**********************************************************************
*                         ����ԭ������
**********************************************************************
*/
int control_key_scan(void);
int hot_key_scan(u16 flag);
void delay10ms(void);

/*
**********************************************************************
*  ��������control_key_scan
*  ���ã����ư���ɨ��
*  ���أ������µİ���ֵ�����û�а������·���0
**********************************************************************
*/
int control_key_scan(void)
{
	if( key1 == 0 )
	{
		delay10ms();
		if ( key1 == 0 )
		{
			return 1;
		}
	}

	if( key2 == 0 )
	{
		delay10ms();
		if ( key2 == 0 )
		{
			return 2;
		}
	}

	if( key3 == 0 )
	{
		delay10ms();
		if ( key3 == 0 )
		{
			return 3;
		}
	}

	if( key4 == 0 )
	{
		delay10ms();
		if ( key4 == 0 )
		{
			return 4;
		}
	}

	return 0;
}



/*
**********************************************************************
*  ��������hot_key_scan
*  ���ã�8·�ȼ�ɨ��
*  ���أ������µİ���ֵ�����û�а������·���0
**********************************************************************
*/
int hot_key_scan(u16 flag)
{	
	if(flag==0) return 0;
	if( hot1 == 0 )
	{
		delay10ms();
		if ( hot1 == 0 )
		{
			return 1;
		}
	}

	if( hot2 == 0 )
	{
		delay10ms();
		if ( hot2 == 0 )
		{
			return 2;
		}
	}

	if( hot3 == 0 )
	{
		delay10ms();
		if ( hot3 == 0 )
		{
			return 3;
		}
	}

	if( hot4 == 0 )
	{
		delay10ms();
		if ( hot4 == 0 )
		{
			return 4;
		}
	}

	if( hot5 == 0 )
	{
		delay10ms();
		if ( hot5 == 0 )
		{
			return 5;
		}
	}

	if( hot6 == 0 )
	{
		delay10ms();
		if ( hot6 == 0 )
		{
			return 6;
		}
	}

	if( hot7 == 0 )
	{
		delay10ms();
		if ( hot7 == 0 )
		{
			return 7;
		}
	}

	if( hot8 == 0 )
	{
		delay10ms();
		if ( hot8 == 0 )
		{
			return 8;
		}
	}

	return 0;
}



/*
**********************************************************************
*  ��������delay10ms
*  ���ã��ȴ�10ms
*  ���أ���
**********************************************************************
*/
void delay10ms(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}