
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
*******************************************************************************
**                                                                           **
**  FILENAME    : LdCom_Cfg.c                                                **
**  Created on  : 2022/06/02 13:43:03                                        **
**  AUTHOR      : zhengfei.li                                                **
**  VENDOR      :                                                            **
**  DESCRIPTION :This file contain configuration information of LdCom module.**
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
******************************************************************************/
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "LdCom_Types.h"
#include "Stub_LdCom.h"

#define LDCOM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "LdCom_MemMap.h"
CONST(LdCom_IPduType,LDCOM_CONST) LdComIPduCfg[LDCOM_IPDU_NUMBER_MAX] =
{
    {   
        LdComIPdu_EthUdpTx,
        PDUR_SRCPDU_LDCOM_ETH_TX,
        &Stub_LdComEthUdpTxConfirmation,
        NULL_PTR,
    },
    {   
        LdComIPdu_EthUdpRx,
        LDCOM_UNUSED_UINT16,
        NULL_PTR,
        &Stub_LdComEthUdpRxIndication,
    },
};
#define LDCOM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "LdCom_MemMap.h"
CONST(LdCom_ConfigType,LDCOM_CONST) LdCom_InitCfgSet =
{
    &LdComIPduCfg[0u],
};
#define LDCOM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "LdCom_MemMap.h"
