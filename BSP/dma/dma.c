/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：dma.c
 * 描述    ：DMA1的各通道配置
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 调试方式：J-Link-OB
**********************************************************************************/
#include "dma.h"

/**
 * @file   MYDMA_Config
 * @brief  DMA1的各通道配置初始化,这里的传输形式是固定的,
*         从存储器->外设模式/8位数据宽度/存储器增量模式
 * @param
*					DMA_CHx:DMA通道CHx
*					cpar:外设地址
*					cmar:存储器地址
*					cndtr:数据传输量
 * @retval 无
 */
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输

    DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
    DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器

}
/**
 * @file   MYDMA_Enable
 * @brief  开启一次DMA传输
 * @param
*					DMA_CHx:DMA通道CHx
*					len:缓存大小
 * @retval 无
 */
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx, u16 len)
{
    DMA_Cmd(DMA_CHx, DISABLE);                         //关闭通道
    DMA_SetCurrDataCounter(DMA_CHx, len);              //DMA通道的DMA缓存的大小
    DMA_Cmd(DMA_CHx, ENABLE);                          //使能通道
}



























