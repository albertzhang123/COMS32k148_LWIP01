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
**  FILENAME    : PduR_IpduM.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Call-back of PDUR for IpduM                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_IPDUM_H
#define PDUR_IPDUM_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_IPDUM_H_AR_MAJOR_VERSION      4u
#define PDUR_IPDUM_H_AR_MINOR_VERSION      2u
#define PDUR_IPDUM_H_AR_PATCH_VERSION      2u
#define PDUR_IPDUM_H_SW_MAJOR_VERSION      2u
#define PDUR_IPDUM_H_SW_MINOR_VERSION      0u
#define PDUR_IPDUM_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_IPDUM_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#else
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_IpduMTransmit          PduR_Transmit

#define PduR_IpduMTriggerTransmit   PduR_IfTriggerTransmit
#define PduR_IpduMRxIndication      PduR_IfRxIndication
#define PduR_IpduMTxConfirmation    PduR_IfTxConfirmation
#endif /*STD_OFF == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_IPDUM_SUPPORT*/
#endif  /* end of PDUR_SECOC_H */
