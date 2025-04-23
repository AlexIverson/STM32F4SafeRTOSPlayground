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

#ifndef STREAMBUFFERAPI_H_
#define STREAMBUFFERAPI_H_

#if defined( __cplusplus )
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#define sbSTREAM_BUFFER_NO_CORE   ( ( portUnsignedBaseType ) 0xFF )

/*---------------------------------------------------------------------------*/

/**
 * Type by which stream buffers are referenced.  For example, a call to
 * xStreamBufferCreate() returns an StreamBufferHandle_t variable that can
 * then be used as a parameter to uxStreamBufferSend(), uxStreamBufferReceive(),
 * etc.
 */
struct StreamBufferDef_t;
typedef struct StreamBufferDef_t *StreamBufferHandle_t;

/* Structure that hold state information on the buffer. */
typedef struct StreamBufferDef_t
{
    volatile portUnsignedBaseType uxTail;                       /* Index to the next item to read within the buffer. */
    volatile portUnsignedBaseType uxHead;                       /* Index to the next item to write within the buffer. */
    volatile portUnsignedBaseType uxBytesStored;                /* Used to keep track of how many bytes are available on the stream buffer */
    portUnsignedBaseType          uxLength;                     /* The length of the buffer pointed to by pucBuffer. */
    portUnsignedBaseType          uxTriggerLevelBytes;          /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    volatile portTaskHandleType   xTaskWaitingToReceive;        /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    volatile portTaskHandleType   xTaskWaitingToSend;           /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    portUInt8Type                 *pucBuffer;                   /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    portUnsignedBaseType          uxFlags;                      /* Holds the configuration flags of the stream buffer */
    volatile portUnsignedBaseType uxTaskWaitingToSendCore;      /* Holds the core ID of a task that is waiting to send data to a message buffer that is full. */
    volatile portUnsignedBaseType uxTaskWaitingToReceiveCore;   /* Holds the core ID of a task waiting for data, or NULL if no tasks are waiting. */
    volatile portSpinlockType     xSpinlock;                    /* The port-specific lock primitive that allows for inter-core exclusive access to shared streambuffers. */
    portUnsignedBaseType          uxLengthMirror;
    portDataAddressType           uxBufferMirror;
    /* SAFERTOSTRACE STREAMBUFFERNUMBER */
} StreamBuffer_t;

/*---------------------------------------------------------------------------*/

KERNEL_CREATE_FUNCTION portBaseType xStreamBufferCreateKrnl( StreamBufferHandle_t  *pxStreamBufferHandle,
                                                             portUnsignedBaseType  uxBufferSizeBytes,
                                                             portUnsignedBaseType  uxTriggerLevelBytes,
                                                             portUnsignedBaseType  uxIsMessageBuffer,
                                                             portUInt8Type *const  pucStreamBufferStorageArea,
                                                             StreamBuffer_t *const pxStreamBuffer );

KERNEL_FUNCTION portBaseType xStreamBufferSendKrnl( StreamBufferHandle_t xStreamBuffer,
                                                    portUInt8Type        *pucTxData,
                                                    portUnsignedBaseType uxDataLengthBytes,
                                                    portTickType         xTicksToWait,
                                                    portUnsignedBaseType *puxBytesWritten );

KERNEL_FUNCTION portBaseType xStreamBufferSendFromISRKrnl( StreamBufferHandle_t xStreamBuffer,
                                                           portUInt8Type        *pucTxData,
                                                           portUnsignedBaseType uxDataLengthBytes,
                                                           portUnsignedBaseType *puxBytesWritten );

KERNEL_FUNCTION portBaseType xStreamBufferReceiveKrnl( StreamBufferHandle_t xStreamBuffer,
                                                       portUInt8Type        *pucRxData,
                                                       portUnsignedBaseType uxBufferLengthBytes,
                                                       portUnsignedBaseType *puxTotalBytesRead,
                                                       portTickType         xTicksToWait );


KERNEL_FUNCTION portBaseType xStreamBufferReceiveFromISRKrnl( StreamBufferHandle_t xStreamBuffer,
                                                              portUInt8Type        *pucRxData,
                                                              portUnsignedBaseType uxBufferLengthBytes,
                                                              portUnsignedBaseType *puxTotalBytesRead );


KERNEL_FUNCTION portBaseType xStreamBufferIsFullKrnl( StreamBufferHandle_t xStreamBuffer, portUnsignedBaseType *puxIsFull );

KERNEL_FUNCTION portBaseType xStreamBufferIsEmptyKrnl( StreamBufferHandle_t xStreamBuffer, portUnsignedBaseType *puxIsEmpty );

KERNEL_FUNCTION portBaseType xStreamBufferResetKrnl( StreamBufferHandle_t xStreamBuffer );

KERNEL_FUNCTION portBaseType xStreamBufferSpacesAvailableKrnl( StreamBufferHandle_t xStreamBuffer,
                                                               portUnsignedBaseType *puxSpace );

KERNEL_FUNCTION portBaseType xStreamBufferBytesAvailableKrnl( StreamBufferHandle_t xStreamBuffer,
                                                              portUnsignedBaseType *puxBytesAvailable );

KERNEL_FUNCTION portBaseType xStreamBufferSetTriggerLevelKrnl( StreamBufferHandle_t xStreamBuffer,
                                                               portUnsignedBaseType uxTriggerLevel );

KERNEL_FUNCTION portBaseType xStreamBufferNextMessageLengthBytesKrnl( StreamBufferHandle_t xStreamBuffer,
                                                                      configMESSAGE_BUFFER_LENGTH_TYPE *pxNextMessageLengthBytes );

KERNEL_FUNCTION void vStreamBufferSendCompletedMulticore( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferReceiveCompletedMulticore( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferReceiveCompletedFromISRMulticore( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferSendCompletedFromISRMulticore( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferRegisterMulticoreRxWaiting( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferRegisterMulticoreRxTimeout( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferRegisterMulticoreTxWaiting( StreamBufferHandle_t xStreamBufferHandle );

KERNEL_FUNCTION void vStreamBufferRegisterMulticoreTxTimeout( StreamBufferHandle_t xStreamBufferHandle );

/* SAFERTOSTRACE SETSTREAMBUFFERNUMBERDEF */
/* SAFERTOSTRACE GETSTREAMBUFFERNUMBERDEF */
/* SAFERTOSTRACE GETSTREAMBUFFERTYPEDEF */

/*---------------------------------------------------------------------------*/

#if defined( __cplusplus )
}
#endif

#endif /* STREAMBUFFERAPI_H_ */
