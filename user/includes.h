/****************************************Copyright (c)****************************************************
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               includes.h
** Descriptions:            includes.h for ucos
**
*********************************************************************************************************/


#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "LDchip.h"
#include "Reg_RW.h"
#include "LEDtest.h"
#include "usart.h"
#include "delay.h"

#include "ff.h"
#include "ffconf.h"
#include "diskio.h"
#include "fatfs_posix.h"

#include "sdio_sdcard.h"

//识别码
#define CODE_LSD		1	 /*流水灯*/
#define CODE_SS	  		2	 /*闪烁*/
#define CODE_AJCF		3	 /*按键触发*/
#define CODE_QM	  		4	 /*全灭*/
#define CODE_LED1_ON	5
#define CODE_LED1_OFF	6

extern void PrintCom(USART_TypeDef* USARTx, uint8_t *Data);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
