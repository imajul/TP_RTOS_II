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
//#include "FreeRTOS.h"
//#include "task.h"

#include "sep.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/
void sepInit(uartManagerHandle_t uartHandle, sepHandle_t *sepHandle)
{
	sepHandle->uartHandle = uartHandle;

}

sepError_t sepGet(sepHandle_t *handle, sepData_t* data, uint32_t timeout)
{
	sepError err = SEP_ERROR;
	uint32_t size;
	uint8_t *msg;

	if (uartManagerGet( handle.uartHandle, NULL, &size, timeout))
	{
		msg = (uint8_t *)pvPortMalloc(size);

		uartManagerGet( handle.uartHandle, msg, &size, timeout);

		switch (msg[0])
		{
			case 'm':
			data->event = TO_LOWER;
			break;

			case 'M':
			data->event = TO_UPPER;
			break;

			default:
			data->event = TO_UNDEFINED;
			break;
		}

	}
	return SEP_OK;
}


sepError_t sepPut(sepHandle_t *handle, sepData_t* data, uint32_t timeout)
{

	return SEP_OK;
}


/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

