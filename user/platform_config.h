/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */

#define USB_LED_PORT                            GPIOD
#define RCC_APB2Periph_GPIO_LED                             RCC_APB2Periph_GPIOD


#if 0
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL)
//#define USE_STM3210B_EVAL
#define USE_STM3210E_EVAL
#endif
/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210B_EVAL

#define USB_DISCONNECT                    GPIOC
#define USB_DISCONNECT_PIN                GPIO_Pin_7
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOC

#define USB_LED_PORT                      GPIOC
#define RCC_APB2Periph_GPIO_LED           RCC_APB2Periph_GPIOC
#else /* USE_STM3210E_EVAL */

#define USB_DISCONNECT                    GPIOC
#define USB_DISCONNECT_PIN                GPIO_Pin_7
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOC

#define USB_LED_PORT                      GPIOC
#define RCC_APB2Periph_GPIO_LED           RCC_APB2Periph_GPIOC

#endif /* USE_STM3210B_EVAL */
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
