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

/* Private function prototypes -----------------------------------------------*/

extern void  LD3320_main(void);
extern void LD3320_Init(void);

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
    /* System Clocks Configuration */
    SYSTICK_Init();
    delay_ms(50);

    USART1_Config(115200); //USART ����
    LD3320_Init();
	LED_gpio_cfg();

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
    printf("��ʼ�����\r\n");
    printf("���\r\n");
    printf("0����ˮ��\r\n");
    printf("1����˸\r\n");
    printf("2����������\r\n");
    printf("3��ȫ��\r\n");
    printf("4������\r\n");
    printf("5���ص�\r\n");

    PlayDemoSound_mp3("����ϵͳ.mp3", 5);

    LD3320_main();		   //LD3320ִ�к���

    NVIC_SystemReset();

}
