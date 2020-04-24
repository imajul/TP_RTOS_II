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
#include <string.h>
#include <ctype.h>
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "sep.h"
#include "uartManager.h"
#include "AO.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/



/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/
static void rxTaskAO(void *pvParameters);

void toLowercallback(AOresponse_t *aoResponse);
void toUppercallback(AOresponse_t *aoResponse);

/*=====[Implementations of public functions]=================================*/
void appInit(void)
{
	uartManagerConfig_t uartConfig;
	static uartManagerHandle_t uartHandle;

	static sepHandle_t sepHandle;

	uartConfig.baudRate = 115200;
	uartConfig.uart = UART_USB;
	uartConfig.chStart = '(';
	uartConfig.chEnd = ')';
	uartConfig.msgMaxLen = 32;

	uartManagerInit(&uartHandle, uartConfig);

	sepInit(&sepHandle, uartHandle);

	xTaskCreate(
		rxTaskAO,
		(char *)"rxTaskAO",
		configMINIMAL_STACK_SIZE * 2,
		(void *)&sepHandle,
		(tskIDLE_PRIORITY + 1UL),
		NULL);
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/
static void rxTaskAO(void *pvParameters)
{
	sepHandle_t handle = *(sepHandle_t *)pvParameters;
	uint32_t size;
	sepData_t data;
	uint8_t *ptr;

	activeObject_t toLowerAO, toUpperAO;            
	AOresponse_t packetResponse, auxResponse;  		// estructura de datos que intercambio con el AO
	
	activeObjectInit(&toLowerAO);					// inicializo los AO
	activeObjectInit(&toUpperAO);

	packetResponse.cola = xQueueCreate(10, sizeof(AOresponse_t));

	if (packetResponse.cola != NULL)
	{
		while (1)
		{
			if (sepGet(&handle, &data, UINT32_MAX))
			{
				packetResponse.data.msg = data.msg;
				ptr = data.msg;

				switch (data.event)
				{
				case TO_LOWER:

					activeObjectCreate(&toLowerAO, toLowercallback);  // instancio el AO (si no existe)
					activeObjectEnqueue(&toLowerAO, &packetResponse); // manda el string para procesar
					break;

				case TO_UPPER:

					activeObjectCreate(&toUpperAO, toUppercallback);  // creo el AO (si no existe)
					activeObjectEnqueue(&toUpperAO, &packetResponse); // encolar
					break;
				}

				if (xQueueReceive(packetResponse.cola, &auxResponse, 0)) // recibo la respuesta procesada
				{
					sepPut(&handle, &(packetResponse.data), 0);			// envio a capa SEP
				}
			}
		}
	}
}

void toLowercallback(AOresponse_t *aoResponse)
{
	uint8_t *ptr = aoResponse->data.msg;

	while (*ptr != '\0')
	{
		if ((*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z'))
		{
			*ptr = tolower(*ptr);
			ptr++;
		}
		else
		{
			strcpy(aoResponse->data.msg, "ERROR\0");
			aoResponse->data.event = TO_ERROR;
			break;
		}
	}
}

void toUppercallback(AOresponse_t *aoResponse)
{
	uint8_t *ptr = aoResponse->data.msg;

	while (*ptr != '\0')
	{
		if ((*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z'))
		{
			*ptr = toupper(*ptr);
			ptr++;
		}
		else
		{
			strcpy(aoResponse->data.msg, "ERROR\0");
			aoResponse->data.event = TO_ERROR;
			break;
		}
	}
}