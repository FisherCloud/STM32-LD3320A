/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：AT.c
 * 描述    ：AT控制指令执行和查询
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 调试方式：J-Link-OB
**********************************************************************************/
#define __AT_LOCAL__
#include "at.h"
//#include "string.h"
//#include "stdio.h"
//#include "usart2.h"
#include "includes.h"

/**
 * @file   AT_Deal
 * @brief  判断编码，执行对应AT指令的函数
 * @param  cmd：AT指令
*					len：指令长度
 * @retval 无
 */
void AT_Deal(u8 *cmd, u16 len)
{
    u8  i;
    u16 code;
    code = 0;
    //u2_printf("len=%d\r\n", len);

    switch(len) //判断编码，执行对应AT指令的函数
    {
    case 8:
        for(i = 0; i < AT_8_NUM; i++) //10字节AT指令查找
        {
            if(!(memcmp(cmd, &AT_Num_8u[i][0], len))) //如果找到了对应的指令
            {
                code = 1000 + i;
                AT_DO(code);                        //执行相应的AT指令
                break;
            }
        }

        break;

    case 9:
        for(i = 0; i < AT_9_NUM; i++) //10字节AT指令查找
        {
            if(!(memcmp(cmd, &AT_Num_9u[i][0], len))) //如果找到了对应的指令
            {
                code = 1100 + i;
                AT_DO(code);                        //执行相应的AT指令
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
 * @brief  执行AT指令函数
 * @param  AT_Code编码
 * @retval 无
 */
void AT_DO(u16 AT_Code)
{
    uint8_t status = 0;

    switch(AT_Code)
    {
    case 1000 :	//查询LED1的状态
        status = GPIO_ReadOutputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN);
        u2_printf("%s : %d\r\n", "客厅灯", status);
        delay_ms(5);
        break;

    case 1001 :	//查询LED2的状态
        status = GPIO_ReadOutputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN);
        u2_printf("%s : %d\r\n", "卧室灯", status);
        delay_ms(5);
        break;

    case 1002 :	//查询LED3的状态
        status = GPIO_ReadOutputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN);
        u2_printf("%s : %d\r\n", "厨房灯", status);
        delay_ms(5);
        break;

    case 1003 :	 ////查询三个LED的状态
        status = GPIO_ReadOutputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN);
        u2_printf("%s : %d\r\n", "客厅灯", status);
        delay_ms(5);
        status = GPIO_ReadOutputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN);
        u2_printf("%s : %d\r\n", "卧室灯", status);
        delay_ms(5);
        status = GPIO_ReadOutputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN);
        u2_printf("%s : %d\r\n", "厨房灯", status);
        delay_ms(5);
        break;

    case 1004:
        status = GPIO_ReadOutputDataBit(Relay_GPIO_PORT, Relay_GPIO_PIN);
        u2_printf("%s : %d\r\n", "继电器", status);
        delay_ms(5);
        break;

    case 1005:
        status = GPIO_ReadOutputDataBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
        u2_printf("%s : %d\r\n", "蜂鸣器", status);
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
