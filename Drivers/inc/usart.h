#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"

#define COMn							3							//定义STM32支持的串口数	

//定义print从哪个串口输出，如果不使用printf，则注释掉这一行
#define PRINTF_SUPPORT		UCORTEX_COM1	
#ifdef PRINTF_SUPPORT
#include <stdio.h>
#endif

#define COM_RX_LEN			128							//串口接收缓冲长度
#if COM_RX_LEN
extern uint16_t COM_RX_STA;							//串口接收状态字
extern uint8_t COM_RX_BUF[COM_RX_LEN];	//串口接收缓冲
#endif


#define USARTx_EXIST_DATA(USARTx)		(((USARTx)->SR >> 5) & 0x0001)
#define USARTx_RECV_DATA(USARTx)		((u8)((USARTx)->DR & (u8)0xFF))

#define USART1_EXIST_DATA 				(USARTx_EXIST_DATA(USART1))
#define USART1_RECV_DATA				(USARTx_RECV_DATA(USART1))

#define USART2_EXIST_DATA 				(USARTx_EXIST_DATA(USART2))
#define USART2_RECV_DATA				(USARTx_RECV_DATA(USART2))

#define USART3_EXIST_DATA 				(USARTx_EXIST_DATA(USART3))
#define USART3_RECV_DATA				(USARTx_RECV_DATA(USART3))

//#define UART4_EXIST_DATA				(USARTx_EXIST_DATA(UART4))
//#define UART4_RECV_DATA				(USARTx_RECV_DATA(UART4))

//#define UART5_EXIST_DATA				(USARTx_EXIST_DATA(UART5))
//#define UART5_RECV_DATA				(USARTx_RECV_DATA(UART5))

#define USARTx_SEND_DATA(USARTx, Data)	(USARTx->DR = (Data & (u16)0x01FF))
#define USARTx_TC(USARTx)				((USARTx->SR >> 6) & 0x0001)
#define USARTx_TXE(USARTx)				(USARTx->SR & USART_FLAG_TXE)


#define USART1_SEND_DATA(Data)			(USARTx_SEND_DATA(USART1, Data))
#define USART1_TC						(USARTx_TC(USART1))
#define USART1_TXE						(USARTx_TXE(USART1))

#define USART2_SEND_DATA(Data)			(USARTx_SEND_DATA(USART2, Data))
#define USART2_TC						(USARTx_TC(USART2))
#define USART2_TXE						(USARTx_TXE(USART2))

#define USART3_SEND_DATA(Data)			(USARTx_SEND_DATA(USART3, Data))
#define USART3_TC						(USARTx_TC(USART3))
#define USART3_TXE						(USARTx_TXE(USART3))


/**
 * @brief 定义COM PORT1，连接到USART1
 */ 
#define UCORTEX_COM1								USART1
#define UCORTEX_COM1_CLK						RCC_APB2Periph_USART1
#define UCORTEX_COM1_TX_PIN					GPIO_Pin_9
#define UCORTEX_COM1_TX_GPIO_PORT		GPIOA
#define UCORTEX_COM1_TX_GPIO_CLK		RCC_APB2Periph_GPIOA
#define UCORTEX_COM1_RX_PIN					GPIO_Pin_10
#define UCORTEX_COM1_RX_GPIO_PORT		GPIOA
#define UCORTEX_COM1_RX_GPIO_CLK		RCC_APB2Periph_GPIOA
#define UCORTEX_COM1_IRQn						USART1_IRQn

/**
 * @brief 定义COM PORT2，连接到USART2
 */ 
#define UCORTEX_COM2								USART2
#define UCORTEX_COM2_CLK						RCC_APB1Periph_USART2
#define UCORTEX_COM2_TX_PIN					GPIO_Pin_2
#define UCORTEX_COM2_TX_GPIO_PORT		GPIOA
#define UCORTEX_COM2_TX_GPIO_CLK		RCC_APB2Periph_GPIOA
#define UCORTEX_COM2_RX_PIN					GPIO_Pin_3
#define UCORTEX_COM2_RX_GPIO_PORT		GPIOA
#define UCORTEX_COM2_RX_GPIO_CLK		RCC_APB2Periph_GPIOA
#define UCORTEX_COM2_IRQn						USART2_IRQn

/**
 * @brief 定义COM PORT3，连接到USART3
 */ 
#define UCORTEX_COM3								USART3
#define UCORTEX_COM3_CLK						RCC_APB1Periph_USART3
#define UCORTEX_COM3_TX_PIN					GPIO_Pin_10
#define UCORTEX_COM3_TX_GPIO_PORT		GPIOB
#define UCORTEX_COM3_TX_GPIO_CLK		RCC_APB2Periph_GPIOB
#define UCORTEX_COM3_RX_PIN					GPIO_Pin_11
#define UCORTEX_COM3_RX_GPIO_PORT		GPIOB
#define UCORTEX_COM3_RX_GPIO_CLK		RCC_APB2Periph_GPIOB
#define UCORTEX_COM3_IRQn						USART3_IRQn

//定义串口端口
typedef enum 
{
	COM1 = 0,
	COM2 = 1,
	COM3 = 2
} COM_TypeDef;

void COM_Init(COM_TypeDef COM, uint32_t BaudRate); //串口初始化
void USART1_HANDLE(void (*sendFunc)(void), void (*recvFunc)(u8 data));

#endif

