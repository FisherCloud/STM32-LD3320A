#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"
#include "sys.h"

#define DS18B20_IO_RCC          RCC_APB2Periph_GPIOA
#define DS18B20_IO_PORT         GPIOA
#define DS18B20_IO_PIN      	GPIO_Pin_15

//IO方向设置
#define DS18B20_IO_IN()  		{DS18B20_IO_PORT->CRH&=0x0FFFFFFF;DS18B20_IO_PORT->CRH|=0x80000000;}
#define DS18B20_IO_OUT() 		{DS18B20_IO_PORT->CRH&=0x0FFFFFFF;DS18B20_IO_PORT->CRH|=0x30000000;}

#define	DS18B20_DQ_IN  	        PAin(15)//数据端口
#define DS18B20_DQ_OUT       	PAout(15)

u8 DS18B20_Init(void);//初始化DS18B20
double DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20

#endif















