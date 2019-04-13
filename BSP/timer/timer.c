/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��timer.c
 * ����    ��ͨ�ö�ʱ��2�жϳ�ʼ��
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: ��
 * ���Է�ʽ��J-Link-OB
********************************************************************************/
#include "timer.h"
//#include "led.h"
#include "usart2.h"
void TimerNvic_Config(void);
/**
 * @file   TimerNvic_Config
 * @brief  �ж����ȼ�NVIC����
 * @param  ��
 * @retval ��
 */
void TimerNvic_Config(void)
{
    NVIC_InitTypeDef	NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 		//�����ж���Ϊ0
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 		//�ж��¼�ΪTIM2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //��Ӧ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //�����ж�
    NVIC_Init(&NVIC_InitStructure);                         //����ָ��������ʼ���жϼĴ���
}
/**
 * @file   TIM2_Config
 * @brief  ��ʼ����ʱ��2
 * @param
*					arr���Զ���װֵ��
*					psc��ʱ��Ԥ��Ƶ��
 * @retval ��
 */
void TIM2_Config(u16 arr, u16 psc)
{
    //��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ����ʱ���ṹ�����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
    TIM_DeInit(TIM2); //��IM2��ʱ����ʼ��λ��λֵ
    TIM_InternalClockConfig(TIM2);//���� TIM2 �ڲ�ʱ��
    //��ʱ��TIM2��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ����ؼĴ���ֵΪ���ֵ	0~65535֮��
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //�Զ���Ԥ��Ƶϵ�����ṩ����ʱ����ʱ��0~65535֮��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ��������ʼ��TIMʱ������Ĵ���
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���жϣ�����һ�����жϺ����������ж�
    TIM_ARRPreloadConfig(TIM2, DISABLE);//��ֹARR Ԥװ�ػ�����

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //ʹ��TIM2�ж�Դ
    TIM_Cmd(TIM2, DISABLE);        //�ر�TIM2
    TimerNvic_Config();	//�ж����ȼ�NVIC����
}


/**
 * @file   TIM2_IRQHandler
 * @brief  ��ʱ��2�жϴ�����
 * @param  ��
 * @retval ��
 */
void TIM2_IRQHandler(void)
{
    USART2_RX_STA |= 1 << 15;
    TIM_Cmd(TIM2, DISABLE);                        //�ر�TIM3
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
}












