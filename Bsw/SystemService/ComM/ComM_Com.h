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
**  FILENAME    : ComM_Com.h                                                  **
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
#ifndef COMM_COM_H
#define COMM_COM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
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
#if (STD_ON == COMM_PNC_SUPPORT)
#define COMM_START_SEC_COMMCOMCBK_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This callback is called when the EIRA or ERA was updated
 *                     in COM.
 * ServiceId           0x36
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_COM_CODE)
ComM_COMCbk(
    uint16 signalId
);
#define COMM_STOP_SEC_COMMCOMCBK_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif/* #if (STD_ON == COMM_PNC_SUPPORT)*/

#endif /* COMM_COM_H */
