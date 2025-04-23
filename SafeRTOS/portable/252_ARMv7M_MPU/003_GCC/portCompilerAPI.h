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

#ifndef PORT_COMPILER_API_H
#define PORT_COMPILER_API_H

/*-----------------------------------------------------------------------------
 * Portable layer version number
 *---------------------------------------------------------------------------*/

/* Portable compiler layer major version number. */
#define portcompapiPORT_COMPILER_MAJOR_VERSION  ( 1 )

/* Portable compiler layer minor version number. */
#define portcompapiPORT_COMPILER_MINOR_VERSION  ( 0 )

/* Portable compiler layer release candidate version number. */
#define portcompapiPORT_COMPILER_RC_VERSION     ( 0 )


#ifndef __ASSEMBLER__

    #ifdef __cplusplus
    extern "C" {
    #endif

    /*-----------------------------------------------------------------------------
     * Scheduler utilities
     *---------------------------------------------------------------------------*/

    /* Raise an SVC exception to perform a context switch. */
    #define portcompapiYIELD()                          \
        __asm volatile                                  \
        (                                               \
            "   svc     %[SvcNum]"                      \
            ::  [SvcNum] "i" ( portapiSVC_YIELD )       \
            :   "memory"                                \
        )

    /* Raise an SVC exception to disable interrupts and increment ulCriticalNesting. */
    #define portcompapiENTER_CRITICAL()                 \
        __asm volatile                                  \
        (                                               \
            "   svc     %[SvcNum]"                      \
            ::  [SvcNum] "i" ( portapiSVC_ENTER_CRIT )  \
            :   "lr", "memory"                          \
        )

    /* Raise an SVC exception to decrement ulCriticalNesting and, if it reached 0,
     * enable interrupts. */
    #define portcompapiEXIT_CRITICAL()                  \
        __asm volatile                                  \
        (                                               \
            "   svc     %[SvcNum]"                      \
            ::  [SvcNum] "i" ( portapiSVC_EXIT_CRIT )   \
            :   "lr", "memory"                          \
        )


    /* Set BASEPRI to configSYSTEM_INTERRUPT_PRIORITY, from an ISR,
     * and return the previous priority. */
    KERNEL_FUNCTION portUnsignedBaseType uxPortSetInterruptMaskFromISR( void );

    #define portcompapiSET_INTERRUPT_MASK_FROM_ISR  uxPortSetInterruptMaskFromISR

    /* Set BASEPRI back to its original value, from an ISR. */
    #define portcompapiCLEAR_INTERRUPT_MASK_FROM_ISR( uxOriginalPriority )              \
    do                                                                                  \
    {                                                                                   \
        /* Prevent MISRA warning. */                                                    \
        ( void )( uxOriginalPriority );                                                 \
        __asm volatile                                                                  \
        (                                                                               \
            "   msr     basepri, %[Priority]"                                           \
            ::  [Priority] "r" ( uxOriginalPriority )                                   \
            :   "memory"                                                                \
        );                                                                              \
        /* MCDC Test Point: STD_IN_MACRO "portcompapiCLEAR_INTERRUPT_MASK_FROM_ISR" */  \
    } while( 0 )


    /*-----------------------------------------------------------------------------
     * Common inline assembly instructions
     *---------------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------------
     * void portcompilerapiDSB( void )
     * Data Synchronization Barrier
     * It completes when all explicit memory accesses before this instruction complete.
     *----------------------------------------------------------------------------*/
    #define portcompapiDSB()    __asm volatile ( "dsb" ::: "memory" )

    /*-----------------------------------------------------------------------------
     * void portcompilerapiISB( void )
     * Instruction Synchronization Barrier
     * Instruction Synchronization Barrier flushes the pipeline in the processor,
     * so that all instructions following the ISB are fetched from cache or memory,
     * after the instruction has been completed.
     *----------------------------------------------------------------------------*/
    #define portcompapiISB()    __asm volatile ( "isb" ::: "memory" )

    /*---------------------------------------------------------------------------*/

    #ifdef __cplusplus
    }
    #endif

#endif /* __ASSEMBLER__ */

#endif /* PORT_COMPILER_API_H */
