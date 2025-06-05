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
**  FILENAME    : ComM_BusSM.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V2.0.0       2020-07-24  Wanglili       R19_11 initial version.
 */
#ifndef COMM_BUSSM_H
#define COMM_BUSSM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComM_Cfg.h"
#include "Rte_ComM_Type.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define COMM_START_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of the actual bus mode by the corresponding
 *                     Bus State Manager. ComM shall propagate the indicated
 *                     state to the users with means of the RTE.
 * ServiceId           0x33
 * ServiceId           Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel,
 *                     ComMode
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_OFF == COMM_VERSION_R19_11)
FUNC(void, COMM_BUSSM_CODE)
ComM_BusSM_ModeIndication(
    NetworkHandleType Channel,
    P2CONST(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);
#else
FUNC(void, COMM_BUSSM_CODE)
ComM_BusSM_ModeIndication(
    NetworkHandleType Channel,
    VAR(ComM_ModeType, AUTOMATIC) ComMode
);
#endif
#define COMM_STOP_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"

#if (STD_ON == COMM_VERSION_R19_11)
#define COMM_START_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification of the corresponding Bus State Manager
 *                     that the actual bus mode is Bus-Sleep.
 * Only applicable for LIN slave nodes.
 * ServiceId           0x34
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE)
ComM_BusSM_BusSleepMode(
    NetworkHandleType Channel
);
#define COMM_STOP_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif /*end of #if (STD_OFF == COMM_VERSION_R19_11)*/

#endif /* COMM_BUSSM_H */
