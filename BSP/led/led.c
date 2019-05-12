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
 * @file   LEDX_Set
 * @brief  LED��
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval ��
 */
void LEDX_On(uint8_t ledx)
{
    switch(ledx)
    {
    case 1:
        GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
        break;

    case 2:
        GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
        break;

    case 3:
        GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
        break;

    default:
        // none
        break;
    }
}

/**
 * @file   LEDX_Off
 * @brief  LED�ر�
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval ��
 */
void LEDX_Off(uint8_t ledx)
{
    switch(ledx)
    {
    case 1:
        GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
        break;

    case 2:
        GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
        break;

    case 3:
        GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
        break;

    default:
        // none
        break;
    }
}

/**
 * @file   LEDXToggle
 * @brief  LED����ת
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval ��
 */
void LEDXToggle(uint8_t ledx)
{
    switch(ledx)
    {
    case 1:
        LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN;
        break;

    case 2:
        LED2_GPIO_PORT->ODR ^= LED2_GPIO_PIN;
        break;

    case 3:
        LED3_GPIO_PORT->ODR ^= LED3_GPIO_PIN;
        break;

    default:
        // none
        break;
    }
}

/**
 * @file   Glide_LED
 * @brief  LED��ˮ��
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval ��
 */
void Glide_LED(void)
{
    LEDXToggle(1);
    delay_ms(200);
    LEDXToggle(2);
    delay_ms(200);
    LEDXToggle(3);
    delay_ms(200);
}

/**
 * @file   Flicker_LED
 * @brief  LED��˸
 * @param
 * @retval ��
 */
void Flicker_LED(void)
{
    LEDXToggle(1);
    LEDXToggle(2);
    LEDXToggle(3);
    delay_ms(200);
}

/**
* @file   On_LED
* @brief  LEDȫ����
* @param
* @retval ��
*/
void On_LED(void)
{
    LEDX_On(1);
    LEDX_On(2);
    LEDX_On(3);
}

/**
* @file   Off_LED
* @brief  LEDȫ���ر�
* @param
* @retval ��
*/
void Off_LED(void)
{
    LEDX_Off(1);
    LEDX_Off(2);
    LEDX_Off(3);
}

/**
* @file   LED_Handle
* @brief  LED������
* @param
* @retval ��
*/
void LED_Handle(uint8_t code)
{
    switch(code)
    {
    case 1: // �򿪿�����
        LEDX_On(1);
        break;

    case 2: // �رտ�����
        LEDX_Off(1);
        break;

    case 3: // �����ҵ�
        LEDX_On(2);
        break;

    case 4: // �ر����ҵ�
        LEDX_Off(2);
        break;

    case 5: // �򿪳�����
        LEDX_On(3);
        break;

    case 6: // �رճ�����
        LEDX_Off(3);
        break;

    case 7: // �����е�
        On_LED();
        break;

    case 8: // �ر����е�
        Off_LED();
        break;

    case 9: // ��ˮ��
        Glide_LED();
        break;

    case 10: // ��˸
        Flicker_LED();
        break;

    default:
        // none
        break;
    }

}
