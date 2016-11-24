#include "iic.h"


I2C_Str IIC_Drv;
I2C_Str_P IIC_Drv_Ptr = &IIC_Drv;

//设置PB7输入输出
#define SDA_IN()  {GPIOB->CRL &= 0x0FFFFFFF; GPIOB->CRL |= 8 << 28;}
#define SDA_OUT() {GPIOB->CRL &= 0x0FFFFFFF; GPIOB->CRL |= 3 << 28;}



/****
	* @brief 产生I2C起始信号
    * @param  None
    * @retval None
    * @note
	*		请参考I2C通信协议，I2C起始信号：当SCL为高电平时，SDA由高变低
	*		如下图所示:方框部分表示I2C起始信号
	*           _____     |
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___  
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \     /
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/\___/
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_
	*          |_____|    | 
	*           start         D7   D6   D5   D4   D3   D2   D1   D0   ACK
	*/
void I2C_Start(I2C_Str_P ptr)
{
	u32 i2c_delay = ptr->I2C_Para.I2C_Speed;
	
	SDA_OUT();	//SDA设置为输出
	*(ptr->I2C_Para.I2C_SDA) = 1;	//SDA: 高
	*(ptr->I2C_Para.I2C_SCL) = 1;	//SCL: 高
	i2c_delay = ptr->I2C_Para.I2C_Speed;//延时>4.7us
	while(i2c_delay--){}
 	*(ptr->I2C_Para.I2C_SDA) = 0;	//当SCL为高电平时，SDA由高变低
	i2c_delay = ptr->I2C_Para.I2C_Speed;//延时>4us
	while(i2c_delay--){}
	*(ptr->I2C_Para.I2C_SCL) = 0;	//SCL变低，钳住I2C总线，准备发送或接收数据 
}

#if 0

/****
	* @brief 产生I2C停止信号
    * @param  None
    * @retval None
    * @note
	*		请参考I2C通信协议，I2C停止信号：当SCL为高电平时，SDA由低变高
	*		发送完STOP信号后，SCL和SDA都为高电平，即释放了I2C总线
	*		如下图所示:方框部分表示I2C停止信号
	*                                    _____
	*         ___  ___  ___  ___        |   __|_
	*   SDA: /   \/   \/   \/   \       |  /  |
	*        \___/\___/\___/\___/\______|_/   |
	*         _    _    _    _    _    _|_____|_
	*   SCL: / \__/ \__/ \__/ \__/ \__/ |     |
	*                                   |_____|
	*        D3   D2   D1   D0   ACK     stop
	*/
void I2C_Stop(void)
{
	u32 i2c_delay = i2c_speed;
	
	SDA_OUT(); 		//SDA设置为输出
	I2C_SDA = 0;	//SDA低电平
	I2C_SCL = 1;	//SCL高电平
 	i2c_delay = i2c_speed;//延时>4us
	while(i2c_delay--){}
	I2C_SDA = 1;	//STOP:当SCL为高电平时，SDA由低变高
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//延时>4.7us						   	
}

/****
	* @brief  等待ACK应答信号
    * @param  None
    * @retval 1 - 未接收到应答信号ACK；0 - 接收到应答信号ACK
    * @note
	*		请参考I2C通信协议，检测ACK应答信号：当SCL为高电平时，读取SDA为低电平
	*		如下图所示:方框部分表示I2C收到应答信号
	*                             ________     _____
	*         ___  ___  ___  ___ | _      |   |   __|_
	*   SDA: /   \/   \/   \/   \|/ \     |   |  /  |
	*        \___/\___/\___/\___/|   \____|___|_/   |
	*         _    _    _    _   | _____  |  _|_____|
	*   SCL: / \__/ \__/ \__/ \__|/     \_|_/ |     |
	*                            |________|   |_____|
	*        D3   D2   D1   D0      ACK        stop
	*/
u8 I2C_Wait_ACK(void)
{
	u32 i2c_delay = i2c_speed;
	u8 timeout = 0;
	
	SDA_IN();			//SDA设置为输入
	I2C_SDA = 1;	//SDA上拉输入
	I2C_SCL=1;	//SCL设置为高电平
	i2c_delay = i2c_speed;
	//printf("i2c_speed = %d\r\n", i2c_speed);
	while(i2c_delay--){}
	while(READ_SDA == 1)//等待ACK
	{
		if(timeout++ > 250)
		{
			I2C_Stop();
			return 1;
		}
	}
	
	I2C_SCL = 0;//钳住I2C总线：时钟信号设为低电平
	return 0;  
}


/****
	* @brief  产生ACK应答信号
    * @param  None
    * @retval None
    * @note
	*		请参考I2C通信协议，产生ACK应答信号: 在SDA为低电平时，SCL产生一个正脉冲
	*		如下图所示:方框部分表示I2C应答信号
	*                             _____     _____
	*         ___  ___  ___  ___ |     |   |   __|_
	*   SDA: /   \/   \/   \/   \|     |   |  /  |
	*        \___/\___/\___/\___/|\____|___|_/   |
	*         _    _    _    _   |  _  |  _|_____|_
	*   SCL: / \__/ \__/ \__/ \__|_/ \_|_/ |     |
	*                            |_____|   |_____|
	*        D3   D2   D1   D0     ACK      stop
	*/
void I2C_ACK(void)
{
	u32 i2c_delay = i2c_speed;
	
	I2C_SCL = 0;	//低电平
	SDA_OUT();		//设置SDA为输出
	I2C_SDA = 0;	//ACK信号
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//延时>4us
	I2C_SCL = 1;	//高电平
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//延时>4us
	I2C_SCL = 0;	//钳住I2C总线：时钟信号设为低电平
}


/****
	* @brief  产生非应答信号NACK
    * @param  None
    * @retval None
    * @note
	*		请参考I2C通信协议，产生ACK非应答信号: 在SDA为高电平时，SCL产生一个正脉冲
	*		如下图所示:方框部分表示I2C非应答信号
	*                             _____      ______
	*         ___  ___  ___  ___ | ____|_   |    __|_
	*   SDA: /   \/   \/   \/   \|/    | \  |   /  |
	*        \___/\___/\___/\___/|     |  \_|__/   |
	*         _    _    _    _   |  _  |  __|______|_
	*   SCL: / \__/ \__/ \__/ \__|_/ \_|_/  |      |
	*                            |_____|    |______|
	*        D3   D2   D1   D0    NACK        stop
	*/	    
void I2C_NACK(void)
{
	u32 i2c_delay = i2c_speed;
	
	I2C_SCL = 0;	//低电平
	SDA_OUT();		//SDA设置为输出
	I2C_SDA = 1;	//NACK信号
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//延时>4us
	I2C_SCL = 1;	//高电平
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//延时>4us
	I2C_SCL = 0;	//钳住I2C总线：时钟信号设为低电平
}


/****
	* @brief  I2C发送一个字节
    * @param  None
    * @retval None
    * @note
	*		请参考I2C通信协议，产生ACK应答信号: 在SDA为高电平时，SCL产生一个正脉冲
	*		如下图所示:方框部分表示I2C起始信号
	*
	*           _____     |<------------I2C数据发送周期------------>|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___ | _ 
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \|/ 
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/|\_
	*        __|_____|_   |   _    _    _    _    _    _    _    _  |  
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                         |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0 |
	*/
void I2C_Send_Byte(u8 data)
{                        
	u8 i = 0;
	u32 i2c_delay = i2c_speed;

	SDA_OUT();									//SDA设为输出
	I2C_SCL = 0;								//钳住I2C总线：SCL设为低电平
	for(i = 0; i < 8; i++)
	{
		if(data&0x80)I2C_SDA = 1;	//高位先传
		else I2C_SDA = 0;
		
		i2c_delay = i2c_speed;
		while(i2c_delay--){}			//延时>4us
	
		I2C_SCL = 1;							//在SCL上产生一个正脉冲
		i2c_delay = i2c_speed;
		while(i2c_delay--){}			//延时>4us
			
		I2C_SCL=0;
		i2c_delay = i2c_speed/3;
		while(i2c_delay--){}			//延时>1us
		data <<= 1;								//右移一位
	}
}


/****
	* @brief  从I2C读取一个字节
    * @param  ack : 0 - NACK; 1 - ACK
    * @retval 接收到的数据
    * @note
	*		请参考I2C通信协议，产生ACK应答信号: 在SDA为高电平时，SCL产生一个正脉冲
	*		如下图所示:方框部分表示I2C起始信号
	*
	*           _____     |<------------I2C数据读取周期(ACK)------------>|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___      |
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \     |
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/\____|_
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _  |
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                              |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0   ACK
	*
	*           _____     |<------------I2C数据读取周期(NACK)----------->|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___  ____|_
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \/    |
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/     |
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _  |
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                              |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0  NACK
	*/
u8 I2C_Read_Byte(u8 ack)
{
	u8 i, receive = 0x00;
	u32 i2c_delay = i2c_speed;
	
	I2C_SCL = 0;									//SCL低电平
	SDA_IN();											//SDA设置为输入
	for(i = 0; i < 8; i++)
	{
		i2c_delay = i2c_speed;
		while(i2c_delay--);
		I2C_SCL = 1;								//高电平
		i2c_delay = i2c_speed;
		while(i2c_delay--);
		receive <<= 1;
		if(READ_SDA) receive |= 1;	//高位在前
		I2C_SCL = 0;
	}
	if (ack == 0) I2C_NACK();			//发送NACK
	else I2C_ACK();								//发送ACK
	
	return receive;								//返回接收到的数据
}

/**
  * @brief  设置I2C速度
  * @param  speed : I2C速度，单位Kbps
  * @retval 返回设置前的I2C速度
	* @note   I2C速度设置范围是: 1Kbps ~ 400Kbps
  */
u16 I2C_SetSpeed(u16 speed)
{
	u16 temp;
	
	//I2C速度必须小于400Kbps，大于 1Kbps
	if((speed > 400)|| (speed < 1)) return 0;
	
	temp = I2C_SPEED_1K / i2c_speed;	//备份原来的i2c速度
	i2c_speed = I2C_SPEED_1K / speed;	//设置新的i2c速度

	return temp;	//返回设置前的i2c速度
}




#endif


void I2C_Init(void)
{
	IIC_Drv_Ptr->IIC_Start = I2C_Start;
}


