#ifndef __DMA_H
#define	__DMA_H
#include "stm32f10x.h"

void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx, u32 cpar, u32 cmar, u16 cndtr); //����DMA1_CHx

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx, u16 len);

#endif




