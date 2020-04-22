
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "queue.h"
#include "sep.h"
#include "app.h"


typedef void (*callBackActObj_t)(AOresponse_t *);  // prototipo de la llamada al callback

typedef struct
{
    TaskFunction_t taskName;
    QueueHandle_t activeObjectQueue;
    callBackActObj_t callbackFunc;
    //bool_t isAlive = false;

} activeObject_t;

bool_t activeObjectCreate(activeObject_t *ao, callBackActObj_t callback);

void activeObjectTask(void *pvParameters);

void activeObjectEnqueue(activeObject_t *ao, AOresponse_t *value);


