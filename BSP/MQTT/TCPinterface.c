#include "TCPinterface.h"

TcpInterface TcpFunc;   //Tcp funcition for external user

bool NetLink_Flag = false;

/*
    获取联网状态
    ping可以看是否真能上网
*/
bool Get_Network_Status(void)
{
    ESP8266_Exit_Transfer(10);//防止在透传模式下，先退出透传
    delay_ms(800);//怀疑此处有时需要时间会长
    NetLink_Flag = ESP8266_Ping("www.baidu.com", 8);//Ping 百度，要4秒内有回应，路由断线重连后，PING可能会慢,百度服务器有时也会坑的

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

