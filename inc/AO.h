
#include "FreeRTOS.h"
#include "task.h"
#include "semaphore.h"
#include "sapi.h"
#include "queue.h"

typedef struct 
{
    char* string;
    QueueHandle_t colaCapa3;

}activeObjectResponse_t;

typedef void (*callBackActObj_t) (activeObjectResponse_t*);

typedef struct 
{
    TaskFunction_t taskName;
    QuedeHandle_t activeObjectQueue;
    callBackActObj_t callbackFunc;
    bool_t isAlive;

}activeObject_t;

