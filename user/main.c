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
#define val 1


struct sensors
{
    char Temp_buff[2]; // 温度
    int Led1_status;   // LED1
    int Led2_status;   // LED2
    int Led3_status;   // LED3
    int Jdq1_status;   // LED4
    int Fmq1_status;   // LED5
};

/* Private function prototypes -----------------------------------------------*/

extern uint8 RunASR(void);
extern void  LD3320_main(void);
extern void LD3320_Init(void);
extern void USB_OTG(void);

//void Get_Command_From_Server(void);
//void Update_Data_To_Server(struct sensors sensor);
void LD_Process(uint8 index);
void Update_Sensors_Status(struct sensors *sensor);

u8  Data_buff[2048];     //数据缓冲区
u8  Data_len[5];         //数据长度缓冲区
u8  Data_id[5];          //数据发送者的id缓冲区
uint8 nAsrStatus = 0;
u16 count = 0;

struct sensors msensor;

// 菜单显示函数
void Menu(void)
{
    printf("口令：\r\n");
    delay_ms(5);
    printf("0、小叮当\r\n");
    delay_ms(5);
    printf("1、打开客厅灯\r\n");
    delay_ms(5);
    printf("2、关闭客厅灯\r\n");
    delay_ms(5);
    printf("3、打开卧室灯\r\n");
    delay_ms(5);
    printf("4、关闭卧室灯\r\n");
    delay_ms(5);
    printf("5、打开厨房灯\r\n");
    delay_ms(5);
    printf("6、关闭厨房灯\r\n");
    delay_ms(5);
    printf("7、打开所有灯\r\n");
    delay_ms(5);
    printf("8、关闭所有灯\r\n");
    delay_ms(5);
    printf("9、流水灯\r\n");
    delay_ms(5);
    printf("10、闪烁\r\n");
    delay_ms(5);
    printf("11、开门\r\n");
    delay_ms(5);
    printf("12、关门\r\n");
    delay_ms(5);
    printf("13、当前温度\r\n");
    delay_ms(5);
    printf("14、播放音乐\r\n");
    delay_ms(5);
    printf("15、退下\r\n");
    delay_ms(5);
    printf("16、你会上天吗\r\n");
    delay_ms(5);
    printf("17、讲个笑话\r\n");
    delay_ms(5);
    printf("18、打开蜂鸣器\r\n");
    delay_ms(5);
    printf("19、关闭蜂鸣器\r\n");
    delay_ms(5);
}

// OLED菜单函数
void Oled_Menu(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, (u8 *)"TEMP:", 16);
    OLED_ShowString(0, 2, (u8 *)"L1:0,L2:0,L3:0", 16);
    OLED_ShowString(0, 4, (u8 *)"DOOR:0,BUZZER:0", 16);
    OLED_ShowString(0, 6, (u8 *)"BY:FisherCloud", 16);
}

// connect tcp server
//void Connect_Tcp_Server(void)
//{
//    ESP8266_Exit_Transfer(10);//检测是否在透传状态，如果在则退出

//    printf("ESP8266准备复位\r\n");

//    if(ESP8266_SoftReset(20))
//    {
//        printf("ESP8266复位失败\r\n");
//    }

//    printf("ESP8266复位成功\r\n");
////    printf("ESP8266设置STA模式\r\n");

////    if(ESP8266_SendCmd_OK((u8*)"AT+CWMODE=1", 50))
////    {
////        printf("ESP8266设置STA模式失败\r\n");
////    }

////    printf("ESP8266设置STA模式成功\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CWLAP", 50))
//    {
//        printf("ESP8266扫描WiFi\r\n");
//    }

//    printf("ESP8266扫描WiFi成功\r\n");
//    printf("\r\nESP8266准备连接AP!\r\n");

//    if(ESP8266_ConnectionAP((u8*)"MI9", (u8*)"woshiyuxin", 50))
//    {
//        printf("ESP8266连接AP失败\r\n");
//    }

//    printf("ESP8266连接AP成功\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIFSR", 50))
//    {
//        printf("ESP8266查询IP地址失败\r\n");    //查询8266的IP，超时时间5s，失败返回4
//    }

//    printf("ESP8266查询IP地址成功\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPMODE=1", 50))
//    {
//        printf("ESP8266开启透传失败\r\n");    //开启透传，超时时间5s，失败返回5
//    }

//    printf("ESP8266开启透传成功\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPMUX=0", 50))
//    {
//        printf("ESP8266设置单路连接\r\n");
//    }

//    printf("ESP8266设置单路连接成功\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPSTART=\"TCP\",\"119.23.61.148\",5050\r\n", 50))
//    {
//        printf("ESP8266连接TCP服务器失败\r\n");
//    }

//    printf("ESP8266连接TCP服务器成功，准备进入透传\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPMODE=1\r\n", 50))
//    {
//        printf("ESP8266设置透传模式失败\r\n");
//        return;
//    }

//    printf("ESP8266设置透传模式成功\r\n");

//    USART2_DATA_FLAG = 1; //标记进入数据透传了
//    USART2_RX_STA = 0;
//    memset(USART2_RX_BUF, 0, USART2_REC_LEN); //清除串口2数据
//}

void Set_AP_Tcp_Server()
{
    u8 res;
    printf("\r\nESP8266准备设置TCP服务器!\r\n");
    res = ESP8266_AP_TCPServer();

    switch(res)
    {
    case 0 :
        printf("ESP8266设置TCP服务器成功!\r\n");
        break;

    case 1 :
        printf("ESP8266软件复位失败! 准备重启系统\r\n");
        NVIC_SystemReset();
        break;

    case 2 :
        printf("ESP8266设置AP模式失败! 准备重启系统\r\n");
        NVIC_SystemReset();
        break;

    case 3 :
        printf("ESP8266设置AP参数失败!,准备重启系统\r\n");
        NVIC_SystemReset();
        break;

    case 4 :
        printf("ESP8266关闭透传失败!,准备重启系统\r\n");
        NVIC_SystemReset();
        break;

    case 5 :
        printf("ESP8266开启多路连接失败!,准备重启系统\r\n");
        NVIC_SystemReset();
        break;

    case 6 :
        printf("ESP8266设置AP模式IP失败!,准备重启系统\r\n");
        NVIC_SystemReset();
        break;

    case 7 :
        printf("ESP8266开启设立服务器失败!,准备重启系统\r\n");
        NVIC_SystemReset();
        break;
    }

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN); //清除串口2数据
}

/**********************************************************
* 名    称： 主函数
* 功    能：	程序入口
* 入口参数：    无
* 出口参数：	无
* 说    明：	无
* 调用方法： 	无
**********************************************************/
int main(void)
{
    uint8 nAsrRes = 50;

    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);
    USART1_Config(115200);				//USART1初始化，波特率115200
    USART2_Config(115200);				// USART2初始化，波特率115200
    LD3320_Init();						// LD3320A语音识别模块初始化
    LED_GPIO_Config();					// LED灯初始化
    Relay_GPIO_Config();				// 继电器初始化
    Buzzer_GPIO_Config();				// 蜂鸣器初始化
    DS18B20_Init();						// DS18B20温度传感器初始化
    OLED_Init();						// OLED初始化
    OLED_DrawBMP(0, 0, 128, 8, BMP2);	// OLED显示启动画面

    /* SD卡挂载 */
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
    delay_ms(5);
    printf("初始化......\r\n");
    delay_ms(5);

    Set_AP_Tcp_Server();					// 设置Local服务器

    printf("初始化完成\r\n");
    delay_ms(5);
    Menu();									// 串口打印菜单

    PlayDemoSound_mp3("启动系统.mp3", val);
    delay_ms(600);
    OLED_DrawBMP(0, 0, 128, 8, BMP3);	// OLED显示启动画面
    delay_ms(1000);
    OLED_DrawBMP(0, 0, 128, 8, BMP4);	// OLED显示启动画面
    delay_ms(1000);
    OLED_DrawBMP(0, 0, 128, 8, BMP5);	// OLED显示启动画面
    delay_ms(1000);

    Oled_Menu();							// OLED显示屏打印菜单

    nAsrStatus = LD_ASR_NONE;				//	初始状态：没有在作ASR
	
    while(1)
    {
        if(bMp3Play != 0)					//	如果还在播放MP3中，则等待，直到MP3播放完毕 即bMp3Play==0
        {
            continue;    					//	bMp3Play 是定义的一个全局变量用来记录MP3播放的状态，不是LD3320芯片内部的寄存器
        }

        USB_OTG();

        switch(nAsrStatus)
        {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;

        case LD_ASR_NONE:
            nAsrStatus = LD_ASR_RUNING;

            if(RunASR() == 0)				//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
            {
                nAsrStatus = LD_ASR_ERROR;
            }

            break;

        case LD_ASR_FOUNDOK:

            nAsrRes = LD_GetResult();		//	一次ASR识别流程结束，去取ASR识别结果

            LD_Process(nAsrRes);			// 执行识别处理函数

            nAsrStatus = LD_ASR_NONE;
            break;

        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
        }//switch

        LED_Handle(nAsrRes);				// 处理LED灯的事件

        if(2000 < count)
        {
            Update_Sensors_Status(&msensor);// 查询并更新各传感器状态
            count = 0;
        }

        count++;

        if(ESP8266_Get_LinkSta())			// 检查ESP8266连接状态
        {
            ;
        }

        if(ESP8266_Get_Data(Data_buff, Data_len, Data_id)) // ESP8266接收到数据
        {
            printf("连接id:%s有%s字节数据到来\r\n", Data_id, Data_len);
            printf("数据:%s\r\n", Data_buff);
            AT_Deal(Data_buff, strlen((char*)Data_buff)); // 处理远程命令
        }

        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
        {
            delay_ms(20);

            if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
            {
				Buzzer_Toggle();
				while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
            }
        }

    }// while
}

//======================================================================

// 识别处理函数
void LD_Process(uint8 index)
{
    int ff = 0;
    printf("\r\n识别码:%d\r\n", index);

    switch(index)		   /*对结果执行相关操作,客户修改*/
    {
    case  0:
        printf("“小叮当”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("上线.mp3", val);
        break;

    case  1:
        printf("“打开客厅灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("客厅灯开.mp3", val);
        break;

    case  2:

        printf("“关闭客厅灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("客厅灯关.mp3", val);
        break;

    case  3:
        printf("“打开卧室灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("卧室灯开.mp3", val);
        break;

    case  4:
        printf("“关闭卧室灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("卧室灯关.mp3", val);
        break;

    case  5:
        printf("“打开厨房灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("厨房灯开.mp3", val);
        break;

    case  6:
        printf("“关闭厨房灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("厨房灯关.mp3", val);
        break;

    case  7:
        printf("“打开所有灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("所有灯开.mp3", val);
        break;

    case  8:
        printf("“关闭所有灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("所有灯关.mp3", val);
        break;

    case  9:
        printf("“流水灯”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("流水灯.mp3", val);
        break;

    case 10:
        printf("“闪烁”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("闪烁.mp3", val);
        break;

    case 11:
        printf("“开门”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("开门.mp3", val);
        OLED_ShowChar(40, 4, '1', 16);
        Relay_ControlON();
        break;

    case 12:
        printf("“关门”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("关门.mp3", val);
        Relay_ControlOFF();
        break;

    case 13:
        printf("“当前温度”命令识别成功\r\n");
        delay_ms(5);
//        PlayDemoSound_mp3("温度1.mp3", val);
//        delay_ms(1000);
//        sprintf(msensor.Temp_buff, "%2.2f\r\n", DS18B20_Get_Temp());
//        printf("%s\r\n", msensor.Temp_buff);
//        memset(msensor.Temp_buff, '\0', sizeof(msensor.Temp_buff));
//        PlayDemoSound_mp3("温度1.mp3", val);
        break;

    case 14:
        printf("“播放音乐”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("笨小孩.mp3", val);
        break;

    case 15:
        printf("“退下”命令识别成功\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("下线.mp3", val);
        break;

    case 16:
        printf("“你会上天吗”命令识别成功\r\n");
        PlayDemoSound_mp3("不会.mp3", val);
        delay_ms(5);
        break;

    case 17:
        printf("“讲个笑话”命令识别成功\r\n");
        delay_ms(5);

        if(ff == 0)
        {
            PlayDemoSound_mp3("笑话2.mp3", val);
            ff++;
        }
        else
        {
            PlayDemoSound_mp3("笑话1.mp3", val);
            ff = 0;
        }

        break;

    case 18:
        printf("“打开警报”命令识别成功\r\n");
        delay_ms(5);
        GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
        break;

    case 19:
        printf("“关闭警报”命令识别成功\r\n");
        delay_ms(5);
        GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
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

    Update_Sensors_Status(&msensor);
}
//==============================================================

//==============================================================
// 从服务器获取指令并执行
//u8 query_cmd[] = "query+cmd";
//void Get_Command_From_Server(void)
//{
//    int length = 0, time = 0;
//    //memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //清除串口2数据

//    u2_printf("AT+CIPSEND\r\n");

//    time = 0;

//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)">"))  //如果接受到>表示指令成功,准备发送数据
//        {
//            break;
//        }

//        delay_ms(100);//延时100ms
//        time++;
//    }

//    if(time < 50)
//    {
//        printf("ESP8266设置透传指令成功，准备发送数据\r\n");
//    }

//    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //清除串口2数据
//
//	//printf("%s\r\n", query_cmd);

//    u2_printf("%s\r\n", query_cmd); // 发送数据

////	delay_ms(3000);
//
//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)"SEND OK"))  //如果接受SEND OK，表示发送成功
//        {
//            USART2_RX_STA = 0;
//            //memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //清除串口2数据
//            //printf("发送数据成功\r\n");
//            break;
//        }

//        if(strstr((const char *)USART2_RX_BUF, (const char *)"link is not valid"))  //如果接受link is not valid，表示连接断开
//        {
//            USART2_RX_STA = 0;
//            //memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //清除串口2数据
//            printf("与服务器连接断开\r\n");
//        }

//        time ++;
//    }

//    if(time)
//    {
//        printf("发送数据成功\r\n");
//    }

//    if(ESP8266_Tcp_GetData(Data_buff, &length))
//    {
//		printf("Data_buff: %s\r\n",Data_buff);
//		AT_Deal(Data_buff, strlen((char *)Data_buff));
//		memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //清除串口2数据
//    }

//}

////=============================================================
//// 更新数据到服务器
//char sensor_data[1024];

//void Update_Data_To_Server(struct sensors sensor)
//{

//    int time = 0;
//    sprintf(sensor_data, "{\n\"SENSORS\":{\n\"TEMP\":\"%s\",\n\"LED1\":\"%d\",\n\"LED2\":\"%d\",\n\"LED3\":\"%d\",\n\"JDQ1\":\"%d\",\n\"FMQ1\":\"%d\"\n}\n}", sensor.Temp_buff, sensor.Led1_status, sensor.Led2_status, sensor.Led3_status, sensor.Jdq1_status, sensor.Fmq1_status);

//    //memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //清除串口2数据

//    u2_printf("AT+CIPSEND\r\n");

//    time = 0;

//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)">"))  //如果接受到>表示指令成功,准备发送数据
//        {
//            break;
//        }

//        delay_ms(100);//延时100ms
//        time++;
//    }

//    if(time < 50)
//    {
//        printf("ESP8266设置透传指令成功，准备发送数据\r\n");
//    }

//    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //清除串口2数据

//	printf("%s\r\n", sensor_data);

//    u2_printf("%s\r\n", sensor_data); // 发送数据

//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)"SEND OK"))  //如果接受SEND OK，表示发送成功
//        {
//            USART2_RX_STA = 0;
//            memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //清除串口2数据
//            //printf("发送数据成功\r\n");
//            break;
//        }

//        if(strstr((const char *)USART2_RX_BUF, (const char *)"link is not valid"))  //如果接受link is not valid，表示连接断开
//        {
//            USART2_RX_STA = 0;
//            memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //清除串口2数据
//            printf("与服务器连接断开\r\n");
//        }

//        time ++;
//    }

//    if(time < 50)
//    {
//        printf("发送数据成功\r\n");
//    }
//}

void Update_Sensors_Status(struct sensors *sensor)
{
    memset(sensor->Temp_buff, '\0', sizeof(sensor->Temp_buff));
    sprintf(sensor->Temp_buff, "%2.2f^C", DS18B20_Get_Temp());
    OLED_ShowString(48, 0, (u8 *)sensor->Temp_buff, 16);

    if(GPIO_ReadOutputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN) == 0)
    {
        sensor->Led1_status = 1;
        OLED_ShowChar(24, 2, '1', 16);
    }
    else
    {
        sensor->Led1_status = 0;
        OLED_ShowChar(24, 2, '0', 16);
    }

    if(GPIO_ReadOutputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN) == 0)
    {
        sensor->Led2_status = 1;
        OLED_ShowChar(64, 2, '1', 16);
    }
    else
    {
        sensor->Led2_status = 0;
        OLED_ShowChar(64, 2, '0', 16);
    }

    if(GPIO_ReadOutputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN) == 0)
    {
        sensor->Led3_status = 1;
        OLED_ShowChar(104, 2, '1', 16);
    }
    else
    {
        sensor->Led3_status = 0;
        OLED_ShowChar(104, 2, '0', 16);
    }

    if(GPIO_ReadOutputDataBit(Relay_GPIO_PORT, Relay_GPIO_PIN) == 0)
    {
        sensor->Jdq1_status = 1;
        OLED_ShowChar(40, 4, '1', 16);
    }
    else
    {
        sensor->Jdq1_status = 0;
        OLED_ShowChar(40, 4, '0', 16);
    }

    if(GPIO_ReadOutputDataBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN) == 1)
    {
        sensor->Fmq1_status = 1;
        OLED_ShowChar(112, 4, '1', 16);
    }
    else
    {
        sensor->Fmq1_status = 0;
        OLED_ShowChar(112, 4, '0', 16);
    }
}
