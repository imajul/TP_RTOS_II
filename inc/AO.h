/*=====[Module Name]===========================================================
 * Copyright YYYY Author Compelte Name <author@mail.com>
 * All rights reserved.
 * License: license text or at least name and link 
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Creation Date: YYYY/MM/DD
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _AO_H_
#define _AO_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "app.h"
#include "queue.h"
#include "FreeRTOS.h"
/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/  

typedef void (*callBackActObj_t)(AOresponse_t *);

/*=====[Definitions of public data types]====================================*/

typedef struct
{
    TaskFunction_t taskName;
    QueueHandle_t activeObjectQueue;
    callBackActObj_t callbackFunc;
    bool_t isCreated;

} activeObject_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

bool_t activeObjectCreate(activeObject_t *ao, callBackActObj_t callback);

void activeObjectTask(void *pvParameters);

void activeObjectEnqueue(activeObject_t *ao, AOresponse_t *value);

void activeObjectDelete(activeObject_t *ao);

void activeObjectInit(activeObject_t *ao);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _AO_H_ */



