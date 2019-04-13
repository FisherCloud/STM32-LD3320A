/***************************乐声电子科技有限公司****************************
**  工程名称：YS-V0.4语音识别开发板程序
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

/* Private function prototypes -----------------------------------------------*/

extern void  LD3320_main(void);
extern void LD3320_Init(void);

/***********************************************************
* 名    称： 主函数
* 功    能：	程序入口
* 入口参数：  无
* 出口参数：	无
* 说    明：	无
* 调用方法： 	无
**********************************************************/
int main(void)
{
    /* System Clocks Configuration */
	SYSTICK_Init();
	delay_ms(500);
    
	USART1_Config(115200); //USART 配置
	
	LD3320_Init();
	
//	{
//        FATFS fs;
//        SD_CardInfo SDCardInfo;

//        /* SD卡接口初始化	 */
//        SD_Init();
//        /* 获取SD卡信息 */
//        SD_GetCardInfo(&SDCardInfo);
//        SD_SelectDeselect((uint32_t)(SDCardInfo.RCA << 16));
//        /* 设置SDIO接口数据宽度 */
//        SD_EnableWideBusOperation(SDIO_BusWide_4b);
//        /* 设置工作模式 */
//        SD_SetDeviceMode(SD_DMA_MODE);
//        /* 文件系统FATFS 初始化 */
//        disk_initialize(0);
//        /* 将SD卡挂载到驱动器0 */
//        f_mount(0, &fs);
//	}
	
	printf("初始化完成\r\n");
	
    LD3320_main();		   //LD3320执行函数

    NVIC_SystemReset();

}
