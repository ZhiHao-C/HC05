#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h"  

#define KEY1_GPIO_PIN        GPIO_Pin_0
#define KEY1_GPIO_PORT       GPIOA
#define KEY1_GPIO_CLK       RCC_APB2Periph_GPIOA

#define KEY2_GPIO_PIN        GPIO_Pin_13
#define KEY2_GPIO_PORT       GPIOC
#define KEY2_GPIO_CLK       RCC_APB2Periph_GPIOC

#define key_on        1
#define key_off       0



void KEY1_GPIO_Config(void);
void KEY2_GPIO_Config(void);
uint8_t key_scan(GPIO_TypeDef* GPIOx,uint16_t PIN);

#endif

