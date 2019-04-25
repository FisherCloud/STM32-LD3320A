#include "TCPinterface.h"

TcpInterface TcpFunc;   //Tcp funcition for external user

bool NetLink_Flag = false;

/*
    ��ȡ����״̬
    ping���Կ��Ƿ���������
*/
bool Get_Network_Status(void)
{
    ESP8266_Exit_Transfer(10);//��ֹ��͸��ģʽ�£����˳�͸��
    delay_ms(800);//���ɴ˴���ʱ��Ҫʱ��᳤
    NetLink_Flag = ESP8266_Ping("www.baidu.com", 8);//Ping �ٶȣ�Ҫ4�����л�Ӧ��·�ɶ���������PING���ܻ���,�ٶȷ�������ʱҲ��ӵ�

    return NetLink_Flag;
}

/*
    Regsister function for external user
*/
void Register_TCPDriver(TcpInterface *tcp_driver)
{
    TcpFunc.Init = tcp_driver->Init;
    TcpFunc.Listen = tcp_driver->Listen;
    TcpFunc.Connect = tcp_driver->Connect;
    TcpFunc.Disconnect = tcp_driver->Disconnect;
    TcpFunc.Send = tcp_driver->Send;
    TcpFunc.Receive = tcp_driver->Receive;
}

