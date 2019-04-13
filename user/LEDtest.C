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
    case CODE_LSD:			/*命令“流水灯”*/
        Glide_LED();
        break;

    case CODE_SS:	 /*命令“闪烁”*/
        Flicker_LED();
        break;

    case CODE_AJCF:		/*命令“按键触发”*/
        Key_LED();
        break;

    case CODE_QM:		/*命令“全灭”*/
        Off_LED();
        break;
	
	case CODE_LED1_ON:		/* 开灯命令 */
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		PlayDemoSound_mp3("开灯.mp3", 5);
		break;
	
	case CODE_LED1_OFF:		/* 关灯命令 */
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		PlayDemoSound_mp3("关灯.mp3", 5);
		break;

    default:
        break;
    }


}
