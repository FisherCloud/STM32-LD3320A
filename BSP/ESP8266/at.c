/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��AT.c
 * ����    ��AT����ָ��ִ�кͲ�ѯ
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/
#define __AT_LOCAL__
#include "at.h"
//#include "string.h"
//#include "stdio.h"
//#include "usart2.h"
#include "includes.h"

/**
 * @file   AT_Deal
 * @brief  �жϱ��룬ִ�ж�ӦATָ��ĺ���
 * @param  cmd��ATָ��
*					len��ָ���
 * @retval ��
 */
void AT_Deal(u8 *cmd, u16 len)
{
    u8  i;
    u16 code;
    code = 0;
    //u2_printf("len=%d\r\n", len);

    switch(len) //�жϱ��룬ִ�ж�ӦATָ��ĺ���
    {
    case 8:
        for(i = 0; i < AT_8_NUM; i++) //10�ֽ�ATָ�����
        {
            if(!(memcmp(cmd, &AT_Num_8u[i][0], len))) //����ҵ��˶�Ӧ��ָ��
            {
                code = 1000 + i;
                AT_DO(code);                        //ִ����Ӧ��ATָ��
                break;
            }
        }

        break;

    case 9:
        for(i = 0; i < AT_9_NUM; i++) //10�ֽ�ATָ�����
        {
            if(!(memcmp(cmd, &AT_Num_9u[i][0], len))) //����ҵ��˶�Ӧ��ָ��
            {
                code = 1100 + i;
                AT_DO(code);                        //ִ����Ӧ��ATָ��
                break;
            }
        }

        break;
    }

    if(code == 0)
    {
        printf("ERROR\r\n");
    }
}

/**
 * @file   AT_Deal
 * @brief  ִ��ATָ���
 * @param  AT_Code����
 * @retval ��
 */
void AT_DO(u16 AT_Code)
{
    uint8_t status = 0;

    switch(AT_Code)
    {
    case 1000 :	//��ѯLED1��״̬
        status = GPIO_ReadOutputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN);
        u2_printf("%s : %d\r\n", "������", status);
        delay_ms(5);
        break;

    case 1001 :	//��ѯLED2��״̬
        status = GPIO_ReadOutputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN);
        u2_printf("%s : %d\r\n", "���ҵ�", status);
        delay_ms(5);
        break;

    case 1002 :	//��ѯLED3��״̬
        status = GPIO_ReadOutputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN);
        u2_printf("%s : %d\r\n", "������", status);
        delay_ms(5);
        break;

    case 1003 :	 ////��ѯ����LED��״̬
        status = GPIO_ReadOutputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN);
        u2_printf("%s : %d\r\n", "������", status);
        delay_ms(5);
        status = GPIO_ReadOutputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN);
        u2_printf("%s : %d\r\n", "���ҵ�", status);
        delay_ms(5);
        status = GPIO_ReadOutputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN);
        u2_printf("%s : %d\r\n", "������", status);
        delay_ms(5);
        break;

    case 1004:
        status = GPIO_ReadOutputDataBit(Relay_GPIO_PORT, Relay_GPIO_PIN);
        u2_printf("%s : %d\r\n", "�̵���", status);
        delay_ms(5);
        break;

    case 1005:
        status = GPIO_ReadOutputDataBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
        u2_printf("%s : %d\r\n", "������", status);
        delay_ms(5);
        break;

    case 1100 :
        LED_Handle(1);
        break;

    case 1101 :
        LED_Handle(2);
        break;

    case 1102 :
        LED_Handle(3);
        break;

    case 1103 :
        LED_Handle(4);
        break;

    case 1104 :
        LED_Handle(5);
        break;

    case 1105 :
        LED_Handle(6);
        break;

    case 1106 :
        LED_Handle(7);
        break;

    case 1107 :
        LED_Handle(8);
        break;

    case 1108 :
        LED_Handle(9);
        break;

    case 1109 :
        LED_Handle(10);
        break;

    case 1110:
        Relay_ControlON();
        break;

    case 1111:
        Relay_ControlOFF();
        break;

    case 1112 :
        GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
        break;

    case 1113 :
        GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
        break;
    }
}
