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
#include "string.h"
#include "stdio.h"
#include "led.h"
#include "usart2.h"
#include "OLED_I2C.h"
#include "ds18b20.h"
#include "Relay.h"
#include "buzzer.h"

char    ALED_STA[] = {'+', 'A', 'L', 'E', 'D', ':', '0', ',', '0', ',', '0', '\r', '\n', '\0'}; //LED状态回复字符串
char    TEMP_DATA[] = {'+', 'T', 'E', 'M', 'P', ':', '3', '0', '.', '5', '5', '\r', '\n', '\0'}; //温度回复字符串
u8     TEMP_Flag;
u16    TEMP_Conter;
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
        //u2_printf("ERROR\r\n");
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
    double temperature;
    char temp_buff[5];

    switch(AT_Code)
    {
    case 1000 :	//查询LED1的状态
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
        {
            u2_printf("\r\n+LED1:0\r\nOK\r\n");
            OLED_ShowStr(30, 4, (u8 *)"0", 1);
        }
        else
        {
            u2_printf("\r\n+LED1:1\r\nOK\r\n");
            OLED_ShowStr(30, 4, (u8 *)"1", 1);
        }

        break;

    case 1001 :	//查询LED2的状态
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
        {
            u2_printf("\r\n+LED2:0\r\nOK\r\n");
            OLED_ShowStr(72, 4, (u8 *)"0,", 1);
        }
        else
        {
            u2_printf("\r\n+LED3:1\r\nOK\r\n");
            OLED_ShowStr(72, 4, (u8 *)"1,", 1);
        }

        break;

    case 1002 :	//查询LED3的状态
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
        {
            u2_printf("\r\n+LED3:0\r\nOK\r\n");
            OLED_ShowStr(114, 4, (u8 *)"0", 1);
        }
        else
        {
            u2_printf("\r\n+LED3:1\r\nOK\r\n");
            OLED_ShowStr(114, 4, (u8 *)"1", 1);
        }

        break;

    case 1003 :	 ////查询三个LED的状态
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
        {
            ALED_STA[6] = '0';
        }
        else
        {
            ALED_STA[6] = '1';
        }

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
        {
            ALED_STA[8] = '0';
        }
        else
        {
            ALED_STA[8] = '1';
        }

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
        {
            ALED_STA[10] = '0';
        }
        else
        {
            ALED_STA[10] = '1';
        }

        u2_printf("%s", ALED_STA);
        break;

    case 1100 :
        u2_printf("OK\r\n");
        LED1_ONOFF(Bit_RESET);
        OLED_ShowStr(30, 4, (u8 *)"1,", 1);
        break;

    case 1101 :
        u2_printf("OK\r\n");
        LED2_ONOFF(Bit_RESET);
        OLED_ShowStr(72, 4, (u8 *)"1,", 1);
        break;

    case 1102 :
        u2_printf("OK\r\n");
        LED3_ONOFF(Bit_RESET);
        OLED_ShowStr(114, 4, (u8 *)"1", 1);
        break;

    case 1103 :
        u2_printf("OK\r\n");
        LED1_ONOFF(Bit_SET);
        OLED_ShowStr(30, 4, (u8 *)"0", 1);
        break;

    case 1104 :
        u2_printf("OK\r\n");
        LED2_ONOFF(Bit_SET);
        OLED_ShowStr(72, 4, (u8 *)"0", 1);
        break;

    case 1105 :
        u2_printf("OK\r\n");
        LED3_ONOFF(Bit_SET);
        OLED_ShowStr(114, 4, (u8 *)"0", 1);
        break;

    case 1106 :
        u2_printf("OK\r\n");//打开所有LED
        LED1_ONOFF(Bit_RESET);
        LED2_ONOFF(Bit_RESET);
        LED3_ONOFF(Bit_RESET);
        OLED_ShowStr(0, 4, (u8 *)"LED1:1,", 1);
        OLED_ShowStr(42, 4, (u8 *)"LED2:1,", 1);
        OLED_ShowStr(84, 4, (u8 *)"LED3:1", 1);
        break;

    case 1107 :
        u2_printf("OK\r\n");//关闭所有LED
        LED1_ONOFF(Bit_SET);
        LED2_ONOFF(Bit_SET);
        LED3_ONOFF(Bit_SET);
        OLED_ShowStr(0, 4, (u8 *)"LED1:0,", 1);
        OLED_ShowStr(42, 4, (u8 *)"LED2:0,", 1);
        OLED_ShowStr(84, 4, (u8 *)"LED3:0", 1);
        break;

    case 1004 :	//查询继电器的状态
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3))
        {
            //u2_printf("\r\n+JDQ1:Close\r\nOK\r\n");
            printf("\r\n+JDQ1:Close\r\nOK\r\n");
        }
        else
        {
            //u2_printf("\r\n+JDQ1:Open\r\nOK\r\n");
            printf("\r\n+JDQ1:Open\r\nOK\r\n");
        }

        break;

    case 1108 :	//控制继电器闭合
        //u2_printf("OK\r\n");
        Relay_ControlON();
        OLED_ShowStr(36, 6, (u8 *)"1", 1);
        break;

    case 1109 :	//控制继电器断开
        //u2_printf("OK\r\n");
        Relay_ControlOFF();
        OLED_ShowStr(36, 6, (u8 *)"0", 1);
        break;

    case 1110: // 打开蜂鸣器
        BUZZER_ONOFF(Bit_SET);
        OLED_ShowStr(90, 6, (u8 *)"1", 1);
        break;

    case 1111: // 关闭蜂鸣器
        BUZZER_ONOFF(Bit_RESET);
        OLED_ShowStr(90, 6, (u8 *)"0", 1);
        break;

    case 1112 :
        //u2_printf("OK\r\n");
        TEMP_Flag = 0;
        TEMP_Conter = 0;
        break;

    case 1113 :
        TEMP_Flag = 0;
        temperature = DS18B20_Get_Temp();
        sprintf(temp_buff, "%2.2f", temperature);
        printf("当前温度=%2.2f°C\r\n", temperature);
        memcpy(&TEMP_DATA[6], temp_buff, 5);
        //u2_printf("%s", TEMP_DATA);
        OLED_ShowStr(72, 5, (u8 *)temp_buff, 1);
        break;

    case 1114:
        //u2_printf("OK\r\n");
        TEMP_Flag = 1;
        TEMP_Conter = 0;
        break;
    }
}
