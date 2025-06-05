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
**  FILENAME    : MemIf_Cfg.h                                                 **
**                                                                            **
**  Created on  : 2021/12/28 09:45:58                                         **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : MemIf pre compile globle macro define                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11/4.2.2                **
**                                                                            **
*******************************************************************************/	
#ifndef MEMIF_CFG_H
#define MEMIF_CFG_H

#include "MemIf_Types.h"

#define MEMIF_FEE_MAP_SUPPORT     STD_ON
#define MEMIF_EA_MAP_SUPPORT      STD_OFF

#define MEMIF_NUMBER_OF_DEVICES     (1u)

#define MEMIF_VERSION_INFO_API  (STD_ON)
#define MEMIF_CFG_USE_FCT_API   (STD_ON)

/* preprocessor switch for DET */
#define MEMIF_DEV_ERROR_DETECT (STD_OFF)

/*---- Include of the MemHwAs ---------------------------*/
#if (MEMIF_FEE_MAP_SUPPORT == STD_ON)
#include "Fee.h"
#endif

#if (MEMIF_EA_MAP_SUPPORT == STD_ON)
#include "Ea.h"
#endif

#define MEMIF_START_SEC_CONST_8BIT
#include "MemIf_MemMap.h"
extern CONST(uint8, MEMIF_CONST) MemIf_NumberOfDevices_u8;
#define MEMIF_STOP_SEC_CONST_8BIT
#include "MemIf_MemMap.h"

#define MEMIF_START_SEC_CONST_32BIT
#include "MemIf_MemMap.h"
extern CONST(MemIf_MemHwAApi_Type, MEMIF_CONST) MemIf_MemHwaApis_at[MEMIF_NUMBER_OF_DEVICES];
#define MEMIF_STOP_SEC_CONST_32BIT
#include "MemIf_MemMap.h"


#endif
