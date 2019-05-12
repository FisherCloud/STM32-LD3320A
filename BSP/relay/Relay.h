#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

#define Relay_GPIO_RCC          RCC_APB2Periph_GPIOC
#define Relay_GPIO_PORT         GPIOC
#define Relay_GPIO_PIN      	GPIO_Pin_6
#define Relay_ControlON()     	GPIO_ResetBits(Relay_GPIO_PORT,Relay_GPIO_PIN)
#define Relay_ControlOFF()     	GPIO_SetBits(Relay_GPIO_PORT,Relay_GPIO_PIN)

void Relay_GPIO_Config(void);

#endif

