#include "usart.h"

#if COM_RX_LEN
/* COM_RX_STA: ���ڽ���״̬��
 * bit15��	������ɱ�־
 * bit14��	���յ�0x0d
 * bit13~0��	���յ�����Ч�ֽ���Ŀ
*/
uint16_t COM_RX_STA = 0;

/* ���ڽ��ջ��� */
uint8_t COM_RX_BUF[COM_RX_LEN];
#endif

/******************************* @printf  *************************************/

#ifdef PRINTF_SUPPORT //���´���,֧��printf����,������Ҫѡ��use MicroLIB
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	return x;
} 

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(PRINTF_SUPPORT, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(PRINTF_SUPPORT, USART_FLAG_TC) == RESET)
	{}
	
	return ch;
}

#endif
/********************************* @end ***************************************/

/**
  * @brief ���崮�����PORT��CLK��
  */
USART_TypeDef* COM_USART[COMn] = {UCORTEX_COM1, UCORTEX_COM2, UCORTEX_COM3}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {UCORTEX_COM1_TX_GPIO_PORT, UCORTEX_COM2_TX_GPIO_PORT, UCORTEX_COM3_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {UCORTEX_COM1_RX_GPIO_PORT, UCORTEX_COM2_RX_GPIO_PORT, UCORTEX_COM3_RX_GPIO_PORT};
 
const uint32_t COM_USART_CLK[COMn] = {UCORTEX_COM1_CLK, UCORTEX_COM2_CLK, UCORTEX_COM3_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {UCORTEX_COM1_TX_GPIO_CLK, UCORTEX_COM2_TX_GPIO_CLK, UCORTEX_COM3_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {UCORTEX_COM1_RX_GPIO_CLK, UCORTEX_COM2_RX_GPIO_CLK, UCORTEX_COM3_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {UCORTEX_COM1_TX_PIN, UCORTEX_COM2_TX_PIN, UCORTEX_COM3_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {UCORTEX_COM1_RX_PIN, UCORTEX_COM2_RX_PIN, UCORTEX_COM3_RX_PIN};

const uint8_t COM_IRQn[COMn] = {UCORTEX_COM1_IRQn, UCORTEX_COM2_IRQn, UCORTEX_COM3_IRQn};


/**
  * @brief  ��ʼ������GPIO
  * @param  COM: ָ��Ҫ��ʼ����COM PORT   
  *     @arg COM1 ����1
  *     @arg COM2 ����2
  *     @arg COM3 ����3
  * @retval None
  * @note None
  */
void COM_GPIOInit(COM_TypeDef COM)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����GPIOʱ�Ӻ͸��ù���ʱ��RCC_APB2Periph_AFIO */
	RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

	/* ���� USART Tx ����������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

	/* ���� USART Rx �������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
	GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);
}

/**
  * @brief  ���ô���
  * @param  COM: ָ��Ҫ��ʼ����COM PORT   
  *     @arg COM1 ����1
  *     @arg COM2 ����2
  *     @arg COM3 ����3
  * @param  Baudrate ���ڲ�����
  * @retval None
  * @note None
  */
void COM_Init(COM_TypeDef COM, uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	COM_GPIOInit(COM);//��ʼ������GPIO

	/* ��������ʱ�ӣ�ע�⴮��1������APB1���������£��������ڹ���APB2����������*/
	if (COM == COM1)
		RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); //��������ʱ��
	else
		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);	//��������ʱ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�������ȼ���2
	/* USARTx NVIC������Ϣ */
	NVIC_InitStructure.NVIC_IRQChannel = COM_IRQn[COM];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//���������������Ϣ��ʼ��NVIC�Ĵ���


	/*    ����������Ϣ
	  -------------
	- �����ʸ��ݲ���ָ��
	- ���ݳ���8bit
	- 1��ֹͣλ
	- ����żУ��λ
	- ��Ӳ������������(RTS �� CTS �ź�)
	- �����ڷ��ͺͽ���
	*/
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(COM_USART[COM], &USART_InitStructure);			//���ݴ���������Ϣ��ʼ������ 
	USART_ITConfig(COM_USART[COM], USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(COM_USART[COM], ENABLE);										//��������
}


void USART1_HANDLE(void (*sendFunc)(void), void (*recvFunc)(u8 data))
{
	
	if(0 != (USART1->SR & (0x01 << 6)))	
	{
		//xBitOff(UART4->SR, 6);
		USART1->SR &= ~(0x01 << 6);

		if(sendFunc)
		{
			sendFunc();
			printf("sendFunc()\r\n");
		}
	}	
	else if(0 != (USART1->SR & (0x01 << 5)))	// �ж��Ƿ�ΪRXNE�ж�
	{
		u8 recvD = USART1_RECV_DATA;
		//printf("U3D = %x\r\n", recvD);
		
		if(recvFunc)
		{
			recvFunc(recvD);
			//printf("recvFunc()\r\n");
		}
	}
	
}


