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

#ifndef TIMERS_API_H
#define TIMERS_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * Type Definitions
 *---------------------------------------------------------------------------*/

/* Type by which software timers are referenced. */
typedef void *timerHandleType;

/* Define the prototype to which timer callback function pointers must conform. */
typedef void ( *timerCallbackFunctionPtrType )( timerHandleType xTimer );

/* Forward reference the timerInstanceParametersType and timerQueueMessageType types. */
typedef struct timerInstanceParameters timerInstanceParametersType;
typedef struct timerQueueMessage timerQueueMessageType;

/* Define the prototype for a timer instance specific message handling function. */
typedef portBaseType ( *timerMessageHandlerFunctionPtrType )( timerQueueMessageType xMessage, portTickType xTimeNow );

/* The structure of the timer task parameters. */
struct timerInstanceParameters
{
    xQueueHandle                                xTimerQueueHandle;
    xList                                       *pxCurrentTimerList;
    xList                                       *pxOverflowTimerList;
    xList                                       xActiveTimerList1;
    xList                                       xActiveTimerList2;
    xTCB                                        xTimerTaskTCB;
    volatile timerMessageHandlerFunctionPtrType pxInstanceMessageHandler;
    portDataAddressType                         uxTimerQueueHandleMirror;
    volatile portCodeAddressType                uxInstanceMessageHandlerMirror;
    portTickType                                xLastSampleTime;
};

/* The definition of the timers themselves.
 * NOTE: this should not be accessed directly by the user application, it is
 * defined here simply to allow correctly sized buffers to be defined.*/
typedef struct timerControlBlock
{
    timerCallbackFunctionPtrType    pxCallbackFunction;     /* The function that will be called when the timer expires. */
    const portCharType             *pcTimerName;            /* Text name. This is not used by the kernel, it is included simply to make debugging easier. */
    xListItem                       xTimerListItem;         /* Standard linked list item as used by all kernel features for event management. */
    portTickType                    xTimerPeriodInTicks;    /* How quickly and often the timer expires. */
    portBaseType                    xIsPeriodic;            /* Set to pdTRUE if the timer should be automatically restarted once expired. Set to pdFALSE if the timer is, in effect, a one shot timer. */
    portBaseType                    xIsActive;              /* Set to pdTRUE while the timer is active. */
    portBaseType                    xTimerID;               /* ID parameter - for application use to assist in identifying instances of timers when a common callback is used. */
    timerInstanceParametersType    *pxTimerInstance;        /* The timer instance to which this timer belongs. */
    void                           *pvObject;               /* Points to the instance of the C++ object that tracks this structure. */
    portBaseType                    xIsDeleted;             /* Boolean flag to discard commands after a timer object has been deleted. */
    portCodeAddressType             uxCallbackMirror;       /* A mirror of the function pointer, used to validate the timer structure. */
    portTickType                    xTimerPeriodMirror;     /* A mirror of xTimerPeriodInTicks. */
    portTaskHandleType              xTaskToNotify;          /* The task to trigger if this is a notification timer. */
    portDataAddressType             uxTaskToNotifyMirror;   /* A mirror of the task to notify */
    /* SAFERTOSTRACE TIMERNUMBER */
} timerControlBlockType;

/* The structure supplied to xTimerCreateKrnl */
typedef struct timerInitParameters
{
    const portCharType *pcTimerName;
    portTickType xTimerPeriodInTicks;
    portBaseType xIsPeriodic;
    portBaseType xTimerID;
    timerControlBlockType *pxNewTimer;
    timerCallbackFunctionPtrType pxCallbackFunction;
    timerInstanceParametersType *pxTimerInstance;
    void *pvObject;
    portTaskHandleType xTaskToNotify;
} timerInitParametersType;

/* The definition of messages that can be sent and received on the timer queue.
 * NOTE: this should not be accessed directly by the user application, it is
 * defined here simply to allow correctly sized buffers to be defined. */
struct timerQueueMessage
{
    portBaseType    xMessageID;         /* The command being sent to the timer service task. */
    portTickType    xMessageValue;      /* An optional value used by a subset of commands, for example, when changing the period of a timer. */
    void           *pvMessageObject;    /* The type of the message depends on the message type. */
};

/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

/* xTimerCreateKrnl creates a new timer. */
KERNEL_CREATE_FUNCTION portBaseType xTimerCreateKrnl( const timerInitParametersType *const pxTimerParameters, timerHandleType *pxCreatedTimer );

/* Functions to control a timer (start/stop/reset/change and delete). */
KERNEL_FUNCTION portBaseType xTimerStartKrnl( timerHandleType xTimer, portTickType xBlockTime );
KERNEL_FUNCTION portBaseType xTimerStopKrnl( timerHandleType xTimer, portTickType xBlockTime );
KERNEL_FUNCTION portBaseType xTimerChangePeriodKrnl( timerHandleType xTimer, portTickType xNewPeriodInTicks, portTickType xBlockTime );
KERNEL_DELETE_FUNCTION portBaseType xTimerDeleteKrnl( timerHandleType xTimer, portTickType xBlockTime );

/* ISR friendly versions or the timer command functions. */
KERNEL_FUNCTION portBaseType xTimerStartFromISRKrnl( timerHandleType xTimer );
KERNEL_FUNCTION portBaseType xTimerStopFromISRKrnl( timerHandleType xTimer );
KERNEL_FUNCTION portBaseType xTimerChangePeriodFromISRKrnl( timerHandleType xTimer, portTickType xNewPeriodInTicks );

/* xTimerIsTimerActiveKrnl: Queries a timer to see if it is active or dormant. */
KERNEL_FUNCTION portBaseType xTimerIsTimerActiveKrnl( timerHandleType xTimer );

/* Returns the ID assigned to the timer. */
KERNEL_FUNCTION portBaseType xTimerGetTimerIDKrnl( timerHandleType xTimer, portBaseType *pxTimerID );

/* Returns a pointer to the Timer Local Storage Object */
KERNEL_FUNCTION void *pvTimerTLSObjectGetKrnl( timerHandleType xTimer );


/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* TIMERS_API_H */
