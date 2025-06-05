/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2020)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    : LdCom.h                                                    **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_H_
#define LDCOM_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "LdCom_Types.h"
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
#define LDCOM_MODULE_ID                                     49u
#define LDCOM_VENDOR_ID                                     62u
#define LDCOM_INSTANCE_ID                                   0u
/*Published information elements*/
#define LDCOM_H_AR_MAJOR_VERSION                            4u
#define LDCOM_H_AR_MINOR_VERSION                            2u
#define LDCOM_H_AR_PATCH_VERSION                            2u
#define LDCOM_H_SW_MAJOR_VERSION                            2u
#define LDCOM_H_SW_MINOR_VERSION                            0u
#define LDCOM_H_SW_PATCH_VERSION                            1u

#define LDCOM_NOUSED(paramter)                              (void)(paramter)
/*******************************************************************************
**                      Public API Declarations                               **
*******************************************************************************/
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"

extern FUNC(void, LDCOM_CODE)
LdCom_Init(
    P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_DATA) config
);

extern FUNC(void, LDCOM_CODE)
LdCom_DeInit(void);

#if (STD_ON == LDCOM_VERSION_INFO_API)
extern FUNC(void, LDCOM_CODE)
LdCom_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo
);
#endif/* STD_ON == LDCOM_VERSION_INFO_API */


extern FUNC(Std_ReturnType, LDCOM_CODE)
LdCom_Transmit(
    PduIdType Id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);
#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define LDCOM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "LdCom_MemMap.h"
extern CONST(LdCom_ConfigType, LDCOM_CONST) LdCom_InitCfgSet;
#define LDCOM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "LdCom_MemMap.h"

#endif/* LDCOM_H_ */

