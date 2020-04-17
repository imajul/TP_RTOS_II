
#include "FreeRTOS.h"
#include "task.h"
#include "semaphore.h"
#include "sapi.h"
#include "queue.h"

typedef struct 
{
    sepData_t data;
    QueueHandle_t colaCapa3;

}activeObjectResponse_t;

typedef void (*callBackActObj_t) (activeObjectResponse_t*);

typedef struct 
{
    TaskFunction_t taskName;
    QueueHandle_t activeObjectQueue;
    callBackActObj_t callbackFunc;
    bool_t isAlive;

}activeObject_t;

bool_t activeObjectCreate( activeObject_t* ao, callBackActObj_t callback, TaskFunction_t taskForAO);

void activeObjectTask( void* pvParameters);

void activeObjectEnqueue( activeObject_t* ao, activeObjectResponse_t* value);

