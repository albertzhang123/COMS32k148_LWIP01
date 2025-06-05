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
**  FILENAME    : PduR_CanTp.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to CANTP                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_CANTP_H
#define  PDUR_CANTP_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_CANTP_H_AR_MAJOR_VERSION      4u
#define PDUR_CANTP_H_AR_MINOR_VERSION      2u
#define PDUR_CANTP_H_AR_PATCH_VERSION      2u
#define PDUR_CANTP_H_SW_MAJOR_VERSION      2u
#define PDUR_CANTP_H_SW_MINOR_VERSION      0u
#define PDUR_CANTP_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_CANTP_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_CanTpStartOfReception   Dcm_StartOfReception
#define PduR_CanTpCopyRxData         Dcm_CopyRxData
#define PduR_CanTpRxIndication       Dcm_TpRxIndication
#define PduR_CanTpCopyTxData         Dcm_CopyTxData
#define PduR_CanTpTxConfirmation     Dcm_TpTxConfirmation
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_CanTpStartOfReception    PduR_TpStartOfReception
#define PduR_CanTpCopyRxData          PduR_TpCopyRxData
#define PduR_CanTpRxIndication        PduR_TpRxIndication
#define PduR_CanTpCopyTxData          PduR_TpCopyTxData
#define PduR_CanTpTxConfirmation      PduR_TpTxConfirmation

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_CANTP_SUPPORT*/

#endif /* end of PDUR_CANTP_H */


