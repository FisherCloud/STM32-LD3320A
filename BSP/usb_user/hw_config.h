/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define BULK_MAX_PACKET_SIZE  0x00000040

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void Interrupts_Config(void);
void GPIO_Config(void);
void Led_RW_ON(void);
void Led_RW_OFF(void);
void USB_Configured_LED(void);
void USB_NotConfigured_LED(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_Disconnect_Config(void);
void Get_SerialNum(void);
void MAL_Config(void);
/* External variables --------------------------------------------------------*/

#define RCC_GPIO_LED                                 RCC_APB2Periph_GPIOC      /*LED使用的GPIO时钟*/
#define GPIO_LED_PORT                                GPIOC                     /*LED数量*/
#define LEDn                                         4            /*LED灯使用的GPIO组*/
#define DS1_PIN                                      GPIO_Pin_3    /*DS1使用的GPIO管脚*/
//#define DS2_PIN                                      GPIO_Pin_7    /*DS2使用的GPIO管脚*/
//#define DS3_PIN                                      GPIO_Pin_8    /*DS3使用的GPIO管脚*/
//#define DS4_PIN                                      GPIO_Pin_9    /*DS4使用的GPIO管脚*/
//#define GPIO_LED_ALL                                 DS1_PIN |DS2_PIN |DS3_PIN |DS4_PIN 


/* SD卡检测 */
#define GPIO_SD_CTRL_PORT                          GPIOD
#define GPIO_SD_NCD                                GPIO_Pin_13   /* input: SD CARD DETECT   ACTIVE 0   */
#define RCC_SD_CTRL                                RCC_APB2Periph_GPIOD

/*USB相关定义*/
#define USB_DISCONNECT            			GPIOC  
#define USB_DISCONNECT_PIN        			GPIO_Pin_7
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOC


#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
