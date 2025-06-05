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
**  FILENAME    : Com_TxInternal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : internal implementation for COM transmit                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
#if(STD_ON == COM_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#if(0u < COM_TXIPDU_NUMBER)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* called by Com_TxIpduController
 * reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
#if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_ResetTxOccurrenceOfPduFilter(PduIdType IpduId);
#endif
/* called by Com_TxIpduController
 * init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC*/
#if(COM_IPDUGROUP_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_ResetTxPduBufferAndSignalBuffer(PduIdType IpduId);
#endif
/* called by Com_TxSignalGroupPack.
 * pack tx group signal(not dynamic)*/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_TxGroupSignalPack(Com_SignalIdType GroupSignalId);
#endif/* COM_TXSIGNALGROUP_NUMBER > 0u */
/* called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 * unpack the tx array group signal*/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
static FUNC(uint64,COM_CODE)
Com_UnPackTxArrayGroupSignalValue(Com_SignalIdType SignalId,uint16 IpduBufferId);
#endif
/* called by Com_UnPackTxArrayGroupSignalValue.
 * unpack all byte value(group signal cover,except MSB byte)*/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u) && (0u < COM_TXIPDUBUFF_SIZE))
static FUNC(uint64,COM_CODE)
Com_TxArrayGroupSignalUnPackHandle(Com_SignalEndiannessType SignalEndianness,uint8 SignalByteLength,uint16* IpduBufferIndex);
#endif
/* called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 * the group signal value changed,judge it will trigger the TxPdu transmit(DIRECT/MIXED mode) or not. */
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
static FUNC(void,COM_CODE)
Com_TxArrayGroupSignalTriggerOnChange(Com_SignalIdType GroupSignalId,boolean* TriggerOnChange);
#endif
/* called by Com_TxSignalTMHandle,Com_SendTxGroupSignalHandle,Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 * calculate the new TMC of the signal new value*/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_TxSignalTMCCalculate
(
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
    Com_SignalType SignalType,
    uint16 SignalBufferId
);
#endif
/* called by Com_TxSignalTMCCalculate.
 * change the signal value to uint64 type*/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER))
static FUNC(uint64,COM_CODE)
Com_GetTxSignalValue(Com_SignalType SignalType,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr);
#endif
/* called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 * update the uint8_n signal value*/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
#if((0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
static FUNC(boolean,COM_CODE)
Com_TxGroupSignalUpdateUint8N(
        uint16 TxIPduBufferId,
        Com_SignalIdType groupSignalId,
        uint16 groupSignalInitValueId);
#endif
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called by Com_IpduGroupControl.
 *                     used to handle Tx Ipdu state change or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,initialize
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_IPDUGROUP_NUMBER > 0u)
FUNC(void,COM_CODE)
Com_TxIpduController(PduIdType TxPduId,boolean initialize)
{
    Com_IpduGroupIdType ipduGroupRefNumber;
    Com_IpduGroupIdType ipduGroupRefIndex;
    Com_IpduGroupIdType ipduGroupId;
    boolean findActiveIpduGroup = FALSE;
    ipduGroupRefNumber = Com_ConfigStd->ComTxIPdu[TxPduId].ComIPduGroupsRefNumber;
    /*if the Pdu not included in any Ipdu Group,the state is always start*/
    if(ipduGroupRefNumber > 0u)
    {
        for(ipduGroupRefIndex = 0u;(ipduGroupRefIndex < ipduGroupRefNumber)&&(FALSE == findActiveIpduGroup);ipduGroupRefIndex++)
        {
            ipduGroupId = Com_ConfigStd->ComTxIPdu[TxPduId].ComIPduGroupsRef[ipduGroupRefIndex];
            if(TRUE == Com_IpduGroupEnable[ipduGroupId])
            {
                findActiveIpduGroup = TRUE;
            }
        }
        /*at least one Ipdu Group is start,the Pdu shall be active*/
        if(TRUE == findActiveIpduGroup)
        {
            /*the pdu restart*/
            if(FALSE == Com_TxIPduRunTimeState[TxPduId].ActiveEnable)
            {
                Com_TxIPduRunTimeState[TxPduId].ActiveEnable = TRUE;
                /*reset ComMinimumDelayTime of I-PDUs in transmission*/
                Com_TxIPduRunTimeState[TxPduId].MDTCnt = 0u;
                /*reset Tx time out counter*/
                if((NULL_PTR == Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse)
                        && (COM_TX_MODE_NONE == Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeMode))
                {
                    Com_TxIPduRunTimeState[TxPduId].DMCnt = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxIpduDM;
                }
                else
                {
                    Com_TxIPduRunTimeState[TxPduId].DMCnt = 0u;
                }
                /*clear all signal/signal group/dest description signal's update bit of the tx Ipdu*/
                Com_ClearTxIPduUpdates(TxPduId);
                /*reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
                #if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
                Com_ResetTxOccurrenceOfPduFilter(TxPduId);
                #endif
                if(TRUE == initialize)
                {
                    /*init the tx ipdu buffer,,all signal buffer(included in the ipdu) and init the TMC,TMS*/
                    Com_ResetTxPduBufferAndSignalBuffer(TxPduId);
                    /*reset the Tx Time Period of I-PDUs in Periodic or MIXED transmission mode*/
                    if(TRUE == Com_TxIPduRunTimeState[TxPduId].TMS)
                    {
                        Com_TxIPduRunTimeState[TxPduId].PeriodCnt =
                                Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeTimeOffset;
                    }
                    else
                    {
                        Com_TxIPduRunTimeState[TxPduId].PeriodCnt =
                                Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse->ComTxModeTimeOffset;
                    }
                    if(NULL_PTR != Com_ConfigStd->ComTxIPdu[TxPduId].ComIPduCounter)
                    {
                        /*set the I-PDU counter to 0 for I-PDUs with ComIPduDirection configured to SEND*/
                        Com_TxIPduRunTimeState[TxPduId].TxIpduCounter = 0u;
                    }
                }
            }
        }
        else
        {
            if(TRUE == Com_TxIPduRunTimeState[TxPduId].ActiveEnable)
            {
                Com_TxIPduRunTimeState[TxPduId].ActiveEnable = FALSE;
                if(TRUE == Com_TxIPduRunTimeState[TxPduId].Transmiting)
                {
                    Com_InvokeErrorNotification(TxPduId);
                    #if(STD_ON == COM_CANCELLATION_SUPPORT)
                    if(TRUE == Com_ConfigStd->ComTxIPdu[TxPduId].ComIPduCancellationSupport)
                    {
                        (void)PduR_ComCancelTransmit(Com_ConfigStd->ComTxIPdu[TxPduId].ComPduIdRef);
                    }
                    #endif
                    Com_TxIPduRunTimeState[TxPduId].Transmiting = FALSE;
                }
                Com_TxIPduRunTimeState[TxPduId].DMCnt = 0u;
            }
        }
    }
}
#endif/*COM_IPDUGROUP_NUMBER > 0u*/
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal.
 *                     the tx signal value change,calculate the new TMC and TMS.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxSignalId,TxPduId,TxSignalDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_TXSIGNAL_NUMBER)
FUNC(void,COM_CODE)
Com_TxSignalTMHandle(Com_SignalIdType TxSignalId,PduIdType TxPduId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) TxSignalDataPtr)
{
    #if(0u < COM_TMCTXSIGNAL_NUMBER)
    Com_SignalIdType txSignalTMCId;
    Com_SignalType signalType;
    uint16 signalBufferId;
    #endif
    boolean oldTMS;
    #if(0u < COM_TMCTXSIGNAL_NUMBER)
    if(NULL_PTR != Com_ConfigStd->ComTxSignal[TxSignalId].ComFilter)
    {
        txSignalTMCId = Com_ConfigStd->ComTxSignal[TxSignalId].ComFilter->ComTMCBufferId;
        signalType = Com_ConfigStd->ComTxSignal[TxSignalId].ComSignalType;
        signalBufferId = Com_ConfigStd->ComTxSignal[TxSignalId].ComSignalInitValueId;
        /*calculate the TMC of the signal*/
        TxSignalTMCRunTime[txSignalTMCId] =
                Com_TxSignalTMCCalculate(TxSignalDataPtr,Com_ConfigStd->ComTxSignal[TxSignalId].ComFilter,signalType,signalBufferId);
    }
    #endif
    oldTMS = Com_TxIPduRunTimeState[TxPduId].TMS;
    Com_TxIPduRunTimeState[TxPduId].TMS = Com_TxIpduTMSCalculate(TxPduId);
    if(TRUE == Com_TxIPduRunTimeState[TxPduId].TMS)
    {
        Com_TxIPduRunTimeState[TxPduId].ipduTxMode = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeMode;
        /*TMS changed,reset PeriodCnt for PERIODIC/MIXED Tx Mode*/
        if(FALSE == oldTMS)
        {
            Com_TxIPduRunTimeState[TxPduId].PeriodCnt = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeTimeOffset;
        }
    }
    else
    {
        Com_TxIPduRunTimeState[TxPduId].ipduTxMode = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse->ComTxModeMode;
        /*TMS changed,reset PeriodCnt for PERIODIC/MIXED Tx Mode*/
        if(TRUE == oldTMS)
        {
            Com_TxIPduRunTimeState[TxPduId].PeriodCnt = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse->ComTxModeTimeOffset;
        }
    }
    COM_NOUSED(TxSignalId);
    COM_NOUSED(TxSignalDataPtr);
    return;
}
#endif/*0u < COM_TXSIGNAL_NUMBER*/
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_SendTxGroupSignalHandle.
 *                     set Tx signal(not dynamic signal) to signalbuffer,return
 *                     the buffer value is changed or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalLength,SignalBufferId,SignalNewDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((COM_TXSIGNALGROUP_NUMBER > 0u) || (0u < COM_TXSIGNAL_NUMBER))
FUNC(boolean,COM_CODE)
Com_SetTxSignalBuff(Com_SignalType SignalType,uint16 SignalLength,uint16 SignalBufferId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr)
{
    boolean ret = FALSE;
    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 valueLoop;
    #endif
    switch(SignalType)
    {
        #if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            if(*((const boolean*)(SignalNewDataPtr)) != Com_SignalBoolRuntimeBuff[SignalBufferId])
            {
                Com_SignalBoolRuntimeBuff[SignalBufferId] =  *((const boolean*)(SignalNewDataPtr));
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            if(*((const uint8*)(SignalNewDataPtr)) != Com_Signal8BitRuntimeBuff[SignalBufferId])
            {
                Com_Signal8BitRuntimeBuff[SignalBufferId] =  *((const uint8*)(SignalNewDataPtr));
                ret = TRUE;
            }
            break;
        case COM_UINT8_N:
        {
            /* Copy Uint8N signal value one byte by one */
            for(valueLoop = 0u; valueLoop < SignalLength; valueLoop++)
            {
                if(((const uint8*)SignalNewDataPtr)[valueLoop] != Com_Signal8BitRuntimeBuff[SignalBufferId + valueLoop])
                {
                    Com_Signal8BitRuntimeBuff[SignalBufferId + valueLoop] = ((const uint8*)SignalNewDataPtr)[valueLoop];
                    ret = TRUE;
                }
            }
            break;
        }
        #endif
        #if(0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            if(*((const uint16*)(SignalNewDataPtr)) != Com_Signal16BitRuntimeBuff[SignalBufferId])
            {
                Com_Signal16BitRuntimeBuff[SignalBufferId] =  *((const uint16*)(SignalNewDataPtr));
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            if(*((const uint32*)(SignalNewDataPtr)) != Com_Signal32BitRuntimeBuff[SignalBufferId])
            {
                Com_Signal32BitRuntimeBuff[SignalBufferId] =  *((const uint32*)(SignalNewDataPtr));
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            if(*((const uint64*)(SignalNewDataPtr)) != Com_Signal64BitRuntimeBuff[SignalBufferId])
            {
                Com_Signal64BitRuntimeBuff[SignalBufferId] =  *((const uint64*)(SignalNewDataPtr));
                ret = TRUE;
            }
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    return ret;
}
#endif/*(COM_TXSIGNALGROUP_NUMBER > 0u) || (0u < COM_TXSIGNAL_NUMBER)*/
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal.
 *                     pack the Tx signal into ipdu Tx buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_TXSIGNAL_NUMBER)
FUNC(void,COM_CODE)
Com_TxSignalPack(Com_SignalIdType TxSignalId)
{
    PduIdType txIpduId;
    uint32 bitPosition;
    Com_SignalType signalType;
    uint16 ipduBufferIndex;
    uint16 signalBufferId;
    #if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
    uint8 value = 0x01u;
    #endif
    #if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
    uint16 signalLength;
    uint16 cnt;
    #endif
    uint64 txData = 0u;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 bitSize;
    Com_TxSignalPackType parameter;
    txIpduId = Com_ConfigStd->ComTxSignal[TxSignalId].ComIpduRefIndex;
    bitPosition = Com_ConfigStd->ComTxSignal[TxSignalId].ComBitPosition;
    signalType = Com_ConfigStd->ComTxSignal[TxSignalId].ComSignalType;
    ipduBufferIndex = Com_ConfigStd->ComTxIPdu[txIpduId].ComTxIPduBufIndex;
    signalBufferId = Com_ConfigStd->ComTxSignal[TxSignalId].ComSignalInitValueId;
    /*pack boolean type signal*/
    if(COM_BOOLEAN == signalType)
    {
        #if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
        if(TRUE == Com_SignalBoolRuntimeBuff[signalBufferId])
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] |=
                    (uint8)(value << (bitPosition & 0x07u));
        }
        else
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] &=
                    (uint8)(~(uint8)(value << (bitPosition & 0x07u)));
        }
        #endif
    }
    /*pack COM_UINT8_N type signal*/
    else if(COM_UINT8_N == signalType)
    {
        #if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
        signalLength = Com_ConfigStd->ComTxSignal[TxSignalId].ComSignalLength;
        for(cnt = 0u;cnt < signalLength;cnt++)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u) + cnt] =
                    Com_Signal8BitRuntimeBuff[signalBufferId + cnt];
        }
        #endif
    }
    /*pack signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
    else
    {
        /*get the signal value need to be packed*/
        switch(signalType)
        {
            #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
            case COM_UINT8:
            case COM_SINT8:
                txData = (uint64)(Com_Signal8BitRuntimeBuff[signalBufferId]);
                break;
            #endif
            #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
            case COM_UINT16:
            case COM_SINT16:
                txData = (uint64)(Com_Signal16BitRuntimeBuff[signalBufferId]);
                break;
            #endif
            #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                txData = (uint64)(Com_Signal32BitRuntimeBuff[signalBufferId]);
                break;
            #endif
            #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                txData = Com_Signal64BitRuntimeBuff[signalBufferId];
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
        /* signal MSB byte value used for the signal value bits cover 9 bytes */
        dataMSBByte = txData;
        /* Align LSB of txData and signal_value in IPdu */
        txData = txData << ((uint8)bitPosition & 0x07u);
        bitSize = Com_ConfigStd->ComTxSignal[TxSignalId].ComBitSize;
        /*signalByteLength:1-9*/
        signalByteLength = 1u + ((bitSize + ((uint8)bitPosition & 0x07u) - 1u)/8u);
        /*the tx signal all value bits are in one byte*/
        if(1u == signalByteLength)
        {
            #if(0u < COM_TXIPDUBUFF_SIZE)
            /*Clear corresponding bits in IPdu*/
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] &=
                    (0xFFu >> (8u - (bitPosition & 0x07u))) | (uint8)(0xFFu << ((bitPosition & 0x07u) + bitSize));
            /*write corresponding bits to IPdu buffer*/
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] |=
                    (0xFFu >> (8u - ((bitPosition & 0x07u) + bitSize))) & ((uint8)txData);
            #endif
        }
        /*the tx signal all value bits are included in 2-9 byte*/
        else if((1u < signalByteLength) && (signalByteLength < 10u))
        {
            parameter.SignalByteLength = signalByteLength;
            parameter.SignalEndianness = Com_ConfigStd->ComTxSignal[TxSignalId].ComSignalEndianness;
            parameter.IpduBufferIndex = ipduBufferIndex;
            parameter.BitPosition = bitPosition;
            parameter.BitSize = bitSize;
            Com_TxSignalPackHandle(parameter,txData,dataMSBByte);
        }
        /*this case will not happen*/
        else
        {
            /*do nothing*/
        }
    }
    return;
}
#endif/*0u < COM_TXSIGNAL_NUMBER*/
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_SendDynSignal.
 *                     send Tx signal handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId,SignalTxProperty,DMSignal,ValueChanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
#if(0u < COM_TXSIGNAL_NUMBER)
FUNC(uint8 , COM_CODE)
Com_SendTxSignalHandle(PduIdType TxIpduId,Com_TransferPropertyType SignalTxProperty,boolean DMSignal,boolean ValueChanged)
{
    uint16 rptNum;
    boolean initDMCnt = FALSE;
    /*the signal config timeout,the DM timer isn't start by other signals,the Tx mode is not NONE.then start the DM timer here*/
    if((0u == Com_TxIPduRunTimeState[TxIpduId].DMCnt) && (TRUE == DMSignal)
            && (COM_TX_MODE_NONE != Com_TxIPduRunTimeState[TxIpduId].ipduTxMode))
    {
        Com_TxIPduRunTimeState[TxIpduId].DMCnt = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxIpduDM;
        initDMCnt = TRUE;
    }
    if((COM_TX_MODE_DIRECT == Com_TxIPduRunTimeState[TxIpduId].ipduTxMode)
            || (COM_TX_MODE_MIXED == Com_TxIPduRunTimeState[TxIpduId].ipduTxMode))
    {
        if(TRUE == Com_TxIPduRunTimeState[TxIpduId].TMS)
        {
            rptNum = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxModeTrue->ComTxModeNumberOfRepetitions;
        }
        else
        {
            rptNum = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxModeFalse->ComTxModeNumberOfRepetitions;
        }
        switch(SignalTxProperty)
        {
            case COM_TRIGGERED:
                Com_TxIPduRunTimeState[TxIpduId].NTimeCnt = 0u;
                Com_TxIPduRunTimeState[TxIpduId].RptNum = rptNum;
                /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR COM module shall reset an already
                  running transmission deadline monitoring timer in case another send request for this I-PDU is initiated*/
                if((1u < rptNum) && (FALSE == initDMCnt) && (TRUE == DMSignal))
                {
                    Com_TxIPduRunTimeState[TxIpduId].DMCnt = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxIpduDM;
                }
                break;
            case COM_TRIGGERED_ON_CHANGE:
                if(TRUE == ValueChanged)
                {
                    Com_TxIPduRunTimeState[TxIpduId].NTimeCnt = 0u;
                    Com_TxIPduRunTimeState[TxIpduId].RptNum = rptNum;
                    /*For an I-PDU with ComTxModeMode DIRECT and ComTxModeNumberOfRepetitions > 0, the AUTOSAR COM module shall reset an already
                      running transmission deadline monitoring timer in case another send request for this I-PDU is initiated*/
                    if((1u < rptNum) && (FALSE == initDMCnt) && (TRUE == DMSignal))
                    {
                        Com_TxIPduRunTimeState[TxIpduId].DMCnt = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxIpduDM;
                    }
                }
                break;
            case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                if(TRUE == ValueChanged)
                {
                    Com_TxIPduRunTimeState[TxIpduId].NTimeCnt = 0u;
                    Com_TxIPduRunTimeState[TxIpduId].RptNum = 1u;
                }
                break;
            case COM_TRIGGERED_WITHOUT_REPETITION:
                Com_TxIPduRunTimeState[TxIpduId].NTimeCnt = 0u;
                Com_TxIPduRunTimeState[TxIpduId].RptNum = 1u;
                break;
            default:
                /*do nothing*/
                break;
        }
    }
    else
    {
        Com_TxIPduRunTimeState[TxIpduId].NTimeCnt = 0u;
        Com_TxIPduRunTimeState[TxIpduId].RptNum = 0u;
    }
    return E_OK;
}
#endif/*0u < COM_TXSIGNAL_NUMBER*/
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_InvalidateSignalGroup.
 *                     send Tx group signal handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxGroupSignalId,SignalDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK,COM_SERVICE_NOT_AVAILABLE
 */
/******************************************************************************/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
FUNC(uint8 , COM_CODE)
Com_SendTxGroupSignalHandle(Com_SignalIdType TxGroupSignalId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr)
{
    uint8 ret;
    Com_SignalGroupIdType signalGroupId;
    Com_TransferPropertyType signalGroupTxProperty;
    Com_TransferPropertyType groupSignalTxProperty;
    boolean signalGroupInitValueOnly;
    #if(0u < COM_TMCTXGROUPSIGNAL_NUMBER)
    Com_SignalIdType txSignalTMCId;
    #endif
    boolean valueChanged;
    Com_SignalType groupSignalType;
    uint16 groupSignalLength;
    uint16 groupSignalBufferId;
    signalGroupId = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalGroupRef;
    signalGroupInitValueOnly = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComInitialValueOnly;
    groupSignalType = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalType;
    /*dynamic signal isn't send by the API,InitialValueOnly signal group don't expect to be called by any API*/
    if((COM_UINT8_DYN == groupSignalType) || (TRUE == signalGroupInitValueOnly))
    {
        ret = COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {
        groupSignalBufferId = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalInitValueId;
        #if(0u < COM_TMCTXGROUPSIGNAL_NUMBER)
        if(NULL_PTR != Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComFilter)
        {
            txSignalTMCId = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComFilter->ComTMCBufferId;
            /*calculate the TMC of the signal*/
            TxGroupSignalTMCBuffer[txSignalTMCId] =
                    Com_TxSignalTMCCalculate(SignalDataPtr,Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComFilter,groupSignalType,groupSignalBufferId);
        }
        #endif
        /*groupSignalLength is used when the signal type is COM_UINT8_N*/
        groupSignalLength = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalLength;
        /*update tx signal buffer and check if the sending signal value changed*/
        valueChanged = Com_SetTxSignalBuff(groupSignalType,groupSignalLength,groupSignalBufferId,SignalDataPtr);
        signalGroupTxProperty = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComTransferProperty;
        switch(signalGroupTxProperty)
        {
            case COM_TRIGGERED:
                Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                Com_TxSignalGroupState[signalGroupId].WithoutRepetition = FALSE;
                break;
            case COM_TRIGGERED_ON_CHANGE:
                if(TRUE == Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComTxGroupSignalUsedTransferProperty)
                {
                    groupSignalTxProperty = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComTransferProperty;
                    if((TRUE == valueChanged)&&(COM_TRIGGERED_ON_CHANGE == groupSignalTxProperty))
                    {
                        Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                        Com_TxSignalGroupState[signalGroupId].WithoutRepetition = FALSE;
                    }
                }
                else
                {
                    if(TRUE == valueChanged)
                    {
                        Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                        Com_TxSignalGroupState[signalGroupId].WithoutRepetition = FALSE;
                    }
                }
                break;
            case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                if(TRUE == valueChanged)
                {
                    Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                    Com_TxSignalGroupState[signalGroupId].WithoutRepetition = TRUE;
                }
                break;
            case COM_TRIGGERED_WITHOUT_REPETITION:
                Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                Com_TxSignalGroupState[signalGroupId].WithoutRepetition = TRUE;
                break;
            default:
                /*do nothing*/
                break;
        }
        ret = E_OK;
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_SendDynSignal.
 *                     set Tx dynamic signal to signalbuffer,return the buffer
 *                     value is changed or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,SignalBufferId,SignalNewDataPtr,Length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER))
FUNC(boolean,COM_CODE)
Com_SetDynTxSignalBuff(PduIdType TxPduId,uint16 SignalBufferId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalNewDataPtr,uint16 Length)
{
    boolean ret = FALSE;
    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 valueLoop;
    #endif
    uint16 oldSignalLength;
    /* Copy Uint8N signal value one byte by one */
    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
    for(valueLoop = 0u; valueLoop < Length; valueLoop++)
    {
        if(((const uint8*)SignalNewDataPtr)[valueLoop] != Com_Signal8BitRuntimeBuff[SignalBufferId + valueLoop])
        {
            Com_Signal8BitRuntimeBuff[SignalBufferId + valueLoop] = ((const uint8*)SignalNewDataPtr)[valueLoop];
            ret = TRUE;
        }
    }
    #endif
    if(FALSE == ret)
    {
        oldSignalLength = Com_TxIPduRunTimeState[TxPduId].TxIpduLength
                - (uint16)Com_ConfigStd->ComTxIPdu[TxPduId].IPduNoDynSignalLength;
        if(oldSignalLength != Length)
        {
            ret = TRUE;
        }
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_SendDynSignal,Com_TxSignalGroupPack.
 *                     dynamic signal pack to tx pdu buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,BitPosition,SignalBufferId,Length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER))
FUNC(void,COM_CODE)
Com_TxDynSignalPack(PduIdType TxPduId,uint32 BitPosition,uint16 SignalBufferId,uint16 Length)
{
    #if((0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
    uint16 ipduBufferIndex;
    uint16 cnt;
    ipduBufferIndex = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxIPduBufIndex;
    for(cnt = 0u;cnt < Length;cnt++)
    {
        Com_TxIPduRuntimeBuff[ipduBufferIndex + (BitPosition/8u) + cnt] =
                Com_Signal8BitRuntimeBuff[SignalBufferId + cnt];
    }
    #endif
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_SendDynSignal,Com_InvalidateSignalGroup.
 *                     send dynamic Tx group signal handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxGroupSignalId,SignalDataPtr,DynLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK,COM_SERVICE_NOT_AVAILABLE
 */
/******************************************************************************/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
FUNC(uint8 , COM_CODE)
Com_SendTxDynGroupSignalHandle(Com_SignalIdType TxGroupSignalId,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,uint16 DynLength)
{
    uint8 ret;
    Com_SignalGroupIdType signalGroupId;
    PduIdType ipduRef;
    Com_TransferPropertyType signalGroupTxProperty;
    Com_TransferPropertyType groupSignalTxProperty;
    boolean signalGroupInitValueOnly;
    boolean valueChanged;
    uint16 groupSignalBufferId;
    signalGroupId = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalGroupRef;
    signalGroupInitValueOnly = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComInitialValueOnly;
    /*group signal(not dynamic) isn't send by the API,InitialValueOnly signal group don't expect to be called by any API*/
    if((COM_UINT8_DYN != Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalType)
            || (TRUE == signalGroupInitValueOnly))
    {
        ret = COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {
        if(DynLength > Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalLength)
        {
            #if(STD_ON == COM_DEV_ERROR_DETECT)
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM);
            #endif
            ret = E_NOT_OK;
        }
        else
        {
            ipduRef = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComIpduRefIndex;
            groupSignalBufferId = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalInitValueId;
            /*update Dynamic tx group signal buffer and check if the sending signal value changed*/
            valueChanged = Com_SetDynTxSignalBuff(ipduRef,groupSignalBufferId,SignalDataPtr,DynLength);
            Com_TxSignalGroupState[signalGroupId].DynamicSignalLength = DynLength;
            signalGroupTxProperty = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComTransferProperty;
            switch(signalGroupTxProperty)
            {
                case COM_TRIGGERED:
                    Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                    Com_TxSignalGroupState[signalGroupId].WithoutRepetition = FALSE;
                    break;
                case COM_TRIGGERED_ON_CHANGE:
                    if(TRUE == Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComTxGroupSignalUsedTransferProperty)
                    {
                        groupSignalTxProperty = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComTransferProperty;
                        if((TRUE == valueChanged)&&(COM_TRIGGERED_ON_CHANGE == groupSignalTxProperty))
                        {
                            Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                            Com_TxSignalGroupState[signalGroupId].WithoutRepetition = FALSE;
                        }
                    }
                    else
                    {
                        if(TRUE == valueChanged)
                        {
                            Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                            Com_TxSignalGroupState[signalGroupId].WithoutRepetition = FALSE;
                        }
                    }
                    break;
                case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                    if(TRUE == valueChanged)
                    {
                        Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                        Com_TxSignalGroupState[signalGroupId].WithoutRepetition = TRUE;
                    }
                    break;
                case COM_TRIGGERED_WITHOUT_REPETITION:
                    Com_TxSignalGroupState[signalGroupId].NTimesTransmitNeed = TRUE;
                    Com_TxSignalGroupState[signalGroupId].WithoutRepetition = TRUE;
                    break;
                default:
                    /*do nothing*/
                    break;
            }
            ret = E_OK;
        }
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignalGroup.
 *                     pack the Tx signal group into ipdu Tx buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
FUNC(void,COM_CODE)
Com_TxSignalGroupPack(Com_SignalGroupIdType SignalGroupId)
{
    Com_SignalIdType groupSignalNumber;
    Com_SignalIdType cnt;
    Com_SignalIdType groupSignalId;
    #if((0u < COM_TMCTXGROUPSIGNAL_NUMBER) && (0u < COM_TMCTXSIGNAL_NUMBER))
    Com_SignalIdType groupSignalTMCId;
    #endif
    uint16 length;
    PduIdType txPduId;
    uint32 bitPosition;
    uint16 signalBufferId;
    groupSignalNumber = Com_ConfigStd->ComTxSignalGroup[SignalGroupId].ComGroupSignalNumber;
    for(cnt = 0u;cnt < groupSignalNumber;cnt++)
    {
        groupSignalId = Com_ConfigStd->ComTxSignalGroup[SignalGroupId].ComTxGroupSignalId[cnt];
        #if((0u < COM_TMCTXGROUPSIGNAL_NUMBER) && (0u < COM_TMCTXSIGNAL_NUMBER))
        if(NULL_PTR != Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter)
        {
            groupSignalTMCId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter->ComTMCBufferId;
            TxSignalTMCRunTime[groupSignalTMCId] = TxGroupSignalTMCBuffer[groupSignalTMCId];
        }
        #endif
        if(COM_UINT8_DYN == Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalType)
        {
            txPduId = Com_ConfigStd->ComTxSignalGroup[SignalGroupId].ComIpduRefIndex;
            bitPosition = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComBitPosition;
            signalBufferId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalInitValueId;
            length = Com_TxSignalGroupState[SignalGroupId].DynamicSignalLength;
            Com_TxDynSignalPack(txPduId,bitPosition,signalBufferId,length);
        }
        else
        {
            Com_TxGroupSignalPack(groupSignalId);
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignalGroupArray.
 *                     update every group signal buffer,calculate every TMC,
 *                     return the signal group is changed or not for trigger
 *                     transmit.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxSignalGroupId,GroupSignalNumber,TxIPduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
FUNC(boolean,COM_CODE)
Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(Com_SignalGroupIdType TxSignalGroupId,Com_SignalIdType GroupSignalNumber,uint16 TxIPduBufferId)
{
    boolean triggerOnChange = FALSE;
    Com_SignalIdType groupSignalId;
    Com_SignalType groupSignalType;
    uint16 groupSignalInitValueId;
    uint16 cnt;
    uint64 groupSignalValue;
    #if(0u < COM_TMCTXSIGNAL_NUMBER)
    Com_SignalIdType txSignalTMCId;
    #endif
    for(cnt = 0u;cnt < GroupSignalNumber;cnt++)
    {
        groupSignalId = Com_ConfigStd->ComTxSignalGroup[TxSignalGroupId].ComTxGroupSignalId[cnt];
        groupSignalType = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalType;
        groupSignalInitValueId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalInitValueId;
        if(COM_UINT8_N == groupSignalType)
        {
            #if((0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
            triggerOnChange = Com_TxGroupSignalUpdateUint8N(TxIPduBufferId,groupSignalId,groupSignalInitValueId);
            #endif
        }
        else
        {
            /*UnPack tx group signal from signal group array,used to calculate the TMC of the signal and judge the signal value if changed or not,if changed update the signal value*/
            groupSignalValue = Com_UnPackTxArrayGroupSignalValue(groupSignalId,TxIPduBufferId);
            #if(0u < COM_TMCTXSIGNAL_NUMBER)
            if(NULL_PTR != Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter)
            {
                txSignalTMCId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter->ComTMCBufferId;
                TxSignalTMCRunTime[txSignalTMCId] =
                        Com_TxSignalTMCCalculate(&groupSignalValue, Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter,groupSignalType,groupSignalInitValueId);
            }
            #endif
            switch(groupSignalType)
            {
                #if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
                case COM_BOOLEAN:
                    if(Com_SignalBoolRuntimeBuff[groupSignalInitValueId] != (boolean)groupSignalValue)
                    {
                        Com_SignalBoolRuntimeBuff[groupSignalInitValueId] = (boolean)groupSignalValue;
                        Com_TxArrayGroupSignalTriggerOnChange(groupSignalId,&triggerOnChange);
                    }
                    break;
                #endif
                #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
                case COM_UINT8:
                case COM_SINT8:
                    if(Com_Signal8BitRuntimeBuff[groupSignalInitValueId] != (uint8)groupSignalValue)
                    {
                        Com_Signal8BitRuntimeBuff[groupSignalInitValueId] = (uint8)groupSignalValue;
                        Com_TxArrayGroupSignalTriggerOnChange(groupSignalId,&triggerOnChange);
                    }
                    break;
                #endif
                #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
                case COM_UINT16:
                case COM_SINT16:
                    if(Com_Signal16BitRuntimeBuff[groupSignalInitValueId] != (uint16)groupSignalValue)
                    {
                        Com_Signal16BitRuntimeBuff[groupSignalInitValueId] = (uint16)groupSignalValue;
                        Com_TxArrayGroupSignalTriggerOnChange(groupSignalId,&triggerOnChange);
                    }
                    break;
                #endif
                #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
                case COM_UINT32:
                case COM_SINT32:
                case COM_FLOAT32:
                    if(Com_Signal32BitRuntimeBuff[groupSignalInitValueId] != (uint32)groupSignalValue)
                    {
                        Com_Signal32BitRuntimeBuff[groupSignalInitValueId] = (uint32)groupSignalValue;
                        Com_TxArrayGroupSignalTriggerOnChange(groupSignalId,&triggerOnChange);
                    }
                    break;
                #endif
                #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
                case COM_UINT64:
                case COM_SINT64:
                case COM_FLOAT64:
                    if(Com_Signal64BitRuntimeBuff[groupSignalInitValueId] != (uint64)groupSignalValue)
                    {
                        Com_Signal64BitRuntimeBuff[groupSignalInitValueId] = (uint64)groupSignalValue;
                        Com_TxArrayGroupSignalTriggerOnChange(groupSignalId,&triggerOnChange);
                    }
                    break;
                #endif
                default:
                    /*do nothing*/
                    break;
            }
        }
    }
    return triggerOnChange;
}
#endif/*(STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0)*/
/******************************************************************************/
/*
 * Brief               Called by Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,
 *                     Com_TriggerTransmit,Com_TxConfirmation,Com_TpTxConfirmation,
 *                     Com_MainFunctionTx,Com_TxIpduController.
 *                     clear all signal group/signal/dest description signal
 *                     update bit of the Tx Pdu
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_ClearTxIPduUpdates(PduIdType TxIpduId)
{
    #if(0u < COM_TXIPDUBUFF_SIZE)
    uint16 cnt;
    uint32 updateBitValue = COM_UNUSED_UINT32;
    Com_SignalGroupIdType signalGroupNumber;
    Com_SignalIdType signalNumber;
    Com_SignalIdType gwDestSignalNumber;
    uint16 ipduBufferIndex;
    uint32 updateBitPosition;
    uint16 signalGroupRef;
    uint16 signalRef;
    uint16 DestSignalRef;
    signalGroupNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRefNumber;
    signalNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRefNumber;
    gwDestSignalNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduDestSignalsRefNumber;
    ipduBufferIndex = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxIPduBufIndex;
    for(cnt = 0u;cnt < signalGroupNumber;cnt++)
    {
        signalGroupRef = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRef[cnt];
        updateBitPosition = Com_ConfigStd->ComTxSignalGroup[signalGroupRef].ComUpdateBitPosition;
        if(updateBitValue != updateBitPosition)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (updateBitPosition/8u)] &=
                    (uint8)(~(uint8)(0x01u << (updateBitPosition & 0x07u)));
        }
    }
    for(cnt = 0u;cnt < signalNumber;cnt++)
    {
        signalRef = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRef[cnt];
        updateBitPosition = Com_ConfigStd->ComTxSignal[signalRef].ComUpdateBitPosition;
        if(updateBitValue != updateBitPosition)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (updateBitPosition/8u)] &=
                    (uint8)(~(uint8)(0x01u << (updateBitPosition & 0x07u)));
        }
    }
    for(cnt = 0u;cnt < gwDestSignalNumber;cnt++)
    {
        DestSignalRef =  Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduDestSignalsRef[cnt];
        updateBitPosition = Com_ConfigStd->ComDestSignal[DestSignalRef].ComUpdateBitPosition;
        if(updateBitValue != updateBitPosition)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (updateBitPosition/8u)] &=
                    (uint8)(~(uint8)(0x01u << (updateBitPosition & 0x07u)));
        }
    }
    #endif
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_TxConfirmation,Com_TpTxConfirmation,
 *                     Com_MainFunctionTx.
 *                     Tx notification for all signals/signal groups of the Tx Pdu
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_ConfirmationProcess(PduIdType TxIpduId)
{
    Com_SignalIdType signalNumber;
    Com_SignalGroupIdType siganlGroupNumber;
    uint16 cnt;
    Com_SignalIdType txSignalId;
    Com_SignalGroupIdType txSignalGroupId;
    signalNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < signalNumber;cnt++)
    {
        txSignalId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComTxSignal[txSignalId].ComNotification)
        {
            Com_ConfigStd->ComTxSignal[txSignalId].ComNotification();
        }
    }
    siganlGroupNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < siganlGroupNumber;cnt++)
    {
        txSignalGroupId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComNotification)
        {
            Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComNotification();
        }
    }
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     tx pdu DM timeout notification.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_TxDMTimeOutNotification(PduIdType TxIpduId)
{
    Com_SignalIdType signalNumber;
    Com_SignalGroupIdType siganlGroupNumber;
    uint16 cnt;
    Com_SignalIdType txSignalId;
    Com_SignalGroupIdType txSignalGroupId;
    signalNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < signalNumber;cnt++)
    {
        txSignalId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComTxSignal[txSignalId].ComTimeoutNotification)
        {
            Com_ConfigStd->ComTxSignal[txSignalId].ComTimeoutNotification();
        }
    }
    siganlGroupNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < siganlGroupNumber;cnt++)
    {
        txSignalGroupId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComTimeoutNotification)
        {
            Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComTimeoutNotification();
        }
    }
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     the handle of Com_MainFunctionTx,when the tx mode is direct.
 *                     return the boolean value for the tx pdu need to transmit or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,MDTRun
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(boolean,COM_CODE)
Com_MainFuncTxDirect(PduIdType IPduId, boolean MDTRun)
{
    boolean ret = FALSE;
    if(Com_TxIPduRunTimeState[IPduId].NTimeCnt > 0u)
    {
        (Com_TxIPduRunTimeState[IPduId].NTimeCnt) -= 1u;
    }
    if((0u == Com_TxIPduRunTimeState[IPduId].NTimeCnt)
            && (Com_TxIPduRunTimeState[IPduId].RptNum > 0u)
            && (FALSE == Com_TxIPduRunTimeState[IPduId].Transmiting))
    {
        /*delay transmit of the MDT */
        if(TRUE == MDTRun)
        {
            Com_TxIPduRunTimeState[IPduId].TxDelay = TRUE;
        }
        else
        {
            ret = TRUE;
        }
        /*reset NTimes counter*/
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TMS)
        {
            Com_TxIPduRunTimeState[IPduId].NTimeCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeTrue->ComTxModeRepetitionPeriod;
        }
        else
        {
            Com_TxIPduRunTimeState[IPduId].NTimeCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeFalse->ComTxModeRepetitionPeriod;
        }
    }
    if(FALSE == MDTRun)
    {
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TxDelay)
        {
            ret = TRUE;
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     the handle of Com_MainFunctionTx,when the tx mode is period.
 *                     return the boolean value for the tx pdu need to transmit or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,MDTRun
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(boolean,COM_CODE)
Com_MainFuncTxPeriod(PduIdType IPduId, boolean MDTRun)
{
    boolean ret = FALSE;
    if(Com_TxIPduRunTimeState[IPduId].PeriodCnt > 0u)
    {
        (Com_TxIPduRunTimeState[IPduId].PeriodCnt) -= 1u;
    }
    if(0u == Com_TxIPduRunTimeState[IPduId].PeriodCnt)
    {
        /*delay transmit of the MDT */
        if(TRUE == MDTRun)
        {
            Com_TxIPduRunTimeState[IPduId].TxDelay = TRUE;
        }
        else
        {
            ret = TRUE;
        }
        /*reset Period counter*/
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TMS)
        {
            Com_TxIPduRunTimeState[IPduId].PeriodCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeTrue->ComTxModeTimePeriod;
        }
        else
        {
            Com_TxIPduRunTimeState[IPduId].PeriodCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeFalse->ComTxModeTimePeriod;
        }
    }
    if(FALSE == MDTRun)
    {
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TxDelay)
        {
            ret = TRUE;
        }
    }
    if(TRUE == ret)
    {
        if(0u == Com_TxIPduRunTimeState[IPduId].DMCnt)
        {
            /*reset the tx MD counter*/
            Com_TxIPduRunTimeState[IPduId].DMCnt = Com_ConfigStd->ComTxIPdu[IPduId].ComTxIpduDM;
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx.
 *                     the handle of Com_MainFunctionTx,when the tx mode is mixed.
 *                     return the boolean value for the tx pdu need to transmit or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,MDTRun
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(boolean,COM_CODE)
Com_MainFuncTxMixed(PduIdType IPduId, boolean MDTRun)
{
    boolean ret = FALSE;
    /*DIRECT mode*/
    if(Com_TxIPduRunTimeState[IPduId].NTimeCnt > 0u)
    {
        (Com_TxIPduRunTimeState[IPduId].NTimeCnt) -= 1u;
    }
    if((0u == Com_TxIPduRunTimeState[IPduId].NTimeCnt)
            && (Com_TxIPduRunTimeState[IPduId].RptNum > 0u)
            && (FALSE == Com_TxIPduRunTimeState[IPduId].Transmiting))
    {
        /*delay transmit of the MDT */
        if(TRUE == MDTRun)
        {
            Com_TxIPduRunTimeState[IPduId].TxDelay = TRUE;
        }
        else
        {
            ret = TRUE;
        }
        /*reset NTimes counter*/
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TMS)
        {
            Com_TxIPduRunTimeState[IPduId].NTimeCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeTrue->ComTxModeRepetitionPeriod;
        }
        else
        {
            Com_TxIPduRunTimeState[IPduId].NTimeCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeFalse->ComTxModeRepetitionPeriod;
        }
    }
    /*PERIODIC mode*/
    if(Com_TxIPduRunTimeState[IPduId].PeriodCnt > 0u)
    {
        (Com_TxIPduRunTimeState[IPduId].PeriodCnt) -= 1u;
    }
    if(0u == Com_TxIPduRunTimeState[IPduId].PeriodCnt)
    {
        /*delay transmit of the MDT */
        if(TRUE == MDTRun)
        {
            Com_TxIPduRunTimeState[IPduId].TxDelay = TRUE;
        }
        else
        {
            ret = TRUE;
        }
        /*reset Period counter*/
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TMS)
        {
            Com_TxIPduRunTimeState[IPduId].PeriodCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeTrue->ComTxModeTimePeriod;
        }
        else
        {
            Com_TxIPduRunTimeState[IPduId].PeriodCnt =
                    Com_ConfigStd->ComTxIPdu[IPduId].ComTxModeFalse->ComTxModeTimePeriod;
        }
    }
    if(FALSE == MDTRun)
    {
        if(TRUE == Com_TxIPduRunTimeState[IPduId].TxDelay)
        {
            ret = TRUE;
        }
    }
    if((TRUE == ret) && (0u == Com_TxIPduRunTimeState[IPduId].RptNum))
    {
        if(0u == Com_TxIPduRunTimeState[IPduId].DMCnt)
        {
            /*reset the tx MD counter,when active mode is PERIODIC*/
            Com_TxIPduRunTimeState[IPduId].DMCnt = Com_ConfigStd->ComTxIPdu[IPduId].ComTxIpduDM;
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               Called by Com_TriggerTransmit,Com_TriggerIPDUSend,
 *                     Com_TriggerIPDUSendWithMetaData,Com_TxIpduController.
 *                     It is called in case the transmission is not possible
 *                     because the corresponding I-PDU group is stopped.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_InvokeErrorNotification(PduIdType IpduId)
{
    uint16 cnt;
    Com_SignalIdType ipduSignalNumber;
    Com_SignalIdType signalId;
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType signalGroupId;
    ipduSignalNumber =  Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalsRef[cnt];
        /*call signal Tx Err Notification*/
        if(NULL_PTR != Com_ConfigStd->ComTxSignal[signalId].ComErrorNotification)
        {
            Com_ConfigStd->ComTxSignal[signalId].ComErrorNotification();
        }
    }
    ipduSignalGroupNumber = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        signalGroupId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        /*call signal group Tx Err Notification*/
        if(NULL_PTR != Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComErrorNotification)
        {
            Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComErrorNotification();
        }
    }
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx,Com_TriggerIPDUSend,
 *                     Com_TriggerIPDUSendWithMetaData.
 *                     Pack the counter value in the pdu buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_PackCounterValue(PduIdType TxIpduId)
{
    #if(0u < COM_TXIPDUBUFF_SIZE)
    uint8 txCounter;
    uint16 ipduBufferIndex;
    uint32 bitPosition;
    uint8 bitSize;
    txCounter = Com_TxIPduRunTimeState[TxIpduId].TxIpduCounter;
    ipduBufferIndex = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxIPduBufIndex;
    bitPosition = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduCounter->ComIPduCounterStartPosition;
    bitSize = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduCounter->ComIPduCounterSize;
    /* Align LSB of txData and signal_value in IPdu */
    txCounter = txCounter << (bitPosition & 0x07u);
    /*Clear corresponding bits in IPdu*/
    Com_TxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] &=
            (0xFFu >> (8u - (bitPosition & 0x07u))) | (uint8)(0xFFu << ((bitPosition & 0x07u) + bitSize));
    /*write corresponding bits to IPdu buffer*/
    Com_TxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] |=
            ((0xFFu >> (8u - ((bitPosition & 0x07u) + bitSize))) & txCounter);
    #endif
    return;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               called by Com_TxIpduController.
 *                     reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_ResetTxOccurrenceOfPduFilter(PduIdType IpduId)
{
    uint16 cnt;
    Com_SignalIdType ipduSignalNumber;
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType signalGroupId;
    Com_SignalIdType ipduDestSignalNumber;
    Com_SignalIdType signalId;
    Com_SignalIdType oneEveryNId;
    uint16 counter;
    uint16 groupSignalNumber;
    uint16 groupSignalId;
    /*reset signal/group signal/dest description signal of tx pdu which ComFilterAlgorithm is ONE_EVERY_N*/
    ipduSignalNumber =  Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComTxSignal[signalId].ComFilter)
        {
            oneEveryNId = Com_ConfigStd->ComTxSignal[signalId].ComFilter->ComOneEveryNId;
            if(COM_UNUSED_UINT16 != oneEveryNId)
            {
                Com_OneEveryNcnt[oneEveryNId] = 0u;
            }
        }
    }
    ipduDestSignalNumber = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduDestSignalsRefNumber;
    for(cnt = 0u;cnt < ipduDestSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduDestSignalsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComDestSignal[signalId].ComFilter)
        {
            oneEveryNId = Com_ConfigStd->ComDestSignal[signalId].ComFilter->ComOneEveryNId;
            if(COM_UNUSED_UINT16 != oneEveryNId)
            {
                Com_OneEveryNcnt[oneEveryNId] = 0u;
            }
        }
    }
    ipduSignalGroupNumber = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        signalGroupId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        groupSignalNumber = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComGroupSignalNumber;
        for(counter = 0u;counter < groupSignalNumber;counter++)
        {
            groupSignalId = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComTxGroupSignalId[counter];
            if(NULL_PTR != Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter)
            {
                oneEveryNId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter->ComOneEveryNId;
                if(COM_UNUSED_UINT16 != oneEveryNId)
                {
                    Com_OneEveryNcnt[oneEveryNId] = 0u;
                }
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_TxIpduController.
 *                     init the tx ipdu buffer,all signal buffer
 *                     (included in the ipdu) and init the TMC.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_IPDUGROUP_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_ResetTxPduBufferAndSignalBuffer(PduIdType IpduId)
{
    #if(0u < COM_TXIPDUBUFF_SIZE)
    uint16 pduBufferId;
    uint16 initPduLength;
    #endif
    uint16 cnt;
    #if(0u < COM_TMCTXSIGNAL_NUMBER)
    Com_SignalIdType txSignalTMCId;
    #endif
    Com_SignalIdType ipduSignalNumber;
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType signalGroupId;
    Com_SignalIdType ipduDestSignalNumber;
    Com_SignalIdType signalId;
    uint16 signalInitValueId;
    Com_SignalType signalType;
    uint16 signalLength;
    uint16 counter;
    uint16 groupSignalNumber;
    uint16 groupSignalId;
    /*init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC*/
    #if(0u < COM_TXIPDUBUFF_SIZE)
    pduBufferId = Com_ConfigStd->ComTxIPdu[IpduId].ComTxIPduBufIndex;
    initPduLength = Com_TxIPduInitState[IpduId].TxIpduLength;
    for(cnt = 0u; cnt < initPduLength;cnt++)
    {
        Com_TxIPduRuntimeBuff[pduBufferId + cnt] = Com_TxIPduInitValue[pduBufferId + cnt];
    }
    #endif
    /*init signal buffer and TMC*/
    ipduSignalNumber =  Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalsRef[cnt];
        signalInitValueId = Com_ConfigStd->ComTxSignal[signalId].ComSignalInitValueId;
        signalType = Com_ConfigStd->ComTxSignal[signalId].ComSignalType;
        if(COM_UINT8_DYN == signalType)
        {
            signalLength = Com_ConfigStd->ComTxSignal[signalId].ComSignalDataInitValueLength;
        }
        else if(COM_UINT8_N == signalType)
        {
            signalLength = Com_ConfigStd->ComTxSignal[signalId].ComSignalLength;
        }
        else
        {
            signalLength = 0u;
        }
        Com_InitSignalBuffer(signalType,signalInitValueId,signalLength);
        #if(0u < COM_TMCTXSIGNAL_NUMBER)
        if(NULL_PTR != Com_ConfigStd->ComTxSignal[signalId].ComFilter)
        {
            txSignalTMCId = Com_ConfigStd->ComTxSignal[signalId].ComFilter->ComTMCBufferId;
            TxSignalTMCRunTime[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
        }
        #endif
    }
    /*init dest description signal buffer and TMC*/
    ipduDestSignalNumber = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduDestSignalsRefNumber;
    for(cnt = 0u;cnt < ipduDestSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduDestSignalsRef[cnt];
        signalInitValueId = Com_ConfigStd->ComDestSignal[signalId].ComSignalInitValueId;
        signalType = Com_ConfigStd->ComDestSignal[signalId].ComSignalType;
        if(COM_UINT8_DYN == signalType)
        {
            signalLength = Com_ConfigStd->ComDestSignal[signalId].ComSignalDataInitValueLength;
        }
        else if(COM_UINT8_N == signalType)
        {
            signalLength = Com_ConfigStd->ComDestSignal[signalId].ComSignalLength;
        }
        else
        {
            signalLength = 0u;
        }
        Com_InitSignalBuffer(signalType,signalInitValueId,signalLength);
        #if(0u < COM_TMCTXSIGNAL_NUMBER)
        if(NULL_PTR != Com_ConfigStd->ComDestSignal[signalId].ComFilter)
        {
            txSignalTMCId = Com_ConfigStd->ComDestSignal[signalId].ComFilter->ComTMCBufferId;
            TxSignalTMCRunTime[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
        }
        #endif
    }
    /*init group signal buffer and TMC*/
    ipduSignalGroupNumber = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        signalGroupId = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        groupSignalNumber = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComGroupSignalNumber;
        for(counter = 0u;counter < groupSignalNumber;counter++)
        {
            groupSignalId = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComTxGroupSignalId[counter];
            signalInitValueId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalInitValueId;
            signalType = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalType;
            if(COM_UINT8_DYN == signalType)
            {
                signalLength = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalDataInitValueLength;
            }
            else if(COM_UINT8_N == signalType)
            {
                signalLength = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalLength;
            }
            else
            {
                signalLength = 0u;
            }
            Com_InitSignalBuffer(signalType,signalInitValueId,signalLength);
            #if((0u < COM_TMCTXSIGNAL_NUMBER) && (0u < COM_TMCTXGROUPSIGNAL_NUMBER))
            if(NULL_PTR != Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter)
            {
                txSignalTMCId = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter->ComTMCBufferId;
                TxGroupSignalTMCBuffer[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
                TxSignalTMCRunTime[txSignalTMCId] = Com_TxSignalInitTMC[txSignalTMCId];
            }
            #endif
        }
    }
    Com_TxIPduRunTimeState[IpduId].TMS = Com_TxIPduInitState[IpduId].TMS;
    Com_TxIPduRunTimeState[IpduId].ipduTxMode = Com_TxIPduInitState[IpduId].ipduTxMode;
    Com_TxIPduRunTimeState[IpduId].TxConfirm = FALSE;
    Com_TxIPduRunTimeState[IpduId].NTimeCnt = 0u;
    Com_TxIPduRunTimeState[IpduId].RptNum = 0u;
    Com_TxIPduRunTimeState[IpduId].Transmiting = FALSE;
    Com_TxIPduRunTimeState[IpduId].TxDelay = FALSE;
    Com_TxIPduRunTimeState[IpduId].TxIpduLength = Com_TxIPduInitState[IpduId].TxIpduLength;
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_TxSignalGroupPack.
 *                     pack tx group signal(not dynamic).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      GroupSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_TXSIGNALGROUP_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_TxGroupSignalPack(Com_SignalIdType GroupSignalId)
{
    Com_SignalGroupIdType signalGroupRef;
    PduIdType txIpduId;
    uint32 bitPosition;
    Com_SignalType signalType;
    uint16 ipduBufferIndex;
    Com_TxSignalPackType parameter;
    uint16 signalBufferId;
    #if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
    uint8 value = 0x01u;
    #endif
    #if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
    uint16 signalLength;
    uint16 cnt;
    #endif
    uint64 txData;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 bitSize;
    signalGroupRef = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComSignalGroupRef;
    txIpduId = Com_ConfigStd->ComTxSignalGroup[signalGroupRef].ComIpduRefIndex;
    bitPosition = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComBitPosition;
    signalType = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComSignalType;
    ipduBufferIndex = Com_ConfigStd->ComTxIPdu[txIpduId].ComTxIPduBufIndex;
    signalBufferId = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComSignalInitValueId;
    /*pack boolean type group signal*/
    if(COM_BOOLEAN == signalType)
    {
        #if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
        if(TRUE == Com_SignalBoolRuntimeBuff[signalBufferId])
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)] |=
                    (uint8)(value<<((uint8)bitPosition & 0x07u));
        }
        else
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)] &=
                    (uint8)(~((uint8)(value<<((uint8)bitPosition & 0x07u))));
        }
        #endif
    }
    /*pack COM_UINT8_N type group signal*/
    else if(COM_UINT8_N == signalType)
    {
        #if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_TXIPDUBUFF_SIZE))
        signalLength = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComSignalLength;
        for(cnt = 0u;cnt < signalLength;cnt++)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u) + cnt] =
                    Com_Signal8BitRuntimeBuff[signalBufferId + cnt];
        }
        #endif
    }
    /*pack group signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
    else
    {
        /*get the group signal value need to be packed*/
        switch(signalType)
        {
            #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
            case COM_UINT8:
            case COM_SINT8:
                txData = (uint64)(Com_Signal8BitRuntimeBuff[signalBufferId]);
                break;
            #endif
            #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
            case COM_UINT16:
            case COM_SINT16:
                txData = (uint64)(Com_Signal16BitRuntimeBuff[signalBufferId]);
                break;
            #endif
            #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                txData = (uint64)(Com_Signal32BitRuntimeBuff[signalBufferId]);
                break;
            #endif
            #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                txData = Com_Signal64BitRuntimeBuff[signalBufferId];
                break;
            #endif
            default:
                /*reach here,the signal type is error*/
                txData = 0u;
                break;
        }
        /* group signal MSB byte value used for the signal value bits cover 9 bytes */
        dataMSBByte = txData;
        /* Align LSB of txData and signal_value in IPdu */
        txData = txData << (bitPosition & 0x07u);
        bitSize = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComBitSize;
        /*signalByteLength:1-9*/
        signalByteLength = 1u + ((bitSize + ((uint8)bitPosition & 0x07u) - 1u)/8u);
        /*the tx signal all value bits are in one byte*/
        if(1u == signalByteLength)
        {
            #if(0u < COM_TXIPDUBUFF_SIZE)
            /*Clear corresponding bits in IPdu*/
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)] &=
                    (0xFFu >> (8u - ((uint8)bitPosition & 0x07u))) | (uint8)(0xFFu << (((uint8)bitPosition & 0x07u) + bitSize));
            /*write corresponding bits to IPdu buffer*/
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)] |=
                    (0xFFu >> (8u - (((uint8)bitPosition & 0x07u) + bitSize))) & ((uint8)txData);
            #endif
        }
        /*the tx signal all value bits are included in 2-9 byte*/
        else if((1u < signalByteLength) && (signalByteLength < 10u))
        {
            parameter.SignalByteLength = signalByteLength;
            parameter.SignalEndianness = Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComSignalEndianness;
            parameter.IpduBufferIndex = ipduBufferIndex;
            parameter.BitPosition = bitPosition;
            parameter.BitSize = bitSize;
            Com_TxSignalPackHandle(parameter,txData,dataMSBByte);
        }
        /*this case will not happen*/
        else
        {
            /*do nothing*/
        }
    }
    return;
}
#endif/* COM_TXSIGNALGROUP_NUMBER > 0u */
/******************************************************************************/
/*
 * Brief               called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 *                     pack tx group signal(not dynamic).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
static FUNC(uint64,COM_CODE)
Com_UnPackTxArrayGroupSignalValue(Com_SignalIdType SignalId,uint16 IpduBufferId)
{
    uint64 signalValue;
    #if(0u < COM_TXIPDUBUFF_SIZE)
    uint32 bitPosition;
    Com_SignalType signalType;
    Com_SignalEndiannessType signalEndianness;
    uint16 ipduBufferIndex;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 bitSize;
    uint64 initValue = COM_UNUSED_UINT64;
    bitPosition = Com_ConfigStd->ComTxGroupSignal[SignalId].ComBitPosition;
    signalType = Com_ConfigStd->ComTxGroupSignal[SignalId].ComSignalType;
    signalEndianness = Com_ConfigStd->ComTxGroupSignal[SignalId].ComSignalEndianness;
    if(COM_BOOLEAN == signalType)
    {
        signalValue = (uint64)((uint8)(((Com_TxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u)]) >> (bitPosition & 0x07u)) & 0x01u));
    }
    else
    {
        bitSize = Com_ConfigStd->ComTxGroupSignal[SignalId].ComBitSize;
        ipduBufferIndex = IpduBufferId + (uint16)(bitPosition/8u);
        /*the length value:1-9*/
        signalByteLength = 1u + ((bitSize + (uint8)(bitPosition & 0x07u) - 1u)/8u);
        /*get all byte value(group signal/signal cover,except MSB byte)*/
        signalValue = Com_TxArrayGroupSignalUnPackHandle(signalEndianness,signalByteLength,&ipduBufferIndex);
        dataMSBByte =(uint64)((uint8)(Com_TxIPduRuntimeBuff[ipduBufferIndex] & (0xffu >> (7u - (uint8)((bitPosition + bitSize - 1u) & 0x07u)))));
        switch(signalType)
        {
            case COM_SINT8:
            case COM_SINT16:
            case COM_SINT32:
            case COM_SINT64:
                if(0u < (dataMSBByte >> ((uint8)((bitPosition + bitSize - 1u) & 0x07u))))
                {
                     /* need flag extension */
                    dataMSBByte |= (initValue << ((uint8)((bitPosition + bitSize - 1u) & 0x07u)));
                }
                break;
            default:
                /*do nothing*/
                break;
        }
        switch(signalByteLength)
        {
            case (uint8)0x01u:
                if(dataMSBByte >= 0x8000000000000000u)
                {
                    signalValue = 0xff00000000000000u | (dataMSBByte >> ((uint8)(bitPosition & 0x07u)));
                }
                else
                {
                    signalValue = dataMSBByte >> ((uint8)(bitPosition & 0x07u));
                }
                break;
            case (uint8)0x02u:
            case (uint8)0x03u:
            case (uint8)0x04u:
            case (uint8)0x05u:
            case (uint8)0x06u:
            case (uint8)0x07u:
            case (uint8)0x08u:
            case (uint8)0x09u:
            signalValue = signalValue >> ((uint8)(bitPosition & 0x07u));
            signalValue |= (dataMSBByte << (((signalByteLength - 1u)*8u) - ((uint8)(bitPosition & 0x07u))));
                break;
            default:
                /*do nothing*/
                break;
        }
    }
    #endif
    return signalValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_UnPackTxArrayGroupSignalValue.
 *                     unpack all byte value(group signal cover,except MSB byte).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalEndianness,SignalByteLength
 * Param-Name[out]     None
 * Param-Name[in/out]  IpduBufferIndex
 * Return              uint64
 */
/******************************************************************************/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u) && (0u < COM_TXIPDUBUFF_SIZE))
static FUNC(uint64,COM_CODE)
Com_TxArrayGroupSignalUnPackHandle(
        Com_SignalEndiannessType SignalEndianness,
        uint8 SignalByteLength,
        uint16* IpduBufferIndex)
{
    uint64 signalValue = 0u;
    uint16 ipduIndex = *IpduBufferIndex;
    switch(SignalByteLength)
    {
        case (uint8)0x01u:
            break;
        case (uint8)0x02u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x03u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x04u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x05u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x06u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x07u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 40u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x08u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 40u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 48u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x09u:
            signalValue = (uint64)Com_TxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 40u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 48u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_TxIPduRuntimeBuff[ipduIndex]) << 56u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        default:
            /*do nothing*/
            break;
    }
    return signalValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 *                     the group signal value changed,judge it will trigger
 *                     the TxPdu transmit(DIRECT/MIXED mode) or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      GroupSignalId
 * Param-Name[out]     TriggerOnChange
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
static FUNC(void,COM_CODE)
Com_TxArrayGroupSignalTriggerOnChange(Com_SignalIdType GroupSignalId,boolean* TriggerOnChange)
{
    if(TRUE == Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComTxGroupSignalUsedTransferProperty)
    {
        if(COM_TRIGGERED_ON_CHANGE == Com_ConfigStd->ComTxGroupSignal[GroupSignalId].ComTransferProperty)
        {
            *TriggerOnChange = TRUE;
        }
    }
    else
    {
        *TriggerOnChange = TRUE;
    }
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_TxSignalTMHandle,Com_SendTxGroupSignalHandle,
 *                     Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 *                     the group signal value changed,judge it will trigger
 *                     calculate the new TMC of the signal new value.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalValuePtr,ComFilter,SignalType,SignalBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_TxSignalTMCCalculate
(
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
    Com_SignalType SignalType,
    uint16 SignalBufferId
)
{
    boolean ret = FALSE;
    Com_FilterAlgorithmType filterType;
    uint64 signalNewValue;
    #if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    Com_SignalIdType oneEveryNId;
    uint32 filterOffset;
    uint32 filterPeriod;
    #endif
    filterType = ComFilter->ComFilterAlgorithm;
    /*COM_UINT8_N just support ALWAYS and NEVER FilterAlgorithm*/
    if(COM_UINT8_N == SignalType)
    {
        if(COM_ALWAYS == filterType)
        {
            ret = TRUE;
        }
    }
    else
    {
        /*tx signal changed to uint64 type*/
        signalNewValue = Com_GetTxSignalValue(SignalType,SignalValuePtr);
        switch(filterType)
        {
            case COM_ALWAYS:
                ret = TRUE;
                break;
            case COM_MASKED_NEW_EQUALS_X:
                if(((signalNewValue) & (ComFilter->ComFilterMask))
                    == (ComFilter->ComFilterX))
                {
                    ret = TRUE;
                }
                break;
            case COM_MASKED_NEW_DIFFERS_X:
                if(((signalNewValue) & (ComFilter->ComFilterMask))
                    != (ComFilter->ComFilterX))
                {
                    ret = TRUE;
                }
                break;
            /*COM_BOOLEAN type not support WITHIN and WITHOUT*/
            case COM_NEW_IS_WITHIN:
                Com_NewIsWithinFilterCalculate(SignalType,signalNewValue,ComFilter,&ret);
                break;
            /*COM_BOOLEAN type not support WITHIN and WITHOUT*/
            case COM_NEW_IS_OUTSIDE:
                Com_NewIsWithoutFilterCalculate(SignalType,signalNewValue,ComFilter,&ret);
                break;
            #if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
            case COM_ONE_EVERY_N:
                filterOffset = ComFilter->ComFilterOffset;
                filterPeriod = ComFilter->ComFilterPeriod;
                oneEveryNId = ComFilter->ComOneEveryNId;
                if(filterPeriod == Com_OneEveryNcnt[oneEveryNId])
                {
                    Com_OneEveryNcnt[oneEveryNId] = 0u;
                }
                if(filterOffset == Com_OneEveryNcnt[oneEveryNId])
                {
                    ret = TRUE;
                }
                Com_OneEveryNcnt[oneEveryNId]++;
                break;
            #endif
            case COM_MASKED_NEW_DIFFERS_MASKED_OLD:
                ret = Com_MaskedNewDiffersMaskedOldFilterCalculate(SignalType,signalNewValue,ComFilter,SignalBufferId);
                break;
            default:/*include NEVER*/
                /*do nothing*/
                break;
        }
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_TxSignalTMCCalculate.
 *                     change the signal value to uint64 type.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValuePtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER))
static FUNC(uint64,COM_CODE)
Com_GetTxSignalValue(Com_SignalType SignalType,P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalValuePtr)
{
    uint64 signalNewValue = 0u;
    switch(SignalType)
    {
        case COM_BOOLEAN:
            signalNewValue = (uint64)(*((const boolean*)SignalValuePtr));
            break;
        case COM_UINT8:
        case COM_SINT8:
            signalNewValue = (uint64)(*((const uint8*)SignalValuePtr));
            break;
        case COM_SINT16:
        case COM_UINT16:
            signalNewValue = (uint64)(*((const uint16*)SignalValuePtr));
            break;
        case COM_SINT32:
        case COM_UINT32:
            signalNewValue = (uint64)(*((const uint32*)SignalValuePtr));
            break;
        case COM_SINT64:
        case COM_UINT64:
            signalNewValue = (*((const uint64*)SignalValuePtr));
            break;
        default:
            /*do nothing*/
            break;
    }
    return signalNewValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 *                     update the uint8_n signal value.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIPduBufferId,groupSignalId,groupSignalInitValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
#if((0u < COM_TXIPDUBUFF_SIZE) && (0u < COM_SIGNAL_8BITBUFF_SIZE))
static FUNC(boolean,COM_CODE)
Com_TxGroupSignalUpdateUint8N(
        uint16 TxIPduBufferId,
        Com_SignalIdType groupSignalId,
        uint16 groupSignalInitValueId)
{
    boolean triggerOnChange = FALSE;
    uint16 counter;
    uint32 bitPosition;
    uint16 groupSignalLength;
    groupSignalLength = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComSignalLength;
    bitPosition = Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComBitPosition;
    for(counter = 0u;counter < groupSignalLength;counter++)
    {
        if(Com_TxIPduRuntimeBuff[TxIPduBufferId + (uint16)(bitPosition/8u) + counter] !=
                Com_Signal8BitRuntimeBuff[groupSignalInitValueId + counter])
        {
            /*if the group signal changed,then update the signal buffer*/
            Com_Signal8BitRuntimeBuff[groupSignalInitValueId + counter] =
                    Com_TxIPduRuntimeBuff[TxIPduBufferId + (uint16)(bitPosition/8u) + counter];
            Com_TxArrayGroupSignalTriggerOnChange(groupSignalId,&triggerOnChange);
        }
    }
    return triggerOnChange;
}
#endif
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif/*0u < COM_TXIPDU_NUMBER*/





