
#include "AO.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semaphore.h"
#include "sapi.h"
#include "queue.h"

bool_t activeObjectCreate( activeObject_t* ao, callBackActObj_t callback, TaskFunction_t taskForAO)
{
    BaseType_t retValue; 

    ao->activeObjectQueue = xQueueCreate(10, sizeof(activeObjectResponse_t));
    ao->taskName = taskForAO;

    if(ao->activeObjectQueue != NULL)
    {
        ao->callbackFunc = callback;

        retValue = xTaskCreate( ao->taskName,
                                 (const char *)"tarea del AO",
                                 configMINIMAL_STACK_SIZE*2,
                                 ao,
                                 taskIDLE_PRIORITY+2,
                                 NULL       );
    }

    if(retValue == pdPASS)
    {
        ao->isAlive = true;
        return true;
    }
    else
    {
        return false;
    }
}

void activeObjectTask( void* pvParameters)
{

}

void activeObjectEnqueue( activeObject_t* ao, activeObjectResponse_t* value)
{
    xQueueSend(ao->activeObjectQueue, value, 0);
}