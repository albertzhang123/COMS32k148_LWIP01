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
**  FILENAME    : PduR_SomeIpTp.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to SOMEIPTP                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_SOMEIPTP_H
#define  PDUR_SOMEIPTP_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_SOMEIPTP_H_AR_MAJOR_VERSION      4u
#define PDUR_SOMEIPTP_H_AR_MINOR_VERSION      2u
#define PDUR_SOMEIPTP_H_AR_PATCH_VERSION      2u
#define PDUR_SOMEIPTP_H_SW_MAJOR_VERSION      2u
#define PDUR_SOMEIPTP_H_SW_MINOR_VERSION      0u
#define PDUR_SOMEIPTP_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_SOMEIPTP_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_SomeIpTpStartOfReception   LdCom_StartOfReception
#define PduR_SomeIpTpCopyRxData         LdCom_CopyRxData
#define PduR_SomeIpTpRxIndication       LdCom_TpRxIndication
#define PduR_SomeIpTpCopyTxData         LdCom_CopyTxData
#define PduR_SomeIpTpTxConfirmation     LdCom_TpTxConfirmation
#define PduR_SomeIpTpTransmit           SoAd_Transmit
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_SomeIpTpStartOfReception    PduR_TpStartOfReception
#define PduR_SomeIpTpCopyRxData          PduR_TpCopyRxData
#define PduR_SomeIpTpRxIndication        PduR_TpRxIndication
#define PduR_SomeIpTpCopyTxData          PduR_TpCopyTxData
#define PduR_SomeIpTpTxConfirmation      PduR_TpTxConfirmation
#define PduR_SomeIpTpTransmit            PduR_Transmit

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_SOMEIPTP_SUPPORT*/

#endif /* end of PDUR_SOMEIPTP_H */


