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
**  FILENAME    : PduR_SoAd.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to SOAD                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_SOAD_H
#define  PDUR_SOAD_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_SOAD_H_AR_MAJOR_VERSION      4u
#define PDUR_SOAD_H_AR_MINOR_VERSION      2u
#define PDUR_SOAD_H_AR_PATCH_VERSION      2u
#define PDUR_SOAD_H_SW_MAJOR_VERSION      2u
#define PDUR_SOAD_H_SW_MINOR_VERSION      0u
#define PDUR_SOAD_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_SOAD_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_SoAdTpStartOfReception   Com_StartOfReception
#define PduR_SoAdTpCopyRxData         Com_CopyRxData
#define PduR_SoAdTpRxIndication       Com_TpRxIndication
#define PduR_SoAdTpCopyTxData         Com_CopyTxData
#define PduR_SoAdTpTxConfirmation     Com_TpTxConfirmation
#define PduR_SoAdIfRxIndication       Com_RxIndication
#define PduR_SoAdIfTxConfirmation     Com_TxConfirmation
#define PduR_SoAdIfTriggerTransmit    Com_TriggerTransmit
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_SoAdIfRxIndication           PduR_IfRxIndication
#define PduR_SoAdIfTxConfirmation         PduR_IfTxConfirmation
#define PduR_SoAdIfTriggerTransmit        PduR_IfTriggerTransmit

#define PduR_SoAdTpStartOfReception       PduR_TpStartOfReception
#define PduR_SoAdTpCopyRxData             PduR_TpCopyRxData
#define PduR_SoAdTpRxIndication           PduR_TpRxIndication
#define PduR_SoAdTpCopyTxData             PduR_TpCopyTxData
#define PduR_SoAdTpTxConfirmation         PduR_TpTxConfirmation

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/

#endif /*STD_ON == PDUR_SOAD_SUPPORT*/

#endif /* end of PDUR_SOAD_H */


