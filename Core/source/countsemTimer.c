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

/*-----------------------------------------------------------------------------
 * Counting Semaphore Timer Initialisation and Configuration Function.
 *---------------------------------------------------------------------------*/

/* Scheduler include files. */
#include "SafeRTOS_API.h"

/* Hardware include files. */
#include "stm32f4xx_hal.h"

/* Demo Program include files. */
#include "PortSpecifics.h"
#include "countsem.h"
#include "countsemTimer.h"


/*-----------------------------------------------------------------------------
 * Local constant definitions
 *---------------------------------------------------------------------------*/

/* Semaphore ISR Test Task Operational State & Counter */
#define countsemTIMER_CLOCK_HZ      ( configCPU_CLOCK_HZ )

#define countsemTIMER_PRESCALER    ( 1800UL )

#define countsemIRQ_PRIORITY        ( 13U )


/*-----------------------------------------------------------------------------
 * Local variable definitions
 *---------------------------------------------------------------------------*/

static TIM_HandleTypeDef xHALTimer = { 0 };


/*-----------------------------------------------------------------------------
 * Routines
 *---------------------------------------------------------------------------*/

/* Counting Semaphore ISR Timer initialisation. */
void vCountSemTimerInit( void )
{
    portUInt32Type ulInitFrequency;

    /* Enable Timer peripheral clock. */
    __HAL_RCC_TIM3_CLK_ENABLE();

    ulInitFrequency  = countsemTIMER_CLOCK_HZ / 2U;
    ulInitFrequency /= countsemTIMER_PRESCALER;
    ulInitFrequency /= countsemCOUNTSEM_ISR_TIMER_FREQUENCY;
    ulInitFrequency &= 0xFFFFUL;
    ulInitFrequency -= 1;

    xHALTimer.Instance = TIM3;
    xHALTimer.Init.Prescaler = countsemTIMER_PRESCALER;
    xHALTimer.Init.CounterMode = 0;
    xHALTimer.Init.Period = ( portUInt16Type ) ulInitFrequency;
    xHALTimer.Init.ClockDivision = 0;
    xHALTimer.Init.RepetitionCounter = 0;

    ( void ) HAL_TIM_Base_Init( &xHALTimer );

    /* Set Timer interrupt priority above kernel - highest */
    NVIC_SetPriority( TIM3_IRQn, countsemIRQ_PRIORITY );

    /* Enable Timer interrupt in peripheral. */
    NVIC_EnableIRQ( TIM3_IRQn );

    /* Start Timer... */
    ( void ) HAL_TIM_Base_Start_IT( &xHALTimer );
}
/*---------------------------------------------------------------------------*/

/* Timer Counter IRQ Handler used for SEMA-ISR Operations */
void TIM3_IRQHandler( void )
{
    /* Clear Semaphore Timer Counter Interrupt Source */
    __HAL_TIM_CLEAR_IT( &xHALTimer, TIM_IT_UPDATE );

    /* Call the counting semaphore demo timer ISR. */
    vCountingSemaphoreTimerHandler();

    /* Context switch if needed. */
    safertosapiYIELD_FROM_ISR();
}
/*---------------------------------------------------------------------------*/
