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
#include "bmp.h"

// 音量
#define val 10

/* Private function prototypes -----------------------------------------------*/

extern uint8 RunASR(void);
extern void  LD3320_main(void);
extern void LD3320_Init(void);

void LD_Process(uint8 index);

uint8 nAsrStatus = 0;

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
    uint8 nAsrRes = -1;

	
    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);

    USART1_Config(115200); //USART 配置
    USART2_Config(115200);
    LD3320_Init();
    LED_gpio_cfg();
    Relay_GPIO_Config();
    Relay_ControlON();

    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);

    TIM3_Int_Init(10, 7200 - 1); /* 72M/7200 = 10K,  10/10K = 1/1000 = 1ms */

//    {
//        FATFS fs;
//        SD_CardInfo SDCardInfo;

//        /* SD卡接口初始化	 */
//        if(SD_Init() == SD_ERROR)
//        {
//            printf("SD卡接口初始化失败\r\n");
//        };

//        /* 文件系统FATFS 初始化 */
//        disk_initialize(0);

//        /* 将SD卡挂载到驱动器0 */
//        f_mount(0, &fs);
//    }

    printf("基于单片机的智能语音控制系统\r\n");
    printf("初始化......\r\n");

//    {
//        u8 ip_buff[12];

//        printf("ESP8266初始化\r\n");

//        if(ESP8266_SoftReset(50))
//        {
//            printf("ESP8266重启失败\r\n");
//        }

//        if(!ESP8266_ConnectionAP((u8 *)AP_SSID, (u8 *)AP_PASS, 100))
//        {
//            printf("%s\r\n", (const char *)USART2_RX_BUF);
//        }

//        if(!ESP8266_GetIP(ip_buff, 50))
//        {
//            printf("%s\r\n", (const char *)USART2_RX_BUF);
//        }

//    }

    printf("初始化完成\r\n");
    printf("口令：\r\n");
    printf("0、流水灯\r\n");
    printf("1、闪烁\r\n");
    printf("2、按键触发\r\n");
    printf("3、全灭\r\n");
    printf("4、开灯\r\n");
    printf("5、关灯\r\n");

    //PlayDemoSound_mp3("启动系统.mp3", val);

    nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR

    while(1)
    {
        if(bMp3Play != 0)			//	如果还在播放MP3中，则等待，直到MP3播放完毕 即bMp3Play==0
        {
            continue;    //	bMp3Play 是定义的一个全局变量用来记录MP3播放的状态，不是LD3320芯片内部的寄存器
        }

        //USB_OTG();

        switch(nAsrStatus)
        {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;

        case LD_ASR_NONE:
            nAsrStatus = LD_ASR_RUNING;

            if(RunASR() == 0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
            {
                nAsrStatus = LD_ASR_ERROR;
            }

            break;

        case LD_ASR_FOUNDOK:

            nAsrRes = LD_GetResult();	//	一次ASR识别流程结束，去取ASR识别结果

            LD_Process(nAsrRes);

            nAsrStatus = LD_ASR_NONE;
            break;

        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
        }//switch


        Board_text(nAsrRes);

        //Mqtt Handle.
        if(ESP8266_Ping("119.23.61.148", 50) == 0)
        {
            if(Mqtt.LinkFlag == false)//Connect part
            {
                Mqtt_Connect();
            }
            else
            {
                Mqtt_DATA_handle();
                Upload_Handle();
            }
        }

    }// while

    //NVIC_SystemReset();

}

//======================================================================

// 识别处理函数
void LD_Process(uint8 index)
{
    printf("\r\n识别码:%d\r\n", index);

    switch(index)		   /*对结果执行相关操作,客户修改*/
    {
    case  0:
        printf("“流水灯”命令识别成功\r\n");
        //PlayDemoSound_mp3("开台灯.mp3", val);
        Glide_LED();
        break;

    case  1:
        printf("“闪烁”命令识别成功\r\n");
        //PlayDemoSound_mp3("开台灯.mp3", val);
        Flicker_LED();
        break;

    case  2:
        printf("“按键触发”命令识别成功\r\n");
        //PlayDemoSound_mp3("开台灯.mp3", val);
        Key_LED();
        break;

    case  3:
        printf("“全灭”命令识别成功\r\n");
        //PlayDemoSound_mp3("关灯.mp3", val);
        Off_LED();
        break;

    case  4:
        printf("“开灯”命令识别成功\r\n");
        //PlayDemoSound_mp3("开灯.mp3", val);
        GPIO_ResetBits(GPIOC, GPIO_Pin_2);
        break;

    case  5:
        printf("“关灯”命令识别成功\r\n");
        //PlayDemoSound_mp3("关灯.mp3", val);
        GPIO_SetBits(GPIOC, GPIO_Pin_2);
        break;

    case  6:
        break;

    case  7:
        break;

    case  8:
        break;

    case  9:
        break;

    case 10:
        break;

    case 11:
        break;

    case 12:
        break;

    case 13:
        break;

    case 14:
        break;

    case 15:
        break;

    case 16:
        break;

    case 17:
        break;

    case 18:
        break;

    case 19:
        break;

    case 20:
        break;

    case 21:
        break;

    case 22:
        break;

    case 23:
        break;

    case 24:
        break;

    case 25:
        break;

    case 26:
        break;

    case 27:
        break;

    case 28:
        break;

    case 29:
        break;

    case 30:
        break;

    case 31:
        break;

    case 32:
        break;

    case 33:
        break;

    case 34:
        break;

    case 35:
        break;

    case 36:
        break;

    case 37:
        break;

    case 38:
        break;

    case 39:
        break;

    case 40:
        break;

    case 41:
        break;

    case 42:
        break;

    case 43:
        break;

    case 44:
        break;

    case 45:
        break;

    case 46:
        break;

    case 47:
        break;

    case 48:
        break;

    case 49:
        break;

    default:
        break;
    }
}
//==============================================================

