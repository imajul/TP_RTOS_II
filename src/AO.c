
#include "AO.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semaphore.h"
#include "sapi.h"
#include "queue.h"

bool_t activeObjectCreate(activeObject_t *ao, callBackActObj_t callback, TaskFunction_t taskForAO)
{
    BaseType_t retValue;

    ao->activeObjectQueue = xQueueCreate(10, sizeof(activeObjectResponse_t));
    ao->taskName = taskForAO;

    if (ao->activeObjectQueue != NULL)
    {
        ao->callbackFunc = callback;

        retValue = xTaskCreate(ao->taskName,
                               (const char *)"tarea del AO",
                               configMINIMAL_STACK_SIZE * 2,
                               ao,
                               taskIDLE_PRIORITY + 2,
                               NULL);
    }

    if (retValue == pdPASS)
    {
        ao->isAlive = true;
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
    activeObjectResponse_t value;
    xQueueReceive(ao.activeObjectQueue, &value, portMAX_DELAY);
    ao.callbackFunc(&value); // llamo al callback del AO
    
}

void activeObjectEnqueue(activeObject_t *ao, activeObjectResponse_t *value)
{
    xQueueSend(ao->activeObjectQueue, value, 0);
}

void toLowercallback(activeObjectResponse_t* aoResponse)
{
    uint8_t *ptr = aoResponse->data.msg;

    while (*ptr != '\0')
    {
        if ((*ptr >= 'A') && (*ptr <= 'Z'))
        {
            *ptr = tolower(*ptr);
            ptr++;
        }
        else
        {
            strcpy(data.msg, "ERROR\0");
            data.event = TO_ERROR;
            break;
        }
    }
    xQueueSend( aoResponse.cola, aoResponse, 0);
}

void toLowercallback(activeObjectResponse_t* aoResponse)
{
    uint8_t *ptr = aoResponse->data.msg;

    while (*ptr != '\0')
    {
        if ((*ptr >= 'A') && (*ptr <= 'Z'))
        {
            *ptr = toupper(*ptr);
            ptr++;
        }
        else
        {
            strcpy(data.msg, "ERROR\0");
            data.event = TO_ERROR;
            break;
        }
    }
    xQueueSend( aoResponse.cola, aoResponse, 0);
}