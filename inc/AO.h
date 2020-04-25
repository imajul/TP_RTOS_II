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
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/  

/*=====[Definitions of public data types]====================================*/
typedef struct {
	uint8_t* msg;
	void* handle;
}aoData_t;

typedef void (*callbackAO_t)(void*);

typedef struct {
    QueueHandle_t queue;
    TaskHandle_t handle;
    callbackAO_t callback;
    bool isCreated;
}ao_t;

/*=====[Prototypes (declarations) of public functions]=======================*/
bool aoCreate(ao_t* ao, callbackAO_t callback);
bool aoIsCreated(ao_t* ao);
void aoEnqueue(ao_t* ao, aoData_t aoData);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _AO_H_ */



