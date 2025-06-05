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
**  FILENAME    : PduR_SecOC.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Call-back of PDUR for SecOC                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_SECOC_H
#define PDUR_SECOC_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_SECOC_H_AR_MAJOR_VERSION      4u
#define PDUR_SECOC_H_AR_MINOR_VERSION      2u
#define PDUR_SECOC_H_AR_PATCH_VERSION      2u
#define PDUR_SECOC_H_SW_MAJOR_VERSION      2u
#define PDUR_SECOC_H_SW_MINOR_VERSION      0u
#define PDUR_SECOC_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_SECOC_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#else
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_SecOCTransmit           PduR_Transmit
#define PduR_SecOCCancelTransmit     PduR_CancelTransmit

#define PduR_SecOCIfRxIndication       PduR_IfRxIndication
#define PduR_SecOCIfTxConfirmation     PduR_IfTxConfirmation

#define PduR_SecOCCancelReceive        PduR_CancelReceive
#define PduR_SecOCTpCopyRxData         PduR_TpCopyRxData
#define PduR_SecOCTpCopyTxData         PduR_TpCopyTxData
#define PduR_SecOCTpRxIndication       PduR_TpRxIndication
#define PduR_SecOCTpStartOfReception   PduR_TpStartOfReception
#define PduR_SecOCTpTxConfirmation     PduR_TpTxConfirmation

#endif /*STD_OFF == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_SECOC_SUPPORT*/

#endif  /* end of PDUR_SECOC_H */
