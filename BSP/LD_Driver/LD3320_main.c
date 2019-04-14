/***************************�������ӿƼ����޹�˾****************************
**  �������ƣ�YS-V0.4����ʶ�𿪷������
**	CPU: STM32f103RCT6
**	����8MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��YS-V0.4����ʶ�𿪷���
**                http://yuesheng001.taobao.com
**  ���ߣ�zdings
**  ��ϵ��751956552@qq.com
**  �޸����ڣ�2012.4.11
**  ˵���������� �߱�����ʶ�𡢴���ͨ�š����������������ʾ��
***************************�������ӿƼ����޹�˾******************************/
#include "includes.h"

/*************�˿���Ϣ********************
 * ����˵��
 * RST      PB6
 * CS   	PB8
 * WR/SPIS  PB9
 * P2/SDCK  PB3
 * P1/SDO   PB4
 * P0/SDI   PB5
 * IRQ      PC1
 * A0		PB7
 * RD       PA0
*****************************************/

/************************************************************************************
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
*********************************************************************************/
uint8 nAsrStatus = 0;

void LD3320_Init(void);

uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void LED_gpio_cfg(void);
void LD_Process(uint8 index);
void USB_OTG();

//======================================================================
// ����
#define val 5
// ʶ��������
void LD_Process(uint8 index)
{
    printf("\r\nʶ����:%d\r\n", index);

    switch(index)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
    {
    case  0:
        printf("����ˮ�ơ�����ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("��̨��.mp3", val);
        Glide_LED();
        break;

    case  1:
        printf("����˸������ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("��̨��.mp3", val);
        Flicker_LED();
        break;

    case  2:
        printf("����������������ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("��̨��.mp3", val);
        Key_LED();
        break;

    case  3:
        printf("��ȫ������ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("�ص�.mp3", val);
        Off_LED();
        break;

    case  4:
        printf("�����ơ�����ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("����.mp3", val);
        GPIO_ResetBits(GPIOC, GPIO_Pin_2);
        break;

    case  5:
        printf("���صơ�����ʶ��ɹ�\r\n");
        PlayDemoSound_mp3("�ص�.mp3", val);
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

/***********************************************************
* ��    �ƣ� LD3320_main(void)
* ��    �ܣ� ������LD3320�������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/

void  LD3320_main(void)
{
    uint8 nAsrRes = 0;

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

            if(RunASR() == 0)	//	����һ��ASRʶ�����̣�ASR��ʼ����ASR���ӹؼ��������ASR����
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

    }// while

}

// U��ģʽ
void USB_OTG()
{
    if(key_user2_GETVALUE() == 0)
    {

        GPIO_Config();
        USB_Configured_LED();//����USB�ӿڵ�LEDָʾ��״̬
        /*MAL���� */
        MAL_Config();

        /*�ж����� */
        Interrupts_Config();

        /*USB�ӿڳ�ʼ��*/
        Set_USBClock();//����USB�ӿ�ʱ��

        USB_Init();//USB�ӿڳ�ʼ��

        while(bDeviceState != CONFIGURED);

        USB_Configured_LED();//����USB�ӿڵ�LEDָʾ��״̬

        //��ֹ�����ⲿ�ж�.ֻ����U��.
        EXTI_DeInit();

        while(1);
    }
}

/***********************************************************
* ��    �ƣ�LD3320_Init(void)
* ��    �ܣ�ģ�������˿ڳ�ʼ����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_Init(void)
{
    LD3320_GPIO_Cfg();
    LD3320_EXTI_Cfg();
    LD3320_Spi_cfg();

    LD_reset();
}

/***********************************************************
* ��    �ƣ�	RunASR(void)
* ��    �ܣ�	����ASR
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
uint8 RunASR(void)
{
    uint8 i = 0;
    uint8 asrflag = 0;

    for(i = 0; i < 5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
    {
        LD_AsrStart();			//��ʼ��ASR
        delay_ms(50);

        if(LD_AsrAddFixed() == 0)	//���ӹؼ����ﵽLD3320оƬ��
        {
            LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            delay_ms(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }

        delay_ms(10);

        if(LD_AsrRun() == 0)
        {
            LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            delay_ms(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }

        asrflag = 1;
        break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
    }

    return asrflag;
}

/***********************************************************
* ��    �ƣ�LD3320_GPIO_Cfg(void)
* ��    �ܣ���ʼ����Ҫ�õ���IO��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_GPIO_Cfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // ����PA8 ���	 8M ����
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

        /*    MCO    configure */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);		//36M
    }
    //io������
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
        //LD_CS	/RSET
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6; //;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
//		//LD_RD
//		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_SetBits(GPIOB, GPIO_Pin_7);	/*A0Ĭ������*/
//		GPIO_SetBits(GPIOA,GPIO_Pin_0);	/*RDĬ������*/
    }
}
/***********************************************************
* ��    �ƣ�LD3320_Spi_cfg(void)
* ��    �ܣ�����SPI���ܺͶ˿ڳ�ʼ��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_Spi_cfg(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //spi�˿�����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 | RCC_APB2Periph_GPIOB, ENABLE);	  //ʹ��SPI3����ʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    //P0/P1/P2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//spis Ƭѡ	WR
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LD_CS_H();

    //spi��������
    SPI_Cmd(SPI3, DISABLE);
    /* SPI3 ���� */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //��������NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //�����ʿ��� SYSCLK/128
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7
    SPI_Init(SPI3, &SPI_InitStructure);
    /* ʹ��SPI3 */
    SPI_Cmd(SPI3, ENABLE);

}
/***********************************************************
* ��    �ƣ� LD3320_EXTI_Cfg(void)
* ��    �ܣ� �ⲿ�жϹ������ú���ض˿�����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD3320_EXTI_Cfg(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //�ж���������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //�ⲿ�ж�������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EXTI_Line1);

    GPIO_SetBits(GPIOC, GPIO_Pin_1);	 //Ĭ�������ж�����

    EXTI_ClearFlag(EXTI_Line1);
    EXTI_ClearITPendingBit(EXTI_Line1);
    //�ж�Ƕ������
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}