/************************************************************************************
**	��Ȩ���У�Copyright (c) 2005 - 2010 ICRoute INC.
** Ӳ��˵����STM32f103vet6  8MHZ  �ڲ�ʱ��Ϊ72MHZ
** 					 LD3320 ��Դ���� 12MHZ  ��ͬ�ľ������޸�LDChip.h �еĶ���ֵ����define CLK_IN  12��
** ���˵�������ж�д�Ĵ�����ʽ�������������ʽ���޸�Reg_RW.c �ļ����ɣ�
** �޸���:  http://shop64790118.taobao.com/
** ʱ�䣺2011.10.15
** ˵�����������޸��� ICRoute��˾ ���׵�Դ���򣬰�Ȩ�������С�
************************************************************************************/

#ifndef LD_CHIP_H
#define LD_CHIP_H

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

//	��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE		0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 	0x40


//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE				0x00	//	��ʾû������ASRʶ��
#define LD_ASR_RUNING			0x01	//	��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//	��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//	��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 		0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬


#define CLK_IN   	36	/* user need modify this value according to clock in */
#define LD_PLL_11			(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

// LD chip fixed values.
#define        RESUM_OF_MUSIC               0x01
#define        CAUSE_MP3_SONG_END           0x20

#define        MASK_INT_SYNC				0x10
#define        MASK_INT_FIFO				0x04
#define    	   MASK_AFIFO_INT				0x01
#define        MASK_FIFO_STATUS_AFULL		0x08

void LD_reset(void);

void LD_Init_Common(void);

void LD_Init_ASR(void);
void LD_Init_MP3(void);

void LD_play(void);
void LD_AdjustMIX2SPVolume(uint8 value);
void LD_ReloadMp3Data(void);
void LD_ReloadMp3Data_2(void);
void fill_the_fifo(void);
void LD_ReloadMp3Data_Again(void);
int PlayDemoSound_mp3(char *path, uint8 val);

uint8 LD_ProcessAsr(uint32 RecogAddr);
void LD_AsrStart(void);
uint8 LD_AsrRun(void);
uint8 LD_AsrAddFixed(void);
uint8 LD_GetResult(void);
uint8 LD_Check_ASRBusyFlag_b2(void);

void LD_ReadMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);
void LD_WriteMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);

extern uint32 nMp3StartPos;
extern uint32 nMp3Size;
extern uint32 nMp3Pos;
extern uint8 bMp3Play;
extern uint8 nLD_Mode;

#define MIC_VOL 0x43
#endif
