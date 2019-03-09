#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__

/*
**********************************************************************
*                         头文件包含
**********************************************************************
*/
#include "common.h"


/*
**********************************************************************
*                         本地宏定义
**********************************************************************
*/


/*
**********************************************************************
*                         外部函数原型声明
**********************************************************************
*/

int control_key_scan(void);
int hot_key_scan(u16 flag);
void delay10ms(void);

#endif