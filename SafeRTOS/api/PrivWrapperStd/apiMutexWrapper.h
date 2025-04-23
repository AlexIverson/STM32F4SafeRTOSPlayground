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

#ifndef API_MUTEX_WRAPPER_H
#define API_MUTEX_WRAPPER_H

/* Scheduler Includes */
#include "SafeRTOS_API.h"
#include "mutexAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


API_FUNCTION portBaseType xMutexGetState( xMutexHandleType xMutex, portUnsignedBaseType *puxMutexState );

API_FUNCTION portBaseType xMutexGive( xMutexHandleType xMutex );

API_FUNCTION portBaseType xMutexTake( xMutexHandleType xMutex, portTickType xTicksToWait );

API_CREATE_FUNCTION portBaseType xMutexCreate( portInt8Type *pcMutexBuffer,
                                               xMutexHandleType *pxMutex );


#ifdef __cplusplus
}
#endif

#endif /* API_MUTEX_WRAPPER_H */
