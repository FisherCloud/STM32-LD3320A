#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"

#define DS18B20_IO_RCC          RCC_APB2Periph_GPIOA
#define DS18B20_IO_PORT         GPIOA
#define DS18B20_IO_PIN      	GPIO_Pin_8

//IO方向设置
#define DS18B20_IO_IN()  		{GPIOB->CRH&=0xFFFFFFF0;GPIOB->CRH|=0x0000000A;}
#define DS18B20_IO_OUT() 		{GPIOB->CRH&=0xFFFFFFF0;GPIOB->CRH|=0x00000003;}

#define	DS18B20_DQ_IN  	        ((GPIOB->IDR &0x100)>>10)// GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_8)//数据端口	
#define DS18B20_DQ_OUT(x)       ((x==1)?(GPIOB->BSRR=0x0100):(GPIOB->BRR = 0x0100))

u8 DS18B20_Init(void);//初始化DS18B20
double DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20

#endif















