#include "handle.h"

bool UpFlag = false;
STRUCT_SENSOR SensorValue = {0, 0, 0, 0, 0};

/*
    上传数据流
*/
void Upload_Datastreams(STRUCT_SENSOR sensor)
{
    uint8_t tmp[100];

    /* Header */
    tmp[0] = 1; //type 1
    tmp[1] = 0;

    sprintf((char *)(tmp + 3), (const char*)STREAM_LED1, sensor.LED1);
    tmp[2] = strlen((const char*)(tmp + 3));
    Mqtt_Publish(ONENET_TOPIC, tmp, tmp[2] + 3); //send
    memset((tmp + 3), 0, 97);

    sprintf((char *)(tmp + 3), (const char*)STREAM_LED2, sensor.LED2);
    tmp[2] = strlen((const char*)(tmp + 3));
    Mqtt_Publish(ONENET_TOPIC, tmp, tmp[2] + 3); //send
    memset((tmp + 3), 0, 97);

    sprintf((char *)(tmp + 3), (const char*)STREAM_LED3, sensor.LED3);
    tmp[2] = strlen((const char*)(tmp + 3));
    Mqtt_Publish(ONENET_TOPIC, tmp, tmp[2] + 3); //send
    memset((tmp + 3), 0, 97);

    sprintf((char *)(tmp + 3), (const char*)STREAM_RELAY, sensor.RELAY);
    tmp[2] = strlen((const char*)(tmp + 3));
    Mqtt_Publish(ONENET_TOPIC, tmp, tmp[2] + 3); //send
    memset((tmp + 3), 0, 97);

    sprintf((char *)(tmp + 3), (const char*)STREAM_TEM, sensor.TEMP);
    tmp[2] = strlen((const char*)(tmp + 3));
    Mqtt_Publish(ONENET_TOPIC, tmp, tmp[2] + 3); //send
    memset((tmp + 3), 0, 97);

}

/*
    通过MQTT pub json格式数据
*/
void Upload_JSON_Data(STRUCT_SENSOR sensor)
{
    cJSON *root;
    char *out;

    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "LED1", cJSON_CreateNumber(sensor.LED1));
    cJSON_AddItemToObject(root, "LED2", cJSON_CreateNumber(sensor.LED2));
    cJSON_AddItemToObject(root, "LED3", cJSON_CreateNumber(sensor.LED3));
    cJSON_AddItemToObject(root, "RELAY", cJSON_CreateNumber(sensor.RELAY));
	cJSON_AddItemToObject(root, "TEMP", cJSON_CreateNumber(sensor.TEMP));
	
    out = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    Mqtt_Publish(PUBTOPIC, (unsigned char *)out, strlen(out));
    printf("%s\r\n", (unsigned char *)out);
    free(out);

}


/*
    Upload data handle
*/
void Upload_Handle(void)
{
    if(UpFlag == true)
    {
        Upload_Datastreams(SensorValue);
        Upload_JSON_Data(SensorValue);
        UpFlag = false;
    }
}

void TimeForUpload(void)
{
    static uint16_t count;
    count ++;

    if(count >= 3000)
    {
        UpFlag = true;
        count = 0;
    }
}

