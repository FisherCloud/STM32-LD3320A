/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：usart2.c
 * 描述    ：串口2的配置和串口2中断处理
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: TX->PA2;RX->PA3
 * 调试方式：J-Link-OB
**********************************************************************************/
#include "usart2.h"
#include "dma.h"
#include "timer.h"
void NVIC_Config2(void);
#if EN_USART2_RX   //如果使能了串口2接收
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART2_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14~0，	接收到的有效字节数目
u16 USART2_RX_STA = 0;                //接收状态标记

/**
  * @file   USART2_Config
  * @brief  USART2 GPIO 配置,工作模式配置
  * @param  bound：波特率
  * @retval 无
  */
void USART2_Config(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2时钟
    USART_DeInit(USART2);  //复位串口2
    /*USART2_TX ->PA2*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
    /*USART2_RX ->PA3*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位：8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位：1位
    USART_InitStructure.USART_Parity = USART_Parity_No;//校验位：无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能接收和发送引脚
    USART_Init(USART2, &USART_InitStructure);     //初始化串口


    USART_Cmd(USART2, ENABLE);                    //使能串口

    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //使能串口2的DMA发送
    MYDMA_Config(DMA1_Channel7, (u32)&USART2->DR, (u32)USART2_TX_BUF, USART2_TRS_LEN); //DMA1通道7,外设为串口2,存储器为USART2_TX_BUF,长度USART2_TRS_LEN.
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    TIM2_Config(100 - 1, 7200 - 1); //10ms的定时周期
    NVIC_Config2();
    USART2_RX_STA = 0;
}
/**
 * @file   NVIC_Config2
 * @brief  Usart2 NVIC 中断向量配置
 * @param  无
 * @retval 无
 */
void NVIC_Config2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
/**
 * @file   USART2_IRQHandler
 * @brief  中断处理函数
 * @param  无
 * @retval 无
 */
void USART2_IRQHandler(void)        //串口2中断服务程序
{
    USART2_RX_BUF[USART2_RX_STA] = USART2->DR;
    USART2_RX_STA++;
}

#endif

#pragma pack(8)

u8  USART2_TX_BUF[USART2_TRS_LEN];   //串口2发送区

/**
 * @file   u2_printf
 * @brief  串口2,printf 函数,确保一次发送数据不超过USART2_TRS_LEN字节
 * @param  无
 * @retval 无
 */
void u2_printf(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    MYDMA_Enable(DMA1_Channel7, strlen((const char*)USART2_TX_BUF));

    while(DMA1_Channel7->CNDTR != 0);	//等待通道7传输完成
}

#pragma pack()





