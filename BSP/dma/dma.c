/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��dma.c
 * ����    ��DMA1�ĸ�ͨ������
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/
#include "dma.h"

/**
 * @file   MYDMA_Config
 * @brief  DMA1�ĸ�ͨ�����ó�ʼ��,����Ĵ�����ʽ�ǹ̶���,
*         �Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
 * @param
*					DMA_CHx:DMAͨ��CHx
*					cpar:�����ַ
*					cmar:�洢����ַ
*					cndtr:���ݴ�����
 * @retval ��
 */
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����

    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
    DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���

}
/**
 * @file   MYDMA_Enable
 * @brief  ����һ��DMA����
 * @param
*					DMA_CHx:DMAͨ��CHx
*					len:�����С
 * @retval ��
 */
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx, u16 len)
{
    DMA_Cmd(DMA_CHx, DISABLE);                         //�ر�ͨ��
    DMA_SetCurrDataCounter(DMA_CHx, len);              //DMAͨ����DMA����Ĵ�С
    DMA_Cmd(DMA_CHx, ENABLE);                          //ʹ��ͨ��
}



























