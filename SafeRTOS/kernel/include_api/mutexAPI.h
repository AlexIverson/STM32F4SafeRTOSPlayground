/*
    Copyright (C)2006 onward WITTENSTEIN high integrity systems Limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorise use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems Limited, Registered Office: Brown's Court, Long Ashton
    Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 1275 395 600
    Email: info@highintegritysystems.com
    www.highintegritysystems.com
*/

#ifndef RECURSIVE_MUTEX_API_H
#define RECURSIVE_MUTEX_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * MACROS AND DEFINITIONS
 *---------------------------------------------------------------------------*/

typedef void *xMutexHandleType;

/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

/* Macros to assist in turning the result from xQueueMessagesWaitingKrnl() into
 * something meaningful to mutexes. */
#define mutexTAKEN                                              ( 0 )   /* the underlying queue is empty */
#define mutexAVAILABLE                                          ( 1 )   /* the underlying queue is full */

KERNEL_CREATE_FUNCTION portBaseType xMutexCreateKrnl( portInt8Type *pcMutexBuffer, xMutexHandleType *pxMutex );
KERNEL_FUNCTION portBaseType xMutexTakeKrnl( xMutexHandleType xMutex, portTickType xTicksToWait );
KERNEL_FUNCTION portBaseType xMutexGiveKrnl( xMutexHandleType xMutex );
KERNEL_FUNCTION portBaseType xMutexGetStateKrnl( xMutexHandleType xMutex, portUnsignedBaseType *puxMutexState );



#ifdef __cplusplus
}
#endif

#endif /* RECURSIVE_MUTEX_API_H */
