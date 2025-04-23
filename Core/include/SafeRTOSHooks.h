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

#ifndef SAFERTOS_HOOKS_H
#define SAFERTOS_HOOKS_H

#include "SafeRTOS_API.h"

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
 * Constants and Macros
 *---------------------------------------------------------------------------*/

/* SVC numbers for demo. */
#define hooksSVC_INCREMENT_VALUE    ( 10U )


/*-----------------------------------------------------------------------------
 * Function prototypes
 *---------------------------------------------------------------------------*/

void vApplicationSVCHook( portUnsignedBaseType uxSvcNumber );

void vErrorHandler( void );


/*-----------------------------------------------------------------------------
 * Variables
 *---------------------------------------------------------------------------*/

extern volatile portUnsignedBaseType uxSvcCounter;
extern volatile portUInt32Type ulIdleHookCallCount;
extern volatile portUInt32Type ulTickHookCallCount;

extern portTaskHandleType xIdleTaskHandle;


/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* SAFERTOS_HOOKS_H */
