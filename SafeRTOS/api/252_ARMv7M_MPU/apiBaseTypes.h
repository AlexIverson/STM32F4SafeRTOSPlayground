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

#ifndef API_BASE_TYPES_H
#define API_BASE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * Type definitions
 *---------------------------------------------------------------------------*/

typedef char                    portCharType;

typedef signed char             portInt8Type;
typedef signed short            portInt16Type;
typedef signed long             portInt32Type;
typedef signed long long        portInt64Type;

typedef unsigned char           portUInt8Type;
typedef unsigned short          portUInt16Type;
typedef unsigned long           portUInt32Type;
typedef unsigned long long      portUInt64Type;

typedef float                   portFloat32Type;
typedef double                  portFloat64Type;

typedef unsigned long           portStackType;
typedef signed long             portBaseType;
typedef unsigned long           portUnsignedBaseType;
typedef unsigned long           portTickType;
typedef void                    *portTaskHandleType;

typedef unsigned long           portCodeAddressType;
typedef unsigned long           portDataAddressType;

typedef unsigned long           portSpinlockType;

#ifndef NULL
#ifdef __cplusplus
#define NULL    ( 0 )
#else
#define NULL    ( ( void * ) 0 )
#endif
#endif

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* API_BASE_TYPES_H */
