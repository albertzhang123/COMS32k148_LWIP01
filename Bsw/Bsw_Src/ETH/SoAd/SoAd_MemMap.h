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
**  FILENAME    : SoAd_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of CanIf             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define SOAD_MEMMAP_VENDOR_ID  62u
#define SOAD_MEMMAP_MODULE_ID  56u
#define SOAD_MEMMAP_AR_MAJOR_VERSION  4u
#define SOAD_MEMMAP_AR_MINOR_VERSION  2u
#define SOAD_MEMMAP_AR_PATCH_VERSION  2u
#define SOAD_MEMMAP_SW_MAJOR_VERSION  2u
#define SOAD_MEMMAP_SW_MINOR_VERSION  0u
#define SOAD_MEMMAP_SW_PATCH_VERSION  0u
#define SOAD_MEMMAP_VENDOR_API_INFIX  0u


#if defined SOAD_START_SEC_CODE
    #undef SOAD_START_SEC_CODE
    #define START_SEC_CODE
#elif defined SOAD_STOP_SEC_CODE
    #undef SOAD_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined SOAD_START_SEC_SOADRXINDICATION_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADRXINDICATION_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_SOADCOPYTXDATA_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADCOPYTXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADCOPYTXDATA_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADCOPYTXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_SOADTCPACCEPTED_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADTCPACCEPTED_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADTCPACCEPTED_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADTCPACCEPTED_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_SOADTCPCONNECTED_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADTCPCONNECTED_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADTCPCONNECTED_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADTCPCONNECTED_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_SOADTCPIPEVENT_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADTCPIPEVENT_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADTCPIPEVENT_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADTCPIPEVENT_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
    #undef SOAD_START_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined SOAD_STOP_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
    #undef SOAD_STOP_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined SOAD_START_SEC_VAR_INIT_8
    #undef SOAD_START_SEC_VAR_INIT_8
    #define START_SEC_VAR_INIT_8
#elif defined SOAD_STOP_SEC_VAR_INIT_8
    #undef SOAD_STOP_SEC_VAR_INIT_8
    #define STOP_SEC_VAR_INIT_8

#elif defined SOAD_START_SEC_VAR_INIT_PTR
    #undef SOAD_START_SEC_VAR_INIT_PTR
    #define START_SEC_VAR_INIT_PTR
#elif defined SOAD_STOP_SEC_VAR_INIT_PTR
    #undef SOAD_STOP_SEC_VAR_INIT_PTR
    #define STOP_SEC_VAR_INIT_PTR

#elif defined SOAD_START_SEC_VAR_NO_INIT_8
    #undef SOAD_START_SEC_VAR_NO_INIT_8
    #define START_SEC_VAR_NO_INIT_8
#elif defined SOAD_STOP_SEC_VAR_NO_INIT_8
    #undef SOAD_STOP_SEC_VAR_NO_INIT_8
    #define STOP_SEC_VAR_NO_INIT_8

#elif defined SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #define START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONST_PBCFG_UNSPECIFIED
#elif defined SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONST_PBCFG_UNSPECIFIED

#elif defined SOAD_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef SOAD_START_SEC_CONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined SOAD_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef SOAD_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined SOAD_START_SEC_PBCONFIG_DATA_32
    #undef SOAD_START_SEC_PBCONFIG_DATA_32
    #define START_SEC_CONST_PBCFG_32
#elif defined SOAD_STOP_SEC_PBCONFIG_DATA_32
    #undef SOAD_STOP_SEC_PBCONFIG_DATA_32
    #define STOP_SEC_CONST_PBCFG_32

#elif defined SOAD_START_SEC_PBCONFIG_DATA_16
    #undef SOAD_START_SEC_PBCONFIG_DATA_16
    #define START_SEC_CONST_PBCFG_16
#elif defined SOAD_STOP_SEC_PBCONFIG_DATA_16
    #undef SOAD_STOP_SEC_PBCONFIG_DATA_16
    #define STOP_SEC_CONST_PBCFG_16

#elif defined SOAD_START_SEC_PBCONFIG_DATA_8
    #undef SOAD_START_SEC_PBCONFIG_DATA_8
    #define START_SEC_CONST_PBCFG_8
#elif defined SOAD_STOP_SEC_PBCONFIG_DATA_8
    #undef SOAD_STOP_SEC_PBCONFIG_DATA_8
    #define STOP_SEC_CONST_PBCFG_8

#elif defined SOAD_START_SEC_PBCONFIG_DATA_BOOLEAN
    #undef SOAD_START_SEC_PBCONFIG_DATA_BOOLEAN
    #define START_SEC_CONST_PBCFG_BOOLEAN
#elif defined SOAD_STOP_SEC_PBCONFIG_DATA_BOOLEAN
    #undef SOAD_STOP_SEC_PBCONFIG_DATA_BOOLEAN
    #define STOP_SEC_CONST_PBCFG_BOOLEAN


#else
#error "SoAd not include this section"
#endif
#include "MemMap.h"

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/

