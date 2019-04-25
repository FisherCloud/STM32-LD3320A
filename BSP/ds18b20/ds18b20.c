/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��ds18b20.c
 * ����    ��DS18B20�ĳ�ʼ���Լ��¶ȵĶ�ȡ����
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

#include "ds18b20.h"
#include "delay.h"

/**
 * @file   DS18B20_Rst
 * @brief  ��λDS18B20
 * @param  ��
 * @retval ��
 */
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET  OUTPUT
    DS18B20_DQ_OUT(0); //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_OUT(1); //DQ=1
    delay_us(15);     //15US
}
/**
 * @file   DS18B20_Check
 * @brief  �ȴ�DS18B20�Ļ�Ӧ
 * @param  ��
 * @retval 1:δ��⵽DS18B20�Ĵ���
*					0:����
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
        return 1;    //δ��⵽DS18B20
    }

    return 0;//��⵽DS18B20
}
/**
 * @file   DS18B20_Read_Bit
 * @brief  ��DS18B20��ȡһ��λ
 * @param  ��
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
 * @brief  ��DS18B20��ȡһ���ֽ�
 * @param  ��
 * @retval ����������
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
 * @brief  дһ���ֽڵ�DS18B20
 * @param  dat��Ҫд����ֽ�
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
 * @brief  ��ʼ�¶�ת��
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
 * @brief  ��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
 * @param  --
 * @retval 1:�����ڣ�0:����
 */
u8 DS18B20_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(DS18B20_IO_RCC, ENABLE);	 //ʹ��PORTA��ʱ��

    GPIO_InitStructure.GPIO_Pin = DS18B20_IO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_IO_PORT, &GPIO_InitStructure);//��ʼ��

    DS18B20_Rst();
    return DS18B20_Check();
}

/**
 * @file   DS18B20_Get_Temp
 * @brief  ��ds18b20�õ��¶�ֵ
 * @param  --
 * @retval ת������¶�ֵ
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
        temp = 0; //�¶�Ϊ��
    }
    else
    {
        temp = 1;    //�¶�Ϊ��
    }

    tem_temp = TH; //��ø߰�λ
    tem_temp <<= 8;
    tem_temp += TL; //��õװ�λ
    tem = (double)tem_temp * 0.0625; //ת��

    if(temp)
    {
        return tem;    //�����¶�ֵ
    }
    else
    {
        return -tem;
    }
}
















