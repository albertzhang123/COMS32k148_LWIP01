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
**  FILENAME    : PduR_Dcm.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to DCM                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_DCM_H
#define  PDUR_DCM_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_DCM_H_AR_MAJOR_VERSION      4u
#define PDUR_DCM_H_AR_MINOR_VERSION      2u
#define PDUR_DCM_H_AR_PATCH_VERSION      2u
#define PDUR_DCM_H_SW_MAJOR_VERSION      2u
#define PDUR_DCM_H_SW_MINOR_VERSION      0u
#define PDUR_DCM_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_DCM_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_DcmTransmit         CanTp_Transmit
#define PduR_DcmCancelTransmit   CanTp_CancelTransmit
#define PduR_DcmChangeParameter  CanTp_ChangeParameter
#define PduR_DcmCancelReceive    CanTp_CancelReceive
#else /* STD_ON == PDUR_ZERO_COST_OPERATION */
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_DcmTransmit          PduR_Transmit
#define PduR_DcmCancelTransmit    PduR_CancelTransmit
#define PduR_DcmChangeParameter   PduR_ChangeParameter
#define PduR_DcmCancelReceive     PduR_CancelReceive

#endif /*STD_ON == PDUR_ZERO_COST_OPERATION */
#endif /*STD_ON == PDUR_DCM_SUPPORT*/

#endif  /* end of PDUR_DCM_H */


