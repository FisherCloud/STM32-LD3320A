/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：delay.c
 * 描述    ：嘀嗒定时器实现ms延时，us延时
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: 无
 * 调试方式：J-Link-OB
********************************************************************************/
//头文件

#include "delay.h"

extern u16 nTime;

/**
 * @file   SYSTICK_Init
 * @brief  初始化SYSTICK，1us中断1次
 * @param  无
 * @retval 无
 */
void SYSTICK_Init(void)
{
    /*SystemCoreClock/ 1000000：1us中断1次；SystemCoreClock/ 1000：1ms中断一次*/
    while(SysTick_Config(SystemCoreClock / 1000000));
}

/**
 * @file   delay_us
 * @brief  微妙延时
 * @param  延时时间
 * @retval 无
 */
void delay_us(u16 nus)
{
    nTime = nus;

    while(nTime);
}

/**
 * @file   delay_ms
 * @brief  毫秒延时
 * @param  延时时间
 * @retval 无
 */
void delay_ms(u16 nms)
{
    while(nms--)
    {
        delay_us(1000);
    }
}
