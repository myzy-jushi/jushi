/*******************************************************************************
 * @name    : GPIOλ������ͷ�ļ�
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ����ͨ��λ��ӳ�����GPIO�ķ���
 * ---------------------------------------------------------------------------- 
 * @description
 * 
 * ��ʵ�������һ������51��Ƭ������IO�ڵķ���������ԭ����ο���Cortex-M3Ȩ��ָ�ϡ�
 * ��5�£�87~92ҳ�������Ҫʹ��GPIO��λ������������Դ�������ͷ�ļ�"bitband.h"��
 *
 * ʹ��λ������ǰ�������ͷ�ļ�
 *
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/

#ifndef __BITBAND_H
#define __BITBAND_H	
#include "stm32f10x.h"																    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) 				((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  					*((__IO uint32_t*)(addr))

//���磺��Ҫ����GPIOA.01��Ϊ���IO�������ʽΪ�� #define PA1out	GPIOout(GPIOA, 1)
//���磺��Ҫ����GPIOA.01��Ϊ����IO�������ʽΪ�� #define PA1in	 GPIOin(GPIOA, 1)
#define GPIOout(GPIOx,bit)					MEM_ADDR(BITBAND((uint32_t)(&GPIOx->ODR), bit))
#define GPIOin(GPIOx,bit)					MEM_ADDR(BITBAND((uint32_t)(&GPIOx->IDR), bit))


#define CREATE_BIT_FLAG(BIT)				(0x01 << BIT)

#define SET_BIT_FLAG(DATA, FLAG)			{DATA |= FLAG;}
#define CLEAN_BIT_FLAG(DATA, FLAG)			{DATA &= ~(FLAG);}
#define CHECK_BIT_FLAG(DATA, FLAG)			(DATA & FLAG)
#define CHECK_BIT_VALUE(DATA, FLAG)			(((DATA & FLAG) > 0) ? 1 : 0)
#define CHECK_BIT_VALUE_TOGGLE(DATA, FLAG)	(((DATA & FLAG) > 0) ? 0 : 1)


#endif

