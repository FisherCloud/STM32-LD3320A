/***************************�������ӿƼ����޹�˾****************************
**  �ļ����ƣ�����ģ�� ��д�ļ�
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

/***********************************************************
* ��    �ƣ�static uint8 spi_send_byte(uint8 byte)
* ��    �ܣ� дSPI
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
static uint8 spi_send_byte(uint8 byte)
{
    /* ѭ����ⷢ�ͻ������Ƿ��ǿ� */
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

    /*ͨ��SPI3���跢������*/
    SPI_I2S_SendData(SPI3, byte);

    /* �ȴ��������ݣ�ѭ�����������ݻ����� */
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

    /* ���ض��������� */
    return SPI_I2S_ReceiveData(SPI3);
}
/***********************************************************
* ��    �ƣ�void LD_WriteReg(uint8 data1,uint8 data2)
* ��    �ܣ� дld3320�Ĵ���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD_WriteReg(uint8 data1, uint8 data2)
{
    LD_CS_L();

    LD_SPIS_L();

    spi_send_byte(0x04);

    spi_send_byte(data1);

    spi_send_byte(data2);

    LD_CS_H();

}
/***********************************************************
* ��    �ƣ�uint8 LD_ReadReg(uint8 reg_add)
* ��    �ܣ���ld3320�Ĵ���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
uint8 LD_ReadReg(uint8 reg_add)
{
    uint8 i;

    LD_CS_L();

    LD_SPIS_L();

    spi_send_byte(0x05);

    spi_send_byte(reg_add);

    i = spi_send_byte(0x00);	/*��SPI*/

    LD_CS_H();

    return(i);
}



