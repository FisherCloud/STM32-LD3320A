#include "TCP_ESP8266.h"
#include <stdlib.h>

//封装8266的TCP驱动
TcpInterface ESP8266_driver = 
{
    ESP8266_TCP_Init,
    ESP8266_TCP_Listen,
    ESP8266_TCP_Connect,
    ESP8266_TCP_Disconnect,
    ESP8266_TCP_Send,
    ESP8266_TCP_Receive,
};


bool ESP8266_TCP_Init(void)
{
    bool err; 
       
       ESP8266_DHCP_Setup();
       ESP8266_Connect_Mode(1);
       err = ESP8266_ConnectionAP(WIFI_SSID, WIFI_PASSWORD, 10);
    
    return err;
}


bool ESP8266_TCP_Listen(char *port)
{
    bool err;
    
    return err;
}


/* 连上服务器后开启透传模式 */
bool ESP8266_TCP_Connect(char *ip, char *port)
{
    bool err;
    
	u16 p = atoi(port);
	
    err = ESP8266_Connect_Server((u8 *)ip, p, 10);//
    if(err == true)
    {    
        err = ESP8266_TransMode(1);//透传模式
    }
    return err;
}


bool ESP8266_TCP_Disconnect(void)
{
    bool err;
    
    err = ESP8266_Disconnect_Server(2);
    
    return err;
}


bool ESP8266_TCP_Send(u8 *dat, u16 len)
{
    bool err;
    
    err = ESP8266_Tcp_SendData(dat, len, 10);
    
    return err;    
}


/* 
    Tcp buff and len :  Loading by uart5 handle.
    Dara src:  uart5
    Return 
*/

int ESP8266_TCP_Receive(u8 *rxdat, int len)
{
    if (ESP8266_Tcp_GetData(rxdat, &len))
		return len;
	else
		return 0;
}
