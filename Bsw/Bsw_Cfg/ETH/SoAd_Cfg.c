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
**  FILENAME    : SoAd_Cfg.c                                                  **
**                                                                            **
**  Created on  : 2022/06/02 17:33:12                                         **
**  Author      : zhengfei.li                                                 **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR classic Platform 4.2.2                         **
**                                                                            **
*******************************************************************************/
#include "SoAd_Types.h"
#include "PduR_SoAd.h"        
//#include "UdpNm_Cbk.h"

#define SOAD_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_PcCfgType,SOAD_CONST) SoAd_PcCfg =
{
    {
        {           
           PduR_SoAdIfRxIndication,
           PduR_SoAdIfTriggerTransmit,
           PduR_SoAdIfTxConfirmation,
           PduR_SoAdTpStartOfReception,
           PduR_SoAdTpCopyRxData,
           PduR_SoAdTpRxIndication,
           PduR_SoAdTpCopyTxData,
           PduR_SoAdTpTxConfirmation,
           NULL_PTR,
           NULL_PTR
        },
        /*{
            UdpNm_SoAdIfRxIndication,
            UdpNm_SoAdIfTriggerTransmit,
           UdpNm_SoAdIfTxConfirmation,
           NULL_PTR,
           NULL_PTR,
           NULL_PTR,
           NULL_PTR,
           NULL_PTR,
           NULL_PTR,
           NULL_PTR
        },*/
    }
};
#define SOAD_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"




