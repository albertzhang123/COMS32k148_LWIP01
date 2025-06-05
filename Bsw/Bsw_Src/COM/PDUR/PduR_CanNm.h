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
**  FILENAME    : PduR_CanNm.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : If COM user data support is enabled for CANNM               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef PDUR_CANNM_H
#define PDUR_CANNM_H
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_CANNM_H_AR_MAJOR_VERSION      4u
#define PDUR_CANNM_H_AR_MINOR_VERSION      2u
#define PDUR_CANNM_H_AR_PATCH_VERSION      2u
#define PDUR_CANNM_H_SW_MAJOR_VERSION      2u
#define PDUR_CANNM_H_SW_MINOR_VERSION      0u
#define PDUR_CANNM_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_CANNM_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_CanNmRxIndication     Com_RxIndication
#define PduR_CanNmTxConfirmation   Com_TxConfirmation
#define PduR_CanNmTriggerTransmit  Com_TriggerTransmit
#else   /*STD_ON == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_CanNmRxIndication               PduR_IfRxIndication
#define PduR_CanNmTxConfirmation             PduR_IfTxConfirmation
#define PduR_CanNmTriggerTransmit            PduR_IfTriggerTransmit


#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/

#endif /*STD_ON == PDUR_CANNM_SUPPORT*/

#endif /* PDUR_CANNM_H */
