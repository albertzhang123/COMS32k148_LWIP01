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
**  FILENAME    : EthSM_TcpIp.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>   <REVISION LOG>
 *  V2.0.0       2020-05-27   HuRongbo   R19_11 EthSM initial version.
 */

#ifndef ETHSM_TCPIP_H
#define ETHSM_TCPIP_H

#include "TcpIp_Types.h"

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

#define ETHSM_START_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the TcpIp to report the actual
 *                     TcpIp state (e.g. online,offline).
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: EthIf controller index to identify the 
 *                     communication network where the TcpIp state is changed.
 *                     TcpIpState: Actual TcpIp state of the specific network
 *                                 handle.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_TcpIpModeIndication (
    uint8 CtrlIdx,
    TcpIp_StateType TcpIpState
);
#define ETHSM_STOP_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"


#endif /* ETHSM_TCPIP_H */
