/***************************乐声电子科技有限公司****************************
**  文件名称：语音模块底层驱动
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

//===========================================================================
// 添加识别码

#define DATE_A 6    /*数组二维数值*/
#define DATE_B 20		/*数组一维数值*/

uint8  sRecog[DATE_A][DATE_B] =
{
    "liu shui deng", \
    "shan shuo", \
    "an jian chu fa", \
    "quan mie", \
    "kai deng", \
    "guan deng"
};	/*添加关键词，用户修改*/
uint8  pCode[DATE_A] =
{
    CODE_LSD, \
    CODE_SS, \
    CODE_AJCF, \
    CODE_QM, \
    CODE_LED1_ON, \
    CODE_LED1_OFF
};	/*添加识别码，用户修改*/

//=========================================================================

uint32 nMp3Size = 0;				 //mp3文件的大小
uint32 nMp3Pos = 0;				 //mp3文件的偏移(最后的偏移就是文件大小)
uint8  nLD_Mode = LD_MODE_IDLE;	   //用来记录当前是在进行ASR识别还是在播放MP3
uint8  bMp3Play = 0;				 //用来记录播放MP3的状态

uint8 ucRegVal;
extern uint8  nAsrStatus;

/***********************************************************
* 名    称：void LD_reset(void)
* 功    能：LD芯片硬件初始化
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
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
* 名    称： void LD_Init_Common(void)
* 功    能： 初始化命令
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
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
* 名    称：void LD_Init_ASR(void)
* 功    能：初始化ASR
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
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
void LD_Init_MP3(void)	//播放初始化
{
    nLD_Mode = LD_MODE_MP3;	   //当前进行MP3播放
    LD_Init_Common();		   //通用初始化

    LD_WriteReg(0xBD, 0x02);	  //内部增益控制 初始化时写入FFH
    LD_WriteReg(0x17, 0x48);	//写48H可以激活DSP
    delay_ms(100);

    LD_WriteReg(0x85, 0x52); 	//内部反馈设置 初始化时写入52H
    LD_WriteReg(0x8F, 0x00);  	//LineOut(线路输出)选择 初始化时写入00H
    LD_WriteReg(0x81, 0x00);	//耳机左音量 设置为00H为最大音量
    LD_WriteReg(0x83, 0x00);	//耳机右音量 设置为00H为最大音量
    LD_WriteReg(0x8E, 0xff);	//喇叭输出音量  本寄存器设置为00H为最大音量	此处声音关闭
    LD_WriteReg(0x8D, 0xff);	//内部增益控制 初始化时写入FFH
    delay_ms(100);
    LD_WriteReg(0x87, 0xff);	//模拟电路控制 MP3播放初始化时写 FFH
    LD_WriteReg(0x89, 0xff);   //模拟电路控制 MP3播放时写 FFH
    delay_ms(100);
    LD_WriteReg(0x22, 0x00);   //FIFO_DATA下限低8位
    LD_WriteReg(0x23, 0x00);	//FIFO_DATA下限高8位
    LD_WriteReg(0x20, 0xef);   //FIFO_DATA上限低8位
    LD_WriteReg(0x21, 0x07);	//FIFO_DATA上限高8位
    LD_WriteReg(0x24, 0x77);
    LD_WriteReg(0x25, 0x03);
    LD_WriteReg(0x26, 0xbb);
    LD_WriteReg(0x27, 0x01);
}


//播放mp3准备
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

//音量调整
void LD_AdjustMIX2SPVolume(uint8 val)
{
    val = ((15 - val) & 0x0f) << 2;		 //
    LD_WriteReg(0x8E, val | 0xc3);
    LD_WriteReg(0x87, 0x78);
}

//填满fifo从而触发fifo中断,为播放mp3做准备
void fill_the_fifo(void)
{
    uint8 ucStatus;
    int i = 0;
    ucStatus = LD_ReadReg(0x06);

    //fifo是否满了
    while(!(ucStatus & MASK_FIFO_STATUS_AFULL))
    {

        LD_WriteReg(0x01, 0xff);
        i++;
        ucStatus = LD_ReadReg(0x06);
    }

}


//=================================================================

/***********************************************************
* 名    称：void ProcessInt0(void)
* 功    能：识别处理函数
* 入口参数：
* 出口参数：
* 说    明：可以利用查询方式，或中断方式进行调用
* 调用方法：
**********************************************************/

uint8 ucRegVal;	  //寄存器备份变量
uint8 ucHighInt;  //寄存器备份变量
uint8 ucLowInt;	  //寄存器备份变量
static	int fd;

void ProcessInt0(void)
{
    uint8 nAsrResCount = 0;

    ucRegVal = LD_ReadReg(0x2B);

    if(nLD_Mode == LD_MODE_ASR_RUN)		//当前进行语音识别
    {
        // 语音识别产生的中断
        // （有声音输入，不论识别成功或失败都有中断）
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
            nAsrStatus = LD_ASR_FOUNDZERO;	//执行没有识别
        }

        LD_WriteReg(0x2b, 0);
        LD_WriteReg(0x1C, 0); /*写0:ADC不可用*/

        return;
//        LD_WriteReg(0x29, 0) ;
//        LD_WriteReg(0x02, 0) ;
//        LD_WriteReg(0x2B, 0);
//        LD_WriteReg(0xBA, 0);
//        LD_WriteReg(0xBC, 0);
//        LD_WriteReg(0x08, 1);	 /*清除FIFO_DATA*/
//        LD_WriteReg(0x08, 0);	/*清除FIFO_DATA后 再次写0*/
    }
    else
    {
        // 声音播放产生的中断，有三种：
        // A. 声音数据已全部播放完。
        // B. 声音数据已发送完毕。
        // C. 声音数据暂时将要用完，需要放入新的数据。
        ucHighInt = LD_ReadReg(0x29);
        ucLowInt = LD_ReadReg(0x02);
        LD_WriteReg(0x29, 0) ;
        LD_WriteReg(0x02, 0) ;

        if(LD_ReadReg(0xBA)&CAUSE_MP3_SONG_END)
        {
            // A. 声音数据已全部播放完。

            LD_WriteReg(0x2B,  0);
            LD_WriteReg(0xBA, 0);
            LD_WriteReg(0xBC, 0x0);
            bMp3Play = 0;					// 声音数据全部播放完后，修改bMp3Play的变量
            LD_WriteReg(0x08, 1);

            LD_WriteReg(0x08, 0);
            LD_WriteReg(0x33, 0);
            close(fd);
            return ;
        }

        if(nMp3Pos >= nMp3Size)
        {
            // B. 声音数据已发送完毕。

            LD_WriteReg(0xBC, 0x01);
            LD_WriteReg(0x29, 0x10);
            close(fd);
            return;
        }

        // C. 声音数据暂时将要用完，需要放入新的数据。

        LD_ReloadMp3Data_Again();

        LD_WriteReg(0x29, ucHighInt);
        LD_WriteReg(0x02, ucLowInt) ;
    }

    //LD3320_delay(1);

}

/*  继续读取mp3文件数据到fifo,直到fifo满
 *	边写mp3文件数据到fifo时,LD3320会变解码播放
 *	当然写mp3文件数据到fifo的时间会短过声音的时间
 *	当声音快播放完毕的时候会进入ProcessInt0函数
 *	ProcessInt0函数又会调用此函数,所以声音得以连续
 */
void LD_ReloadMp3Data_Again(void)
{
    uint8 val;
    uint8 ucStatus;

    ucStatus = LD_ReadReg(0x06);

    //fifo是否满了
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

        //等待MP3播放完毕
        while(!(LD_ReadReg(0xBA)&CAUSE_MP3_SONG_END));

        LD_WriteReg(0x2B,  0);
        LD_WriteReg(0xBA, 0);
        LD_WriteReg(0xBC, 0x0);
        bMp3Play = 0;					// 声音数据全部播放完后，修改bMp3Play的变量
        LD_WriteReg(0x08, 1);

        LD_WriteReg(0x08, 0);
        LD_WriteReg(0x33, 0);

        close(fd);

    }

}

/*
 * 播放mp3时调用此函数即可
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

    //将LD3320初始化为播放MP3模式
    LD_Init_MP3();
    //设置耳机音量
    LD_AdjustMIX2SPVolume(val);
    //开始播放
    LD_play();

    return 0;

}

//================================================================

/***********************************************************
* 名    称：uint8 LD_Check_ASRBusyFlag_b2(void)
* 功    能：检测 ASR 是否忙状态
* 入口参数：flag ---1：空闲状态
* 出口参数：
* 说    明：
* 调用方法：
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
* 名    称： void LD_AsrStart(void)
* 功    能：
* 入口参数：  ASR初始化
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
void LD_AsrStart(void)
{
    LD_Init_ASR();
}
/***********************************************************
* 名    称： uint8 LD_AsrRun(void)
* 功    能： ASR执行函数
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
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
* 名    称：uint8 LD_AsrAddFixed(void)
* 功    能：添加识别关键词语
* 入口参数：
* 出口参数： flag-- 1：success
* 说    明：开发者可以学习"语音识别芯片LD3320高阶秘籍.pdf"中
						关于垃圾词语吸收错误的用法，提高识别率。
* 调用方法：
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
* 名    称： uint8 LD_GetResult(void)
* 功    能： 获取识别结果
* 入口参数：
* 出口参数： 返回识别码 LD_ReadReg(0xc5 );
* 说    明：
* 调用方法：
**********************************************************/
uint8 LD_GetResult(void)
{
    return LD_ReadReg(0xc5);
}



