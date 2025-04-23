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

/* SafeRTOS includes. */
#include "SafeRTOS_API.h"

/* Demo application includes. */
#include "PortSpecifics.h"


/*-----------------------------------------------------------------------------
 * Maths Test Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acUnprivilegedTask1Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acUnprivilegedTask2Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acUnprivilegedTask3Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acUnprivilegedTask4Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acPrivilegedTask1Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acPrivilegedTask2Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acPrivilegedTask3Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };

portInt8Type acPrivilegedTask4Stack[ portspecMATHS_TEST_STACK_SIZE ] __attribute__( ( aligned ( portspecMATHS_TEST_STACK_SIZE ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Queue Blocking Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acQueueTestTask1Stack[ portspecBLOCK_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueTestTask2Stack[ portspecBLOCK_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueTestTask3Stack[ portspecBLOCK_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueTestTask4Stack[ portspecBLOCK_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueTestTask5Stack[ portspecBLOCK_Q_STACK_SIZE ]  __attribute__( ( aligned ( portspecBLOCK_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueTestTask6Stack[ portspecBLOCK_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_Q_STACK_SIZE ) ) ) = { 0 };

/* Queue buffers. */
portInt8Type acQueue1Buffer[ portspecBLOCK_Q_BUFFER_LENGTH_1 ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

portInt8Type acQueue2Buffer[ portspecBLOCK_Q_BUFFER_LENGTH_1 ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

portInt8Type acQueue3Buffer[ portspecBLOCK_Q_BUFFER_LENGTH_5 ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Block Time Test Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acBlockTimeTestTask1Stack[ portspecBLOCK_TIME_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_TIME_STACK_SIZE ) ) ) = { 0 };

portInt8Type acBlockTimeTestTask2Stack[ portspecBLOCK_TIME_STACK_SIZE ] __attribute__( ( aligned ( portspecBLOCK_TIME_STACK_SIZE ) ) ) = { 0 };

/* Queue buffer. */
portInt8Type acQueueBuffer[ portspecBLOCK_TIME_BUFFER_LENGTH ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Counting Semaphore Test Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acCountSemTestTask1Stack[ portspecCOUNTSEM_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecCOUNTSEM_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acCountSemTestTask2Stack[ portspecCOUNTSEM_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecCOUNTSEM_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acCountSemTestTask3Stack[ portspecCOUNTSEM_TASK3_STACK_SIZE ] __attribute__( ( aligned ( portspecCOUNTSEM_TASK3_STACK_SIZE ) ) ) = { 0 };

/* Semaphore buffers.
 * No actual data is stored into these buffers so the buffer
 * need only be large enough to hold the queue structure itself. */
portInt8Type acCountSem1[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

portInt8Type acCountSem2[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

portInt8Type acCountSem3[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Create Delete Test Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acCreateTaskStack[ portspecCREATE_TASK_STACK_SIZE ] __attribute__( ( aligned( portspecCREATE_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acCreatedDummyTaskStack[ portspecCREATED_TASKS_STACK_SIZE ] __attribute__( ( aligned( portspecCREATED_TASKS_STACK_SIZE ) ) ) = { 0 };
portInt8Type acDeleterTaskStack[ portspecCREATED_TASKS_STACK_SIZE ] __attribute__( ( aligned( portspecCREATED_TASKS_STACK_SIZE ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Dynamic Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acContinuousIncrementTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecDYNAMIC_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acLimitedIncrementTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecDYNAMIC_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acCounterControlTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecDYNAMIC_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueSendWhenSuspendedTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecDYNAMIC_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acQueueReceiveWhenSuspendedTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecDYNAMIC_TASK_STACK_SIZE ) ) ) = { 0 };

/* Queue buffer. */
portInt8Type acSuspendTestQueueBuffer[ portspecDYNAMIC_TASK_SUSPEND_QUEUE_BUFFER_LENGTH ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * LED Flash Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stack. */
portInt8Type acLedTaskStack[ portspecNUMBER_OF_LEDS ][ portspecLED_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecLED_TASK_STACK_SIZE ) ) ) = { 0 };
#ifndef USE_DISC_F429ZI
#define USE_DISC_F429ZI (1)
#endif


#if ( USE_NUCLEO_F429ZI == 1 )
/* Task parameters. */
const xLedFlashTaskParameters xLedTaskParameters[ portspecNUMBER_OF_LEDS ] =
{
    { 1U, 500U },
    { 2U, 1000U },
};
#elif ( USE_DISC_F429ZI == 1 )
/* Task parameters. */
const xLedFlashTaskParameters xLedTaskParameters[ portspecNUMBER_OF_LEDS ] =
{
    { 1U, 100U },
};
#else
#error "No board defined!"
#endif

/*-----------------------------------------------------------------------------
 * Poll Q Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acPollQueueTestTask1Stack[ portspecPOLL_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecPOLL_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acPollQueueTestTask2Stack[ portspecPOLL_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecPOLL_Q_STACK_SIZE ) ) ) = { 0 };

portInt8Type acPollQueueTestTask3Stack[ portspecPOLL_Q_STACK_SIZE ] __attribute__( ( aligned ( portspecPOLL_Q_STACK_SIZE ) ) ) = { 0 };

/* Queue buffers. */
portInt8Type acPollQueue1Buffer[ portspecPOLL_Q_BUFFER_LENGTH ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

portInt8Type acPollQueue2Buffer[ portspecPOLL_Q_BUFFER_LENGTH ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Binary Semaphore Test Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acSemaphoreTestTask1Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecSEMAPHORE_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acSemaphoreTestTask2Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecSEMAPHORE_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acSemaphoreTestTask3Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecSEMAPHORE_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acSemaphoreTestTask4Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecSEMAPHORE_TASK_STACK_SIZE ) ) ) = { 0 };

/* Semaphore buffers. No actual data is stored into these buffers so the
 * buffer need only be large enough to hold the queue structure itself. */
portInt8Type acSemaphore1[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };
portInt8Type acSemaphore2[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

/*-----------------------------------------------------------------------------
 * Timer Demo Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acTimerTestTaskStack[ portspecTIMER_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecTIMER_TASK_STACK_SIZE ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Task Notify Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acNotifiedTaskStack[ portspecNOTIFIED_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecNOTIFIED_TASK_STACK_SIZE ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Recursive Mutex Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acRecursiveMutexControllingTaskStack[ portspecMUTEX_STACK_SIZE ] __attribute__( ( aligned ( portspecMUTEX_STACK_SIZE ) ) ) = { 0 };
portInt8Type acRecursiveMutexBlockingTaskStack[ portspecMUTEX_STACK_SIZE ] __attribute__( ( aligned ( portspecMUTEX_STACK_SIZE ) ) ) = { 0 };
portInt8Type acRecursiveMutexPollingTaskStack[ portspecMUTEX_STACK_SIZE ] __attribute__( ( aligned ( portspecMUTEX_STACK_SIZE ) ) ) = { 0 };

/* Mutex buffer.
 * No actual data is stored into this buffer so the buffer need only be large
 * enough to hold the queue structure itself. */
portInt8Type acMutexBuffer[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Com Test Task Parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acTxStack[ portspecCOM_TEST_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecCOM_TEST_TASK_STACK_SIZE ) ) ) = { 0 };

portInt8Type acRxStack[ portspecCOM_TEST_TASK_STACK_SIZE ] __attribute__( ( aligned ( portspecCOM_TEST_TASK_STACK_SIZE ) ) ) = { 0 };

/*---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Event Multiplex Demo task parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acEvtMplxBlockTaskStack[ portspecEVT_MPLX_BLOCK_TASK_STACK_SIZE ]  __attribute__( ( aligned( portspecEVT_MPLX_BLOCK_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acEvtMplxControlTaskStack[ portspecEVT_MPLX_CONTROL_TASK_STACK_SIZE ]  __attribute__( ( aligned( portspecEVT_MPLX_BLOCK_TASK_STACK_SIZE ) ) ) = { 0 };

/* Queue buffers. */
portInt8Type acEvtMplxQueue1Buffer[ portspecEVT_MPLX_Q_BUFFER_LENGTH ] __attribute__( ( aligned( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };
portInt8Type acEvtMplxQueue2Buffer[ portspecEVT_MPLX_Q_BUFFER_LENGTH ] __attribute__( ( aligned( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };

/* Semaphore buffer.
 * No actual data is stored in this buffer so the buffer
 * need only be large enough to hold the queue structure itself. */
portInt8Type acEvtMplxSemaphoreBuffer[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };

/* Mutex buffer.
 * No actual data is stored in this buffer so the buffer
 * need only be large enough to hold the queue structure itself. */
portInt8Type acEvtMplxMutexBuffer[ safertosapiQUEUE_OVERHEAD_BYTES ] __attribute__( ( aligned( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };

/* Event Multiplex buffer. */
portInt8Type acEvtMplxBuffer[ portspecEVT_MPLX_BUFFER_LENGTH ] __attribute__( ( aligned( safertosapiWORD_ALIGNMENT ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * Stream buffer task parameters
 *---------------------------------------------------------------------------*/

/* Task stacks. */
portInt8Type acStreamBufferTaskStack[ portspecNUMBER_OF_STREAM_BUFFER_TASKS ][ portspecSTREAM_BUFFER_TASK_STACK_SIZE ] __attribute__( ( aligned( portspecSTREAM_BUFFER_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acStreamBufferTaskWakeCheckTxStack[ portspecSTREAM_BUFFER_TASK_STACK_SIZE ] __attribute__( ( aligned( portspecSTREAM_BUFFER_TASK_STACK_SIZE ) ) ) = { 0 };
portInt8Type acStreamBufferTaskWakeCheckRxStack[ portspecSTREAM_BUFFER_TASK_STACK_SIZE ] __attribute__( ( aligned( portspecSTREAM_BUFFER_TASK_STACK_SIZE ) ) ) = { 0 };


/*---------------------------------------------------------------------------*/
