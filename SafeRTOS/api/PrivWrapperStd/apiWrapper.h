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

#ifndef API_WRAPPER_H
#define API_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * API Wrapper version number
 *---------------------------------------------------------------------------*/

#define safertosAPI_WRAPPER_MAJOR_VERSION   ( 1 )
#define safertosAPI_WRAPPER_MINOR_VERSION   ( 1 )
#define safertosAPI_WRAPPER_RC_VERSION      ( 0 )


/*----------------------------------------------------------------------------
 * API Wrapper Definitions
 *--------------------------------------------------------------------------*/

/* The wrapper functions are necessary to ensure that the API functions are
* always called in privileged mode and that the correct operating mode is
* restored afterwards. */

API_FUNCTION portTickType xTaskGetTickCount( void );

API_FUNCTION portTaskHandleType xTaskGetCurrentTaskHandle( void );

API_CREATE_FUNCTION portBaseType xTaskCreate( const xTaskParameters * const pxTaskParameters,
                                              portTaskHandleType *pxCreatedTask );

API_DELETE_FUNCTION portBaseType xTaskDelete( portTaskHandleType xTaskToDelete );

API_FUNCTION portBaseType xTaskDelay( portTickType xTicksToDelay );

API_FUNCTION portBaseType xTaskDelayUntil( portTickType *pxPreviousWakeTime,
                                           portTickType xTimeIncrement );

API_FUNCTION portBaseType xTaskPriorityGet( portTaskHandleType xTask,
                                            portUnsignedBaseType *puxPriority );

API_FUNCTION portBaseType xTaskPrioritySet( portTaskHandleType xTask,
                                            portUnsignedBaseType uxNewPriority );

API_FUNCTION portBaseType xTaskSuspend( portTaskHandleType xTaskToSuspend );

API_FUNCTION portBaseType xTaskResume( portTaskHandleType xTaskToResume );

API_INIT_FUNCTION portBaseType xTaskStartScheduler( void );

API_FUNCTION void vTaskSuspendScheduler( void );

API_FUNCTION portBaseType xTaskResumeScheduler( void );

API_INIT_FUNCTION portBaseType xTaskInitializeScheduler( const xPORT_INIT_PARAMETERS *const pxPortInitParameters );

API_FUNCTION portBaseType xTaskIsSchedulerStarted( void );

API_FUNCTION portBaseType xTaskNotifyWait( portUnsignedBaseType uxBitsToClearOnEntry,
                                           portUnsignedBaseType uxBitsToClearOnExit,
                                           portUnsignedBaseType *puxNotificationValue,
                                           portTickType xTicksToWait );

API_FUNCTION portBaseType xTaskNotifySend( portTaskHandleType xTaskToNotify,
                                           portBaseType xAction,
                                           portUnsignedBaseType uxValue );

API_FUNCTION void *pvTaskTLSObjectGet( void );

API_CREATE_FUNCTION portBaseType xQueueCreate( portInt8Type *pcQueueMemory,
                                               portUnsignedBaseType uxBufferLength,
                                               portUnsignedBaseType uxQueueLength,
                                               portUnsignedBaseType uxItemSize,
                                               xQueueHandle *pxQueue );

API_FUNCTION portBaseType xQueueSend( xQueueHandle xQueue,
                                      const void *const pvItemToQueue,
                                      portTickType xTicksToWait );

API_FUNCTION portBaseType xQueueSendToFront( xQueueHandle xQueue,
                                             const void *const pvItemToQueue,
                                             portTickType xTicksToWait );

API_FUNCTION portBaseType xQueueReceive( xQueueHandle xQueue, void *const pvBuffer,
                                         portTickType xTicksToWait );

API_FUNCTION portBaseType xQueuePeek( xQueueHandle xQueue, void *const pvBuffer,
                                      portTickType xTicksToWait );

API_FUNCTION portBaseType xQueueMessagesWaiting( const xQueueHandle xQueue,
                                                 portUnsignedBaseType *puxMessagesWaiting );


/*-----------------------------------------------------------------------------
 * API Macro Definitions
 *---------------------------------------------------------------------------*/

/* Map Kernel ISR Functions directly to the Secure Wrapper Functions. */
#define xTaskNotifySendFromISR          xTaskNotifySendFromISRKrnl
#define xTaskIsSchedulerStartedFromISR  xTaskIsSchedulerStartedFromISRKrnl
#define xTaskGetTickCountFromISR        xTaskGetTickCountFromISRKrnl
#define xQueueSendFromISR               xQueueSendFromISRKrnl
#define xQueueSendToFrontFromISR        xQueueSendToFrontFromISRKrnl
#define xQueueReceiveFromISR            xQueueReceiveFromISRKrnl

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* API_WRAPPER_H */
