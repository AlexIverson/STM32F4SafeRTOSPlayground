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


#ifndef SEMAPHORE_API_H
#define SEMAPHORE_API_H

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
 * MACROS AND DEFINITIONS
 *---------------------------------------------------------------------------*/

typedef void *xSemaphoreHandle;

/* do not block when giving semaphores */
#define semSEM_GIVE_BLOCK_TIME              ( ( portTickType ) 0 )

/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

KERNEL_CREATE_FUNCTION portBaseType xSemaphoreCreateBinaryKrnl( portInt8Type *pcSemaphoreBuffer, xSemaphoreHandle *pxSemaphore );
KERNEL_CREATE_FUNCTION portBaseType xSemaphoreCreateCountingKrnl( portUnsignedBaseType uxMaxCount, portUnsignedBaseType uxInitialCount, portInt8Type *pcSemaphoreBuffer, xSemaphoreHandle *pxSemaphore );
KERNEL_FUNCTION portBaseType xSemaphoreGetCountDepthKrnl( xSemaphoreHandle xSemaphore, portUnsignedBaseType *puxCountDepth );
KERNEL_FUNCTION portBaseType xSemaphoreTakeKrnl( xSemaphoreHandle xSemaphore, portTickType xBlockTime );
KERNEL_FUNCTION portBaseType xSemaphoreGiveKrnl( xSemaphoreHandle xSemaphore );

#ifdef __cplusplus
}
#endif

#endif /* SEMAPHORE_API_H */
