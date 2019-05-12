#include "includes.h"

#define WAKE_UP_BUTTON_RCC		RCC_APB2Periph_GPIOA
#define WAKE_UP_BUTTON_PORT		GPIOA
#define WAKE_UP_BUTTON_PIN		GPIO_Pin_0

static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* init gpio configuration */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | WAKE_UP_BUTTON_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = WAKE_UP_BUTTON_PIN;
    GPIO_Init(WAKE_UP_BUTTON_PORT, &GPIO_InitStructure);
}

void hw_key_init(void)
{
    GPIO_Configuration();
}
