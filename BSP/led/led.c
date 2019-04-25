/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��led.c
 * ����    ��LED����������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: D1->PC13;D2->PB0;D3->PB1
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

//ͷ�ļ�
#include "led.h"

/**
 * @file   LED_GPIO_Config
 * @brief  LED����������
 * @param  ��
 * @retval ��
 */
void LED_GPIO_Config(void)
{
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED1_GPIO_RCC | LED2_GPIO_RCC | LED3_GPIO_RCC, ENABLE); //ʹ��GPIO������ʱ��
    /*D1*/
    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN; //ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
    /*D2*/
    GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
    /*D3*/
    GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

/**
 * @file   LEDXToggle
 * @brief  LED����ת
 * @param  ledx:1--PC13;2--PB0;3--PB1
 * @retval ��
 */
void LEDXToggle(uint8_t ledx)
{
    if(ledx == 1)
    {
        LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN;
    }
    else if(ledx == 2)
    {
        LED2_GPIO_PORT->ODR ^= LED2_GPIO_PIN;
    }
    else
    {
        LED3_GPIO_PORT->ODR ^= LED3_GPIO_PIN;
    }


}

