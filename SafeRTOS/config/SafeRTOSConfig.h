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

#ifndef SAFERTOS_CONFIG_H
#define SAFERTOS_CONFIG_H

//#include "system_stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define configINCLUDE_FPU_SUPPORT  ( 1 )
#define configMAX_SB_LOCK_ATTEMPTS ( 10 )

/* Macros to determine linker script placement of SafeRTOS code and data */
#define configKERNEL_FUNC_DEF           __attribute__ ( ( section ( "kernel_functions" ) ) )
#define configKERNEL_UNPRIV_FUNC_DEF    __attribute__ ( ( section ( "kernel_unpriv_functions" ) ) )
#define configKERNEL_DATA_DEF           __attribute__ ( ( nocommon ) ) __attribute__ ( ( section ( "kernel_data" ) ) )

/* The type used by the SafeRTOS EvntMplx feature */
#ifndef configMESSAGE_BUFFER_LENGTH_TYPE
    #ifndef __ASSEMBLER__
        typedef unsigned long MessageBufferType;
        #define configMESSAGE_BUFFER_LENGTH_TYPE MessageBufferType
    #endif
#endif
/*--------------------------------------------------------------------------*/

/* This macro defines the maximum number of task priority levels. Each task
 * must have a priority in the range of 0 to (configMAX_PRIORITIES - 1).
 */
#define configMAX_PRIORITIES                ( 6U )

/* The minimum permitted size for the stack of a task that uses the FPU.
 * This constant assumes that ulAdditionalStackCheckMarginBytes was set to 0.
 */
#define configMINIMAL_STACK_SIZE_WITH_FPU   ( 512U )    /* Needs to be a power of two value. */

/* The minimum permitted size for the stack of a task that does not use the FPU.
 * This constant assumes that ulAdditionalStackCheckMarginBytes was set to
 * 0. */
#define configMINIMAL_STACK_SIZE_NO_FPU     ( 256U )    /* Needs to be a power of two value. */

/* This macro defines the frequency of the SafeRTOS scheduler tick.*/
#define configTICK_RATE_HZ                  ( ( portTickType ) 1000U )

/* This macro calculates the number of ticks per millisecond.
 * NOTE: This integer calculation is only correct for values of
 * configTICK_RATE_HZ less than or equal to 1000 that are also divisors
 * of 1000.
 */
#define configTICK_RATE_MS                  ( ( portTickType ) 1000U / configTICK_RATE_HZ )

#define configCPU_CLOCK_HZ                  ( 180000000UL ) /* SystemCoreClock */

/* This defines the priority of the kernel tick interrupt handler.
 * It is set to the lowest possible priority - priority 15, or ( 15 << 4 ) as
 * only four priority bits are implemented.
 */
#define configKERNEL_INTERRUPT_PRIORITY     ( 255U )

/* This defines the maximum interrupt priority level that is permitted to use
 * SafeRTOS API calls. Note that on the Cortex-M4 higher numbers mean low
 * priority therefore only interrupt routines with a priority equal to or
 * greater than configSYSTEM_INTERRUPT_PRIORITY may utilise API calls.
 * Here it is defined as priority 10, or ( 10 << 4 ) as only four priority bits
 * are implemented.
 */
#define configSYSTEM_INTERRUPT_PRIORITY     ( 175U )

/* This defines the priority of the SVC call handler - it must be one higher
 * than configSYSTEM_INTERRUPT_PRIORITY. Here it is defined as priority 4, or
 * ( 9 << 4 ) as only four priority bits are implemented. */
#define configSYSCALL_INTERRUPT_PRIORITY    ( 159U )

/* Size of the Queue registry for use with the StateViewer plugin.
 * Set it to 0 to disable.
 */
#define configQUEUE_REGISTRY_SIZE           ( 0U )

/* Map the interrupt handlers to their CMSIS names so that the handlers will be
 * called when the interrupt occurs. Alternatively the startup file should be
 * modified to call the correct handler directly.
 */
#define vSafeRTOSPendSVHandler              PendSV_Handler
#define vSafeRTOSSVCHandler                 SVC_Handler

/* vTaskProcessSystemTickFromISR should be mapped to SysTick_Handler when the
 * SysTick handler will be used to generate the tick interrupt for the RTOS.
 * When the SysTick interrupt is not being used to generate the tick, then the
 * actual tick interrupt handler should call vTaskProcessSystemTickFromISR().
 * Likewise, the setting of pxSetupTickInterruptFunction in the scheduler
 * initialisation function (SafeRTOSConfig.c) should be set to NULL when the
 * SysTick is to be used to generate the RTOS tick interrupt.
 * If an alternative interrupt is to be used then a setup function for the tick
 * interrupt should be provided in the pxSetupTickInterruptFunction field of
 * xPortInit.
 */
#define vTaskProcessSystemTickFromISR       SysTick_Handler


/* Define Sub-region settings for the kernel regions. */
#define configMPU_UNPRIV_FLASH_REGION_SUBMASK   ( mpuREGION_ALL_SUB_REGIONS_ENABLED )
#define configMPU_KERNEL_FLASH_REGION_SUBMASK   ( mpuREGION_ALL_SUB_REGIONS_ENABLED )
#define configMPU_KERNEL_DATA_REGION_SUBMASK    ( mpuREGION_ALL_SUB_REGIONS_ENABLED )


/* Macros used for run-time statistics. This is the interface with a
 * counter/timer, usually the SysTick timer, that has an accessible counter
 * that increments or decrements by a reasonable number of counts per tick
 * interrupt. Two macros are required, as follows:
 *
 * configRTS_COUNTER_VALUE:
 * A macro expected to return an ASCENDING value.
 *
 * configRTS_COUNTER_MAX:
 * A macro that is either a constant or an expression involving a dereferenced
 * register pointer (e.g. a reload register or match register), that gives the
 * maximum numeric value that configRTS_COUNTER_VALUE can reach.
 */
#define configRTS_COUNTER_MAX_REG   ( ( portUInt32Type * ) 0xE000E014U )
#define configRTS_COUNTER_VAL_REG   ( ( volatile portUInt32Type * ) 0xE000E018U )

#define configRTS_COUNTER_MAX       ( *configRTS_COUNTER_MAX_REG )
#define configRTS_COUNTER_VALUE     ( *configRTS_COUNTER_MAX_REG - *configRTS_COUNTER_VAL_REG )

/* Note this must reflect the number of the MPU regions supported by the processor. */
#define configNUM_MPU_REGIONS                   ( 8U )

/* Errata workarounds. */
#define configARM_ERRATA_837070                 ( 0 )   /* Only applicable to Cortex M7 r0p1. */
#define configARM_ERRATA_838869                 ( 1 )   /* Only applicable to Cortex M3 and M4. */



#define configTIMER_TASK_PRIORITY               ( 2U )
#define configTIMER_CMD_QUEUE_LEN               ( 10U )


/* Timer Task Parameters */
#ifdef __cplusplus
}
#endif

#endif /* SAFERTOS_CONFIG_H */
