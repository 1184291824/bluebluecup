#ifndef __LCD1602_H__
#define __LCD1602_H__

/*
**********************************************************************
*                         ͷ�ļ�����
**********************************************************************
*/
#include "common.h"


/*
**********************************************************************
*                         ���غ궨��
**********************************************************************
*/
#define LCD1602_DATA_PORT	 P0			// LCD1602��8λ���ݶ˿�


/*
**********************************************************************
*                         �ⲿ����ԭ������
**********************************************************************
*/

void Lcd1602Init(void);
void Lcd1602SetCursor(u8 x,u8 y);
void Lcd1602ShowStr(u8 x, u8 y, u8 *pStr);


#endif