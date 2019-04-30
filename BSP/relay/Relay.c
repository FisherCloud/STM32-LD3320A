/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��Relay.c
 * ����    ���̵���GPIO��������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: Relay_GPIO-->PB3
 * ���Է�ʽ��J-Link-OB
*********************************************************************************/
//ͷ�ļ�
#include "Relay.h"

/**
 * @file   Relay_GPIO_Config
 * @brief  Relay����������
 * @param  ��
 * @retval ��
 */
void Relay_GPIO_Config(void)
{
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;
    /*PB3Ϊjtag�ӿڣ���û�йر�jtag����֮ǰ���ڳ������޷����ó���ͨIO��ģʽ����Ҫ�����������
    ����Ҫ����AFIOʱ�ӣ�Ȼ����AFIO�������У��ͷ��������*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_APB2PeriphClockCmd(Relay_GPIO_RCC, ENABLE); //ʹ��GPIO������ʱ��
    /*Relay*/
    GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN; //ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ
    GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
}

