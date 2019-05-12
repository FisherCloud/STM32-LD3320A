//ͷ�ļ�
#include "buzzer.h"


/**
 * @file   Buzzer_GPIO_Config
 * @brief  Buzzer����������
 * @param  ��
 * @retval ��
 */
void Buzzer_GPIO_Config(void)
{

    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BUZZER_GPIO_RCC, ENABLE); //ʹ��GPIO������ʱ��
    /*BUZZER*/
    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN; //ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO

    GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
}

void Buzzer_Toggle(void)
{
	BUZZER_GPIO_PORT->ODR ^= BUZZER_GPIO_PIN;
}
