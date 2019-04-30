/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2019,����,China.
  *                            All Rights Reserved
  *
  *                     By(�Ĵ�ʦ����ѧ�������ѧѧԺ)
  *                     https://github.com/FisherCloud
  *
  * FileName   : main.c
  * Version    : v3.0
  * Author     : ����
  * Date       : 2019-04-14
  * Description:    LD3320 STM32F103Rx ����ʶ��
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

// ����
#define val 10

/* Private function prototypes -----------------------------------------------*/

extern uint8 RunASR(void);
extern void  LD3320_main(void);
extern void LD3320_Init(void);

void LD_Process(uint8 index);

uint8 nAsrStatus = 0;

/***********************************************************
* ��    �ƣ� ������
* ��    �ܣ�	�������
* ��ڲ�����    ��
* ���ڲ�����	��
* ˵    ����	��
* ���÷����� 	��
**********************************************************/
int main(void)
{
    uint8 nAsrRes = -1;

	
    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);

    USART1_Config(115200); //USART ����
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

//        /* SD���ӿڳ�ʼ��	 */
//        if(SD_Init() == SD_ERROR)
//        {
//            printf("SD���ӿڳ�ʼ��ʧ��\r\n");
//        };

//        /* �ļ�ϵͳFATFS ��ʼ�� */
//        disk_initialize(0);

//        /* ��SD�����ص�������0 */
//        f_mount(0, &fs);
//    }

    printf("���ڵ�Ƭ����������������ϵͳ\r\n");
    printf("��ʼ��......\r\n");

//    {
//        u8 ip_buff[12];

//        printf("ESP8266��ʼ��\r\n");

//        if(ESP8266_SoftReset(50))
//        {
//            printf("ESP8266����ʧ��\r\n");
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

    printf("��ʼ�����\r\n");
    printf("���\r\n");
    printf("0����ˮ��\r\n");
    printf("1����˸\r\n");
    printf("2����������\r\n");
    printf("3��ȫ��\r\n");
    printf("4������\r\n");
    printf("5���ص�\r\n");

    //PlayDemoSound_mp3("����ϵͳ.mp3", val);

    nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR

    while(1)
    {
        if(bMp3Play != 0)			//	������ڲ���MP3�У���ȴ���ֱ��MP3������� ��bMp3Play==0
        {
            continue;    //	bMp3Play �Ƕ����һ��ȫ�ֱ���������¼MP3���ŵ�״̬������LD3320оƬ�ڲ��ļĴ���
        }

        //USB_OTG();

        switch(nAsrStatus)
        {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;

        case LD_ASR_NONE:
            nAsrStatus = LD_ASR_RUNING;

            if(RunASR() == 0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
            {
                nAsrStatus = LD_ASR_ERROR;
            }

            break;

        case LD_ASR_FOUNDOK:

            nAsrRes = LD_GetResult();	//	һ��ASRʶ�����̽�����ȥȡASRʶ����

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

// ʶ������
void LD_Process(uint8 index)
{
    printf("\r\nʶ����:%d\r\n", index);

    switch(index)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
    {
    case  0:
        printf("����ˮ�ơ�����ʶ��ɹ�\r\n");
        //PlayDemoSound_mp3("��̨��.mp3", val);
        Glide_LED();
        break;

    case  1:
        printf("����˸������ʶ��ɹ�\r\n");
        //PlayDemoSound_mp3("��̨��.mp3", val);
        Flicker_LED();
        break;

    case  2:
        printf("����������������ʶ��ɹ�\r\n");
        //PlayDemoSound_mp3("��̨��.mp3", val);
        Key_LED();
        break;

    case  3:
        printf("��ȫ������ʶ��ɹ�\r\n");
        //PlayDemoSound_mp3("�ص�.mp3", val);
        Off_LED();
        break;

    case  4:
        printf("�����ơ�����ʶ��ɹ�\r\n");
        //PlayDemoSound_mp3("����.mp3", val);
        GPIO_ResetBits(GPIOC, GPIO_Pin_2);
        break;

    case  5:
        printf("���صơ�����ʶ��ɹ�\r\n");
        //PlayDemoSound_mp3("�ص�.mp3", val);
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

