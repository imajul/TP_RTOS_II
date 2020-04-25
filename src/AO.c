/*=====[Module Name]===========================================================
 * Copyright YYYY Author Compelte Name <author@mail.com>
 * All rights reserved.
 * License: license text or at least name and link 
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Creation Date: YYYY/MM/DD
 */

/*=====[Inclusion of own header]=============================================*/
#include "AO.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/
void aoTask(void *pvParameters);

/*=====[Implementations of public functions]=================================*/
bool aoCreate(ao_t* ao, callbackAO_t callback)
{
	if(ao->isCreated == true)
		return true;

	ao->queue = xQueueCreate(10, sizeof(aoData_t));
	if (ao->queue == NULL)
		return false;

	ao->callback = callback;
	ao->isCreated = true;
	ao->handle = NULL;

	return true;
}

bool aoIsCreated(ao_t* ao)
{
	return ao->isCreated;
}

void aoEnqueue(ao_t* ao, aoData_t aoData)
{
	BaseType_t err;

	if(ao->isCreated)
	{
		xQueueSend(ao->queue, &aoData, 0);

		if(ao->handle == NULL)
		{
			err = xTaskCreate(aoTask,
					"aoTask",
					configMINIMAL_STACK_SIZE * 2,
					ao,
					(tskIDLE_PRIORITY + 2UL),
					&ao->handle);

			if(err != pdTRUE)
			{
				vQueueDelete(ao->queue);
				ao->isCreated = false;
			}
		}
	}
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/
void aoTask(void *pvParameters)
{
    ao_t* ao = (ao_t*)pvParameters;
    aoData_t aoData;

    if(ao->isCreated)
    {
    	while( uxQueueMessagesWaiting(ao->queue) )
    	{
    		xQueueReceive(ao->queue, &aoData, 0);
			ao->callback(&aoData);
    	}

    	ao->isCreated = false;
    	ao->handle = NULL;

    	vQueueDelete(ao->queue);
    	vTaskDelete(NULL);
    }
}

