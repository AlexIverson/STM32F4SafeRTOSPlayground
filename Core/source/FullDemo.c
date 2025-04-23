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

/****************************************************************************
 * This is a 'quick start' project to demonstrate use of SafeRTOS running on
 * an STM32F4 board.
 * It can be used to quickly evaluate SafeRTOS and as a reference for the
 * project settings and syntax necessary to utilise SafeRTOS.
 *
 * The following tasks and demonstrations are created:
 *
 * - A set of standard demo tasks
 * These are tasks that are common to nearly all SafeRTOS demo projects.
 * Their only purpose is to provide examples of how the SafeRTOS API can be
 * used and provide no particular functionality of interest. The Demo tasks
 * exist to demonstrate queues, semaphores, task management and interrupt
 * interactions.
 *
 * - A 'Check' task
 * The check task only executes every 5 seconds, but has the highest priority
 * so is guaranteed to get processing time. It queries all the standard demo
 * tasks to see if they are all executing as expected.
 *
 * Particular points to note within this file include how task stacks are
 * defined and aligned, how queues are dimensioned and aligned, and how the
 * kernel is initialised.
 *
 ****************************************************************************/

/* Library includes */
#include <stdlib.h>

/* SafeRTOS includes */
#include "SafeRTOS_API.h"
//#include "stm32f429xx.h"

/* Demo application include files.  These files are common to all demo
 * applications. */
#include "comtest.h"
#include "MathsTest.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "dynamic.h"
#include "Flash.h"
#include "PollQ.h"
#include "semtest.h"
#include "countsem.h"
#include "countsemTimer.h"
#include "TimerDemo.h"
#include "TaskNotify.h"
#include "TaskMutex.h"
#include "EvtMplxDemo.h"
#include "StreamBufferDemo.h"
#include "TaskCreateDeleteDemo.h"

/* Demo application include files specific to this demo. */
#include "FullDemo.h"
#include "partest.h"
#include "PortSpecifics.h"
#include "SafeRTOSHooks.h"

/*-----------------------------------------------------------------------------
 * Local constant definitions
 *---------------------------------------------------------------------------*/

/* LED indicators to be used by the Check task. */
#define fulldemoLED_CHECK_TASK                  ( 0U )

/* LED indicators to be used by the COM Test tasks.
 * Note that this is higher than the number of available LED
 * and hence this is effectively disabled. */
#define fulldemoLED_COM_TEST_BASE               ( 4U )

/* If many demo tasks are disabled, set this to zero to prevent the check
 * task from displaying an error due to low CPU time. */
#define fulldemoCHECK_IDLE_TIMES                ( 1 )

/* Base periods of the timers used in the timer and task notify demo. */
#define fulldemoTIMER_TEST_PERIOD               ( 50U )
#define fulldemoTASK_NOTIFY_PERIOD              ( 50U )

/* Check task operational definitions. */
#define fulldemoCHECK_TASK_STACK_SIZE           ( 512U )

#define fulldemoCHECK_TASK_NORMAL_CYCLE_RATE    ( ( portTickType ) 2000U / configTICK_RATE_MS )
#define fulldemoCHECK_TASK_ERROR_CYCLE_RATE     ( ( portTickType ) 1000U / configTICK_RATE_MS )

/* Check task error identifiers.
 * Used to keep track of errors that may have been detected. */
#define fulldemoCHECK_TASK_ERROR_COM_TEST               ( 0x00000001UL )
#define fulldemoCHECK_TASK_ERROR_SVC_COUNT              ( 0x00000002UL )
#define fulldemoCHECK_TASK_ERROR_MATHS_TASKS            ( 0x00000004UL )
#define fulldemoCHECK_TASK_ERROR_BLOCKING_QUEUES        ( 0x00000008UL )
#define fulldemoCHECK_TASK_ERROR_BLOCK_TIME_TEST        ( 0x00000010UL )
#define fulldemoCHECK_TASK_ERROR_COUNTING_SEMAPHORES    ( 0x00000020UL )
#define fulldemoCHECK_TASK_ERROR_CREATE_TASKS           ( 0x00000040UL )
#define fulldemoCHECK_TASK_ERROR_DYNAMIC_PRIORITIES     ( 0x00000080UL )
#define fulldemoCHECK_TASK_ERROR_POLLING_QUEUES         ( 0x00000100UL )
#define fulldemoCHECK_TASK_ERROR_BINARY_SEMAPHORES      ( 0x00000200UL )
#define fulldemoCHECK_TASK_ERROR_TIMER_TASKS            ( 0x00000400UL )
#define fulldemoCHECK_TASK_ERROR_TASK_NOTIFY            ( 0x00000800UL )
#define fulldemoCHECK_TASK_ERROR_MUTEX                  ( 0x00001000UL )
#define fulldemoCHECK_TASK_ERROR_TICK_HOOK              ( 0x00002000UL )
#define fulldemoCHECK_TASK_ERROR_IDLE_HOOK              ( 0x00004000UL )
#define fulldemoCHECK_TASK_ERROR_INSUFFICIENT_IDLE      ( 0x00008000UL )
#define fulldemoCHECK_TASK_ERROR_EXCESSIVE_IDLE         ( 0x00010000UL )
#define fulldemoCHECK_TICK_HOOK_COUNT                   ( 0x00020000UL )
#define fulldemoCHECK_TASK_ERROR_EVT_MPLX               ( 0x00040000UL )
#define fulldemoCHECK_TASK_STREAM_BUFFER                ( 0x00080000UL )


/*-----------------------------------------------------------------------------
 * Public Constants
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Public Variable Declaration
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Local Prototypes
 *---------------------------------------------------------------------------*/

static void prvCheckTask( void *pvParameters );


/*-----------------------------------------------------------------------------
 * Local Variables
 *---------------------------------------------------------------------------*/

/* Declare task TCB:
 * Due to the use of the MPU background region, by default, all RAM can only
 * be accessed in privileged mode unless a specific MPU region has been setup
 * allowing unprivileged access. */
static xTCB xCheckTaskTCB = { 0 };

/* Declare task stacks:
 * These ARE protected by MPU regions so the alignment must follow the
 * MPU alignment rules, and basically be aligned to the same power of two
 * value as their length in bytes. */
static portInt8Type acCheckTaskStack[ fulldemoCHECK_TASK_STACK_SIZE ] __attribute__ ( ( aligned ( fulldemoCHECK_TASK_STACK_SIZE ) ) ) = { 0 };


/*-----------------------------------------------------------------------------
 * External Variables
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Public Function Definitions
 *---------------------------------------------------------------------------*/

portBaseType xFullDemoCreate( void )
{
    portBaseType xCreateResult;

    /* The structure passed to xTaskCreate() to create the check task. */
    xTaskParameters xCheckTaskParams =
    {
        &prvCheckTask,                  /* The function that implements the task being created. */
        "Check",                        /* The name of the task being created. The kernel does not use this itself, its just to assist debugging. */
        &xCheckTaskTCB,                 /* The TCB for the check task. */
        acCheckTaskStack,               /* The buffer allocated for use as the task stack. */
        fulldemoCHECK_TASK_STACK_SIZE,  /* The size of the buffer allocated for use as the task stack - note this is in BYTES! */
        NULL,                           /* The task parameter, not used in this case. */
        fulldemoCHECK_TASK_PRIORITY,    /* The priority to assigned to the task being created. */
        NULL,                           /* Thread Local Storage not used. */
        pdTRUE,                         /* Check task does use the FPU. */
        {                               /* MPU task parameters. */
            mpuPRIVILEGED_TASK,         /* Check task is privileged. */
            {
                { NULL, 0U, 0U, 0U },   /* No additional region definitions are required. */
                { NULL, 0U, 0U, 0U },
                { NULL, 0U, 0U, 0U }
            }
        }
    };

    /* Create the check task. */
    xCreateResult = xTaskCreate( &xCheckTaskParams, /* The structure containing the task parameters created at the start of this function. */
                                 NULL );            /* This parameter can be used to receive a handle to the created task, but is not used in this case. */


    if( pdPASS == xCreateResult )
    {
        xCreateResult = xStartLEDFlashTasks( fulldemoFLASH_LED_TASKS_PRIORITY );
    }

    return xCreateResult;
}
/*---------------------------------------------------------------------------*/

static void prvCheckTask( void *pvParameters )
{
    portBaseType xCheckStatus;
    portTickType xLastTime;
    portTickType xCheckTaskCycleRate;
    portUInt32Type ulErrorsDetected = 0U;
    portUInt32Type ulIdleHookLastCount = 0U;
    portUInt32Type ulTickHookLastCount = 0U;


    /* The parameters are not used in this task. This just prevents a compiler
     * warning. */
    ( void ) pvParameters;

    /* Switch Check LED ON to show we good to go. */
    vParTestSetLED( fulldemoLED_CHECK_TASK, pdTRUE );

    /* No errors have been detected yet. */
    xCheckTaskCycleRate = fulldemoCHECK_TASK_NORMAL_CYCLE_RATE;

    /* Initialise the variable used in calls to xTaskDelayUntil() to control
     * the tasks execution period to the current time. */
    xLastTime = xTaskGetTickCount();

    /* Forever loop... */
    for( ;; )
    {
        /* Delay until the next check time. */
        ( void ) xTaskDelayUntil( &xLastTime, xCheckTaskCycleRate );

        /**********************************************************************
         * Check Demo Test operations.
         **********************************************************************
         * Ask each set of demo tasks to report their status.
         **********************************************************************
         * This set of demo tasks reported an error.
         * Send an error message to the display task and latch that an error
         * has occurred during this loop.
         * Note that only a pointer to the error message is actually queued so
         * this assumes the string is in const, non-stack memory.
         * This assumption is fine for a simple demo, but not for a high
         * integrity application!
         *********************************************************************/

        

        /**********************************************************************
         * Check SafeRTOS operating parameters.
         *********************************************************************/

        /* Check that the Tick Hook is being called. */
        if( ulTickHookCallCount == ulTickHookLastCount )
        {
            ulErrorsDetected |= fulldemoCHECK_TASK_ERROR_TICK_HOOK;
        }
        ulTickHookLastCount = ulTickHookCallCount;

        /* Finally, check that the idle task is getting called. */
        if( ulIdleHookCallCount == ulIdleHookLastCount )
        {
            ulErrorsDetected |= fulldemoCHECK_TASK_ERROR_IDLE_HOOK;
        }
        ulIdleHookLastCount = ulIdleHookCallCount;

        /* Check that the tick hook has been called at every tick. */
        if( xTaskGetTickCount() != ulTickHookCallCount )
        {
            ulErrorsDetected |= fulldemoCHECK_TICK_HOOK_COUNT;
        }

        /* If an error has been detected... toggle check LED faster
         * to give a visible indication of the error detection. */
        if( 0UL != ulErrorsDetected )
        {
            xCheckTaskCycleRate = fulldemoCHECK_TASK_ERROR_CYCLE_RATE;
        }
        /* Toggle the check task LED. */
        vParTestToggleLED( fulldemoLED_CHECK_TASK );
    }
}
/*---------------------------------------------------------------------------*/
