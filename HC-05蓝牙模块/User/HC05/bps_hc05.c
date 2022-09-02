#include "bps_hc05.h"
#include "bps_usart.h"
#include <stdio.h>
#include <string.h>

extern void clean_readbuff(void);
extern void delay_ms(int n);
extern uint8_t readbuff[500];
extern uint16_t n;
extern uint8_t readflag;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = HC05_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}







void HC05_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(HC05_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(HC05_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = HC05_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HC05_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = HC05_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HC05_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//将state引脚配置为浮空输入
	GPIO_InitStructure.GPIO_Pin = HC05_STATE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HC05_STATE_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = HC05_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(HC05_USARTx, &USART_InitStructure);

	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(HC05_USARTx, USART_IT_RXNE, ENABLE);	
	
	USART_ITConfig(HC05_USARTx, USART_IT_IDLE, ENABLE);	
	// 使能串口
	USART_Cmd(HC05_USARTx, ENABLE);	 

}

//发送一个字节
void HC05_USART_Sendbyte(USART_TypeDef* USARTx,uint8_t Data)
{
	USART_SendData(HC05_USARTx,Data);
	while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));//判断发送数据寄存器是否将数据发送到发送移位寄存器
}

//发送两个字节
void HC05_USART_Send_twobyte(USART_TypeDef* USARTx,uint16_t Data)
{
	uint8_t temp_h,temp_l;
	temp_h=Data>>8;
	temp_l=Data;
	USART_SendData(HC05_USARTx,temp_h);
	while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));
	USART_SendData(HC05_USARTx,temp_l);
	while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));
}

//发送一个数组
void HC05_USART_Sendarr(USART_TypeDef* USARTx,uint8_t*arr,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		USART_SendData(HC05_USARTx,arr[i]);
		while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));
	}
}

//发送一个字符串（可以是中文）
void HC05_USART_Sendstr(USART_TypeDef* USARTx,uint8_t *str)
{
	while ((*str)!=0)
	{
		USART_SendData(HC05_USARTx,*str);
		str++;
		while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));
	}
	delay_ms(10);
}




//该函数功能未实现（尝试发送AT指令发现没有返回值）有待修改
//uint8_t HC05_Send_CMD(char* cmd,uint8_t clean)
//{
//	int time=500;
//	HC05_USART_Sendstr(HC05_USARTx,(uint8_t *)cmd);
//	while(USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TC)==RESET && time>0)
//	{
//		int i;
//		while(time--)
//		{
//			for(i=0;i<5520;i++);
//		}
//	}
//	delay_ms(100);
//	if(strstr((char *)readbuff,"ok"))
//	{
//		printf("OK\n");
//	}
//	else
//	{
//		printf("未检测到响应\n");
//	}
//	if(time<=0)
//	{
//		return 0;
//	}
//	
//	if(clean==1)
//	{
//		clean_readbuff();
//	}
//	readflag=0;
//	n=0;
//	return 1;
//}
