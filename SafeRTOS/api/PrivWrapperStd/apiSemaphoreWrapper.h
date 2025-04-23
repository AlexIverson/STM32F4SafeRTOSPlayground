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

#ifndef API_SEMAPHORE_WRAPPER_H
#define API_SEMAPHORE_WRAPPER_H

/* Scheduler Includes */
#include "SafeRTOS_API.h"
#include "semaphoreAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


API_CREATE_FUNCTION portBaseType xSemaphoreCreateBinary( portInt8Type *pcSemaphoreBuffer,
                                                         xSemaphoreHandle *pxSemaphore );

API_CREATE_FUNCTION portBaseType xSemaphoreCreateCounting( portUnsignedBaseType uxMaxCount,
                                                           portUnsignedBaseType uxInitialCount,
                                                           portInt8Type *pcSemaphoreBuffer,
                                                           xSemaphoreHandle *pxSemaphore );

#define xSemaphoreTake( xSemaphore, xBlockTime )                xQueueReceive( ( xQueueHandle )( xSemaphore ), NULL, ( xBlockTime ) )
#define xSemaphoreGive( xSemaphore )                            xQueueSend( ( xQueueHandle )( xSemaphore ), NULL, semSEM_GIVE_BLOCK_TIME )
#define xSemaphoreGiveFromISR( xSemaphore )                     xQueueSendFromISR( ( xQueueHandle )( xSemaphore ), NULL )
#define xSemaphoreTakeFromISR( xSemaphore )                     xQueueReceiveFromISR( ( xQueueHandle )( xSemaphore ), NULL )
#define xSemaphoreGetCountDepth( xSemaphore, puxCountDepth )    xQueueMessagesWaiting( ( xQueueHandle )( xSemaphore ), ( puxCountDepth ) )

#ifdef __cplusplus
}
#endif

#endif /* API_SEMAPHORE_WRAPPER_H */
