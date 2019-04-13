/***************************乐声电子科技有限公司****************************
**  工程名称：YS-V0.3语音识别开发板 测试程序
**	CPU: STC89LE52
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：YS-V0.3语音识别开发板
**                http://yuesheng001.taobao.com
**  作者：zdings
**  联系：751956552@qq.com
**  修改日期：2012.4.1
**  说明：
***************************乐声电子科技有限公司******************************/
#ifndef __LEDTEST_H__
#define __LEDTEST_H__


void Glide_LED(void);
void Flicker_LED(void);
void Key_LED(void);
void Off_LED(void);
void Board_text(uint8 Code_Val);


#endif
