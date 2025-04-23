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

#ifndef API_EVNT_MPLX_WRAPPER_H
#define API_EVNT_MPLX_WRAPPER_H

/* Scheduler Includes */
#include "SafeRTOS_API.h"
#include "evtmplxAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


API_CREATE_FUNCTION portBaseType xEvtMplxCreate( portInt8Type *pcEvtMplxMemoryBuffer,
                                                 portUnsignedBaseType uxBufferLengthInBytes,
                                                 portUnsignedBaseType uxMaximumRegisteredObjectEvents,
                                                 portTaskHandleType xOwnerTaskHandle,
                                                 evtMplxHandleType *pxEvtMplxHandle );

API_FUNCTION portBaseType xEvtMplxAddObjectEvents( evtMplxHandleType xEvtMplxHandle,
                                                   void *pvTargetObjectHandle,
                                                   portUnsignedBaseType uxEvents );

API_FUNCTION portBaseType xEvtMplxModifyObjectEvents( evtMplxHandleType xEvtMplxHandle,
                                                      void *pvTargetObjectHandle,
                                                      portUnsignedBaseType uxEvents );

API_FUNCTION portBaseType xEvtMplxRemoveObjectEvents( evtMplxHandleType xEvtMplxHandle,
                                                      const void *pvTargetObjectHandle );

API_FUNCTION portBaseType xEvtMplxWait( evtMplxHandleType xEvtMplxHandle,
                                        evtMplxObjectEventsType axObjectEvents[],
                                        portUnsignedBaseType uxObjectEventsArraySize,
                                        portUnsignedBaseType *puxNumberOfObjectEvents,
                                        portTickType xTicksToWait );


#ifdef __cplusplus
}
#endif

#endif /* API_EVNT_MPLX_WRAPPER_H */
