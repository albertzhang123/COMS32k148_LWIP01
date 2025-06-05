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
**  FILENAME    : EthSM.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>   <REVISION LOG>
 *  V2.0.0       2020-05-27   HuRongbo   R19_11 EthSM initial version.
 *  V2.0.1       2021-03-05   HuRongbo   Bug fix.
 */

#ifndef ETHSM_H
#define ETHSM_H

#include "EthSM_Types.h"
#include "EthSM_Cfg.h"
#include "Eth_GeneralTypes.h"
#include "Rte_ComM_Type.h"
#include "SchM_EthSM.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ETHSM_VENDOR_ID                     62U
#define ETHSM_MODULE_ID                     143U
#define ETHSM_INSTANCE_ID                   0U
#define ETHSM_H_AR_MAJOR_VERSION            4U
#define ETHSM_H_AR_MINOR_VERSION            2U
#define ETHSM_H_AR_PATCH_VERSION            2U
#define ETHSM_H_SW_MAJOR_VERSION            2U
#define ETHSM_H_SW_MINOR_VERSION            0U
#define ETHSM_H_SW_PATCH_VERSION            1U

/*Development errors used by the EthSM module*/
#define ETHSM_E_INVALID_NETWORK_MODE        0x01U
#define ETHSM_E_UNINIT                      0x02U
#define ETHSM_E_PARAM_POINTER               0x03U
#define ETHSM_E_INVALID_NETWORK_HANDLE      0x04U
#define ETHSM_E_INVALID_TCP_IP_MODE         0x05U
#define ETHSM_E_PARAM_CONTROLLER            0x07U

/*service id*/
#define ETHSM_SID_INIT                      0x07U
#define ETHSM_SID_GETVERSIONINFO            0x02U
#define ETHSM_SID_REQUESTCOMMODE            0x05U
#define ETHSM_SID_GETCURRENTCOMMODE         0x04U
#define ETHSM_SID_TCPIPMODEINDICATION       0x08U
#define ETHSM_SID_GETCURRENTINTERNALMODE    0x03U
#define ETHSM_SID_CTRLMODEINDICATION        0x09U
#define ETHSM_SID_TRCVLINKSTATECHG          0x06U
#define ETHSM_SID_MAINFUNCTION              0x01U

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
#define ETHSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthSM_MemMap.h"
extern CONST(EthSM_ConfigType,ETHSM_CONST) EthSM_Config;
#define ETHSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthSM_MemMap.h"
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service initializes the EthSM module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_Init( void );

/******************************************************************************/
/*
 * Brief               Output the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfo: Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == ETHSM_VERSION_INFO_API)
FUNC(void,ETHSM_CODE) EthSM_GetVersionInfo(
    P2VAR(Std_VersionInfoType,AUTOMATIC,AUTOMATIC) VersionInfo
);
#endif

/******************************************************************************/
/*
 * Brief               Handles the communication mode and sets the Ethernet 
 *                     network active or passive.
 * ServiceId           0x05
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle: Handle of destinated communication network
 *                                    for request.
 *                     ComM_Mode: Requested communication mode.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Service accepted
 *                     E_NOT_OK: Service denied
 */
/******************************************************************************/
FUNC(Std_ReturnType,ETHSM_CODE) EthSM_RequestComMode(
    NetworkHandleType NetworkHandle,
    ComM_ModeType ComM_Mode
);

/******************************************************************************/
/*
 * Brief               This service shall put out the current communication mode
 *                     of a Ethernet network.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle: Network handle whose current communication
 *                                    mode shall be put out
 * Param-Name[out]     ComM_ModePtr: Pointer where to put out the current
 *                                   communication mode
 * Param-Name[in/out]  None
 * Return              E_OK: Service accepted
 *                     E_NOT_OK: Service denied
 */
/******************************************************************************/
FUNC(Std_ReturnType,ETHSM_CODE) EthSM_GetCurrentComMode(
    NetworkHandleType NetworkHandle,
    P2VAR(ComM_ModeType, AUTOMATIC, AUTOMATIC) ComM_ModePtr
);

#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called when mode has been read out.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (only for different Ethernet controllers)
 * Param-Name[in]      CtrlIdx: Ethernet Interface Controller whose mode has
 *                              changed.
 *                     CtrlMode: Notified Ethernet Interface Controller mode.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_CtrlModeIndication (
    uint8 CtrlIdx,
    Eth_ModeType CtrlMode
);
#define ETHSM_STOP_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the Ethernet Interface to 
 *                     report a transceiver link state change.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: Index of the Ethernet controller within the 
 *                              context of the Ethernet Interface
 *                     TransceiverLinkState: Actual transceiver link state of 
 *                                           the specific network handle
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_TrcvLinkStateChg (
    uint8 CtrlIdx,
    EthTrcv_LinkStateType TransceiverLinkState
);
#define ETHSM_STOP_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthSM_MemMap.h"

#endif /* ETHSM_H */
