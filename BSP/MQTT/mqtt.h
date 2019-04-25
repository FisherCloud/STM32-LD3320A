#ifndef __MQTT_H
#define __MQTT_H

#include "includes.h"


/* Platform */
#define CLIENT_ID       "3332838"
#define USER_NAME       "minecraft"
#define PASS_WORD       "woshiyuxin"
#define SERVER_ADDR     "119.23.61.148"
#define SERVER_PORT     "61613"
/* MQTT need */
#define SUBTOPIC        "device/in"
#define PUBTOPIC        "device/out"
#define ONENET_TOPIC    "$dp"           //- -#


typedef struct
{
    uint8_t Pack[255];  //buff use for mqtt pack
    uint8_t *Data;      //data stream p for mqtt data
    uint16_t DataLen;   //length of data steam
    bool LinkFlag;
    uint8_t pingCnt;    //number of ping, clean after receive PINGRESP
} MQTT_TRANSFER_STRUCT;


extern MQTT_TRANSFER_STRUCT Mqtt;


/* 函数声明 */
bool Mqtt_Connect(void);
void Mqtt_Disconnect(void);
bool Mqtt_Publish(char *topic, unsigned char *payload, uint16_t payloadlen);
bool Mqtt_Subscribe(void);
void Mqtt_DATA_handle(void);
void Mqtt_Ping(void);

#endif

