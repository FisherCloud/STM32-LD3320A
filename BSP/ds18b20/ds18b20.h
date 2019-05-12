#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"
#include "sys.h"

#define DS18B20_IO_RCC          RCC_APB2Periph_GPIOA
#define DS18B20_IO_PORT         GPIOA
#define DS18B20_IO_PIN      	GPIO_Pin_15

//IO��������
#define DS18B20_IO_IN()  		{DS18B20_IO_PORT->CRH&=0x0FFFFFFF;DS18B20_IO_PORT->CRH|=0x80000000;}
#define DS18B20_IO_OUT() 		{DS18B20_IO_PORT->CRH&=0x0FFFFFFF;DS18B20_IO_PORT->CRH|=0x30000000;}

#define	DS18B20_DQ_IN  	        PAin(15)//���ݶ˿�
#define DS18B20_DQ_OUT       	PAout(15)

u8 DS18B20_Init(void);//��ʼ��DS18B20
double DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20

#endif















