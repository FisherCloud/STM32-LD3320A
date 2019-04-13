/***************************�������ӿƼ����޹�˾****************************
**  �ļ����ƣ�����ģ��ײ�����
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

//===========================================================================
// ���ʶ����

#define DATE_A 6    /*�����ά��ֵ*/
#define DATE_B 20		/*����һά��ֵ*/

uint8  sRecog[DATE_A][DATE_B] =
{
    "liu shui deng", \
    "shan shuo", \
    "an jian chu fa", \
    "quan mie", \
    "kai deng", \
    "guan deng"
};	/*��ӹؼ��ʣ��û��޸�*/
uint8  pCode[DATE_A] =
{
    CODE_LSD, \
    CODE_SS, \
    CODE_AJCF, \
    CODE_QM, \
    CODE_LED1_ON, \
    CODE_LED1_OFF
};	/*���ʶ���룬�û��޸�*/

//=========================================================================

uint32 nMp3Size = 0;				 //mp3�ļ��Ĵ�С
uint32 nMp3Pos = 0;				 //mp3�ļ���ƫ��(����ƫ�ƾ����ļ���С)
uint8  nLD_Mode = LD_MODE_IDLE;	   //������¼��ǰ���ڽ���ASRʶ�����ڲ���MP3
uint8  bMp3Play = 0;				 //������¼����MP3��״̬

uint8 ucRegVal;
extern uint8  nAsrStatus;

/***********************************************************
* ��    �ƣ�void LD_reset(void)
* ��    �ܣ�LDоƬӲ����ʼ��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD_reset(void)
{
    LD_RST_H();
    LD3320_delay(100);
    LD_RST_L();
    LD3320_delay(100);
    LD_RST_H();
    LD3320_delay(100);
    LD_CS_L();
    LD3320_delay(100);
    LD_CS_H();
    LD3320_delay(100);
}

/***********************************************************
* ��    �ƣ� void LD_Init_Common(void)
* ��    �ܣ� ��ʼ������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD_Init_Common(void)
{

    LD_ReadReg(0x06);
    LD_WriteReg(0x17, 0x35);
    LD3320_delay(5);
    LD_ReadReg(0x06);

    LD_WriteReg(0x89, 0x03);
    LD3320_delay(5);
    LD_WriteReg(0xCF, 0x43);
    LD3320_delay(5);
    LD_WriteReg(0xCB, 0x02);

    /*PLL setting*/
    LD_WriteReg(0x11, LD_PLL_11);

    if(nLD_Mode == LD_MODE_MP3)
    {
        LD_WriteReg(0x1E, 0x00);
        LD_WriteReg(0x19, LD_PLL_MP3_19);
        LD_WriteReg(0x1B, LD_PLL_MP3_1B);
        LD_WriteReg(0x1D, LD_PLL_MP3_1D);
    }
    else
    {
        LD_WriteReg(0x1E, 0x00);
        LD_WriteReg(0x19, LD_PLL_ASR_19);
        LD_WriteReg(0x1B, LD_PLL_ASR_1B);
        LD_WriteReg(0x1D, LD_PLL_ASR_1D);
    }

    LD3320_delay(5);

    LD_WriteReg(0xCD, 0x04);
    LD_WriteReg(0x17, 0x4c);
    LD3320_delay(1);
    LD_WriteReg(0xB9, 0x00);
    LD_WriteReg(0xCF, 0x4F);
    LD_WriteReg(0x6F, 0xFF);
}

/***********************************************************
* ��    �ƣ�void LD_Init_ASR(void)
* ��    �ܣ���ʼ��ASR
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD_Init_ASR(void)
{
    nLD_Mode = LD_MODE_ASR_RUN;
    LD_Init_Common();

    LD_WriteReg(0xBD, 0x00);
    LD_WriteReg(0x17, 0x48);
    LD3320_delay(5);
    LD_WriteReg(0x3C, 0x80);
    LD_WriteReg(0x3E, 0x07);
    LD_WriteReg(0x38, 0xff);
    LD_WriteReg(0x3A, 0x07);
    LD_WriteReg(0x40, 0);
    LD_WriteReg(0x42, 8);
    LD_WriteReg(0x44, 0);
    LD_WriteReg(0x46, 8);
    LD3320_delay(1);
}

//==================================================================
void LD_Init_MP3(void)	//���ų�ʼ��
{
    nLD_Mode = LD_MODE_MP3;	   //��ǰ����MP3����
    LD_Init_Common();		   //ͨ�ó�ʼ��

    LD_WriteReg(0xBD, 0x02);	  //�ڲ�������� ��ʼ��ʱд��FFH
    LD_WriteReg(0x17, 0x48);	//д48H���Լ���DSP
    delay_ms(100);

    LD_WriteReg(0x85, 0x52); 	//�ڲ��������� ��ʼ��ʱд��52H
    LD_WriteReg(0x8F, 0x00);  	//LineOut(��·���)ѡ�� ��ʼ��ʱд��00H
    LD_WriteReg(0x81, 0x00);	//���������� ����Ϊ00HΪ�������
    LD_WriteReg(0x83, 0x00);	//���������� ����Ϊ00HΪ�������
    LD_WriteReg(0x8E, 0xff);	//�����������  ���Ĵ�������Ϊ00HΪ�������	�˴������ر�
    LD_WriteReg(0x8D, 0xff);	//�ڲ�������� ��ʼ��ʱд��FFH
    delay_ms(100);
    LD_WriteReg(0x87, 0xff);	//ģ���·���� MP3���ų�ʼ��ʱд FFH
    LD_WriteReg(0x89, 0xff);   //ģ���·���� MP3����ʱд FFH
    delay_ms(100);
    LD_WriteReg(0x22, 0x00);   //FIFO_DATA���޵�8λ
    LD_WriteReg(0x23, 0x00);	//FIFO_DATA���޸�8λ
    LD_WriteReg(0x20, 0xef);   //FIFO_DATA���޵�8λ
    LD_WriteReg(0x21, 0x07);	//FIFO_DATA���޸�8λ
    LD_WriteReg(0x24, 0x77);
    LD_WriteReg(0x25, 0x03);
    LD_WriteReg(0x26, 0xbb);
    LD_WriteReg(0x27, 0x01);
}


//����mp3׼��
void LD_play()
{
    nMp3Pos = 0;
    bMp3Play = 1;

    if(nMp3Pos >=  nMp3Size)
    {
        return ;
    }

    fill_the_fifo();
//	LD_ReloadMp3Data();

    LD_WriteReg(0xBA, 0x00);
    LD_WriteReg(0x17, 0x48);
    LD_WriteReg(0x33, 0x01);
    LD_WriteReg(0x29, 0x04);

    LD_WriteReg(0x02, 0x01);
    LD_WriteReg(0x85, 0x5A);

}

//��������
void LD_AdjustMIX2SPVolume(uint8 val)
{
    val = ((15 - val) & 0x0f) << 2;		 //
    LD_WriteReg(0x8E, val | 0xc3);
    LD_WriteReg(0x87, 0x78);
}

//����fifo�Ӷ�����fifo�ж�,Ϊ����mp3��׼��
void fill_the_fifo(void)
{
    uint8 ucStatus;
    int i = 0;
    ucStatus = LD_ReadReg(0x06);

    //fifo�Ƿ�����
    while(!(ucStatus & MASK_FIFO_STATUS_AFULL))
    {

        LD_WriteReg(0x01, 0xff);
        i++;
        ucStatus = LD_ReadReg(0x06);
    }

}


//=================================================================

/***********************************************************
* ��    �ƣ�void ProcessInt0(void)
* ��    �ܣ�ʶ������
* ��ڲ�����
* ���ڲ�����
* ˵    �����������ò�ѯ��ʽ�����жϷ�ʽ���е���
* ���÷�����
**********************************************************/

uint8 ucRegVal;	  //�Ĵ������ݱ���
uint8 ucHighInt;  //�Ĵ������ݱ���
uint8 ucLowInt;	  //�Ĵ������ݱ���
static	int fd;

void ProcessInt0(void)
{
    uint8 nAsrResCount = 0;

    ucRegVal = LD_ReadReg(0x2B);

    if(nLD_Mode == LD_MODE_ASR_RUN)		//��ǰ��������ʶ��
    {
        // ����ʶ��������ж�
        // �����������룬����ʶ��ɹ���ʧ�ܶ����жϣ�
        LD_WriteReg(0x29, 0) ;
        LD_WriteReg(0x02, 0) ;

        if((ucRegVal & 0x10) && LD_ReadReg(0xb2) == 0x21 && LD_ReadReg(0xbf) == 0x35)
        {
            nAsrResCount = LD_ReadReg(0xba);

            if(nAsrResCount > 0 && nAsrResCount <= 4)
            {
                nAsrStatus = LD_ASR_FOUNDOK;
            }
            else
            {
                nAsrStatus = LD_ASR_FOUNDZERO;
            }
        }
        else
        {
            nAsrStatus = LD_ASR_FOUNDZERO;	//ִ��û��ʶ��
        }

        LD_WriteReg(0x2b, 0);
        LD_WriteReg(0x1C, 0); /*д0:ADC������*/

        return;
//        LD_WriteReg(0x29, 0) ;
//        LD_WriteReg(0x02, 0) ;
//        LD_WriteReg(0x2B, 0);
//        LD_WriteReg(0xBA, 0);
//        LD_WriteReg(0xBC, 0);
//        LD_WriteReg(0x08, 1);	 /*���FIFO_DATA*/
//        LD_WriteReg(0x08, 0);	/*���FIFO_DATA�� �ٴ�д0*/
    }
    else
    {
        // �������Ų������жϣ������֣�
        // A. ����������ȫ�������ꡣ
        // B. ���������ѷ�����ϡ�
        // C. ����������ʱ��Ҫ���꣬��Ҫ�����µ����ݡ�
        ucHighInt = LD_ReadReg(0x29);
        ucLowInt = LD_ReadReg(0x02);
        LD_WriteReg(0x29, 0) ;
        LD_WriteReg(0x02, 0) ;

        if(LD_ReadReg(0xBA)&CAUSE_MP3_SONG_END)
        {
            // A. ����������ȫ�������ꡣ

            LD_WriteReg(0x2B,  0);
            LD_WriteReg(0xBA, 0);
            LD_WriteReg(0xBC, 0x0);
            bMp3Play = 0;					// ��������ȫ����������޸�bMp3Play�ı���
            LD_WriteReg(0x08, 1);

            LD_WriteReg(0x08, 0);
            LD_WriteReg(0x33, 0);
            close(fd);
            return ;
        }

        if(nMp3Pos >= nMp3Size)
        {
            // B. ���������ѷ�����ϡ�

            LD_WriteReg(0xBC, 0x01);
            LD_WriteReg(0x29, 0x10);
            close(fd);
            return;
        }

        // C. ����������ʱ��Ҫ���꣬��Ҫ�����µ����ݡ�

        LD_ReloadMp3Data_Again();

        LD_WriteReg(0x29, ucHighInt);
        LD_WriteReg(0x02, ucLowInt) ;
    }

    //LD3320_delay(1);

}

/*  ������ȡmp3�ļ����ݵ�fifo,ֱ��fifo��
 *	��дmp3�ļ����ݵ�fifoʱ,LD3320�����벥��
 *	��Ȼдmp3�ļ����ݵ�fifo��ʱ���̹�������ʱ��
 *	�������첥����ϵ�ʱ������ProcessInt0����
 *	ProcessInt0�����ֻ���ô˺���,����������������
 */
void LD_ReloadMp3Data_Again(void)
{
    uint8 val;
    uint8 ucStatus;

    ucStatus = LD_ReadReg(0x06);

    //fifo�Ƿ�����
    while(!(ucStatus & MASK_FIFO_STATUS_AFULL) && nMp3Pos <= nMp3Size)
    {

        nMp3Pos++;
        read(fd, &val, 1);

        LD_WriteReg(0x01, val);

        ucStatus = LD_ReadReg(0x06);
    }

    if(nMp3Pos >= nMp3Size)
    {
        LD_WriteReg(0xBC, 0x01);
        LD_WriteReg(0x29, 0x10);

        //�ȴ�MP3�������
        while(!(LD_ReadReg(0xBA)&CAUSE_MP3_SONG_END));

        LD_WriteReg(0x2B,  0);
        LD_WriteReg(0xBA, 0);
        LD_WriteReg(0xBC, 0x0);
        bMp3Play = 0;					// ��������ȫ����������޸�bMp3Play�ı���
        LD_WriteReg(0x08, 1);

        LD_WriteReg(0x08, 0);
        LD_WriteReg(0x33, 0);

        close(fd);

    }

}

/*
 * ����mp3ʱ���ô˺�������
 */
int PlayDemoSound_mp3(char *path, uint8 val)
{

    fd = open(path, 0, 0);

    if(fd == -1)
    {
        close(fd);
        return fd;
    }

    bMp3Play = 1;

    nMp3Size = get_file_size(fd) - 1;

    //��LD3320��ʼ��Ϊ����MP3ģʽ
    LD_Init_MP3();
    //���ö�������
    LD_AdjustMIX2SPVolume(val);
    //��ʼ����
    LD_play();

    return 0;

}

//================================================================

/***********************************************************
* ��    �ƣ�uint8 LD_Check_ASRBusyFlag_b2(void)
* ��    �ܣ���� ASR �Ƿ�æ״̬
* ��ڲ�����flag ---1������״̬
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
uint8 LD_Check_ASRBusyFlag_b2(void)
{
    uint8 j;
    uint8 flag = 0;

    for(j = 0; j < 10; j++)
    {
        if(LD_ReadReg(0xb2) == 0x21)
        {
            flag = 1;
            break;
        }

        LD3320_delay(10);
    }

    return flag;
}
/***********************************************************
* ��    �ƣ� void LD_AsrStart(void)
* ��    �ܣ�
* ��ڲ�����  ASR��ʼ��
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void LD_AsrStart(void)
{
    LD_Init_ASR();
}
/***********************************************************
* ��    �ƣ� uint8 LD_AsrRun(void)
* ��    �ܣ� ASRִ�к���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
uint8 LD_AsrRun(void)
{
    LD_WriteReg(0x35, MIC_VOL);
    LD_WriteReg(0x1C, 0x09);
    LD_WriteReg(0xBD, 0x20);
    LD_WriteReg(0x08, 0x01);
    LD3320_delay(5);
    LD_WriteReg(0x08, 0x00);
    LD3320_delay(5);

    if(LD_Check_ASRBusyFlag_b2() == 0)
    {
        return 0;
    }

    LD_WriteReg(0xB2, 0xff);
    LD_WriteReg(0x37, 0x06);
    LD_WriteReg(0x37, 0x06);
    LD3320_delay(5);
    LD_WriteReg(0x1C, 0x0b);
    LD_WriteReg(0x29, 0x10);

    LD_WriteReg(0xBD, 0x00);
    return 1;
}

/***********************************************************
* ��    �ƣ�uint8 LD_AsrAddFixed(void)
* ��    �ܣ����ʶ��ؼ�����
* ��ڲ�����
* ���ڲ����� flag-- 1��success
* ˵    ���������߿���ѧϰ"����ʶ��оƬLD3320�߽��ؼ�.pdf"��
						���������������մ�����÷������ʶ���ʡ�
* ���÷�����
**********************************************************/
uint8 LD_AsrAddFixed(void)
{
    uint8 k, flag;
    uint8 nAsrAddLength;

    flag = 1;

    for(k = 0; k < DATE_A; k++)
    {
        if(LD_Check_ASRBusyFlag_b2() == 0)
        {
            flag = 0;
            break;
        }

        LD_WriteReg(0xc1, pCode[k]);
        LD_WriteReg(0xc3, 0);
        LD_WriteReg(0x08, 0x04);
        LD3320_delay(1);
        LD_WriteReg(0x08, 0x00);
        LD3320_delay(1);

        for(nAsrAddLength = 0; nAsrAddLength < DATE_B; nAsrAddLength++)
        {
            if(sRecog[k][nAsrAddLength] == 0)
            {
                break;
            }

            LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
        }

        LD_WriteReg(0xb9, nAsrAddLength);
        LD_WriteReg(0xb2, 0xff);
        LD_WriteReg(0x37, 0x04);
//		LD_WriteReg(0x37, 0x04);
    }

    return flag;
}

/***********************************************************
* ��    �ƣ� uint8 LD_GetResult(void)
* ��    �ܣ� ��ȡʶ����
* ��ڲ�����
* ���ڲ����� ����ʶ���� LD_ReadReg(0xc5 );
* ˵    ����
* ���÷�����
**********************************************************/
uint8 LD_GetResult(void)
{
    return LD_ReadReg(0xc5);
}



