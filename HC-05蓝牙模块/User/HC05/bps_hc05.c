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
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = HC05_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}







void HC05_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(HC05_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	RCC_APB1PeriphClockCmd(HC05_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = HC05_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HC05_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = HC05_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HC05_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//��state��������Ϊ��������
	GPIO_InitStructure.GPIO_Pin = HC05_STATE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HC05_STATE_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = HC05_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(HC05_USARTx, &USART_InitStructure);

	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(HC05_USARTx, USART_IT_RXNE, ENABLE);	
	
	USART_ITConfig(HC05_USARTx, USART_IT_IDLE, ENABLE);	
	// ʹ�ܴ���
	USART_Cmd(HC05_USARTx, ENABLE);	 

}

//����һ���ֽ�
void HC05_USART_Sendbyte(USART_TypeDef* USARTx,uint8_t Data)
{
	USART_SendData(HC05_USARTx,Data);
	while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));//�жϷ������ݼĴ����Ƿ����ݷ��͵�������λ�Ĵ���
}

//���������ֽ�
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

//����һ������
void HC05_USART_Sendarr(USART_TypeDef* USARTx,uint8_t*arr,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		USART_SendData(HC05_USARTx,arr[i]);
		while (!USART_GetFlagStatus(HC05_USARTx,USART_FLAG_TXE));
	}
}

//����һ���ַ��������������ģ�
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




//�ú�������δʵ�֣����Է���ATָ���û�з���ֵ���д��޸�
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
//		printf("δ��⵽��Ӧ\n");
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
