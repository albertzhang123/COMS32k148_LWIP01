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
**  FILENAME    : PduR_LinTp.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to LINTP                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_LINTP_H
#define  PDUR_LINTP_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_LINTP_H_AR_MAJOR_VERSION      4u
#define PDUR_LINTP_H_AR_MINOR_VERSION      2u
#define PDUR_LINTP_H_AR_PATCH_VERSION      2u
#define PDUR_LINTP_H_SW_MAJOR_VERSION      2u
#define PDUR_LINTP_H_SW_MINOR_VERSION      0u
#define PDUR_LINTP_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_LINTP_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_LinTpStartOfReception     Dcm_StartOfReception
#define PduR_LinTpCopyRxData           Dcm_CopyRxData
#define PduR_LinTpRxIndication         Dcm_TpRxIndication
#define PduR_LinTpCopyTxData           Dcm_CopyTxData
#define PduR_LinTpTxConfirmation       Dcm_TpTxConfirmation
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_LinTpStartOfReception     PduR_TpStartOfReception
#define PduR_LinTpCopyRxData           PduR_TpCopyRxData
#define PduR_LinTpRxIndication         PduR_TpRxIndication
#define PduR_LinTpCopyTxData           PduR_TpCopyTxData
#define PduR_LinTpTxConfirmation       PduR_TpTxConfirmation

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_LINTP_SUPPORT*/

#endif /* end of PDUR_LINTP_H */


