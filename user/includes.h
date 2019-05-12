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

#include "oled.h"
#include "Relay.h"
#include "led.h"
#include "buzzer.h"
#include "ds18b20.h"

#include "sys.h"
#include "LDchip.h"
#include "Reg_RW.h"
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

/////*  Interface   */
//#include "TCPinterface.h"

////ESP8266
#include "ESP8266.h"
#include "at.h"
//#include "bsp_esp8266.h"
//#include "TCP_ESP8266.h"

//Lib
//#include "mqtt.h"
//#include "MQTTPacket.h"
//#include "handle.h"
//#include "cJSON.h"



#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
