#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

#define BUZZER_GPIO_RCC         RCC_APB2Periph_GPIOC
#define BUZZER_GPIO_PORT        GPIOC
#define BUZZER_GPIO_PIN      	GPIO_Pin_5
#define BUZZER_ONOFF(x)     	GPIO_WriteBit(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN,x);

void Buzzer_GPIO_Config(void);
#endif // __BUZZER_H
