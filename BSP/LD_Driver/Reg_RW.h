/************************************************************************************
**	��Ȩ���У�Copyright (c) 2005 - 2010 ICRoute INC.
** Ӳ��˵����STM32f103vet6  8MHZ  �ڲ�ʱ��Ϊ72MHZ
** 					 LD3320 ��Դ���� 12MHZ  ��ͬ�ľ������޸�LDChip.h �еĶ���ֵ����define CLK_IN  12��
** ���˵�������ж�д�Ĵ�����ʽ�������������ʽ���޸�Reg_RW.c �ļ����ɣ�
** �޸���:  http://shop64790118.taobao.com/
** ʱ�䣺2011.10.15
** ˵�����������޸��� ICRoute��˾ ���׵�Դ���򣬰�Ȩ�������С�
************************************************************************************/

// Reg_RW.h  ��дLD3320оƬ�ļĴ������������ַ�����
#ifndef REG_RW_H
#define REG_RW_H

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

/*************�˿���Ϣ********************
 * ����˵��
 * RST      PB6
 * CS   	  PB8
 * WR/SPIS  PB9
 * P2/SDCK  PB3
 * P1/SDO   PB4
 * P0/SDI   PB5
 * IRQ      PC1
 * A0				PB7
 * RD				PA0
*****************************************/
#define LD_RST_H() GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LD_RST_L() GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define LD_CS_H()	GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LD_CS_L()	GPIO_ResetBits(GPIOB, GPIO_Pin_8)

#define LD_SPIS_H()  GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define LD_SPIS_L()  GPIO_ResetBits(GPIOB, GPIO_Pin_9)

/****************************************************************
�������� LD_WriteReg
���ܣ�дLD3320оƬ�ļĴ���
������  address, 8λ�޷�����������ַ
		dataout��8λ�޷���������Ҫд�������
����ֵ����
****************************************************************/
void LD_WriteReg(unsigned char address, unsigned char dataout);

/****************************************************************
�������� LD_ReadReg
���ܣ���LD3320оƬ�ļĴ���
������  address, 8λ�޷�����������ַ
����ֵ��8λ�޷�����������ȡ�Ľ��
****************************************************************/
unsigned char LD_ReadReg(unsigned char address);



#endif
