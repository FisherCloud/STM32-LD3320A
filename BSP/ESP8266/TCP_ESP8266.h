#ifndef __TCP_ESP8266_H
#define __TCP_ESP8266_H

#include "includes.h"

#define WIFI_SSID           "506super"
#define WIFI_PASSWORD       "jkxy1234"



/* º¯ÊýÉùÃ÷ */
bool ESP8266_TCP_Init(void);
bool ESP8266_TCP_Listen(char *port);
bool ESP8266_TCP_Connect(char *ip, char *port);
bool ESP8266_TCP_Disconnect(void);
bool ESP8266_TCP_Send(u8 *dat, u16 len);
int ESP8266_TCP_Receive(u8 *rxdat, int len);


#endif
