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
**  FILENAME    : PduR_J1939Rm.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to J1939RM                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_J1939RM_H
#define PDUR_J1939RM_H
/*******************************************************************************
**                      Include                                               **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_J1939RM_H_AR_MAJOR_VERSION      4u
#define PDUR_J1939RM_H_AR_MINOR_VERSION      2u
#define PDUR_J1939RM_H_AR_PATCH_VERSION      2u
#define PDUR_J1939RM_H_SW_MAJOR_VERSION      2u
#define PDUR_J1939RM_H_SW_MINOR_VERSION      0u
#define PDUR_J1939RM_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_J1939RM_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_J1939RmTransmit        CanIf_Transmit
#define PduR_J1939RmCancelTransmit  CanIf_CancelTransmit
#else /* STD_ON == PDUR_ZERO_COST_OPERATION */
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_J1939RmTransmit         PduR_Transmit
#define PduR_J1939RmCancelTransmit   PduR_CancelTransmit

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_J1939RM_SUPPORT*/

#endif  /* end of PDUR_J1939RM_H */
