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
 * Create a single persistent task which periodically dynamically creates another
 * two tasks. The original task is called the creator task, one of the created tasks
 * is called the dummy task, and one is called the deleter task.
 *
 * The deleter task will delete the dummy task before deleting itself, before the
 * creator task creates them again.
 */

/* PortSpecifics.h includes some 'declare once' declarations. */
#define CREATE_DELETE_TASK_C

#include <stdlib.h>
#include <limits.h>

/* Scheduler include files. */
#include "SafeRTOS_API.h"

/* Demo program include files. */
#include "TaskCreateDeleteDemo.h"
#include "PortSpecifics.h"


/*-----------------------------------------------------------------------------
 * Local function prototypes
 *---------------------------------------------------------------------------*/

/* The task originally created which is responsible for periodically dynamically
 * creating another pair of tasks. */
static void vCreatorTask( void *pvParameters );

/* The task function of the dynamically created tasks. */
static void vDummyTask( void *pvParameters );
static void vDeleterTask( void *pvParameters );


/*-----------------------------------------------------------------------------
 * Local variables
 *---------------------------------------------------------------------------*/

/* A variable which is incremented every time the dynamic tasks are created. This
 * is used to check that the task is still running. */
portspecCREATE_DELETE_DATA_SECTION static volatile portUnsignedBaseType uxCreationCount = 0U;

/* A variable that is incremented within the task delete hook so the create task
 * can ensure the same number of tasks are deleted as created. */
portspecCREATE_DELETE_DATA_SECTION volatile portUnsignedBaseType uxCreateDeleteDemoDeleteCount = 0U;

/* Used to log the detection of any error. */
portspecCREATE_DELETE_DATA_SECTION static volatile portBaseType xErrorOccurred = pdFALSE;

/* Handle of the created task. */
portspecCREATE_DELETE_DATA_SECTION static portTaskHandleType xCreatedTask = NULL;

/* Task TCBs. */
portspecTCB_DATA_SECTION static xTCB xCreateTaskTCB = { 0 };
portspecTCB_DATA_SECTION xTCB xDummyTaskTCB = { 0 };
portspecTCB_DATA_SECTION xTCB xDeleterTaskTCB = { 0 };

/* Static as its address is passed as the task parameter. */
portspecCREATE_DELETE_DATA_SECTION static portUnsignedBaseType uxPriorityParameter = 0U;


/*-----------------------------------------------------------------------------
 * Function definitions
 *---------------------------------------------------------------------------*/

portBaseType xStartTaskCreateDeleteDemo( portUnsignedBaseType uxPriority )
{
    portBaseType xStatus = pdPASS;
    xTaskParameters xCreatorTaskParameters =
    {
        &vCreatorTask,                      /* Task code */
        "CREATOR",                          /* Task name */
        &xCreateTaskTCB,                    /* TCB */
        acCreateTaskStack,                  /* Stack buffer */
        portspecCREATE_TASK_STACK_SIZE,     /* Stack depth bytes */
        ( void * )( &uxPriorityParameter ), /* Parameters */
        taskIDLE_PRIORITY,                  /* Priority */
        NULL,                               /* TLS object */
        xCreateTaskPortParameters           /* Port-specific task parameters */
    };

    /* Create the Creator tasks - passing in as a parameter the priority at which
     * the other tasks should be created. */
    uxPriorityParameter = uxPriority;
    xCreatorTaskParameters.uxPriority = uxPriority;
    if( xTaskCreate( &xCreatorTaskParameters, NULL ) != pdPASS )
    {
        xStatus = pdFAIL;
    }

    return xStatus;
}
/*---------------------------------------------------------------------------*/

static void vDummyTask( void *pvParameters )
{
    volatile portInt32Type lFirstDummyVariable;
    volatile portInt32Type lSecondDummyVariable;
    portInt32Type lResult;
    const portTickType xDelay = ( portTickType ) 200U / configTICK_RATE_MS;

    ( void ) pvParameters;

    for( ;; )
    {
        /* Do something random just to use some stack and registers. */
        lFirstDummyVariable = 2L;
        lSecondDummyVariable = 89L;
        lResult = lFirstDummyVariable;
        lResult *= lSecondDummyVariable;
        lSecondDummyVariable = lResult;

        if( xTaskDelay( xDelay ) != pdPASS )
        {
            xErrorOccurred = pdTRUE;
        }
    }
}
/*---------------------------------------------------------------------------*/

static void vDeleterTask( void *pvParameters )
{
    volatile portInt32Type lFirstDummyVariable;
    volatile portInt32Type lSecondDummyVariable;
    portInt32Type lResult;
    portTaskHandleType xTaskToDelete = *( portTaskHandleType * ) pvParameters;
    const portTickType xDelay = ( portTickType ) 200U / configTICK_RATE_MS;

    for( ;; )
    {
        /* Do something random just to use some stack and registers. */
        lFirstDummyVariable = 2L;
        lSecondDummyVariable = 89L;
        lResult = lFirstDummyVariable;
        lResult *= lSecondDummyVariable;
        lSecondDummyVariable = lResult;

        if( xTaskDelay( xDelay ) != pdPASS )
        {
            xErrorOccurred = pdTRUE;
        }

        if( NULL != xTaskToDelete )
        {
            /* Make sure the other task has a go before we delete it. */
            if( xTaskDelay( ( portTickType ) 0U ) != pdPASS )
            {
                xErrorOccurred = pdTRUE;
            }

            /* Delete the other task that was created by vCreatorTask(). */
            if( xTaskDelete( xTaskToDelete ) != pdPASS )
            {
                xErrorOccurred = pdTRUE;
            }

            /* Delete ourselves. */
            if( xTaskDelete( NULL ) != pdPASS )
            {
                xErrorOccurred = pdTRUE;
            }
        }
    }
}
/*---------------------------------------------------------------------------*/

static void vCreatorTask( void *pvParameters )
{
    const portTickType xDelay = ( portTickType ) 1000 / configTICK_RATE_MS;
    portUnsignedBaseType uxPriority;
    portUnsignedBaseType uxExpectedDeleteCount = 0U;

    xTaskParameters xCreatedDummyTaskParameters =
    {
        &vDummyTask,                        /* Task code */
        "tcdDUMMY",                         /* Task name */
        &xDummyTaskTCB,                     /* TCB */
        acCreatedDummyTaskStack,            /* Stack buffer */
        portspecCREATED_TASKS_STACK_SIZE,   /* Stack depth bytes */
        NULL,                               /* Parameters */
        taskIDLE_PRIORITY,                  /* Priority */
        NULL,                               /* TLS object */
        xCreatedDummyTaskPortParameters     /* Port-specific task parameters */
    };
    xTaskParameters xDeleterTaskParameters =
    {
        &vDeleterTask,                      /* Task code */
        "tcdDELETER",                       /* Task name */
        &xDeleterTaskTCB,                   /* TCB */
        acDeleterTaskStack,                 /* Stack buffer */
        portspecCREATED_TASKS_STACK_SIZE,   /* Stack depth bytes */
        &xCreatedTask,                      /* Parameters */
        taskIDLE_PRIORITY,                  /* Priority */
        NULL,                               /* TLS object */
        xDeleterTaskPortParameters          /* Port-specific task parameters */
    };

    uxPriority = *( ( portUnsignedBaseType * ) pvParameters );
    xCreatedDummyTaskParameters.uxPriority = uxPriority;
    xDeleterTaskParameters.uxPriority = uxPriority;

    for( ;; )
    {
        /* Just loop round, delaying then creating the two tasks. */
        if( xTaskDelay( xDelay ) != pdPASS )
        {
            xErrorOccurred = pdTRUE;
        }

        /* Check the task delete function has been called the expected number
         * of times. */
        if( uxExpectedDeleteCount != uxCreateDeleteDemoDeleteCount )
        {
            xErrorOccurred = pdTRUE;
        }

        if( xTaskCreate( &xCreatedDummyTaskParameters, &xCreatedTask ) != pdPASS )
        {
            xErrorOccurred = pdTRUE;
        }
        if( xTaskCreate( &xDeleterTaskParameters, NULL ) != pdPASS )
        {
            xErrorOccurred = pdTRUE;
        }

        ++uxCreationCount;

        /* When we run again we expect the two tasks just created to have
         * been deleted. */
        uxExpectedDeleteCount += 2U;
    }
}
/*---------------------------------------------------------------------------*/

/* This is called to check that the creator task is still running and that there
 * are not any more than two extra tasks. */
portBaseType xIsCreateTaskStillRunning( void )
{
    portspecCOMMON_PRIV_DATA_SECTION static portUnsignedBaseType uxLastCreationCount = ( portUnsignedBaseType ) ULONG_MAX;
    portBaseType xReturn = pdTRUE;

    if( uxLastCreationCount == uxCreationCount )
    {
        xReturn = pdFALSE;
    }
    else
    {
        uxLastCreationCount = uxCreationCount;
    }

    if( pdTRUE == xErrorOccurred )
    {
        xReturn = pdFALSE;
    }
    else
    {
        /* Everything is okay. */
    }

    return xReturn;
}
/*---------------------------------------------------------------------------*/
