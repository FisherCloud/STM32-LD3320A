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
#define val 1

/* Private function prototypes -----------------------------------------------*/

extern uint8 RunASR(void);
extern void  LD3320_main(void);
extern void LD3320_Init(void);
extern void USB_OTG(void);

void LD_Process(uint8 index);

u8  Data_buff[2048];     //���ݻ�����
u8  Data_len[5];         //���ݳ��Ȼ�����
u8  Data_id[5];          //���ݷ����ߵ�id������
uint8 nAsrStatus = 0;
char Temp_buff[2]; // �¶Ȼ�����
u16 count = 0;

// �˵���ʾ����
void Menu(void)
{
    printf("���\r\n");
    delay_ms(5);
    printf("0��С����\r\n");
    delay_ms(5);
    printf("1���򿪿�����\r\n");
    delay_ms(5);
    printf("2���رտ�����\r\n");
    delay_ms(5);
    printf("3�������ҵ�\r\n");
    delay_ms(5);
    printf("4���ر����ҵ�\r\n");
    delay_ms(5);
    printf("5���򿪳�����\r\n");
    delay_ms(5);
    printf("6���رճ�����\r\n");
    delay_ms(5);
    printf("7�������е�\r\n");
    delay_ms(5);
    printf("8���ر����е�\r\n");
    delay_ms(5);
    printf("9����ˮ��\r\n");
    delay_ms(5);
    printf("10����˸\r\n");
    delay_ms(5);
    printf("11������\r\n");
    delay_ms(5);
    printf("12������\r\n");
    delay_ms(5);
    printf("13����ǰ�¶�\r\n");
    delay_ms(5);
    printf("14����������\r\n");
    delay_ms(5);
    printf("15������\r\n");
    delay_ms(5);
    printf("16�����������\r\n");
    delay_ms(5);
    printf("17������Ц��\r\n");
    delay_ms(5);
	printf("18���򿪷�����\r\n");
    delay_ms(5);
    printf("19���رշ�����\r\n");
    delay_ms(5);
}

// OLED�˵�����
void Oled_Menu(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, (u8 *)"TEMP:", 16);
    OLED_ShowString(0, 2, (u8 *)"L1:0,L2:0,L3:0", 16);
    OLED_ShowString(0, 4, (u8 *)"DOOR:0,BUZZER:0", 16);
    OLED_ShowString(0, 6, (u8 *)"BY:FisherCloud", 16);
}

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
    uint8 nAsrRes = 50;
    uint8 res;

    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);

    USART1_Config(115200); //USART ����
    USART2_Config(115200);
    LD3320_Init();
    LED_GPIO_Config();
    Relay_GPIO_Config();
	Buzzer_GPIO_Config();
    DS18B20_Init();
    OLED_Init();
    OLED_DrawBMP(0, 0, 128, 8, BMP1);

    {
        FATFS fs;
        SD_CardInfo SDCardInfo;

        /* SD���ӿڳ�ʼ��	 */
        if(SD_Init() == SD_ERROR)
        {
            printf("SD���ӿڳ�ʼ��ʧ��\r\n");
        };

        /* �ļ�ϵͳFATFS ��ʼ�� */
        disk_initialize(0);

        /* ��SD�����ص�������0 */
        f_mount(0, &fs);
    }

    printf("���ڵ�Ƭ����������������ϵͳ\r\n");
    delay_ms(5);
    printf("��ʼ��......\r\n");
    delay_ms(5);

    printf("\r\nESP8266׼������TCP������!\r\n");
    res = ESP8266_AP_TCPServer();

    switch(res)
    {
    case 0 :
        printf("ESP8266����TCP�������ɹ�!\r\n");
        break;

    case 1 :
        printf("ESP8266�����λʧ��! ׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;

    case 2 :
        printf("ESP8266����APģʽʧ��! ׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;

    case 3 :
        printf("ESP8266����AP����ʧ��!,׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;

    case 4 :
        printf("ESP8266�ر�͸��ʧ��!,׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;

    case 5 :
        printf("ESP8266������·����ʧ��!,׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;

    case 6 :
        printf("ESP8266����APģʽIPʧ��!,׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;

    case 7 :
        printf("ESP8266��������������ʧ��!,׼������ϵͳ\r\n");
        NVIC_SystemReset();
        break;
    }

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN); //�������2����

    printf("��ʼ�����\r\n");
    delay_ms(5);
    Menu();

    PlayDemoSound_mp3("����ϵͳ.mp3", val);

    delay_ms(3500);
    Oled_Menu();

    nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR

    while(1)
    {
        if(bMp3Play != 0)			//	������ڲ���MP3�У���ȴ���ֱ��MP3������� ��bMp3Play==0
        {
            continue;    //	bMp3Play �Ƕ����һ��ȫ�ֱ���������¼MP3���ŵ�״̬������LD3320оƬ�ڲ��ļĴ���
        }

        USB_OTG();

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

        if(ESP8266_Get_LinkSta())
        {
            ;
        }

        if(ESP8266_Get_Data(Data_buff, Data_len, Data_id))
        {
            printf("��ǰ�¶�%2.2f\r\n", DS18B20_Get_Temp());
            AT_Deal(Data_buff, strlen((char *)Data_buff));
        }

        LED_Handle(nAsrRes);

        if(2000 < count)
        {
            double t =  DS18B20_Get_Temp();
            memset(Temp_buff, '\0', sizeof(Temp_buff));
            sprintf(Temp_buff, "%2.2f\r\n", t);
            //printf("%s\r\n", Temp_buff);
            OLED_ShowString(60, 0, (u8 *)Temp_buff, 16);
            count = 0;
        }

        count++;

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
        printf("��С����������ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("����.mp3", val);
        break;

    case  1:
        printf("���򿪿����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƿ�.mp3", val);
        OLED_ShowChar(24, 2, '1', 16);
        break;

    case  2:

        printf("���رտ����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƹ�.mp3", val);
        OLED_ShowChar(24, 2, '0', 16);
        break;

    case  3:
        printf("�������ҵơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���ҵƿ�.mp3", val);
        OLED_ShowChar(64, 2, '1', 16);
        break;

    case  4:
        printf("���ر����ҵơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���ҵƹ�.mp3", val);
        OLED_ShowChar(64, 2, '0', 16);
        break;

    case  5:
        printf("���򿪳����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƿ�.mp3", val);
        OLED_ShowChar(104, 2, '1', 16);
        break;

    case  6:
        printf("���رճ����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƹ�.mp3", val);
        OLED_ShowChar(104, 2, '0', 16);
        break;

    case  7:
        printf("�������еơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���еƿ�.mp3", val);
        OLED_ShowChar(24, 2, '1', 16);
        OLED_ShowChar(64, 2, '1', 16);
        OLED_ShowChar(104, 2, '1', 16);
        break;

    case  8:
        printf("���ر����еơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���еƹ�.mp3", val);
        OLED_ShowChar(24, 2, '0', 16);
        OLED_ShowChar(64, 2, '0', 16);
        OLED_ShowChar(104, 2, '0', 16);
        break;

    case  9:
        printf("����ˮ�ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("��ˮ��.mp3", val);
        break;

    case 10:
        printf("����˸������ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("��˸.mp3", val);
        break;

    case 11:
        printf("�����š�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("����.mp3", val);
        OLED_ShowChar(40, 4, '1', 16);
        Relay_ControlON();
        break;

    case 12:
        printf("�����š�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("����.mp3", val);
        OLED_ShowChar(40, 4, '0', 16);
        Relay_ControlOFF();
        break;

    case 13:
        printf("����ǰ�¶ȡ�����ʶ��ɹ�\r\n");
        delay_ms(5);
//        PlayDemoSound_mp3("�¶�1.mp3", val);
        delay_ms(1000);
        sprintf(Temp_buff, "%2.2f\r\n", DS18B20_Get_Temp());
        printf("%s\r\n", Temp_buff);
        memset(Temp_buff, '\0', sizeof(Temp_buff));
//        PlayDemoSound_mp3("�¶�1.mp3", val);
        break;

    case 14:
        printf("���������֡�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("��С��.mp3", val);
        break;

    case 15:
        printf("�����¡�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("����.mp3", val);
        break;

    case 16:
        printf("���������������ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("����.mp3", val);
        delay_ms(5);
        break;

    case 17:
        printf("������Ц��������ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("Ц��1.mp3", val);
        break;

    case 18:
		printf("���򿪾���������ʶ��ɹ�\r\n");
        delay_ms(5);
		GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
        break;

    case 19:
		printf("���رվ���������ʶ��ɹ�\r\n");
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
}
//==============================================================

