/***************************�������ӿƼ����޹�˾****************************
**  �������ƣ�YS-V0.4����ʶ�𿪷������
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

/* Private function prototypes -----------------------------------------------*/

extern void  LD3320_main(void);
extern void LD3320_Init(void);

/***********************************************************
* ��    �ƣ� ������
* ��    �ܣ�	�������
* ��ڲ�����  ��
* ���ڲ�����	��
* ˵    ����	��
* ���÷����� 	��
**********************************************************/
int main(void)
{
    /* System Clocks Configuration */
	SYSTICK_Init();
	delay_ms(500);
    
	USART1_Config(115200); //USART ����
	
	LD3320_Init();
	
//	{
//        FATFS fs;
//        SD_CardInfo SDCardInfo;

//        /* SD���ӿڳ�ʼ��	 */
//        SD_Init();
//        /* ��ȡSD����Ϣ */
//        SD_GetCardInfo(&SDCardInfo);
//        SD_SelectDeselect((uint32_t)(SDCardInfo.RCA << 16));
//        /* ����SDIO�ӿ����ݿ�� */
//        SD_EnableWideBusOperation(SDIO_BusWide_4b);
//        /* ���ù���ģʽ */
//        SD_SetDeviceMode(SD_DMA_MODE);
//        /* �ļ�ϵͳFATFS ��ʼ�� */
//        disk_initialize(0);
//        /* ��SD�����ص�������0 */
//        f_mount(0, &fs);
//	}
	
	printf("��ʼ�����\r\n");
	
    LD3320_main();		   //LD3320ִ�к���

    NVIC_SystemReset();

}
