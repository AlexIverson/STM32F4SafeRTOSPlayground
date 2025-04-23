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

/* Library includes */
#include <stdlib.h>

/* SafeRTOS includes */
#include "SafeRTOS_API.h"

/* Program Includes */
#include "PortSpecifics.h"
#include "FullDemo.h"
#include "SafeRTOSHooks.h"


/*-----------------------------------------------------------------------------
 * Local Constants
 *---------------------------------------------------------------------------*/

/* Scheduler Initialisation Definitions */
/* They indicate the location of both vector table and system stack
 * The system stack location is the first entry in the vector table
 * on Cortex-M4 devices) as well as size of the system stack in bytes. */

#define configSYSTEM_STACK_SIZE             ( ( portUnsignedBaseType ) _Min_Stack_Size )
#define configSTACK_CHECK_MARGIN            ( 0U )

/* The user configuration for the idle task. */
#define configIDLE_TASK_STACK_SIZE          ( configMINIMAL_STACK_SIZE_NO_FPU )

/* The user configuration for the timer module. */
#define configTIMER_TASK_STACK_SIZE         ( 512U )


#define configTIMER_CMD_QUEUE_BUFFER_SIZE   ( ( configTIMER_CMD_QUEUE_LEN * sizeof( timerQueueMessageType ) ) + safertosapiQUEUE_OVERHEAD_BYTES )

/* The size of the Idle task data section. */
#define configIDLE_TASK_DATA_SIZE           ( 0x20U )

/* The address and size of the Idle Hook data section. */
#define configIDLE_HOOK_DATA_ADDR           ( ( void * ) &lnkIdleHookDataStart )
#define configIDLE_HOOK_DATA_SIZE           ( ( portUInt32Type ) &lnkIdleHookDataEnd -  ( portUInt32Type ) &lnkIdleHookDataStart )

/* MPU General Peripherals Hardware address range */
#define configPERIPHERALS_START_ADDRESS     ( 0x40000000U )
#define configPERIPHERALS_END_ADDRESS       ( 0x60000000U )

/*-----------------------------------------------------------------------------
 * Local Prototypes
 *---------------------------------------------------------------------------*/

/* The call to xTaskInitializeScheduler is included within a wrapper
 * initialisation function. */
portBaseType xInitializeScheduler( void );

/*-----------------------------------------------------------------------------
 * Local Variables
 *---------------------------------------------------------------------------*/

static portInt8Type acIdleTaskStack[ configIDLE_TASK_STACK_SIZE ] __attribute__( ( aligned ( configIDLE_TASK_STACK_SIZE ) ) ) = { 0 };

/* Declare the stack for the timer task, it cannot be done in the timer
 * module as the syntax for alignment is port specific. Also the callback
 * functions are executed in the timer task and their complexity/stack
 * requirements are application specific. */
static portInt8Type acTimerTaskStack[ configTIMER_TASK_STACK_SIZE ] __attribute__( ( aligned ( configTIMER_TASK_STACK_SIZE ) ) ) = { 0 };

/* The buffer for the timer command queue. */
static portInt8Type acTimerCommandQueueBuffer[ configTIMER_CMD_QUEUE_BUFFER_SIZE ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

/* Two linker-defined symbols that give the start address and the size of the
 * Idle Hook data section. */
extern portUInt32Type lnkIdleHookDataStart;
extern portUInt32Type lnkIdleHookDataEnd;

extern portUInt32Type _Min_Stack_Size[];

/*---------------------------------------------------------------------------*/

portBaseType xInitializeScheduler( void )
{
    portBaseType xInitSchedResult;
    /* The structure passed to xTaskInitializeScheduler() to configure the kernel
     * with the application defined constants and call back functions. */
    const xPORT_INIT_PARAMETERS xPortInit =
    {
        configCPU_CLOCK_HZ,                 /* ulCPUClockHz */
        configTICK_RATE_HZ,                 /* ulTickRateHz */

        /* Hook functions */
        &vApplicationSVCHook,               /* pxSVCHookFunction */

        /* System Stack Parameters */
        configSYSTEM_STACK_SIZE,            /* uxSystemStackSizeBytes */
        configSTACK_CHECK_MARGIN,           /* uxAdditionalStackCheckMarginBytes */

        /* Idle Task Parameters */
        acIdleTaskStack,                    /* pcIdleTaskStackBuffer */
        configIDLE_TASK_STACK_SIZE,         /* uxIdleTaskStackSizeBytes */
        pdFALSE,                            /* The idle hook will not use the FPU. */

        /* MPU Operating parameters */
        {                                   /* xIdleTaskMPUParameters */
            mpuUNPRIVILEGED_TASK,           /* The idle hook will be executed in unprivileged mode. */
            {
                {
                    configIDLE_HOOK_DATA_ADDR,
                    configIDLE_HOOK_DATA_SIZE,
                    ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE |
                      mpuREGION_EXECUTE_NEVER |
                      mpuREGION_OUTER_AND_INNER_WRITE_BACK_WRITE_AND_READ_ALLOCATE ),
                      mpuREGION_ALL_SUB_REGIONS_ENABLED
                },
                { NULL, 0U, 0U, 0U },
                { NULL, 0U, 0U, 0U }
            }
        },
        NULL,                               /* No Thread Local Storage for the Idle Task. */

        /* Timer feature initialisation. */
        configTIMER_TASK_PRIORITY,          /* uxTimerTaskPriority */
        configTIMER_TASK_STACK_SIZE,        /* uxTimerTaskStackSize */
        acTimerTaskStack,                   /* pcTimerTaskStackBuffer */
        configTIMER_CMD_QUEUE_LEN,          /* uxTimerCommandQueueLength */
        configTIMER_CMD_QUEUE_BUFFER_SIZE,  /* uxTimerCommandQueueBufferSize */
        acTimerCommandQueueBuffer,          /* pcTimerCommandQueueBuffer */
    };

    /* Initialise the kernel by passing in a pointer to the
     * xPortInit structure and return the resulting error code. */
    xInitSchedResult = xTaskInitializeScheduler( &xPortInit );

    if( pdPASS == xInitSchedResult )
    {
        /* Configure a global region mapped onto the peripherals */
        xInitSchedResult = xMPUConfigureGlobalRegion( mpuGLOBAL_CONFIGURABLE_REGION_FIRST,
                                                      configPERIPHERALS_START_ADDRESS,
                                                      ( mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE |
                                                        mpuREGION_EXECUTE_NEVER ),
                                                      ( configPERIPHERALS_END_ADDRESS - configPERIPHERALS_START_ADDRESS ),
                                                      mpuREGION_ALL_SUB_REGIONS_ENABLED );
    }

    return xInitSchedResult;
}
/*---------------------------------------------------------------------------*/
