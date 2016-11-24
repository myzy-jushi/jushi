#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include "stm32f10x.h"

#define TIMx_ON(TIMx)			(TIMx->CR1 |= 0x01)
#define TIMx_OFF(TIMx)			(TIMx->CR1 = ~((~TIMx->CR1) | 0x01))

#define TIMx_2_7_ENABLE(x)		(RCC->APB1ENR |= (1 << (x - 2)))

extern vu32 SystemRunningTime;

void Timer_Init(void);
void Run_Time_Handle(void (*func)(void));

#endif





