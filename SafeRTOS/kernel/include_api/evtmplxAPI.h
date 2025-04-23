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


#ifndef EVT_MPLX_API_H
#define EVT_MPLX_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * Constants
 *---------------------------------------------------------------------------*/
/* The following are the valid values for the uxEvents parameter in calls to
 * xEvtMplxAddObjectEventsKrnl() and xEvtMplxModifyObjectEventsKrnl():
 * evtmplxQUEUE_MESSAGE_WAITING       - The queue contains at least one message
 *                                      waiting to be received.
 * evtmplxQUEUE_SPACE_AVAILABLE       - The queue has space for at least one
 *                                      message to be sent.
 * evtmplxSEMAPHORE_AVAILABLE         - The semaphore (either binary or
 *                                      counting) is available to take.
 * evtmplxMUTEX_AVAILABLE             - The mutex is available to take.
 * evtmplxTASK_NOTIFICATION_RECEIVED  - The task that owns the event multiplex
 *                                      object has received a task
 *                                      notification.
 * evtmplxEVENT_GROUP_BITS_SET        - The event group has been updated due
 *                                      to a call to either
 *                                      xEventGroupSetBitsKrnl() or
 *                                      xEventGroupSetBitsFromISRKrnl(). */
#define evtmplxQUEUE_MESSAGE_WAITING          ( ( portUnsignedBaseType ) 0x00000001 )
#define evtmplxQUEUE_SPACE_AVAILABLE          ( ( portUnsignedBaseType ) 0x00000002 )
#define evtmplxSEMAPHORE_AVAILABLE            ( ( portUnsignedBaseType ) 0x00000001 )
#define evtmplxMUTEX_AVAILABLE                ( ( portUnsignedBaseType ) 0x00000001 )
#define evtmplxTASK_NOTIFICATION_RECEIVED     ( ( portUnsignedBaseType ) 0x00000010 )
#define evtmplxEVENT_GROUP_BITS_SET           ( ( portUnsignedBaseType ) 0x00000100 )
/* This constant indicates that no object-events have occurred. It is useful for
 * checking the uxEvents member of elements of the axObjectEvents parameter used
 * in calls to xEvtMplxWaitKrnl(). */
#define evtmplxNO_EVENTS                      ( ( portUnsignedBaseType ) 0 )


/*-----------------------------------------------------------------------------
 * Type Definitions
 *---------------------------------------------------------------------------*/
/* Type by which event multiplex objects are referenced. */
typedef void *evtMplxHandleType;

/* Structure that defines an event multiplex "object-events". */
typedef struct evtMplxObjectEvents
{
    void                   *pvObjectHandle;
    portUnsignedBaseType    uxEvents;
} evtMplxObjectEventsType;

/* Control structure for each "object-events" type. */
typedef struct evtMplxObjectEventControl
{
    evtMplxObjectEventsType     xObjectEvent;
    xListItem                   xObjectEventListItem;
    portUnsignedBaseType        uxCurrentEvents;
} evtMplxObjectEventControlType;

/* Definition of the event multiplex object. */
typedef struct evtMplx
{
    xTCB                               *pxOwnerTCB;         /* Allows the event multiplex object to access the owner task. */
    xListItem                           xEvtMplxListItem; /* Allows the owner task to access the event multiplex object. */
    portBaseType                        xTaskIsBlocked;
    portDataAddressType                 uxOwnerTCBMirror;
    portUnsignedBaseType                uxNumberOfRegisteredObjectEvents;
    portUnsignedBaseType                uxMaximumRegisteredObjectEvents;
    evtMplxObjectEventControlType      *paxRegisteredObjectEvents;
} evtMplxType;


/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/
/* Provides the required size (in bytes) of the buffer that should be passed to
 * xEvtMplxCreateKrnl() in order to create an event multiplex object capable of holding
 * uxMaximumRegisteredObjectEvents object-events.  */
#define evtmplxGET_REQUIRED_CREATE_BUFFER_SIZE( uxMaximumRegisteredObjectEvents ) \
                ( ( portUnsignedBaseType ) sizeof( evtMplxType ) + ( ( portUnsignedBaseType ) sizeof( evtMplxObjectEventControlType ) * ( uxMaximumRegisteredObjectEvents ) ) )

/* Initialises the supplied event multiplex object and supplies the corresponding
 * handle to the calling function. */
KERNEL_CREATE_FUNCTION portBaseType xEvtMplxCreateKrnl(   portInt8Type *pcEvtMplxMemoryBuffer,
                                                          portUnsignedBaseType uxBufferLengthInBytes,
                                                          portUnsignedBaseType uxMaximumRegisteredObjectEvents,
                                                          portTaskHandleType xOwnerTaskHandle,
                                                          evtMplxHandleType *pxEvtMplxHandle );

/* Registers an object-event consisting of the target object and the specified
 * events with the event multiplex object referenced from the supplied event multiplex
 * object handle.
 * Note that this can only be done if there is no current association between
 * the target object and the event multiplex object. If an association already
 * exists, use xEvtMplxModifyObjectEventsKrnl() instead. */
KERNEL_FUNCTION portBaseType xEvtMplxAddObjectEventsKrnl(   evtMplxHandleType xEvtMplxHandle,
                                                            void *pvTargetObjectHandle,
                                                            portUnsignedBaseType uxEvents );

/* Modifies the set of object-events for the target object that are registered
 * with the event multiplex object referenced from the supplied event multiplex object
 * handle. */
KERNEL_FUNCTION portBaseType xEvtMplxModifyObjectEventsKrnl(   evtMplxHandleType xEvtMplxHandle,
                                                               const void *pvTargetObjectHandle,
                                                               portUnsignedBaseType uxEvents );

/* Removes the registration of the specified target object from the event multiplex
 * object referenced from the supplied event multiplex object handle. */
KERNEL_FUNCTION portBaseType xEvtMplxRemoveObjectEventsKrnl(   evtMplxHandleType xEvtMplxHandle,
                                                               const void *pvTargetObjectHandle );

/* If any of the object-events registered with the event multiplex object referenced
 * from the supplied event multiplex object handle have occurred, they are copied to
 * the supplied object-events array. If none of the registered object-events
 * have occurred, xEvtMplxWaitKrnl() will block for a maximum of xTicksToWait. */
KERNEL_FUNCTION portBaseType xEvtMplxWaitKrnl(   evtMplxHandleType xEvtMplxHandle,
                                                 evtMplxObjectEventsType axObjectEvents[],
                                                 portUnsignedBaseType uxObjectEventsArraySize,
                                                 portUnsignedBaseType *puxNumberOfObjectEvents,
                                                 portTickType xTicksToWait );

/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* EVT_MPLX_API_H */
