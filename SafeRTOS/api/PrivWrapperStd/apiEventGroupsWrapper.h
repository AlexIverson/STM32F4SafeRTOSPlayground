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

#ifndef API_EVENT_GROUPS_WRAPPER_H
#define API_EVENT_GROUPS_WRAPPER_H

/* Scheduler Includes */
#include "SafeRTOS_API.h"
#include "eventGroupsAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

API_CREATE_FUNCTION portBaseType xEventGroupCreate( eventGroupType *pxEventGroup,
                                                    eventGroupHandleType *pxEventGroupHandle );

API_FUNCTION portBaseType xEventGroupGetBits( eventGroupHandleType xEventGroupHandle,
                                              eventBitsType *pxEventBitsSet );

API_FUNCTION portBaseType xEventGroupSetBits( eventGroupHandleType xEventGroupHandle,
                                              const eventBitsType xBitsToSet );

API_FUNCTION portBaseType xEventGroupClearBits( eventGroupHandleType xEventGroupHandle,
                                                const eventBitsType xBitsToClear );

API_FUNCTION portBaseType xEventGroupWaitBits( eventGroupHandleType xEventGroupHandle,
                                               const eventBitsType xBitsToWaitFor,
                                               const portBaseType xClearOnExit,
                                               const portBaseType xWaitForAllBits,
                                               eventBitsType *pxEventBitsSet,
                                               portTickType xTicksToWait );

API_DELETE_FUNCTION portBaseType xEventGroupDelete( eventGroupHandleType xEventGroupHandle );


/*---------------------------------------------------------------------------*/

/* Map Kernel ISR Functions directly to the Secure Wrapper Functions. */
#define xEventGroupGetBitsFromISR       xEventGroupGetBitsFromISRKrnl
#define xEventGroupSetBitsFromISR       xEventGroupSetBitsFromISRKrnl
#define xEventGroupClearBitsFromISR     xEventGroupClearBitsFromISRKrnl


/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* API_EVENT_GROUPS_WRAPPER_H */
