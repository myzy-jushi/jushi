#include "timer.h"

vu32 SystemRunningTime = 0x00000000;

void Run_Time_Handle(void (*func)(void))
{
	
	// 0.1ms
	if(TIM4->SR & 0x0001)	// 溢出中断
	{
		SystemRunningTime++;
		/*
		if(func)
		{
			func();
			printf("Run_Time_func!\r\n");
		}
		*/
	}
	
	TIM4->SR &= ~(1 << 0);	// 清除中断标志位
}

void Timer3_Init(u16 arr, u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//选择中断分组
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢断式中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//响应式中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//使能中断
	NVIC_Init(&NVIC_InitStructure);//初始化
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIMx_2_7_ENABLE(3);					// TIM4时钟使能
	TIM3->ARR = arr;					// 自动装载值
	TIM3->PSC = psc;					// 分频系数
	TIM3->DIER |= (1 << 0);				// 允许更新中断
	TIM3->DIER |= (1 << 6);				// 允许触发中断
	TIMx_ON(TIM3);
	
}


void Timer4_Init(u16 arr, u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//选择中断分组
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢断式中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//响应式中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//使能中断
	NVIC_Init(&NVIC_InitStructure);//初始化
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIMx_2_7_ENABLE(4);					// TIM4时钟使能
	TIM4->ARR = arr;					// 自动装载值
	TIM4->PSC = psc;					// 分频系数
	TIM4->DIER |= (1 << 0);				// 允许更新中断
	TIM4->DIER |= (1 << 6);				// 允许触发中断
	TIMx_ON(TIM4);
	
}


void Timer_Init(void)
{
	Timer4_Init(9, 719);		// 0.1ms
	
	//Timer3_Init(9, 71);
	
}


