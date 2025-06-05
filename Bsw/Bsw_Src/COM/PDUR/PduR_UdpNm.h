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
**  FILENAME    : PduR_UdpNm.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : If COM user data support is enabled for UDPNM               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef PDUR_UDPNM_H
#define PDUR_UDPNM_H
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_UDPNM_H_AR_MAJOR_VERSION      4u
#define PDUR_UDPNM_H_AR_MINOR_VERSION      2u
#define PDUR_UDPNM_H_AR_PATCH_VERSION      2u
#define PDUR_UDPNM_H_SW_MAJOR_VERSION      2u
#define PDUR_UDPNM_H_SW_MINOR_VERSION      0u
#define PDUR_UDPNM_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_UDPNM_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_UdpNmRxIndication       Com_RxIndication
#define PduR_UdpNmTxConfirmation     Com_TxConfirmation
#define PduR_UdpNmTriggerTransmit    Com_TriggerTransmit
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_UdpNmRxIndication         PduR_IfRxIndication
#define PduR_UdpNmTxConfirmation       PduR_IfTxConfirmation
#define PduR_UdpNmTriggerTransmit      PduR_IfTriggerTransmit

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_UDPNM_SUPPORT*/

#endif /* PDUR_UDPNM_H */
