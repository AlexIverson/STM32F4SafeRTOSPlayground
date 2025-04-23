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

#ifndef FULL_DEMO_H
#define FULL_DEMO_H


/* Task priorities:
 * The lowest priority is always zero (0).
 * The highest priority is (configMAX_PRIORITIES - 1).
 * Any number of tasks can share the same priority.
 * Note, the timer task priority is set in FullDemo.h. */
#define fulldemoCHECK_TASK_PRIORITY             ( 4U )
#define fulldemoFLASH_LED_TASKS_PRIORITY        ( 3U )
#define fulldemoCOM_TEST_TASK_PRIORITY          ( 2U )
#define fulldemoBLOCKING_QUEUE_TASKS_PRIORITY   ( 1U )
#define fulldemoPOLLED_QUEUE_TASKS_PRIORITY     ( 0U )
#define fulldemoSEMAPHORE_TASKS_PRIORITY        ( 1U )
#define fulldemoTASK_CREATE_DELETE_PRIORITY     ( 1U )
#define fulldemoSTREAM_BUFFER_TASKS_PRIORITY    ( 2U )


/* The following constants define which components of the demonstration
 * application are included in the build. In each case, set to '1' to include or
 * '0' to exclude. */
#define fulldemoINCLUDE_FLASH_DEMO              ( 1 )
#define fulldemoINCLUDE_MATHS_DEMO              ( 0 ) /* The maths task demo is intended for processors that implement a floating point unit (FPU). */
#define fulldemoINCLUDE_POLL_Q_DEMO             ( 0 )
#define fulldemoINCLUDE_BLOCK_Q_DEMO            ( 0 )
#define fulldemoINCLUDE_DYNAMIC_DEMO            ( 0 )
#define fulldemoINCLUDE_BLOCK_TIME_DEMO         ( 0 ) /* The block time demo is very processor intensive and can be disabled in demo applications that use processors with lower clock speeds. */
#define fulldemoINCLUDE_SEMA_COUNT_DEMO         ( 0 )
#define fulldemoINCLUDE_SEMA_BINARY_DEMO        ( 0 )
#define fulldemoINCLUDE_TIMER_DEMO              ( 0 )
#define fulldemoINCLUDE_TASK_CREATE_DELETE_DEMO ( 0 )
#define fulldemoINCLUDE_TASK_NOTIFY_DEMO        ( 0 )
#define fulldemoINCLUDE_REC_MUTEX_DEMO          ( 0 )
#define fulldemoINCLUDE_SVC_TEST_DEMO           ( 0 )
#define fulldemoINCLUDE_EVT_MPLX_DEMO           ( 0 )
#define fulldemoINCLUDE_STREAM_BUFFER_DEMO      ( 0 )

/* The com test demo requires an external loopback. */
#define fulldemoINCLUDE_COM_TEST_DEMO           ( 0 )


/*-----------------------------------------------------------------------------
 * Public Prototypes
 *---------------------------------------------------------------------------*/

/* Prototype for the function that creates the demo application tasks. */
portBaseType xFullDemoCreate( void );
/*---------------------------------------------------------------------------*/

#endif /* FULL_DEMO_H */
