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


struct sensors
{
    char Temp_buff[2]; // �¶�
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

u8  Data_buff[2048];     //���ݻ�����
u8  Data_len[5];         //���ݳ��Ȼ�����
u8  Data_id[5];          //���ݷ����ߵ�id������
uint8 nAsrStatus = 0;
u16 count = 0;

struct sensors msensor;

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

// connect tcp server
//void Connect_Tcp_Server(void)
//{
//    ESP8266_Exit_Transfer(10);//����Ƿ���͸��״̬����������˳�

//    printf("ESP8266׼����λ\r\n");

//    if(ESP8266_SoftReset(20))
//    {
//        printf("ESP8266��λʧ��\r\n");
//    }

//    printf("ESP8266��λ�ɹ�\r\n");
////    printf("ESP8266����STAģʽ\r\n");

////    if(ESP8266_SendCmd_OK((u8*)"AT+CWMODE=1", 50))
////    {
////        printf("ESP8266����STAģʽʧ��\r\n");
////    }

////    printf("ESP8266����STAģʽ�ɹ�\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CWLAP", 50))
//    {
//        printf("ESP8266ɨ��WiFi\r\n");
//    }

//    printf("ESP8266ɨ��WiFi�ɹ�\r\n");
//    printf("\r\nESP8266׼������AP!\r\n");

//    if(ESP8266_ConnectionAP((u8*)"MI9", (u8*)"woshiyuxin", 50))
//    {
//        printf("ESP8266����APʧ��\r\n");
//    }

//    printf("ESP8266����AP�ɹ�\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIFSR", 50))
//    {
//        printf("ESP8266��ѯIP��ַʧ��\r\n");    //��ѯ8266��IP����ʱʱ��5s��ʧ�ܷ���4
//    }

//    printf("ESP8266��ѯIP��ַ�ɹ�\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPMODE=1", 50))
//    {
//        printf("ESP8266����͸��ʧ��\r\n");    //����͸������ʱʱ��5s��ʧ�ܷ���5
//    }

//    printf("ESP8266����͸���ɹ�\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPMUX=0", 50))
//    {
//        printf("ESP8266���õ�·����\r\n");
//    }

//    printf("ESP8266���õ�·���ӳɹ�\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPSTART=\"TCP\",\"119.23.61.148\",5050\r\n", 50))
//    {
//        printf("ESP8266����TCP������ʧ��\r\n");
//    }

//    printf("ESP8266����TCP�������ɹ���׼������͸��\r\n");

//    if(ESP8266_SendCmd_OK((u8*)"AT+CIPMODE=1\r\n", 50))
//    {
//        printf("ESP8266����͸��ģʽʧ��\r\n");
//        return;
//    }

//    printf("ESP8266����͸��ģʽ�ɹ�\r\n");

//    USART2_DATA_FLAG = 1; //��ǽ�������͸����
//    USART2_RX_STA = 0;
//    memset(USART2_RX_BUF, 0, USART2_REC_LEN); //�������2����
//}

void Set_AP_Tcp_Server()
{
    u8 res;
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
}

/**********************************************************
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

    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);
    USART1_Config(115200);				//USART1��ʼ����������115200
    USART2_Config(115200);				// USART2��ʼ����������115200
    LD3320_Init();						// LD3320A����ʶ��ģ���ʼ��
    LED_GPIO_Config();					// LED�Ƴ�ʼ��
    Relay_GPIO_Config();				// �̵�����ʼ��
    Buzzer_GPIO_Config();				// ��������ʼ��
    DS18B20_Init();						// DS18B20�¶ȴ�������ʼ��
    OLED_Init();						// OLED��ʼ��
    OLED_DrawBMP(0, 0, 128, 8, BMP2);	// OLED��ʾ��������

    /* SD������ */
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

    Set_AP_Tcp_Server();					// ����Local������

    printf("��ʼ�����\r\n");
    delay_ms(5);
    Menu();									// ���ڴ�ӡ�˵�

    PlayDemoSound_mp3("����ϵͳ.mp3", val);
    delay_ms(600);
    OLED_DrawBMP(0, 0, 128, 8, BMP3);	// OLED��ʾ��������
    delay_ms(1000);
    OLED_DrawBMP(0, 0, 128, 8, BMP4);	// OLED��ʾ��������
    delay_ms(1000);
    OLED_DrawBMP(0, 0, 128, 8, BMP5);	// OLED��ʾ��������
    delay_ms(1000);

    Oled_Menu();							// OLED��ʾ����ӡ�˵�

    nAsrStatus = LD_ASR_NONE;				//	��ʼ״̬��û������ASR
	
    while(1)
    {
        if(bMp3Play != 0)					//	������ڲ���MP3�У���ȴ���ֱ��MP3������� ��bMp3Play==0
        {
            continue;    					//	bMp3Play �Ƕ����һ��ȫ�ֱ���������¼MP3���ŵ�״̬������LD3320оƬ�ڲ��ļĴ���
        }

        USB_OTG();

        switch(nAsrStatus)
        {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;

        case LD_ASR_NONE:
            nAsrStatus = LD_ASR_RUNING;

            if(RunASR() == 0)				//	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
            {
                nAsrStatus = LD_ASR_ERROR;
            }

            break;

        case LD_ASR_FOUNDOK:

            nAsrRes = LD_GetResult();		//	һ��ASRʶ�����̽�����ȥȡASRʶ����

            LD_Process(nAsrRes);			// ִ��ʶ������

            nAsrStatus = LD_ASR_NONE;
            break;

        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
        }//switch

        LED_Handle(nAsrRes);				// ����LED�Ƶ��¼�

        if(2000 < count)
        {
            Update_Sensors_Status(&msensor);// ��ѯ�����¸�������״̬
            count = 0;
        }

        count++;

        if(ESP8266_Get_LinkSta())			// ���ESP8266����״̬
        {
            ;
        }

        if(ESP8266_Get_Data(Data_buff, Data_len, Data_id)) // ESP8266���յ�����
        {
            printf("����id:%s��%s�ֽ����ݵ���\r\n", Data_id, Data_len);
            printf("����:%s\r\n", Data_buff);
            AT_Deal(Data_buff, strlen((char*)Data_buff)); // ����Զ������
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

// ʶ������
void LD_Process(uint8 index)
{
    int ff = 0;
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
        break;

    case  2:

        printf("���رտ����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƹ�.mp3", val);
        break;

    case  3:
        printf("�������ҵơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���ҵƿ�.mp3", val);
        break;

    case  4:
        printf("���ر����ҵơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���ҵƹ�.mp3", val);
        break;

    case  5:
        printf("���򿪳����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƿ�.mp3", val);
        break;

    case  6:
        printf("���رճ����ơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("�����ƹ�.mp3", val);
        break;

    case  7:
        printf("�������еơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���еƿ�.mp3", val);
        break;

    case  8:
        printf("���ر����еơ�����ʶ��ɹ�\r\n");
        delay_ms(5);
        PlayDemoSound_mp3("���еƹ�.mp3", val);
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
        Relay_ControlOFF();
        break;

    case 13:
        printf("����ǰ�¶ȡ�����ʶ��ɹ�\r\n");
        delay_ms(5);
//        PlayDemoSound_mp3("�¶�1.mp3", val);
//        delay_ms(1000);
//        sprintf(msensor.Temp_buff, "%2.2f\r\n", DS18B20_Get_Temp());
//        printf("%s\r\n", msensor.Temp_buff);
//        memset(msensor.Temp_buff, '\0', sizeof(msensor.Temp_buff));
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

        if(ff == 0)
        {
            PlayDemoSound_mp3("Ц��2.mp3", val);
            ff++;
        }
        else
        {
            PlayDemoSound_mp3("Ц��1.mp3", val);
            ff = 0;
        }

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

    Update_Sensors_Status(&msensor);
}
//==============================================================

//==============================================================
// �ӷ�������ȡָ�ִ��
//u8 query_cmd[] = "query+cmd";
//void Get_Command_From_Server(void)
//{
//    int length = 0, time = 0;
//    //memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //�������2����

//    u2_printf("AT+CIPSEND\r\n");

//    time = 0;

//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)">"))  //������ܵ�>��ʾָ��ɹ�,׼����������
//        {
//            break;
//        }

//        delay_ms(100);//��ʱ100ms
//        time++;
//    }

//    if(time < 50)
//    {
//        printf("ESP8266����͸��ָ��ɹ���׼����������\r\n");
//    }

//    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //�������2����
//
//	//printf("%s\r\n", query_cmd);

//    u2_printf("%s\r\n", query_cmd); // ��������

////	delay_ms(3000);
//
//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)"SEND OK"))  //�������SEND OK����ʾ���ͳɹ�
//        {
//            USART2_RX_STA = 0;
//            //memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //�������2����
//            //printf("�������ݳɹ�\r\n");
//            break;
//        }

//        if(strstr((const char *)USART2_RX_BUF, (const char *)"link is not valid"))  //�������link is not valid����ʾ���ӶϿ�
//        {
//            USART2_RX_STA = 0;
//            //memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //�������2����
//            printf("����������ӶϿ�\r\n");
//        }

//        time ++;
//    }

//    if(time)
//    {
//        printf("�������ݳɹ�\r\n");
//    }

//    if(ESP8266_Tcp_GetData(Data_buff, &length))
//    {
//		printf("Data_buff: %s\r\n",Data_buff);
//		AT_Deal(Data_buff, strlen((char *)Data_buff));
//		memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //�������2����
//    }

//}

////=============================================================
//// �������ݵ�������
//char sensor_data[1024];

//void Update_Data_To_Server(struct sensors sensor)
//{

//    int time = 0;
//    sprintf(sensor_data, "{\n\"SENSORS\":{\n\"TEMP\":\"%s\",\n\"LED1\":\"%d\",\n\"LED2\":\"%d\",\n\"LED3\":\"%d\",\n\"JDQ1\":\"%d\",\n\"FMQ1\":\"%d\"\n}\n}", sensor.Temp_buff, sensor.Led1_status, sensor.Led2_status, sensor.Led3_status, sensor.Jdq1_status, sensor.Fmq1_status);

//    //memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //�������2����

//    u2_printf("AT+CIPSEND\r\n");

//    time = 0;

//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)">"))  //������ܵ�>��ʾָ��ɹ�,׼����������
//        {
//            break;
//        }

//        delay_ms(100);//��ʱ100ms
//        time++;
//    }

//    if(time < 50)
//    {
//        printf("ESP8266����͸��ָ��ɹ���׼����������\r\n");
//    }

//    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //�������2����

//	printf("%s\r\n", sensor_data);

//    u2_printf("%s\r\n", sensor_data); // ��������

//    while(time < 50)
//    {
//        if(strstr((const char *)USART2_RX_BUF, (const char *)"SEND OK"))  //�������SEND OK����ʾ���ͳɹ�
//        {
//            USART2_RX_STA = 0;
//            memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //�������2����
//            //printf("�������ݳɹ�\r\n");
//            break;
//        }

//        if(strstr((const char *)USART2_RX_BUF, (const char *)"link is not valid"))  //�������link is not valid����ʾ���ӶϿ�
//        {
//            USART2_RX_STA = 0;
//            memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //�������2����
//            printf("����������ӶϿ�\r\n");
//        }

//        time ++;
//    }

//    if(time < 50)
//    {
//        printf("�������ݳɹ�\r\n");
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
