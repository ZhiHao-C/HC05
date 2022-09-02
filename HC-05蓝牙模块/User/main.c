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

void delay_ms(int n)//延时函数
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
		if(readflag==1)//接收到完整的数据帧（不定长度）
		{
			//手机输入响应命令完成相应操作
			/*
			strstr函数就是在第一个参数（一般是数组）中找第二个参数
			如果找到返回在一个参数的哪个位置如（strstr("hello world","world"则返回hello world 中world)）
      world 地址			
			没找到返回一个null
			*/
			if( strstr((char *)readbuff,"绿灯") )
			{
				led_G(1);
				led_R(0);
				led_B(0);
				clean_readbuff();//清除存在数据的数组

			}
			else if( strstr((char *)readbuff,"红灯") )
			{
				led_G(0);
				led_R(1);
				led_B(0);
				clean_readbuff();

			}
			else
			{
				HC05_USART_Sendstr(USART2,"命令错误");
				clean_readbuff();
			}
			readflag=0;
			n=0;
		}

	}

	
}


//接收触发中断函数
//实现连续接收多个字节数据
void USART2_IRQHandler(void)
{
	uint8_t temp;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		temp=USART_ReceiveData(USART2);
		if((temp != 0x0a) && (temp != 0x0d))//0x0a和0x0d为ASCII码中的回车和换行一般作为数据结尾 不需要这两个数据
		{
			readbuff[n]=temp;
			n++;
		}	
	}
	
	if(USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )  //数据帧接收完毕
	{
		readflag= 1;
		USART_ReceiveData( USART2 );     //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
	}	
	
	 USART_ClearFlag(USART2, USART_FLAG_RXNE);
}	


//定时中断函数
//作用循环扫描判断是否处于配对状态
void SysTick_Handler(void)
{
	
	x++;
	if(x>=50)
	{
		if(GPIO_ReadInputDataBit(HC05_STATE_GPIO_PORT,HC05_STATE_GPIO_PIN)==1)
		{
			printf("当前处于配对状态\n");
		}
		else
		{
			printf("当前处于未配对状态\n");
		}
		printf("state:%d\n",GPIO_ReadInputDataBit(HC05_STATE_GPIO_PORT,HC05_STATE_GPIO_PIN));
		x=0;
	}
		
}
