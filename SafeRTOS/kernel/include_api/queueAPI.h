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

/*
    API documentation is included in the user manual.  Do not refer to this
    file for documentation.
*/

#ifndef QUEUE_API_H
#define QUEUE_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * MACROS AND DEFINITIONS
 *---------------------------------------------------------------------------*/

typedef void *xQueueHandle;
/*
 * Definition of the queue used by the scheduler. This is meant for internal Kernel use, not to be used directly by user code.
 * Items are queued by copy, not reference.
 */
typedef struct QueueDefinition
{
    portInt8Type *pcHead;                       /* Points to the beginning of the queue storage area. */
    portInt8Type *pcTail;                       /* Points to the byte at the end of the queue storage area. */

    portInt8Type *pcWriteTo;                    /* Points to the free next place in the storage area. */
    portInt8Type *pcReadFrom;                   /* Points to the last place that a queued item was read from. */

    xList xTasksWaitingToSend;                  /* List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    xList xTasksWaitingToReceive;               /* List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    portUnsignedBaseType uxItemsWaiting;        /* The number of items currently in the queue. */
    portUnsignedBaseType uxMaxNumberOfItems;    /* The length of the queue defined as the number of items it will hold, not the number of bytes. */
    portUnsignedBaseType uxItemSize;            /* The size of each item that the queue will hold. */

    portBaseType xRxLock;                       /* Set to queueUNLOCKED when the queue is not locked, or notes whether or not the queue has been accessed if the queue is locked. */
    portBaseType xTxLock;                       /* Set to queueUNLOCKED when the queue is not locked, or notes whether or not the queue has been accessed if the queue is locked. */

    portUnsignedBaseType uxQueueType;           /* Indicates whether the queue is a mutex or just a regular queue. */
    portUnsignedBaseType uxMutexRecursiveDepth; /* Indicates the depth of recursive calls to a mutex. */
    xListItem xMutexHolder;                     /* A list item so that tasks can keep a record of multiple mutexes held. */

    xList xRegisteredEvtMplxObjects;            /* The list of event multiplex objects that have registered an interest in this queue. */

    portDataAddressType uxHeadMirror;          /* The uxHeadMirror will be set to the bitwise inverse (XOR) of the value assigned to pcHead. */

    /* SAFERTOSTRACE QUEUEDEFINITION */
} xQUEUE;
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

KERNEL_CREATE_FUNCTION portBaseType xQueueCreateKrnl( portInt8Type *pcQueueMemory, portUnsignedBaseType uxBufferLength, portUnsignedBaseType uxQueueLength, portUnsignedBaseType uxItemSize, xQueueHandle *pxCreatedQueue );
KERNEL_FUNCTION portBaseType xQueueSendKrnl( xQueueHandle xQueue, const void *const pvItemToQueue, portTickType xTicksToWait );
KERNEL_FUNCTION portBaseType xQueueSendToFrontKrnl( xQueueHandle xQueue, const void *const pvItemToQueue, portTickType xTicksToWait );
KERNEL_FUNCTION portBaseType xQueueReceiveKrnl( xQueueHandle xQueue, void *const pvBuffer, portTickType xTicksToWait );
KERNEL_FUNCTION portBaseType xQueuePeekKrnl( xQueueHandle xQueue, void *const pvBuffer, portTickType xTicksToWait );
KERNEL_FUNCTION portBaseType xQueueMessagesWaitingKrnl( const xQueueHandle xQueue, portUnsignedBaseType *puxMessagesWaiting );
KERNEL_FUNCTION portBaseType xQueueSendFromISRKrnl( xQueueHandle xQueue, const void *const pvItemToQueue );
KERNEL_FUNCTION portBaseType xQueueSendToFrontFromISRKrnl( xQueueHandle xQueue, const void *const pvItemToQueue );
KERNEL_FUNCTION portBaseType xQueueReceiveFromISRKrnl( xQueueHandle xQueue, void *const pvBuffer );

/* SAFERTOSTRACE QUEUEFUNCTIONS */


#ifdef __cplusplus
}
#endif

#endif /* QUEUE_API_H */
