//头文件
#include "buzzer.h"


/**
 * @file   Buzzer_GPIO_Config
 * @brief  Buzzer灯引脚配置
 * @param  无
 * @retval 无
 */
void Buzzer_GPIO_Config(void)
{

    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUZZER_GPIO_RCC, ENABLE); //使能GPIO的外设时钟
    /*BUZZER*/
    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN; //选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO

    GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
}

void Buzzer_Toggle(void)
{
	BUZZER_GPIO_PORT->ODR ^= BUZZER_GPIO_PIN;
}
