/*******************************************************************************
 * @name    : GPIO位带操作头文件
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : 介绍通过位带映射操作GPIO的方法
 * ---------------------------------------------------------------------------- 
 * @description
 * 
 * 本实验介绍了一种类似51单片机操作IO口的方法，具体原理请参考《Cortex-M3权威指南》
 * 第5章（87~92页），如果要使用GPIO的位带操作，请在源码中添加头文件"bitband.h"。
 *
 * 使用位带操作前请包含此头文件
 *
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/

#ifndef __BITBAND_H
#define __BITBAND_H	
#include "stm32f10x.h"																    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) 				((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  					*((__IO uint32_t*)(addr))

//例如：需要定义GPIOA.01作为输出IO，定义格式为： #define PA1out	GPIOout(GPIOA, 1)
//例如：需要定义GPIOA.01作为输入IO，定义格式为： #define PA1in	 GPIOin(GPIOA, 1)
#define GPIOout(GPIOx,bit)					MEM_ADDR(BITBAND((uint32_t)(&GPIOx->ODR), bit))
#define GPIOin(GPIOx,bit)					MEM_ADDR(BITBAND((uint32_t)(&GPIOx->IDR), bit))


#define CREATE_BIT_FLAG(BIT)				(0x01 << BIT)

#define SET_BIT_FLAG(DATA, FLAG)			{DATA |= FLAG;}
#define CLEAN_BIT_FLAG(DATA, FLAG)			{DATA &= ~(FLAG);}
#define CHECK_BIT_FLAG(DATA, FLAG)			(DATA & FLAG)
#define CHECK_BIT_VALUE(DATA, FLAG)			(((DATA & FLAG) > 0) ? 1 : 0)
#define CHECK_BIT_VALUE_TOGGLE(DATA, FLAG)	(((DATA & FLAG) > 0) ? 0 : 1)


#endif

