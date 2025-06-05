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
**  FILENAME    : PduR_Com.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration supplied by PDUR to COM                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_COM_H
#define  PDUR_COM_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_COM_H_AR_MAJOR_VERSION      4u
#define PDUR_COM_H_AR_MINOR_VERSION      2u
#define PDUR_COM_H_AR_PATCH_VERSION      2u
#define PDUR_COM_H_SW_MAJOR_VERSION      2u
#define PDUR_COM_H_SW_MINOR_VERSION      0u
#define PDUR_COM_H_SW_PATCH_VERSION      0u

#if(STD_ON == PDUR_COM_SUPPORT)
/* Zero cost enable */
#if(STD_ON == PDUR_ZERO_COST_OPERATION)
#define PduR_ComTransmit        CanIf_Transmit
#define PduR_ComCancelTransmit  CanIf_CancelTransmit
#else /* STD_ON == PDUR_ZERO_COST_OPERATION */
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PduR_ComTransmit          PduR_Transmit
#define PduR_ComCancelTransmit    PduR_CancelTransmit
#define PduR_ComChangeParameter   PduR_ChangeParameter
#define PduR_ComCancelReceive     PduR_CancelReceive
#endif /*STD_ON == PDUR_ZERO_COST_OPERATION*/
#endif /*STD_ON == PDUR_COM_SUPPORT*/

#endif  /* end of PDUR_COM_H */


