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

#ifndef API_MPU_H
#define API_MPU_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
 * MPU Macro Definitions
 *---------------------------------------------------------------------------*/

/* Definitions used to define tasks as privileged or unprivileged. */
#define mpuUNPRIVILEGED_TASK                ( ( portUnsignedBaseType ) 0 )
#define mpuPRIVILEGED_TASK                  ( ( portUnsignedBaseType ) 1 )

#if ( configNUM_MPU_REGIONS == 8 )

/* MPU configurable system region identifiers. */
#define mpuUNPRIVILEGED_FLASH_REGION        ( 0U )
#define mpuGLOBAL_CONFIGURABLE_REGION_FIRST ( 1U )
#define mpuGLOBAL_CONFIGURABLE_REGION_LAST  ( 1U )
#define mpuCONFIGURABLE_REGION_FIRST        ( 2U )
#define mpuCONFIGURABLE_REGION_LAST         ( 4U )
#define mpuSTACK_REGION                     ( 5U )
#define mpuKERNEL_FLASH_REGION              ( 6U )
#define mpuKERNEL_RAM_REGION                ( 7U )

#elif ( configNUM_MPU_REGIONS == 16 )

/* MPU configurable system region identifiers. */
#define mpuUNPRIVILEGED_FLASH_REGION        ( 0U )
#define mpuGLOBAL_CONFIGURABLE_REGION_FIRST ( 1U )
#define mpuGLOBAL_CONFIGURABLE_REGION_LAST  ( 5U )
#define mpuCONFIGURABLE_REGION_FIRST        ( 6U )
#define mpuCONFIGURABLE_REGION_LAST         ( 12U )
#define mpuSTACK_REGION                     ( 13U )
#define mpuKERNEL_FLASH_REGION              ( 14U )
#define mpuKERNEL_RAM_REGION                ( 15U )

#else

#error "Error: This port only supports 8 or 16 MPU regions"

#endif

/* MPU configurable task regions (LAST - FIRST + 1). */
#define mpuCONFIGURABLE_REGION_NUM          ( mpuCONFIGURABLE_REGION_LAST - mpuCONFIGURABLE_REGION_FIRST + 1U )

/* One more region for the task's stack. */
#define mpuTASK_REGION_NUM                  ( mpuCONFIGURABLE_REGION_NUM + 1U )

/*
 * Constants used to configure the region attributes.
 */

/* Region Share enable bit */
#define mpuREGION_SHAREABLE                 ( 0x01UL << 18U )

/* Instruction Executable access disable bit */
#define mpuREGION_EXECUTE_NEVER             ( 0x01UL << 28U )

/* Sub-Region Disable definitions */
#define mpuREGION_ALL_SUB_REGIONS_ENABLED   ( 0x00UL )
#define mpuREGION_1ST_SUB_REGION_DISABLE    ( 0x01UL <<  8U )
#define mpuREGION_2ND_SUB_REGION_DISABLE    ( 0x01UL <<  9U )
#define mpuREGION_3RD_SUB_REGION_DISABLE    ( 0x01UL << 10U )
#define mpuREGION_4TH_SUB_REGION_DISABLE    ( 0x01UL << 11U )
#define mpuREGION_5TH_SUB_REGION_DISABLE    ( 0x01UL << 12U )
#define mpuREGION_6TH_SUB_REGION_DISABLE    ( 0x01UL << 13U )
#define mpuREGION_7TH_SUB_REGION_DISABLE    ( 0x01UL << 14U )
#define mpuREGION_8TH_SUB_REGION_DISABLE    ( 0x01UL << 15U )

/* Region Data Access Permission field bit definitions */
#define mpuREGION_PRIVILEGED_NO_ACCESS_USER_NO_ACCESS       ( 0x00UL << 24U )
#define mpuREGION_PRIVILEGED_READ_WRITE_USER_NO_ACCESS      ( 0x01UL << 24U )
#define mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_ONLY      ( 0x02UL << 24U )
#define mpuREGION_PRIVILEGED_READ_WRITE_USER_READ_WRITE     ( 0x03UL << 24U )
#define mpuREGION_PRIVILEGED_READ_ONLY_USER_NO_ACCESS       ( 0x05UL << 24U )
#define mpuREGION_PRIVILEGED_READ_ONLY_USER_READ_ONLY       ( 0x06UL << 24U )

/* Region Type extension, Cached and Buffered field definitions */
#define mpuREGION_STRONGLY_ORDERED                                                                  ( 0x00UL << 16U )
#define mpuREGION_SHARED_DEVICE                                                                     ( 0x01UL << 16U )
#define mpuREGION_OUTER_AND_INNER_WRITE_THROUGH_NO_WRITE_ALLOCATE                                   ( 0x02UL << 16U )
#define mpuREGION_OUTER_AND_INNER_WRITE_BACK_NO_WRITE_ALLOCATE                                      ( 0x03UL << 16U )
#define mpuREGION_OUTER_AND_INNER_NONCACHEABLE                                                      ( 0x08UL << 16U )
#define mpuREGION_OUTER_AND_INNER_WRITE_BACK_WRITE_AND_READ_ALLOCATE                                ( 0x0BUL << 16U )
#define mpuREGION_NONSHARED_DEVICE                                                                  ( 0x10UL << 16U )
#define mpuREGION_OUTER_NONCACHEABLE_INNER_WRITE_BACK_WRITE_AND_READ_ALLOCATE                       ( 0x21UL << 16U )
#define mpuREGION_OUTER_NONCACHEABLE_INNER_WRITE_THROUGH_NO_WRITE_ALLOCATE                          ( 0x22UL << 16U )
#define mpuREGION_OUTER_NONCACHEABLE_INNER_WRITE_BACK_NO_WRITE_ALLOCATE                             ( 0x23UL << 16U )
#define mpuREGION_OUTER_WRITE_BACK_WRITE_AND_READ_ALLOCATE_INNER_NONCACHEABLE                       ( 0x28UL << 16U )
#define mpuREGION_OUTER_WRITE_BACK_WRITE_AND_READ_ALLOCATE_INNER_WRITE_THROUGH_NO_WRITE_ALLOCATE    ( 0x2AUL << 16U )
#define mpuREGION_OUTER_WRITE_BACK_WRITE_AND_READ_ALLOCATE_INNER_WRITE_BACK_NO_WRITE_ALLOCATE       ( 0x2BUL << 16U )
#define mpuREGION_OUTER_WRITE_THROUGH_NO_WRITE_ALLOCATE_INNER_NONCACHEABLE                          ( 0x30UL << 16U )
#define mpuREGION_OUTER_WRITE_THROUGH_NO_WRITE_ALLOCATE_INNER_WRITE_BACK_WRITE_AND_READ_ALLOCATE    ( 0x31UL << 16U )
#define mpuREGION_OUTER_WRITE_THROUGH_NO_WRITE_ALLOCATE_INNER_WRITE_BACK_NO_WRITE_ALLOCATE          ( 0x33UL << 16U )
#define mpuREGION_OUTER_WRITE_BACK_NO_WRITE_ALLOCATE_INNER_NONCACHEABLE                             ( 0x38UL << 16U )
#define mpuREGION_OUTER_WRITE_BACK_NO_WRITE_ALLOCATE_INNER_WRITE_BACK_WRITE_AND_READ_ALLOCATE       ( 0x39UL << 16U )
#define mpuREGION_OUTER_WRITE_BACK_NO_WRITE_ALLOCATE_INNER_WRITE_THROUGH_NO_WRITE_ALLOCATE          ( 0x3AUL << 16U )

/* These define the default cache policies for the memory regions as used by the default memory map. */
#define mpuREGION_INTERNAL_FLASH_DEFAULT_CACHE_POLICY   ( mpuREGION_OUTER_AND_INNER_WRITE_THROUGH_NO_WRITE_ALLOCATE )
#define mpuREGION_INTERNAL_RAM_DEFAULT_CACHE_POLICY     ( mpuREGION_OUTER_AND_INNER_WRITE_BACK_WRITE_AND_READ_ALLOCATE )
#define mpuREGION_PERIPHERAL_DEFAULT_CACHE_POLICY       ( mpuREGION_NONSHARED_DEVICE )


/*-----------------------------------------------------------------------------
 * MPU Structure Definitions
 *---------------------------------------------------------------------------*/

/* Defines MPU Region Memory Settings Structure */
typedef struct mpuRegionConfig
{
    void            *pvBaseAddress;
    portUInt32Type  ulLengthInBytes;
    portUInt32Type  ulAccessPermissions;
    portUInt32Type  ulSubRegionControl;

} mpuRegionConfigType;

/* Defines MPU Task Parameter Settings Structure */
/* Set of memory ranges allocated to a task when an MPU
 * is used along with the privilege level of the task. */
typedef struct mpuTaskParam
{
    portUnsignedBaseType    uxPrivilegeLevel;
    mpuRegionConfigType     axRegions[ mpuCONFIGURABLE_REGION_NUM ];

} mpuTaskParamType;

/* MPU region register settings - holds actual MPU region values. */
typedef struct mpuRegionRegisters
{
    portUInt32Type ulRegionBaseAddress;
    portUInt32Type ulRegionAttribute;

} mpuRegionRegistersType;


/*-----------------------------------------------------------------------------
 * MPU API Functions
 *---------------------------------------------------------------------------*/

API_FUNCTION portBaseType xMPUSetTaskRegions( portTaskHandleType pxSetTaskToModify,
                                              const mpuTaskParamType * const pxSetTaskMpuParams );

API_FUNCTION void vMPUTaskExecuteInUnprivilegedMode( void );

API_INIT_FUNCTION portBaseType xMPUConfigureGlobalRegion( portUInt32Type ulRegionNumber,
                                                          portUInt32Type ulRegionBeginAddress,
                                                          portUInt32Type ulRegionAccess,
                                                          portUInt32Type ulRegionSize,
                                                          portUInt32Type ulSubRegionDisable );


/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* API_MPU_H */
