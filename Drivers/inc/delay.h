#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"


void Delay_Init(void);	//SysTick延时初始化
void delay_ms(uint16_t xms);//毫秒延时函数
void delay_us(uint32_t xus);//微秒延时函数

#endif

