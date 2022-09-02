#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "bps_usart.h"
#include "bps_hc05.h"
#include "bps_systick.h"
#include <stdio.h>
#include <string.h>

uint16_t x;
uint8_t readbuff[500];
uint16_t n;
uint8_t readflag;

void delay_ms(int n)//��ʱ����
{
	int i;
	while (n--)
	{
		for(i=0;i<5520;i++);
	}
}


void clean_readbuff(void)
{
	static uint16_t i=0;
	for(i=0;i<500;i++)
	{
		readbuff[i]=0;
	}
}




int main()
{
	HC05_USART_Config();
	USART_Config();
	LED_GPIO_Config();
	Systick_Init();
	
	
	while (1)
	{
		if(readflag==1)//���յ�����������֡���������ȣ�
		{
			//�ֻ�������Ӧ���������Ӧ����
			/*
			strstr���������ڵ�һ��������һ�������飩���ҵڶ�������
			����ҵ�������һ���������ĸ�λ���磨strstr("hello world","world"�򷵻�hello world ��world)��
      world ��ַ			
			û�ҵ�����һ��null
			*/
			if( strstr((char *)readbuff,"�̵�") )
			{
				led_G(1);
				led_R(0);
				led_B(0);
				clean_readbuff();//����������ݵ�����

			}
			else if( strstr((char *)readbuff,"���") )
			{
				led_G(0);
				led_R(1);
				led_B(0);
				clean_readbuff();

			}
			else
			{
				HC05_USART_Sendstr(USART2,"�������");
				clean_readbuff();
			}
			readflag=0;
			n=0;
		}

	}

	
}


//���մ����жϺ���
//ʵ���������ն���ֽ�����
void USART2_IRQHandler(void)
{
	uint8_t temp;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		temp=USART_ReceiveData(USART2);
		if((temp != 0x0a) && (temp != 0x0d))//0x0a��0x0dΪASCII���еĻس��ͻ���һ����Ϊ���ݽ�β ����Ҫ����������
		{
			readbuff[n]=temp;
			n++;
		}	
	}
	
	if(USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )  //����֡�������
	{
		readflag= 1;
		USART_ReceiveData( USART2 );     //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)	
	}	
	
	 USART_ClearFlag(USART2, USART_FLAG_RXNE);
}	


//��ʱ�жϺ���
//����ѭ��ɨ���ж��Ƿ������״̬
void SysTick_Handler(void)
{
	
	x++;
	if(x>=50)
	{
		if(GPIO_ReadInputDataBit(HC05_STATE_GPIO_PORT,HC05_STATE_GPIO_PIN)==1)
		{
			printf("��ǰ�������״̬\n");
		}
		else
		{
			printf("��ǰ����δ���״̬\n");
		}
		printf("state:%d\n",GPIO_ReadInputDataBit(HC05_STATE_GPIO_PORT,HC05_STATE_GPIO_PIN));
		x=0;
	}
		
}
