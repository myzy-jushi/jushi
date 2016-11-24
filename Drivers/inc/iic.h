#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"
#include "bitband.h"


typedef struct I2C_Para_Struct
{
	vu32 *I2C_SDA;
	vu32 *I2C_SCL;

	u32	I2C_Speed;
}I2C_Para_Str, *I2C_Para_Str_P;

typedef struct I2C_Struct
{
	I2C_Para_Str I2C_Para;
	
	void (*I2C_Start)(struct I2C_Struct *);
	
}I2C_Str, *I2C_Str_P;



#endif








