/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：timer.c
 * 描述    ：通用定时器2中断初始化
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: 无
 * 调试方式：J-Link-OB
********************************************************************************/
#include "timer.h"
//#include "led.h"
#include "usart2.h"
void TimerNvic_Config(void);
/**
 * @file   TimerNvic_Config
 * @brief  中断优先级NVIC设置
 * @param  无
 * @retval 无
 */
void TimerNvic_Config(void)
{
    NVIC_InitTypeDef	NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//设置中断组为0
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//中断事件为TIM2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //响应优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //允许中断
    NVIC_Init(&NVIC_InitStructure);                         //根据指定参数初始化中断寄存器
}
/**
 * @file   TIM2_Config
 * @brief  初始化定时器2
 * @param
*					arr：自动重装值。
*					psc：时钟预分频数
 * @retval 无
 */
void TIM2_Config(u16 arr, u16 psc)
{
    //定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个定时器结构体变量
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
    TIM_DeInit(TIM2); //将IM2定时器初始化位复位值
    TIM_InternalClockConfig(TIM2);//配置 TIM2 内部时钟
    //定时器TIM2初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重载寄存器值为最大值	0~65535之间
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //自定义预分频系数，提供给定时器的时钟0~65535之间
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定参数初始化TIM时间基数寄存器
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清中断，以免一启用中断后立即产生中断
    TIM_ARRPreloadConfig(TIM2, DISABLE);//禁止ARR 预装载缓冲器

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能TIM2中断源
    TIM_Cmd(TIM2, DISABLE);        //关闭TIM2
    TimerNvic_Config();	//中断优先级NVIC设置
}


/**
 * @file   TIM2_IRQHandler
 * @brief  定时器2中断处理函数
 * @param  无
 * @retval 无
 */
void TIM2_IRQHandler(void)
{
    USART2_RX_STA |= 1 << 15;
    TIM_Cmd(TIM2, DISABLE);                        //关闭TIM3
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
}












