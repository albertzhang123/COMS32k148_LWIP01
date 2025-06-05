/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2018)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : EthIf.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef ETHIF_CBK_H
#define ETHIF_CBK_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "EthIf_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define ETHIF_AR_RELEASE_MAJOR_VERSION     19
#define ETHIF_AR_RELEASE_MINOR_VERSION     11
#define ETHIF_AR_RELEASE_REVISION_VERSION  0
#define ETHIF_SW_MAJOR_VERSION             2
#define ETHIF_SW_MINOR_VERSION             0
#define ETHIF_SW_PATCH_VERSION             0

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/* @req SWS_EthIf_00085 */
extern FUNC(void, ETHIF_CODE) EthIf_RxIndication( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_FrameType, ETHIF_VAR) FrameType, VAR(boolean, ETHIF_VAR) IsBroadcast, P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr, P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr, VAR(uint16, ETHIF_VAR) LenByte );
/* @req SWS_EthIf_00091 */
extern FUNC(void, ETHIF_CODE) EthIf_TxConfirmation( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx );
/* @req SWS_EthIf_00231 */
extern FUNC(void, ETHIF_CODE) EthIf_CtrlModeIndication( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode );
/* @req SWS_EthIf_00232 */
extern FUNC(void, ETHIF_CODE) EthIf_TrcvModeIndication( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode );

#endif

