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

#ifndef PORT_SPECIFICS_H
#define PORT_SPECIFICS_H

/* SafeRTOS Includes */
#include "SafeRTOS_API.h"


/*-----------------------------------------------------------------------------
 * Constants common to all demo tasks
 *---------------------------------------------------------------------------*/

/* This constant is used where it is desirable to locate all task TCBs in a
 * specified area of memory.
 * Not required for this product variant. */
#define portspecTCB_DATA_SECTION

/* This constant is used where it is desirable to locate all common privileged
 * demo data in a specified area of memory.
 * Not required for this product variant. */
#define portspecCOMMON_PRIV_DATA_SECTION


/*-----------------------------------------------------------------------------
 * Maths Test task parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecMATHS_TEST_STACK_SIZE   ( configMINIMAL_STACK_SIZE_WITH_FPU )

/* Task stacks */
extern portInt8Type acUnprivilegedTask1Stack[ portspecMATHS_TEST_STACK_SIZE ];
extern portInt8Type acUnprivilegedTask2Stack[ portspecMATHS_TEST_STACK_SIZE ];
extern portInt8Type acUnprivilegedTask3Stack[ portspecMATHS_TEST_STACK_SIZE ];
extern portInt8Type acUnprivilegedTask4Stack[ portspecMATHS_TEST_STACK_SIZE ];

extern portInt8Type acPrivilegedTask1Stack[ portspecMATHS_TEST_STACK_SIZE ];
extern portInt8Type acPrivilegedTask2Stack[ portspecMATHS_TEST_STACK_SIZE ];
extern portInt8Type acPrivilegedTask3Stack[ portspecMATHS_TEST_STACK_SIZE ];
extern portInt8Type acPrivilegedTask4Stack[ portspecMATHS_TEST_STACK_SIZE ];

#ifdef MATHS_TEST_C

/* MPU parameters */
/* Define portspecMATHS_TEST_DATA_SECTION
 * so that all Maths Test static data is in the same section. */
#define portspecMATHS_TEST_DATA_SECTION     __attribute__ (( section ( "__maths_test_data__" ) ) )

/* Two linker-defined symbols that give the start and end address of the
 * Maths test data section. */
extern portUInt32Type lnkMathsTestDataStart;
extern portUInt32Type lnkMathsTestDataEnd;

/* These constants are used to give the Unprivileged tasks
 * access to the file scope check variables. */
#define portspecMATHS_TEST_DATA_ADDR    ( ( void * ) &lnkMathsTestDataStart )
#define portspecMATHS_TEST_DATA_SIZE    ( ( portUInt32Type ) &lnkMathsTestDataEnd - ( portUInt32Type ) &lnkMathsTestDataStart )

/* This definition corresponds to the xMPUParameters structure that is passed
 * to xTaskCreate() as part of the xTaskParameters structure when the Maths
 * Test tasks are created. It defines the privilege level and region
 * definitions for the Maths test tasks. All the Blocking Queue tasks run
 * in Unprivileged (User) mode with 1 additional region that corresponds to the
 * linker section that holds the Blocking Queue static data. */
#define portspecMATHS_PRIV_NO_REGION_PARMS  \
pdTRUE,                                     \
{                                           \
    mpuPRIVILEGED_TASK,                     \
    {                                       \
        { NULL, 0U, 0U, 0U },               \
        { NULL, 0U, 0U, 0U },               \
        { NULL, 0U, 0U, 0U }                \
    }                                       \
}

#define portspecMATHS_UNPRIV_REGION_PARMS                       \
pdTRUE,                                                         \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecMATHS_TEST_DATA_ADDR,                       \
            portspecMATHS_TEST_DATA_SIZE,                       \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xMathsPrivTask1PortParameters       portspecMATHS_PRIV_NO_REGION_PARMS
#define xMathsPrivTask2PortParameters       portspecMATHS_PRIV_NO_REGION_PARMS
#define xMathsPrivTask3PortParameters       portspecMATHS_PRIV_NO_REGION_PARMS
#define xMathsPrivTask4PortParameters       portspecMATHS_PRIV_NO_REGION_PARMS

#define xMathsUnprivTask1PortParameters     portspecMATHS_UNPRIV_REGION_PARMS
#define xMathsUnprivTask2PortParameters     portspecMATHS_UNPRIV_REGION_PARMS
#define xMathsUnprivTask3PortParameters     portspecMATHS_UNPRIV_REGION_PARMS
#define xMathsUnprivTask4PortParameters     portspecMATHS_UNPRIV_REGION_PARMS

#endif /* MATHS_TEST_C */


/*-----------------------------------------------------------------------------
 * Blocking Queue demo parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecBLOCK_Q_STACK_SIZE          ( 512U )
#define portspecBLOCK_Q_QUEUE_LENGTH_1      ( 1U )
#define portspecBLOCK_Q_QUEUE_LENGTH_5      ( 5U )
#define portspecBLOCK_Q_QUEUE_ITEM_SIZE     ( sizeof( portUInt16Type ) )
#define portspecBLOCK_Q_BUFFER_LENGTH_1     ( ( portspecBLOCK_Q_QUEUE_LENGTH_1 * portspecBLOCK_Q_QUEUE_ITEM_SIZE ) + safertosapiQUEUE_OVERHEAD_BYTES )
#define portspecBLOCK_Q_BUFFER_LENGTH_5     ( ( portspecBLOCK_Q_QUEUE_LENGTH_5 * portspecBLOCK_Q_QUEUE_ITEM_SIZE ) + safertosapiQUEUE_OVERHEAD_BYTES )

/* Task stacks */
extern portInt8Type acQueueTestTask1Stack[ portspecBLOCK_Q_STACK_SIZE ];
extern portInt8Type acQueueTestTask2Stack[ portspecBLOCK_Q_STACK_SIZE ];
extern portInt8Type acQueueTestTask3Stack[ portspecBLOCK_Q_STACK_SIZE ];
extern portInt8Type acQueueTestTask4Stack[ portspecBLOCK_Q_STACK_SIZE ];
extern portInt8Type acQueueTestTask5Stack[ portspecBLOCK_Q_STACK_SIZE ];
extern portInt8Type acQueueTestTask6Stack[ portspecBLOCK_Q_STACK_SIZE ];

/* Queue buffers */
extern portInt8Type acQueue1Buffer[ portspecBLOCK_Q_BUFFER_LENGTH_1 ];
extern portInt8Type acQueue2Buffer[ portspecBLOCK_Q_BUFFER_LENGTH_1 ];
extern portInt8Type acQueue3Buffer[ portspecBLOCK_Q_BUFFER_LENGTH_5 ];

#ifdef BLOCK_Q_C

/* MPU parameters */
/* Define portspecBLOCK_Q_DATA_SECTION
 * so that all Blocking Queue static data is in the same section. */
#define portspecBLOCK_Q_DATA_SECTION    __attribute__ (( section ( "__block_q_data__" ) ) )

/* Two linker-defined symbols that give the start and end address of the
 * Blocking Queue test data section. */
extern portUInt32Type lnkBlockQDataStart;
extern portUInt32Type lnkBlockQDataEnd;

/* The address and size of the Blocking Queue data section. */
#define portspecBLOCK_Q_DATA_ADDR   ( ( void * ) &lnkBlockQDataStart )
#define portspecBLOCK_Q_DATA_SIZE   ( ( portUInt32Type ) &lnkBlockQDataEnd - ( portUInt32Type ) &lnkBlockQDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Blocking Queue tasks are created. It defines the
 * privilege level and region definitions for the Blocking Queue tasks as well
 * as informing the scheduler that the FPU will not be used by these tasks.
 * All the Blocking Queue tasks run in Unprivileged (User) mode with 1
 * additional region that corresponds to the linker section that holds the
 * Blocking Queue static data. */
#define portspecBLOCK_Q_TASK_PARAMETERS                         \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecBLOCK_Q_DATA_ADDR,                          \
            portspecBLOCK_Q_DATA_SIZE,                          \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xBlockingQueueConsumerTask1PortParameters   portspecBLOCK_Q_TASK_PARAMETERS
#define xBlockingQueueProducerTask1PortParameters   portspecBLOCK_Q_TASK_PARAMETERS
#define xBlockingQueueConsumerTask2PortParameters   portspecBLOCK_Q_TASK_PARAMETERS
#define xBlockingQueueProducerTask2PortParameters   portspecBLOCK_Q_TASK_PARAMETERS
#define xBlockingQueueProducerTask3PortParameters   portspecBLOCK_Q_TASK_PARAMETERS
#define xBlockingQueueConsumerTask3PortParameters   portspecBLOCK_Q_TASK_PARAMETERS

#endif /* BLOCK_Q_C */


/*-----------------------------------------------------------------------------
 * Block Time demo parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecBLOCK_TIME_STACK_SIZE       ( 512U )
#define portspecBLOCK_TIME_QUEUE_LENGTH     ( 5U )
#define portspecBLOCK_TIME_QUEUE_ITEM_SIZE  ( sizeof( portBaseType ) )
#define portspecBLOCK_TIME_BUFFER_LENGTH    ( ( portspecBLOCK_TIME_QUEUE_LENGTH * portspecBLOCK_TIME_QUEUE_ITEM_SIZE ) + safertosapiQUEUE_OVERHEAD_BYTES )

/* Task stacks */
extern portInt8Type acBlockTimeTestTask1Stack[ portspecBLOCK_TIME_STACK_SIZE ];
extern portInt8Type acBlockTimeTestTask2Stack[ portspecBLOCK_TIME_STACK_SIZE ];

/* Queue buffer */
extern portInt8Type acQueueBuffer[ portspecBLOCK_TIME_BUFFER_LENGTH ];

#ifdef BLOCK_TIME_TEST_C

/* MPU parameters */
/* Define portspecBLOCK_TIME_TEST_DATA_SECTION
 * so that all Block Time Test static data is in the same section. */
#define portspecBLOCK_TIME_TEST_DATA_SECTION    __attribute__ (( section ( "__block_tim_data__" ) ) )

/* Linker defined symbols that give the start and end address of the
 * Block Time Test data section. */
extern portUInt32Type lnkBlockTimeDataStart;
extern portUInt32Type lnkBlockTimeDataEnd;

/* The base address and size of the Block Time data section. */
#define portspecBLOCK_TIME_DATA_ADDR   ( ( void * ) &lnkBlockTimeDataStart )
#define portspecBLOCK_TIME_DATA_SIZE   ( ( portUInt32Type ) &lnkBlockTimeDataEnd - ( portUInt32Type ) &lnkBlockTimeDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Block Time Test tasks are created. It defines the
 * privilege level and region definitions for the Block Time Test tasks as well
 * as informing the scheduler that the FPU will not be used by these tasks.
 * All the Block Time Test tasks run in Unprivileged (User) mode with 1
 * additional region that corresponds to the linker section that holds the
 * Block Time Test static data. */
#define portspecBLOCK_TIME_TEST_PARAMETERS                      \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecBLOCK_TIME_DATA_ADDR,                       \
            portspecBLOCK_TIME_DATA_SIZE,                       \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xPrimaryBlockTimeTestTaskPortParameters     portspecBLOCK_TIME_TEST_PARAMETERS
#define xSecondaryBlockTimeTestTaskPortParameters   portspecBLOCK_TIME_TEST_PARAMETERS

#endif /* BLOCK_TIME_TEST_C */


/*-----------------------------------------------------------------------------
 * Counting Semaphore demo parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecCOUNTSEM_TASK_STACK_SIZE    ( 512U )
#define portspecCOUNTSEM_TASK3_STACK_SIZE   ( 512U )

/* Task stacks. */
extern portInt8Type acCountSemTestTask1Stack[ portspecCOUNTSEM_TASK_STACK_SIZE ];
extern portInt8Type acCountSemTestTask2Stack[ portspecCOUNTSEM_TASK_STACK_SIZE ];
extern portInt8Type acCountSemTestTask3Stack[ portspecCOUNTSEM_TASK3_STACK_SIZE ];

/* Counting Semaphore Test Semaphore buffers.
 * No actual data is stored into these buffers so the buffer
 * need only to be large enough to hold the queue structure itself. */
extern portInt8Type acCountSem1[ safertosapiQUEUE_OVERHEAD_BYTES ];
extern portInt8Type acCountSem2[ safertosapiQUEUE_OVERHEAD_BYTES ];
extern portInt8Type acCountSem3[ safertosapiQUEUE_OVERHEAD_BYTES ];

#ifdef COUNTING_SEMAPHORE_TEST_C

/* The Interrupt Task will run unprivileged. */
#define portspecCOUNTSEM_INTERRUPT_TASK_RUNS_UNPRIVILEGED   1

/* MPU parameters */
/* Define portspecCOUNT_SEM_TASK_DATA_SECTION and portspecCOUNTSEM_TASK_ZERO_DATA_SECTION
 * so that all Counting Semaphore static data is in the same section. */
#define portspecCOUNTSEM_TASK_DATA_SECTION          __attribute__ ( ( section ("__counting_semaphore_task_data__" ) ) )
#define portspecCOUNTSEM_TASK_ZERO_DATA_SECTION     __attribute__ ( ( section ("__counting_semaphore_task_zero_data__") ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Counting Semaphore Test data section. */
extern portUInt32Type lnkCountSemDataStart;
extern portUInt32Type lnkCountSemDataEnd;

/* The address and size of the Counting Semaphore Test data section. */
#define portspecCOUNTSEM_TASK_DATA_ADDR     ( ( void * ) &lnkCountSemDataStart )
#define portspecCOUNTSEM_TASK_DATA_SIZE     ( ( portUInt32Type ) &lnkCountSemDataEnd - ( portUInt32Type ) &lnkCountSemDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Counting Semaphore tasks are created. It defines the
 * privilege level and region definitions for the Create Delete Test tasks as
 * well as informing the scheduler that the FPU will not be used by these tasks.
 * All the Create Delete Test tasks run in Unprivileged (User) mode with 1
 * additional region that corresponds to the linker section that holds the
 * Create Delete Test static data. */
#define portspecCOUNTING_SEMAPHORE_PARAMETERS                   \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecCOUNTSEM_TASK_DATA_ADDR,                    \
            portspecCOUNTSEM_TASK_DATA_SIZE,                    \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define portspecINTERRUPT_SEMAPHORE_PARAMETERS  \
pdFALSE,                                        \
{                                               \
    mpuPRIVILEGED_TASK,                         \
    {                                           \
        { NULL, 0U, 0U, 0U },                   \
        { NULL, 0U, 0U, 0U },                   \
        { NULL, 0U, 0U, 0U }                    \
    }                                           \
}

#define portspecINTERRUPT_MPU_UPDATE_PARAMETERS                 \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecCOUNTSEM_TASK_DATA_ADDR,                    \
            portspecCOUNTSEM_TASK_DATA_SIZE,                    \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xCountSemaphoreTestTask1PortParameters      portspecCOUNTING_SEMAPHORE_PARAMETERS
#define xCountSemaphoreTestTask2PortParameters      portspecCOUNTING_SEMAPHORE_PARAMETERS
#define xCountSemaphoreTestTask3PortParameters      portspecINTERRUPT_SEMAPHORE_PARAMETERS
#define xCountSemaphoreTestTask3MpuUpdParameters    portspecINTERRUPT_MPU_UPDATE_PARAMETERS

#endif /* COUNTING_SEMAPHORE_TEST_C */


/*-----------------------------------------------------------------------------
 * Create Delete demo parameters.
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecCREATE_TASK_STACK_SIZE      ( 512U )
#define portspecCREATED_TASKS_STACK_SIZE    ( configMINIMAL_STACK_SIZE_NO_FPU )

/* Task stacks */
extern portInt8Type acCreateTaskStack[ portspecCREATE_TASK_STACK_SIZE ];
extern portInt8Type acCreatedDummyTaskStack[ portspecCREATED_TASKS_STACK_SIZE ];
extern portInt8Type acDeleterTaskStack[ portspecCREATED_TASKS_STACK_SIZE ];

#ifdef CREATE_DELETE_TASK_C

/* MPU parameters */
/* Define portspecCREATE_DELETE_DATA_SECTION
 * so that all Create Delete Test static data is in the same section. */
#define portspecCREATE_DELETE_DATA_SECTION  __attribute__ ( ( section ( "__create_delete_data__") ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Create Delete Test data section. */
extern portUInt32Type lnkCreateDeleteDataStart;
extern portUInt32Type lnkCreateDeleteDataEnd;

/* The address and size of the Create Delete Test data section. */
#define portspecCREATE_DELETE_DATA_ADDR     ( ( void * ) &lnkCreateDeleteDataStart )
#define portspecCREATE_DELETE_DATA_SIZE     ( ( portUInt32Type ) &lnkCreateDeleteDataEnd - ( portUInt32Type ) &lnkCreateDeleteDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Create Delete Test tasks are created. It defines the
 * privilege level and region definitions for the Create Delete Test tasks as
 * well as informing the scheduler that the FPU will not be used by these tasks.
 * All the Create Delete Test tasks run in Unprivileged (User) mode with 1
 * additional region that corresponds to the linker section that holds the
 * Create Delete Test static data. */
#define portspecCREATE_DELETE_TASK_PARAMETERS                   \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecCREATE_DELETE_DATA_ADDR,                    \
            portspecCREATE_DELETE_DATA_SIZE,                    \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xCreateTaskPortParameters       portspecCREATE_DELETE_TASK_PARAMETERS
#define xCreatedDummyTaskPortParameters portspecCREATE_DELETE_TASK_PARAMETERS
#define xDeleterTaskPortParameters      portspecCREATE_DELETE_TASK_PARAMETERS

#endif /* CREATE_DELETE_TASK_C */


/*-----------------------------------------------------------------------------
 * Dynamic task demo parameters.
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecDYNAMIC_TASK_STACK_SIZE                     ( 512U )
#define portspecDYNAMIC_TASK_SUSPENDED_QUEUE_LENGTH         ( 1U )
#define portspecDYNAMIC_TASK_QUEUE_ITEM_SIZE                ( sizeof( portUInt32Type ) )
#define portspecDYNAMIC_TASK_SUSPEND_QUEUE_BUFFER_LENGTH    ( ( portspecDYNAMIC_TASK_SUSPENDED_QUEUE_LENGTH * portspecDYNAMIC_TASK_QUEUE_ITEM_SIZE ) + safertosapiQUEUE_OVERHEAD_BYTES )

/* Task stacks */
extern portInt8Type acContinuousIncrementTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ];
extern portInt8Type acLimitedIncrementTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ];
extern portInt8Type acCounterControlTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ];
extern portInt8Type acQueueSendWhenSuspendedTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ];
extern portInt8Type acQueueReceiveWhenSuspendedTaskStack[ portspecDYNAMIC_TASK_STACK_SIZE ];

/* Queue buffers */
extern portInt8Type acSuspendTestQueueBuffer[ portspecDYNAMIC_TASK_SUSPEND_QUEUE_BUFFER_LENGTH ];

#ifdef DYNAMIC_MANIPULATION_C

/* MPU parameters */
/* Define portspecDYNAMIC_TASK_DATA_SECTION
 * so that all Dynamic Task static data is in the same section. */
#define portspecDYNAMIC_TASK_DATA_SECTION   __attribute__ ( ( section ( "__dynamic_task_data__") ) )

/* Two linker-defined symbols that give the start and end address of the
 * Dynamic Task Test data section. */
extern portUInt32Type lnkDynamicTaskDataStart;
extern portUInt32Type lnkDynamicTaskDataEnd;

/* The address and size of the Dynamic Task data section. */
#define portspecDYNAMIC_TASK_DATA_ADDR      ( ( void * ) &lnkDynamicTaskDataStart )
#define portspecDYNAMIC_TASK_DATA_SIZE      ( ( portUInt32Type ) &lnkDynamicTaskDataEnd - ( portUInt32Type ) &lnkDynamicTaskDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Dynamic Tasks are created. It defines the privilege level
 * and region definitions for the Dynamic Tasks as well as informing the
 * scheduler that the FPU will not be used by these tasks. All the Dynamic
 * Tasks run in Unprivileged (User) mode with 1 additional region that
 * corresponds to the linker section that holds the Dynamic Tasks static data. */
#define portspecDYNAMIC_TASK_PARAMETERS                         \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecDYNAMIC_TASK_DATA_ADDR,                     \
            portspecDYNAMIC_TASK_DATA_SIZE,                     \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xContinuousIncrementTaskPortParameters          portspecDYNAMIC_TASK_PARAMETERS
#define xLimitedIncrementTaskPortParameters             portspecDYNAMIC_TASK_PARAMETERS
#define xCounterControlTaskPortParameters               portspecDYNAMIC_TASK_PARAMETERS
#define xQueueSendWhenSuspendedTaskPortParameters       portspecDYNAMIC_TASK_PARAMETERS
#define xQueueReceiveWhenSuspendedTaskPortParameters    portspecDYNAMIC_TASK_PARAMETERS

#endif /* DYNAMIC_MANIPULATION_C */


/*-----------------------------------------------------------------------------
 * LED Flash demo parameters.
 *---------------------------------------------------------------------------*/

/* Constants */
#if( USE_NUCLEO_F429ZI == 1 )
#define portspecNUMBER_OF_LEDS          ( 2U )

#elif( USE_DISC_F429ZI == 1 )
#define portspecNUMBER_OF_LEDS          ( 1U )

#else
#error "No board defined!"
#endif

#define portspecLED_TASK_STACK_SIZE     ( configMINIMAL_STACK_SIZE_NO_FPU )

/* Structure used to pass parameters to the LED Flash tasks. */
typedef struct LED_FLASH_TASK_PARAMETERS
{
    portUnsignedBaseType uxLEDNumber;   /* The LED to be flashed. */
    portTickType xFlashRate_ms;         /* The flash rate in milliseconds. */
} xLedFlashTaskParameters;

/* LED Flash Task input parameters */
extern const xLedFlashTaskParameters xLedTaskParameters[ portspecNUMBER_OF_LEDS ];

/* Task stacks */
extern portInt8Type acLedTaskStack[ portspecNUMBER_OF_LEDS ][ portspecLED_TASK_STACK_SIZE ];

#ifdef FLASH_LED_C

/* MPU parameters */
/* Define portspecLED_TASK_DATA_SECTION
 * so that all LED Task static data is in the same section. */
#define portspecLED_TASK_DATA_SECTION   __attribute__ ( ( section ( "__led_task_data__") ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Flash LED Test data section. */
extern portUInt32Type lnkLedTaskDataStart;
extern portUInt32Type lnkLedTaskDataEnd;

/* The address and size of the LED Task data section. */
#define portspecLED_TASK_DATA_ADDR      ( ( void * ) &lnkLedTaskDataStart )
#define portspecLED_TASK_DATA_SIZE      ( ( portUInt32Type ) &lnkLedTaskDataEnd - ( portUInt32Type ) &lnkLedTaskDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the LED Tasks are created. It defines the privilege level and
 * region definitions for the LED Tasks as well as informing the scheduler that
 * the FPU will not be used by these tasks. All the LED Tasks run in
 * Unprivileged (User) mode with 1 additional region that corresponds to the
 * linker section that holds the Flash task static data. */
#define portspecLED_FLASH_TASK_PARAMETERS                       \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecLED_TASK_DATA_ADDR,                         \
            portspecLED_TASK_DATA_SIZE,                         \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xLEDFlashTaskPortParameters     portspecLED_FLASH_TASK_PARAMETERS

#endif /* FLASH_LED_C */


/*-----------------------------------------------------------------------------
 * Poll Queue task parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecPOLL_Q_STACK_SIZE       ( 512U )
#define portspecPOLL_Q_QUEUE_LENGTH     ( 10U )
#define portspecPOLL_Q_QUEUE_ITEM_SIZE  ( sizeof( portUInt16Type ) )
#define portspecPOLL_Q_BUFFER_LENGTH    ( ( portspecPOLL_Q_QUEUE_LENGTH * portspecPOLL_Q_QUEUE_ITEM_SIZE ) + safertosapiQUEUE_OVERHEAD_BYTES )

/* Task stacks */
extern portInt8Type acPollQueueTestTask1Stack[ portspecPOLL_Q_STACK_SIZE ];
extern portInt8Type acPollQueueTestTask2Stack[ portspecPOLL_Q_STACK_SIZE ];
extern portInt8Type acPollQueueTestTask3Stack[ portspecPOLL_Q_STACK_SIZE ];

/* Queue buffers */
extern portInt8Type acPollQueue1Buffer[ portspecPOLL_Q_BUFFER_LENGTH ];
extern portInt8Type acPollQueue2Buffer[ portspecPOLL_Q_BUFFER_LENGTH ];

#ifdef POLLED_Q_C

/* MPU parameters */
/* Define portspecPOLL_Q_DATA_SECTION
 * so that all Poll Queue static data is in the same section. */
#define portspecPOLL_Q_DATA_SECTION     __attribute__ ( ( section ( "__poll_q_data__") ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Poll Queue Test data section. */
extern portUInt32Type lnkPollQDataStart;
extern portUInt32Type lnkPollQDataEnd;

/* The address and size of the Poll Queue data section. */
#define portspecPOLL_Q_DATA_ADDR    ( ( void * ) &lnkPollQDataStart )
#define portspecPOLL_Q_DATA_SIZE    ( ( portUInt32Type ) &lnkPollQDataEnd - ( portUInt32Type ) &lnkPollQDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Poll Queue tasks are created. It defines the privilege
 * level and region definitions for the Poll Queue tasks as well as informing
 * the scheduler that the FPU will not be used by these tasks. All the Poll
 * Queue tasks run in Unprivileged (User) mode with 1 additional region that
 * corresponds to the linker section that holds the Poll Queue static data. */
#define portspecPOLL_Q_PARAMETERS                               \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecPOLL_Q_DATA_ADDR,                           \
            portspecPOLL_Q_DATA_SIZE,                           \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xPolledQueueConsumerTaskPortParameters  portspecPOLL_Q_PARAMETERS
#define xPolledQueueProducerTaskPortParameters  portspecPOLL_Q_PARAMETERS
#define xPolledQueueSendFrontTaskPortParameters portspecPOLL_Q_PARAMETERS

#endif /* POLLED_Q_C */


/*-----------------------------------------------------------------------------
 * Binary Semaphore demo parameters.
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecSEMAPHORE_TASK_STACK_SIZE   ( 512U )

/* Task stacks */
extern portInt8Type acSemaphoreTestTask1Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ];
extern portInt8Type acSemaphoreTestTask2Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ];
extern portInt8Type acSemaphoreTestTask3Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ];
extern portInt8Type acSemaphoreTestTask4Stack[ portspecSEMAPHORE_TASK_STACK_SIZE ];

/* Semaphore buffers.
 * No actual data is stored into these buffers so the buffer need only be large
 * enough to hold the queue structure itself. */
extern portInt8Type acSemaphore1[ safertosapiQUEUE_OVERHEAD_BYTES ];
extern portInt8Type acSemaphore2[ safertosapiQUEUE_OVERHEAD_BYTES ];

#ifdef SEMAPHORE_TEST_C

/* MPU parameters */
/* Define portspecSEMAPHORE_TASK_DATA_SECTION
 * so that all Binary Semaphore static data is in the same section. */
#define portspecSEMAPHORE_TASK_DATA_SECTION         __attribute__ ( ( section ( "__binary_semaphore_task_data__") ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Binary Semaphore Test data section. */
extern portUInt32Type lnkBinarySemaphoreDataStart;
extern portUInt32Type lnkBinarySemaphoreDataEnd;

/* The address and size of the Binary Semaphore Task data section. */
#define portspecBINARY_SEMAPHORE_TASK_DATA_ADDR     (  ( void * ) &lnkBinarySemaphoreDataStart )
#define portspecBINARY_SEMAPHORE_TASK_DATA_SIZE     (  ( portUInt32Type ) &lnkBinarySemaphoreDataEnd -  ( portUInt32Type ) &lnkBinarySemaphoreDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Binary Semaphore tasks are created. It defines the
 * privilege level and region definitions for the Binary Semaphore tasks as
 * well as informing the scheduler that the FPU will not be used by these tasks.
 * All the Binary Semaphore tasks run in Unprivileged (User) mode with 1
 * additional region that corresponds to the linker section that holds the
 * Binary Semaphore tasks static data. */
#define portspecBINARY_SEMAPHORE_PARAMETERS                     \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecBINARY_SEMAPHORE_TASK_DATA_ADDR,            \
            portspecBINARY_SEMAPHORE_TASK_DATA_SIZE,            \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xSemaphoreTestTask1PortParameters   portspecBINARY_SEMAPHORE_PARAMETERS
#define xSemaphoreTestTask2PortParameters   portspecBINARY_SEMAPHORE_PARAMETERS
#define xSemaphoreTestTask3PortParameters   portspecBINARY_SEMAPHORE_PARAMETERS
#define xSemaphoreTestTask4PortParameters   portspecBINARY_SEMAPHORE_PARAMETERS

#endif /* SEMAPHORE_TEST_C */


/*-----------------------------------------------------------------------------
 * Timer demo parameters.
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecTIMER_TASK_STACK_SIZE   ( 1024U )

/* Task stacks */
extern portInt8Type acTimerTestTaskStack[ portspecTIMER_TASK_STACK_SIZE ];

#ifdef TIMERTEST_C

/* Define portspecTIMER_TEST_DATA_SECTION and portspecTIMER_TEST_ZERO_DATA_SECTION
 * so that all timer demo static data is in the same section. */
#define portspecTIMER_TEST_DATA_SECTION         __attribute__ ( ( section ("__timer_demo_task_data__" ) ) )
#define portspecTIMER_TEST_ZERO_DATA_SECTION    __attribute__ ( ( section ("__timer_demo_task_zero_data__") ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Timer demo data section. */
extern portUInt32Type lnkTimerTestDataStart;
extern portUInt32Type lnkTimerTestDataEnd;

/* The address and size of the Timer Test data section. */
#define portspecTIMER_TEST_DATA_ADDR    ( ( void * ) &lnkTimerTestDataStart )
#define portspecTIMER_TEST_DATA_SIZE    ( ( portUInt32Type ) &lnkTimerTestDataEnd - ( portUInt32Type ) &lnkTimerTestDataStart )

/* This definition corresponds to the xMPUParameters structure that is passed
 * to xTaskCreate() as part of the xTaskParameters structure when the Timer
 * demo tasks are created. It defines the privilege level and region
 * definitions for the Timer demo tasks. All the Timer demo tasks run in
 * Unprivileged (User) mode with 1 additional region that corresponds to the
 * linker section that holds the Timer demo static data. */
#define portspecTIMER_TEST_TASK_PARAMETERS                      \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecTIMER_TEST_DATA_ADDR,                       \
            portspecTIMER_TEST_DATA_SIZE,                       \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xTimerTestTaskPortParameters    portspecTIMER_TEST_TASK_PARAMETERS

#endif /* TIMERTEST_C */


/*-----------------------------------------------------------------------------
 * Task Notify demo task parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecNOTIFIED_TASK_STACK_SIZE    ( 512U )

/* Task stacks */
extern portInt8Type acNotifiedTaskStack[ portspecNOTIFIED_TASK_STACK_SIZE ];

#ifdef TASKNOTIFYDEMO_C

/* MPU parameters */
/* Define portspecTASK_NOTIFY_DATA_SECTION and portspecTASK_NOTIFY_ZERO_DATA_SECTION
 * so that all task notify demo static data is in the same section. */
#define portspecTASK_NOTIFY_DATA_SECTION        __attribute__ ( ( section ("__notified_task_data__" ) ) )
#define portspecTASK_NOTIFY_ZERO_DATA_SECTION   __attribute__ ( ( section ("__notified_task_zero_data__" ) ) )

/* Two linker-defined symbols that give the start address and the size of the
 * Task Notify demo data section. */
extern portUInt32Type lnkTaskNotifyDataStart;
extern portUInt32Type lnkTaskNotifyDataEnd;

/* The address and size of the Notify demo data section. */
#define portspecTASK_NOTIFY_DATA_ADDR       ( ( void * ) &lnkTaskNotifyDataStart )
#define portspecTASK_NOTIFY_DATA_SIZE       ( ( portUInt32Type ) &lnkTaskNotifyDataEnd - ( portUInt32Type ) &lnkTaskNotifyDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * that is passed to xTaskCreate() as part of the xTaskParameters structure
 * when the Notify demo tasks are created. It defines the privilege level and
 * region definitions for the Notify demo tasks. All the Notify demo tasks run in
 * Unprivileged (User) mode with 1 additional region that corresponds to the
 * linker section that holds the Notify demo static data. */
#define portspecNOTIFIED_TASK_PARAMETERS                        \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecTASK_NOTIFY_DATA_ADDR,                      \
            portspecTASK_NOTIFY_DATA_SIZE,                      \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xNotifiedTaskPortParameters     portspecNOTIFIED_TASK_PARAMETERS

#endif /* TASKNOTIFYDEMO_C */


/*-----------------------------------------------------------------------------
 * Recursive mutex task parameters
 *---------------------------------------------------------------------------*/

/* Task constants */
#define portspecMUTEX_STACK_SIZE    ( 512U )

/* Mutex demo task stacks */
extern portInt8Type acRecursiveMutexControllingTaskStack[ portspecMUTEX_STACK_SIZE ];
extern portInt8Type acRecursiveMutexBlockingTaskStack[ portspecMUTEX_STACK_SIZE ];
extern portInt8Type acRecursiveMutexPollingTaskStack[ portspecMUTEX_STACK_SIZE ];

/* Mutex buffer */
extern portInt8Type acMutexBuffer[ safertosapiQUEUE_OVERHEAD_BYTES ];

#ifdef TASK_MUTEX_C

/* MPU parameters */
/* Define portspecREC_MUTEX_DATA_SECTION
 * so that all static data is in the same section. */
#define portspecMUTEX_TASK_DATA_SECTION     __attribute__ ( ( section ( "__rec_mutex_data__") ) )

/* Two linker-defined symbols that give the start and end address of the
 * Mutex demo data section. */
extern portUInt32Type lnkRecMutexDataStart;
extern portUInt32Type lnkRecMutexDataEnd;

/* The address and size of the Recursive Mutex data section. */
#define portspecREC_MUTEX_DATA_ADDR         ( ( void * ) &lnkRecMutexDataStart )
#define portspecREC_MUTEX_DATA_SIZE         ( ( portUInt32Type ) &lnkRecMutexDataEnd - ( portUInt32Type ) &lnkRecMutexDataStart )

/* This definition corresponds to the xMPUParameters structure that is passed
 * to xTaskCreate() as part of the xTaskParameters structure when the
 * Recursive Mutex tasks are created. It defines the privilege level and region
 * definitions for the Recursive Mutex tasks. All the Recursive Mutex tasks run in
 * Unprivileged (User) mode with 1 additional region that corresponds to the
 * linker section that holds the Recursive Mutex static data. */
#define portspecREC_MUTEX_TASK_PARAMETERS                               \
pdFALSE,                                                                \
{                                                                       \
    mpuUNPRIVILEGED_TASK,                                               \
    {                                                                   \
        {                                                               \
            portspecREC_MUTEX_DATA_ADDR,                                \
            portspecREC_MUTEX_DATA_SIZE,                                \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE |         \
              mpuREGION_EXECUTE_NEVER |                                 \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),            \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                           \
        },                                                              \
        { NULL, 0U, 0U, 0U },                                           \
        { NULL, 0U, 0U, 0U }                                            \
    }                                                                   \
}

#define xRecursiveMutexControllingTaskPortParameters    portspecREC_MUTEX_TASK_PARAMETERS
#define xRecursiveMutexBlockingTaskPortParameters       portspecREC_MUTEX_TASK_PARAMETERS
#define xRecursiveMutexPollingTaskPortParameters        portspecREC_MUTEX_TASK_PARAMETERS

#endif /* TASK_MUTEX_C */


/*-----------------------------------------------------------------------------
 * ComTest demo parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecCOM_TEST_TASK_STACK_SIZE    ( 512U )

/* Task stacks */
extern portInt8Type acTxStack[ portspecCOM_TEST_TASK_STACK_SIZE ];
extern portInt8Type acRxStack[ portspecCOM_TEST_TASK_STACK_SIZE ];

#ifdef COMTEST_C

/* MPU parameters */
/* Define portspecCOM_TEST_DATA_SECTION and portspecCOM_TEST_ZERO_DATA_SECTION
 * so that all Com Test static data is in the same section. */
#define portspecCOM_TEST_DATA_SECTION       __attribute__ ( ( section ("__comtest_task_data__" ) ) )
#define portspecCOM_TEST_ZERO_DATA_SECTION  __attribute__ ( ( section ("__comtest_task_zero_data__") ) )

/* Two linker-defined symbols that give the start and end address of the
 * Com Test data section. */
extern portUInt32Type lnkComTestDataStart;
extern portUInt32Type lnkComTestDataEnd;

/* The address and size of the Com Test data section. */
#define portspecCOM_TEST_DATA_ADDR      ( ( void * ) &lnkComTestDataStart )
#define portspecCOM_TEST_DATA_SIZE      ( ( portUInt32Type ) &lnkComTestDataEnd - ( portUInt32Type ) &lnkComTestDataStart )

/* This definition corresponds to the xUsingFPU and xMPUParameters structure
 * members that are passed to xTaskCreate() as part of the xTaskParameters
 * structure when the Com Test tasks are created. It defines the privilege
 * level and region definitions for the Com Test tasks as well as informing the
 * scheduler that the FPU will not be used by these tasks.
 * All the Com Test tasks run in Unprivileged (User) mode with 1 additional
 * region which corresponds to the linker section that holds the Com Test tasks
 * static data. */
#define portspecCOM_TEST_TASK_PARAMETERS                        \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecCOM_TEST_DATA_ADDR,                         \
            portspecCOM_TEST_DATA_SIZE,                         \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xComTxTaskPortParameters    portspecCOM_TEST_TASK_PARAMETERS
#define xComRxTaskPortParameters    portspecCOM_TEST_TASK_PARAMETERS

#endif /* COMTEST_C */
/*---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Evt Mplx Demo Task Parameters
 *---------------------------------------------------------------------------*/

/* Constants */
#define portspecEVT_MPLX_BLOCK_TASK_STACK_SIZE      ( 512U )
#define portspecEVT_MPLX_CONTROL_TASK_STACK_SIZE    ( 512U )
#define portspecEVT_MPLX_Q_QUEUE_LENGTH             ( 2U )
#define portspecEVT_MPLX_Q_QUEUE_ITEM_SIZE          ( sizeof( portUnsignedBaseType ) )
#define portspecEVT_MPLX_Q_BUFFER_LENGTH            ( ( portspecEVT_MPLX_Q_QUEUE_LENGTH * portspecEVT_MPLX_Q_QUEUE_ITEM_SIZE ) + safertosapiQUEUE_OVERHEAD_BYTES )
#define portspecMAXIMUM_NUMBER_OF_OBJECT_EVENTS     ( 6U )
#define portspecEVT_MPLX_BUFFER_LENGTH              evtmplxGET_REQUIRED_CREATE_BUFFER_SIZE( portspecMAXIMUM_NUMBER_OF_OBJECT_EVENTS )

/* Task stacks */
extern portInt8Type acEvtMplxBlockTaskStack[ portspecEVT_MPLX_BLOCK_TASK_STACK_SIZE ];
extern portInt8Type acEvtMplxControlTaskStack[ portspecEVT_MPLX_CONTROL_TASK_STACK_SIZE ];

/* Queue buffers */
extern portInt8Type acEvtMplxQueue1Buffer[ portspecEVT_MPLX_Q_BUFFER_LENGTH ];
extern portInt8Type acEvtMplxQueue2Buffer[ portspecEVT_MPLX_Q_BUFFER_LENGTH ];

/* Semaphore buffer */
extern portInt8Type acEvtMplxSemaphoreBuffer[ safertosapiQUEUE_OVERHEAD_BYTES ];

/* Mutex buffer */
extern portInt8Type acEvtMplxMutexBuffer[ safertosapiQUEUE_OVERHEAD_BYTES ];

/* Evt Mplx buffer. */
extern portInt8Type acEvtMplxBuffer[ portspecEVT_MPLX_BUFFER_LENGTH ];

#ifdef EVT_MPLX_DEMO_C

/* Define portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION so that all static data is in the
 * same section. */
#define portspecEVT_MPLX_DEMO_ZERO_DATA_SECTION      __attribute__( ( section( "__evt_mplx_data__" ) ) )

/* Two linker-defined symbols that give the start and end address of the
 * Evt Mplx data section. */
extern portUInt32Type lnkEvtMplxDataStart;
extern portUInt32Type lnkEvtMplxDataEnd;

/* The address and size of the Evt Mplx Test data section. */
#define portspecEVT_MPLX_DATA_ADDR  ( ( void * ) &lnkEvtMplxDataStart )
#define portspecEVT_MPLX_DATA_SIZE  ( 0x40U )

/* This definition corresponds to the xMPUParameters structure that is passed
 * to xTaskCreate() as part of the xTaskParameters structure when the Evt Mplx
 * demo tasks are created. It defines the privilege level and region definitions
 * for the Evt Mplx demo tasks. All the Evt Mplx demo tasks run in
 * Unprivileged (User) mode with 1 additional region that corresponds to the
 * linker section that holds the Evt Mplx demo static data. */
#define portspecEVT_MPLX_DEMO_PARAMETERS                        \
pdFALSE,                                                        \
{                                                               \
    mpuUNPRIVILEGED_TASK,                                       \
    {                                                           \
        {                                                       \
            portspecEVT_MPLX_DATA_ADDR,                         \
            portspecEVT_MPLX_DATA_SIZE,                         \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE | \
              mpuREGION_EXECUTE_NEVER |                         \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),    \
            mpuREGION_ALL_SUB_REGIONS_ENABLED                   \
        },                                                      \
        { NULL, 0U, 0U, 0U },                                   \
        { NULL, 0U, 0U, 0U }                                    \
    }                                                           \
}

#define xEvtMplxBlockTaskPortParameters     portspecEVT_MPLX_DEMO_PARAMETERS
#define xEvtMplxControlTaskPortParameters   portspecEVT_MPLX_DEMO_PARAMETERS

#endif  /* EVT_MPLX_DEMO_C */


/*-----------------------------------------------------------------------------
 * Stream Buffer demo parameters
 *---------------------------------------------------------------------------*/

#define portspecSTREAM_BUFFER_TASK_STACK_SIZE ( 1024U )
#define portspecNUMBER_OF_STREAM_BUFFER_TASKS ( 3U )

/* Task stacks. */
extern portInt8Type acStreamBufferTaskStack[ portspecNUMBER_OF_STREAM_BUFFER_TASKS ][ portspecSTREAM_BUFFER_TASK_STACK_SIZE ];
extern portInt8Type acStreamBufferTaskWakeCheckTxStack[ portspecSTREAM_BUFFER_TASK_STACK_SIZE ];
extern portInt8Type acStreamBufferTaskWakeCheckRxStack[ portspecSTREAM_BUFFER_TASK_STACK_SIZE ];

#ifdef STREAM_BUFFER_C

/* Used to dimension the array used to hold the streams. */
#define portspecSTORAGE_SIZE_BYTES              ( 10U )
#define portspecSTREAM_BUFFER_MESSAGE_LENGTH    ( 4U )
#define portspecSTREAM_BUFFER_RX_LENGTH         ( portspecSTORAGE_SIZE_BYTES )
#define portspecSB_BLOCK_TIME_FOREVER           ( safertosapiMAX_DELAY )

extern portUInt32Type lnkStreamBufferTestDataStart;
extern portUInt32Type lnkStreamBufferTestDataEnd;

/* portspecMULTICORE_STREAM_BUFFER_DEMO Controls whether the stream buffer
 * demo is run all from the same core or across separate cores
 * portspecSTREAM_BUFFER_GET_CPU_ID must retrieve the core number if multicore is to be used*/
#define portspecMULTICORE_STREAM_BUFFER_DEMO    0

#if ( portspecMULTICORE_STREAM_BUFFER_DEMO == 1 )
    #error "multicore not supported on this platform"
#else
    #define portspecSTREAM_BUFFER_DATA_SECTION              __attribute__( ( section( "__stream_buffer_task_data__" ) ) )
    #define portspecSTREAM_BUFFER_DATA_SECTION_COMMON       portspecSTREAM_BUFFER_DATA_SECTION

    #define portspecSTREAM_BUFFER_GET_CPU_ID        ( 0U )
    #define portspecSB_BLOCK_TIME                   ( 0U )

    /* Core ID for the task to be created on */
    #define portspecSTREAM_BUFFER_CREATION_CORE     ( 0U )
    #define portspecSTREAM_BUFFER_FIRST_TASK        ( 0U )
    #define portspecSTREAM_BUFFER_SECOND_TASK       ( 0U )
    #define portspecSTREAM_BUFFER_THIRD_TASK        ( 0U )
#endif


/* These constants are used to give the stream buffer tasks access
 * to the file scope check variables. */
#define portspecSTREAM_BUFFER_DATA_ADDRESS      ( ( void *) &lnkStreamBufferTestDataStart )
#define portspecSTREAM_BUFFER_TEST_DATA_SIZE    ( ( portUInt32Type ) &lnkStreamBufferTestDataEnd - ( portUInt32Type ) &lnkStreamBufferTestDataStart )


/* MPU parameters. */
/* This definition corresponds to the xMPUParameters structure that is passed
 * to xTaskCreate() as part of the xTaskParameters structure when the stream buffer Tasks
 * are created. It defines the privilege level and region definitions for the
 * stream buffer Tasks.
 * All the stream buffer Tasks run in Unprivileged (User) mode
 */
#define portspecSTREAM_BUFFER_TASK_PARAMS                                           \
pdFALSE,                                                                            \
{                                                                                   \
    mpuUNPRIVILEGED_TASK,                                                           \
    {                                                                               \
        {                                                                           \
            portspecSTREAM_BUFFER_DATA_ADDRESS,                                     \
            portspecSTREAM_BUFFER_TEST_DATA_SIZE,                                   \
            ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE |                     \
              mpuREGION_EXECUTE_NEVER |                                             \
              mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY ),                        \
              mpuREGION_ALL_SUB_REGIONS_ENABLED                                     \
        },                                                                          \
        { NULL, 0U, 0U, 0U },                                                       \
        { NULL, 0U, 0U, 0U }                                                        \
    }                                                                               \
}

#define xStreamBufferTaskPortParameters      portspecSTREAM_BUFFER_TASK_PARAMS

#endif /* STREAM_BUFFER_C */

#endif /* PORT_SPECIFICS_H */
