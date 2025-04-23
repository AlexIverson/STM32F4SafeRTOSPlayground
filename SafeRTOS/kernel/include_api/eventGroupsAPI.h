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


#ifndef EVENT_GROUPS_API_H
#define EVENT_GROUPS_API_H

#ifdef __cplusplus
extern "C" {
#endif



/*-----------------------------------------------------------------------------
 * Type Definitions
 *---------------------------------------------------------------------------*/

/* Type by which event groups are referenced. */
typedef void *eventGroupHandleType;

/* The type that holds event bits matches portTickType (necessary because of the
 * use of lists to manage events). */
typedef portTickType eventBitsType;

/* The definition of the event groups themselves. NOTE: this should not be
 * accessed directly by the user application, it is defined here simply to allow
 * correctly sized buffers to be defined. */
typedef struct eventGroup
{
    eventBitsType           xEventBits;                     /* The bits that can be set to indicate that an event has occurred. */
    xList                   xTasksWaitingForBits;           /* The list of tasks waiting for one or more bits to be set. */
    xList                   xRegisteredEvtMplxObjects;      /* The list of event multiplex objects that have registered an interest in this event group. */
    portDataAddressType     uxEventMirror;                  /* The uxEventMirror will be set to the bitwise inverse (XOR) of the address of the eventGroup object. */
    /* SAFERTOSTRACE EVENTGROUPNUMBER */
} eventGroupType;


/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

/* xEventGroupCreateKrnl creates a new event group. */
KERNEL_CREATE_FUNCTION portBaseType xEventGroupCreateKrnl( eventGroupType *pxEventGroup,
                                                           eventGroupHandleType *pxEventGroupHandle );

/* xEventGroupGetBitsKrnl() retrieves the set of bits that are currently set within
 * the event group. */
KERNEL_FUNCTION portBaseType xEventGroupGetBitsKrnl( eventGroupHandleType xEventGroupHandle,
                                                     eventBitsType *pxEventBitsSet );

/* xEventGroupGetBitsFromISRKrnl() is a version of xEventGroupGetBitsKrnl() that can be
 * called from an ISR. */
KERNEL_FUNCTION portBaseType xEventGroupGetBitsFromISRKrnl( eventGroupHandleType xEventGroupHandle,
                                                            eventBitsType *pxEventBitsSet );

/* xEventGroupSetBitsKrnl() set bits within an event group. Setting bits in an event
 * group will automatically unblock tasks that are blocked waiting for the
 * bits. */
KERNEL_FUNCTION portBaseType xEventGroupSetBitsKrnl( eventGroupHandleType xEventGroupHandle,
                                                     const eventBitsType xBitsToSet );

/* xEventGroupSetBitsFromISRKrnl() is a version of xEventGroupSetBitsKrnl() that can be
 * called from an ISR. Setting bits in an event group is not a deterministic
 * operation because there are an unknown number of tasks that may be waiting
 * for the bit or bits being set. Therefore, xEventGroupSetBitsFromISRKrnl() sends a
 * message to the timer task to have the set operation performed in the context
 * of the timer task. */
KERNEL_FUNCTION portBaseType xEventGroupSetBitsFromISRKrnl( eventGroupHandleType xEventGroupHandle,
                                                            const eventBitsType xBitsToSet );

/* xEventGroupClearBitsKrnl() clears the specified bits within the event group. The
 * original setting of the event bits can be optionally retrieved by supplying a
 * non-NULL pointer for pxEventBitsSet. */
KERNEL_FUNCTION portBaseType xEventGroupClearBitsKrnl( eventGroupHandleType xEventGroupHandle,
                                                       const eventBitsType xBitsToClear );

/* xEventGroupClearBitsFromISRKrnl() is a version of xEventGroupClearBitsKrnl() that can
 * be called from an ISR. */
KERNEL_FUNCTION portBaseType xEventGroupClearBitsFromISRKrnl( eventGroupHandleType xEventGroupHandle,
                                                              const eventBitsType xBitsToClear );

/* xEventGroupWaitBitsKrnl() blocks to wait for one or more bits to be set within
 * the specified event group. */
KERNEL_FUNCTION portBaseType xEventGroupWaitBitsKrnl( eventGroupHandleType xEventGroupHandle,
                                                      const eventBitsType xBitsToWaitFor,
                                                      const portBaseType xClearOnExit,
                                                      const portBaseType xWaitForAllBits,
                                                      eventBitsType *pxEventBitsSet,
                                                      portTickType xTicksToWait );

/* xEventGroupDeleteKrnl() deletes an event group that was previously created by a
 * call to xEventGroupCreateKrnl().  Tasks that are blocked on the event group will
 * be unblocked and obtain 0 as the event group's value. */
KERNEL_DELETE_FUNCTION portBaseType xEventGroupDeleteKrnl( eventGroupHandleType xEventGroupHandle );

/* SAFERTOSTRACE UXGETEVENTGROUPNUMBERPROTO */



#ifdef __cplusplus
}
#endif

#endif /* EVENT_GROUPS_API_H */
