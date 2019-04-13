/***************************乐声电子科技有限公司****************************
**  文件名称：语音模块 读写文件
**	CPU: STM32f103RCT6
**	晶振：8MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.4语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.4.11
**  说明：本程序 具备语音识别、串口通信、开发板基本功能演示。
***************************乐声电子科技有限公司******************************/
#include "includes.h"

/***********************************************************
* 名    称：static uint8 spi_send_byte(uint8 byte)
* 功    能： 写SPI
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
static uint8 spi_send_byte(uint8 byte)
{
    /* 循环检测发送缓冲区是否是空 */
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

    /*通过SPI3外设发出数据*/
    SPI_I2S_SendData(SPI3, byte);

    /* 等待接收数据，循环检查接收数据缓冲区 */
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

    /* 返回读出的数据 */
    return SPI_I2S_ReceiveData(SPI3);
}
/***********************************************************
* 名    称：void LD_WriteReg(uint8 data1,uint8 data2)
* 功    能： 写ld3320寄存器
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
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
* 名    称：uint8 LD_ReadReg(uint8 reg_add)
* 功    能：读ld3320寄存器
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
uint8 LD_ReadReg(uint8 reg_add)
{
    uint8 i;

    LD_CS_L();

    LD_SPIS_L();

    spi_send_byte(0x05);

    spi_send_byte(reg_add);

    i = spi_send_byte(0x00);	/*读SPI*/

    LD_CS_H();

    return(i);
}



