#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "bitband.h"	//使用位带操作，一定要包含此头文件

#define LED_FLAG					(CREATE_BIT_FLAG(0))
#define LED_SET()					{SET_BIT_FLAG(LED_Ctrl_Ptr->LED_Status, LED_FLAG);}
#define LED_CLEAN()					{CLEAN_BIT_FLAG(LED_Ctrl_Ptr->LED_Status, LED_FLAG);}
#define LED_CHECK					(CHECK_BIT_VALUE_TOGGLE(LED_Ctrl_Ptr->LED_Status, LED_FLAG))

#define LED_IN						GPIOin(GPIOA, 1)	//定义PB12位带操作
#define LED							GPIOout(GPIOA, 1)
#define LED_ON()					{LED = 0;}
#define LED_OFF()					{LED = 1;}
#define LED_ON_CHECK()				{LED = LED_CHECK;}


typedef enum
{
	LED_Twinkle_Count_Enable,
	LED_Twinkle_Count_Disable,
}LED_Twinkle_Count_Mode;

typedef enum
{
	LED_Control_Mode_Average,
	LED_Control_Mode_Free,
}LED_Control_Mode;

typedef struct LED_Control_Struct
{
	
	// 设置变量
	u8 	TwinkleFlag;
	u8 	TwinkleCountCMP;
	u8	LED_Status;
	u16 IntervalTime_ms;
	LED_Twinkle_Count_Mode CountMode;
	LED_Control_Mode CtrlMode;
	u32 LED_ON_Time_ms;
	u32 LED_OFF_Time_ms;

	// 临时变量
	u8 TempCounter;
	u8 Step;
	u32 TimeRec;
	
	void (*LED_Ctrl_Func)(struct LED_Control_Struct *);

	void (*LED_On)(void);
	void (*LED_Off)(void);
	void (*LED_Toggle)(void);
	void (*LED_ON_Check)(void);
}LED_Control_Str, *LED_Control_Str_P;


void LED_Init(void);			//LED IO初始化

extern LED_Control_Str_P LED_Ctrl_Ptr;


#endif

