#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"

//#define AP_SSID  "PandoraBox"    //无线路由ssid
//#define AP_PASS  "woshiyuxin"   //密码
#define AP_SSID  "506super"    //无线路由ssid
#define AP_PASS  "jkxy1234"   //密码
//#define AP_SSID  "MI9"    //无线路由ssid
//#define AP_PASS  "woshiyuxin"   //密码

#define SSID  "ESP8266"    //ssid
#define PASS  "012345678"           //密码
#define AP_ECN   WPA_WPA2_PSK       //WPA_WPA2_PSK加密方式

//#define SERVER_IP    "119.23.61.148"    //服务器IP
//#define SERVER_PORT   61613             //服务器端口号

#define OPEN            0
#define WEP             1
#define WPA_PSK         2
#define WPA2_PSK        3
#define WPA_WPA2_PSK    4

#define BUFF_LEN        1024
extern u8 Esp8266_DdtaBuff[1024];

u8 ESP8266_STA_TCPServer(void);
u8 ESP8266_STA_TCPCLient(void);
u8 ESP8266_AP_TCPServer(void);
u8 ESP8266_SoftReset(u16 timeout);
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout);
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout);
u8 ESP8266_ConnectionAP(u8 *ssid, u8 *password, u16 timeout);
u8 ESP8266_GetIP(u8 *ip, u16 timeout);
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout);
u8 ESP8266_Tcp_SendData(u8 *databuff, u16 data_len, u16 timeout);
u8 ESP8266_Get_LinkSta(void);
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id);
u8 ESP8266_Tcp_GetData(u8 *data, int *len);

u8 ESP8266_Ping(char *IP, uint8_t time);
u8 ESP8266_DHCP_Setup(void);
u8 ESP8266_Connect_Mode(u8 mode);
u8 ESP8266_TransMode(u8 mode);

u8 ESP8266_Connect_Server(u8 *ip, u16 port, u16 timeout);
u8 ESP8266_Disconnect_Server( u16 timeout);

void ESP8266_Exit_Transfer(u16 timeout);

u16 Char_to_Hex(u8 *char_data, u16 len);
#endif



