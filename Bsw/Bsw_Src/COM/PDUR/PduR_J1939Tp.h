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
**  FILENAME    : PduR_J1939Tp.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of PDUR                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_J1939TP_H
#define  PDUR_J1939TP_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_J1939TP_H_AR_MAJOR_VERSION      4u
#define PDUR_J1939TP_H_AR_MINOR_VERSION      2u
#define PDUR_J1939TP_H_AR_PATCH_VERSION      2u
#define PDUR_J1939TP_H_SW_MAJOR_VERSION      2u
#define PDUR_J1939TP_H_SW_MINOR_VERSION      0u
#define PDUR_J1939TP_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_J1939TP_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_J1939TpStartOfReception     J1939Dcm_StartOfReception
#define PduR_J1939TpCopyRxData           J1939Dcm_CopyRxData
#define PduR_J1939TpRxIndication         J1939Dcm_TpRxIndication
#define PduR_J1939TpCopyTxData           J1939Dcm_CopyTxData
#define PduR_J1939TpTxConfirmation       J1939Dcm_TpTxConfirmation
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_J1939TpStartOfReception     PduR_TpStartOfReception
#define PduR_J1939TpCopyRxData           PduR_TpCopyRxData
#define PduR_J1939TpRxIndication         PduR_TpRxIndication
#define PduR_J1939TpCopyTxData           PduR_TpCopyTxData
#define PduR_J1939TpTxConfirmation       PduR_TpTxConfirmation

#endif/*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_J1939TP_SUPPORT*/

#endif /* end of PDUR_J1939TP_H */

