#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"



#define BUZZER_GPIO_RCC         RCC_APB2Periph_GPIOB
#define BUZZER_GPIO_PORT        GPIOB
#define BUZZER_GPIO_PIN      	GPIO_Pin_8
#define BUZZER_ONOFF(x)     	GPIO_WriteBit(GPIOB,GPIO_Pin_8,x);

void Buzzer_GPIO_Config(void);
#endif // __BUZZER_H
