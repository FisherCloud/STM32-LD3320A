#ifndef __USART_H
#define __USART_H


#include "stm32f10x.h"
#include "stdio.h"

#define USART_REC_LEN  			256  	//定义最大接收字节数 256
#define EN_USART1_RX 			  1		//使能（1）/禁止（0）串口1接收

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;         		//接收状态标记

void USART1_Config(u32 bound);
#endif


