#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "delay.h"

#define LED1_GPIO_RCC           	RCC_APB2Periph_GPIOC
#define LED1_GPIO_PORT          	GPIOC
#define LED1_GPIO_PIN      			GPIO_Pin_2
#define LED1_ONOFF(x)     			GPIO_WriteBit(LED1_GPIO_PORT,LED1_GPIO_PIN,x);

#define LED2_GPIO_RCC           	RCC_APB2Periph_GPIOC
#define LED2_GPIO_PORT          	GPIOC
#define LED2_GPIO_PIN      			GPIO_Pin_3
#define LED2_ONOFF(x)     			GPIO_WriteBit(LED2_GPIO_PORT,LED2_GPIO_PIN,x);

#define LED3_GPIO_RCC           	RCC_APB2Periph_GPIOC
#define LED3_GPIO_PORT          	GPIOC
#define LED3_GPIO_PIN      			GPIO_Pin_4
#define LED3_ONOFF(x)      			GPIO_WriteBit(LED3_GPIO_PORT,LED3_GPIO_PIN,x);

void LED_GPIO_Config(void);
void LEDX_On(uint8_t ledx);
void LEDX_Off(uint8_t ledx);
void LEDXToggle(uint8_t ledx);
void Glide_LED(void);
void Flicker_LED(void);
void On_LED(void);
void Off_LED(void);
void LED_Handle(uint8_t code);
#endif
