/*
    Copyright (C)2006 onward WITTENSTEIN high integrity systems Limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorise use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems Limited, Registered Office: Brown's Court, Long Ashton
    Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 1275 395 600
    Email: info@highintegritysystems.com
    www.highintegritysystems.com
*/

/* Tests the event multiplex functionality. */

/* PortSpecifics.h may include some 'declare once' declarations. */
#define EVT_MPLX_DEMO_C

/* Scheduler include files. */
#include "SafeRTOS_API.h"

/* Demo program include files. */
#include "PortSpecifics.h"
#include "EvtMplxDemo.h"

#if ( configQUEUE_REGISTRY_SIZE > 0 )
    #include "queue_register.h"
#endif

/*---------------------------------------------------------------------------*/
/* Constant Definitions                                                      */
/*---------------------------------------------------------------------------*/
/* The priorities of the tasks created in this file. */
#define evtmplxBLOCK_TASK_PRIORITY                  ( ( portUnsignedBaseType ) 2 )
#define evtmplxCONTROL_TASK_PRIORITY                ( ( portUnsignedBaseType ) 1 )

/* The maximum number of object-events that can be retrieved by a call to
 * xEvtMplxWait(). */
#define evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS      ( ( portUnsignedBaseType ) 3 )

/* Even if an API call is not expected to block, due to the exact timing of the
 * tick interrupt and the processing of other tasks in the demo, it is
 * conceivable that the elapsed time could be non-zero, but it will be *very*
 * short. */
#define evtmplxMINIMUM_ELAPSED_TIME_NON_BLOCKING    ( ( portTickType ) 1 )

/* Initial value of items sent to xEvtMplxQueue1Handle. */
#define evtmplxQUEUE_ITEM_DEFAULT_VALUE             ( ( portUnsignedBaseType ) 0x12345678 )

/* Event bit definitions used with the event multiplex demo event group. */
#define evtmplxEVENT_GROUP_INITIALISED              ( ( eventBitsType ) 0x01 )
#define evtmplxCONTROL_TASK_BIT                     ( ( eventBitsType ) 0x02 )

/* Block times used in this demo. */
#define evtmplxNO_BLOCK_TIME                        ( ( portTickType ) 0 )
#define evtmplxMAX_TICKS_TO_WAIT                    ( ( portTickType ) 50 )


/*---------------------------------------------------------------------------*/
/* Private Function Declarations                                             */
/*---------------------------------------------------------------------------*/
/* The task body of the task that blocks on the event multiplex object. */
static void prvEvtMplxBlockTask( void *pvParameters );

/* The task body of the task that triggers events registered with the event multiplex
 * object. */
static void prvEvtMplxControlTask( void *pvParameters );


/*---------------------------------------------------------------------------*/
/* Private Variables                                                         */
/*---------------------------------------------------------------------------*/
/* TCBs for the tasks created in this file. */
portspecTCB_DATA_SECTION static xTCB xEvtMplxBlockTaskTCB = { 0 };
portspecTCB_DATA_SECTION static xTCB xEvtMplxControlTaskTCB = { 0 };

/* The event multiplex blocking task is deleted and then subsequently recreated,
 * therefore the xTaskParameters structure needs to be file scope. */
portspecCOMMON_PRIV_DATA_SECTION static xTaskParameters xEvtMplxBlockTaskParameters =
{
    &prvEvtMplxBlockTask,                           /* Task Code */
    "Event Multiplex Block Task",                   /* Task Name */
    &xEvtMplxBlockTaskTCB,                          /* TCB */
    acEvtMplxBlockTaskStack,                        /* Stack Buffer */
    portspecEVT_MPLX_BLOCK_TASK_STACK_SIZE,         /* Stack Depth Bytes */
    NULL,                                           /* Parameters */
    evtmplxBLOCK_TASK_PRIORITY,                     /* Priority */
    NULL,                                           /* TLS Object */
    xEvtMplxBlockTaskPortParameters                 /* Port Specific Task Parameters */
};

/* An event group to be used by the event multiplex demo. Note that this is located
 * in "privileged access only" memory as the demo tasks will not access the
 * structure directly but only through API commands. */
portspecCOMMON_PRIV_DATA_SECTION static eventGroupType xEvtMplxDemoEventGroup = { 0 };

/* Handles of the tasks created in this file. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION portTaskHandleType xEvtMplxBlockTaskHandle = NULL;
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static portTaskHandleType xEvtMplxControlTaskHandle = NULL;

/* Handles of the queues used in the event multiplex demo. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static xQueueHandle xEvtMplxQueue1Handle = NULL;
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static xQueueHandle xEvtMplxQueue2Handle = NULL;

/* Handle of the semaphore used in the event multiplex demo. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static xSemaphoreHandle xEvtMplxSemaphoreHandle = NULL;

/* Handle of the mutex used in the event multiplex demo. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static xMutexHandleType xEvtMplxMutexHandle = NULL;

/* The handle by which the event multiplex demo event group is to be accessed. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static eventGroupHandleType xEvtMplxDemoEventGroupHandle = NULL;

/* The handle by which the event multiplex object in this demo is to be accessed. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static evtMplxHandleType xEvtMplxHandle = NULL;

/* Variables used to ensure that the tasks are still executing without error. */
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static volatile portUnsignedBaseType uxEvtMplxBlockTaskCycleCount = 0U;
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static volatile portUnsignedBaseType uxEvtMplxControlTaskCycleCount = 0U;
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static volatile portBaseType xEvtMplxBlockTaskErrorOccurred = pdFALSE;
portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION static volatile portBaseType xEvtMplxControlTaskErrorOccurred = pdFALSE;


/*---------------------------------------------------------------------------*/
/* Public Function Definitions                                               */
/*---------------------------------------------------------------------------*/

portBaseType xStartEvtMplxDemoTasks( void )
{
    portBaseType xStatus = pdPASS;
    xTaskParameters xEvtMplxControlTaskParameters =
    {
        &prvEvtMplxControlTask,                     /* Task Code */
        "Event Multiplex Control Task",             /* Task Name */
        &xEvtMplxControlTaskTCB,                    /* TCB */
        acEvtMplxControlTaskStack,                  /* Stack Buffer */
        portspecEVT_MPLX_CONTROL_TASK_STACK_SIZE,   /* Stack Depth Bytes */
        NULL,                                       /* Parameters */
        evtmplxCONTROL_TASK_PRIORITY,               /* Priority */
        NULL,                                       /* TLS Object */
        xEvtMplxControlTaskPortParameters           /* Port Specific Task Parameters */
    };

    /* Create the first queue that is of interest to the
     * event multiplex blocking task. */
    if( xQueueCreate( acEvtMplxQueue1Buffer,
                      portspecEVT_MPLX_Q_BUFFER_LENGTH,
                      portspecEVT_MPLX_Q_QUEUE_LENGTH,
                      portspecEVT_MPLX_Q_QUEUE_ITEM_SIZE,
                      &xEvtMplxQueue1Handle ) == pdPASS )
    {
#if ( configQUEUE_REGISTRY_SIZE > 0 )
        vQueueAddToRegistry( xEvtMplxQueue1Handle, "Event Multiplex Q1" );
#endif
    }
    else
    {
        xStatus = pdFAIL;
    }

    /* Create the second queue that is of interest to the 
     * event multiplex blocking task. */
    if( xQueueCreate( acEvtMplxQueue2Buffer,
                      portspecEVT_MPLX_Q_BUFFER_LENGTH,
                      portspecEVT_MPLX_Q_QUEUE_LENGTH,
                      portspecEVT_MPLX_Q_QUEUE_ITEM_SIZE,
                      &xEvtMplxQueue2Handle ) == pdPASS )
    {
#if ( configQUEUE_REGISTRY_SIZE > 0 )
        vQueueAddToRegistry( xEvtMplxQueue2Handle, "Event Multiplex Q2" );
#endif
    }
    else
    {
        xStatus = pdFAIL;
    }

    /* Create a binary semaphore that is of interest to the event multiplex blocking
     * task. */
    if( pdPASS != xSemaphoreCreateBinary( acEvtMplxSemaphoreBuffer, &xEvtMplxSemaphoreHandle ) )
    {
        xStatus = pdFAIL;
    }

    /* Create a mutex that is of interest to the event multiplex blocking task. */
    if( pdPASS != xMutexCreate( acEvtMplxMutexBuffer, &xEvtMplxMutexHandle ) )
    {
        xStatus = pdFAIL;
    }

    /* Create an event group that is of interest to the event multiplex blocking
     * task. */
    if( pdPASS != xEventGroupCreate( &xEvtMplxDemoEventGroup, &xEvtMplxDemoEventGroupHandle ) )
    {
        xStatus = pdFAIL;
    }

    /* Set a bit in the event multiplex demo event group. */
    if( pdPASS != xEventGroupSetBits( xEvtMplxDemoEventGroupHandle,
                                      evtmplxEVENT_GROUP_INITIALISED ) )
    {
        xStatus = pdFAIL;
    }

    /* Create the task that will block on an event multiplex object. */
    if( pdPASS != xTaskCreate( &xEvtMplxBlockTaskParameters, &xEvtMplxBlockTaskHandle ) )
    {
        xStatus = pdFAIL;
    }

    /* Initialise the event multiplex object used in this demo, setting the event
     * multiplex block task as the owner. */
    if( pdPASS != xEvtMplxCreate( acEvtMplxBuffer,
                                  portspecEVT_MPLX_BUFFER_LENGTH,
                                  portspecMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                  xEvtMplxBlockTaskHandle,
                                  &xEvtMplxHandle ) )
    {
        xStatus = pdFAIL;
    }


    /* Create the task that will trigger events that will unblock the task
     * waiting on the event multiplex object. */
    if( pdPASS != xTaskCreate( &xEvtMplxControlTaskParameters, &xEvtMplxControlTaskHandle ) )
    {
        xStatus = pdFAIL;
    }

    return xStatus;
}
/*---------------------------------------------------------------------------*/

portBaseType xAreEvtMplxDemoTasksStillRunning( void )
{
    portspecCOMMON_PRIV_DATA_SECTION static portUnsignedBaseType uxEvtMplxBlockTaskLastCycleCount = 0U;
    portspecCOMMON_PRIV_DATA_SECTION static portUnsignedBaseType uxEvtMplxControlTaskLastCycleCount = 0U;
    portBaseType xResult = pdPASS;

    /* Has the Event Multiplex Block task performed at least one cycle since this
     * function was last called? */
    if( uxEvtMplxBlockTaskCycleCount == uxEvtMplxBlockTaskLastCycleCount )
    {
        xResult = pdFAIL;
    }

    /* Have any errors been detected by the Event Multiplex Block task? */
    if( pdTRUE == xEvtMplxBlockTaskErrorOccurred )
    {
        xResult = pdFAIL;
    }

    /* Record the current value of the Event Multiplex Block task cycle count
     * variable. */
    uxEvtMplxBlockTaskLastCycleCount = uxEvtMplxBlockTaskCycleCount;


    /* Has the Event Multiplex Control task performed at least one cycle since this
     * function was last called? */
    if( uxEvtMplxControlTaskCycleCount == uxEvtMplxControlTaskLastCycleCount )
    {
        xResult = pdFAIL;
    }

    /* Have any errors been detected by the Event Multiplex Control task? */
    if( pdTRUE == xEvtMplxControlTaskErrorOccurred )
    {
        xResult = pdFAIL;
    }

    /* Record the current value of the Event Multiplex Control task cycle count
     * variable. */
    uxEvtMplxControlTaskLastCycleCount = uxEvtMplxControlTaskCycleCount;

    return xResult;
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Private Function Definitions                                              */
/*---------------------------------------------------------------------------*/

static void prvEvtMplxBlockTask( void *pvParameters )
{
    portBaseType xStatus;
    portTickType xLastExecutionTime = 0U;
    portTickType xElapsedTime;
    evtMplxObjectEventsType axOccurredObjectEvents [ evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS ] = { { 0 } };
    portUnsignedBaseType uxNumberOfObjectEvents;
    portUnsignedBaseType uxQueueItemReceived = 0U;
    portUnsignedBaseType uxRetrievedNotificationValue = 0U;
    eventBitsType xEventBitsSet = 0U;

    /* Prevent compiler warnings. */
    ( void ) pvParameters;

    /* Is the event multiplex handle valid? If it is not, then this task must have
     * been deleted and created again, in which case, the event multiplex object also
     * needs to be created again. */
    if( NULL == xEvtMplxHandle )
    {
        xStatus = xEvtMplxCreate( acEvtMplxBuffer,
                                  portspecEVT_MPLX_BUFFER_LENGTH,
                                  portspecMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                  xEvtMplxBlockTaskHandle,
                                  &xEvtMplxHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
    }

    /* Calling xEvtMplxWait() before adding any object-events to the event
     * multiplex object should result in an error code being returned. */
    uxNumberOfObjectEvents = 0U;
    xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                            evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                            &uxNumberOfObjectEvents, evtmplxMAX_TICKS_TO_WAIT );
    if( errINVALID_EVT_MPLX_OPERATION != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Register an interest in space being available in Event Multiplex Q1. */
    xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle, xEvtMplxQueue1Handle,
                                       evtmplxQUEUE_SPACE_AVAILABLE );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Register an interest in space being available in Event Multiplex Q2. */
    xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle, xEvtMplxQueue2Handle,
                                       evtmplxQUEUE_SPACE_AVAILABLE );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Record the current time. */
    xLastExecutionTime = xTaskGetTickCount();

    /* Call xEvtMplxWait() - even though a block time has been specified, this
     * call should return immediately as both queues are currently empty. */
    uxNumberOfObjectEvents = 0U;
    xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                            evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                            &uxNumberOfObjectEvents, evtmplxMAX_TICKS_TO_WAIT );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Calculate how much processing time elapsed. */
    xElapsedTime = xTaskGetTickCount() - xLastExecutionTime;

    /* Due to the exact timing of the tick interrupt and the processing of other
     * tasks in the demo, it is conceivable that the elapsed time could be
     * non-zero, but it will be *very* short. */
    if( xElapsedTime > evtmplxMINIMUM_ELAPSED_TIME_NON_BLOCKING )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* There should have been 2 events reported. */
    if( 2U != uxNumberOfObjectEvents )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* The first received entry should be for Event Multiplex Q1. */
    if( axOccurredObjectEvents[ 0 ].pvObjectHandle != xEvtMplxQueue1Handle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].uxEvents != evtmplxQUEUE_SPACE_AVAILABLE )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* The second received entry should be for Event Multiplex Q2. */
    if( axOccurredObjectEvents[ 1 ].pvObjectHandle != xEvtMplxQueue2Handle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 1 ].uxEvents != evtmplxQUEUE_SPACE_AVAILABLE )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* The third entry should be unused. */
    if( axOccurredObjectEvents[ 2 ].pvObjectHandle != NULL )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 2 ].uxEvents != evtmplxNO_EVENTS )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Change the configuration to messages being received in Event Multiplex Q1. */
    xStatus = xEvtMplxModifyObjectEvents( xEvtMplxHandle,
                                          xEvtMplxQueue1Handle,
                                          evtmplxQUEUE_MESSAGE_WAITING );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Record the current time. */
    xLastExecutionTime = xTaskGetTickCount();

    /* Call xEvtMplxWait() - even though a block time has been specified, this
     * call should return immediately as Event Multiplex Q2 is currently empty. */
    uxNumberOfObjectEvents = 0U;
    xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                            evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                            &uxNumberOfObjectEvents, evtmplxMAX_TICKS_TO_WAIT );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Calculate how much processing time elapsed. */
    xElapsedTime = xTaskGetTickCount() - xLastExecutionTime;

    /* Due to the exact timing of the tick interrupt and the processing of other
     * tasks in the demo, it is conceivable that the elapsed time could be
     * non-zero, but it will be *very* short. */
    if( xElapsedTime > evtmplxMINIMUM_ELAPSED_TIME_NON_BLOCKING )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* There should have been 1 event reported. */
    if( 1U != uxNumberOfObjectEvents )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* The first received entry should be for Event Multiplex Q2. */
    if( axOccurredObjectEvents[ 0 ].pvObjectHandle != xEvtMplxQueue2Handle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].uxEvents != evtmplxQUEUE_SPACE_AVAILABLE )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Change the configuration to messages being received in Event Multiplex Q2. */
    xStatus = xEvtMplxModifyObjectEvents( xEvtMplxHandle,
                                          xEvtMplxQueue2Handle,
                                          evtmplxQUEUE_MESSAGE_WAITING );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Register an interest in a semaphore being available. */
    xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle, xEvtMplxSemaphoreHandle,
                                       evtmplxSEMAPHORE_AVAILABLE );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Call xEvtMplxWait() - even though a block time has been specified, this
     * call should return immediately as the event multiplex semaphore is currently
     * available. */
    uxNumberOfObjectEvents = 0U;
    xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                            evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                            &uxNumberOfObjectEvents, evtmplxMAX_TICKS_TO_WAIT );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( 1U != uxNumberOfObjectEvents )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].pvObjectHandle != xEvtMplxSemaphoreHandle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].uxEvents != evtmplxSEMAPHORE_AVAILABLE )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Take the semaphore. */
    xStatus = xSemaphoreTake( xEvtMplxSemaphoreHandle, evtmplxNO_BLOCK_TIME );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Register an interest in a mutex being available. */
    xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle, xEvtMplxMutexHandle,
                                       evtmplxMUTEX_AVAILABLE );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Call xEvtMplxWait() - even though a block time has been specified, this
     * call should return immediately as the event multiplex mutex is currently
     * available. */
    uxNumberOfObjectEvents = 0U;
    xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                            evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                            &uxNumberOfObjectEvents, evtmplxMAX_TICKS_TO_WAIT );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( 1U != uxNumberOfObjectEvents )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].pvObjectHandle != xEvtMplxMutexHandle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].uxEvents != evtmplxMUTEX_AVAILABLE )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Take the mutex. */
    xStatus = xMutexTake( xEvtMplxMutexHandle, evtmplxNO_BLOCK_TIME );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Send a notification to this task BEFORE adding the task notification
     * event to the event multiplex object. */
    xStatus = xTaskNotifySend( xEvtMplxBlockTaskHandle,
                               taskNOTIFICATION_SET_VAL_WITH_OVERWRITE,
                               uxQueueItemReceived );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Register an interest in a task notification being received. */
    xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle,
                                       xTaskGetCurrentTaskHandle(),
                                       evtmplxTASK_NOTIFICATION_RECEIVED );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Register an interest in the event multiplex demo event group being updated. */
    xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle,
                                       xEvtMplxDemoEventGroupHandle,
                                       evtmplxEVENT_GROUP_BITS_SET );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Call xEvtMplxWait() - even though a block time has been specified, this
     * call should return immediately as there is a task notification waiting
     * and the evtmplxEVENT_GROUP_INITIALISED bit is set in the event multiplex demo
     * event group. */
    uxNumberOfObjectEvents = 0U;
    xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                            evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                            &uxNumberOfObjectEvents, evtmplxMAX_TICKS_TO_WAIT );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( 2U != uxNumberOfObjectEvents )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].pvObjectHandle != xEvtMplxBlockTaskHandle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 0 ].uxEvents != evtmplxTASK_NOTIFICATION_RECEIVED )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 1 ].pvObjectHandle != xEvtMplxDemoEventGroupHandle )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( axOccurredObjectEvents[ 1 ].uxEvents != evtmplxEVENT_GROUP_BITS_SET )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Retrieve the task notification value. */
    xStatus = xTaskNotifyWait( 0U, 0U, &uxRetrievedNotificationValue,
                               evtmplxNO_BLOCK_TIME );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    if( uxQueueItemReceived != uxRetrievedNotificationValue )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Retrieve the event multiplex demo event group event bits. */
    xStatus = xEventGroupGetBits( xEvtMplxDemoEventGroupHandle, &xEventBitsSet );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }
    if( evtmplxEVENT_GROUP_INITIALISED != xEventBitsSet )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }

    /* Clear the evtmplxEVENT_GROUP_INITIALISED bit in the event multiplex demo event
     * group. */
    xStatus = xEventGroupClearBits( xEvtMplxDemoEventGroupHandle,
                                    evtmplxEVENT_GROUP_INITIALISED );
    if( pdPASS != xStatus )
    {
        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
    }


    /* Task body. */
    for( ;; )
    {
        /* Delay long enough for the (lower priority) control task to suspend
         * itself. */
        xStatus = xTaskDelay( evtmplxMAX_TICKS_TO_WAIT );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }


        /* De-register an interest in a task notification being received. */
        xStatus = xEvtMplxRemoveObjectEvents( xEvtMplxHandle,
                                              xTaskGetCurrentTaskHandle() );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Send a notification to this task again. */
        xStatus = xTaskNotifySend( xEvtMplxBlockTaskHandle,
                                   taskNOTIFICATION_SET_VAL_WITH_OVERWRITE,
                                   uxQueueItemReceived );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }


        /* Record the current time. */
        xLastExecutionTime = xTaskGetTickCount();

        /* Block on the event multiplex object - this call should timeout. */
        uxNumberOfObjectEvents = 0U;
        xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                                evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                &uxNumberOfObjectEvents,
                                evtmplxMAX_TICKS_TO_WAIT );

        /* Calculate how much processing time elapsed. */
        xElapsedTime = xTaskGetTickCount() - xLastExecutionTime;

        /* The elapsed time should be at least equal to the block time. */
        if( xElapsedTime < evtmplxMAX_TICKS_TO_WAIT )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Check the expected values were reported by the call to xEvtMplxWait(). */
        if( errEVT_MPLX_NO_EVENTS_OCCURRED != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        if( 0U != uxNumberOfObjectEvents )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }


        /* Retrieve the task notification value BEFORE adding the task
         * notification event to the event multiplex object. */
        uxRetrievedNotificationValue = 0U;
        xStatus = xTaskNotifyWait( 0U, 0U, &uxRetrievedNotificationValue,
                                   evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        if( uxQueueItemReceived != uxRetrievedNotificationValue )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Register an interest in a task notification being received. */
        xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle,
                                           xTaskGetCurrentTaskHandle(),
                                           evtmplxTASK_NOTIFICATION_RECEIVED );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }


        /* Resume the (lower priority) control task. */
        xStatus = xTaskResume( xEvtMplxControlTaskHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }


        /* Block on the event multiplex object - this call should not timeout. */
        uxNumberOfObjectEvents = 0U;
        xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                                evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                &uxNumberOfObjectEvents,
                                evtmplxMAX_TICKS_TO_WAIT );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        else if( 1U != uxNumberOfObjectEvents )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        else
        {
            if( xEvtMplxQueue1Handle == axOccurredObjectEvents[ 0 ].pvObjectHandle )
            {
                if( evtmplxQUEUE_MESSAGE_WAITING == axOccurredObjectEvents[ 0 ].uxEvents )
                {
                    /* Retrieve the item from Event Multiplex Q1 without blocking. */
                    xStatus = xQueueReceive( xEvtMplxQueue1Handle, &uxQueueItemReceived,
                                             evtmplxNO_BLOCK_TIME );
                    if( pdPASS != xStatus )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }
                    else if( uxQueueItemReceived != evtmplxQUEUE_ITEM_DEFAULT_VALUE )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }
                    else
                    {
                        /* Post the bitwise inverse of the received item to
                         * Event Multiplex Q2. */
                        uxQueueItemReceived = ~uxQueueItemReceived;
                        xStatus = xQueueSend( xEvtMplxQueue2Handle,
                                              &uxQueueItemReceived,
                                              evtmplxNO_BLOCK_TIME );
                        if( pdPASS != xStatus )
                        {
                            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                        }
                        else
                        {
                            /* The event multiplex object should now have 1
                             * object-event available (the message in Event Multiplex
                             * Q2). */
                            uxNumberOfObjectEvents = 0U;
                            xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                                                    evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                                    &uxNumberOfObjectEvents,
                                                    evtmplxNO_BLOCK_TIME );
                            if( pdPASS != xStatus )
                            {
                                xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                            }
                            else if( 1U != uxNumberOfObjectEvents )
                            {
                                xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                            }
                            else
                            {
                                /* De-register an interest in messages being
                                 * received in Event Multiplex Q2. */
                                xStatus = xEvtMplxRemoveObjectEvents( xEvtMplxHandle,
                                                                      xEvtMplxQueue2Handle );
                                if( pdPASS != xStatus )
                                {
                                    xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                                }
                            }
                        }
                    }
                }
                else
                {
                    xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                }
            }
            else
            {
                xEvtMplxBlockTaskErrorOccurred = pdTRUE;
            }
        }


        /* Block on the event multiplex object - this call should not timeout.
         * There should be 2 events available when the the event multiplex blocking
         * task unblocks. */
        uxNumberOfObjectEvents = 0U;
        xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                                evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                &uxNumberOfObjectEvents,
                                evtmplxMAX_TICKS_TO_WAIT );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        else if( 2U != uxNumberOfObjectEvents )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        else
        {
            /* The first event should be a task notification. */
            if( xTaskGetCurrentTaskHandle() == axOccurredObjectEvents[ 0 ].pvObjectHandle )
            {
                if( evtmplxTASK_NOTIFICATION_RECEIVED == axOccurredObjectEvents[ 0 ].uxEvents )
                {
                    /* Retrieve the task notification value. */
                    uxRetrievedNotificationValue = 0U;
                    xStatus = xTaskNotifyWait( 0U, 0U, &uxRetrievedNotificationValue,
                                               evtmplxNO_BLOCK_TIME );
                    if( pdPASS != xStatus )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }

                    if( uxQueueItemReceived != uxRetrievedNotificationValue )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }
                }
                else
                {
                    xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                }
            }
            else
            {
                xEvtMplxBlockTaskErrorOccurred = pdTRUE;
            }

            /* The second event should be event group bits being set. */
            if( xEvtMplxDemoEventGroupHandle == axOccurredObjectEvents[ 1 ].pvObjectHandle )
            {
                if( evtmplxEVENT_GROUP_BITS_SET == axOccurredObjectEvents[ 1 ].uxEvents )
                {
                    /* Retrieve the event multiplex demo event group event bits. */
                    xStatus = xEventGroupGetBits( xEvtMplxDemoEventGroupHandle, &xEventBitsSet );
                    if( pdPASS != xStatus )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }
                    if( evtmplxCONTROL_TASK_BIT != xEventBitsSet )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }

                    /* Clear the evtmplxCONTROL_TASK_BIT bit in the event multiplex
                     * demo event group. */
                    xStatus = xEventGroupClearBits( xEvtMplxDemoEventGroupHandle,
                                                    evtmplxCONTROL_TASK_BIT );
                    if( pdPASS != xStatus )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }
                }
                else
                {
                    xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                }
            }
            else
            {
                xEvtMplxBlockTaskErrorOccurred = pdTRUE;
            }
        }


        /* Register interest in there being space in Event Multiplex Q2 to send a
         * message. */
        xStatus = xEvtMplxAddObjectEvents( xEvtMplxHandle,
                                           xEvtMplxQueue2Handle,
                                           evtmplxQUEUE_SPACE_AVAILABLE );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* The event multiplex object should now have 1 object-event available (space
         * in Event Multiplex Q2 to send a message). */
        uxNumberOfObjectEvents = 0U;
        xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                                evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                &uxNumberOfObjectEvents,
                                evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        else if( 1U != uxNumberOfObjectEvents )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        else
        {
            if( xEvtMplxQueue2Handle == axOccurredObjectEvents[ 0 ].pvObjectHandle )
            {
                if( evtmplxQUEUE_SPACE_AVAILABLE == axOccurredObjectEvents[ 0 ].uxEvents )
                {
                    /* Modify the set of registered events for Event Multiplex Q2. */
                    xStatus = xEvtMplxModifyObjectEvents( xEvtMplxHandle,
                                                          xEvtMplxQueue2Handle,
                                                          evtmplxQUEUE_MESSAGE_WAITING );
                    if( pdPASS != xStatus )
                    {
                        xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                    }
                }
                else
                {
                    xEvtMplxBlockTaskErrorOccurred = pdTRUE;
                }
            }
            else
            {
                xEvtMplxBlockTaskErrorOccurred = pdTRUE;
            }
        }


        /* Release the semaphore. */
        xStatus = xSemaphoreGive( xEvtMplxSemaphoreHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Release the mutex. */
        xStatus = xMutexGive( xEvtMplxMutexHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Call xEvtMplxWait() without a block time. */
        uxNumberOfObjectEvents = 0U;
        xStatus = xEvtMplxWait( xEvtMplxHandle, axOccurredObjectEvents,
                                evtmplxMAXIMUM_NUMBER_OF_OBJECT_EVENTS,
                                &uxNumberOfObjectEvents, evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        if( 2U != uxNumberOfObjectEvents )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        if( axOccurredObjectEvents[ 0 ].pvObjectHandle != xEvtMplxSemaphoreHandle )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        if( axOccurredObjectEvents[ 0 ].uxEvents != evtmplxSEMAPHORE_AVAILABLE )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        if( axOccurredObjectEvents[ 1 ].pvObjectHandle != xEvtMplxMutexHandle )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }
        if( axOccurredObjectEvents[ 1 ].uxEvents != evtmplxMUTEX_AVAILABLE )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Take the semaphore once again. */
        xStatus = xSemaphoreTake( xEvtMplxSemaphoreHandle, evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }

        /* Take the mutex once again. */
        xStatus = xMutexTake( xEvtMplxMutexHandle, evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxBlockTaskErrorOccurred = pdTRUE;
        }


        /* Update the count value so the check task knows this task is still
         * executing. */
        uxEvtMplxBlockTaskCycleCount++;
    }
}
/*---------------------------------------------------------------------------*/

static void prvEvtMplxControlTask( void *pvParameters )
{
    portBaseType xStatus;
    portUnsignedBaseType uxQueueItemToSend = evtmplxQUEUE_ITEM_DEFAULT_VALUE;
    portUnsignedBaseType uxQueueItemReceived = 0U;

    /* Prevent compiler warnings. */
    ( void ) pvParameters;

    /* Task body. */
    for( ;; )
    {
        /* Suspend this task. */
        xStatus = xTaskSuspend( NULL );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Send an item to Event Multiplex Q1 without blocking. */
        xStatus = xQueueSend( xEvtMplxQueue1Handle, &uxQueueItemToSend,
                              evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* When this line is reached, Event Multiplex Q1 should now be empty, but
         * there should be an item in Event Multiplex Q2. */
        xStatus = xQueueReceive( xEvtMplxQueue1Handle, &uxQueueItemReceived,
                                 evtmplxNO_BLOCK_TIME );
        if( errQUEUE_EMPTY != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }
        xStatus = xQueueReceive( xEvtMplxQueue2Handle, &uxQueueItemReceived,
                                 evtmplxNO_BLOCK_TIME );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }
        if( uxQueueItemReceived != ~evtmplxQUEUE_ITEM_DEFAULT_VALUE )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }


        /* Temporarily raise the priority of the event multiplex control task above
         * that of the event multiplex blocking task. */
        xStatus = xTaskPrioritySet( NULL, ( evtmplxBLOCK_TASK_PRIORITY + 1U ) );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Send a notification to the event multiplex blocking task. */
        xStatus = xTaskNotifySend( xEvtMplxBlockTaskHandle,
                                   taskNOTIFICATION_SET_VAL_WITH_OVERWRITE,
                                   uxQueueItemReceived );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Set a bit in the event multiplex demo event group. */
        xStatus = xEventGroupSetBits( xEvtMplxDemoEventGroupHandle,
                                      evtmplxCONTROL_TASK_BIT );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Restore the original priority of the event multiplex control task - this
         * should cause a context switch. */
        xStatus = xTaskPrioritySet( NULL, evtmplxCONTROL_TASK_PRIORITY );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }


        /* Delete the event multiplex blocking task. This should cause the event multiplex
         * object buffer to be reset to a state that will allow it to be
         * reused. */
        xStatus = xTaskDelete( xEvtMplxBlockTaskHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Set the handle to NULL, so the event multiplex blocking task knows to
         * recreate the event multiplex object when it is next run. */
        xEvtMplxHandle = NULL;

        /* Ensure the semaphore is available the next time the event multiplex
         * blocking task runs. */
        xStatus = xSemaphoreGive( xEvtMplxSemaphoreHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Set a bit in the event multiplex demo event group. */
        xStatus = xEventGroupSetBits( xEvtMplxDemoEventGroupHandle,
                                      evtmplxEVENT_GROUP_INITIALISED );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }

        /* Recreate the task that will block on an event multiplex object. */
        xStatus = xTaskCreate( &xEvtMplxBlockTaskParameters,
                               &xEvtMplxBlockTaskHandle );
        if( pdPASS != xStatus )
        {
            xEvtMplxControlTaskErrorOccurred = pdTRUE;
        }


        /* Update the count value so the check task knows this task is still
         * executing. */
        uxEvtMplxControlTaskCycleCount++;
    }
}
/*---------------------------------------------------------------------------*/
