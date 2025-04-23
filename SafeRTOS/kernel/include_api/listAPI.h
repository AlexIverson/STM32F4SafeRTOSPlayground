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

#ifndef LIST_API_H
#define LIST_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * MACROS AND DEFINITIONS
 *---------------------------------------------------------------------------*/
struct xLIST_ITEM
{
    volatile portTickType xItemValue;
    struct xLIST_ITEM *volatile pxNext;
    struct xLIST_ITEM *volatile pxPrevious;
    void *pvOwner;
    void *volatile pvContainer;
};
typedef struct xLIST_ITEM xListItem;

struct xMINI_LIST_ITEM
{
    volatile portTickType xItemValue;
    struct xLIST_ITEM *volatile pxNext;
    struct xLIST_ITEM *volatile pxPrevious;
    void *pvOwner;
};
typedef struct xMINI_LIST_ITEM xMiniListItem;

typedef struct xLIST
{
    volatile portUnsignedBaseType uxNumberOfItems;
    xListItem *volatile pxIndex;
    xMiniListItem xListEnd;
} xList;


/*-----------------------------------------------------------------------------
 * Public API
 *---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* LIST_API_H */
