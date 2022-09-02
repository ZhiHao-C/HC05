#ifndef __BPS_HC05_H__
#define __BPS_HC05_H__

#include "stm32f10x.h" 

//// 串口1-USART1
//#define  DEBUG_USARTx                   USART1
//#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
//#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

//#define  DEBUG_USART_IRQ                USART1_IRQn
//#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


//串口2-USART2
#define  HC05_USARTx                   USART2
#define  HC05_USART_CLK                RCC_APB1Periph_USART2
#define  HC05_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  HC05_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  HC05_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  HC05_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  HC05_USART_TX_GPIO_PORT       GPIOA   
#define  HC05_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  HC05_USART_RX_GPIO_PORT       GPIOA
#define  HC05_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  HC05_USART_IRQ                USART2_IRQn
#define  HC05_USART_IRQHandler         USART2_IRQHandler

#define  HC05_STATE_GPIO_PORT       GPIOA   
#define  HC05_STATE_GPIO_PIN        GPIO_Pin_4






void HC05_USART_Config(void);
void HC05_USART_Send_twobyte(USART_TypeDef* USARTx,uint16_t Data);
void HC05_USART_Sendbyte(USART_TypeDef* USARTx,uint8_t Data);
void HC05_USART_Sendarr(USART_TypeDef* USARTx,uint8_t*arr,uint8_t num);
void HC05_USART_Sendstr(USART_TypeDef* USARTx,uint8_t *str);

uint8_t HC05_Send_CMD(char* cmd,uint8_t clean);

#endif
