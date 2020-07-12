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

static void toLowercallback(void* pvParameters);
static void toUppercallback(void* pvParameters);

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
	uartConfig.msgMaxLen = 64;

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
	sepData_t sepData;
	
	static ao_t aoToLower = {0};
	static ao_t aoToUpper = {0};
	aoData_t aoData = {0};

	while (1)
	{
		if (sepGet(&handle, &sepData, UINT32_MAX))
		{
			aoData.msg = sepData.msg;
			aoData.handle = &handle;

			switch (sepData.event)
			{
				case TO_LOWER:
					if( aoIsCreated(&aoToLower) == false )
						aoCreate(&aoToLower, toLowercallback);

					aoEnqueue(&aoToLower, aoData);
					break;

				case TO_UPPER:
					if( aoIsCreated(&aoToUpper) == false )
						aoCreate(&aoToUpper, toUppercallback);

					aoEnqueue(&aoToUpper, aoData);
					break;
			}
		}
	}
}

static void toLowercallback(void* pvParameters)
{
	aoData_t aoData = *(aoData_t*)pvParameters;

	sepHandle_t* handle = (sepHandle_t*)aoData.handle;
	uint8_t* ptr = aoData.msg;

	sepData_t sepData;

	sepData.msg = aoData.msg;
	sepData.event = TO_LOWER;

	while(*ptr != '\0')
	{
		if( (*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z') )
		{
			*ptr = tolower(*ptr);
			ptr++;
		}
		else
		{
			strcpy(sepData.msg, "ERROR\0");
			sepData.event = TO_ERROR;
			break;
		}
	}

	sepPut(handle, &sepData, 0);
}

static void toUppercallback(void* pvParameters)
{
	aoData_t aoData = *(aoData_t*)pvParameters;

	sepHandle_t* handle = (sepHandle_t*)aoData.handle;
	uint8_t* ptr = aoData.msg;

	sepData_t sepData;

	sepData.msg = aoData.msg;
	sepData.event = TO_UPPER;

	while(*ptr != '\0')
	{
		if( (*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z') )
		{
			*ptr = toupper(*ptr);
			ptr++;
		}
		else
		{
			strcpy(sepData.msg, "ERROR\0");
			sepData.event = TO_ERROR;
			break;
		}
	}

	sepPut(handle, &sepData, 0);
}
