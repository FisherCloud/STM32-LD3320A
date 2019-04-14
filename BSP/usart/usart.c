/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：usart.c
 * 描述    ：串口1的配置和串口1中断处理
 * 作者    ：zhuoyingxingyu
 * 版本更新: 2016-04-08
 * 硬件连接: TX->PA9;RX->PA10
 * 调试方式：J-Link-OB
**********************************************************************************/

#include "usart.h"

//函数声明
void NVIC_Config1(void);


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕

    USART1->DR = (u8) ch;
    return ch;
}
#endif

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN];  //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15,接收完成标志
//bit14,接收到0x0d
//bit13~0,接收到的有效字节数目
u16 USART_RX_STA = 0; //接收状态标记

/**
 * @file   USART1_Config
 * @brief  USART1 GPIO 配置,工作模式配置
 * @param  bound：波特率
 * @retval 无
 */
void USART1_Config(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
    USART_DeInit(USART1);  //复位串口1
    /*USART1_TX ->PA9*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位：8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位：1位
    USART_InitStructure.USART_Parity = USART_Parity_No;//校验位：无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能接收和发送引脚
    /*将以上赋完值的结构体带入库函数USART_Init进行初始化*/
    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口中断接收
    USART_Cmd(USART1, ENABLE);                    //使能串口

    NVIC_Config1();// Usart1 NVIC 中断向量配置
}

/**
 * @file   NVIC_Config1
 * @brief  Usart1 NVIC 中断向量配置
 * @param  无
 * @retval 无
 */
void NVIC_Config1(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


#endif
