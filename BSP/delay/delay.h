#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32f10x.h"
void SYSTICK_Init(void);
void delay_us(u16 nus);
void delay_ms(u16 nms);
#endif
