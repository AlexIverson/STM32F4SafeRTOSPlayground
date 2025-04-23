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

#ifndef SAFERTOS_API_H
#define SAFERTOS_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * Port-specific API version number
 *---------------------------------------------------------------------------*/

#define safertosAPI_PORT_MAJOR_VERSION  ( 1 )
#define safertosAPI_PORT_MINOR_VERSION  ( 0 )
#define safertosAPI_PORT_RC_VERSION     ( 0 )

/* Application specific configuration options. */
#include "SafeRTOSConfig.h"

/*---------------------------------------------------------------------------*/

/* Errata workarounds macros must be defined. */
#ifndef configARM_ERRATA_837070
#error "configARM_ERRATA_837070 must be defined as 0 or 1."
#endif

#ifndef configARM_ERRATA_838869
#error "configARM_ERRATA_838869 must be defined as 0 or 1."
#endif

/*---------------------------------------------------------------------------*/

/* Define kernel function area definitions. */
#define KERNEL_FUNCTION         configKERNEL_FUNC_DEF
#define KERNEL_INIT_FUNCTION    configKERNEL_FUNC_DEF
#define KERNEL_CREATE_FUNCTION  configKERNEL_FUNC_DEF
#define KERNEL_DELETE_FUNCTION  configKERNEL_FUNC_DEF
#define KERNEL_UNPRIV_FUNCTION  configKERNEL_UNPRIV_FUNC_DEF
#define KERNEL_DATA             configKERNEL_DATA_DEF
#define KERNEL_DATA_MIRROR      configKERNEL_DATA_DEF

#define API_FUNCTION            configKERNEL_UNPRIV_FUNC_DEF
#define API_INIT_FUNCTION       configKERNEL_UNPRIV_FUNC_DEF
#define API_CREATE_FUNCTION     configKERNEL_UNPRIV_FUNC_DEF
#define API_DELETE_FUNCTION     configKERNEL_UNPRIV_FUNC_DEF

/* Basic SafeRTOS definitions. */
#include "apiBaseTypes.h"
#include "projdefs.h"
#include "apiComplexTypes.h"
#include "portAPI.h"

#include "listAPI.h"
#include "queueAPI.h"
#include "semaphoreAPI.h"
#include "timersAPI.h"
#include "eventGroupsAPI.h"
#include "mutexAPI.h"
#include "evtmplxAPI.h"
#include "taskAPI.h"
#include "streambufferAPI.h"
#include "apiMPU.h"
#include "apiWrapper.h"

#include "apiEventGroupsWrapper.h"
#include "apiEvtMplxWrapper.h"
#include "apiMutexWrapper.h"
#include "apiSemaphoreWrapper.h"
#include "apiStreamBufferWrapper.h"
#include "apiTimersWrapper.h"

#if ( configINCLUDE_CHECKPOINTS == 1 )
#include "apiCheckpointsWrapper.h"
#endif

/* SAFERTOSTRACE RTSINCLUDE */

/* MCDC Test Point: PROTOTYPE */

/* SAFERTOSTRACE PORTQUEUEOVERHEADBYTES */
#define safertosapiQUEUE_OVERHEAD_BYTES         ( 140U )
/* SAFERTOSTRACE PORTQUEUEOVERHEADBYTESENDIF */

#define safertosapiMAX_DELAY                    ( ( portTickType ) 0xFFFFFFFFU )

#define safertosapiWORD_ALIGNMENT               ( 4U )

#define safertosapiSTACK_ALIGNMENT              ( 8U )


/*-----------------------------------------------------------------------------
 * Critical sections management
 *---------------------------------------------------------------------------*/

#define safertosapiSET_INTERRUPT_MASK_FROM_ISR      portcompapiSET_INTERRUPT_MASK_FROM_ISR

#define safertosapiCLEAR_INTERRUPT_MASK_FROM_ISR    portcompapiCLEAR_INTERRUPT_MASK_FROM_ISR

#define safertosapiENTER_CRITICAL                   portcompapiENTER_CRITICAL

#define safertosapiEXIT_CRITICAL                    portcompapiEXIT_CRITICAL


/*-----------------------------------------------------------------------------
 * Scheduler utilities
 *---------------------------------------------------------------------------*/

#define safertosapiYIELD            portcompapiYIELD

#define safertosapiYIELD_FROM_ISR   portapiYIELD_FROM_ISR


/*-----------------------------------------------------------------------------
 * Prototypes for application functions
 *---------------------------------------------------------------------------*/

/* The application must instantiate an error hook function. */
void vApplicationErrorHook( portTaskHandleType xHandleOfTaskWithError,
                            portBaseType xErrorCode );

/* The application may define vApplicationSetupTickInterruptHook(),
 * when it is desired to use an alternative timer to generate the system tick.
 * A weak function is provided if the application does not want to implement
 * this hook function. */
void vApplicationSetupTickInterruptHook( portUInt32Type ulClockHz,
                                         portUInt32Type ulRateHz );

/* The application may define vApplicationTaskDeleteHook(),
 * when it is desired to perform additional processing when a task is deleted.
 * A weak function is provided if the application does not want to implement
 * this hook function. */
void vApplicationTaskDeleteHook( portTaskHandleType xTaskBeingDeleted );

/* The application may define vApplicationTickHook(),
 * when it is desired to perform additional processing on each tick interrupt.
 * A weak function is provided if the application does not want to implement
 * this hook function. */
void vApplicationTickHook( void );

/* The application may define vApplicationIdleHook(),
 * when it is desired to perform background processing.
 * A weak function is provided if the application does not want to implement
 * this hook function. */
void vApplicationIdleHook( void );

/* SAFERTOSTRACE TRCINCLUDE */

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* SAFERTOS_API_H */
