/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2019,余鑫,China.
  *                            All Rights Reserved
  *                              
  *                     By(四川师范大学计算机科学学院)
  *                     https://github.com/FisherCloud
  *    
  * FileName   : main.c   
  * Version    : v3.0		
  * Author     : 余鑫			
  * Date       : 2019-04-14         
  * Description:    LD3320 STM32F103Rx 语音识别
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */
#include "includes.h"

/* Private function prototypes -----------------------------------------------*/

extern void  LD3320_main(void);
extern void LD3320_Init(void);

/***********************************************************
* 名    称： 主函数
* 功    能：	程序入口
* 入口参数：    无
* 出口参数：	无
* 说    明：	无
* 调用方法： 	无
**********************************************************/
int main(void)
{
    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);

    USART1_Config(115200); //USART 配置
    LD3320_Init();
	LED_gpio_cfg();

    {
        FATFS fs;
        SD_CardInfo SDCardInfo;

        /* SD卡接口初始化	 */
        if(SD_Init() == SD_ERROR)
        {
            printf("SD卡接口初始化失败\r\n");
        };

        /* 文件系统FATFS 初始化 */
        disk_initialize(0);

        /* 将SD卡挂载到驱动器0 */
        f_mount(0, &fs);
    }

    printf("基于单片机的智能语音控制系统\r\n");
    printf("初始化完成\r\n");
    printf("口令：\r\n");
    printf("0、流水灯\r\n");
    printf("1、闪烁\r\n");
    printf("2、按键触发\r\n");
    printf("3、全灭\r\n");
    printf("4、开灯\r\n");
    printf("5、关灯\r\n");

    PlayDemoSound_mp3("启动系统.mp3", 5);

    LD3320_main();		   //LD3320执行函数

    NVIC_SystemReset();

}
