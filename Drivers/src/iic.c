#include "iic.h"


I2C_Str IIC_Drv;
I2C_Str_P IIC_Drv_Ptr = &IIC_Drv;

//����PB7�������
#define SDA_IN()  {GPIOB->CRL &= 0x0FFFFFFF; GPIOB->CRL |= 8 << 28;}
#define SDA_OUT() {GPIOB->CRL &= 0x0FFFFFFF; GPIOB->CRL |= 3 << 28;}



/****
	* @brief ����I2C��ʼ�ź�
    * @param  None
    * @retval None
    * @note
	*		��ο�I2Cͨ��Э�飬I2C��ʼ�źţ���SCLΪ�ߵ�ƽʱ��SDA�ɸ߱��
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
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
	
	SDA_OUT();	//SDA����Ϊ���
	*(ptr->I2C_Para.I2C_SDA) = 1;	//SDA: ��
	*(ptr->I2C_Para.I2C_SCL) = 1;	//SCL: ��
	i2c_delay = ptr->I2C_Para.I2C_Speed;//��ʱ>4.7us
	while(i2c_delay--){}
 	*(ptr->I2C_Para.I2C_SDA) = 0;	//��SCLΪ�ߵ�ƽʱ��SDA�ɸ߱��
	i2c_delay = ptr->I2C_Para.I2C_Speed;//��ʱ>4us
	while(i2c_delay--){}
	*(ptr->I2C_Para.I2C_SCL) = 0;	//SCL��ͣ�ǯסI2C���ߣ�׼�����ͻ�������� 
}

#if 0

/****
	* @brief ����I2Cֹͣ�ź�
    * @param  None
    * @retval None
    * @note
	*		��ο�I2Cͨ��Э�飬I2Cֹͣ�źţ���SCLΪ�ߵ�ƽʱ��SDA�ɵͱ��
	*		������STOP�źź�SCL��SDA��Ϊ�ߵ�ƽ�����ͷ���I2C����
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2Cֹͣ�ź�
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
	
	SDA_OUT(); 		//SDA����Ϊ���
	I2C_SDA = 0;	//SDA�͵�ƽ
	I2C_SCL = 1;	//SCL�ߵ�ƽ
 	i2c_delay = i2c_speed;//��ʱ>4us
	while(i2c_delay--){}
	I2C_SDA = 1;	//STOP:��SCLΪ�ߵ�ƽʱ��SDA�ɵͱ��
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4.7us						   	
}

/****
	* @brief  �ȴ�ACKӦ���ź�
    * @param  None
    * @retval 1 - δ���յ�Ӧ���ź�ACK��0 - ���յ�Ӧ���ź�ACK
    * @note
	*		��ο�I2Cͨ��Э�飬���ACKӦ���źţ���SCLΪ�ߵ�ƽʱ����ȡSDAΪ�͵�ƽ
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C�յ�Ӧ���ź�
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
	
	SDA_IN();			//SDA����Ϊ����
	I2C_SDA = 1;	//SDA��������
	I2C_SCL=1;	//SCL����Ϊ�ߵ�ƽ
	i2c_delay = i2c_speed;
	//printf("i2c_speed = %d\r\n", i2c_speed);
	while(i2c_delay--){}
	while(READ_SDA == 1)//�ȴ�ACK
	{
		if(timeout++ > 250)
		{
			I2C_Stop();
			return 1;
		}
	}
	
	I2C_SCL = 0;//ǯסI2C���ߣ�ʱ���ź���Ϊ�͵�ƽ
	return 0;  
}


/****
	* @brief  ����ACKӦ���ź�
    * @param  None
    * @retval None
    * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�͵�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2CӦ���ź�
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
	
	I2C_SCL = 0;	//�͵�ƽ
	SDA_OUT();		//����SDAΪ���
	I2C_SDA = 0;	//ACK�ź�
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL = 1;	//�ߵ�ƽ
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL = 0;	//ǯסI2C���ߣ�ʱ���ź���Ϊ�͵�ƽ
}


/****
	* @brief  ������Ӧ���ź�NACK
    * @param  None
    * @retval None
    * @note
	*		��ο�I2Cͨ��Э�飬����ACK��Ӧ���ź�: ��SDAΪ�ߵ�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��Ӧ���ź�
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
	
	I2C_SCL = 0;	//�͵�ƽ
	SDA_OUT();		//SDA����Ϊ���
	I2C_SDA = 1;	//NACK�ź�
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL = 1;	//�ߵ�ƽ
	i2c_delay = i2c_speed;
	while(i2c_delay--){}//��ʱ>4us
	I2C_SCL = 0;	//ǯסI2C���ߣ�ʱ���ź���Ϊ�͵�ƽ
}


/****
	* @brief  I2C����һ���ֽ�
    * @param  None
    * @retval None
    * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�ߵ�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*
	*           _____     |<------------I2C���ݷ�������------------>|
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

	SDA_OUT();									//SDA��Ϊ���
	I2C_SCL = 0;								//ǯסI2C���ߣ�SCL��Ϊ�͵�ƽ
	for(i = 0; i < 8; i++)
	{
		if(data&0x80)I2C_SDA = 1;	//��λ�ȴ�
		else I2C_SDA = 0;
		
		i2c_delay = i2c_speed;
		while(i2c_delay--){}			//��ʱ>4us
	
		I2C_SCL = 1;							//��SCL�ϲ���һ��������
		i2c_delay = i2c_speed;
		while(i2c_delay--){}			//��ʱ>4us
			
		I2C_SCL=0;
		i2c_delay = i2c_speed/3;
		while(i2c_delay--){}			//��ʱ>1us
		data <<= 1;								//����һλ
	}
}


/****
	* @brief  ��I2C��ȡһ���ֽ�
    * @param  ack : 0 - NACK; 1 - ACK
    * @retval ���յ�������
    * @note
	*		��ο�I2Cͨ��Э�飬����ACKӦ���ź�: ��SDAΪ�ߵ�ƽʱ��SCL����һ��������
	*		����ͼ��ʾ:���򲿷ֱ�ʾI2C��ʼ�ź�
	*
	*           _____     |<------------I2C���ݶ�ȡ����(ACK)------------>|
	*        __|__   |    |  ___  ___  ___  ___  ___  ___  ___  ___      |
	*   SDA:   |  \__|____|_/   \/   \/   \/   \/   \/   \/   \/   \     |
	*          |     |    | \___/\___/\___/\___/\___/\___/\___/\___/\____|_
	*        __|_____|_   |   _    _    _    _    _    _    _    _    _  |
	*   SCL:   |     | \__|__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \__/ \_|_
	*          |_____|    |                                              |
	*           start     |   D7   D6   D5   D4   D3   D2   D1   D0   ACK
	*
	*           _____     |<------------I2C���ݶ�ȡ����(NACK)----------->|
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
	
	I2C_SCL = 0;									//SCL�͵�ƽ
	SDA_IN();											//SDA����Ϊ����
	for(i = 0; i < 8; i++)
	{
		i2c_delay = i2c_speed;
		while(i2c_delay--);
		I2C_SCL = 1;								//�ߵ�ƽ
		i2c_delay = i2c_speed;
		while(i2c_delay--);
		receive <<= 1;
		if(READ_SDA) receive |= 1;	//��λ��ǰ
		I2C_SCL = 0;
	}
	if (ack == 0) I2C_NACK();			//����NACK
	else I2C_ACK();								//����ACK
	
	return receive;								//���ؽ��յ�������
}

/**
  * @brief  ����I2C�ٶ�
  * @param  speed : I2C�ٶȣ���λKbps
  * @retval ��������ǰ��I2C�ٶ�
	* @note   I2C�ٶ����÷�Χ��: 1Kbps ~ 400Kbps
  */
u16 I2C_SetSpeed(u16 speed)
{
	u16 temp;
	
	//I2C�ٶȱ���С��400Kbps������ 1Kbps
	if((speed > 400)|| (speed < 1)) return 0;
	
	temp = I2C_SPEED_1K / i2c_speed;	//����ԭ����i2c�ٶ�
	i2c_speed = I2C_SPEED_1K / speed;	//�����µ�i2c�ٶ�

	return temp;	//��������ǰ��i2c�ٶ�
}




#endif


void I2C_Init(void)
{
	IIC_Drv_Ptr->IIC_Start = I2C_Start;
}


