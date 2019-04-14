/**
  ******************************************************************************
  * @file USART/Interrupt/stm32f10x_it.c
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include <stm32f10x.h>
#include "includes.h"

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while(1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
u16 nTime;
void SysTick_Handler(void)
{
    if(nTime)
    {
        nTime--;
    }
}

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
    /* Process All SDIO Interrupt Sources */
    SD_ProcessIRQSrc();
}

/***********************************************************
* ��    �ƣ�  EXTI1_IRQHandler(void)
* ��    �ܣ� �ⲿ�жϺ���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
extern void ProcessInt0(void);
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        ProcessInt0();
        //printf("�����ж�\r\n");
        EXTI_ClearFlag(EXTI_Line1);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
    CTR_HP();
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_Istr(void);

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    USB_Istr();
}


/**
 * @file   TIM2_IRQHandler
 * @brief  ��ʱ��2�жϴ�����
 * @param  ��
 * @retval ��
 */
void TIM2_IRQHandler(void)
{
    USART2_RX_STA |= 1 << 15;
    TIM_Cmd(TIM2, DISABLE);                        //�ر�TIM3
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
}

/**
 * @file   TIM3_IRQHandler
 * @brief  ��ʱ��3�жϷ������
 * @param  ��
 * @retval ��
 */
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   //���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    //���TIMx�����жϱ�־
		// ��������Լ��Ĵ������
    }
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  */
/**
 * @file   USART1_IRQHandler
 * @brief  ����1�жϷ������
 * @param  ��
 * @retval ��
 */
void USART1_IRQHandler(void)
{
    u8 Res;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(USART1); //(USART1->DR);	//��ȡ���յ�������

        if((USART_RX_STA & 0x8000) == 0) //����δ���
        {
            if(USART_RX_STA & 0x4000) //���յ���0x0d
            {
                if(Res != 0x0a)
                {
                    USART_RX_STA = 0;    //���մ���,���¿�ʼ
                }
                else
                {
                    USART_RX_STA |= 0x8000;    //���������
                }
            }
            else //��û�յ�0X0D
            {
                if(Res == 0x0d)
                {
                    USART_RX_STA |= 0x4000;
                }
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
                    USART_RX_STA++;

                    if(USART_RX_STA > (USART_REC_LEN - 1))
                    {
                        USART_RX_STA = 0;    //�������ݴ���,���¿�ʼ����
                    }
                }
            }
        }
    }
}

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
/**
 * @file   USART2_IRQHandler
 * @brief  �жϴ�����
 * @param  ��
 * @retval ��
 */
void USART2_IRQHandler(void)        //����2�жϷ������
{
    USART2_RX_BUF[USART2_RX_STA] = USART2->DR;
    USART2_RX_STA++;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
