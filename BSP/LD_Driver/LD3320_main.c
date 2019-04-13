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
 * A0				PB7
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

    printf("���ڵ�Ƭ����������������ϵͳ\r\n");
    printf("���\r\n");
    printf("1����ˮ��\r\n");
    printf("2����˸\r\n");
    printf("3����������\r\n");
    printf("4��ȫ��\r\n");
    printf("5������\r\n");
    printf("6���ص�\r\n");

    //PlayDemoSound_mp3("YS-0.5����.mp3", 5);

    nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR

    while(1)
    {
        if(bMp3Play != 0)			//	������ڲ���MP3�У���ȴ���ֱ��MP3������� ��bMp3Play==0
        {
            continue;    //	bMp3Play �Ƕ����һ��ȫ�ֱ���������¼MP3���ŵ�״̬������LD3320оƬ�ڲ��ļĴ���
        }

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
            delay_ms(100);

            printf("\r\nʶ����:%d\r\n", nAsrRes);

            switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
            {
            case CODE_LSD:			/*�����ˮ�ơ�*/
                printf("����ˮ�ơ�����ʶ��ɹ�\r\n");
                break;

            case CODE_SS:	 /*�����˸��*/
                printf("����˸������ʶ��ɹ�\r\n");
                break;

            case CODE_AJCF:		/*�������������*/

                printf("����������������ʶ��ɹ�\r\n");
                break;

            case CODE_QM:		/*���ȫ��*/

                printf("��ȫ������ʶ��ɹ�\r\n");
                break;

            case CODE_LED1_ON:		/* �������� */
                printf("�����ơ�����ʶ��ɹ�\r\n");
                break;

            case CODE_LED1_OFF:		/* �ص����� */
                printf("���صơ�����ʶ��ɹ�\r\n");
                break;

            default:
                break;
            }

            nAsrStatus = LD_ASR_NONE;
            break;

        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
        }//switch

        //�����������ʱ���֣��û���ɾ����
        Board_text(nAsrRes);
    }// while

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
    LED_gpio_cfg();

    LD_reset();
}

/***********************************************************
* ��    �ƣ� void Delay_( int i)
* ��    �ܣ� ����ʱ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void Delay_(int i)
{
    while(i--)
    {

    }
}
/***********************************************************
* ��    �ƣ�	LD3320_delay(unsigned long uldata)
* ��    �ܣ�	����ʱ����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void  LD3320_delay(unsigned long uldata)
{
    unsigned int j  =  0;
    unsigned int g  =  0;

    for(j = 0; j < 5; j++)
    {
        for(g = 0; g < uldata; g++)
        {
            Delay_(120);
        }
    }
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
        LD3320_delay(100);

        if(LD_AsrAddFixed() == 0)	//��ӹؼ����ﵽLD3320оƬ��
        {
            LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }

        LD3320_delay(10);

        if(LD_AsrRun() == 0)
        {
            LD_reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            LD3320_delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
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
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //�������NSS
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


/***********************************************************
* ��    �ƣ�  EXTI1_IRQHandler(void)
* ��    �ܣ� �ⲿ�жϺ���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        ProcessInt0();
        //printf( "�����ж�\r\n");	/*text........................*/
        EXTI_ClearFlag(EXTI_Line1);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
/***********************************************************
* ��    �ƣ�void LED_gpio_cfg(void)
* ��    �ܣ�LED�˿�����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LED_gpio_cfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
}

