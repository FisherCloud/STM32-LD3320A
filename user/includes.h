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

#include <stm32f10x.h>
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct
{
    uint8_t count;
    enum {IDLE = 0, ACTIVE} status;
    uint8_t index;
    uint8_t Buf[255];
} UART_REV_STRUCT;

#include "LDchip.h"
#include "Reg_RW.h"
#include "LEDtest.h"
#include "dma.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "timer.h"
#include "timer3.h"
#include "user_key.h"

#include "ff.h"
#include "ffconf.h"
#include "diskio.h"
#include "fatfs_posix.h"

#include "sdio_sdcard.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

///*  Interface   */
#include "TCPinterface.h"

////ESP8266
#include "ESP8266.h"
//#include "bsp_esp8266.h"
#include "TCP_ESP8266.h"

//Lib
#include "MQTTPacket.h"
#include "cJSON.h"

//#include "net_handle.h"
//#include "mqtt_handle.h"
//#include "upload_handle.h"
//#include "bsp_heartbeat.h"

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
