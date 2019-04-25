#ifndef __TCPINTERFACE_H
#define __TCPINTERFACE_H

//#include <stdbool.h>
#include <stdint.h>
#include "includes.h"

/*
    TCP�ӿڣ�Լ����������
*/
typedef struct
{
    bool (*Init)(void);
    bool (*Listen)(char *port);
    bool (*Connect)(char *ip, char *port);
    bool (*Disconnect)(void);
    bool (*Send)(u8 *, u16 len);
    int (*Receive)(u8 *, int len);
} TcpInterface;

extern TcpInterface TcpFunc;
extern bool NetLink_Flag;
//���ڱ�����
extern TcpInterface ESP8266_driver;

/* �������� */
bool Get_Network_Status(void);

void Register_TCPDriver(TcpInterface *tcp_driver);

#endif

