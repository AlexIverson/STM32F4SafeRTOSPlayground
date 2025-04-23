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

#include "SafeRTOS_API.h"
#include "stm32f4xx_hal.h"
#include "FullDemo.h"
#include "TimerDemo.h"
#include "TaskNotify.h"
#include "TaskCreateDeleteDemo.h"
#include "partest.h"
#include "SafeRTOSHooks.h"

/*-----------------------------------------------------------------------------
 * Constants and Macros
 *---------------------------------------------------------------------------*/

/* Prefix macro for the Idle Hook data objects. */
#define hooksIDLE_HOOK_INIT_DATA_SECTION    __attribute__( ( section( "__idle_hook_init_data__" ) ) )
#define hooksIDLE_HOOK_ZERO_DATA_SECTION    __attribute__( ( section( "__idle_hook_zero_data__" ) ) )


/*-----------------------------------------------------------------------------
 * Filescope function prototypes
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Global variables
 *---------------------------------------------------------------------------*/

/* A counter to test the SVC hook. */
volatile portUnsignedBaseType uxSvcCounter = 0U;

/* Incremented in the idle task hook so the check task
 * can ensure the idle task hook function is being called as expected. */
hooksIDLE_HOOK_ZERO_DATA_SECTION volatile portUInt32Type ulIdleHookCallCount = 0U;

/* Incremented within the tick hook so the check task can ensure the tick hook
 * is being called. */
volatile portUInt32Type ulTickHookCallCount = 0U;

/* The check task needs to know the idle task's handle
 * so it can query its runtime statistics. */
hooksIDLE_HOOK_ZERO_DATA_SECTION portTaskHandleType xIdleTaskHandle = NULL;


/*-----------------------------------------------------------------------------
 * Filescope variables
 *---------------------------------------------------------------------------*/

/* Flag to track first execution of the idle hook function. */
hooksIDLE_HOOK_INIT_DATA_SECTION static portBaseType xIdleTaskFirstExecution = pdTRUE;

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
    uint32_t ulTickVal;

    if( xTaskIsSchedulerStarted() == pdTRUE )
    {
        ulTickVal = xTaskGetTickCount() * configTICK_RATE_MS;
    }
    else
    {
        ulTickVal = uwTick;
    }

    return ulTickVal;
}
/*--------------------------------------------------------------------------*/

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay  specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay( uint32_t Delay )
{
    ( void ) xTaskDelay( Delay / configTICK_RATE_MS );
}
/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * SafeRTOS Hook functions
 *---------------------------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* First time that the Idle task hook has executed? */
    if( pdFALSE != xIdleTaskFirstExecution )
    {
        /* Yes, clear the flag. */
        xIdleTaskFirstExecution = pdFALSE;

        /* Now store a copy of our task handle so that the check task
         * can examine the runtime statistics of the idle task. */
        xIdleTaskHandle = xTaskGetCurrentTaskHandle();
    }

    /* Increment a counter so the check task can see that the idle task is
     * still running. */
    ulIdleHookCallCount++;
}
/*---------------------------------------------------------------------------*/

void vApplicationTickHook( void )
{
#if ( fulldemoINCLUDE_TIMER_DEMO == 1 )
    /* Call the periodic timer test, which tests the timer API functions that
     * can be called from an ISR. */
    vTimerPeriodicISRTests();
#endif

#if ( fulldemoINCLUDE_TASK_NOTIFY_DEMO == 1 )
    /* Call the periodic notify test, which tests the task notify API functions
     * that can be called from an ISR. */
    vNotifyTaskFromISR();
#endif

    /* Increment a counter so the check task can see that the tick hook is
     * being called. */
    ulTickHookCallCount++;

    /* Context switch if needed. */
    safertosapiYIELD_FROM_ISR();
}
/*---------------------------------------------------------------------------*/

void vApplicationTaskDeleteHook( portTaskHandleType xTaskBeingDeleted )
{
    if( ( &xDeleterTaskTCB == xTaskBeingDeleted ) || ( &xDummyTaskTCB == xTaskBeingDeleted ) )
    {
        /* Increment a counter each time a task is deleted.
         * The task create/delete tasks use this counter to ensure the expected number
         * of tasks have been deleted. */
        uxCreateDeleteDemoDeleteCount++;
    }
}
/*---------------------------------------------------------------------------*/

void vApplicationSVCHook( portUnsignedBaseType uxSvcNumber )
{
    switch( uxSvcNumber )
    {
        case hooksSVC_INCREMENT_VALUE:
            uxSvcCounter++;
            break;
        default:
            break;
    }
}
/*---------------------------------------------------------------------------*/

void vApplicationErrorHook( portTaskHandleType xHandleOfTaskWithError,
                            portBaseType xErrorCode )
{
    /* The parameters are not used, these lines prevent compiler warnings. */
    ( void ) xHandleOfTaskWithError;
    ( void ) xErrorCode;

    /* Will only get here if an internal kernel error occurs. */
    vErrorHandler();
}
/*---------------------------------------------------------------------------*/

void vErrorHandler( void )
{
    /* Switch All LED Indicators ON! */
    vParTestSetLED( 0, pdTRUE );
    vParTestSetLED( 1, pdTRUE );
    vParTestSetLED( 2, pdTRUE );

    /* Will only get here if an internal kernel error occurs. */
    for( ;; );
}
/*---------------------------------------------------------------------------*/
