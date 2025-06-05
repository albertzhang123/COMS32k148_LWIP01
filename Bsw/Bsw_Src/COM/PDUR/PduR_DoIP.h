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
**  FILENAME    : PduR_DoIP.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to DOIP                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_DOIP_H
#define  PDUR_DOIP_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_DOIP_H_AR_MAJOR_VERSION      4u
#define PDUR_DOIP_H_AR_MINOR_VERSION      2u
#define PDUR_DOIP_H_AR_PATCH_VERSION      2u
#define PDUR_DOIP_H_SW_MAJOR_VERSION      2u
#define PDUR_DOIP_H_SW_MINOR_VERSION      0u
#define PDUR_DOIP_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_DOIP_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_DoIPTpStartOfReception     Dcm_StartOfReception
#define PduR_DoIPTpCopyRxData           Dcm_CopyRxData
#define PduR_DoIPTpRxIndication         Dcm_TpRxIndication
#define PduR_DoIPTpCopyTxData           Dcm_CopyTxData
#define PduR_DoIPTpTxConfirmation       Dcm_TpTxConfirmation
#define PduR_DoIPIfTxConfirmation       Dcm_TxConfirmation
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_DoIPTpStartOfReception     PduR_TpStartOfReception
#define PduR_DoIPTpCopyRxData           PduR_TpCopyRxData
#define PduR_DoIPTpRxIndication         PduR_TpRxIndication
#define PduR_DoIPTpCopyTxData           PduR_TpCopyTxData
#define PduR_DoIPTpTxConfirmation       PduR_TpTxConfirmation
#define PduR_DoIPIfTxConfirmation       PduR_IfTxConfirmation

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_DOIP_SUPPORT*/

#endif /* end of PDUR_DOIP_H */


