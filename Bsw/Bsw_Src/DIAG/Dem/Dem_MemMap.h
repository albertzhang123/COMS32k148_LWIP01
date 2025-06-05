/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2020)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Dem_MemMap.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Memory mapping abstraction declaration of DEM               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2018-4-20  tao.yu    Initial version                         */
/*  V1.0.1       2019-9-17  tao.yu    fix some bug,change event callback      */
/*  V1.0.2       2019-12-25  tao.yu    QAC fix     							  */
/*  V1.0.3       2020-1-7   tao.yu    Commercial project problem modification */
/*  V1.0.4       2020-3-10   tao.yu    enable condition has no impact on      */
/* Dem_ResetEventDebounceStatus,  Dem_ResetEventStatus and Dem_<...>ClearDTC. */
/* 	V2.0.0		 2020-07-15  pengfei.zhu	fix bugs by PIM; optimized code   */
/* 	V2.0.1		 2020-09-15  pengfei.zhu	QAC test 						  */
/*******************************************************************************
**                       Version  information                                 **
*******************************************************************************/
#if defined DEM_START_SEC_CODE
    #undef DEM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined DEM_STOP_SEC_CODE
    #undef DEM_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined DEM_START_SEC_CALLOUT_CODE
    #undef DEM_START_SEC_CALLOUT_CODE
    #define START_SEC_CALLOUT_CODE
#elif defined DEM_STOP_SEC_CALLOUT_CODE
    #undef DEM_STOP_SEC_CALLOUT_CODE
    #define STOP_SEC_CALLOUT_CODE

#elif defined DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #undef DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #undef DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define STOP_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined DEM_START_SEC_VAR_CLEARED_PTR
    #undef DEM_START_SEC_VAR_CLEARED_PTR
    #define START_SEC_VAR_CLEARED_PTR
#elif defined DEM_STOP_SEC_VAR_CLEARED_PTR
    #undef DEM_STOP_SEC_VAR_CLEARED_PTR
    #define STOP_SEC_VAR_CLEARED_PTR

#elif defined DEM_START_SEC_VAR_CLEARED_32
    #undef DEM_START_SEC_VAR_CLEARED_32
    #define START_SEC_VAR_CLEARED_32
#elif defined DEM_STOP_SEC_VAR_CLEARED_32
    #undef DEM_STOP_SEC_VAR_CLEARED_32
    #define STOP_SEC_VAR_CLEARED_32

#elif defined DEM_START_SEC_VAR_CLEARED_16
    #undef DEM_START_SEC_VAR_CLEARED_16
    #define START_SEC_VAR_CLEARED_16
#elif defined DEM_STOP_SEC_VAR_CLEARED_16
    #undef DEM_STOP_SEC_VAR_CLEARED_16
    #define STOP_SEC_VAR_CLEARED_16

#elif defined DEM_START_SEC_VAR_CLEARED_8
    #undef DEM_START_SEC_VAR_CLEARED_8
    #define START_SEC_VAR_CLEARED_8
#elif defined DEM_STOP_SEC_VAR_CLEARED_8
    #undef DEM_STOP_SEC_VAR_CLEARED_8
    #define STOP_SEC_VAR_CLEARED_8

#elif defined DEM_START_SEC_VAR_CLEARED_BOOLEAN
    #undef DEM_START_SEC_VAR_CLEARED_BOOLEAN
    #define START_SEC_VAR_CLEARED_BOOLEAN
#elif defined DEM_STOP_SEC_VAR_CLEARED_BOOLEAN
    #undef DEM_STOP_SEC_VAR_CLEARED_BOOLEAN
    #define STOP_SEC_VAR_CLEARED_BOOLEAN
/*Init data(clear)*/
#elif defined DEM_START_SEC_VAR_INIT_UNSPECIFIED
    #undef DEM_START_SEC_VAR_INIT_UNSPECIFIED
    #define START_SEC_VAR_INIT_UNSPECIFIED
#elif defined DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_INIT_UNSPECIFIED

#elif defined DEM_START_SEC_VAR_INIT_PTR
    #undef DEM_START_SEC_VAR_INIT_PTR
    #define START_SEC_VAR_INIT_PTR
#elif defined DEM_STOP_SEC_VAR_INIT_PTR
    #undef DEM_STOP_SEC_VAR_INIT_PTR
    #define STOP_SEC_VAR_INIT_PTR

#elif defined DEM_START_SEC_VAR_INIT_32
    #undef DEM_START_SEC_VAR_INIT_32
    #define START_SEC_VAR_INIT_32
#elif defined DEM_STOP_SEC_VAR_INIT_32
    #undef DEM_STOP_SEC_VAR_INIT_32
    #define STOP_SEC_VAR_INIT_32

#elif defined DEM_START_SEC_VAR_INIT_16
    #undef DEM_START_SEC_VAR_INIT_16
    #define START_SEC_VAR_INIT_16
#elif defined DEM_STOP_SEC_VAR_INIT_16
    #undef DEM_STOP_SEC_VAR_INIT_16
    #define STOP_SEC_VAR_INIT_16

#elif defined DEM_START_SEC_VAR_INIT_8
    #undef DEM_START_SEC_VAR_INIT_8
    #define START_SEC_VAR_INIT_8
#elif defined DEM_STOP_SEC_VAR_INIT_8
    #undef DEM_STOP_SEC_VAR_INIT_8
    #define STOP_SEC_VAR_INIT_8

#elif defined DEM_START_SEC_VAR_INIT_BOOLEAN
    #undef DEM_START_SEC_VAR_INIT_BOOLEAN
    #define START_SEC_VAR_INIT_BOOLEAN
#elif defined DEM_STOP_SEC_VAR_INIT_BOOLEAN
    #undef DEM_STOP_SEC_VAR_INIT_BOOLEAN
    #define STOP_SEC_VAR_INIT_BOOLEAN
/*No-Init data(clear)*/
#elif defined DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #define START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined DEM_START_SEC_VAR_NO_INIT_PTR
    #undef DEM_START_SEC_VAR_NO_INIT_PTR
    #define START_SEC_VAR_NO_INIT_PTR
#elif defined DEM_STOP_SEC_VAR_NO_INIT_PTR
    #undef DEM_STOP_SEC_VAR_NO_INIT_PTR
    #define STOP_SEC_VAR_NO_INIT_PTR

#elif defined DEM_START_SEC_VAR_NO_INIT_32
    #undef DEM_START_SEC_VAR_NO_INIT_32
    #define START_SEC_VAR_NO_INIT_32
#elif defined DEM_STOP_SEC_VAR_NO_INIT_32
    #undef DEM_STOP_SEC_VAR_NO_INIT_32
    #define STOP_SEC_VAR_NO_INIT_32

#elif defined DEM_START_SEC_VAR_NO_INIT_16
    #undef DEM_START_SEC_VAR_NO_INIT_16
    #define START_SEC_VAR_NO_INIT_16
#elif defined DEM_STOP_SEC_VAR_NO_INIT_16
    #undef DEM_STOP_SEC_VAR_NO_INIT_16
    #define STOP_SEC_VAR_NO_INIT_16

#elif defined DEM_START_SEC_VAR_NO_INIT_8
    #undef DEM_START_SEC_VAR_NO_INIT_8
    #define START_SEC_VAR_NO_INIT_8
#elif defined DEM_STOP_SEC_VAR_NO_INIT_8
    #undef DEM_STOP_SEC_VAR_NO_INIT_8
    #define STOP_SEC_VAR_NO_INIT_8

#elif defined DEM_START_SEC_VAR_NO_INIT_BOOLEAN
    #undef DEM_START_SEC_VAR_NO_INIT_BOOLEAN
    #define START_SEC_VAR_NO_INIT_BOOLEAN
#elif defined DEM_STOP_SEC_VAR_NO_INIT_BOOLEAN
    #undef DEM_STOP_SEC_VAR_NO_INIT_BOOLEAN
    #define STOP_SEC_VAR_NO_INIT_BOOLEAN

#elif defined DEM_START_SEC_VAR_POWER_ON_INIT_32
    #undef DEM_START_SEC_VAR_POWER_ON_INIT_32
    #define START_SEC_VAR_POWER_ON_INIT_32
#elif defined DEM_STOP_SEC_VAR_POWER_ON_INIT_32
    #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_32
    #define STOP_SEC_VAR_POWER_ON_INIT_32

#elif defined DEM_START_SEC_VAR_POWER_ON_INIT_16
    #undef DEM_START_SEC_VAR_POWER_ON_INIT_16
    #define START_SEC_VAR_POWER_ON_INIT_16
#elif defined DEM_STOP_SEC_VAR_POWER_ON_INIT_16
    #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_16
    #define STOP_SEC_VAR_POWER_ON_INIT_16

#elif defined DEM_START_SEC_VAR_POWER_ON_INIT_8
    #undef DEM_START_SEC_VAR_POWER_ON_INIT_8
    #define START_SEC_VAR_POWER_ON_INIT_8
#elif defined DEM_STOP_SEC_VAR_POWER_ON_INIT_8
    #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_8
    #define STOP_SEC_VAR_POWER_ON_INIT_8

#elif defined DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
    #undef DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
    #define START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#elif defined DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
    #undef DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
    #define STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN

#elif defined DEM_START_SEC_PBCFG_GLOBALROOT
    #undef DEM_START_SEC_PBCFG_GLOBALROOT
    #define START_SEC_PBCFG_GLOBALROOT
#elif defined DEM_STOP_SEC_PBCFG_GLOBALROOT
    #undef DEM_STOP_SEC_PBCFG_GLOBALROOT
    #define STOP_SEC_PBCFG_GLOBALROOT

#elif defined DEM_START_SEC_CONST_PBCFG_UNSPECIFIED
    #undef DEM_START_SEC_CONST_PBCFG_UNSPECIFIED
    #define START_SEC_CONST_PBCFG_UNSPECIFIED
#elif defined DEM_STOP_SEC_CONST_PBCFG_UNSPECIFIED
    #undef DEM_STOP_SEC_CONST_PBCFG_UNSPECIFIED
    #define STOP_SEC_CONST_PBCFG_UNSPECIFIED

#elif defined DEM_START_SEC_CONST_PBCFG_PTR
    #undef DEM_START_SEC_CONST_PBCFG_PTR
    #define START_SEC_CONST_PBCFG_PTR
#elif defined DEM_STOP_SEC_CONST_PBCFG_PTR
    #undef DEM_STOP_SEC_CONST_PBCFG_PTR
    #define STOP_SEC_CONST_PBCFG_PTR

#elif defined DEM_START_SEC_CONST_PBCFG_32
    #undef DEM_START_SEC_CONST_PBCFG_32
    #define START_SEC_CONST_PBCFG_32
#elif defined DEM_STOP_SEC_CONST_PBCFG_32
    #undef DEM_STOP_SEC_CONST_PBCFG_32
    #define STOP_SEC_CONST_PBCFG_32

#elif defined DEM_START_SEC_CONST_PBCFG_16
    #undef DEM_START_SEC_CONST_PBCFG_16
    #define START_SEC_CONST_PBCFG_16
#elif defined DEM_STOP_SEC_CONST_PBCFG_16
    #undef DEM_STOP_SEC_CONST_PBCFG_16
    #define STOP_SEC_CONST_PBCFG_16

#elif defined DEM_START_SEC_CONST_PBCFG_8
    #undef DEM_START_SEC_CONST_PBCFG_8
    #define START_SEC_CONST_PBCFG_8
#elif defined DEM_STOP_SEC_CONST_PBCFG_8
    #undef DEM_STOP_SEC_CONST_PBCFG_8
    #define STOP_SEC_CONST_PBCFG_8

#elif defined DEM_START_SEC_CONST_PBCFG_BOOLEAN
    #undef DEM_START_SEC_CONST_PBCFG_BOOLEAN
    #define START_SEC_CONST_PBCFG_BOOLEAN
#elif defined DEM_STOP_SEC_CONST_PBCFG_BOOLEAN
    #undef DEM_STOP_SEC_CONST_PBCFG_BOOLEAN
    #define STOP_SEC_CONST_PBCFG_BOOLEAN

#elif defined DEM_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef DEM_START_SEC_CONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined DEM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef DEM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined DEM_START_SEC_CONFIG_DATA_PTR
    #undef DEM_START_SEC_CONFIG_DATA_PTR
    #define START_SEC_CONFIG_DATA_PTR
#elif defined DEM_STOP_SEC_CONFIG_DATA_PTR
    #undef DEM_STOP_SEC_CONFIG_DATA_PTR
    #define STOP_SEC_CONFIG_DATA_PTR

#elif defined DEM_START_SEC_CONFIG_DATA_32
    #undef DEM_START_SEC_CONFIG_DATA_32
    #define START_SEC_CONFIG_DATA_32
#elif defined DEM_STOP_SEC_CONFIG_DATA_32
    #undef DEM_STOP_SEC_CONFIG_DATA_32
    #define STOP_SEC_CONFIG_DATA_32

#elif defined DEM_START_SEC_CONFIG_DATA_16
    #undef DEM_START_SEC_CONFIG_DATA_16
    #define START_SEC_CONFIG_DATA_16
#elif defined DEM_STOP_SEC_CONFIG_DATA_16
    #undef DEM_STOP_SEC_CONFIG_DATA_16
    #define STOP_SEC_CONFIG_DATA_16

#elif defined DEM_START_SEC_CONFIG_DATA_8
    #undef DEM_START_SEC_CONFIG_DATA_8
    #define START_SEC_CONFIG_DATA_8
#elif defined DEM_STOP_SEC_CONFIG_DATA_8
    #undef DEM_STOP_SEC_CONFIG_DATA_8
    #define STOP_SEC_CONFIG_DATA_8

#elif defined DEM_START_SEC_CONFIG_DATA_BOOLEAN
    #undef DEM_START_SEC_CONFIG_DATA_BOOLEAN
    #define START_SEC_CONFIG_DATA_BOOLEAN
#elif defined DEM_STOP_SEC_CONFIG_DATA_BOOLEAN
    #undef DEM_STOP_SEC_CONFIG_DATA_BOOLEAN
    #define STOP_SEC_CONFIG_DATA_BOOLEAN

#elif defined DEM_START_SEC_CONST_UNSPECIFIED
    #undef DEM_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#elif defined DEM_STOP_SEC_CONST_UNSPECIFIED
    #undef DEM_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST_UNSPECIFIED

#elif defined DEM_START_SEC_CONST_PTR
    #undef DEM_START_SEC_CONST_PTR
    #define START_SEC_CONST_PTR
#elif defined DEM_STOP_SEC_CONST_PTR
    #undef DEM_STOP_SEC_CONST_PTR
    #define STOP_SEC_CONST_PTR

#elif defined DEM_START_SEC_CONST_32
    #undef DEM_START_SEC_CONST_32
    #define START_SEC_CONST_32
#elif defined DEM_STOP_SEC_CONST_32
    #undef DEM_STOP_SEC_CONST_32
    #define STOP_SEC_CONST_32

#elif defined DEM_START_SEC_CONST_16
    #undef DEM_START_SEC_CONST_16
    #define START_SEC_CONST_16
#elif defined DEM_STOP_SEC_CONST_16
    #undef DEM_STOP_SEC_CONST_16
    #define STOP_SEC_CONST_16

#elif defined DEM_START_SEC_CONST_8
    #undef DEM_START_SEC_CONST_8
    #define START_SEC_CONST_8
#elif defined DEM_STOP_SEC_CONST_8
    #undef DEM_STOP_SEC_CONST_8
    #define STOP_SEC_CONST_8

#elif defined DEM_START_SEC_CONST_BOOLEAN
    #undef DEM_START_SEC_CONST_BOOLEAN
    #define START_SEC_CONST_BOOLEAN
#elif defined DEM_STOP_SEC_CONST_BOOLEAN
    #undef DEM_STOP_SEC_CONST_BOOLEAN
    #define STOP_SEC_CONST_BOOLEAN

#else
#error "Dem not include this section"
#endif

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DEM_MEMMAP_AR_MAJOR_VERSION  	 4U
#define DEM_MEMMAP_AR_MINOR_VERSION 	 2U
#define DEM_MEMMAP_AR_PATCH_VERSION  	 2U
#define DEM_MEMMAP_SW_MAJOR_VERSION 	 2U
#define DEM_MEMMAP_SW_MINOR_VERSION 	 0U
#define DEM_MEMMAP_SW_PATCH_VERSION 	 1U

/*=======[M E M M A P  S Y M B O L  D E F I N E]==============================*/

#include "MemMap.h"

