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


#ifndef CREATE_DELETE_TASK_H
#define CREATE_DELETE_TASK_H

/*---------------------------------------------------------------------------*/

/* Incremented within the task delete hook so the create task can ensure the
 * same number of tasks are deleted as created. */
extern volatile portUnsignedBaseType uxCreateDeleteDemoDeleteCount;

/* The TCBs of the self-deleting tasks */
extern xTCB xDummyTaskTCB;
extern xTCB xDeleterTaskTCB;
/*---------------------------------------------------------------------------*/

portBaseType xStartTaskCreateDeleteDemo( portUnsignedBaseType uxPriority );
portBaseType xIsCreateTaskStillRunning( void );

/*---------------------------------------------------------------------------*/

#endif /* CREATE_DELETE_TASK_H */


