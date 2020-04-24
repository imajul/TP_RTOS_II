
#include "sapi.h"
#include "AO.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "app.h"
#include "sep.h"

bool_t activeObjectCreate(activeObject_t *ao, callBackActObj_t callback)
{
    if(ao->isCreated == true)
    {
        return true;
    }

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
        ao->isCreated = true;
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
    
    while ( xQueuePeek(ao.activeObjectQueue, &value, 0) == pdPASS )  // verifico si hay datos en la cola
    {
        xQueueReceive(ao.activeObjectQueue, &value, 0);    // recibo el dato a procesar
        ao.callbackFunc(&value);                           // llamo al callback del AO
        xQueueSend(value.cola, (void *)&value, 0);                 // envio la trama procesada a la cola de respuestas
    }

    activeObjectDelete(&ao);        // si la cola esta vacia, destruyo el AO
}

void activeObjectEnqueue(activeObject_t *ao, AOresponse_t *val)
{
    xQueueSend(ao->activeObjectQueue, (void *)val, 0);
}

void activeObjectInit(activeObject_t *ao)
{
    ao->isCreated == false;                   // seteo el flag de vida
}

void activeObjectDelete(activeObject_t *ao)
{
    vQueueDelete(ao->activeObjectQueue);   // elimino la cola
    vTaskDelete(activeObjectTask);         // elimino la tarea 
    ao->isCreated == false;                  // borro el flag de vida
}
