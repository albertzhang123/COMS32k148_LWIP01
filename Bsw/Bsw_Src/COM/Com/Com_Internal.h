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
**  FILENAME    : Com_Internal.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : COM internal header for internal API declarations           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef  COM_INTERNAL_H
#define  COM_INTERNAL_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com.h"
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*used by Com_GwUint8DYNSignalHandle API*/
typedef struct
{
    PduIdType TxPduId;
    uint32 DestSignalBitPosition;
    uint32 UpdateBitPosition;
}Com_GwUint8DynSignalType;

/*used by Com_TxSignalPackHandle API*/
typedef struct
{
    uint8 SignalByteLength;
    Com_SignalEndiannessType SignalEndianness;
    uint16 IpduBufferIndex;
    uint32 BitPosition;
    uint8 BitSize;
}Com_TxSignalPackType;
/*******************************************************************************
**                      Internal Global Functions                             **
*******************************************************************************/
/*********************************************************************
**Internal public functions definition in Com_Internal.c *************
**for Com.c,Com_GwInternal.c,Com_TxInternal.c,Com_RxInternal.c********
**********************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* called by Com_MainFunctionRx,Com_ResetRxPduBufferAndSignalBuffer,Com_ResetTxPduBufferAndSignalBuffer.
 * init rx signal/group signal buffer*/
#if((0u < COM_IPDUGROUP_NUMBER) || (0u < COM_RXIPDU_NUMBER))
extern FUNC(void , COM_CODE)
Com_InitSignalBuffer(Com_SignalType SignalType,uint16 SignalInitValueId,uint16 SignalLength);
#endif
/* called by Com_RxSignalUnPackHandle,Com_TxArrayGroupSignalUnPackHandle.
 * base the signal endianness,get the high byte number of the (group)signal*/
#if((0u < COM_RXIPDUBUFF_SIZE) ||   \
((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)  \
        && (COM_TXSIGNALGROUP_NUMBER > 0u) && (0u < COM_TXIPDUBUFF_SIZE)))
extern FUNC(uint16 , COM_CODE)
Com_SignalEndiannessHandle(Com_SignalEndiannessType SignalEndianness,uint16 IpduBufferIndex);
#endif
/* called by Com_TxSignalPack,Com_TxGroupSignalPack,Com_DestSignalPack.
 * pack signal which signal type is not boolean,UINT8_N,UINT8_DYN,and the signal value bits cover 2-9 bytes*/
#if((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GWMAPPING_NUMBER))
extern FUNC(void , COM_CODE)
Com_TxSignalPackHandle(
        Com_TxSignalPackType Parameter,
        uint64 TxValue,
        uint64 TxMSBValue);
#endif
/* called by Com_TxSignalTMCCalculate,Com_DestSignalTMCCalculate.
 * when FilterAlgorithm is NEW_IS_WITHIN,calculate the signal's TMC */
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER) ||(0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
extern FUNC(void , COM_CODE)
Com_NewIsWithinFilterCalculate(
        Com_SignalType SignalType,
        uint64 SignalValue,
        P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
        boolean* FilterOk);
#endif
/* called by Com_TxSignalTMCCalculate,Com_DestSignalTMCCalculate.
 * when FilterAlgorithm is NEW_IS_WITHOUT,calculate the signal's TMC */
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER) ||(0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
extern FUNC(void , COM_CODE)
Com_NewIsWithoutFilterCalculate(
        Com_SignalType SignalType,
        uint64 SignalValue,
        P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
        boolean* FilterOk);
#endif
/* called by Com_TxSignalTMHandle,Com_TxIpduTMSHandle.
 * calculate the tx ipdu TMS(signal/Group signal/Dest signal)*/
#if((0u < COM_TXSIGNAL_NUMBER) ||  \
        ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)) ||  \
        (0u < COM_GWMAPPING_NUMBER))
extern FUNC(boolean , COM_CODE)
Com_TxIpduTMSCalculate(PduIdType TxIpduId);
#endif
/* called by Com_SendSignalGroupArray,Com_SendSignalGroup,Com_GwUint8DYNSignal,Com_GwNotArraySignal,Com_GwUint8NSignal.
 * calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter.*/
#if(((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)) || (0u < COM_GWMAPPING_NUMBER))
extern FUNC(void , COM_CODE)
Com_TxIpduTMSHandle(PduIdType TxPduId,uint16* RptNum);
#endif
/*called by Com_SendSignal,Com_SendDynSignal,Com_SendSignalGroup,Com_SendSignalGroupArray.
 * set tx signal update bit to 1*/
#if (0u < COM_TXIPDU_NUMBER)
extern FUNC(void , COM_CODE)
Com_SetSignalUpdateBit(PduIdType TxIpduId,uint32 UpdateBitPosition);
#endif
/* called by Com_MainFunctionTx,Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,
 * Com_RxPduCounterValid,Com_RxPduReplicationValid.
 * update the expected counter*/
#if((0u < COM_TXIPDU_NUMBER) || (0u < COM_RXIPDU_NUMBER))
extern FUNC(void , COM_CODE)
Com_PduCounterIncrease(boolean IsTxPdu,PduIdType IpduId);
#endif

/* called by Com_TxSignalTMCCalculate,Com_RxSignalFilter.
 * when FilterAlgorithm is MASKED_NEW_DIFFERS_MASKED_OLD,calculate the signal's TMC */
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER) || (COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u))
extern FUNC(boolean,COM_CODE)
Com_MaskedNewDiffersMaskedOldFilterCalculate
(
    Com_SignalType SignalType,
    uint64 SignalValue,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
    uint16 SignalBufferId
);
#endif
/*********************************************************************
**Internal public functions definition in Com_TxInternal.c for Com.c**
**********************************************************************/
#if(0u < COM_TXIPDU_NUMBER)
/*called by Com_IpduGroupControl.
 * used to handle Tx Ipdu state change or not*/
#if((COM_IPDUGROUP_NUMBER > 0u) && (0u < COM_TXIPDU_NUMBER))
extern FUNC(void,COM_CODE)
Com_TxIpduController(PduIdType TxPduId,boolean initialize);
#endif/*(COM_IPDUGROUP_NUMBER > 0u) && (0u < COM_TXIPDU_NUMBER)*/
/*called by Com_SendSignal.
 * the tx signal value change,calculate the new TMC and TMS*/
#if(0u < COM_TXSIGNAL_NUMBER)
extern FUNC(void,COM_CODE)
Com_TxSignalTMHandle(Com_SignalIdType TxSignalId,PduIdType TxPduId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) TxSignalDataPtr);
#endif/*0u < COM_TXSIGNAL_NUMBER*/
/* called by Com_SendSignal,Com_SendTxGroupSignalHandle.
 * set Tx signal(not dynamic signal) to signalbuffer,return the buffer value is changed or not*/
#if((COM_TXSIGNALGROUP_NUMBER > 0u) || (0u < COM_TXSIGNAL_NUMBER))
extern FUNC(boolean,COM_CODE)
Com_SetTxSignalBuff(Com_SignalType SignalType,uint16 SignalLength,uint16 SignalBufferId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr);
#endif/*(COM_TXSIGNALGROUP_NUMBER > 0u) || (0u < COM_TXSIGNAL_NUMBER)*/
/* called by Com_SendSignal.
 * pack the Tx signal into ipdu Tx buffer*/
#if(0u < COM_TXSIGNAL_NUMBER)
extern FUNC(void,COM_CODE)
Com_TxSignalPack(Com_SignalIdType TxSignalId);
#endif/*0u < COM_TXSIGNAL_NUMBER*/
/* called by Com_SendSignal,Com_SendDynSignal.
 * send Tx signal handle*/
#if(0u < COM_TXSIGNAL_NUMBER)
extern FUNC(uint8 , COM_CODE)
Com_SendTxSignalHandle(PduIdType TxIpduId,Com_TransferPropertyType SignalTxProperty,boolean DMSignal,boolean ValueChanged);
#endif/*0u < COM_TXSIGNAL_NUMBER*/
/* called by Com_SendSignal,Com_InvalidateSignalGroup.
 * send Tx group signal handle*/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
extern FUNC(uint8 , COM_CODE)/*return value:E_OK,COM_SERVICE_NOT_AVAILABLE*/
Com_SendTxGroupSignalHandle(Com_SignalIdType TxGroupSignalId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr);
#endif
/* called by Com_SendDynSignal.
 * set Tx dynamic signal to signalbuffer,return the buffer value is changed or not*/
#if((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER))
extern FUNC(boolean,COM_CODE)
Com_SetDynTxSignalBuff(PduIdType TxPduId,uint16 SignalBufferId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr,uint16 Length);
#endif
/* called by Com_SendDynSignal,Com_TxSignalGroupPack.
 * dynamic signal pack to tx pdu buffer*/
#if((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER))
extern FUNC(void,COM_CODE)
Com_TxDynSignalPack(PduIdType TxPduId,uint32 BitPosition,uint16 SignalBufferId,uint16 Length);
#endif
/* called by Com_SendDynSignal,Com_InvalidateSignalGroup.
 * send dynamic Tx group signal handle*/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
extern FUNC(uint8 , COM_CODE)/*return value:E_OK,COM_SERVICE_NOT_AVAILABLE*/
Com_SendTxDynGroupSignalHandle(Com_SignalIdType TxGroupSignalId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,uint16 DynLength);
#endif
/* called by Com_SendSignalGroup.
 * pack the Tx signal group into ipdu Tx buffer*/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
extern FUNC(void,COM_CODE)
Com_TxSignalGroupPack(Com_SignalGroupIdType SignalGroupId);
#endif
/* called by Com_SendSignalGroupArray.
 * update every group signal buffer,calculate every TMC,return the signal group is changed or not for trigger transmit*/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
extern FUNC(boolean,COM_CODE)
Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(Com_SignalGroupIdType TxSignalGroupId,Com_SignalIdType GroupSignalNumber,uint16 TxIPduBufferId);
#endif/*(STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u)*/
/* called by Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,Com_TriggerTransmit,
 * Com_TxConfirmation,Com_TpTxConfirmation,Com_MainFunctionTx,Com_TxIpduController.
 * clear all signal group/signal/dest description signal update bit of the Tx Pdu*/
extern FUNC(void,COM_CODE)
Com_ClearTxIPduUpdates(PduIdType TxIpduId);
/* called by Com_TxConfirmation,Com_TpTxConfirmation,Com_MainFunctionTx.
 * Tx notification for all signals/signal groups of the Tx Pdu*/
extern FUNC(void,COM_CODE)
Com_ConfirmationProcess(PduIdType TxIpduId);
/* called by Com_MainFunctionTx.
 * tx pdu DM timeout notification*/
extern FUNC(void,COM_CODE)
Com_TxDMTimeOutNotification(PduIdType TxIpduId);
/* called by Com_MainFunctionTx.
 * the handle of Com_MainFunctionTx,when the tx mode is direct.
 * return the boolean value for the tx pdu need to transmit or not*/
extern FUNC(boolean,COM_CODE)
Com_MainFuncTxDirect(PduIdType IPduId, boolean MDTRun);
/* called by Com_MainFunctionTx.
 * the handle of Com_MainFunctionTx,when the tx mode is period.
 * return the boolean value for the tx pdu need to transmit or not*/
extern FUNC(boolean,COM_CODE)
Com_MainFuncTxPeriod(PduIdType IPduId, boolean MDTRun);
/* called by Com_MainFunctionTx.
 * the handle of Com_MainFunctionTx,when the tx mode is mixed.
 * return the boolean value for the tx pdu need to transmit or not*/
extern FUNC(boolean,COM_CODE)
Com_MainFuncTxMixed(PduIdType IPduId, boolean MDTRun);
/* called by Com_TriggerTransmit,Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,Com_TxIpduController.
 * It is called in case the transmission is not possible because the corresponding I-PDU group is stopped.*/
extern FUNC(void,COM_CODE)
Com_InvokeErrorNotification(PduIdType IpduId);
/* called by Com_MainFunctionTx,Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData.
 * Pack the counter value in the pdu buffer*/
extern FUNC(void,COM_CODE)
Com_PackCounterValue(PduIdType TxIpduId);
#endif/*0u < COM_TXIPDU_NUMBER*/
/*********************************************************************
**Internal public functions definition in Com_RxInternal.c for Com.c**
**********************************************************************/
#if(0u < COM_RXIPDU_NUMBER)
/* called by Com_IpduGroupControl.
 * used to handle Rx Ipdu state change or not*/
#if(COM_IPDUGROUP_NUMBER > 0u)
extern FUNC(void,COM_CODE)
Com_RxIpduController(PduIdType RxPduId,boolean initialize);
#endif/*COM_IPDUGROUP_NUMBER > 0u*/
/* called by Com_RxIpduController,Com_ReceptionDMControl.
 * set the reception deadline monitoring timer for the included signals and signal groups to the configured ComFirstTimeout*/
#if((COM_IPDUGROUP_NUMBER > 0u) && ((COM_RXSIGNAL_NUMBER+COM_RXSIGNALGROUP_NUMBER) > 0u))
extern FUNC(void,COM_CODE)
Com_ResetRxPduTimeOut(PduIdType IpduId);
#endif
/* called by Com_RxIpduController,Com_ReceptionDMControl.
 * set the reception deadline monitoring timer for the included signals and signal groups to 0*/
#if((COM_IPDUGROUP_NUMBER > 0u) && ((COM_RXSIGNAL_NUMBER+COM_RXSIGNALGROUP_NUMBER) > 0u))
extern FUNC(void,COM_CODE)
Com_DisableRxPduTimeOut(PduIdType IpduId);
#endif
/* called by Com_RxIndication,Com_TpRxIndication.
 * reset receive MD timer for all signals/signal groups(which is update) of one rx pdu*/
#if(COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u)
extern FUNC(void,COM_CODE)
Com_ResetUpdateDMTime(PduIdType IpduId,P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfo);
#endif
/* called by Com_RxIndication,Com_TpRxIndication,Com_MainFunctionRx.
 * unpack the rx pdu data and invoke notification*/
extern FUNC(void,COM_CODE)
Com_IndicationProcess(PduIdType RxIpduId);
/* called by Com_TpRxIndication.
 * execute all configured ComDataInvalidActions for all included signals and signal groups*/
#if(STD_ON == COM_RXTPPDU_SUPPORT)
extern FUNC(void,COM_CODE)
Com_TpPduInvalidAction(PduIdType RxIpduId);
#endif
/* called by Com_TpRxIndication.
 * unpack the receive pdu's counter*/
#if(STD_ON == COM_RXTPPDU_SUPPORT)
extern FUNC(uint8,COM_CODE)
Com_UnPackCounterValue(PduIdType IpduId);
#endif
/* called by Com_TpRxIndication,Com_RxIndication.
 * judge the receive pdu's counter is valid or not*/
extern FUNC(boolean,COM_CODE)
Com_RxPduCounterValid(PduIdType IpduId,uint8 RxCounter);
/* called by Com_RxIndication.
 * judge the receive pdu's Replication is valid or not*/
extern FUNC(boolean,COM_CODE)
Com_RxPduReplicationValid(PduIdType IpduId,uint8 RxCounter,P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
/* called by Com_RxIndication,Com_RxPduReplicationValid.
 * copy the Rx Pdu data to RxPdu Buffer*/
extern FUNC(void,COM_CODE)
Com_CopyPduDataToRxBuffer(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
#endif/*0u < COM_RXIPDU_NUMBER*/
/*********************************************************************
**Internal public functions definition in Com_GwInternal.c for Com.c**
**********************************************************************/
/* called by Com_MainFunctionRouteSignals.
 * gateway signal/group signal/signal group/description signal*/
#if((0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER))
extern FUNC(void,COM_CODE)
Com_GwSignal(Com_SignalIdType GWMappingId,Com_GWSignalType GWSourceSignalType,Com_SignalIdType GWSourceSignalId);
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif/*COM_INTERNAL_H*/



