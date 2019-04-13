/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��usart2.c
 * ����    ������2�����úʹ���2�жϴ���
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: TX->PA2;RX->PA3
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/
#include "usart2.h"
#include "dma.h"
#include "timer.h"
void NVIC_Config2(void);
#if EN_USART2_RX   //���ʹ���˴���2����
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART2_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA = 0;                //����״̬���

/**
  * @file   USART2_Config
  * @brief  USART2 GPIO ����,����ģʽ����
  * @param  bound��������
  * @retval ��
  */
void USART2_Config(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2ʱ��
    USART_DeInit(USART2);  //��λ����2
    /*USART2_TX ->PA2*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
    /*USART2_RX ->PA3*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//������;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ��8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ��1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;//У��λ����
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܽ��պͷ�������
    USART_Init(USART2, &USART_InitStructure);     //��ʼ������


    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���

    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //ʹ�ܴ���2��DMA����
    MYDMA_Config(DMA1_Channel7, (u32)&USART2->DR, (u32)USART2_TX_BUF, USART2_TRS_LEN); //DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART2_TX_BUF,����USART2_TRS_LEN.
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    TIM2_Config(100 - 1, 7200 - 1); //10ms�Ķ�ʱ����
    NVIC_Config2();
    USART2_RX_STA = 0;
}
/**
 * @file   NVIC_Config2
 * @brief  Usart2 NVIC �ж���������
 * @param  ��
 * @retval ��
 */
void NVIC_Config2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
/**
 * @file   USART2_IRQHandler
 * @brief  �жϴ�����
 * @param  ��
 * @retval ��
 */
void USART2_IRQHandler(void)        //����2�жϷ������
{
    USART2_RX_BUF[USART2_RX_STA] = USART2->DR;
    USART2_RX_STA++;
}

#endif

#pragma pack(8)

u8  USART2_TX_BUF[USART2_TRS_LEN];   //����2������

/**
 * @file   u2_printf
 * @brief  ����2,printf ����,ȷ��һ�η������ݲ�����USART2_TRS_LEN�ֽ�
 * @param  ��
 * @retval ��
 */
void u2_printf(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    MYDMA_Enable(DMA1_Channel7, strlen((const char*)USART2_TX_BUF));

    while(DMA1_Channel7->CNDTR != 0);	//�ȴ�ͨ��7�������
}

#pragma pack()





