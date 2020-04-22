
#include "AO.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "queue.h"
#include "app.h"

bool_t activeObjectCreate(activeObject_t *ao, callBackActObj_t callback)
{
    BaseType_t retValue;

    ao->activeObjectQueue = xQueueCreate(10, sizeof(AOresponse_t));
    

    if (ao->activeObjectQueue != NULL)
    {
        ao->callbackFunc = callback;

        retValue = xTaskCreate(activeObjectTask,
                               (const char *)"tarea del AO",
                               configMINIMAL_STACK_SIZE * 2,
                               ao,
							   (tskIDLE_PRIORITY + 2UL),
                               NULL);
    }

    if (retValue == pdPASS)
    {
        //ao->isAlive = true;
        return true;
    }
    else
    {
        return false;
    }
}

void activeObjectTask(void *pvParameters)
{
    activeObject_t ao = *(activeObject_t *)pvParameters;
    AOresponse_t value;
    
    while (TRUE)
    {
        xQueueReceive(ao.activeObjectQueue, &value, portMAX_DELAY); 
        ao.callbackFunc(&value); // llamo al callback del AO
        xQueueSend(value.cola, &value, 0); 
    } 
}

void activeObjectEnqueue(activeObject_t *ao, AOresponse_t *val)
{
    xQueueSend(ao->activeObjectQueue, val, 0);
}

