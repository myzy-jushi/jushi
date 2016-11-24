#include "led.h"
#include "timer.h"

LED_Control_Str LED_Ctrl;
LED_Control_Str_P LED_Ctrl_Ptr = &LED_Ctrl;



void Led_Ctrl_Func(LED_Control_Str_P ptr)
{	
	if(1 == ptr->TwinkleFlag)
	{
		if(LED_Control_Mode_Average == ptr->CtrlMode)
		{
			if(0 == ptr->TimeRec)
			{
				ptr->TimeRec = SystemRunningTime;
				ptr->LED_Off();
				
				ptr->Step = 0;
			}
			else
			{
				
				if(0 == ptr->Step)
				{
					if(SystemRunningTime - ptr->TimeRec >= ptr->IntervalTime_ms * 10)
					{
						ptr->LED_ON_Check();
						ptr->Step = 1;
						ptr->TimeRec = SystemRunningTime;
					}
				}
				else if(1 == ptr->Step)
				{
					if(SystemRunningTime - ptr->TimeRec >= ptr->IntervalTime_ms * 10)
					{
						ptr->TimeRec = 0;
						
						ptr->TempCounter++;
					}
				}
				
			}

		}
		else if(LED_Control_Mode_Free == ptr->CtrlMode)
		{
			if(0 == ptr->TimeRec)
			{
				ptr->TimeRec = SystemRunningTime;
				ptr->LED_ON_Check();
				
				ptr->Step = 0;
			}
			else
			{
				if(0 == ptr->Step)
				{
					if(SystemRunningTime - ptr->TimeRec >= ptr->LED_ON_Time_ms * 10)
					{
						ptr->LED_Off();
						ptr->Step = 1;
						ptr->TimeRec = SystemRunningTime;
					}
				}
				else if(1 == ptr->Step)
				{
					if(SystemRunningTime - ptr->TimeRec >= ptr->LED_OFF_Time_ms * 10)
					{
						ptr->TimeRec = 0;
						
						ptr->TempCounter++;
					}
				}
				
			}
		}		

		if(LED_Twinkle_Count_Enable == ptr->CountMode)
		{
			if(ptr->TempCounter >= ptr->TwinkleCountCMP)
			{
				ptr->TwinkleFlag = 0;
				ptr->TempCounter = 0;
				ptr->Step = 0;
			}
		}
		else if(LED_Twinkle_Count_Disable == ptr->CountMode)
		{
			ptr->TempCounter = 0;
		}
	}
	else
	{
		ptr->TimeRec = 0;
		ptr->LED_ON_Check();
		ptr->TempCounter = 0;
		ptr->Step = 0;
	}
}


/**
  * @brief  点亮LED指示灯
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_On_Func(void)
{
	LED_ON();
}

/**
  * @brief  关闭LED指示灯
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_Off_Func(void)
{
	LED_OFF();
}

/**
  * @brief  LED指示灯反转
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_Toggle_Func(void)
{
	LED = ~LED;
}

void LED_ON_Check_Func(void)
{
	LED_ON_CHECK();
}

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	   
	/* 使能GPIOB时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* 配置GPIOB.12为推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, GPIO_Pin_1);//设置GPIOB.12为高电平，关闭LED
}

/**
  * @brief  LED IO初始化
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void LED_Init(void)
{
	LED_GPIO_Init();

	LED_Ctrl_Ptr->TwinkleFlag = 0;
	LED_Ctrl_Ptr->TwinkleCountCMP = 0;
	LED_Ctrl_Ptr->LED_Status = 0;
	LED_Ctrl_Ptr->IntervalTime_ms = 200;
	LED_Ctrl_Ptr->CountMode = LED_Twinkle_Count_Disable;
	LED_Ctrl_Ptr->CtrlMode = LED_Control_Mode_Free;
	LED_Ctrl_Ptr->LED_ON_Time_ms = 35;
	LED_Ctrl_Ptr->LED_OFF_Time_ms = 3000;
	LED_Ctrl_Ptr->TempCounter = 0;
	LED_Ctrl_Ptr->Step = 0;
	LED_Ctrl_Ptr->TimeRec = 0;
	
	LED_Ctrl_Ptr->LED_Ctrl_Func = Led_Ctrl_Func;
	LED_Ctrl_Ptr->LED_On = LED_On_Func;
	LED_Ctrl_Ptr->LED_Off = LED_Off_Func;
	LED_Ctrl_Ptr->LED_Toggle = LED_Toggle_Func;
	LED_Ctrl_Ptr->LED_ON_Check = LED_ON_Check_Func;

	LED_SET();
	
}


