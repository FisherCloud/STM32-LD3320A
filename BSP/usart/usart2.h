#ifndef __USART2_H
#define __USART2_H

#include "includes.h"

#define USART2_REC_LEN  		1024     //���崮��2�������ֽ��� 1024
#define USART2_TRS_LEN          1024     //���崮��2���ӷ��ͽ��� 1024
#define EN_USART2_RX 			1		 //ʹ�ܣ�1��/��ֹ��0������2����

extern u8  USART2_RX_BUF[USART2_REC_LEN]; //����2���ջ���,���USART2_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART2_TX_BUF[USART2_TRS_LEN]; //����2���ͻ���,���USART2_TRS_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART2_RX_STA;         		  //����2����״̬���
extern u8  USART2_DATA_FLAG;               //���ݽ��ձ��

void USART2_Config(u32 bound);

void u2_printf(char* fmt, ...);
#endif
