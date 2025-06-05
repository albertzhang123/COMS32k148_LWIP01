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
**  FILENAME    : EthSM_Cfg.c                                                 **
**                                                                            **
**  Created on  : 2022/06/02 14:32:14                                         **
**  Author      :                                                             **
**  Vendor      :                                                             **
**  DESCRIPTION : build configuration parameter of ETHSM                      **
**                                                                            **
**  SPECIFICATION(S) :  AUTOSAR  classic  Platform  R19-11                    **
**                                                                            **
*******************************************************************************/

#include "EthSM_Types.h"
#include "EthSM_Cfg.h"

#define ETHSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthSM_MemMap.h"


CONST( EthSM_NetworkType,ETHSM_CONST)EthSM_Network[ETHSM_CHANNEL_NUM] =
{
    {
        0x00, /* EthSMChannelId */
        0x04, /* EthSMComMNetworkHandleRef */
        0x00 /* EthSMEthIfControllerRef */
    }
};

CONST(EthSM_ConfigType,ETHSM_CONST) EthSM_Config = 
{
    EthSM_Network /* EthSMNetwork */
};

#define ETHSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthSM_MemMap.h"
