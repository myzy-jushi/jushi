#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"


void Delay_Init(void);	//SysTick��ʱ��ʼ��
void delay_ms(uint16_t xms);//������ʱ����
void delay_us(uint32_t xus);//΢����ʱ����

#endif

