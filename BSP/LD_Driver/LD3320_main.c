#include "includes.h"

/*************端口信息********************
 * 接线说明
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
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/


void LD3320_Init(void);

uint8 RunASR(void);
void ProcessInt0(void);
void LD3320_EXTI_Cfg(void);
void LD3320_Spi_cfg(void);
void LD3320_GPIO_Cfg(void);
void LED_gpio_cfg(void);
void USB_OTG(void);

/***********************************************************
* 名    称： LD3320_main(void)
* 功    能： 主函数LD3320程序入口
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/

//void  LD3320_main(void)
//{
//    uint8 nAsrRes = 0;

//    nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR

//    while(1)
//    {
//        if(bMp3Play != 0)			//	如果还在播放MP3中，则等待，直到MP3播放完毕 即bMp3Play==0
//        {
//            continue;    //	bMp3Play 是定义的一个全局变量用来记录MP3播放的状态，不是LD3320芯片内部的寄存器
//        }
//
//		USB_OTG();

//        switch(nAsrStatus)
//        {
//        case LD_ASR_RUNING:
//        case LD_ASR_ERROR:
//            break;

//        case LD_ASR_NONE:
//            nAsrStatus = LD_ASR_RUNING;

//            if(RunASR() == 0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
//            {
//                nAsrStatus = LD_ASR_ERROR;
//            }

//            break;

//        case LD_ASR_FOUNDOK:

//            nAsrRes = LD_GetResult();	//	一次ASR识别流程结束，去取ASR识别结果

//            LD_Process(nAsrRes);

//            nAsrStatus = LD_ASR_NONE;
//            break;

//        case LD_ASR_FOUNDZERO:
//        default:
//            nAsrStatus = LD_ASR_NONE;
//            break;
//        }//switch

//    }// while

//}

// U盘模式
void USB_OTG(void)
{
    if(key_user2_GETVALUE() == 0)
    {

        GPIO_Config();
        USB_Configured_LED();//设置USB接口的LED指示灯状态
        /*MAL配置 */
        MAL_Config();

        /*中断配置 */
        Interrupts_Config();

        /*USB接口初始化*/
        Set_USBClock();//设置USB接口时钟

        USB_Init();//USB接口初始化

        while(bDeviceState != CONFIGURED);

        USB_Configured_LED();//设置USB接口的LED指示灯状态

        //禁止所有外部中断.只能用U盘.
        EXTI_DeInit();

        while(1);
    }
}

/***********************************************************
* 名    称：LD3320_Init(void)
* 功    能：模块驱动端口初始配置
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void LD3320_Init(void)
{
    LD3320_GPIO_Cfg();
    LD3320_EXTI_Cfg();
    LD3320_Spi_cfg();

    LD_reset();
}

/***********************************************************
* 名    称：	RunASR(void)
* 功    能：	运行ASR
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
uint8 RunASR(void)
{
    uint8 i = 0;
    uint8 asrflag = 0;

    for(i = 0; i < 5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
    {
        LD_AsrStart();			//初始化ASR
        delay_ms(50);

        if(LD_AsrAddFixed() == 0)	//添加关键词语到LD3320芯片中
        {
            LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
            delay_ms(50);			//	并从初始化开始重新ASR识别流程
            continue;
        }

        delay_ms(10);

        if(LD_AsrRun() == 0)
        {
            LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
            delay_ms(50);			//	并从初始化开始重新ASR识别流程
            continue;
        }

        asrflag = 1;
        break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
    }

    return asrflag;
}

/***********************************************************
* 名    称：LD3320_GPIO_Cfg(void)
* 功    能：初始化需要用到的IO口
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void LD3320_GPIO_Cfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 配置PA8 输出	 8M 波形
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

        /*    MCO    configure */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);		//36M
    }
    //io口配置
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
        GPIO_SetBits(GPIOB, GPIO_Pin_7);	/*A0默认拉高*/
//		GPIO_SetBits(GPIOA,GPIO_Pin_0);	/*RD默认拉高*/
    }
}
/***********************************************************
* 名    称：LD3320_Spi_cfg(void)
* 功    能：配置SPI功能和端口初始化
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void LD3320_Spi_cfg(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //spi端口配置
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 | RCC_APB2Periph_GPIOB, ENABLE);	  //使能SPI3外设时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    //P0/P1/P2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//spis 片选	WR
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LD_CS_H();

    //spi功能配置
    SPI_Cmd(SPI3, DISABLE);
    /* SPI3 配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //时钟极性 空闲状态时，SCK保持低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //波特率控制 SYSCLK/128
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7
    SPI_Init(SPI3, &SPI_InitStructure);
    /* 使能SPI3 */
    SPI_Cmd(SPI3, ENABLE);

}
/***********************************************************
* 名    称： LD3320_EXTI_Cfg(void)
* 功    能： 外部中断功能配置和相关端口配置
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void LD3320_EXTI_Cfg(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //中断引脚配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //外部中断线配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EXTI_Line1);

    GPIO_SetBits(GPIOC, GPIO_Pin_1);	 //默认拉高中断引脚

    EXTI_ClearFlag(EXTI_Line1);
    EXTI_ClearITPendingBit(EXTI_Line1);
    //中断嵌套配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
