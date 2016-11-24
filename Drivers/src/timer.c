#include "timer.h"

vu32 SystemRunningTime = 0x00000000;

void Run_Time_Handle(void (*func)(void))
{
	
	// 0.1ms
	if(TIM4->SR & 0x0001)	// ����ж�
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
	
	TIM4->SR &= ~(1 << 0);	// ����жϱ�־λ
}

void Timer3_Init(u16 arr, u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//ѡ���жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//����ʽ�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//��Ӧʽ�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);//��ʼ��
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIMx_2_7_ENABLE(3);					// TIM4ʱ��ʹ��
	TIM3->ARR = arr;					// �Զ�װ��ֵ
	TIM3->PSC = psc;					// ��Ƶϵ��
	TIM3->DIER |= (1 << 0);				// ��������ж�
	TIM3->DIER |= (1 << 6);				// �������ж�
	TIMx_ON(TIM3);
	
}


void Timer4_Init(u16 arr, u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//ѡ���жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//����ʽ�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//��Ӧʽ�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);//��ʼ��
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIMx_2_7_ENABLE(4);					// TIM4ʱ��ʹ��
	TIM4->ARR = arr;					// �Զ�װ��ֵ
	TIM4->PSC = psc;					// ��Ƶϵ��
	TIM4->DIER |= (1 << 0);				// ��������ж�
	TIM4->DIER |= (1 << 6);				// �������ж�
	TIMx_ON(TIM4);
	
}


void Timer_Init(void)
{
	Timer4_Init(9, 719);		// 0.1ms
	
	//Timer3_Init(9, 71);
	
}


