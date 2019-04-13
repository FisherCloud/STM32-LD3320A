/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��delay.c
 * ����    ����શ�ʱ��ʵ��ms��ʱ��us��ʱ
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: ��
 * ���Է�ʽ��J-Link-OB
********************************************************************************/
//ͷ�ļ�

#include "delay.h"

extern u16 nTime;

/**
 * @file   SYSTICK_Init
 * @brief  ��ʼ��SYSTICK��1us�ж�1��
 * @param  ��
 * @retval ��
 */
void SYSTICK_Init(void)
{
    /*SystemCoreClock/ 1000000��1us�ж�1�Σ�SystemCoreClock/ 1000��1ms�ж�һ��*/
    while(SysTick_Config(SystemCoreClock / 1000000));
}

/**
 * @file   delay_us
 * @brief  ΢����ʱ
 * @param  ��ʱʱ��
 * @retval ��
 */
void delay_us(u16 nus)
{
    nTime = nus;

    while(nTime);
}

/**
 * @file   delay_ms
 * @brief  ������ʱ
 * @param  ��ʱʱ��
 * @retval ��
 */
void delay_ms(u16 nms)
{
    while(nms--)
    {
        delay_us(1000);
    }
}
