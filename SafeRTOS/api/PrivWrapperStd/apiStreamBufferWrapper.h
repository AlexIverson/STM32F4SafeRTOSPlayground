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

#ifndef API_STREAM_BUFFER_WRAPPER_H
#define API_STREAM_BUFFER_WRAPPER_H

/* Scheduler Includes */
#include "SafeRTOS_API.h"
#include "streambufferAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


API_CREATE_FUNCTION portBaseType xStreamBufferCreate( StreamBufferHandle_t  *pxStreamBufferHandle,
                                                      portUnsignedBaseType  uxBufferSizeBytes,
                                                      portUnsignedBaseType  uxTriggerLevelBytes,
                                                      portUnsignedBaseType  uxIsMessageBuffer,
                                                      portUInt8Type *const  pucStreamBufferStorageArea,
                                                      StreamBuffer_t *const pxStreamBuffer );

API_FUNCTION portBaseType xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                                             portUInt8Type        *pucTxData,
                                             portUnsignedBaseType uxDataLengthBytes,
                                             portTickType         xTicksToWait,
                                             portUnsignedBaseType *puxBytesWritten ) ;

API_FUNCTION portBaseType xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                                portUInt8Type        *pucRxData,
                                                portUnsignedBaseType uxBufferLengthBytes,
                                                portUnsignedBaseType *puxTotalBytesRead,
                                                portTickType         xTicksToWait );

API_FUNCTION portBaseType xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer, portUnsignedBaseType *puxIsFull );

API_FUNCTION portBaseType xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer, portUnsignedBaseType *puxIsEmpty );

API_FUNCTION portBaseType xStreamBufferReset( StreamBufferHandle_t xStreamBuffer );

API_FUNCTION portBaseType xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer,
                                                        portUnsignedBaseType *puxSpace );

API_FUNCTION portBaseType xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer,
                                                       portUnsignedBaseType *puxBytesAvailable);

API_FUNCTION portBaseType xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                                        portUnsignedBaseType uxTriggerLevel );

API_FUNCTION portBaseType xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer,
                                                               configMESSAGE_BUFFER_LENGTH_TYPE *puxNextMessageLengthBytes );

/*---------------------------------------------------------------------------*/

#define xStreamBufferSendFromISR        xStreamBufferSendFromISRKrnl
#define xStreamBufferReceiveFromISR     xStreamBufferReceiveFromISRKrnl

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* API_STREAM_BUFFER_WRAPPER_H */
