/*
    Copyright (C)2006 onward WITTENSTEIN high integrity systems Limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorise use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems Limited, Registered Office: Brown's Court,
    Long Ashton Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 1275 395 600
    Email: info@highintegritysystems.com
    www.highintegritysystems.com
*/

/*
 * Creates a number of tasks which each flash an LED at a fixed rate, providing
 * instant visual feedback that the scheduler is still operational.
 *
 * The exact number of LED flash tasks created, the LED that they toggle and the
 * rate at which the LED is toggled are determined within PortSpecifics.c and
 * PortSpecifics.h.
 */

/* PortSpecifics.h includes some 'declare once' declarations. */
#define FLASH_LED_C

#include <stdlib.h>

/* Scheduler include files. */
#include "SafeRTOS_API.h"

/* Demo program include files. */
#include "flash.h"
#include "PortSpecifics.h"
#include "partest.h"

/*---------------------------------------------------------------------------*/

/* The task that is created to toggle the LED. */
static void vLEDFlashTask( void *pvParameters );

/* LED Flash Task TCBs. */
portspecTCB_DATA_SECTION static xTCB xLEDFlashTaskTCBs[ portspecNUMBER_OF_LEDS ] = { { 0 } };

/*---------------------------------------------------------------------------*/

portBaseType xStartLEDFlashTasks( portUnsignedBaseType uxPriority )
{
    portBaseType xStatus = pdPASS;
    portUnsignedBaseType uxLEDTask;
    xTaskParameters xLEDFlashTaskParameters =
    {
        &vLEDFlashTask,                 /* Task code */
        "LEDx",                         /* Task name */
        NULL,                           /* TCB - different for each task. */
        NULL,                           /* Stack buffer - different for each task. */
        portspecLED_TASK_STACK_SIZE,    /* Stack depth bytes */
        NULL,                           /* Parameters - different for each task. */
        taskIDLE_PRIORITY,              /* Priority */
        NULL,                           /* TLS object */
        xLEDFlashTaskPortParameters     /* Port-specific task parameters */
    };

    /* Set up the priority of the LED Flash Tasks. */
    xLEDFlashTaskParameters.uxPriority = uxPriority;

    /* Create each task in turn, assigning it the appropriate parameters. */
    for( uxLEDTask = 0U; uxLEDTask < portspecNUMBER_OF_LEDS; uxLEDTask++ )
    {
        xLEDFlashTaskParameters.pxTCB = &xLEDFlashTaskTCBs[ uxLEDTask ];
        xLEDFlashTaskParameters.pcStackBuffer = acLedTaskStack[ uxLEDTask ];
        xLEDFlashTaskParameters.pvParameters  = ( void * ) &xLedTaskParameters[ uxLEDTask ];

        /* Create a task to toggle the LED. */
        if( xTaskCreate( &xLEDFlashTaskParameters, ( portTaskHandleType * ) NULL ) != pdPASS )
        {
            xStatus = pdFAIL;
        }
    }

    return xStatus;
}
/*---------------------------------------------------------------------------*/

static void vLEDFlashTask( void *pvParameters )
{
    portTickType xFlashLast;
    portTickType xFlashRate;
    portUnsignedBaseType uxFlashLED;
    xLedFlashTaskParameters *pxFlashParameters;

    /* Retrieve task LED identifier & flash rate. */
    pxFlashParameters = ( xLedFlashTaskParameters * ) pvParameters;

    uxFlashLED = pxFlashParameters->uxLEDNumber;
    xFlashRate = pxFlashParameters->xFlashRate_ms;
    xFlashRate = ( xFlashRate / configTICK_RATE_MS );

    /* Initialise xLastFlashTime prior to First Call tovTaskDelayUntil(). */
    xFlashLast = xTaskGetTickCount();

    for(;;)
    {
        /* Delay for flash period, then toggle LED state. */
        ( void ) xTaskDelayUntil( &xFlashLast, xFlashRate );

        vParTestToggleLED( uxFlashLED );
    }
}
/*---------------------------------------------------------------------------*/
