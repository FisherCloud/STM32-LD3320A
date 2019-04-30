#ifndef __HANDLE_H
#define __HANDLE_H

#include "includes.h"

#define STREAM_LED1    "{\"datastreams\":[{\"id\":\"LED1\", \"datapoints\":[{\"value\": %d}]}]}"
#define STREAM_LED2    "{\"datastreams\":[{\"id\":\"LED2\", \"datapoints\":[{\"value\": %d}]}]}"
#define STREAM_LED3    "{\"datastreams\":[{\"id\":\"LED3\", \"datapoints\":[{\"value\": %d}]}]}"
#define STREAM_RELAY   "{\"datastreams\":[{\"id\":\"RELAY\", \"datapoints\":[{\"value\": %d}]}]}"
#define STREAM_TEM     "{\"datastreams\":[{\"id\":\"TEMP\", \"datapoints\":[{\"value\": %d}]}]}"

typedef struct
{
    uint8_t LED1;
    uint8_t LED2;
    uint16_t LED3;
    uint8_t RELAY;
    uint8_t TEMP;
} STRUCT_SENSOR;

extern bool UpFlag;
extern STRUCT_SENSOR SensorValue;

void Upload_Datastreams(STRUCT_SENSOR sensor);
void Upload_JSON_Data(STRUCT_SENSOR sensor);
void TimeForUpload(void);
void Upload_Handle(void);

#endif

