#include "includes.h"

/***********************************************************
* 名    称：void LED_gpio_cfg(void)
* 功    能：LED端口配置
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void LED_gpio_cfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
}


/***********************************************************
* 名    称：void Glide_LED(void)
* 功    能： 实现流水灯功能
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void Glide_LED(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    delay_ms(200);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    delay_ms(200);
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    delay_ms(200);
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
    delay_ms(200);

}

/***********************************************************
* 名    称： void Flicker_LED(void)
* 功    能：  实现灯闪烁功能
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void Flicker_LED(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
    delay_ms(200);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    delay_ms(200);
}

/***********************************************************
* 名    称： void Key_LED(void)
* 功    能： 按键触发LED显示
* 入口参数：
* 出口参数：
* 说    明： 独立按键点亮独立LED
* 调用方法：
**********************************************************/
void Key_LED(void)
{

}


/***********************************************************
* 名    称：void Off_LED(void)
* 功    能：  关闭全部LED
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void Off_LED(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
}

/***********************************************************
* 名    称：void Board_text(uint8 Code_Val)
* 功    能：	开发板测试程序
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void Board_text(uint8 Code_Val)
{
    switch(Code_Val)		   /*对结果执行相关操作,客户修改*/
    {
    case 0:			/*命令“流水灯”*/
        Glide_LED();
        break;

    case 1:	 /*命令“闪烁”*/
        Flicker_LED();
        break;

    case 2:		/*命令“按键触发”*/
        Key_LED();
        break;

    case 3:		/*命令“全灭”*/
        Off_LED();
        break;
	
	case 4:		/* 开灯命令 */
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		break;
	
	case 5:		/* 关灯命令 */
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		break;

    default:
        break;
    }
}
