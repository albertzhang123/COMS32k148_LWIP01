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
**  FILENAME    : ComM_BswM.h                                                 **
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
#ifndef COMM_BSWM_H
#define COMM_BSWM_H

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
#if (( STD_ON == COMM_BSWM_USED)||( STD_ON == COMM_ECUM_USED))
#define COMM_START_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               EcuM or BswM shall indicate to ComM when communication
 *                     is allowed.
 * ServiceId           0x35
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel,
 *                     Allowed
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_ECUMBSWM_CODE)
ComM_CommunicationAllowed(
    NetworkHandleType Channel,
    boolean Allowed
);
#define COMM_STOP_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif /*end of if (( STD_ON == COMM_BSWM_USED)||( STD_ON == COMM_ECUM_USED))*/
#endif /* COMM_BSWM_H */
