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

#ifndef API_TIMERS_WRAPPER_H
#define API_TIMERS_WRAPPER_H

/* Scheduler Includes */
#include "SafeRTOS_API.h"
#include "timersAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


API_CREATE_FUNCTION portBaseType xTimerCreate( const timerInitParametersType *const pxTimerParameters,
                                               timerHandleType *pxCreatedTimer );

API_FUNCTION portBaseType xTimerStart( timerHandleType xTimer,
                                       portTickType xBlockTime );

API_FUNCTION portBaseType xTimerStop( timerHandleType xTimer,
                                      portTickType xBlockTime );

API_FUNCTION portBaseType xTimerChangePeriod( timerHandleType xTimer,
                                              portTickType xNewPeriodInTicks,
                                              portTickType xBlockTime );

API_DELETE_FUNCTION portBaseType xTimerDelete( timerHandleType xTimer,
                                               portTickType xBlockTime );

API_FUNCTION portBaseType xTimerGetTimerID( timerHandleType xTimer,
                                            portBaseType *pxTimerID );

API_FUNCTION portBaseType xTimerIsTimerActive( timerHandleType xTimer );

API_FUNCTION void *pvTimerTLSObjectGet( timerHandleType xTimer );

/*---------------------------------------------------------------------------*/

#define xTimerStartFromISR              xTimerStartFromISRKrnl
#define xTimerStopFromISR               xTimerStopFromISRKrnl
#define xTimerChangePeriodFromISR       xTimerChangePeriodFromISRKrnl

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* API_TIMERS_WRAPPER_H */
