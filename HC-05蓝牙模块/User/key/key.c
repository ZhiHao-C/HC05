#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "key.h"

void KEY1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin=KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
}

void KEY2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin=KEY2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}


uint8_t key_scan(GPIO_TypeDef* GPIOx,uint16_t PIN)
{
	if(GPIO_ReadInputDataBit(GPIOx,PIN)==key_on)
	{
		while(GPIO_ReadInputDataBit(GPIOx,PIN)==key_on){}
		return 1;
	}
	return 0;

}
