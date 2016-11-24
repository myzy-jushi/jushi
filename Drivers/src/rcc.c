#include "rcc.h"


/**********************************************************************************
������: RCC_Config
��������: ����ϵͳ������ʱ��
�������: ��
������: PLL_CLK = 72MHz, HCLK = 72MHz, PCLK1 = 36MHz, PCLK2 = 72MHz
����ֵ: �����óɹ�������SUCCESS, ������ʧ�ܣ�����ERROR
***********************************************************************************/

FlagStatus Check_HSEStatus(void)
{
	FlagStatus bitStatus = RESET;

	if(( RCC->CR & 1 << 17) != (u32)RESET)
	{
		bitStatus = SET;
	}

	return bitStatus;
}

	
ErrorStatus RCC_WaitingForHSEStartUp(void)
{
	vu32 StartUpCounter = HSEStartUp_TimeOut;
	ErrorStatus status = ERROR;
	FlagStatus HSEStatus = RESET;
	
	do
	{
		HSEStatus = Check_HSEStatus();
		StartUpCounter--;
	}while((HSEStatus == RESET) && (StartUpCounter));

	if(RESET != HSEStatus)
	{
		status = SUCCESS;
	}

	return status;
}

ErrorStatus RCC_Init(void)
{
	//ErrorStatus HSEStartUpStatus;	/*����HSE����λ*/
	ErrorStatus cbRccConfigStatus;
	RCC_DeInit();
	
	/*1. ͨ��RCC���ʱ�ӿ��ƼĴ���(RCC Clock Control Register)����HSE*/
	//RCC_HSEConfig(RCC_HSE_ON);		/*ͨ��RCC����HSE*/
	
	//Reset HSEON bit
	RCC->CR &= 0xFFFAFFFF;		//HSEBYP:0 HSEON:0
	//set HSEON Enable
	RCC->CR |= 1 << 16;
	
	//HSEStartUpStatus = RCC_WaitForHSEStartUp();		/*�ȴ�HSE�ȶ�(ͨ������λ�ж�)*/
	
	if(SUCCESS == RCC_WaitingForHSEStartUp())	/*���HSE�����ɹ�*/
	//if(SUCCESS == HSEStartUpStatus)
	{
		/*2. ����FLASHԤȡ����*/
		//FLASH_SetLatency(FLASH_Latency_2);	/*����FLASH������ʱ����Ϊ2*/
		//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	/*ʹ��FLASHԤȡ����*/
		FLASH->ACR |= 0x12;
		//while(1 != ((FLASH->ACR >> 5) & 1));

		/*3. ����PLL���໷*/
		//RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/*4. ����HCLK*/
		//RCC_HCLKConfig(RCC_SYSCLK_Div1);	/*����HCLKʱ��ԴΪSYSCLK 1��Ƶ*/
		
		/*5. ����PCLK1*/
		//RCC_PCLK1Config(RCC_HCLK_Div2);		/*����PCLK1ʱ��ԴΪHCLK(AHB) 2��Ƶ*/
		
		/*6. ����PCLK2*/
		//RCC_PCLK2Config(RCC_HCLK_Div1);		/*����PCLK2ʱ��ԴΪHCLK(AHB) 1��Ƶ*/
		
		RCC->CFGR |= 0x071D040C;

		/*7. ʹ��PLL���ҵȴ�PLL�ȶ�*/
		//RCC_PLLCmd(ENABLE);		/*ʹ��PLL*/
		RCC->CR |= 1 << 24;
		//while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLRDY));		/*�ȴ�PLL�ȶ����*/
		while(1 != ((RCC->CR >> 25) & 1));

		/*8. ��SYSCLKʱ��Դ�л�ΪPLL�����ȴ��ȶ�*/
		//RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		/*ѡ��PLL��ΪSYSCLKʱ��Դ*/
		RCC->CFGR |= 0x0000000E;
		//while(0x08 != RCC_GetSYSCLKSource());		/*�ȴ�PLL��ΪSYSCLKʱ��Դ*/
		while(0x08 != (RCC->CFGR & (u32)0x0000000C));
		
		cbRccConfigStatus = SUCCESS;
	}
	else	/*���HSE����ʧ��*/
	{
		cbRccConfigStatus = ERROR;
	}

	return (cbRccConfigStatus);
}






