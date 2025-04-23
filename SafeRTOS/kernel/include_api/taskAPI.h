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

/*
    API documentation is included in the user manual.  Do not refer to this
    file for documentation.
*/

#ifndef TASK_API_H
#define TASK_API_H

#ifdef __cplusplus
extern "C" {
#endif

extern volatile portBaseType xHigherPriorityTaskWoken;
extern xTCB *volatile pxCurrentTCB;

/*-----------------------------------------------------------------------------
 * MACROS AND DEFINITIONS
 *---------------------------------------------------------------------------*/

/* Actions that can be performed when vTaskNotify() is called. */
#define taskNOTIFICATION_NO_ACTION                      ( 0 )   /* Notify the task without updating its notify value. */
#define taskNOTIFICATION_SET_BITS                       ( 1 )   /* Set bits in the task's notification value. */
#define taskNOTIFICATION_INCREMENT                      ( 2 )   /* Increment the task's notification value. */
#define taskNOTIFICATION_SET_VAL_WITH_OVERWRITE         ( 3 )   /* Set the task's notification value to a specific value even if the previous value has not yet been read by the task. */
#define taskNOTIFICATION_SET_VAL_WITHOUT_OVERWRITE      ( 4 )   /* Set the task's notification value if the previous value has been read by the task. */

/* Notification states. */
#define taskNOTIFICATION_NOT_WAITING                    ( ( portBaseType ) 0 )
#define taskNOTIFICATION_WAITING                        ( ( portBaseType ) 1 )
#define taskNOTIFICATION_NOTIFIED                       ( ( portBaseType ) 2 )

typedef struct xTIME_OUT
{
    portUnsignedBaseType uxOverflowCount;
    portTickType xTimeOnEntering;
} xTimeOutType;

#define taskIDLE_PRIORITY                               ( 0U )

/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/

KERNEL_CREATE_FUNCTION portBaseType xTaskCreateKrnl( const xTaskParameters *const pxTaskParameters, portTaskHandleType *pxCreatedTask );
KERNEL_DELETE_FUNCTION portBaseType xTaskDeleteKrnl( portTaskHandleType xTaskToDelete );
KERNEL_FUNCTION portBaseType xTaskDelayKrnl( portTickType xTicksToDelay );
KERNEL_FUNCTION portBaseType xTaskDelayUntilKrnl( portTickType *pxPreviousWakeTime, portTickType xTimeIncrement );
KERNEL_FUNCTION portBaseType xTaskPriorityGetKrnl( portTaskHandleType xTask, portUnsignedBaseType *puxPriority );
KERNEL_FUNCTION portBaseType xTaskPrioritySetKrnl( portTaskHandleType xTask, portUnsignedBaseType uxNewPriority );
KERNEL_FUNCTION portBaseType xTaskSuspendKrnl( portTaskHandleType xTaskToSuspend );
KERNEL_FUNCTION portBaseType xTaskResumeKrnl( portTaskHandleType xTaskToResume );
KERNEL_INIT_FUNCTION portBaseType xTaskStartSchedulerKrnl( void );
KERNEL_FUNCTION void vTaskSuspendSchedulerKrnl( void );
KERNEL_FUNCTION portBaseType xTaskResumeSchedulerKrnl( void );
KERNEL_FUNCTION portTickType xTaskGetTickCountKrnl( void );
KERNEL_FUNCTION portTickType xTaskGetTickCountFromISRKrnl( void );
KERNEL_INIT_FUNCTION portBaseType xTaskInitializeSchedulerKrnl( const xPORT_INIT_PARAMETERS *const pxPortInitParameters );
KERNEL_FUNCTION portTaskHandleType xTaskGetCurrentTaskHandleKrnl( void );
KERNEL_FUNCTION portBaseType xTaskIsSchedulerStartedKrnl( void );
KERNEL_FUNCTION portBaseType xTaskIsSchedulerStartedFromISRKrnl( void );

KERNEL_FUNCTION portBaseType xTaskNotifyWaitKrnl( portUnsignedBaseType uxBitsToClearOnEntry,
                                                  portUnsignedBaseType uxBitsToClearOnExit,
                                                  portUnsignedBaseType *puxNotificationValue,
                                                  portTickType xTicksToWait );
KERNEL_FUNCTION portBaseType xTaskNotifySendKrnl( portTaskHandleType xTaskToNotify,
                                                  portBaseType xAction,
                                                  portUnsignedBaseType uxValue );
KERNEL_FUNCTION portBaseType xTaskNotifySendFromISRKrnl( portTaskHandleType xTaskToNotify,
                                                         portBaseType xAction,
                                                         portUnsignedBaseType uxValue);

/* Returns a pointer to the Task Local Storage Object */
KERNEL_FUNCTION void *pvTaskTLSObjectGetKrnl( void );

/*
 * Handler that should be called from the system tick handler.
 * If an alternative tick handler is not being used then this function should be
 * installed as the interrupt handler for the interrupt that will generate the
 * system tick. (see SafeRTOSConfig.h).
 */
KERNEL_FUNCTION void vTaskProcessSystemTickFromISR( void );

/* SAFERTOSTRACE TASKGETTASKNUMBERDEF */

/*
 * The idle task, which, as all tasks, is implemented as a never ending loop.
 * The idle task is automatically created and added to the ready lists when
 * the scheduler is started. It is declared with global scope so that the port
 * layer can link it into the idle task xTaskParameter structure.
 */
KERNEL_FUNCTION void vIdleTask( void *pvParameters );

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* TASK_API_H */
