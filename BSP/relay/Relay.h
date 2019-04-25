#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

#define Relay_GPIO_RCC          RCC_APB2Periph_GPIOB
#define Relay_GPIO_PORT         GPIOB
#define Relay_GPIO_PIN      	GPIO_Pin_11
#define Relay_ControlON()     	GPIO_SetBits(Relay_GPIO_PORT,Relay_GPIO_PIN)
#define Relay_ControlOFF()     	GPIO_ResetBits(Relay_GPIO_PORT,Relay_GPIO_PIN)

void Relay_GPIO_Config(void);

#endif
