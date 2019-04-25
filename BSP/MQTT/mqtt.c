#include "mqtt.h"

bool MqttLink_Flag = false;
TcpInterface *tcp = &TcpFunc; //Global,Get Tcp user funciton

MQTT_TRANSFER_STRUCT Mqtt =
{
    0,
    0,
    0,
    false,
    0
};


/*
    Mqtt Initial
*/
bool Mqtt_Connect(void)
{
    MQTTPacket_connectData pack = MQTTPacket_connectData_initializer;  //Init an connect pack
    uint16_t len;

    Register_TCPDriver(&ESP8266_driver);//使用8266的驱动

    //Serialize connect pack
    pack.MQTTVersion = 4;   //use 3.1.1
    pack.clientID.cstring = CLIENT_ID;
    pack.keepAliveInterval = 20;
    pack.cleansession = 1;
    pack.username.cstring = USER_NAME;
    pack.password.cstring = PASS_WORD;


    len = MQTTSerialize_connect(Mqtt.Pack, sizeof(Mqtt.Pack), &pack);

    //Check network status
    if(Get_Network_Status() == false)
    {
        tcp->Init();//reconnect wifi

        if(Get_Network_Status() == false)
        {
            Mqtt.LinkFlag = false;
            return Mqtt.LinkFlag;
        }
    }

    //Connect Mqtt
    tcp->Disconnect();                     //Disconnect first,because server maybe still keep device alive!
    tcp->Connect(SERVER_ADDR, SERVER_PORT);//Connect Onenet server,had been checked netlink before.
    delay_ms(100);
    tcp->Send(Mqtt.Pack, len);

    delay_ms(300);//Need some time for Server back!

    if(MQTTPacket_read(Mqtt.Pack, sizeof(Mqtt.Pack), tcp->Receive) == CONNACK) //if receive ack,netlink is true too!
    {
        Mqtt.LinkFlag = true;
    }
    else
    {
        Mqtt.LinkFlag = false;
    }

    return Mqtt.LinkFlag;
}

/*
    Close Mqtt
*/
void Mqtt_Disconnect(void)
{
    uint16_t len;

    len = MQTTSerialize_disconnect(Mqtt.Pack, sizeof(Mqtt.Pack));
    tcp->Send(Mqtt.Pack, len);
    Mqtt.LinkFlag = false;
}

/*
    Mqtt pub function
*/
bool Mqtt_Publish(char *topic, unsigned char *payload, uint16_t payloadlen)
{
    MQTTString topicString = MQTTString_initializer;
    uint16_t len;
//    bool err;

    topicString.cstring = topic;
    len = MQTTSerialize_publish(Mqtt.Pack, sizeof(Mqtt.Pack), 0, 0, 0, 0, topicString, payload, payloadlen);
    tcp->Send(Mqtt.Pack, len);

//    USART_SEND_Buf( USART1, Mqtt.Pack, len);
//    USART_SEND_Buf( USART1, "\r\n", strlen("\r\n"));
//    delay_ms(100);//Need some time!
//    if (MQTTPacket_read( Mqtt.Pack, sizeof(Mqtt.Pack), tcp->Receive) == PUBACK) //Qos 0无puback
//        err = true;
//    else
//        err = false;

    return true;
}

/*
    Mqtt sub function
*/
bool Mqtt_Subscribe(void)
{
    MQTTString topicString = MQTTString_initializer;
    int32_t msgid = 1;
    int32_t req_qos = 0;
    uint16_t len;
    bool err;

    topicString.cstring = SUBTOPIC;
    len = MQTTSerialize_subscribe(Mqtt.Pack, sizeof(Mqtt.Pack), 0, msgid, 1, &topicString, &req_qos);
    tcp->Send(Mqtt.Pack, len);
    delay_ms(300);//Need some time for Server back!

    if(MQTTPacket_read(Mqtt.Pack, sizeof(Mqtt.Pack), tcp->Receive) == SUBACK)
    {
        err = true;
    }
    else
    {
        err = false;
    }

    return err;
}

/*
    Send ping to Mqtt server
*/
void Mqtt_Ping(void)
{
    static uint16_t count;
    uint16_t len;

    if(Mqtt.LinkFlag == true)//连接上才保活
    {
        if(Mqtt.pingCnt > 3)//have never receive PINGRESP
        {
            Mqtt.LinkFlag = false;
            Mqtt.pingCnt = 0;
        }
        else
        {
            count ++;

            if(count >= 10000)// 20sec,理论最多ping2次
            {
                count = 0;
                len = MQTTSerialize_pingreq(Mqtt.Pack, sizeof(Mqtt.Pack));
                tcp->Send(Mqtt.Pack, len);
                Mqtt.pingCnt ++;//add one
            }
        }
    }
}


/*
    TCP data for Mqtt handle
*/
enum msgTypes msgType;
void Mqtt_DATA_handle(void)
{
    //enum msgTypes msgType;

    unsigned char dup;
    int qos;
    unsigned char retained;
    unsigned short packetid;
    MQTTString* topicName;


//    if(uart5_ok_flag == true)//Need recv data from uart5(Esp8266 hw interface)
//    {
    msgType = (enum msgTypes)MQTTPacket_read(Mqtt.Pack, sizeof(Mqtt.Pack), tcp->Receive);

    switch(msgType)
    {
    case PUBLISH://receive in

        MQTTDeserialize_publish((unsigned char*)(&dup), \
                                (int*)(&qos), \
                                (unsigned char*)(&retained), \
                                (unsigned short*)(&packetid), \
                                (MQTTString*)(topicName), \
                                (&Mqtt.Data), (int*)(&Mqtt.DataLen), \
                                Mqtt.Pack, sizeof(Mqtt.Pack));
#if 1
        printf("\r\nRecv an pub:%s\r\n", Mqtt.Data);
//			USART_SEND_Buf(USART1, "\r\n", strlen("\r\n"));
//			USART_SEND_Buf(USART1, "Recv an pub:", strlen("Recv an pub:"));
//			USART_SEND_Buf(USART1, Mqtt.Data, Mqtt.DataLen);
//			USART_SEND_Buf(USART1, "\r\n", strlen("\r\n"));
#endif
        //Need add data handle
        break;

    case PINGRESP://Keep connect server
        Mqtt.pingCnt = 0;
        break;

    default:
        break;
    }

//        uart5_ok_flag = false;
//    }
}

