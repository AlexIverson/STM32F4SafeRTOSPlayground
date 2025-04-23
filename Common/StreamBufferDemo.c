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

/* PortSpecifics.h includes some 'declare once' declarations. */
#define STREAM_BUFFER_C

#include <stdlib.h>

/* Scheduler include files. */
#include "SafeRTOS_API.h"

/* Demo program include files. */
#include "StreamBufferDemo.h"
#include "PortSpecifics.h"


/*-----------------------------------------------------------------------------
 * Local function prototypes
 *---------------------------------------------------------------------------*/

/* The tasks that are created to demo the StreamBuffer. */
static void prvStreamBufferInitialProducerTask( void *pvParameters );
static void prvStreamBufferPassoverTask1( void *pvParameters );
static void prvStreamBufferPassoverTask2( void *pvParameters );
static void prvStreamBufferErrorCheck( portUInt8Type *pucArrayToCheck, portUInt8Type ucLengthToCheck );
static portBaseType prvInitWakeCheck( portUnsignedBaseType uxPriority );
static void prvStreamBufferTxWaitCheck( void *pvParameters );
static void prvStreamBufferRxWaitCheck( void *pvParameters );


/*-----------------------------------------------------------------------------
 * Local variables
 *---------------------------------------------------------------------------*/

portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUInt8Type aucStreamBufferMessage[ portspecSTREAM_BUFFER_MESSAGE_LENGTH ] = { 1U, 2U, 3U, 4U };
const size_t xStreamBufferSizeBytes = portspecSTORAGE_SIZE_BYTES;
const size_t xStreamBufferTrigger = 1U;

portspecSTREAM_BUFFER_DATA_SECTION_COMMON portBaseType xTransmitWakeupOk = pdFALSE;
portspecSTREAM_BUFFER_DATA_SECTION_COMMON portBaseType xReceiveWakeupOk = pdFALSE;

/* StreamBuffer  Task TCBs. */
portspecTCB_DATA_SECTION static xTCB xStreamBufferTaskTCBs[ portspecNUMBER_OF_STREAM_BUFFER_TASKS ] = { { 0U } };
portspecTCB_DATA_SECTION static xTCB xWakeCheckTaskRxTCB = { 0U };
portspecTCB_DATA_SECTION static xTCB xWakeCheckTaskTxTCB = { 0U };

/* Defines the memory that will actually hold the streams within the stream buffer. */
portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUInt8Type aucStorageBuffer0[ portspecSTORAGE_SIZE_BYTES ] = { 0U };
portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUInt8Type aucStorageBuffer1[ portspecSTORAGE_SIZE_BYTES ] = { 0U };
portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUInt8Type aucStorageBuffer2[ portspecSTORAGE_SIZE_BYTES ] = { 0U };

portspecSTREAM_BUFFER_DATA_SECTION portUInt8Type aucStorageBufferWakeCheck[ 80U ] = { 0U };
portspecSTREAM_BUFFER_DATA_SECTION portUInt8Type aucBufferToSend[ 60U ] = { 1U };

/* The variable used to hold the stream buffer structure.*/
portspecSTREAM_BUFFER_DATA_SECTION_COMMON StreamBuffer_t xStreamBufferStruct0 = { 0U };
portspecSTREAM_BUFFER_DATA_SECTION_COMMON StreamBuffer_t xStreamBufferStruct1 = { 0U };
portspecSTREAM_BUFFER_DATA_SECTION_COMMON StreamBuffer_t xStreamBufferStruct2 = { 0U };

portspecSTREAM_BUFFER_DATA_SECTION StreamBuffer_t xStreamBufferStructWakeCheck = { 0U };

/* The handle of each stream buffer. */
portspecSTREAM_BUFFER_DATA_SECTION_COMMON StreamBufferHandle_t xStreamBufferDemoHandle0 = NULL;
portspecSTREAM_BUFFER_DATA_SECTION_COMMON StreamBufferHandle_t xStreamBufferDemoHandle1 = NULL;
portspecSTREAM_BUFFER_DATA_SECTION_COMMON StreamBufferHandle_t xStreamBufferDemoHandle2 = NULL;

portspecSTREAM_BUFFER_DATA_SECTION StreamBufferHandle_t xStreamBufferDemoHandleWakeCheck = NULL;

#if ( portspecMULTICORE_STREAM_BUFFER_DEMO == 0U )
    portspecSTREAM_BUFFER_DATA_SECTION portUnsignedBaseType uxCpuNumber = 0U;
#endif

portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUnsignedBaseType uxTask0Count = 0U;
portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUnsignedBaseType uxTask1Count = 0U;
portspecSTREAM_BUFFER_DATA_SECTION_COMMON portUnsignedBaseType uxTask2Count = 0U;


/*-----------------------------------------------------------------------------
 * Function definitions
 *---------------------------------------------------------------------------*/

portBaseType xStartStreamBufferTasks( portUnsignedBaseType uxPriority )
{
    portBaseType xStatus;

    xTaskParameters xStreamBufferTaskParameters0 =
    {
        &prvStreamBufferInitialProducerTask,    /* Task Code */
        "StreamBuffer Producer",                /* Task Name */
        &xStreamBufferTaskTCBs[ 0U ],           /* TCB - different for each task. */
        acStreamBufferTaskStack[ 0U ],          /* Stack Buffer - different for each task. */
        portspecSTREAM_BUFFER_TASK_STACK_SIZE,  /* Stack Depth Bytes */
        NULL,                                   /* Parameters - different for each task. */
        uxPriority,                             /* Priority */
        NULL,                                   /* TLS Object */
        xStreamBufferTaskPortParameters         /* Port Specific Task Parameters */
    };

    xTaskParameters xStreamBufferTaskParameters1 =
    {
        &prvStreamBufferPassoverTask1,          /* Task Code */
        "StreamBuffer Passover #1",             /* Task Name */
        &xStreamBufferTaskTCBs[ 1U ],           /* TCB - different for each task. */
        acStreamBufferTaskStack[ 1U ],          /* Stack Buffer - different for each task. */
        portspecSTREAM_BUFFER_TASK_STACK_SIZE,  /* Stack Depth Bytes */
        NULL,                                   /* Parameters - different for each task. */
        uxPriority,                             /* Priority */
        NULL,                                   /* TLS Object */
        xStreamBufferTaskPortParameters         /* Port Specific Task Parameters */
    };

    xTaskParameters xStreamBufferTaskParameters2 =
    {
        &prvStreamBufferPassoverTask2,          /* Task Code */
        "StreamBuffer Passover #2",             /* Task Name */
        &xStreamBufferTaskTCBs[ 2U ],           /* TCB - different for each task. */
        acStreamBufferTaskStack[ 2U ],          /* Stack Buffer - different for each task. */
        portspecSTREAM_BUFFER_TASK_STACK_SIZE,  /* Stack Depth Bytes */
        NULL,                                   /* Parameters - different for each task. */
        uxPriority,                             /* Priority */
        NULL,                                   /* TLS Object */
        xStreamBufferTaskPortParameters         /* Port Specific Task Parameters */
    };

#if ( portspecMULTICORE_STREAM_BUFFER_DEMO == 1 )

    if ( portspecSTREAM_BUFFER_CREATION_CORE == portspecSTREAM_BUFFER_GET_CPU_ID )
    {
        /* Create the stream buffer. */
        xStatus = xStreamBufferCreate( &xStreamBufferDemoHandle0, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBuffer0, &xStreamBufferStruct0 );
        xStatus = xStreamBufferCreate( &xStreamBufferDemoHandle1, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBuffer1, &xStreamBufferStruct1 );
        xStatus = xStreamBufferCreate( &xStreamBufferDemoHandle2, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBuffer2, &xStreamBufferStruct2 );
    }

#else

    /* Create the stream buffer. */
    xStatus = xStreamBufferCreate( &xStreamBufferDemoHandle0, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBuffer0, &xStreamBufferStruct0 );
    xStatus = xStreamBufferCreate( &xStreamBufferDemoHandle1, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBuffer1, &xStreamBufferStruct1 );
    xStatus = xStreamBufferCreate( &xStreamBufferDemoHandle2, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBuffer2, &xStreamBufferStruct2 );

#endif

    xStatus = xStreamBufferCreate( &xStreamBufferDemoHandleWakeCheck, xStreamBufferSizeBytes, xStreamBufferTrigger, pdFALSE, aucStorageBufferWakeCheck, &xStreamBufferStructWakeCheck );

    if ( portspecSTREAM_BUFFER_FIRST_TASK == portspecSTREAM_BUFFER_GET_CPU_ID )
    {
        /* Create a task for the StreamBuffer producer. */
        if( pdPASS != xTaskCreate( &xStreamBufferTaskParameters0, ( portTaskHandleType * ) NULL ) )
        {
            xStatus = pdFAIL;
        }
    }
    if ( portspecSTREAM_BUFFER_SECOND_TASK == portspecSTREAM_BUFFER_GET_CPU_ID )
    {
        /* Create a task for the StreamBuffer producer. */
        if( pdPASS != xTaskCreate( &xStreamBufferTaskParameters1, ( portTaskHandleType * ) NULL ) )
        {
            xStatus = pdFAIL;
        }
    }

    if ( portspecSTREAM_BUFFER_THIRD_TASK == portspecSTREAM_BUFFER_GET_CPU_ID )
    {
        /* Create a task for the StreamBuffer producer. */
        if( pdPASS != xTaskCreate( &xStreamBufferTaskParameters2, ( portTaskHandleType * ) NULL ) )
        {
            xStatus = pdFAIL;
        }
    }

    prvInitWakeCheck( uxPriority );

    return xStatus;
}
/*---------------------------------------------------------------------------*/

static portBaseType prvInitWakeCheck( portUnsignedBaseType uxPriority )
{
    portBaseType xReturn = pdPASS;

    xTaskParameters xStreamBufferTaskParametersWakeCheckRx =
    {
        &prvStreamBufferRxWaitCheck,                    /* Task Code */
        "StreamBufferWaitCheckRx",                      /* Task Name */
        &xWakeCheckTaskRxTCB,                           /* TCB - different for each task. */
        acStreamBufferTaskWakeCheckRxStack,             /* Stack Buffer - different for each task. */
        portspecSTREAM_BUFFER_TASK_STACK_SIZE,          /* Stack Depth Bytes */
        NULL,                                           /* Parameters - different for each task. */
        uxPriority,                                     /* Priority */
        NULL,                                           /* TLS Object */
        xStreamBufferTaskPortParameters                 /* Port Specific Task Parameters */
    };

    xTaskParameters xStreamBufferTaskParametersWakeCheckTx =
    {
        &prvStreamBufferTxWaitCheck,                    /* Task Code */
        "StreamBufferWaitCheckTx",                      /* Task Name */
        &xWakeCheckTaskTxTCB,                           /* TCB - different for each task. */
        acStreamBufferTaskWakeCheckTxStack,             /* Stack Buffer - different for each task. */
        portspecSTREAM_BUFFER_TASK_STACK_SIZE,          /* Stack Depth Bytes */
        NULL,                                           /* Parameters - different for each task. */
        uxPriority,                                     /* Priority */
        NULL,                                           /* TLS Object */
        xStreamBufferTaskPortParameters                 /* Port Specific Task Parameters */
    };

    /* Create a task for the StreamBuffer producer. */
    if( pdPASS != xTaskCreate( &xStreamBufferTaskParametersWakeCheckRx, ( portTaskHandleType * ) NULL ) )
    {
        xReturn = pdFAIL;
    }

    /* Create a task for the StreamBuffer producer. */
    if( pdPASS != xTaskCreate( &xStreamBufferTaskParametersWakeCheckTx, ( portTaskHandleType * ) NULL ) )
    {
        xReturn = pdFAIL;
    }

    return xReturn;
}
/*---------------------------------------------------------------------------*/

static void prvStreamBufferInitialProducerTask( void *pvParameters )
{
    portUInt8Type aucRxData[ portspecSTREAM_BUFFER_MESSAGE_LENGTH ];
    portUnsignedBaseType uxErrorCount = 0U;
    portTickType xLast;
    portTickType xRate = 200U;
    portBaseType xResetResult = 0;
    portUnsignedBaseType uxReceivedBytes = 0U;
    portUnsignedBaseType uxNumBytesWritten = 0U;
    portUnsignedBaseType uxIsEmpty = 0U;

    ( void ) pvParameters;

    /* Initialise xLastFlashTime prior to First Call tovTaskDelayUntil(). */
    xLast = xTaskGetTickCount();
    ( void ) xTaskDelayUntil( &xLast, xRate );

    for( ;; )
    {
        /* Check to see if the stream buffer is empty. */
        xStreamBufferIsEmpty( xStreamBufferDemoHandle0, &uxIsEmpty );
        if( pdTRUE == uxIsEmpty )
        {
            /* If it is empty, add new text. */
            xStreamBufferSend( xStreamBufferDemoHandle0,
                               aucStreamBufferMessage,
                               sizeof(aucStreamBufferMessage),
                               portspecSB_BLOCK_TIME,
                               &uxNumBytesWritten);
        }
        uxReceivedBytes = 0U;

        /* Check to see if the stream buffer is empty. */
        xStreamBufferIsEmpty( xStreamBufferDemoHandle2, &uxIsEmpty );
        if( pdFALSE == uxIsEmpty )
        {
            /* If it is not empty, read the contents. */
            while ( 0U == uxReceivedBytes )
            {
                xStreamBufferReceive( xStreamBufferDemoHandle2,
                                      aucRxData,
                                      portspecSTREAM_BUFFER_MESSAGE_LENGTH,
                                      &uxReceivedBytes,
                                      portspecSB_BLOCK_TIME );
            }
            prvStreamBufferErrorCheck( aucRxData,portspecSTREAM_BUFFER_MESSAGE_LENGTH );

            /* Clear the buffer. */
            xResetResult = xStreamBufferReset( xStreamBufferDemoHandle2 );
            if( pdFAIL == xResetResult )
            {
                uxErrorCount++;
            }
        }

        uxTask0Count++;

        /* Delay for  Period then toggle StreamBuffer state. */
        ( void ) xTaskDelayUntil( &xLast, xRate );
    }
}
/*---------------------------------------------------------------------------*/

static void prvStreamBufferPassoverTask1( void *pvParameters )
{
    portUnsignedBaseType uxErrorCount = 0U;
    portTickType xLast;
    portTickType xRate = 200U;
    portUInt8Type aucRxData[ portspecSTREAM_BUFFER_MESSAGE_LENGTH ];
    portBaseType xResetResult = 0;
    portUnsignedBaseType uxReceivedBytes;
    portUnsignedBaseType uxNumBytesWritten = 0U;
    portUnsignedBaseType uxIsEmpty = 0U;

    ( void ) pvParameters;

    /* Initialise xLastFlashTime prior to First Call tovTaskDelayUntil(). */
    xLast = xTaskGetTickCount();
    ( void ) xTaskDelayUntil( &xLast, xRate );


    for( ;; )
    {
        uxReceivedBytes = 0U;

        /* Check to see if the stream buffer is empty. */
        xStreamBufferIsEmpty( xStreamBufferDemoHandle0, &uxIsEmpty );
        if( pdFALSE == uxIsEmpty )
        {
            /* If it is not empty, read the contents. */
            while ( 0U == uxReceivedBytes )
            {
                xStreamBufferReceive( xStreamBufferDemoHandle0,
                                      aucRxData,
                                      portspecSTREAM_BUFFER_MESSAGE_LENGTH,
                                      &uxReceivedBytes,
                                      portspecSB_BLOCK_TIME );
            }
            prvStreamBufferErrorCheck( aucRxData,portspecSTREAM_BUFFER_MESSAGE_LENGTH);

            /* Clear the buffer. */
            xResetResult = xStreamBufferReset( xStreamBufferDemoHandle0 );
            if( pdFAIL == xResetResult )
            {
                uxErrorCount++;
            }

            /* Now send the message down the chain. */
            /* Check to see if the stream buffer is empty. */
            xStreamBufferIsEmpty( xStreamBufferDemoHandle1, &uxIsEmpty );
            if( pdTRUE == uxIsEmpty )
            {
                /* If it is empty, add new text. */
                xStreamBufferSend( xStreamBufferDemoHandle1,
                                   aucRxData,
                                   sizeof( aucRxData ),
                                   portspecSB_BLOCK_TIME,
                                   &uxNumBytesWritten);
            }

        }

        uxTask1Count++;

        /* Delay for a period, then toggle StreamBuffer state. */
        ( void ) xTaskDelayUntil( &xLast, xRate );
    }
}
/*---------------------------------------------------------------------------*/

static void prvStreamBufferPassoverTask2( void *pvParameters )
{
    portUnsignedBaseType uxErrorCount = 0U;
    portTickType xLast;
    portTickType xRate = 200U;
    portUInt8Type aucRxData[ portspecSTREAM_BUFFER_MESSAGE_LENGTH ];
    portUnsignedBaseType uxReceivedBytes;
    portUnsignedBaseType uxNumBytesWritten = 0U;
    portUnsignedBaseType uxIsEmpty = 0U;
    portBaseType xStatus;

    ( void ) pvParameters;

    /* Initialise xLastFlashTime prior to First Call tovTaskDelayUntil(). */
    xLast = xTaskGetTickCount();
    ( void ) xTaskDelayUntil( &xLast, xRate );

    for( ;; )
    {
        uxReceivedBytes = 0U;

        /* Check to see if the stream buffer is empty. */
        xStreamBufferIsEmpty( xStreamBufferDemoHandle1, &uxIsEmpty );
        if( pdFALSE == uxIsEmpty )
        {
            while ( 0U == uxReceivedBytes )
            {
                /* If it is not empty, read the contents. */
                xStatus = xStreamBufferReceive( xStreamBufferDemoHandle1,
                                                aucRxData,
                                                portspecSTREAM_BUFFER_MESSAGE_LENGTH,
                                                &uxReceivedBytes,
                                                portspecSB_BLOCK_TIME );
            }
            if( pdPASS != xStatus )
            {
                uxErrorCount++;
            }

            prvStreamBufferErrorCheck(aucRxData, portspecSTREAM_BUFFER_MESSAGE_LENGTH);

            /* Clear the buffer. */
            xStatus = xStreamBufferReset( xStreamBufferDemoHandle1 );
            if( pdPASS != xStatus )
            {
                uxErrorCount++;
            }

            /* Now send the message down the chain. */
            /* Check to see if the stream buffer is empty. */
            xStreamBufferIsEmpty( xStreamBufferDemoHandle2, &uxIsEmpty );
            if( pdTRUE == uxIsEmpty )
            {
                /* If it is empty, add new text. */
                xStreamBufferSend( xStreamBufferDemoHandle2,
                                   aucRxData,
                                   sizeof( aucRxData ),
                                   portspecSB_BLOCK_TIME,
                                   &uxNumBytesWritten);
            }
        }

        uxTask2Count++;

        /* Delay for a period, then toggle StreamBuffer state. */
        ( void ) xTaskDelayUntil( &xLast, xRate );
    }
}
/*---------------------------------------------------------------------------*/

static void prvStreamBufferTxWaitCheck( void *pvParameters )
{
    portTickType xLast;
    portTickType xRate = 0U;
    portUnsignedBaseType uxNumBytesWritten = 0U;

    ( void ) pvParameters;

    /* Initialise xLastFlashTime prior to First Call tovTaskDelayUntil(). */
    xLast = xTaskGetTickCount();
    ( void ) xTaskDelayUntil( &xLast, xRate );

    for( ;; )
    {
        xStreamBufferSend( xStreamBufferDemoHandleWakeCheck,
                           aucStreamBufferMessage,
                           sizeof( aucStreamBufferMessage ),
                           portspecSB_BLOCK_TIME_FOREVER,
                           &uxNumBytesWritten);

        if( 4U == uxNumBytesWritten )
        {
            xTransmitWakeupOk = pdTRUE;
            xTaskDelete( NULL );
        }
    }
}
/*---------------------------------------------------------------------------*/

static void prvStreamBufferRxWaitCheck( void *pvParameters )
{
    portUnsignedBaseType uxReceivedBytes = 0U;
    portUInt8Type aucRxData[ portspecSTREAM_BUFFER_MESSAGE_LENGTH ];

    ( void ) pvParameters;

    xStreamBufferReceive( xStreamBufferDemoHandleWakeCheck,
                          aucRxData,
                          portspecSTREAM_BUFFER_MESSAGE_LENGTH,
                          &uxReceivedBytes,
                          portspecSB_BLOCK_TIME_FOREVER );

    if( 4U == uxReceivedBytes )
    {
        xReceiveWakeupOk = pdTRUE;
        xTaskDelete( NULL );
    }
}
/*---------------------------------------------------------------------------*/

void prvStreamBufferErrorCheck( portUInt8Type *pucArrayToCheck, portUInt8Type ucLengthToCheck )
{
    portUnsignedBaseType uxIndex;

    /* Check the message. */
    for( uxIndex = 0U; uxIndex < ucLengthToCheck; uxIndex++)
    {
        if( pucArrayToCheck[ uxIndex ] != aucStreamBufferMessage[ uxIndex ] )
        {
            safertosapiENTER_CRITICAL();
            for( ;; )
            {
                /* Loop forever. */
            }
        }
    }
}
/*---------------------------------------------------------------------------*/

portBaseType xAreStreamBufferDemoTasksStillRunning( void )
{
    portBaseType xReturn = pdTRUE;
    portspecCOMMON_PRIV_DATA_SECTION static portUnsignedBaseType uxTask0CountPrevious = 0U;
    portspecCOMMON_PRIV_DATA_SECTION static portUnsignedBaseType uxTask1CountPrevious = 0U;
    portspecCOMMON_PRIV_DATA_SECTION static portUnsignedBaseType uxTask2CountPrevious = 0U;

    if( pdTRUE != xReceiveWakeupOk )
    {
        xReturn = pdFALSE;
    }
    else if( pdTRUE != xTransmitWakeupOk )
    {
        xReturn = pdFALSE;
    }
    else if( uxTask0Count == uxTask0CountPrevious )
    {
        xReturn = pdFALSE;
    }
    else if( uxTask1Count == uxTask1CountPrevious )
    {
        xReturn = pdFALSE;
    }
    else if( uxTask2Count == uxTask2CountPrevious )
    {
        xReturn = pdFALSE;
    }
    else
    {
        xReturn = pdTRUE;
    }

    uxTask0CountPrevious = uxTask0Count;
    uxTask1CountPrevious = uxTask1Count;
    uxTask2CountPrevious = uxTask2Count;

    return xReturn;
}
/*---------------------------------------------------------------------------*/
