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
**  FILENAME    : Com_RxInternal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : internal implementation for COM receive                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
#if(0u < COM_RXIPDU_NUMBER)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* called by Com_RxIpduController
 * reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
#if((COM_IPDUGROUP_NUMBER > 0u) && (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u))
static FUNC(void,COM_CODE)
Com_ResetRxOccurrenceOfPduFilter(PduIdType IpduId);
#endif
/* called by Com_RxIpduController
 * init the rx ipdu buffer,all signal buffer(included in the ipdu,except source signal)*/
#if(COM_IPDUGROUP_NUMBER > 0u)
static FUNC(void,COM_CODE)
Com_ResetRxPduBufferAndSignalBuffer(PduIdType IpduId);
#endif
/* called by Com_IndicationProcess.
 * unpack the rx source description signal to gateway buffer*/
#if(0u < COM_SOURCE_SIGNAL_NUMBER)
static FUNC(void,COM_CODE)
Com_SourceSignalUnPack(Com_SignalIdType SourceSignalId,uint16 RxDynSignalLength,uint16 IpduBufferId);
#endif
/* called by Com_IndicationProcess.
 * unpack the rx signal and notification up layer*/
#if(0u < COM_RXSIGNAL_NUMBER)
static FUNC(void,COM_CODE)
Com_SignalRxIndication(Com_SignalIdType RxSignalId,uint16 RxDynSignalLength,uint16 IpduBufferId);
#endif
/* called by Com_SignalRxIndication.
 * unpack the rx signal(signal type is COM_UINT8_N) and notification up layer*/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void,COM_CODE)
Com_Rx8NSignalHandle(Com_SignalIdType SignalId,uint16 SignalLength,uint16 BufferId);
#endif
/* called by Com_SignalRxIndication.
 * unpack the rx signal(signal type is COM_UINT8_DYN) and notification up layer*/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void,COM_CODE)
Com_Rx8DYNSignalHandle(Com_SignalIdType SignalId,uint16 SignalLength,uint16 BufferId);
#endif
/* called by Com_SignalRxIndication.
 * unpack the rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN) and notification up layer*/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void,COM_CODE)
Com_RxSignalHandle(Com_SignalIdType SignalId);
#endif
/* called by Com_RxSignalHandle.
 * unpack the rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN)*/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(uint64,COM_CODE)
Com_RxSignalUnPack(Com_SignalIdType SignalId);
#endif
/* called by Com_SourceSignalUnPack.
 * unpack the rx source description signal*/
#if((0u < COM_SOURCE_SIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(uint64,COM_CODE)
Com_SourceSignalNotArrayUnPack(Com_SignalIdType SourceSignalId);
#endif
/* called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 * judge the rx (group) signal value is invalid value or not*/
#if((0u < COM_RXGROUPSIGNAL_NUMBER) || (0u < COM_RXSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_SignalInvalidateHandle(Com_SignalType SignalType,uint16 InvalidValueId, uint64 Value);
#endif
/* called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,Com_RxSignalGroupFilter.
 * get the rx (group) signal's init value*/
#if((0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(uint64,COM_CODE)
Com_RxSignalReplaceHanlde(Com_SignalType SignalType,uint16 InitValueId);
#endif
/* called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification.
 * calculate the rx signal value is filter out or not*/
#if((0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_RxSignalFilter(
    uint64 Value,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
    #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint16 SignalBufferId,
    #endif
    Com_SignalType SignalType
);
#endif
/* called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification.
 * update the rx signal value buffer*/
#if(0u < COM_RXSIGNAL_NUMBER)
static FUNC(void,COM_CODE)
Com_RxSignalBuffHanlde(uint64 Value,Com_SignalIdType Id);
#endif
/* called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 * update the gateway signal value buffer of the rx signal*/
#if((0u < COM_RXGROUPSIGNAL_NUMBER) || (0u < COM_RXSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_UpdateGWSignalBuffer(uint64 Value,uint16 GWSignalBufferId,Com_SignalType SignalType);
#endif
/* called by Com_IndicationProcess.
 * handle signal group RxIndication of one rx pdu*/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_SignalGroupRxIndication(Com_SignalGroupIdType RxSignalGroupId,uint16 RxDynSignalLength,uint16 IpduBufferId);
#endif
/* called by Com_SignalGroupRxIndication,Com_RxGroupSignalBuffHanlde.
 * unpack rx group signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN)*/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(uint64,COM_CODE)
Com_RxGroupSignalUnPack(Com_SignalIdType SignalId,uint16 IpduBufferId);
#endif
/* called by Com_RxGroupSignalUnPack,Com_RxSignalUnPack,Com_SourceSignalNotArrayUnPack.
 * unpack all byte value(group signal/signal cover,except MSB byte)*/
#if(0u < COM_RXIPDUBUFF_SIZE)
static FUNC(uint64,COM_CODE)
Com_RxSignalUnPackHandle(Com_SignalEndiannessType SignalEndianness,uint8 SignalByteLength,uint16* IpduBufferIndex);
#endif
/* called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 * judge the rx signal group value is filter out or not*/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_RxSignalGroupFilter(Com_SignalGroupIdType SignalGroupId,boolean InvalidSignalGroup,uint16 IpduBufferId);
#endif
/* called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 * update the rx group signal value buffer*/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_RxGroupSignalBuffHanlde(boolean InvalidSignalGroup,Com_SignalIdType Id,uint16 IpduBufferId,uint16 RxDynSignalLength);
#endif
/* called by Com_TpPduInvalidAction.
 * rx signal value replace,and notification*/
#if((STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_InvalidSignalReplaceAndNotification(Com_SignalIdType RxSignalId);
#endif


#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
/*Called by Com_SignalGroupInvalidCalculate.
 *Uint8_Dyn Group Signal Invalid Calculate*/
static FUNC(void,COM_CODE)
Com_Uint8DynGroupSignalInvalidCalculate(
        uint16 RxDynSignalLength,
        Com_SignalIdType groupSignalId,
        uint16 IpduBufferId,
        boolean* signalGroupInvalid);
/*Called by Com_SignalGroupRxIndication.
 *Signal Group Invalid Calculate*/
static FUNC(boolean,COM_CODE)
Com_SignalGroupInvalidCalculate(
        Com_SignalGroupIdType RxSignalGroupId,
        Com_SignalIdType groupSignalNumber,
        uint16 IpduBufferId,
        uint16 RxDynSignalLength);
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
 *                     used to handle Rx Ipdu state change or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,PduInfo
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_IPDUGROUP_NUMBER > 0u)
FUNC(void,COM_CODE)
Com_RxIpduController(PduIdType RxPduId,boolean initialize)
{
    Com_IpduGroupIdType ipduGroupRefNumber;
    Com_IpduGroupIdType index;
    Com_IpduGroupIdType ipduGroupId;
    boolean findActiveIpduGroup = FALSE;
    ipduGroupRefNumber = Com_ConfigStd->ComRxIPdu[RxPduId].ComIPduGroupsRefNumber;
    /*if the Pdu not included in any Ipdu Group,the state is always start*/
    if(ipduGroupRefNumber > 0u)
    {
        for(index = 0u;(index < ipduGroupRefNumber)&&(FALSE == findActiveIpduGroup);index++)
        {
            ipduGroupId = Com_ConfigStd->ComRxIPdu[RxPduId].ComIPduGroupsRef[index];
            if(TRUE == Com_IpduGroupEnable[ipduGroupId])
            {
                findActiveIpduGroup = TRUE;
            }
        }
        /*at least one Ipdu Group is start,the Pdu shall be active*/
        if(TRUE == findActiveIpduGroup)
        {
            /*the pdu restart*/
            if(FALSE == Com_RxIPduRunTimeState[RxPduId].ActiveEnable)
            {
                Com_RxIPduRunTimeState[RxPduId].ActiveEnable = TRUE;
                /*reset rx i-pdu(all signal/signal group) DM Counter*/
                #if((COM_RXSIGNAL_NUMBER+COM_RXSIGNALGROUP_NUMBER) > 0u)
                Com_ResetRxPduTimeOut(RxPduId);
                #endif
                /*reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N*/
                #if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
                Com_ResetRxOccurrenceOfPduFilter(RxPduId);
                #endif
                if(TRUE == initialize)
                {
                    /*init the rx ipdu buffer,all signal buffer(included in the ipdu,except source signal)*/
                    Com_ResetRxPduBufferAndSignalBuffer(RxPduId);
                    if(NULL_PTR != Com_ConfigStd->ComRxIPdu[RxPduId].ComIPduCounter)
                    {
                        /*accept for I-PDUs with ComIPduDirection configured to RECEIVED any next incoming I-PDU counter*/
                        Com_RxIPduRunTimeState[RxPduId].RxAnyCounterPdu = TRUE;
                        Com_RxIPduRunTimeState[RxPduId].RxReplicationNumber = 0u;
                    }
                }
            }
        }
        else
        {
            if(TRUE == Com_RxIPduRunTimeState[RxPduId].ActiveEnable)
            {
                Com_RxIPduRunTimeState[RxPduId].ActiveEnable = FALSE;
                /*disable rx i-pdu(all signal/signal group) DM Counter*/
                #if((COM_RXSIGNAL_NUMBER+COM_RXSIGNALGROUP_NUMBER) > 0u)
                Com_DisableRxPduTimeOut(RxPduId);
                #endif
            }
        }
    }
    return;
}
#endif/*COM_IPDUGROUP_NUMBER > 0u*/
/******************************************************************************/
/*
 * Brief               Called by Com_RxIpduController,Com_ReceptionDMControl.
 *                     set the reception deadline monitoring timer for the
 *                     included signals and signal groups to the configured
 *                     ComFirstTimeout
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((COM_IPDUGROUP_NUMBER > 0u) && ((COM_RXSIGNAL_NUMBER+COM_RXSIGNALGROUP_NUMBER) > 0u))
FUNC(void,COM_CODE)
Com_ResetRxPduTimeOut(PduIdType IpduId)
{
    Com_SignalIdType cnt;
    uint16 timeCntIndex;
    #if(COM_RXSIGNAL_NUMBER > 0u)
    Com_SignalIdType ipduSignalRefNumber;
    Com_SignalIdType rxSignalId;
    #endif
    #if(COM_RXSIGNALGROUP_NUMBER > 0u)
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType rxSignalGroupId;
    #endif
    #if(COM_RXSIGNAL_NUMBER > 0u)
    ipduSignalRefNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalRefNumber;cnt++)
    {
        rxSignalId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRef[cnt];
        timeCntIndex = Com_ConfigStd->ComRxSignal[rxSignalId].ComTimeCntIndex;
        Com_TimeOutCnt[timeCntIndex] = Com_ConfigStd->ComRxSignal[rxSignalId].ComFirstTimeout;
    }
    #endif
    #if(COM_RXSIGNALGROUP_NUMBER > 0u)
    ipduSignalGroupNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        rxSignalGroupId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        timeCntIndex = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComTimeoutCntIndex;
        Com_TimeOutCnt[timeCntIndex] = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComFirstTimeout;
    }
    #endif
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_RxIpduController,Com_ReceptionDMControl.
 *                     set the reception deadline monitoring timer for the
 *                     included signals and signal groups to 0
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((COM_IPDUGROUP_NUMBER > 0u) && ((COM_RXSIGNAL_NUMBER+COM_RXSIGNALGROUP_NUMBER) > 0u))
FUNC(void,COM_CODE)
Com_DisableRxPduTimeOut(PduIdType IpduId)
{
    Com_SignalIdType cnt;
    uint16 timeCntIndex;
    #if(COM_RXSIGNAL_NUMBER > 0u)
    Com_SignalIdType ipduSignalRefNumber;
    Com_SignalIdType rxSignalId;
    #endif
    #if(COM_RXSIGNALGROUP_NUMBER > 0u)
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType rxSignalGroupId;
    #endif
    #if(COM_RXSIGNAL_NUMBER > 0u)
    ipduSignalRefNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalRefNumber;cnt++)
    {
        rxSignalId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRef[cnt];
        timeCntIndex = Com_ConfigStd->ComRxSignal[rxSignalId].ComTimeCntIndex;
        Com_TimeOutCnt[timeCntIndex] = 0u;
    }
    #endif
    #if(COM_RXSIGNALGROUP_NUMBER > 0u)
    ipduSignalGroupNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        rxSignalGroupId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        timeCntIndex = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComTimeoutCntIndex;
        Com_TimeOutCnt[timeCntIndex] = 0u;
    }
    #endif
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_RxIndication,Com_TpRxIndication.
 *                     reset receive MD timer for all signals/signal groups
 *                     (which is update) of one rx pdu
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,PduInfo
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u)
FUNC(void,COM_CODE)
Com_ResetUpdateDMTime(PduIdType IpduId,P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfo)
{
    Com_SignalIdType cnt;
    uint16 timeCntIndex;
    uint32 updateBitPosition;
    uint8 updateBitOffset;
    #if(COM_RXSIGNAL_NUMBER > 0u)
    Com_SignalIdType ipduSignalRefNumber;
    Com_SignalIdType rxSignalId;
    #endif
    #if(COM_RXSIGNALGROUP_NUMBER > 0u)
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType rxSignalGroupId;
    #endif
    #if(COM_RXSIGNAL_NUMBER > 0u)
    ipduSignalRefNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalRefNumber;cnt++)
    {
        rxSignalId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRef[cnt];
        timeCntIndex = Com_ConfigStd->ComRxSignal[rxSignalId].ComTimeCntIndex;
        updateBitPosition = Com_ConfigStd->ComRxSignal[rxSignalId].ComUpdateBitPosition;
        /*the rx signal have update bit*/
        if(COM_UNUSED_UINT32 != updateBitPosition)
        {
            updateBitOffset = (uint8)(0x01u << ((updateBitPosition) & 0x07u));
            /*update bit is 1*/
            if(updateBitOffset == ((PduInfo->SduDataPtr[updateBitPosition/8u]) & updateBitOffset))
            {
                Com_TimeOutCnt[timeCntIndex] = Com_ConfigStd->ComRxSignal[rxSignalId].ComTimeout;
            }
        }
        /*the rx signal not have update bit*/
        else
        {
            Com_TimeOutCnt[timeCntIndex] = Com_ConfigStd->ComRxSignal[rxSignalId].ComTimeout;
        }
    }
    #endif
    #if(COM_RXSIGNALGROUP_NUMBER > 0u)
    ipduSignalGroupNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        rxSignalGroupId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        timeCntIndex = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComTimeoutCntIndex;
        updateBitPosition = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComUpdateBitPosition;
        /*the rx signal group have update bit*/
        if(COM_UNUSED_UINT32 != updateBitPosition)
        {
            updateBitOffset = (uint8)(0x01u << ((updateBitPosition) & 0x07u));
            /*update bit is 1*/
            if(updateBitOffset == ((PduInfo->SduDataPtr[updateBitPosition/8u]) & updateBitOffset))
            {
                Com_TimeOutCnt[timeCntIndex] = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComTimeout;
            }
        }
        /*the rx signal group not have update bit*/
        else
        {
            Com_TimeOutCnt[timeCntIndex] = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComTimeout;
        }
    }
    #endif
    return;
}
#endif/*COM_RXSIGNAL_NUMBER + COM_RXSIGNALGROUP_NUMBER > 0u*/
/******************************************************************************/
/*
 * Brief               Called by Com_RxIndication,Com_TpRxIndication,Com_MainFunctionRx.
 *                     unpack the rx pdu data and invoke notification
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_IndicationProcess(PduIdType RxIpduId)
{
    #if(0u < COM_RXSIGNAL_NUMBER)
    Com_SignalIdType signalNumber;
    Com_SignalIdType rxSignalId;
    #endif
    #if(0u < COM_RXSIGNALGROUP_NUMBER)
    Com_SignalGroupIdType siganlGroupNumber;
    Com_SignalGroupIdType rxSignalGroupId;
    boolean updateSignalGroup = FALSE;
    #endif
    #if(0u < COM_SOURCE_SIGNAL_NUMBER)
    Com_SignalIdType gwSourceSignalNumber;
    Com_SignalIdType sourceSignalId;
    #endif
    uint16 cnt;
    uint16 rxDynSignalLength;
    uint16 ipduBufferId;
    #if((0u < COM_RXSIGNAL_NUMBER) || (0u < COM_SOURCE_SIGNAL_NUMBER))
    boolean updateSignal = FALSE;
    #endif
    uint32 updateBitPosition;
    uint8 updateBitOffset;
    #if(COM_GWSOURCESIGNAL_UPDATE_NUMBER > 0u)
    uint16 gwUpdataStateId;
    #endif
    ipduBufferId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComRxIPduBufIndex;
    rxDynSignalLength = Com_RxIPduRunTimeState[RxIpduId].RxIpduLength -
            (uint16)Com_ConfigStd->ComRxIPdu[RxIpduId].IPduNoDynSignalLength;
    /*store the rx dynamic signal length which really receive,not replace by any reason for gateway*/
    Com_RxIPduRunTimeState[RxIpduId].GWDynamicSignalLength = rxDynSignalLength;
    #if(0u < COM_RXSIGNAL_NUMBER)
    signalNumber = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < signalNumber;cnt++)
    {
        rxSignalId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalsRef[cnt];
        updateBitPosition = Com_ConfigStd->ComRxSignal[rxSignalId].ComUpdateBitPosition;
        /*the rx signal have update bit*/
        if(COM_UNUSED_UINT32 != updateBitPosition)
        {
            updateBitOffset = (uint8)(0x01u << ((updateBitPosition) & 0x07u));
            /*update bit is 1*/
            if(updateBitOffset == ((Com_RxIPduRuntimeBuff[ipduBufferId + (uint16)(updateBitPosition/8u)]) & updateBitOffset))
            {
                #if(COM_GWSOURCESIGNAL_UPDATE_NUMBER > 0u)
                gwUpdataStateId = Com_ConfigStd->ComRxSignal[rxSignalId].GWUpdataStateId;
                /*the signal need gateway and have update bit*/
                if(COM_UNUSED_UINT16 != gwUpdataStateId)
                {
                    Com_GWSourceSignalUpdate[gwUpdataStateId] = TRUE;
                }
                #endif
                updateSignal = TRUE;
            }
        }
        /*the rx signal not have update bit*/
        else
        {
            updateSignal = TRUE;
        }
        if(TRUE == updateSignal)
        {
            Com_SignalRxIndication(rxSignalId,rxDynSignalLength,ipduBufferId);
            updateSignal = FALSE;
        }
    }
    #endif/*0u < COM_RXSIGNAL_NUMBER*/
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    siganlGroupNumber = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < siganlGroupNumber;cnt++)
    {
        rxSignalGroupId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalGroupsRef[cnt];
        updateBitPosition = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComUpdateBitPosition;
        /*the rx group signal have update bit*/
        if(COM_UNUSED_UINT32 != updateBitPosition)
        {
            updateBitOffset = (uint8)(0x01u << ((updateBitPosition) & 0x07u));
            /*update bit is 1*/
            if(updateBitOffset == ((Com_RxIPduRuntimeBuff[ipduBufferId + (uint16)(updateBitPosition/8u)]) & updateBitOffset))
            {
                #if(COM_GWSOURCESIGNAL_UPDATE_NUMBER > 0u)
                gwUpdataStateId = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].GWUpdataStateId;
                /*the signal group (or group signal included in the signal group) need gateway and have update bit*/
                if(COM_UNUSED_UINT16 != gwUpdataStateId)
                {
                    Com_GWSourceSignalUpdate[gwUpdataStateId] = TRUE;
                }
                #endif
                updateSignalGroup = TRUE;
            }
        }
        /*the rx group signal not have update bit*/
        else
        {
            updateSignalGroup = TRUE;
        }
        if(TRUE == updateSignalGroup)
        {
            Com_SignalGroupRxIndication(rxSignalGroupId,rxDynSignalLength,ipduBufferId);
            updateSignalGroup = FALSE;
        }
    }
    #endif
    #if(0u < COM_SOURCE_SIGNAL_NUMBER)
    gwSourceSignalNumber = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSourceSignalsRefNumber;
    for(cnt = 0u;cnt < gwSourceSignalNumber;cnt++)
    {
        sourceSignalId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSourceSignalsRef[cnt];
        updateBitPosition = Com_ConfigStd->ComSourceSignal[sourceSignalId].ComUpdateBitPosition;
        /*the source signal have update bit*/
        if(COM_UNUSED_UINT32 != updateBitPosition)
        {
            updateBitOffset = (uint8)(0x01u << ((updateBitPosition) & 0x07u));
            /*update bit is 1*/
            if(updateBitOffset == ((Com_RxIPduRuntimeBuff[ipduBufferId + (uint16)(updateBitPosition/8u)]) & updateBitOffset))
            {
                #if(COM_GWSOURCESIGNAL_UPDATE_NUMBER > 0)
                gwUpdataStateId = Com_ConfigStd->ComSourceSignal[sourceSignalId].GWUpdataStateId;
                Com_GWSourceSignalUpdate[gwUpdataStateId] = TRUE;
                #endif
                updateSignal = TRUE;
            }
        }
        /*the rx signal not have update bit*/
        else
        {
            updateSignal = TRUE;
        }
        if(TRUE == updateSignal)
        {
            /*unpack the rx source description signal to gateway buffer*/
            Com_SourceSignalUnPack(sourceSignalId,rxDynSignalLength,ipduBufferId);
            updateSignal = FALSE;
        }
    }
    #endif
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_TpRxIndication.
 *                     execute all configured ComDataInvalidActions for all
 *                     included signals and signal groups
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == COM_RXTPPDU_SUPPORT)
FUNC(void,COM_CODE)
Com_TpPduInvalidAction(PduIdType RxIpduId)
{
    #if(0u < COM_RXSIGNAL_NUMBER)
    Com_SignalIdType signalNumber;
    Com_SignalIdType rxSignalId;
    uint16 invalidValueId;
    #endif
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    Com_SignalGroupIdType siganlGroupNumber;
    Com_SignalIdType counter;
    uint16 rxDynSignalLength;
    uint16 ipduBufferId;
    Com_SignalGroupIdType rxSignalGroupId;
    Com_SignalIdType groupSignalNumber;
    Com_SignalIdType groupSignalId;
    #endif
    Com_SignalIdType cnt;
    #if(0u < COM_RXSIGNAL_NUMBER)
    signalNumber = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < signalNumber;cnt++)
    {
        rxSignalId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalsRef[cnt];
        invalidValueId = Com_ConfigStd->ComRxSignal[rxSignalId].ComSignalDataInvalidValueId;
        if(COM_UNUSED_UINT16 != invalidValueId)
        {
            if(COM_INVALID_ACTION_NOTIFY == Com_ConfigStd->ComRxSignal[rxSignalId].ComDataInvalidAction)
            {
                if(NULL_PTR != Com_ConfigStd->ComRxSignal[rxSignalId].ComInvalidNotification)
                {
                    Com_ConfigStd->ComRxSignal[rxSignalId].ComInvalidNotification();
                }
            }
            else
            {
                Com_InvalidSignalReplaceAndNotification(rxSignalId);
            }
        }
    }
    #endif
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    siganlGroupNumber = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < siganlGroupNumber;cnt++)
    {
        rxSignalGroupId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComIPduSignalGroupsRef[cnt];
        groupSignalNumber = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComGroupSignalNumber;
        if(COM_INVALID_ACTION_NOTIFY == Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComDataInvalidAction)
        {
            if(NULL_PTR != Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComInvalidNotification)
            {
                Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComInvalidNotification();
            }
        }
        else
        {
            ipduBufferId = Com_ConfigStd->ComRxIPdu[RxIpduId].ComRxIPduBufIndex;
            if(TRUE == Com_RxSignalGroupFilter(rxSignalGroupId,TRUE,ipduBufferId))
            {
                rxDynSignalLength = Com_RxIPduRunTimeState[RxIpduId].RxIpduLength
                        - (uint16)Com_ConfigStd->ComRxIPdu[RxIpduId].IPduNoDynSignalLength;
                /* update all group signal init value into group signal runtime buffer */
                for(counter = 0u;counter < groupSignalNumber;counter++)
                {
                    groupSignalId = Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComRxGroupSignalId[counter];
                    Com_RxGroupSignalBuffHanlde(TRUE,groupSignalId,ipduBufferId,rxDynSignalLength);
                }
                if(NULL_PTR != Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComNotification)
                {
                    Com_ConfigStd->ComRxSignalGroup[rxSignalGroupId].ComNotification();
                }
            }
        }
    }
    #endif
    return;
}
#endif/*STD_ON == COM_RXTPPDU_SUPPORT*/
/******************************************************************************/
/*
 * Brief               Called by Com_TpRxIndication.
 *                     unpack the receive pdu's counter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == COM_RXTPPDU_SUPPORT)
FUNC(uint8,COM_CODE)
Com_UnPackCounterValue(PduIdType IpduId)
{
    uint8 counterValue;
    uint16 rxPduBufferId;
    uint32 bitPosition;
    uint8 bitSize;
    rxPduBufferId = Com_ConfigStd->ComRxIPdu[IpduId].ComRxIPduBufIndex;
    bitPosition = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterStartPosition;
    bitSize = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterSize;
    counterValue = Com_RxIPduRuntimeBuff[rxPduBufferId + (uint16)(bitPosition/8u)];
    counterValue = counterValue << (8u - (((uint8)bitPosition & 0x07u) + bitSize));
    counterValue = counterValue >> (8u - bitSize);
    return counterValue;
}
#endif/*STD_ON == COM_RXTPPDU_SUPPORT*/
/******************************************************************************/
/*
 * Brief               Called by Com_TpRxIndication,Com_RxIndication.
 *                     judge the receive pdu's counter is valid or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,RxCounter
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(boolean,COM_CODE)
Com_RxPduCounterValid(PduIdType IpduId,uint8 RxCounter)
{
    boolean receiveCounterIsValid = FALSE;
    uint8 counterThreshold;
    uint8 maxCounter;
    uint8 expectedCounter;
    /*accept any incoming I-PDU, regardless of the value of the I-PDU counter*/
    if(TRUE == Com_RxIPduRunTimeState[IpduId].RxAnyCounterPdu)
    {
        receiveCounterIsValid = TRUE;
        /*update the expected counter*/
        Com_RxIPduRunTimeState[IpduId].RxIpduCounter = RxCounter;
        Com_PduCounterIncrease(FALSE,IpduId);
        Com_RxIPduRunTimeState[IpduId].RxAnyCounterPdu = FALSE;
    }
    else
    {
        maxCounter = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduMaxCounter;
        counterThreshold = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterThreshold;
        expectedCounter = Com_RxIPduRunTimeState[IpduId].RxIpduCounter;
        /*respect to counter wrap-around*/
        if((maxCounter - expectedCounter) < counterThreshold)
        {
            if((RxCounter >= expectedCounter) || (RxCounter < (counterThreshold - (maxCounter - expectedCounter))))
            {
                receiveCounterIsValid = TRUE;
            }
        }
        /*respect to counter not wrap-around*/
        else
        {
            if((RxCounter >= expectedCounter) && (RxCounter <= (expectedCounter + counterThreshold)))
            {
                receiveCounterIsValid = TRUE;
            }
        }
        /*update the expected counter*/
        Com_RxIPduRunTimeState[IpduId].RxIpduCounter = RxCounter;
        Com_PduCounterIncrease(FALSE,IpduId);
        if(FALSE == receiveCounterIsValid)
        {
            if(NULL_PTR != Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterErrorNotification)
            {
                Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterErrorNotification(IpduId,expectedCounter,RxCounter);
            }
        }
    }
    return receiveCounterIsValid;
}
/******************************************************************************/
/*
 * Brief               Called by Com_RxIndication,Com_RxPduReplicationValid.
 *                     copy the Rx Pdu data to RxPdu Buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,COM_CODE)
Com_CopyPduDataToRxBuffer(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
    uint16 pduBufferPos;
    PduLengthType cnt;
    #if(STD_ON == COM_METADATA_SUPPORT)
    uint16 metaDataStartPos;
    uint8 metadataLength;
    metadataLength = Com_ConfigStd->ComRxIPdu[RxPduId].MetaDataLength;
    #endif
    pduBufferPos = Com_ConfigStd->ComRxIPdu[RxPduId].ComRxIPduBufIndex;
    #if(STD_ON == COM_METADATA_SUPPORT)
    metaDataStartPos = (uint16)Com_ConfigStd->ComRxIPdu[RxPduId].IPduNoDynSignalLength
            + (uint16)Com_ConfigStd->ComRxIPdu[RxPduId].IPduMaxDynSignalLength
            + pduBufferPos;
    #endif
    /*copy receive pdu data to rx pdu buffer*/
    for(cnt = 0u; cnt < PduInfoPtr->SduLength; cnt++)
    {
        #if(0u < COM_RXIPDUBUFF_SIZE)
        Com_RxIPduRuntimeBuff[pduBufferPos + cnt] = PduInfoPtr->SduDataPtr[cnt];
        #endif
    }
    #if(STD_ON == COM_METADATA_SUPPORT)
    /*copy meta data to rx pdu buffer*/
    for(cnt = 0u; cnt < metadataLength; cnt++)
    {
        #if(0u < COM_RXIPDUBUFF_SIZE)
        Com_RxIPduRuntimeBuff[metaDataStartPos + cnt] = PduInfoPtr->MetaDataPtr[cnt];
        #endif
    }
    #endif
    /*update the pdu length*/
    Com_RxIPduRunTimeState[RxPduId].RxIpduLength = (uint16)PduInfoPtr->SduLength;
    return;
}
/******************************************************************************/
/*
 * Brief               Called by Com_RxIndication.
 *                     judge the receive pdu's Replication is valid or not
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId,RxCounter,PduInfoPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(boolean,COM_CODE)
Com_RxPduReplicationValid(PduIdType IpduId,uint8 RxCounter,P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
    boolean rxPduValid = FALSE;
    uint8 iPduReplicationQuorum;
    PduLengthType cnt;
    uint8 updateCounter;
    boolean dataEqual = TRUE;
    iPduReplicationQuorum = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduReplication->ComIPduReplicationQuorum;
    /*accept any incoming I-PDU, regardless of the value of the I-PDU counter*/
    if(TRUE == Com_RxIPduRunTimeState[IpduId].RxAnyCounterPdu)
    {
        Com_RxIPduRunTimeState[IpduId].RxIpduCounter = RxCounter;
        /*copy receive pdu data to rx pdu buffer*/
        Com_CopyPduDataToRxBuffer(IpduId,PduInfoPtr);
        Com_RxIPduRunTimeState[IpduId].RxReplicationNumber = 1u;
        if(1u == iPduReplicationQuorum)
        {
            /*update the expected counter*/
            Com_PduCounterIncrease(FALSE,IpduId);
            Com_RxIPduRunTimeState[IpduId].RxReplicationNumber = 0u;
            rxPduValid = TRUE;
        }
        Com_RxIPduRunTimeState[IpduId].RxAnyCounterPdu = FALSE;
    }
    else
    {
        /*the rx pdu counter is expect counter*/
        if(RxCounter == Com_RxIPduRunTimeState[IpduId].RxIpduCounter)
        {
            if(0u == Com_RxIPduRunTimeState[IpduId].RxReplicationNumber)
            {
                /*copy receive pdu data to rx pdu buffer*/
                Com_CopyPduDataToRxBuffer(IpduId,PduInfoPtr);
                Com_RxIPduRunTimeState[IpduId].RxReplicationNumber = 1u;
                if(1u == iPduReplicationQuorum)
                {
                    /*update the expected counter*/
                    Com_PduCounterIncrease(FALSE,IpduId);
                    Com_RxIPduRunTimeState[IpduId].RxReplicationNumber = 0u;
                    rxPduValid = TRUE;
                }
            }
            else
            {
                if(Com_RxIPduRunTimeState[IpduId].RxIpduLength == PduInfoPtr->SduLength)
                {
                    for(cnt = 0u;(cnt < PduInfoPtr->SduLength)&&(TRUE == dataEqual);cnt++)
                    {
                        if(Com_RxIPduRuntimeBuff[Com_ConfigStd->ComRxIPdu[IpduId].ComRxIPduBufIndex + cnt] != PduInfoPtr->SduDataPtr[cnt])
                        {
                            dataEqual = FALSE;
                        }
                    }
                }
                else
                {
                    dataEqual = FALSE;
                }
                if(TRUE == dataEqual)
                {
                    (Com_RxIPduRunTimeState[IpduId].RxReplicationNumber) += 1u;
                    if(iPduReplicationQuorum == Com_RxIPduRunTimeState[IpduId].RxReplicationNumber)
                    {
                        /*update the expected counter*/
                        Com_PduCounterIncrease(FALSE,IpduId);
                        Com_RxIPduRunTimeState[IpduId].RxReplicationNumber = 0u;
                        rxPduValid = TRUE;
                    }
                }
            }
        }
        /*rx counter isn't expect counter*/
        else
        {
            if(RxCounter == Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduMaxCounter)
            {
                updateCounter = 0u;
            }
            else
            {
                updateCounter = RxCounter + 1u;
            }
            if(updateCounter != Com_RxIPduRunTimeState[IpduId].RxIpduCounter)
            {
                if(NULL_PTR != Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterErrorNotification)
                {
                    Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduCounterErrorNotification(IpduId,Com_RxIPduRunTimeState[IpduId].RxIpduCounter,RxCounter);
                }
                /*update the expected counter*/
                Com_PduCounterIncrease(FALSE,IpduId);
                Com_RxIPduRunTimeState[IpduId].RxReplicationNumber = 0u;
            }
        }
    }
    return rxPduValid;
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
 * Brief               called by Com_RxIpduController.
 *                     reset OCCURRENCE of filter with ComFilterAlgorithm
 *                     ONE_EVERY_N.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((COM_IPDUGROUP_NUMBER > 0u) && (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u))
static FUNC(void,COM_CODE)
Com_ResetRxOccurrenceOfPduFilter(PduIdType IpduId)
{
    uint16 cnt;
    Com_SignalIdType oneEveryNId;
    #if(0u < COM_RXSIGNAL_NUMBER)
    Com_SignalIdType ipduSignalNumber;
    Com_SignalIdType signalId;
    #endif
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType signalGroupId;
    uint16 counter;
    uint16 groupSignalNumber;
    uint16 groupSignalId;
    #endif
    /*reset signal/group signal of rx pdu which ComFilterAlgorithm is ONE_EVERY_N*/
    #if(0u < COM_RXSIGNAL_NUMBER)
    ipduSignalNumber =  Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRef[cnt];
        if(NULL_PTR != Com_ConfigStd->ComRxSignal[signalId].ComFilter)
        {
            oneEveryNId = Com_ConfigStd->ComRxSignal[signalId].ComFilter->ComOneEveryNId;
            if(COM_UNUSED_UINT16 != oneEveryNId)
            {
                Com_OneEveryNcnt[oneEveryNId] = 0u;
            }
        }
    }
    #endif
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    ipduSignalGroupNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        signalGroupId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        groupSignalNumber = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComGroupSignalNumber;
        for(counter = 0u;counter < groupSignalNumber;counter++)
        {
            groupSignalId = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComRxGroupSignalId[counter];
            if(NULL_PTR != Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter)
            {
                oneEveryNId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter->ComOneEveryNId;
                if(COM_UNUSED_UINT16 != oneEveryNId)
                {
                    Com_OneEveryNcnt[oneEveryNId] = 0u;
                }
            }
        }
    }
    #endif
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxIpduController.
 *                     init the rx ipdu buffer,all signal buffer(included in
 *                     the ipdu,except source signal).
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
Com_ResetRxPduBufferAndSignalBuffer(PduIdType IpduId)
{
    #if(0u < COM_RXIPDUBUFF_SIZE)
    uint16 pduBufferId;
    uint16 initPduLength;
    #endif
    uint16 cnt;
    uint16 signalInitValueId;
    Com_SignalType signalType;
    uint16 signalLength;
    #if(0u < COM_RXSIGNAL_NUMBER)
    Com_SignalIdType ipduSignalNumber;
    Com_SignalIdType signalId;
    #endif
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    Com_SignalGroupIdType ipduSignalGroupNumber;
    Com_SignalGroupIdType signalGroupId;
    uint16 counter;
    uint16 groupSignalNumber;
    uint16 groupSignalId;
    #endif
    /*init the rx ipdu buffer,all signal buffer(included in the ipdu,except source description signal)*/
    #if(0u < COM_RXIPDUBUFF_SIZE)
    pduBufferId = Com_ConfigStd->ComRxIPdu[IpduId].ComRxIPduBufIndex;
    initPduLength = Com_RxIPduInitState[IpduId].RxIpduLength;
    /*init rx pdu buffer*/
    for(cnt = 0u; cnt < initPduLength;cnt++)
    {
        Com_RxIPduRuntimeBuff[pduBufferId + cnt] = Com_RxIPduInitValue[pduBufferId + cnt];
    }
    #endif
    /*init signal buffer*/
    #if(0u < COM_RXSIGNAL_NUMBER)
    ipduSignalNumber =  Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRefNumber;
    for(cnt = 0u;cnt < ipduSignalNumber;cnt++)
    {
        signalId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalsRef[cnt];
        signalInitValueId = Com_ConfigStd->ComRxSignal[signalId].ComSignalInitValueId;
        signalType = Com_ConfigStd->ComRxSignal[signalId].ComSignalType;
        if(COM_UINT8_DYN == signalType)
        {
            signalLength = Com_ConfigStd->ComRxSignal[signalId].ComSignalDataInitValueLength;
        }
        else if(COM_UINT8_N == signalType)
        {
            signalLength = Com_ConfigStd->ComRxSignal[signalId].ComSignalLength;
        }
        else
        {
            signalLength = 0u;
        }
        Com_InitSignalBuffer(signalType,signalInitValueId,signalLength);
    }
    #endif
    /*init group signal buffer*/
    #if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
    ipduSignalGroupNumber = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRefNumber;
    for(cnt = 0u;cnt < ipduSignalGroupNumber;cnt++)
    {
        signalGroupId = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduSignalGroupsRef[cnt];
        groupSignalNumber = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComGroupSignalNumber;
        for(counter = 0u;counter < groupSignalNumber;counter++)
        {
            groupSignalId = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComRxGroupSignalId[counter];
            signalInitValueId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalInitValueId;
            signalType = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType;
            if(COM_UINT8_DYN == signalType)
            {
                signalLength = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalDataInitValueLength;
            }
            else if(COM_UINT8_N == signalType)
            {
                signalLength = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalLength;
            }
            else
            {
                signalLength = 0u;
            }
            Com_InitSignalBuffer(signalType,signalInitValueId,signalLength);
        }
    }
    #endif
    Com_RxIPduRunTimeState[IpduId].GWDynamicSignalLength = 0u;
    Com_RxIPduRunTimeState[IpduId].NeedGW = FALSE;
    Com_RxIPduRunTimeState[IpduId].Receiving = FALSE;
    Com_RxIPduRunTimeState[IpduId].RxIndication = FALSE;
    Com_RxIPduRunTimeState[IpduId].RxIpduLength = Com_RxIPduInitState[IpduId].RxIpduLength;
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_IndicationProcess.
 *                     unpack the rx source description signal to gateway buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SourceSignalId,SourceSignalId,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_SOURCE_SIGNAL_NUMBER)
static FUNC(void,COM_CODE)
Com_SourceSignalUnPack(Com_SignalIdType SourceSignalId,uint16 RxDynSignalLength,uint16 IpduBufferId)
{
    #if(0u < COM_RXIPDUBUFF_SIZE)
    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    uint32 bitPosition;
    uint16 cnt;
    uint16 signal8NLength;
    #endif
    uint16 signalBufferId;
    uint64 sourceSignalValue;
    signalBufferId = Com_ConfigStd->ComSourceSignal[SourceSignalId].GWSignalBufferId;
    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    bitPosition = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComBitPosition;
    #endif
    if(COM_UINT8_N == Com_ConfigStd->ComSourceSignal[SourceSignalId].ComSignalType)
    {
        #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
        signal8NLength = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComSignalLength;
        for(cnt = 0u;cnt < signal8NLength;cnt++)
        {
            Com_GWSignal8BitBuff[signalBufferId + cnt] =
                    Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + cnt];
        }
        #endif
    }
    else if(COM_UINT8_DYN == Com_ConfigStd->ComSourceSignal[SourceSignalId].ComSignalType)
    {
        #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
        for(cnt = 0u;cnt < RxDynSignalLength;cnt++)
        {
            Com_GWSignal8BitBuff[signalBufferId + cnt] = Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + cnt];
        }
        #endif
    }
    else
    {
        sourceSignalValue = Com_SourceSignalNotArrayUnPack(SourceSignalId);
        switch(Com_ConfigStd->ComSourceSignal[SourceSignalId].ComSignalType)
        {
            #if(0u < COM_GWSIGNAL_BOOLBUFF_SIZE)
            case COM_BOOLEAN:
                Com_GWSignalBoolBuff[signalBufferId] = (boolean)sourceSignalValue;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
            case COM_SINT8:
            case COM_UINT8:
                Com_GWSignal8BitBuff[signalBufferId] = (uint8)sourceSignalValue;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_16BITBUFF_SIZE)
            case COM_SINT16:
            case COM_UINT16:
                Com_GWSignal16BitBuff[signalBufferId] = (uint16)sourceSignalValue;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_32BITBUFF_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                Com_GWSignal32BitBuff[signalBufferId] = (uint32)sourceSignalValue;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_64BITBUFF_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                Com_GWSignal64BitBuff[signalBufferId] = sourceSignalValue;
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    #endif
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_IndicationProcess.
 *                     unpack the rx signal and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalId,RxDynSignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_RXSIGNAL_NUMBER)
static FUNC(void,COM_CODE)
Com_SignalRxIndication(Com_SignalIdType RxSignalId,uint16 RxDynSignalLength,uint16 IpduBufferId)
{
    #if(0u < COM_RXIPDUBUFF_SIZE)
    uint16 signal8NLength;
    #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
    uint16 gwSignalBufferId;
    uint16 cnt;
    uint32 bitPosition;
    #endif
    if(COM_UINT8_N == Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalType)
    {
        signal8NLength = Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalLength;
        #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
        gwSignalBufferId = Com_ConfigStd->ComRxSignal[RxSignalId].GWSignalBufferId;
        bitPosition = Com_ConfigStd->ComRxSignal[RxSignalId].ComBitPosition;
        /*the rx signal need to be gateway,then store the value to gateway buffer*/
        if(COM_UNUSED_UINT16 != gwSignalBufferId)
        {
            for(cnt = 0u;cnt < signal8NLength;cnt++)
            {
                Com_GWSignal8BitBuff[gwSignalBufferId + cnt] = Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8U) + cnt];
            }
        }
        #endif
        Com_Rx8NSignalHandle(RxSignalId,signal8NLength,IpduBufferId);
    }
    else if(COM_UINT8_DYN == Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalType)
    {
        #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
        gwSignalBufferId = Com_ConfigStd->ComRxSignal[RxSignalId].GWSignalBufferId;
        bitPosition = Com_ConfigStd->ComRxSignal[RxSignalId].ComBitPosition;
        /*the rx signal need to be gateway,then store the value to gateway buffer*/
        if(COM_UNUSED_UINT16 != gwSignalBufferId)
        {
            for(cnt = 0u;cnt < RxDynSignalLength;cnt++)
            {
                Com_GWSignal8BitBuff[gwSignalBufferId + cnt] = Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8U) + cnt];
            }
        }
        #endif
        Com_Rx8DYNSignalHandle(RxSignalId,RxDynSignalLength,IpduBufferId);
    }
    else
    {
        Com_RxSignalHandle(RxSignalId);
    }
    #endif
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SignalRxIndication.
 *                     unpack the rx signal(signal type is COM_UINT8_N)
 *                     and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId,SignalLength,BufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void,COM_CODE)
Com_Rx8NSignalHandle(Com_SignalIdType SignalId,uint16 SignalLength,uint16 BufferId)
{
    boolean needFilter = TRUE;
    uint16 invalidValueId;
    uint32 bitPosition;
    uint16 cnt;
    boolean invalidValue = TRUE;
    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 initValueId;
    initValueId = Com_ConfigStd->ComRxSignal[SignalId].ComSignalInitValueId;
    #endif
    bitPosition = Com_ConfigStd->ComRxSignal[SignalId].ComBitPosition;
    invalidValueId = Com_ConfigStd->ComRxSignal[SignalId].ComSignalDataInvalidValueId;
    /*the rx signal configuration invalid value*/
    if(COM_UNUSED_UINT16 != invalidValueId)
    {
        #if(0u < COM_SIGNAL_8BIT_INVALID_SIZE)
        for(cnt = 0u;(cnt < SignalLength)&&(TRUE == invalidValue);cnt++)
        {
            if(Com_Signal8BitInvalidValue[invalidValueId + cnt] !=
                    Com_RxIPduRuntimeBuff[BufferId + (uint16)(bitPosition/8u) + cnt])
            {
                invalidValue = FALSE;
            }
        }
        #endif
        if(TRUE == invalidValue)
        {
            if(COM_INVALID_ACTION_NOTIFY == Com_ConfigStd->ComRxSignal[SignalId].ComDataInvalidAction)
            {
                if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComInvalidNotification)
                {
                    Com_ConfigStd->ComRxSignal[SignalId].ComInvalidNotification();
                }
            }
            else
            {
                if((NULL_PTR == Com_ConfigStd->ComRxSignal[SignalId].ComFilter) ||
                        ((NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComFilter)
                                && (COM_ALWAYS == Com_ConfigStd->ComRxSignal[SignalId].ComFilter->ComFilterAlgorithm)))
                {
                    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
                    for(cnt = 0u;cnt < SignalLength;cnt++)
                    {
                        Com_Signal8BitRuntimeBuff[initValueId + cnt] = Com_Signal8BitInitValue[initValueId + cnt];
                    }
                    #endif
                    if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComNotification)
                    {
                        Com_ConfigStd->ComRxSignal[SignalId].ComNotification();
                    }
                }
            }
            needFilter = FALSE;
        }
    }
    if(TRUE == needFilter)
    {
        if((NULL_PTR == Com_ConfigStd->ComRxSignal[SignalId].ComFilter) ||
                ((NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComFilter)
                        && (COM_ALWAYS == Com_ConfigStd->ComRxSignal[SignalId].ComFilter->ComFilterAlgorithm)))
        {
            #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                Com_Signal8BitRuntimeBuff[initValueId + cnt] =
                        Com_RxIPduRuntimeBuff[BufferId + (uint16)(bitPosition/8u) + cnt];
            }
            #endif
            if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComNotification)
            {
                Com_ConfigStd->ComRxSignal[SignalId].ComNotification();
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SignalRxIndication.
 *                     unpack the rx signal(signal type is COM_UINT8_DYN)
 *                     and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId,SignalLength,BufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void,COM_CODE)
Com_Rx8DYNSignalHandle(Com_SignalIdType SignalId,uint16 SignalLength,uint16 BufferId)
{
    boolean needFilter = TRUE;
    uint16 invalidValueId;
    uint32 bitPosition;
    uint16 cnt;
    uint16 initValueLength;
    boolean invalidValue = TRUE;
    uint16 rxIpduLength;
    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 initValueId;
    initValueId = Com_ConfigStd->ComRxSignal[SignalId].ComSignalInitValueId;
    #endif
    initValueLength = Com_ConfigStd->ComRxSignal[SignalId].ComSignalDataInitValueLength;
    bitPosition = Com_ConfigStd->ComRxSignal[SignalId].ComBitPosition;
    invalidValueId = Com_ConfigStd->ComRxSignal[SignalId].ComSignalDataInvalidValueId;
    /*the rx signal configuration invalid value*/
    if(COM_UNUSED_UINT16 != invalidValueId)
    {
        if(SignalLength != Com_ConfigStd->ComRxSignal[SignalId].ComSignalDataInvalidValueLength)
        {
            invalidValue = FALSE;
        }
        else
        {
            #if(0u < COM_SIGNAL_8BIT_INVALID_SIZE)
            for(cnt = 0u;(cnt < SignalLength)&&(TRUE == invalidValue);cnt++)
            {
                if(Com_Signal8BitInvalidValue[invalidValueId + cnt] !=
                        Com_RxIPduRuntimeBuff[BufferId + (uint16)(bitPosition/8u) + cnt])
                {
                    invalidValue = FALSE;
                }
            }
            #endif
        }
        if(TRUE == invalidValue)
        {
            if(COM_INVALID_ACTION_NOTIFY == Com_ConfigStd->ComRxSignal[SignalId].ComDataInvalidAction)
            {
                if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComInvalidNotification)
                {
                    Com_ConfigStd->ComRxSignal[SignalId].ComInvalidNotification();
                }
            }
            else
            {
                if((NULL_PTR == Com_ConfigStd->ComRxSignal[SignalId].ComFilter) ||
                        ((NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComFilter)
                                && (COM_ALWAYS == Com_ConfigStd->ComRxSignal[SignalId].ComFilter->ComFilterAlgorithm)))
                {
                    #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
                    for(cnt = 0u;cnt < initValueLength;cnt++)
                    {
                        Com_Signal8BitRuntimeBuff[initValueId + cnt] = Com_Signal8BitInitValue[initValueId + cnt];
                    }
                    #endif
                    /*change the rx Ipdu length to Init rx Ipdu length*/
                    rxIpduLength = Com_RxIPduRunTimeState[Com_ConfigStd->ComRxSignal[SignalId].ComIpduRefIndex].RxIpduLength;
                    rxIpduLength = rxIpduLength - SignalLength + initValueLength;
                    Com_RxIPduRunTimeState[Com_ConfigStd->ComRxSignal[SignalId].ComIpduRefIndex].RxIpduLength = rxIpduLength;
                    if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComNotification)
                    {
                        Com_ConfigStd->ComRxSignal[SignalId].ComNotification();
                    }
                }
            }
            needFilter = FALSE;
        }
    }
    if(TRUE == needFilter)
    {
        if((NULL_PTR == Com_ConfigStd->ComRxSignal[SignalId].ComFilter) ||
                ((NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComFilter)
                        && (COM_ALWAYS == Com_ConfigStd->ComRxSignal[SignalId].ComFilter->ComFilterAlgorithm)))
        {
            #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                Com_Signal8BitRuntimeBuff[initValueId + cnt] =
                        Com_RxIPduRuntimeBuff[BufferId + (uint16)(bitPosition/8u) + cnt];
            }
            #endif
            if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComNotification)
            {
                Com_ConfigStd->ComRxSignal[SignalId].ComNotification();
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SignalRxIndication.
 *                     unpack the rx signal(signal type isn't COM_UINT8_N or
 *                     COM_UINT8_DYN) and notification up layer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(void,COM_CODE)
Com_RxSignalHandle(Com_SignalIdType SignalId)
{
    boolean needFilter = TRUE;
    uint64 unPackValue;
    uint16 invalidValueId;
    uint16 initValueId;
    uint16 gwSignalBufferId;
    Com_SignalType signalType;
    boolean filterPass;
    boolean invalidValue;
    invalidValueId = Com_ConfigStd->ComRxSignal[SignalId].ComSignalDataInvalidValueId;
    gwSignalBufferId = Com_ConfigStd->ComRxSignal[SignalId].GWSignalBufferId;
    signalType = Com_ConfigStd->ComRxSignal[SignalId].ComSignalType;
    initValueId = Com_ConfigStd->ComRxSignal[SignalId].ComSignalInitValueId;
    /* unpack the signal value from rx ipdu buffer, and do B-End and L-End exchanging,Flag Extension
       and return the result as an uint64 value to variable 'unPackValue'. */
    unPackValue = Com_RxSignalUnPack(SignalId);
    /* update signal value into GW signal buffer */
    Com_UpdateGWSignalBuffer(unPackValue,gwSignalBufferId,signalType);
    /*the rx signal configuration invalid value*/
    if(COM_UNUSED_UINT16 != invalidValueId)
    {
        invalidValue = Com_SignalInvalidateHandle(signalType,invalidValueId,unPackValue);
        if(TRUE == invalidValue)
        {
            if(COM_INVALID_ACTION_NOTIFY == Com_ConfigStd->ComRxSignal[SignalId].ComDataInvalidAction)
            {
                if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComInvalidNotification)
                {
                    Com_ConfigStd->ComRxSignal[SignalId].ComInvalidNotification();
                }
                needFilter = FALSE;
            }
            else
            {
                /*replace signal runtime buffer, with init value. */
                unPackValue = Com_RxSignalReplaceHanlde(signalType,initValueId);
            }
        }
    }
    if(TRUE == needFilter)
    {
        filterPass = Com_RxSignalFilter(
                         unPackValue,
                         Com_ConfigStd->ComRxSignal[SignalId].ComFilter,
                         #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                         initValueId,
                         #endif
                         signalType);
        if(TRUE == filterPass)
        {
            /* update signal value into signal runtime buffer */
            Com_RxSignalBuffHanlde(unPackValue, SignalId);
            /* if this signal has normal rx notification function configured,
               invoke this function. */
            if(NULL_PTR != Com_ConfigStd->ComRxSignal[SignalId].ComNotification)
            {
                Com_ConfigStd->ComRxSignal[SignalId].ComNotification();
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 *                     judge the rx (group) signal value is invalid value or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,InvalidValueId,Value
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((0u < COM_RXGROUPSIGNAL_NUMBER) || (0u < COM_RXSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_SignalInvalidateHandle(Com_SignalType SignalType,uint16 InvalidValueId, uint64 Value)
{
    boolean ret = FALSE;
    switch(SignalType)
    {
        #if(0u < COM_SIGNAL_BOOL_INVALID_SIZE)
        case COM_BOOLEAN:
            if(((boolean)Value) == Com_SignalBoolInvalidValue[InvalidValueId])
            {
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_8BIT_INVALID_SIZE)
        case COM_SINT8:
        case COM_UINT8:
            if(((uint8)Value) == Com_Signal8BitInvalidValue[InvalidValueId])
            {
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_16BIT_INVALID_SIZE)
        case COM_SINT16:
        case COM_UINT16:
            if(((uint16)Value) == Com_Signal16BitInvalidValue[InvalidValueId])
            {
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_32BIT_INVALID_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            if(((uint32)Value) == Com_Signal32BitInvalidValue[InvalidValueId])
            {
                ret = TRUE;
            }
            break;
        #endif
        #if(0u < COM_SIGNAL_64BIT_INVALID_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            if(Value == Com_Signal64BitInvalidValue[InvalidValueId])
            {
                ret = TRUE;
            }
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    COM_NOUSED(InvalidValueId);
    COM_NOUSED(Value);
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,
 *                     Com_RxSignalGroupFilter.
 *                     get the rx (group) signal's init value.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,InitValueId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
#if((0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(uint64,COM_CODE)
Com_RxSignalReplaceHanlde(Com_SignalType SignalType,uint16 InitValueId)
{
    uint64 unPackValue = COM_UNUSED_UINT64;
    switch(SignalType)
    {
        #if(0u < COM_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            unPackValue = (uint64)Com_SignalBoolInitValue[InitValueId];
            break;
        #endif
        #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_SINT8:
        case COM_UINT8:
            unPackValue = (uint64)Com_Signal8BitInitValue[InitValueId];
            break;
        #endif
        #if(0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_SINT16:
        case COM_UINT16:
            unPackValue = (uint64)Com_Signal16BitInitValue[InitValueId];
            break;
        #endif
        #if(0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            unPackValue = (uint64)Com_Signal32BitInitValue[InitValueId];
            break;
        #endif
        #if(0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            unPackValue = Com_Signal64BitInitValue[InitValueId];
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    return unPackValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,
 *                     Com_InvalidSignalReplaceAndNotification.
 *                     update the rx signal value buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Value,Id
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(0u < COM_RXSIGNAL_NUMBER)
static FUNC(void,COM_CODE)
Com_RxSignalBuffHanlde(uint64 Value,Com_SignalIdType Id)
{
    uint16 initValue;
    Com_SignalType rxSignalType;
    initValue = Com_ConfigStd->ComRxSignal[Id].ComSignalInitValueId;
    rxSignalType = Com_ConfigStd->ComRxSignal[Id].ComSignalType;
    switch(rxSignalType)
    {
        #if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            Com_SignalBoolRuntimeBuff[initValue] = (boolean)Value;
            break;
        #endif
        #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_SINT8:
        case COM_UINT8:
            Com_Signal8BitRuntimeBuff[initValue] = (uint8)Value;
            break;
        #endif
        #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_SINT16:
        case COM_UINT16:
            Com_Signal16BitRuntimeBuff[initValue] = (uint16)Value;
            break;
        #endif
        #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            Com_Signal32BitRuntimeBuff[initValue] = (uint32)Value;
            break;
        #endif
        #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            Com_Signal64BitRuntimeBuff[initValue] = (uint64)Value;
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 *                     update the gateway signal value buffer of the rx signal.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Value,GWSignalBufferId,SignalType
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXGROUPSIGNAL_NUMBER) || (0u < COM_RXSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_UpdateGWSignalBuffer(uint64 Value,uint16 GWSignalBufferId,Com_SignalType SignalType)
{
    /*the rx signal need gateway,store signal value in gateway buffer*/
    if(COM_UNUSED_UINT16 != GWSignalBufferId)
    {
        switch(SignalType)
        {
            #if(0u < COM_GWSIGNAL_BOOLBUFF_SIZE)
            case COM_BOOLEAN:
                Com_GWSignalBoolBuff[GWSignalBufferId] = (boolean)Value;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_8BITBUFF_SIZE)
            case COM_SINT8:
            case COM_UINT8:
                Com_GWSignal8BitBuff[GWSignalBufferId] = (uint8)Value;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_16BITBUFF_SIZE)
            case COM_SINT16:
            case COM_UINT16:
                Com_GWSignal16BitBuff[GWSignalBufferId] = (uint16)Value;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_32BITBUFF_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                Com_GWSignal32BitBuff[GWSignalBufferId] = (uint32)Value;
                break;
            #endif
            #if(0u < COM_GWSIGNAL_64BITBUFF_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                Com_GWSignal64BitBuff[GWSignalBufferId] = (uint64)Value;
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    COM_NOUSED(Value);
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_IndicationProcess.
 *                     handle signal group RxIndication of one rx pdu.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalGroupId,RxDynSignalLength,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_SignalGroupRxIndication(Com_SignalGroupIdType RxSignalGroupId,uint16 RxDynSignalLength,uint16 IpduBufferId)
{
    Com_SignalIdType counter,groupSignalNumber;
    boolean signalGroupInvalid;
    boolean needFilter = TRUE;
    Com_SignalIdType groupSignalId;
    #if(STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
    uint16 iPduSignalGroupArrayStartBufId;
    uint16 cnt,signalGroupArrayLength;
    #endif
    groupSignalNumber = Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComGroupSignalNumber;
    /*if the signal group have invalid signal or not*/
    signalGroupInvalid = Com_SignalGroupInvalidCalculate(RxSignalGroupId,groupSignalNumber,IpduBufferId,RxDynSignalLength);
    /*the signal group is invalid if at least one group signal is invalid*/
    if(TRUE == signalGroupInvalid)
    {
        if(COM_INVALID_ACTION_NOTIFY == Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComDataInvalidAction)
        {
            if(NULL_PTR != Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComInvalidNotification)
            {
                Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComInvalidNotification();
            }
            needFilter = FALSE;
        }
        else
        {
            /*replace the rx pdu buffer for signal group array*/
            #if((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_RXIPDUBUFF_SIZE))
            if(NULL_PTR != Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComSignalGroupArray)
            {
                iPduSignalGroupArrayStartBufId = Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComSignalGroupArray->ComIPduStartBufIndex;
                signalGroupArrayLength = Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComSignalGroupArray->SignalGroupArrayLength;
                for(cnt = 0u;cnt < signalGroupArrayLength;cnt++)
                {
                    Com_RxIPduRuntimeBuff[iPduSignalGroupArrayStartBufId + cnt] = Com_RxIPduInitValue[iPduSignalGroupArrayStartBufId + cnt];
                }
            }
            #endif
        }
    }
    if(TRUE == needFilter)
    {
        if(TRUE == Com_RxSignalGroupFilter(RxSignalGroupId,signalGroupInvalid,IpduBufferId))
        {
            /* update all group signal init value into group signal runtime buffer */
            for(counter = 0u;counter < groupSignalNumber;counter++)
            {
                groupSignalId = Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComRxGroupSignalId[counter];
                Com_RxGroupSignalBuffHanlde(signalGroupInvalid,groupSignalId,IpduBufferId,RxDynSignalLength);
            }
            /* if this group signal has normal rx notification function configured,
               invoke this function. */
            if(NULL_PTR != Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComNotification)
            {
                Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComNotification();
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 *                     judge the rx signal group value is filter out or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId,InvalidSignalGroup,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
/* called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 * judge the rx signal group value is filter out or not*/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_RxSignalGroupFilter(Com_SignalGroupIdType SignalGroupId,boolean InvalidSignalGroup,uint16 IpduBufferId)
{
    boolean ret;
    Com_SignalIdType groupSignalNumber;
    Com_SignalIdType cnt;
    boolean signalGroupFilter = TRUE;
    Com_SignalIdType groupSignalId;
    Com_SignalType groupSignalType;
    uint16 groupSignalInitValueId;
    uint64 groupSignalValue;
    groupSignalNumber = Com_ConfigStd->ComRxSignalGroup[SignalGroupId].ComGroupSignalNumber;
    /*the signal group value is invalid,use the init value replace*/
    if(TRUE == InvalidSignalGroup)
    {
        for(cnt = 0u;(cnt < groupSignalNumber)&&(TRUE == signalGroupFilter);cnt++)
        {
            groupSignalId = Com_ConfigStd->ComRxSignalGroup[SignalGroupId].ComRxGroupSignalId[cnt];
            groupSignalType = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType;
            if((COM_UINT8_N == groupSignalType) || (COM_UINT8_DYN == groupSignalType))
            {
                if((NULL_PTR != Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter)
                        && (COM_NEVER == Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter->ComFilterAlgorithm))
                {
                    signalGroupFilter = FALSE;
                }
            }
            else
            {
                groupSignalInitValueId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalInitValueId;
                /*get the init value except COM_UINT8_N and UNIT8_DYN*/
                groupSignalValue = Com_RxSignalReplaceHanlde(groupSignalType,groupSignalInitValueId);
                signalGroupFilter = Com_RxSignalFilter(
                                        groupSignalValue,
                                        Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter,
                                        #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                                        groupSignalInitValueId,
                                        #endif
                                        groupSignalType);
            }
        }
        if(TRUE == signalGroupFilter)
        {
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }
    else
    {
        for(cnt = 0u;(cnt < groupSignalNumber)&&(TRUE == signalGroupFilter);cnt++)
        {
            groupSignalId = Com_ConfigStd->ComRxSignalGroup[SignalGroupId].ComRxGroupSignalId[cnt];
            groupSignalType = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType;
            #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            groupSignalInitValueId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalInitValueId;
            #endif
            if((COM_UINT8_N == groupSignalType) || (COM_UINT8_DYN == groupSignalType))
            {
                if((NULL_PTR != Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter)
                        && (COM_NEVER == Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter->ComFilterAlgorithm))
                {
                    signalGroupFilter = FALSE;
                }
            }
            else
            {
                /*get the unpack value except COM_UINT8_N and UNIT8_DYN*/
                groupSignalValue = Com_RxGroupSignalUnPack(groupSignalId,IpduBufferId);
                signalGroupFilter = Com_RxSignalFilter(
                                        groupSignalValue,
                                        Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComFilter,
                                        #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                                        groupSignalInitValueId,
                                        #endif
                                        groupSignalType);
            }
        }
        if(TRUE == signalGroupFilter)
        {
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 *                     update the rx group signal value buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      InvalidSignalGroup,Id,IpduBufferId,RxDynSignalLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_RxGroupSignalBuffHanlde(boolean InvalidSignalGroup,Com_SignalIdType Id,uint16 IpduBufferId,uint16 RxDynSignalLength)
{
    uint16 initValueId;
    Com_SignalType signalType;
    uint16 SignalLength = 0u;
    uint32 bitPosition;
    uint16 cnt;
    Com_SignalGroupIdType signalGroupId;
    PduIdType rxIpduId;
    uint16 rxIpduMinLength;
    uint64 value;
    initValueId = Com_ConfigStd->ComRxGroupSignal[Id].ComSignalInitValueId;
    signalType = Com_ConfigStd->ComRxGroupSignal[Id].ComSignalType;
    /*rx signal group is invalid,use the init value update the rx group signal buffer*/
    if(TRUE == InvalidSignalGroup)
    {
        #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
        if(COM_UINT8_N == signalType)
        {
            SignalLength = Com_ConfigStd->ComRxGroupSignal[Id].ComSignalLength;
        }
        if(COM_UINT8_DYN == signalType)
        {
            SignalLength = Com_ConfigStd->ComRxGroupSignal[Id].ComSignalDataInitValueLength;
            /*change the rx Ipdu length to Init rx Ipdu length*/
            signalGroupId = Com_ConfigStd->ComRxGroupSignal[Id].ComSignalGroupRef;
            rxIpduId = Com_ConfigStd->ComRxSignalGroup[signalGroupId].ComIpduRefIndex;
            rxIpduMinLength = (uint16)Com_ConfigStd->ComRxIPdu[rxIpduId].IPduNoDynSignalLength;
            Com_RxIPduRunTimeState[rxIpduId].RxIpduLength = rxIpduMinLength + SignalLength;
        }
        #endif/*0u < COM_SIGNAL_8BITBUFF_SIZE*/
        Com_InitSignalBuffer(signalType,initValueId,SignalLength);
    }
    /*rx signal group is valid,use the really rx value update the rx goup signal buffer*/
    else
    {
        bitPosition = Com_ConfigStd->ComRxGroupSignal[Id].ComBitPosition;
        #if((0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_RXIPDUBUFF_SIZE))
        if(COM_UINT8_N == signalType)
        {
            SignalLength = Com_ConfigStd->ComRxGroupSignal[Id].ComSignalLength;
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                Com_Signal8BitRuntimeBuff[initValueId + cnt] = Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + cnt];
            }
        }
        else if(COM_UINT8_DYN == signalType)
        {
            for(cnt = 0u;cnt < RxDynSignalLength;cnt++)
            {
                Com_Signal8BitRuntimeBuff[initValueId + cnt] = Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + cnt];
            }
        }
        else
        #endif/*(0u < COM_SIGNAL_8BITBUFF_SIZE) && (0u < COM_RXIPDUBUFF_SIZE)*/
        {
            value = Com_RxGroupSignalUnPack(Id,IpduBufferId);
            switch(signalType)
            {
                #if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
                case COM_BOOLEAN:
                    Com_SignalBoolRuntimeBuff[initValueId] = (boolean)value;
                    break;
                #endif
                #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
                case COM_SINT8:
                case COM_UINT8:
                    Com_Signal8BitRuntimeBuff[initValueId] = (uint8)value;
                    break;
                #endif
                #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
                case COM_SINT16:
                case COM_UINT16:
                    Com_Signal16BitRuntimeBuff[initValueId] = (uint16)value;
                    break;
                #endif
                #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
                case COM_SINT32:
                case COM_UINT32:
                case COM_FLOAT32:
                    Com_Signal32BitRuntimeBuff[initValueId] = (uint32)value;
                    break;
                #endif
                #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
                case COM_SINT64:
                case COM_UINT64:
                case COM_FLOAT64:
                    Com_Signal64BitRuntimeBuff[initValueId] = value;
                    break;
                #endif
                default:
                    /*do nothing*/
                    break;
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_TpPduInvalidAction.
 *                     rx signal value replace,and notification.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXSIGNAL_NUMBER))
static FUNC(void,COM_CODE)
Com_InvalidSignalReplaceAndNotification(Com_SignalIdType RxSignalId)
{
    uint16 initValueId;
    uint16 signal8NLength;
    uint16 counter;
    uint16 initValueLength;
    uint64 initValue;
    Com_SignalType signalType;
    initValueId = Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalInitValueId;
    signalType = Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalType;
    if(COM_UINT8_N == signalType)
    {
        if((NULL_PTR == Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter) ||
                ((NULL_PTR != Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter)
                        && (COM_ALWAYS == Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter->ComFilterAlgorithm)))
        {
            signal8NLength = Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalLength;
            #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
            for(counter = 0u;counter < signal8NLength;counter++)
            {
                Com_Signal8BitRuntimeBuff[initValueId + counter] = Com_Signal8BitInitValue[initValueId + counter];
            }
            #endif
            if(NULL_PTR != Com_ConfigStd->ComRxSignal[RxSignalId].ComNotification)
            {
                Com_ConfigStd->ComRxSignal[RxSignalId].ComNotification();
            }
        }
    }
    else if(COM_UINT8_DYN == signalType)
    {
        if((NULL_PTR == Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter) ||
                ((NULL_PTR != Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter)
                        && (COM_ALWAYS == Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter->ComFilterAlgorithm)))
        {
            initValueLength = Com_ConfigStd->ComRxSignal[RxSignalId].ComSignalDataInitValueLength;
            #if(0u < COM_SIGNAL_8BITBUFF_SIZE)
            for(counter = 0u;counter < initValueLength;counter++)
            {
                Com_Signal8BitRuntimeBuff[initValueId + counter] = Com_Signal8BitInitValue[initValueId + counter];
            }
            #endif
            if(NULL_PTR != Com_ConfigStd->ComRxSignal[RxSignalId].ComNotification)
            {
                Com_ConfigStd->ComRxSignal[RxSignalId].ComNotification();
            }
        }
    }
    else
    {
        /*replace signal runtime buffer, with init value. */
        initValue = Com_RxSignalReplaceHanlde(signalType,initValueId);
        if(TRUE == Com_RxSignalFilter(
                       initValue,
                       Com_ConfigStd->ComRxSignal[RxSignalId].ComFilter,
                       #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                       initValueId,
                       #endif
                       signalType))
        {
            /* update signal value into signal runtime buffer */
            Com_RxSignalBuffHanlde(initValue,RxSignalId);
            if(NULL_PTR != Com_ConfigStd->ComRxSignal[RxSignalId].ComNotification)
            {
                Com_ConfigStd->ComRxSignal[RxSignalId].ComNotification();
            }
        }
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication,Com_RxGroupSignalBuffHanlde.
 *                     unpack rx group signal(signal type isn't COM_UINT8_N
 *                     or COM_UINT8_DYN).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(uint64,COM_CODE)
Com_RxGroupSignalUnPack(Com_SignalIdType SignalId,uint16 IpduBufferId)
{
    uint64 signalValue;
    uint32 bitPosition;
    Com_SignalType signalType;
    Com_SignalEndiannessType signalEndianness;
    uint16 ipduBufferIndex;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 bitSize;
    uint64 initValue = COM_UNUSED_UINT64;
    bitPosition = Com_ConfigStd->ComRxGroupSignal[SignalId].ComBitPosition;
    signalType = Com_ConfigStd->ComRxGroupSignal[SignalId].ComSignalType;
    signalEndianness = Com_ConfigStd->ComRxGroupSignal[SignalId].ComSignalEndianness;
    if(COM_BOOLEAN == signalType)
    {
        #if(0u < COM_RXIPDUBUFF_SIZE)
        signalValue = (uint64)((uint8)((Com_RxIPduRuntimeBuff[IpduBufferId + (bitPosition/8u)] >> (bitPosition & 0x07u)) & 0x01u));
        #endif
    }
    else
    {
        bitSize = Com_ConfigStd->ComRxGroupSignal[SignalId].ComBitSize;
        ipduBufferIndex = IpduBufferId + (uint16)(bitPosition/8u);
        /*the length value:1-9*/
        signalByteLength = 1u + ((bitSize + (uint8)(bitPosition & 0x07u) - 1u)/8u);
        /*get all byte value(group signal/signal cover,except MSB byte)*/
        #if(0u < COM_RXIPDUBUFF_SIZE)
        signalValue = Com_RxSignalUnPackHandle(signalEndianness,signalByteLength,&ipduBufferIndex);
        dataMSBByte = (uint64)((uint8)(Com_RxIPduRuntimeBuff[ipduBufferIndex] & (uint8)(0xffu >> (7u - ((bitPosition + bitSize - 1u) & 0x07u)))));
        #endif
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
            signalValue |= (dataMSBByte << (((signalByteLength - 1u)*8u) - (uint8)(bitPosition & 0x07u)));
                break;
            default:
                /*do nothing*/
                break;
        }
    }
    return signalValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxSignalHandle.
 *                     unpack the rx signal(signal type isn't COM_UINT8_N or
 *                     COM_UINT8_DYN).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
#if((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(uint64,COM_CODE)
Com_RxSignalUnPack(Com_SignalIdType SignalId)
{
    uint64 signalValue;
    PduIdType rxIpduId;
    uint32 bitPosition;
    Com_SignalType signalType;
    uint16 ipduBufferIndex;
    Com_SignalEndiannessType signalEndianness;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 bitSize;
    uint64 initValue = COM_UNUSED_UINT64;
    rxIpduId = Com_ConfigStd->ComRxSignal[SignalId].ComIpduRefIndex;
    bitPosition = Com_ConfigStd->ComRxSignal[SignalId].ComBitPosition;
    signalType = Com_ConfigStd->ComRxSignal[SignalId].ComSignalType;
    ipduBufferIndex = Com_ConfigStd->ComRxIPdu[rxIpduId].ComRxIPduBufIndex;
    signalEndianness = Com_ConfigStd->ComRxSignal[SignalId].ComSignalEndianness;
    if(COM_BOOLEAN == signalType)
    {
        signalValue = (uint64)((uint8)((Com_RxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] >> (bitPosition & 0x07u)) & 0x01u));
    }
    else
    {
        bitSize = Com_ConfigStd->ComRxSignal[SignalId].ComBitSize;
        ipduBufferIndex = ipduBufferIndex + (uint16)(bitPosition/8u);
        /*the length value:1-9*/
        signalByteLength = 1u + ((bitSize + (uint8)(bitPosition & 0x07u) - 1u)/8u);
        /*get all byte value(group signal/signal cover,except MSB byte)*/
        signalValue = Com_RxSignalUnPackHandle(signalEndianness,signalByteLength,&ipduBufferIndex);
        dataMSBByte = (uint64)((uint8)(Com_RxIPduRuntimeBuff[ipduBufferIndex] & (uint8)(0xffu >> (7u - ((bitPosition + bitSize - 1u) & 0x07u)))));
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
    return signalValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_SourceSignalUnPack.
 *                     unpack the rx source description signal.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SourceSignalId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64
 */
/******************************************************************************/
#if((0u < COM_SOURCE_SIGNAL_NUMBER) && (0u < COM_RXIPDUBUFF_SIZE))
static FUNC(uint64,COM_CODE)
Com_SourceSignalNotArrayUnPack(Com_SignalIdType SourceSignalId)
{
    uint64 signalValue;
    PduIdType rxIpduId;
    uint32 bitPosition;
    Com_SignalType signalType;
    uint16 ipduBufferIndex;
    Com_SignalEndiannessType signalEndianness;
    uint8 signalByteLength;
    uint64 dataMSBByte;
    uint8 bitSize;
    uint64 initValue = COM_UNUSED_UINT64;
    rxIpduId = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComGwIPduRef;
    bitPosition = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComBitPosition;
    signalType = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComSignalType;
    ipduBufferIndex = Com_ConfigStd->ComRxIPdu[rxIpduId].ComRxIPduBufIndex;
    signalEndianness = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComSignalEndianness;
    if(COM_BOOLEAN == signalType)
    {
        signalValue =(uint64)((uint8)((Com_RxIPduRuntimeBuff[ipduBufferIndex + (bitPosition/8u)] >> (bitPosition & 0x07u)) & 0x01u));
    }
    else
    {
        bitSize = Com_ConfigStd->ComSourceSignal[SourceSignalId].ComBitSize;
        ipduBufferIndex = ipduBufferIndex + (uint16)(bitPosition/8u);
        /*the length value:1-9*/
        signalByteLength = 1u + ((bitSize + (uint8)(bitPosition & 0x07u) - 1u)/8u);
        /*get all byte value(group signal/signal cover,except MSB byte)*/
        signalValue = Com_RxSignalUnPackHandle(signalEndianness,signalByteLength,&ipduBufferIndex);
        dataMSBByte =(uint64)((uint8)(Com_RxIPduRuntimeBuff[ipduBufferIndex] & (uint8)(0xffU >> (7u - ((bitPosition + bitSize - 1u) & 0x07u)))));
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
    return signalValue;
}
#endif
/******************************************************************************/
/*
 * Brief               called by Com_RxGroupSignalUnPack,Com_RxSignalUnPack,
 *                     Com_SourceSignalNotArrayUnPack.
 *                     unpack all byte value(group signal/signal cover,except MSB byte).
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalEndianness,SignalByteLength,
 * Param-Name[out]     None
 * Param-Name[in/out]  IpduBufferIndex
 * Return              uint64
 */
/******************************************************************************/
#if(0u < COM_RXIPDUBUFF_SIZE)
static FUNC(uint64,COM_CODE)
Com_RxSignalUnPackHandle(Com_SignalEndiannessType SignalEndianness,uint8 SignalByteLength,uint16* IpduBufferIndex)
{
    uint64 signalValue = 0u;
    uint16 ipduIndex = *IpduBufferIndex;
    switch(SignalByteLength)
    {
        case (uint8)0x01u:
            break;
        case (uint8)0x02u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x03u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x04u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x05u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x06u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x07u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 40u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x08u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 40u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 48u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            break;
        case (uint8)0x09u:
            signalValue = (uint64)Com_RxIPduRuntimeBuff[ipduIndex];
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 8u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 16u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 24u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 32u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 40u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 48u;
            *IpduBufferIndex = Com_SignalEndiannessHandle(SignalEndianness,ipduIndex);
            ipduIndex = *IpduBufferIndex;
            signalValue |= ((uint64)Com_RxIPduRuntimeBuff[ipduIndex]) << 56u;
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
 * Brief               called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,
 *                     Com_RxSignalGroupFilter.
 *                     calculate the rx signal value is filter out or not.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Value,ComFilter,SignalBufferId,SignalType
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
#if((0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
static FUNC(boolean,COM_CODE)
Com_RxSignalFilter(
    uint64 Value,
    P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
    #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint16 SignalBufferId,
    #endif
    Com_SignalType SignalType
)
{
    boolean ret = TRUE;
    Com_FilterAlgorithmType filterType;
    #if(COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
    Com_SignalIdType oneEveryNId;
    uint32 filterOffset;
    uint32 filterPeriod;
    #endif
    #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    Com_SignalIdType maskNewDifferMaskOldId;
    #endif
    if(NULL_PTR != ComFilter)
    {
        ret = FALSE;
        filterType = ComFilter->ComFilterAlgorithm;
        switch(filterType)
        {
            case COM_ALWAYS:
                ret = TRUE;
                break;
            case COM_MASKED_NEW_EQUALS_X:
                if(((Value) & (ComFilter->ComFilterMask))
                    == (ComFilter->ComFilterX))
                {
                    ret = TRUE;
                }
                break;
            case COM_MASKED_NEW_DIFFERS_X:
                if(((Value) & (ComFilter->ComFilterMask))
                    != (ComFilter->ComFilterX))
                {
                    ret = TRUE;
                }
                break;
            /*COM_BOOLEAN not support WITHIN and WITHOUT*/
            case COM_NEW_IS_WITHIN:
                Com_NewIsWithinFilterCalculate(SignalType,Value,ComFilter,&ret);
                break;
            /*COM_BOOLEAN not support WITHIN and OUTSIDE*/
            case COM_NEW_IS_OUTSIDE:
                Com_NewIsWithoutFilterCalculate(SignalType,Value,ComFilter,&ret);
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
            #if(COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            case COM_MASKED_NEW_DIFFERS_MASKED_OLD:
                maskNewDifferMaskOldId = ComFilter->ComMaskNewDifferMaskOldId;
                if(TRUE == Com_MaskNewDifferMaskOldTimeOut[maskNewDifferMaskOldId])
                {
                    ret = TRUE;
                    Com_MaskNewDifferMaskOldTimeOut[maskNewDifferMaskOldId] = FALSE;
                }
                else
                {
                    ret = Com_MaskedNewDiffersMaskedOldFilterCalculate(SignalType,Value,ComFilter,SignalBufferId);
                }
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    return ret;
}
#endif

#if((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER))
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupInvalidCalculate.
 *                     Uint8_Dyn Group Signal Invalid Calculate.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxDynSignalLength,groupSignalId,IpduBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  signalGroupInvalid
 * Return              None
 */
/******************************************************************************/
static FUNC(void,COM_CODE)
Com_Uint8DynGroupSignalInvalidCalculate(
        uint16 RxDynSignalLength,
        Com_SignalIdType groupSignalId,
        uint16 IpduBufferId,
        boolean* signalGroupInvalid)
{
    #if(((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)) && (0u < COM_RXIPDUBUFF_SIZE))
    uint16 counter;
    uint32 bitPosition;
    #endif
    boolean invalidValue = TRUE;
    uint16 invalidValueId;
    uint16 gwSignalBufferId;
    #if(((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)) && (0u < COM_RXIPDUBUFF_SIZE))
    bitPosition = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComBitPosition;
    #endif
    invalidValueId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalDataInvalidValueId;
    gwSignalBufferId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].GWSignalBufferId;
    /*the group signal need gateway,store group signal value in gateway buffer*/
    if(COM_UNUSED_UINT16 != gwSignalBufferId)
    {
        #if((0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (0u < COM_RXIPDUBUFF_SIZE))
        for(counter = 0u;counter < RxDynSignalLength;counter++)
        {
            Com_GWSignal8BitBuff[gwSignalBufferId + counter] =
                    Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + counter];
        }
        #endif
    }
    if((COM_UNUSED_UINT16 != invalidValueId)&&(FALSE == *signalGroupInvalid))
    {
        if(RxDynSignalLength != Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalDataInvalidValueLength)
        {
            invalidValue = FALSE;
        }
        else
        {
            #if((0u < COM_SIGNAL_8BIT_INVALID_SIZE) && (0u < COM_RXIPDUBUFF_SIZE))
            for(counter = 0u;(counter < RxDynSignalLength)&&(TRUE == invalidValue);counter++)
            {
                if(Com_Signal8BitInvalidValue[invalidValueId + counter] !=
                        Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + counter])
                {
                    invalidValue = FALSE;
                }
            }
            #endif
        }
        if(TRUE == invalidValue)
        {
            *signalGroupInvalid = TRUE;
        }
    }
    return;
}
/******************************************************************************/
/*
 * Brief               called by Com_SignalGroupRxIndication.
 *                     Signal Group Invalid Calculate.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxSignalGroupId,groupSignalNumber,IpduBufferId,RxDynSignalLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/******************************************************************************/
static FUNC(boolean,COM_CODE)
Com_SignalGroupInvalidCalculate(
        Com_SignalGroupIdType RxSignalGroupId,
        Com_SignalIdType groupSignalNumber,
        uint16 IpduBufferId,
        uint16 RxDynSignalLength)
{
    boolean signalGroupInvalid = FALSE;
    Com_SignalIdType cnt,groupSignalId;
    boolean invalidValue;
    uint16 invalidValueId;
    #if(((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)) && (0u < COM_RXIPDUBUFF_SIZE))
    uint32 bitPosition;
    uint16 counter;
    #endif
    uint16 gwSignalBufferId;
    #if((0u < COM_RXIPDUBUFF_SIZE) && ((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)))
    uint16 signal8NLength;
    #endif
    uint64 unPackValue;
    for(cnt = 0u;cnt < groupSignalNumber;cnt++)
    {
        invalidValue = TRUE;
        groupSignalId = Com_ConfigStd->ComRxSignalGroup[RxSignalGroupId].ComRxGroupSignalId[cnt];
        #if(((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)) && (0u < COM_RXIPDUBUFF_SIZE))
        bitPosition = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComBitPosition;
        #endif
        invalidValueId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalDataInvalidValueId;
        gwSignalBufferId = Com_ConfigStd->ComRxGroupSignal[groupSignalId].GWSignalBufferId;
        if(COM_UINT8_N == Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType)
        {
            #if((0u < COM_RXIPDUBUFF_SIZE) && ((0u < COM_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_GWSIGNAL_8BITBUFF_SIZE)))
            signal8NLength = Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalLength;
            #endif
            #if((0u < COM_GWSIGNAL_8BITBUFF_SIZE) && (0u < COM_RXIPDUBUFF_SIZE))
            /*the group signal need gateway,store group signal value in gateway buffer*/
            if(COM_UNUSED_UINT16 != gwSignalBufferId)
            {
                for(counter = 0u;counter < signal8NLength;counter++)
                {
                    Com_GWSignal8BitBuff[gwSignalBufferId + counter] =
                            Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + counter];
                }
            }
            #endif
            #if((0u < COM_SIGNAL_8BIT_INVALID_SIZE) && (0u < COM_RXIPDUBUFF_SIZE))
            if((COM_UNUSED_UINT16 != invalidValueId)&&(FALSE == signalGroupInvalid))
            {
                for(counter = 0u;(counter < signal8NLength)&&(TRUE == invalidValue);counter++)
                {
                    if(Com_Signal8BitInvalidValue[invalidValueId + counter] !=
                            Com_RxIPduRuntimeBuff[IpduBufferId + (uint16)(bitPosition/8u) + counter])
                    {
                        invalidValue = FALSE;
                    }
                }
                if(TRUE == invalidValue)
                {
                    signalGroupInvalid = TRUE;
                }
            }
            #endif
        }
        else if(COM_UINT8_DYN == Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType)
        {
            Com_Uint8DynGroupSignalInvalidCalculate(
                    RxDynSignalLength,
                    groupSignalId,
                    IpduBufferId,
                    &signalGroupInvalid);
        }
        else
        {
            unPackValue = Com_RxGroupSignalUnPack(groupSignalId,IpduBufferId);
            Com_UpdateGWSignalBuffer(unPackValue,gwSignalBufferId,Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType);
            if((COM_UNUSED_UINT16 != invalidValueId)&&(FALSE == signalGroupInvalid))
            {
                invalidValue = Com_SignalInvalidateHandle(Com_ConfigStd->ComRxGroupSignal[groupSignalId].ComSignalType,invalidValueId,unPackValue);
                if(TRUE == invalidValue)
                {
                    signalGroupInvalid = TRUE;
                }
            }
        }
    }
    return signalGroupInvalid;
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif/*0u < COM_RXIPDU_NUMBER*/









