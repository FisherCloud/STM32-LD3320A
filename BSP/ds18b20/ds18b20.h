#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"

#define DS18B20_IO_RCC          RCC_APB2Periph_GPIOA
#define DS18B20_IO_PORT         GPIOA
#define DS18B20_IO_PIN      	GPIO_Pin_8

//IO��������
#define DS18B20_IO_IN()  		{GPIOB->CRH&=0xFFFFFFF0;GPIOB->CRH|=0x0000000A;}
#define DS18B20_IO_OUT() 		{GPIOB->CRH&=0xFFFFFFF0;GPIOB->CRH|=0x00000003;}

#define	DS18B20_DQ_IN  	        ((GPIOB->IDR &0x100)>>10)// GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_8)//���ݶ˿�	
#define DS18B20_DQ_OUT(x)       ((x==1)?(GPIOB->BSRR=0x0100):(GPIOB->BRR = 0x0100))

u8 DS18B20_Init(void);//��ʼ��DS18B20
double DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20

#endif















