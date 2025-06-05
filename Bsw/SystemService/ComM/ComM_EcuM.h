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
**  FILENAME    : ComM_EcuM.h                                                 **
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
#ifndef COMM_ECUM_H
#define COMM_ECUM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "ComM_Cfg.h"
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
#if (STD_ON == COMM_ECUM_USED)
#define COMM_START_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification of a wake up on the corresponding channel.
 * ServiceId           0x2A
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_ECUM_CODE)
ComM_EcuM_WakeUpIndication(
    NetworkHandleType Channel
);
#define COMM_STOP_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"

#if (STD_ON == COMM_VERSION_R19_11)
#if (STD_ON == COMM_PNC_SUPPORT)
#define COMM_START_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification of a wake up on the corresponding partial
 *                     network cluster.
 * ServiceId           0x37
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      PNCid
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE)
ComM_EcuM_PNCWakeUpIndication(
    PNCHandleType PNCid
);
#define COMM_STOP_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif/* (STD_ON == COMM_PNC_SUPPORT)*/
#endif /* #if (STD_ON == COMM_VERSION_R19_11)*/
#endif /* #if (STD_ON == COMM_ECUM_USED)*/

#endif /* COMM_ECUM_H */
