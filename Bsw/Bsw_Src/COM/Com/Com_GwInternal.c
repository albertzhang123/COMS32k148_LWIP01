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
**  FILENAME    : Com_GwInternal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : internal implementation for COM gateway signal              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
#if((0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER))
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* called by Com_GwSignal.
 * gateway signal/group signal/description signal which signal type isn't COM_UINT8_N and COM_UINT8_DYN*/
static FUNC(void,COM_CODE)
Com_GwNotArraySignal(Com_SignalType SignalType,uint16 GWSignalBufferId,Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId);
/* called by Com_GwSignal.
 * gateway signal/group signal/description signal which signal type is COM_UINT8_N*/
#if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
static FUNC(void,COM_CODE)
Com_GwUint8NSignal(uint16 SignalBufferId,uint16 SignalLength,Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId);
#endif
/* called by Com_GwSignal.
 * gateway signal/group signal/description signal which signal type is COM_UINT8_DYN*/
#if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
static FUNC(void,COM_CODE)
Com_GwUint8DYNSignal(uint16 SignalBufferId,uint16 SignalLength,Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId);
#endif
/* called by Com_GwNotArraySignal.
 * gateway dest signal is packed to dest tx pdu buffer*/
#if(0u < COM_TXIPDUBUFF_SIZE)
static FUNC(void,COM_CODE)
Com_DestSignalPack(Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId, uint64 SignalValue);
#endif
/* called by Com_GwNotArraySignal.
 * calculate dest signal/group signal/description signal TMC*/
#if (0u < COM_TMCTXSIGNAL_NUMBER)
static FUNC(boolean,COM_CODE)
Com_DestSignalTMCCalculate
(
    Com_SignalType SignalType,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST)ComFilter,
    uint64 NewSignalValue,
    uint64 OldSignalValue
);
#endif
/* called by Com_GwNotArraySignal,Com_GwUint8NSignal,Com_GwUint8DYNSignal.
 * gateway pdu(which include gateway signal) handle*/
#if((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_DST_SIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_GwSignalOfPduHandle(PduIdType TxPduId,Com_TransferPropertyType SignalTxProperty,uint16 RptNum,boolean ValueChanged);
#endif
/* called by Com_GwNotArraySignal,Com_GwUint8NSignal,Com_GwUint8DYNSignal.
 * gateway pdu(which include gateway group signal) handle*/
#if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
static FUNC(void,COM_CODE)
Com_GwGroupSignalOfPduHandle(Com_SignalIdType TxGroupSignalId,uint16 RptNum,boolean ValueChanged);
#endif
/* called by Com_GwUint8DYNSignal.
 * gateway uint8_dyn signal handle*/
#if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
static FUNC(uint16,COM_CODE)
Com_GwUint8DYNSignalHandle(
        uint16 SignalBufferId,
        uint16 SignalLength,
        Com_GwUint8DynSignalType parameter,
        boolean* valueChanged);
#endif
/*Called by Com_GwNotArraySignal.
 *Get the GW Signal's New value and Old value*/
static FUNC(uint64,COM_CODE)
Com_GwSignalNewOldValueGet(
        Com_SignalType SignalType,
        uint16 GWSignalBufferId,
        uint16 txSignalValueId,
        uint64* sourceSignalValue);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionRouteSignals.
 *                     gateway signal/group signal/signal group/description signal
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      GWMappingId,GWSourceSignalType,GWSourceSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_GwSignal(Com_SignalIdType GWMappingId,Com_GWSignalType GWSourceSignalType,Com_SignalIdType GWSourceSignalId)
{
    Com_SignalIdType gwDestNumber;
    Com_SignalIdType gwDestCounter;
    Com_SignalType signalType;
    uint16 gwSignalBufferId;
    Com_GWSignalType gwDestSignalType;
    Com_SignalIdType gwDestSignalId;
    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    uint16 signalLength;
    uint16 rxIpduId;
    #endif
    #if((0u < COM_RXGROUPSIGNAL_NUMBER) && (0u < COM_GWSIGNAL_8BITBUFF_SIZE))
    Com_SignalGroupIdType signalGroupId;
    #endif
    gwDestNumber = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDestNumber;
    switch(GWSourceSignalType)
    {
        #if(0u < COM_SOURCE_SIGNAL_NUMBER)
        case COM_DESCRIPTION:
            signalType = Com_ConfigStd->ComSourceSignal[GWSourceSignalId].ComSignalType;
            gwSignalBufferId = Com_ConfigStd->ComSourceSignal[GWSourceSignalId].GWSignalBufferId;
            for(gwDestCounter = 0u;gwDestCounter < gwDestNumber;gwDestCounter++)
            {
                gwDestSignalType = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDest[gwDestCounter].ComGwDestSignalType;
                gwDestSignalId = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDest[gwDestCounter].ComGwDestSignalId;
                switch(signalType)
                {
                    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
                    case COM_UINT8_N:
                        signalLength = Com_ConfigStd->ComSourceSignal[GWSourceSignalId].ComSignalLength;
                        Com_GwUint8NSignal(gwSignalBufferId,signalLength,gwDestSignalType,gwDestSignalId);
                        break;
                    case COM_UINT8_DYN:
                        rxIpduId = Com_ConfigStd->ComSourceSignal[GWSourceSignalId].ComGwIPduRef;
                        signalLength = Com_RxIPduRunTimeState[rxIpduId].GWDynamicSignalLength;
                        Com_GwUint8DYNSignal(gwSignalBufferId,signalLength,gwDestSignalType,gwDestSignalId);
                        break;
                    #endif
                    default:
                        /*Except COM_UINT8_N,COM_UINT8_DYN Type Signal*/
                        Com_GwNotArraySignal(signalType,gwSignalBufferId,gwDestSignalType,gwDestSignalId);
                        break;
                }
            }
            break;
        #endif/*0u < COM_SOURCE_SIGNAL_NUMBER*/
        #if(0u < COM_RXSIGNAL_NUMBER)
        case COM_SIGNAL:
            signalType = Com_ConfigStd->ComRxSignal[GWSourceSignalId].ComSignalType;
            gwSignalBufferId = Com_ConfigStd->ComRxSignal[GWSourceSignalId].GWSignalBufferId;
            for(gwDestCounter = 0u;gwDestCounter < gwDestNumber;gwDestCounter++)
            {
                gwDestSignalType = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDest[gwDestCounter].ComGwDestSignalType;
                gwDestSignalId = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDest[gwDestCounter].ComGwDestSignalId;
                switch(signalType)
                {
                    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
                    case COM_UINT8_N:
                        signalLength = Com_ConfigStd->ComRxSignal[GWSourceSignalId].ComSignalLength;
                        Com_GwUint8NSignal(gwSignalBufferId,signalLength,gwDestSignalType,gwDestSignalId);
                        break;
                    case COM_UINT8_DYN:
                        rxIpduId = Com_ConfigStd->ComRxSignal[GWSourceSignalId].ComIpduRefIndex;
                        signalLength = Com_RxIPduRunTimeState[rxIpduId].GWDynamicSignalLength;
                        Com_GwUint8DYNSignal(gwSignalBufferId,signalLength,gwDestSignalType,gwDestSignalId);
                        break;
                    #endif
                    default:
                        /*Except COM_UINT8_N,COM_UINT8_DYN Type Signal*/
                        Com_GwNotArraySignal(signalType,gwSignalBufferId,gwDestSignalType,gwDestSignalId);
                        break;
                }
            }
            break;
        #endif
        #if(0u < COM_RXGROUPSIGNAL_NUMBER)
        case COM_GROUP_SIGNAL:
            signalType = Com_ConfigStd->ComRxGroupSignal[GWSourceSignalId].ComSignalType;
            gwSignalBufferId = Com_ConfigStd->ComRxGroupSignal[GWSourceSignalId].GWSignalBufferId;
            for(gwDestCounter = 0u;gwDestCounter < gwDestNumber;gwDestCounter++)
            {
                gwDestSignalType = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDest[gwDestCounter].ComGwDestSignalType;
                gwDestSignalId = Com_ConfigStd->ComGwMapping[GWMappingId].ComGwDest[gwDestCounter].ComGwDestSignalId;
                switch(signalType)
                {
                    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
                    case COM_UINT8_N:
                        signalLength = Com_ConfigStd->ComRxGroupSignal[GWSourceSignalId].ComSignalLength;
                        Com_GwUint8NSignal(gwSignalBufferId,signalLength,gwDestSignalType,gwDestSignalId);
                        break;
                    case COM_UINT8_DYN:
                        signalGroupId = Com_ConfigStd->ComRxGroupSignal[GWSourceSignalId].ComSignalGroupRef;
                        rxIpduId = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComIpduRefIndex;
                        signalLength = Com_RxIPduRunTimeState[rxIpduId].GWDynamicSignalLength;
                        Com_GwUint8DYNSignal(gwSignalBufferId,signalLength,gwDestSignalType,gwDestSignalId);
                        break;
                    #endif
                    default:
                        /*Except COM_UINT8_N,COM_UINT8_DYN Type Signal*/
                        Com_GwNotArraySignal(signalType,gwSignalBufferId,gwDestSignalType,gwDestSignalId);
                        break;
                }
            }
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
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
 * Brief               Called by Com_GwSignal.
 *                     gateway signal/group signal/description signal which
 *                     signal type isn't COM_UINT8_N and COM_UINT8_DYN
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,GWSignalBufferId,DestSignalType,
 *                     DestSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,COM_CODE)
Com_GwNotArraySignal(Com_SignalType SignalType,uint16 GWSignalBufferId,Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId)
{
    boolean valueChanged = FALSE;
    uint64 sourceSignalValue = 0u;
    uint64 destSignalValue;
    PduIdType txPduId = 0u;
    uint16 txSignalValueId = 0u;
    uint32 updateBitPosition = COM_UNUSED_UINT32;
    Com_TransferPropertyType signalTxProperty;
    #if (0u < COM_TMCTXSIGNAL_NUMBER)
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST)comFilter = NULL_PTR;
    #endif
    uint16 rptNum;
    #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
    Com_SignalGroupIdType signalGroupId;
    #endif
    switch(DestSignalType)
    {
        #if(0u < COM_TXSIGNAL_NUMBER)
        case COM_SIGNAL:
            txSignalValueId = Com_ConfigStd->ComTxSignal[DestSignalId].ComSignalInitValueId;
            txPduId = Com_ConfigStd->ComTxSignal[DestSignalId].ComIpduRefIndex;
            updateBitPosition = Com_ConfigStd->ComTxSignal[DestSignalId].ComUpdateBitPosition;
            signalTxProperty = Com_ConfigStd->ComTxSignal[DestSignalId].ComTransferProperty;
            #if (0u < COM_TMCTXSIGNAL_NUMBER)
            comFilter = Com_ConfigStd->ComTxSignal[DestSignalId].ComFilter;
            #endif
            break;
        #endif
        #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
        case COM_GROUP_SIGNAL:
            txSignalValueId = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalInitValueId;
            signalGroupId = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalGroupRef;
            txPduId = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComIpduRefIndex;
            updateBitPosition = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComUpdateBitPosition;
            #if (0u < COM_TMCTXSIGNAL_NUMBER)
            comFilter = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComFilter;
            #endif
            break;
        #endif
        #if(0u < COM_DST_SIGNAL_NUMBER)
        case COM_DESCRIPTION:
            txSignalValueId = Com_ConfigStd->ComDestSignal[DestSignalId].ComSignalInitValueId;
            txPduId = Com_ConfigStd->ComDestSignal[DestSignalId].ComGwIPduRef;
            updateBitPosition = Com_ConfigStd->ComDestSignal[DestSignalId].ComUpdateBitPosition;
            signalTxProperty = Com_ConfigStd->ComDestSignal[DestSignalId].ComTransferProperty;
            #if (0u < COM_TMCTXSIGNAL_NUMBER)
            comFilter = Com_ConfigStd->ComDestSignal[DestSignalId].ComFilter;
            #endif
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    destSignalValue = Com_GwSignalNewOldValueGet(
            SignalType,
            GWSignalBufferId,
            txSignalValueId,
            &sourceSignalValue);
    if(sourceSignalValue != destSignalValue)
    {
        valueChanged = TRUE;
        /*pack the new value*/
        #if(0u < COM_TXIPDUBUFF_SIZE)
        Com_DestSignalPack(DestSignalType,DestSignalId,sourceSignalValue);
        #endif
    }
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(txPduId,updateBitPosition);
    #if (0u < COM_TMCTXSIGNAL_NUMBER)
    if(NULL_PTR != comFilter)
    {
        /*calculate the TMC of the signal*/
        TxSignalTMCRunTime[comFilter->ComTMCBufferId] =
                Com_DestSignalTMCCalculate(SignalType,comFilter,sourceSignalValue,destSignalValue);
    }
    #endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txPduId,&rptNum);
    switch(DestSignalType)
    {
        #if(0u < COM_TXSIGNAL_NUMBER)
        case COM_SIGNAL:
            /*handle the tx pdu (which include the gateway signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txPduId,signalTxProperty,rptNum,valueChanged);
            /*the tx signal configuration timeout*/
            if(0u != Com_ConfigStd->ComTxSignal[DestSignalId].ComTimeout)
            {
                if((0u == Com_TxIPduRunTimeState[txPduId].DMCnt) || (1u < Com_TxIPduRunTimeState[txPduId].RptNum))
                {
                    Com_TxIPduRunTimeState[txPduId].DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
            break;
        #endif
        #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
        case COM_GROUP_SIGNAL:
            /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
            Com_GwGroupSignalOfPduHandle(DestSignalId,rptNum,valueChanged);
            /*the tx signal group(include the gateway group signal)configuration timeout*/
            if(0u != Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComTimeout)
            {
                if((0u == Com_TxIPduRunTimeState[txPduId].DMCnt) || (1u < Com_TxIPduRunTimeState[txPduId].RptNum))
                {
                    Com_TxIPduRunTimeState[txPduId].DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
            break;
        #endif
        #if(0u < COM_DST_SIGNAL_NUMBER)
        case COM_DESCRIPTION:
            /*handle the tx pdu (which include the gateway signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txPduId,signalTxProperty,rptNum,valueChanged);
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_GwSignal.
 *                     gateway signal/group signal/description signal which
 *                     signal type is COM_UINT8_N
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalBufferId,SignalLength,DestSignalType,
 *                     DestSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* called by Com_GwSignal.
 * gateway signal/group signal/description signal which signal type is COM_UINT8_N*/
#if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
static FUNC(void,COM_CODE)
Com_GwUint8NSignal(uint16 SignalBufferId,uint16 SignalLength,Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId)
{
    #if(0u < COM_TXIPDUBUFF_SIZE)
    boolean valueChanged = FALSE;
    uint16 cnt;
    PduIdType txPduId;
    uint16 txIPduBufId;
    uint32 destSignalBitPosition;
    uint32 updateBitPosition;
    uint16 rptNum;
    #if((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_DST_SIGNAL_NUMBER))
    Com_TransferPropertyType signalTxProperty;
    #endif
    #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (COM_TXSIGNALGROUP_NUMBER))
    Com_SignalGroupIdType txSignalGroupId;
    #endif
    switch(DestSignalType)
    {
        #if(0u < COM_TXSIGNAL_NUMBER)
        case COM_SIGNAL:
            txPduId = Com_ConfigStd->ComTxSignal[DestSignalId].ComIpduRefIndex;
            destSignalBitPosition = Com_ConfigStd->ComTxSignal[DestSignalId].ComBitPosition;
            updateBitPosition = Com_ConfigStd->ComTxSignal[DestSignalId].ComUpdateBitPosition;
            txIPduBufId = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIPduBufIndex;
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                if((Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)(destSignalBitPosition/8u) + cnt] )
                        != (Com_GWSignal8BitBuff[SignalBufferId + cnt]))
                {
                    valueChanged = TRUE;
                    Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)(destSignalBitPosition/8u) + cnt]
                                          = Com_GWSignal8BitBuff[SignalBufferId + cnt];
                }
            }
            /*if configuration update bit,set update bit to 1*/
            Com_SetSignalUpdateBit(txPduId,updateBitPosition);
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txPduId,&rptNum);
            signalTxProperty = Com_ConfigStd->ComTxSignal[DestSignalId].ComTransferProperty;
            /*handle the tx pdu (which include the gateway signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txPduId,signalTxProperty,rptNum,valueChanged);
            /*the tx signal configuration timeout*/
            if(0u != Com_ConfigStd->ComTxSignal[DestSignalId].ComTimeout)
            {
                if((0u == Com_TxIPduRunTimeState[txPduId].DMCnt) || (1u < Com_TxIPduRunTimeState[txPduId].RptNum))
                {
                    Com_TxIPduRunTimeState[txPduId].DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
            break;
        #endif
        #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
        case COM_GROUP_SIGNAL:
            txSignalGroupId = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalGroupRef;
            txPduId = Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComIpduRefIndex;
            destSignalBitPosition = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComBitPosition;
            updateBitPosition = Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComUpdateBitPosition;
            txIPduBufId = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIPduBufIndex;
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                if((Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)(destSignalBitPosition/8u) + cnt] )
                        != (Com_GWSignal8BitBuff[SignalBufferId + cnt]))
                {
                    valueChanged = TRUE;
                    Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)(destSignalBitPosition/8u) + cnt]
                                          = Com_GWSignal8BitBuff[SignalBufferId + cnt];
                }
            }
            /*if configuration update bit,set update bit to 1*/
            Com_SetSignalUpdateBit(txPduId,updateBitPosition);
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txPduId,&rptNum);
            /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
            Com_GwGroupSignalOfPduHandle(DestSignalId,rptNum,valueChanged);
            /*the tx signal group(include the gateway group signal)configuration timeout*/
            if(0u != Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComTimeout)
            {
                if((0u == Com_TxIPduRunTimeState[txPduId].DMCnt) || (1u < Com_TxIPduRunTimeState[txPduId].RptNum))
                {
                    Com_TxIPduRunTimeState[txPduId].DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
            break;
        #endif
        #if(0u < COM_DST_SIGNAL_NUMBER)
        case COM_DESCRIPTION:
            txPduId = Com_ConfigStd->ComDestSignal[DestSignalId].ComGwIPduRef;
            destSignalBitPosition = Com_ConfigStd->ComDestSignal[DestSignalId].ComBitPosition;
            updateBitPosition = Com_ConfigStd->ComDestSignal[DestSignalId].ComUpdateBitPosition;
            txIPduBufId = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIPduBufIndex;
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                if((Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)(destSignalBitPosition/8u) + cnt] )
                        != (Com_GWSignal8BitBuff[SignalBufferId + cnt]))
                {
                    valueChanged = TRUE;
                    Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)(destSignalBitPosition/8u) + cnt]
                                          = Com_GWSignal8BitBuff[SignalBufferId + cnt];
                }
            }
            /*if configuration update bit,set update bit to 1*/
            Com_SetSignalUpdateBit(txPduId,updateBitPosition);
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txPduId,&rptNum);
            signalTxProperty = Com_ConfigStd->ComDestSignal[DestSignalId].ComTransferProperty;
            /*handle the tx pdu (which include the gateway description signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txPduId,signalTxProperty,rptNum,valueChanged);
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    #endif
    return;
}
#endif/*(0u < COM_GWSIGNAL_8BITBUFF_SIZE)*/
/******************************************************************************/
/*
 * Brief               Called by Com_GwSignal.
 *                     gateway signal/group signal/description signal which
 *                     signal type is COM_UINT8_DYN
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalBufferId,SignalLength,DestSignalType,
 *                     DestSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
static FUNC(void,COM_CODE)
Com_GwUint8DYNSignal(uint16 SignalBufferId,uint16 SignalLength,Com_GWSignalType DestSignalType,Com_SignalIdType DestSignalId)
{
    #if(0u < COM_TXIPDUBUFF_SIZE)
    boolean valueChanged = FALSE;
    PduIdType txPduId;
    Com_GwUint8DynSignalType parameter;
    uint16 rptNum;
    #if((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_DST_SIGNAL_NUMBER))
    Com_TransferPropertyType signalTxProperty;
    #endif
    #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (COM_TXSIGNALGROUP_NUMBER))
    Com_SignalGroupIdType txSignalGroupId;
    #endif
    switch(DestSignalType)
    {
        #if(0u < COM_TXSIGNAL_NUMBER)
        case COM_SIGNAL:
            txPduId = Com_ConfigStd->ComTxSignal[DestSignalId].ComIpduRefIndex;
            parameter.TxPduId = txPduId;
            parameter.DestSignalBitPosition = Com_ConfigStd->ComTxSignal[DestSignalId].ComBitPosition;
            parameter.UpdateBitPosition = Com_ConfigStd->ComTxSignal[DestSignalId].ComUpdateBitPosition;
            rptNum = Com_GwUint8DYNSignalHandle(
                    SignalBufferId,
                    SignalLength,
                    parameter,
                    &valueChanged);
            signalTxProperty = Com_ConfigStd->ComTxSignal[DestSignalId].ComTransferProperty;
            /*handle the tx pdu (which include the gateway signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txPduId,signalTxProperty,rptNum,valueChanged);
            /*the tx signal configuration timeout*/
            if(0u != Com_ConfigStd->ComTxSignal[DestSignalId].ComTimeout)
            {
                if((0u == Com_TxIPduRunTimeState[txPduId].DMCnt) || (1u < Com_TxIPduRunTimeState[txPduId].RptNum))
                {
                    Com_TxIPduRunTimeState[txPduId].DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
            break;
        #endif
        #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
        case COM_GROUP_SIGNAL:
            txSignalGroupId = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalGroupRef;
            txPduId = Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComIpduRefIndex;
            parameter.TxPduId = txPduId;
            parameter.DestSignalBitPosition = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComBitPosition;
            parameter.UpdateBitPosition = Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComUpdateBitPosition;
            rptNum = Com_GwUint8DYNSignalHandle(
                    SignalBufferId,
                    SignalLength,
                    parameter,
                    &valueChanged);
            /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
            Com_GwGroupSignalOfPduHandle(DestSignalId,rptNum,valueChanged);
            /*the tx signal group(include the gateway group signal)configuration timeout*/
            if(0u != Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComTimeout)
            {
                if((0u == Com_TxIPduRunTimeState[txPduId].DMCnt) || (1u < Com_TxIPduRunTimeState[txPduId].RptNum))
                {
                    Com_TxIPduRunTimeState[txPduId].DMCnt = Com_ConfigStd->ComTxIPdu[txPduId].ComTxIpduDM;
                }
            }
            break;
        #endif
        #if(0u < COM_DST_SIGNAL_NUMBER)
        case COM_DESCRIPTION:
            txPduId = Com_ConfigStd->ComDestSignal[DestSignalId].ComGwIPduRef;
            parameter.TxPduId = txPduId;
            parameter.DestSignalBitPosition = Com_ConfigStd->ComDestSignal[DestSignalId].ComBitPosition;
            parameter.UpdateBitPosition = Com_ConfigStd->ComDestSignal[DestSignalId].ComUpdateBitPosition;
            rptNum = Com_GwUint8DYNSignalHandle(
                    SignalBufferId,
                    SignalLength,
                    parameter,
                    &valueChanged);
            signalTxProperty = Com_ConfigStd->ComDestSignal[DestSignalId].ComTransferProperty;
            /*handle the tx pdu (which include the gateway description signal) transmit parameter*/
            Com_GwSignalOfPduHandle(txPduId,signalTxProperty,rptNum,valueChanged);
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    #endif/*0u < COM_TXIPDUBUFF_SIZE*/
    return;
}
#endif/*(0u < COM_GWSIGNAL_8BITBUFF_SIZE)*/
/******************************************************************************/
/*
 * Brief               Called by Com_GwNotArraySignal.
 *                     gateway dest signal is packed to dest tx pdu buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DestSignalType,DestSignalId,SignalValue
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_TXIPDUBUFF_SIZE)
static FUNC(void,COM_CODE)
Com_DestSignalPack(
        Com_GWSignalType DestSignalType,
        Com_SignalIdType DestSignalId,
        uint64 SignalValue)
{
    PduIdType txIpduId = 0u;
    uint32 bitPosition = 0u;
    Com_SignalType signalType = COM_BOOLEAN;
    uint16 ipduBufferIndex;
    #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (COM_TXSIGNALGROUP_NUMBER))
    Com_SignalGroupIdType signalGroupId;
    #endif
    Com_SignalEndiannessType signalEndianness;
    uint8 bitSize;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 value = 0x01u;
    Com_TxSignalPackType parameter;
    switch(DestSignalType)
    {
        #if(0u < COM_TXSIGNAL_NUMBER)
        case COM_SIGNAL:
            txIpduId = Com_ConfigStd->ComTxSignal[DestSignalId].ComIpduRefIndex;
            bitPosition = Com_ConfigStd->ComTxSignal[DestSignalId].ComBitPosition;
            signalType = Com_ConfigStd->ComTxSignal[DestSignalId].ComSignalType;
            signalEndianness = Com_ConfigStd->ComTxSignal[DestSignalId].ComSignalEndianness;
            bitSize = Com_ConfigStd->ComTxSignal[DestSignalId].ComBitSize;
            break;
        #endif
        #if((0u < COM_TXGROUPSIGNAL_NUMBER) && (COM_TXSIGNALGROUP_NUMBER))
        case COM_GROUP_SIGNAL:
            signalGroupId = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalGroupRef;
            txIpduId = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComIpduRefIndex;
            bitPosition = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComBitPosition;
            signalType = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalType;
            signalEndianness = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComSignalEndianness;
            bitSize = Com_ConfigStd->ComTxGroupSignal[DestSignalId].ComBitSize;
            break;
        #endif
        #if(0u < COM_DST_SIGNAL_NUMBER)
        case COM_DESCRIPTION:
            txIpduId = Com_ConfigStd->ComDestSignal[DestSignalId].ComGwIPduRef;
            bitPosition = Com_ConfigStd->ComDestSignal[DestSignalId].ComBitPosition;
            signalType = Com_ConfigStd->ComDestSignal[DestSignalId].ComSignalType;
            signalEndianness = Com_ConfigStd->ComDestSignal[DestSignalId].ComSignalEndianness;
            bitSize = Com_ConfigStd->ComDestSignal[DestSignalId].ComBitSize;
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    ipduBufferIndex = Com_ConfigStd->ComTxIPdu[txIpduId].ComTxIPduBufIndex;
    if(COM_BOOLEAN == signalType)
    {
        if(0u < SignalValue)
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)]
                                  |= (uint8)(value<<((uint8)bitPosition & 0x07u));
        }
        else
        {
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)]
                                  &= (uint8)(~(uint8)(value<<((uint8)bitPosition & 0x07u)));
        }
    }
    else
    {
        /* signal MSB byte value used for the signal value bits cover 9 bytes */
        dataMSBByte = SignalValue;
        /* Align LSB of txData and signal_value in IPdu */
        SignalValue = SignalValue << ((uint8)bitPosition & 0x07u);
        /*signalByteLength:1-9*/
        signalByteLength = 1u + ((bitSize + ((uint8)bitPosition & 0x07u) - 1u)/8u);
        /*the tx signal all value bits are in one byte*/
        if(1u == signalByteLength)
        {
            /*Clear corresponding bits in IPdu*/
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)]
                                  &= (0xFFu >> (8u - ((uint8)bitPosition & 0x07u))) | (uint8)(0xFFu << (((uint8)bitPosition & 0x07u) + bitSize));
            /*write corresponding bits to IPdu buffer*/
            Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(bitPosition/8u)]
                                  |= (0xFFu >> (8u - (((uint8)bitPosition & 0x07u) + bitSize))) & ((uint8)SignalValue);
        }
        /*the tx signal all value bits are included in 2-9 byte*/
        else if((1u < signalByteLength) && (signalByteLength < 10u))
        {
            parameter.SignalByteLength = signalByteLength;
            parameter.SignalEndianness = signalEndianness;
            parameter.IpduBufferIndex = ipduBufferIndex;
            parameter.BitPosition = bitPosition;
            parameter.BitSize = bitSize;
            Com_TxSignalPackHandle(parameter,SignalValue,dataMSBByte);
        }
        /*this case will not happen*/
        else
        {
            /*do nothing*/
        }
    }
    return;
}
#endif/*0u < COM_TXIPDUBUFF_SIZE*/
/******************************************************************************/
/*
 * Brief               Called by Com_GwNotArraySignal.
 *                     calculate dest signal/group signal/description signal TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,ComFilter,NewSignalValue,OldSignalValue
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if (0u < COM_TMCTXSIGNAL_NUMBER)
static FUNC(boolean,COM_CODE)
Com_DestSignalTMCCalculate
(
    Com_SignalType SignalType,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST)ComFilter,
    uint64 NewSignalValue,
    uint64 OldSignalValue
)
{
    boolean ret = FALSE;
    Com_FilterAlgorithmType filterType;
    #if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    Com_SignalIdType oneEveryNId;
    uint32 filterOffset;
    uint32 filterPeriod;
    #endif
    filterType = ComFilter->ComFilterAlgorithm;
    switch(filterType)
    {
        case COM_ALWAYS:
            ret = TRUE;
            break;
        case COM_MASKED_NEW_EQUALS_X:
            if(((NewSignalValue) & (ComFilter->ComFilterMask))
                == (ComFilter->ComFilterX))
            {
                ret = TRUE;
            }
            break;
        case COM_MASKED_NEW_DIFFERS_X:
            if(((NewSignalValue) & (ComFilter->ComFilterMask))
                != (ComFilter->ComFilterX))
            {
                ret = TRUE;
            }
            break;
        /*COM_BOOLEAN not support WITHIN and WITHOUT*/
        case COM_NEW_IS_WITHIN:
            Com_NewIsWithinFilterCalculate(SignalType,NewSignalValue,ComFilter,&ret);
            break;
        /*COM_BOOLEAN not support WITHIN and WITHOUT*/
        case COM_NEW_IS_OUTSIDE:
            Com_NewIsWithoutFilterCalculate(SignalType,NewSignalValue,ComFilter,&ret);
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
            if(((NewSignalValue) & (ComFilter->ComFilterMask))
                != ((OldSignalValue) & (ComFilter->ComFilterMask)))
            {
                ret = TRUE;
            }
            break;
        default:
            /*do nothing*/
            break;
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_GwNotArraySignal,Com_GwUint8NSignal,
 *                     Com_GwUint8DYNSignal.
 *                     gateway pdu(which include gateway signal) handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,SignalTxProperty,RptNum,ValueChanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TXSIGNAL_NUMBER) || (0u < COM_DST_SIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_GwSignalOfPduHandle(PduIdType TxPduId,Com_TransferPropertyType SignalTxProperty,uint16 RptNum,boolean ValueChanged)
{
    if((COM_TX_MODE_DIRECT == Com_TxIPduRunTimeState[TxPduId].ipduTxMode)
            || (COM_TX_MODE_MIXED == Com_TxIPduRunTimeState[TxPduId].ipduTxMode))
    {
        switch(SignalTxProperty)
        {
            case COM_TRIGGERED:
                Com_TxIPduRunTimeState[TxPduId].NTimeCnt = 0u;
                Com_TxIPduRunTimeState[TxPduId].RptNum = RptNum;
                break;
            case COM_TRIGGERED_ON_CHANGE:
                if(TRUE == ValueChanged)
                {
                    Com_TxIPduRunTimeState[TxPduId].NTimeCnt = 0u;
                    Com_TxIPduRunTimeState[TxPduId].RptNum = RptNum;
                }
                break;
            case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                if(TRUE == ValueChanged)
                {
                    Com_TxIPduRunTimeState[TxPduId].NTimeCnt = 0u;
                    Com_TxIPduRunTimeState[TxPduId].RptNum = 1u;
                }
                break;
            case COM_TRIGGERED_WITHOUT_REPETITION:
                Com_TxIPduRunTimeState[TxPduId].NTimeCnt = 0u;
                Com_TxIPduRunTimeState[TxPduId].RptNum = 1u;
                break;
            default:
                /*do nothing*/
                break;
        }
    }
    else
    {
        Com_TxIPduRunTimeState[TxPduId].NTimeCnt = 0u;
        Com_TxIPduRunTimeState[TxPduId].RptNum = 0u;
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_GwNotArraySignal,Com_GwUint8NSignal,
 *                     Com_GwUint8DYNSignal.
 *                     gateway pdu(which include gateway group signal) handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId,SignalGroupTxProperty,TxGroupSignalId,RptNum,
 *                     ValueChanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TXGROUPSIGNAL_NUMBER) && (0u < COM_TXSIGNALGROUP_NUMBER))
static FUNC(void,COM_CODE)
Com_GwGroupSignalOfPduHandle(Com_SignalIdType TxGroupSignalId,uint16 RptNum,boolean ValueChanged)
{
    Com_TransferPropertyType signalGroupTxProperty;
    Com_SignalGroupIdType txSignalGroupId;
    PduIdType txPduId;
    Com_TransferPropertyType signalTxProperty;
    txSignalGroupId = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComSignalGroupRef;
    txPduId = Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComIpduRefIndex;
    signalGroupTxProperty = Com_ConfigStd->ComTxSignalGroup[txSignalGroupId].ComTransferProperty;
    if((COM_TX_MODE_DIRECT == Com_TxIPduRunTimeState[txPduId].ipduTxMode)
            || (COM_TX_MODE_MIXED == Com_TxIPduRunTimeState[txPduId].ipduTxMode))
    {
        switch(signalGroupTxProperty)
        {
            case COM_TRIGGERED:
                Com_TxIPduRunTimeState[txPduId].NTimeCnt = 0u;
                Com_TxIPduRunTimeState[txPduId].RptNum = RptNum;
                break;
            case COM_TRIGGERED_ON_CHANGE:
                if(TRUE == Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComTxGroupSignalUsedTransferProperty)
                {
                    signalTxProperty = Com_ConfigStd->ComTxGroupSignal[TxGroupSignalId].ComTransferProperty;
                    if((TRUE == ValueChanged) && (COM_TRIGGERED_ON_CHANGE == signalTxProperty))
                    {
                        Com_TxIPduRunTimeState[txPduId].NTimeCnt = 0u;
                        Com_TxIPduRunTimeState[txPduId].RptNum = RptNum;
                    }
                }
                else
                {
                    if(TRUE == ValueChanged)
                    {
                        Com_TxIPduRunTimeState[txPduId].NTimeCnt = 0u;
                        Com_TxIPduRunTimeState[txPduId].RptNum = RptNum;
                    }
                }
                break;
            case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                if(TRUE == ValueChanged)
                {
                    Com_TxIPduRunTimeState[txPduId].NTimeCnt = 0u;
                    Com_TxIPduRunTimeState[txPduId].RptNum = 1u;
                }
                break;
            case COM_TRIGGERED_WITHOUT_REPETITION:
                Com_TxIPduRunTimeState[txPduId].NTimeCnt = 0u;
                Com_TxIPduRunTimeState[txPduId].RptNum = 1u;
                break;
            default:
                /*do nothing*/
                break;
        }
    }
    else
    {
        Com_TxIPduRunTimeState[txPduId].NTimeCnt = 0u;
        Com_TxIPduRunTimeState[txPduId].RptNum = 0u;
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_GwUint8DYNSignal.
 *                     gateway uint8_dyn signal handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalBufferId,SignalLength,parameter,valueChanged
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint16
 */
/******************************************************************************/
#if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
static FUNC(uint16,COM_CODE)
Com_GwUint8DYNSignalHandle(
        uint16 SignalBufferId,
        uint16 SignalLength,
        Com_GwUint8DynSignalType parameter,
        boolean* valueChanged)
{
    uint16 destSignalLength;
    uint16 txIPduBufId;
    uint16 rptNum;
    uint16 cnt;
    *valueChanged = FALSE;
    destSignalLength = Com_TxIPduRunTimeState[parameter.TxPduId].TxIpduLength
            - Com_ConfigStd->ComTxIPdu[parameter.TxPduId].IPduNoDynSignalLength;
    txIPduBufId = Com_ConfigStd->ComTxIPdu[parameter.TxPduId].ComTxIPduBufIndex;
    for(cnt = 0u;cnt < SignalLength;cnt++)
    {
        if((Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)((parameter.DestSignalBitPosition)/8u) + cnt] )
                != (Com_GWSignal8BitBuff[SignalBufferId + cnt]))
        {
            *valueChanged = TRUE;
            Com_TxIPduRuntimeBuff[txIPduBufId + (uint16)((parameter.DestSignalBitPosition)/8u) + cnt]
                                  = Com_GWSignal8BitBuff[SignalBufferId + cnt];
        }
    }
    if(SignalLength != destSignalLength)
    {
        *valueChanged = TRUE;
        Com_TxIPduRunTimeState[parameter.TxPduId].TxIpduLength =
                Com_ConfigStd->ComTxIPdu[parameter.TxPduId].IPduNoDynSignalLength + SignalLength;
    }
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(parameter.TxPduId,parameter.UpdateBitPosition);
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter.*/
    Com_TxIpduTMSHandle(parameter.TxPduId,&rptNum);
    return rptNum;
}
#endif/*(0u < COM_GWSIGNAL_8BITBUFF_SIZE)*/

/*Called by Com_GwNotArraySignal.
 *Get the GW Signal's New value and Old value*/
static FUNC(uint64,COM_CODE)
Com_GwSignalNewOldValueGet(
        Com_SignalType SignalType,
        uint16 GWSignalBufferId,
        uint16 txSignalValueId,
        uint64* sourceSignalValue)
{
    uint64 destSignalValue = 0u;
    switch(SignalType)
    {
        #if ((0u < COM_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_GWSIGNAL_BOOLBUFF_SIZE))
        case COM_BOOLEAN:
            *sourceSignalValue = (uint64)Com_GWSignalBoolBuff[GWSignalBufferId];
            destSignalValue = (uint64)Com_SignalBoolRuntimeBuff[txSignalValueId];
            Com_SignalBoolRuntimeBuff[txSignalValueId] = Com_GWSignalBoolBuff[GWSignalBufferId];
            break;
        #endif
        #if ((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_GWSIGNAL_8BITBUFF_SIZE))
        case COM_UINT8:
        case COM_SINT8:
            *sourceSignalValue = (uint64)Com_GWSignal8BitBuff[GWSignalBufferId];
            destSignalValue = (uint64)Com_Signal8BitRuntimeBuff[txSignalValueId];
            Com_Signal8BitRuntimeBuff[txSignalValueId] = Com_GWSignal8BitBuff[GWSignalBufferId];
            break;
        #endif
        #if ((0u < COM_SIGNAL_16BITBUFF_SIZE) && (0u < COM_GWSIGNAL_16BITBUFF_SIZE))
        case COM_UINT16:
        case COM_SINT16:
            *sourceSignalValue = (uint64)Com_GWSignal16BitBuff[GWSignalBufferId];
            destSignalValue = (uint64)Com_Signal16BitRuntimeBuff[txSignalValueId];
            Com_Signal16BitRuntimeBuff[txSignalValueId] = Com_GWSignal16BitBuff[GWSignalBufferId];
            break;
        #endif
        #if ((0u < COM_SIGNAL_32BITBUFF_SIZE) && (0u < COM_GWSIGNAL_32BITBUFF_SIZE))
        case COM_UINT32:
        case COM_SINT32:
        case COM_FLOAT32:
            *sourceSignalValue = (uint64)Com_GWSignal32BitBuff[GWSignalBufferId];
            destSignalValue = (uint64)Com_Signal32BitRuntimeBuff[txSignalValueId];
            Com_Signal32BitRuntimeBuff[txSignalValueId] = Com_GWSignal32BitBuff[GWSignalBufferId];
            break;
        #endif
        #if ((0u < COM_SIGNAL_64BITBUFF_SIZE) && (0u < COM_GWSIGNAL_64BITBUFF_SIZE))
        case COM_UINT64:
        case COM_SINT64:
        case COM_FLOAT64:
            *sourceSignalValue = Com_GWSignal64BitBuff[GWSignalBufferId];
            destSignalValue = Com_Signal64BitRuntimeBuff[txSignalValueId];
            Com_Signal64BitRuntimeBuff[txSignalValueId] = Com_GWSignal64BitBuff[GWSignalBufferId];
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    return destSignalValue;
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif/*(0u < COM_GWMAPPING_NUMBER) && (0u < COM_RXIPDU_NUMBER) && (0u < COM_TXIPDU_NUMBER)*/












