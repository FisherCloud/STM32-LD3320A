/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：ds18b20.c
 * 描述    ：DS18B20的初始化以及温度的读取操作
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 调试方式：J-Link-OB
**********************************************************************************/

#include "ds18b20.h"
#include "delay.h"

/**
 * @file   DS18B20_Rst
 * @brief  复位DS18B20
 * @param  无
 * @retval 无
 */
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET  OUTPUT
    DS18B20_DQ_OUT(0); //拉低DQ
    delay_us(750);    //拉低750us
    DS18B20_DQ_OUT(1); //DQ=1
    delay_us(15);     //15US
}
/**
 * @file   DS18B20_Check
 * @brief  等待DS18B20的回应
 * @param  无
 * @retval 1:未检测到DS18B20的存在
*					0:存在
 */
u8 DS18B20_Check(void)
{
    u8 retry = 0;
    DS18B20_IO_IN();//SET  INPUT

    while(DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    };

    if(retry >= 200)
    {
        return 1;
    }
    else
    {
        retry = 0;
    }

    while(!DS18B20_DQ_IN && retry < 240)
    {
        retry++;
        delay_us(1);
    };

    if(retry >= 240)
    {
        return 1;    //未检测到DS18B20
    }

    return 0;//检测到DS18B20
}
/**
 * @file   DS18B20_Read_Bit
 * @brief  从DS18B20读取一个位
 * @param  无
 * @retval 1/0
 */
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
    DS18B20_IO_OUT();//SET  OUTPUT
    DS18B20_DQ_OUT(0);
    delay_us(2);
    DS18B20_DQ_OUT(1);
    DS18B20_IO_IN();//SET  INPUT
    delay_us(12);

    if(DS18B20_DQ_IN)
    {
        data = 1;
    }
    else
    {
        data = 0;
    }

    delay_us(50);
    return data;
}
/**
 * @file   DS18B20_Read_Byte
 * @brief  从DS18B20读取一个字节
 * @param  无
 * @retval 读到的数据
 */
u8 DS18B20_Read_Byte(void)    // read one byte
{
    u8 i, j, dat;
    dat = 0;

    for(i = 1; i <= 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }

    return dat;
}

/**
 * @file   DS18B20_Write_Byte
 * @brief  写一个字节到DS18B20
 * @param  dat：要写入的字节
 * @retval --
 */
void DS18B20_Write_Byte(u8 dat)
{
    u8 j;
    u8 testb;
    DS18B20_IO_OUT();//SET OUTPUT;

    for(j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;

        if(testb)
        {
            DS18B20_DQ_OUT(0);// Write 1
            delay_us(2);
            DS18B20_DQ_OUT(1);
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT(0);// Write 0
            delay_us(60);
            DS18B20_DQ_OUT(1);
            delay_us(2);
        }
    }
}
/**
 * @file   DS18B20_Start
 * @brief  开始温度转换
 * @param  --
 * @retval --
 */
void DS18B20_Start(void)// ds1820 start convert
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}



/**
 * @file   DS18B20_Init
 * @brief  初始化DS18B20的IO口 DQ 同时检测DS的存在
 * @param  --
 * @retval 1:不存在；0:存在
 */
u8 DS18B20_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(DS18B20_IO_RCC, ENABLE);	 //使能PORTA口时钟

    GPIO_InitStructure.GPIO_Pin = DS18B20_IO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_IO_PORT, &GPIO_InitStructure);//初始化

    DS18B20_Rst();
    return DS18B20_Check();
}

/**
 * @file   DS18B20_Get_Temp
 * @brief  从ds18b20得到温度值
 * @param  --
 * @retval 转化后的温度值
 */
double DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL, TH;
    short tem_temp;
    double tem;

    DS18B20_Start();  // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert
    TL = DS18B20_Read_Byte(); // LSB
    TH = DS18B20_Read_Byte(); // MSB

    if(TH > 7)
    {
        TH = ~TH;
        TL = ~TL;
        temp = 0; //温度为负
    }
    else
    {
        temp = 1;    //温度为正
    }

    tem_temp = TH; //获得高八位
    tem_temp <<= 8;
    tem_temp += TL; //获得底八位
    tem = (double)tem_temp * 0.0625; //转换

    if(temp)
    {
        return tem;    //返回温度值
    }
    else
    {
        return -tem;
    }
}
















