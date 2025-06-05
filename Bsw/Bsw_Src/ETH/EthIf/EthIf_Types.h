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
**  FILENAME    : EthIf_Types.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YangBo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef ETHIF_TYPES_H
#define ETHIF_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Eth_GeneralTypes.h"
//#include "EcuM.h"
#include"Platform_Types.h"
#include "ComStack_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#if !defined(ETHIF_ETH_MAIJORVERSION)
#define ETHIF_ETH_MAIJORVERSION 4u
#endif/* !defined(ETHIF_ETH_MAIJORVERSION) */
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* @req SWS_EthIf_00150 */
typedef enum 
{
    ETHIF_STATE_UNINIT,   /* Ethernet Interface is not yet configured*/
    ETHIF_STATE_INIT,     /* Ethernet Interface is configured*/
}EthIf_StateType;


typedef void (* EthIfULRxIndicationType)(uint8 CtrlIdx,Eth_FrameType FrameType,boolean IsBroadcast,const uint8* PhysAddrPtr,const uint8* DataPtr,uint16 LenByte);
typedef void (* EthIfULTxConfirmationType)(uint8 CtrlIdx,Eth_BufIdxType BufIdx);
/* Function pointer for User defined TrcvLinkState callback */
typedef void (* EthIfTrcvLinkStateChgType) ( uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState );

typedef struct
{
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_SetControllerMode)(uint8 CtrlIdx, Eth_ModeType CtrlMode);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetControllerMode)(uint8 CtrlIdx, Eth_ModeType* CtrlModePtr);
#if(4u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(BufReq_ReturnType, ETHIF_APPL_CODE, Eth_ProvideTxBuffer)(uint8 CtrlIdx, Eth_BufIdxType* BufIdxPtr, uint8** BufPtr, uint16* LenBytePtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(BufReq_ReturnType, ETHIF_APPL_CODE, Eth_ProvideTxBuffer)(uint8 CtrlIdx,uint8 Priority, Eth_BufIdxType* BufIdxPtr, uint8** BufPtr, uint16* LenBytePtr);
#endif
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_Transmit)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, Eth_FrameType FrameType , boolean TxConfirmation, uint16 LenByte, const uint8* PhysAddrPtr);
    P2FUNC(void, ETHIF_APPL_CODE, Eth_SetPhysAddr)(uint8 CtrlIdx, const uint8* PhysAddrPtr);
    P2FUNC(void, ETHIF_APPL_CODE, Eth_GetPhysAddr)(uint8 CtrlIdx, uint8* PhysAddrPtr);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_UpdatePhysAddrFilter)(uint8 CtrlIdx, const uint8* PhysAddrPtr, Eth_FilterActionType Action);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetCurrentTime)(uint8 CtrlIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
    P2FUNC(void, ETHIF_APPL_CODE, Eth_EnableEgressTimeStamp)(uint8 CtrlIdx, Eth_BufIdxType BufIdx);
#if(4u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetEgressTimeStamp)(uint8 CtrlIdx, Eth_BufIdxType BufIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetEgressTimeStamp)(uint8 CtrlIdx, uint8 BufIdx, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#endif
#if(4u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetIngressTimeStamp)(uint8 CtrlIdx, Eth_DataType* DataPtr, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, Eth_GetIngressTimeStamp)(uint8 CtrlIdx, const Eth_DataType* DataPtr, Eth_TimeStampQualType* timeQualPtr, Eth_TimeStampType* timeStampPtr);
#endif
#if(4u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(void, ETHIF_APPL_CODE, Eth_Receive)(uint8 CtrlIdx,Eth_RxStatusType* RxStatusPtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(void, ETHIF_APPL_CODE, Eth_Receive)(uint8 CtrlIdx,uint8 FifoIdx,Eth_RxStatusType* RxStatusPtr);
#endif
    P2FUNC(void, ETHIF_APPL_CODE, Eth_TxConfirmation)(uint8 CtrlIdx);
}EthIf_EthDriverApiType;

typedef struct
{
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_SetTransceiverWakeupMode)(uint8 TrcvIdx, EthTrcv_WakeupModeType TrcvWakeupMode);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetTransceiverWakeupMode)(uint8 TrcvIdx, EthTrcv_WakeupModeType* TrcvWakeupModePtr);
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_TransceiverLinkStateRequest)(uint8 TrcvIdx, EthTrcv_LinkStateType LinkState);
#if(4u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetTransceiverMode)(uint8 TrcvIdx,EthTrcv_ModeType* TrcvModePtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetTransceiverMode)(uint8 TrcvIdx,Eth_ModeType* TrcvModePtr);
#endif
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_GetLinkState)(uint8 TrcvIdx,EthTrcv_LinkStateType* LinkStatePtr);
#if(4u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_SetTransceiverMode)(uint8 TrcvIdx,EthTrcv_ModeType CtrlMode);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_SetTransceiverMode)(uint8 TrcvIdx,Eth_ModeType TrcvMode);
#endif
    P2FUNC(Std_ReturnType, ETHIF_APPL_CODE, EthTrcv_CheckWakeup)(uint8 TrcvIdx);
}EthIf_EthTrcvDriverApiType;

typedef struct
{
    uint8 EthIfCtrlIdx;
    uint16 EthIfCtrlMtu;
    uint8 EthIfMaxTxBufsTotal;
    uint16 EthIfVlanId;
    uint8 EthIfPhysControllerIdRef;
    uint8 EthIfEthTrcvId;
    uint8 EthIfSwitchRefOrPortGroupRef;
}EthIf_ControllerCfgType;

typedef struct
{
    uint16 EthIfFrameType;
    uint8 EthIfOwner;
}EthIf_FrameOwnerCfgType;

typedef struct
{
    uint8 EthIfSwitchIdx;
    uint8 EthIfSwitchRef;
}EthIf_SwitchType;

typedef struct
{
    uint8 EthIfEthTrcvIdx;/*to identify the different Ethtrcv channel*/
    uint8 EthTrcvDriverId;/*to identify the different Trcv API*/
    EcuM_WakeupSourceType WakeUpSource;
}EthIfEthTrcvRefType;

typedef struct
{
    uint8 EthIfTrcvIdx;
    const EthIfEthTrcvRefType* EthIfEthTrcvRef;
}EthIf_TrcvType;

typedef struct
{
    uint8 EthIfEthCtrlId;/*to identify the different Eth channel*/
    uint8 EthDriverId;/*to identify the different driver API*/
}EthIfEthCtrlRefType;

typedef struct
{
    uint8 EthIfPhysControllerIdx;
    const EthIfEthCtrlRefType* EthIfEthCtrlRef;
}EthIf_PhysControllerType;

/* @req SWS_EthIf_00149 */
typedef struct {
    P2CONST(EthIf_ControllerCfgType, AUTOMATIC, ETHIF_APPL_CONST) EthIfCtrlCfg;   /*pointer to hold controller config data*/
    P2CONST(EthIf_FrameOwnerCfgType, AUTOMATIC, ETHIF_APPL_CONST) EthIfOwnerCfg;  /* pointer to hold Owner config data */
    P2CONST(EthIf_TrcvType, AUTOMATIC, ETHIF_APPL_CONST)          EthIfTrcvCfg;/*pointer to holad trcv config data*/
    P2CONST(EthIf_PhysControllerType, AUTOMATIC, ETHIF_APPL_CONST) EthIf_PhysControllerCfg;/*pointer to hold PhysController config data*/
    P2CONST(EthIf_SwitchType, AUTOMATIC, ETHIF_APPL_CONST)      EthIfSwitchCfg; /* pointer to hold switches config data */
    P2CONST(EthIfULTxConfirmationType, AUTOMATIC, ETHIF_APPL_CONST)    EthIfULTxConfirmation; /* pointer to hold Tx confirmation functions list */
    P2CONST(EthIfULRxIndicationType, AUTOMATIC, ETHIF_APPL_CONST)      EthIfULRxIndication; /* Ptr to Rx indication function list */
    P2CONST(EthIfTrcvLinkStateChgType, AUTOMATIC, ETHIF_APPL_CONST)    EthIfTrcvLink;       /*pointer to hold Trcv Link state functions list */
    uint8                         EthIfCtrlCount;     /* No of If Controllers configure */
    uint8                         EthIfTrcvCount;     /* No of Trcv configured */
    uint8                         EthIfSwitchCount;   /* No of Switches configured */
    uint8                         EthIfOwnersCount;   /* No fo Owners configured */
    uint8                         EthIfPhysControllerCount;   /* No fo Eth */
}EthIf_ConfigType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/


/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/


#endif
