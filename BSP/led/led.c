/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：led.c
 * 描述    ：LED灯引脚配置
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: D1->PC13;D2->PB0;D3->PB1
 * 调试方式：J-Link-OB
**********************************************************************************/

//头文件
#include "led.h"

/**
 * @file   LED_GPIO_Config
 * @brief  LED灯引脚配置
 * @param  无
 * @retval 无
 */
void LED_GPIO_Config(void)
{
    //定义一个GPIO_InitTypeDef 类型的结构体
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED1_GPIO_RCC | LED2_GPIO_RCC | LED3_GPIO_RCC, ENABLE); //使能GPIO的外设时钟
    /*D1*/
    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN; //选择要用的GPIO引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置引脚模式为推免输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置引脚速度为50MHZ
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);//调用库函数，初始化GPIO
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
 * @brief  LED打开
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval 无
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
 * @brief  LED关闭
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval 无
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
 * @brief  LED亮灭翻转
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval 无
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
 * @brief  LED流水灯
 * @param  ledx:1--PC2;2--PC3;3--PC4
 * @retval 无
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
 * @brief  LED闪烁
 * @param
 * @retval 无
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
* @brief  LED全部打开
* @param
* @retval 无
*/
void On_LED(void)
{
    LEDX_On(1);
    LEDX_On(2);
    LEDX_On(3);
}

/**
* @file   Off_LED
* @brief  LED全部关闭
* @param
* @retval 无
*/
void Off_LED(void)
{
    LEDX_Off(1);
    LEDX_Off(2);
    LEDX_Off(3);
}

/**
* @file   LED_Handle
* @brief  LED处理函数
* @param
* @retval 无
*/
void LED_Handle(uint8_t code)
{
    switch(code)
    {
    case 1: // 打开客厅灯
        LEDX_On(1);
        break;

    case 2: // 关闭客厅灯
        LEDX_Off(1);
        break;

    case 3: // 打开卧室灯
        LEDX_On(2);
        break;

    case 4: // 关闭卧室灯
        LEDX_Off(2);
        break;

    case 5: // 打开厨房灯
        LEDX_On(3);
        break;

    case 6: // 关闭厨房灯
        LEDX_Off(3);
        break;

    case 7: // 打开所有灯
        On_LED();
        break;

    case 8: // 关闭所有灯
        Off_LED();
        break;

    case 9: // 流水灯
        Glide_LED();
        break;

    case 10: // 闪烁
        Flicker_LED();
        break;

    default:
        // none
        break;
    }

}
