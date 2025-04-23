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

#ifndef PORT_API_H
#define PORT_API_H

/*-----------------------------------------------------------------------------
 * SVC Numbers (they need to be defined before the #include's)
 *---------------------------------------------------------------------------*/

#define portapiSVC_YIELD            ( 0 )
#define portapiSVC_ENTER_CRIT       ( 1 )
#define portapiSVC_EXIT_CRIT        ( 2 )
#define portapiSVC_RAISE_PRIV       ( 3 )
#define portapiSVC_LOWER_PRIV       ( 4 )
#define portapiSVC_START_FIRST      ( 5 )


/*-----------------------------------------------------------------------------
 * Include files
 *---------------------------------------------------------------------------*/

#include "portCompilerAPI.h"

#ifndef __ASSEMBLER__

    #ifdef __cplusplus
    extern "C" {
    #endif

    /*-----------------------------------------------------------------------------
     * Utilities required by the common task code
     *---------------------------------------------------------------------------*/

    /* Macro that is conventionally used within an ISR to perform a context
     * switch if required. */
    #define portapiSCB_INT_CTRL     ( ( volatile portUnsignedBaseType * ) 0xE000ED04U )
    #define portapiSCB_PENDSVSET    ( 0x10000000U )

    /* Request a yield from an ISR by setting the PendSV flag. */
    #define portapiREQUEST_YIELD()                                  \
    do                                                              \
    {                                                               \
        *portapiSCB_INT_CTRL = portapiSCB_PENDSVSET;                \
        portcompapiDSB();                                           \
        portcompapiISB();                                           \
        /* MCDC Test Point: STD_IN_MACRO "portapiREQUEST_YIELD" */  \
    } while( 0 )
    /*---------------------------------------------------------------------------*/

    /* If the interrupt has caused a higher priority task to unblock than the current task, request
     * a context switch. (This will happen in a PendsSV after this interrupt has exited) */
    #define portapiYIELD_FROM_ISR()                                         \
    do                                                                      \
    {                                                                       \
        if( pdFALSE != xHigherPriorityTaskWoken )                           \
        {                                                                   \
            xHigherPriorityTaskWoken = pdFALSE;                             \
            portapiREQUEST_YIELD();                                         \
            /* MCDC Test Point: STD_IF_IN_MACRO "portapiYIELD_FROM_ISR" */  \
        }                                                                   \
        /* MCDC Test Point: ADD_ELSE_IN_MACRO "portapiYIELD_FROM_ISR" */    \
    } while( 0 )
    /*---------------------------------------------------------------------------*/

    #ifdef __cplusplus
    }
    #endif

#endif /* __ASSEMBLER__ */

#endif /* PORT_API_H */
