/***************************乐声电子科技有限公司****************************
**  文件名称：开发板测试、演示文件
**	CPU: STM32f103RCT6
**	晶振：8MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.4语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.4.11
**  说明：本程序 具备语音识别、串口通信、开发板基本功能演示。
***************************乐声电子科技有限公司******************************/
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
    delay_ms(50);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    delay_ms(50);
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    delay_ms(50);
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
    delay_ms(50);

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
    delay_ms(50);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    delay_ms(50);
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
    case 1:			/*命令“流水灯”*/
        Glide_LED();
        break;

    case 2:	 /*命令“闪烁”*/
        Flicker_LED();
        break;

    case 3:		/*命令“按键触发”*/
        Key_LED();
        break;

    case 4:		/*命令“全灭”*/
        Off_LED();
        break;
	
	case 5:		/* 开灯命令 */
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		break;
	
	case 6:		/* 关灯命令 */
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		break;

    default:
        break;
    }
}
