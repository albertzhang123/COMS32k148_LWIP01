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
**  FILENAME    : Com_Internal.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : COM internal API definitions                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Internal.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionRx,
 *                     Com_ResetRxPduBufferAndSignalBuffer,
 *                     Com_ResetTxPduBufferAndSignalBuffer.
 *                     init rx signal/group signal buffer
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalInitValueId,SignalLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_IPDUGROUP_NUMBER) || (0u < COM_RXIPDU_NUMBER))
FUNC(void , COM_CODE)
Com_InitSignalBuffer(Com_SignalType SignalType,uint16 SignalInitValueId,uint16 SignalLength)
{
    #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
    uint16 cnt;
    #endif
    switch(SignalType)
    {
        /*According to the signal type,
         *Copy the init signal value to signal runtime buffer*/
        #if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            Com_SignalBoolRuntimeBuff[SignalInitValueId] = Com_SignalBoolInitValue[SignalInitValueId];
            break;
        #endif
        #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8_N:
        case COM_UINT8_DYN:
            for(cnt = 0u;cnt < SignalLength;cnt++)
            {
                Com_Signal8BitRuntimeBuff[SignalInitValueId + cnt] =
                        Com_Signal8BitInitValue[SignalInitValueId + cnt];
            }
            break;
        case COM_UINT8:
        case COM_SINT8:
            Com_Signal8BitRuntimeBuff[SignalInitValueId] = Com_Signal8BitInitValue[SignalInitValueId];
            break;
        #endif
        #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            Com_Signal16BitRuntimeBuff[SignalInitValueId] = Com_Signal16BitInitValue[SignalInitValueId];
            break;
        #endif
        #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_UINT32:
        case COM_SINT32:
        case COM_FLOAT32:
            Com_Signal32BitRuntimeBuff[SignalInitValueId] = Com_Signal32BitInitValue[SignalInitValueId];
            break;
        #endif
        #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_UINT64:
        case COM_SINT64:
        case COM_FLOAT64:
            Com_Signal64BitRuntimeBuff[SignalInitValueId] = Com_Signal64BitInitValue[SignalInitValueId];
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
 * Brief               Called by Com_RxSignalUnPackHandle,
 *                     Com_TxArrayGroupSignalUnPackHandle.
 *                     base the signal endianness,get the high byte number
 *                     of the (group)signal
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalEndianness,IpduBufferIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_RXIPDUBUFF_SIZE) ||   \
((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)  \
        && (COM_TXSIGNALGROUP_NUMBER > 0u) && (0u < COM_TXIPDUBUFF_SIZE)))
FUNC(uint16 , COM_CODE)
Com_SignalEndiannessHandle(Com_SignalEndiannessType SignalEndianness,uint16 IpduBufferIndex)
{
    uint16 retIpduBufferIndex = IpduBufferIndex;
    if(COM_BIG_ENDIAN == SignalEndianness)
    {
        retIpduBufferIndex--;
    }
    else
    {
        retIpduBufferIndex++;
    }
    return retIpduBufferIndex;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalPack,Com_TxGroupSignalPack,
 *                     Com_DestSignalPack.
 *                     pack signal which signal type is not boolean,COM_UINT8_N,
 *                     COM_UINT8_DYN,and the signal value bits cover 2-9 bytes
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Parameter,TxValue,TxMSBValue
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GWMAPPING_NUMBER))
FUNC(void , COM_CODE)
Com_TxSignalPackHandle(
        Com_TxSignalPackType Parameter,
        uint64 TxValue,
        uint64 TxMSBValue)
{
    #if(0u < COM_TXIPDUBUFF_SIZE)
    uint8 cnt;
    uint32 bitPosition;
    bitPosition = Parameter.BitPosition;
    if(COM_BIG_ENDIAN == Parameter.SignalEndianness)
    {
        for(cnt = 0u;cnt < Parameter.SignalByteLength;cnt++)
        {
            if(0u == cnt)
            {
                /* Clear corresponding bits in IPdu */
                Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u)] &=
                        (0xFFu >> (8u - ((uint8)bitPosition & 0x07u)));
                /* write LSB byte in IPdu */
                Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u)] |= ((uint8)(TxValue));
            }
            else if(Parameter.SignalByteLength == (cnt + 1u))
            {
                if(8u == cnt)
                {
                    /* Clear corresponding bits in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) - 8u] &=
                            (uint8)(0xFFu << ((((uint8)bitPosition & 0x07u) + Parameter.BitSize) - 64u));
                    /* write MSB byte in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) - 8u] |=
                            ((uint8)(TxMSBValue)) & (0xFFu >> (72u - (((uint8)bitPosition & 0x07u) + Parameter.BitSize)));
                }
                else
                {
                    TxValue = TxValue >> 8u;
                    /* Clear corresponding bits in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) - cnt] &=
                            (uint8)(0xFFu << ((((uint8)bitPosition & 0x07u) + Parameter.BitSize) - (cnt*8u)));
                    /* write MSB byte in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) - cnt] |=
                            ((uint8)(TxValue)) & (0xFFu >> (((Parameter.SignalByteLength)*8u) - (((uint8)bitPosition & 0x07u) + Parameter.BitSize)));
                }
            }
            else
            {
                TxValue = TxValue >> 8u;
                /* write txIPduBuffer next byte directly by copy value */
                Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) - cnt] = (uint8)(TxValue);
            }
        }
    }
    else
    {
        for(cnt = 0u;cnt < Parameter.SignalByteLength;cnt++)
        {
            if(0u == cnt)
            {
                /* Clear corresponding bits in IPdu */
                Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u)] &=
                        (0xFFu >> (8u - ((uint8)bitPosition & 0x07u)));
                /* write LSB byte in IPdu */
                Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u)] |= ((uint8)(TxValue));
            }
            else if(Parameter.SignalByteLength == (cnt + 1u))
            {
                if(8u == cnt)
                {
                    /* Clear corresponding bits in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) + 8u] &=
                            (uint8)(0xFFu << ((((uint8)bitPosition & 0x07u) + Parameter.BitSize) - 64u));
                    /* write MSB byte in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) + 8u] |=
                            ((uint8)(TxMSBValue)) & (0xFFu >> (72u - (((uint8)bitPosition & 0x07u) + Parameter.BitSize)));
                }
                else
                {
                    TxValue = TxValue >> 8u;
                    /* Clear corresponding bits in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) + cnt] &=
                            (uint8)(0xFFu << ((((uint8)bitPosition & 0x07u) + Parameter.BitSize) - (cnt*8u)));
                    /* write MSB byte in IPdu */
                    Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) + cnt] |=
                            ((uint8)(TxValue)) & (0xFFu >> (((Parameter.SignalByteLength)*8u) - (((uint8)bitPosition & 0x07u) + Parameter.BitSize)));
                }
            }
            else
            {
                TxValue = TxValue >> 8u;
                /* write txIPduBuffer next byte directly by copy value */
                Com_TxIPduRuntimeBuff[Parameter.IpduBufferIndex + (uint16)(bitPosition/8u) + cnt] = (uint8)(TxValue);
            }
        }
    }
    #endif
    return;
}
#endif/*(0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GWMAPPING_NUMBER)*/
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMCCalculate,
 *                     Com_DestSignalTMCCalculate,Com_RxSignalFilter.
 *                     when FilterAlgorithm is NEW_IS_WITHIN,calculate
 *                     the signal's TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValue,ComFilter,FilterOk
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER) ||(0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
FUNC(void , COM_CODE)
Com_NewIsWithinFilterCalculate(
        Com_SignalType SignalType,
        uint64 SignalValue,
        P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
        boolean* FilterOk)
{
    *FilterOk = FALSE;
    switch(SignalType)
    {
        /*According to the signal type,
         *Calculated the signal value is filtered or not?*/
        case COM_SINT8:
            if((((sint8)(SignalValue)) <= ((sint8)(ComFilter->ComFilterMax)))
                && (((sint8)(SignalValue)) >= ((sint8)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_SINT16:
            if((((sint16)(SignalValue)) <= ((sint16)(ComFilter->ComFilterMax)))
                && (((sint16)(SignalValue)) >= ((sint16)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_SINT32:
            if((((sint32)(SignalValue)) <= ((sint32)(ComFilter->ComFilterMax)))
                && (((sint32)(SignalValue)) >= ((sint32)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_SINT64:
            if((((sint64)(SignalValue)) <= ((sint64)(ComFilter->ComFilterMax)))
                && (((sint64)(SignalValue)) >= ((sint64)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_UINT8:
        case COM_UINT16:
        case COM_UINT32:
        case COM_UINT64:
            if(((SignalValue) <= (ComFilter->ComFilterMax))
                && ((SignalValue) >= (ComFilter->ComFilterMin)))
            {
                *FilterOk = TRUE;
            }
            break;
        default:
            /*do nothing*/
            break;
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMCCalculate,
 *                     Com_DestSignalTMCCalculate,Com_RxSignalFilter.
 *                     when FilterAlgorithm is NEW_IS_WITHOUT,
 *                     calculate the signal's TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValue,ComFilter,FilterOk
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER) ||(0u < COM_RXSIGNAL_NUMBER) || (0u < COM_RXGROUPSIGNAL_NUMBER))
FUNC(void , COM_CODE)
Com_NewIsWithoutFilterCalculate(
        Com_SignalType SignalType,
        uint64 SignalValue,
        P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
        boolean* FilterOk)
{
    *FilterOk = FALSE;
    switch(SignalType)
    {
        /*According to the signal type,
         *Calculated the signal value is filtered or not?*/
        case COM_SINT8:
            if((((sint8)(SignalValue)) > ((sint8)(ComFilter->ComFilterMax)))
                || (((sint8)(SignalValue)) < ((sint8)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_SINT16:
            if((((sint16)(SignalValue)) > ((sint16)(ComFilter->ComFilterMax)))
                || (((sint16)(SignalValue)) < ((sint16)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_SINT32:
            if((((sint32)(SignalValue)) > ((sint32)(ComFilter->ComFilterMax)))
                || (((sint32)(SignalValue)) < ((sint32)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_SINT64:
            if((((sint64)(SignalValue)) > ((sint64)(ComFilter->ComFilterMax)))
                || (((sint64)(SignalValue)) < ((sint64)(ComFilter->ComFilterMin))))
            {
                *FilterOk = TRUE;
            }
            break;
        case COM_UINT8:
        case COM_UINT16:
        case COM_UINT32:
        case COM_UINT64:
            if(((SignalValue) > (ComFilter->ComFilterMax))
                || ((SignalValue) < (ComFilter->ComFilterMin)))
            {
                *FilterOk = TRUE;
            }
            break;
        default:
            /*do nothing*/
            break;
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMHandle,Com_TxIpduTMSHandle.
 *                     calculate the tx ipdu TMS(signal/Group signal/Dest signal)
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TXSIGNAL_NUMBER) ||  \
        ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)) ||  \
        (0u < COM_GWMAPPING_NUMBER))
FUNC(boolean , COM_CODE)
Com_TxIpduTMSCalculate(PduIdType TxIpduId)
{
    boolean ret = TRUE;
    #if(0u < COM_TMCTXSIGNAL_NUMBER)
    uint16 cnt;
    uint16 counter;
    Com_SignalIdType txSignalNumber;
    Com_SignalIdType txSignalId;
    Com_SignalIdType destSignalNumber;
    Com_SignalIdType destSignalId;
    Com_SignalGroupIdType signalGroupNumber;
    Com_SignalGroupIdType signalGroupId;
    Com_SignalIdType groupSignalNumber;
    Com_SignalIdType groupSignalId;
    /*just configuration two TxMode,need to Calculate the TMS of TxPdu.
     *ComTxModeTrue must be configured,ComTxModeFalse select configuration*/
    if(NULL_PTR != Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxModeFalse)
    {
        ret = FALSE;
        /*Calculate TMS based on all signals contained in this TxPdu*/
        txSignalNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRefNumber;
        for(cnt = 0u;(cnt < txSignalNumber)&&(FALSE == ret);cnt++)
        {
            txSignalId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalsRef[cnt];
            if(NULL_PTR != Com_ConfigStd->ComTxSignal[txSignalId].ComFilter)
            {
                if(TRUE == TxSignalTMCRunTime[Com_ConfigStd->ComTxSignal[txSignalId].ComFilter->ComTMCBufferId])
                {
                    ret = TRUE;
                }
            }
        }
        if(FALSE == ret)
        {
            /*Calculate TMS based on all dest description signals contained in this TxPdu*/
            destSignalNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduDestSignalsRefNumber;
            for(cnt = 0u;(cnt < destSignalNumber)&&(FALSE == ret);cnt++)
            {
                destSignalId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduDestSignalsRef[cnt];
                if(NULL_PTR != Com_ConfigStd->ComDestSignal[destSignalId].ComFilter)
                {
                    if(TRUE == TxSignalTMCRunTime[Com_ConfigStd->ComDestSignal[destSignalId].ComFilter->ComTMCBufferId])
                    {
                        ret = TRUE;
                    }
                }
            }
        }
        if(FALSE == ret)
        {
            /*Calculate TMS based on all group signals contained in this TxPdu*/
            signalGroupNumber = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRefNumber;
            for(cnt = 0u;(cnt < signalGroupNumber)&&(FALSE == ret);cnt++)
            {
                signalGroupId = Com_ConfigStd->ComTxIPdu[TxIpduId].ComIPduSignalGroupsRef[cnt];
                groupSignalNumber = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComGroupSignalNumber;
                for(counter = 0u;(counter < groupSignalNumber)&&(FALSE == ret);counter++)
                {
                    groupSignalId = Com_ConfigStd->ComTxSignalGroup[signalGroupId].ComTxGroupSignalId[counter];
                    if(NULL_PTR != Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter)
                    {
                        if(TRUE == TxSignalTMCRunTime[Com_ConfigStd->ComTxGroupSignal[groupSignalId].ComFilter->ComTMCBufferId])
                        {
                            ret = TRUE;
                        }
                    }
                }
            }
        }
    }
    #endif
    COM_NOUSED(TxIpduId);
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignalGroupArray,Com_SendSignalGroup,
 *                     Com_GwUint8DYNSignal,Com_GwNotArraySignal,Com_GwUint8NSignal.
 *                     calculate the TMS,store new Tx mode,get rptNum value for
 *                     DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  RptNum
 * Return              None
 */
/******************************************************************************/
#if(((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)) || (0u < COM_GWMAPPING_NUMBER))
FUNC(void , COM_CODE)
Com_TxIpduTMSHandle(PduIdType TxPduId,uint16* RptNum)
{
    boolean oldTMS;
    oldTMS = Com_TxIPduRunTimeState[TxPduId].TMS;
    /*calculate the TMS of the Ipdu,need to query every signal/group signal/dest signal,until find one TRUE TMC*/
    Com_TxIPduRunTimeState[TxPduId].TMS = Com_TxIpduTMSCalculate(TxPduId);
    /*According to the TMS get send mode, event trigger send times, send cycles*/
    if(TRUE == Com_TxIPduRunTimeState[TxPduId].TMS)
    {
        Com_TxIPduRunTimeState[TxPduId].ipduTxMode = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeMode;
        *RptNum = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeNumberOfRepetitions;
        if(FALSE == oldTMS)
        {
            Com_TxIPduRunTimeState[TxPduId].PeriodCnt = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeTrue->ComTxModeTimeOffset;
        }
    }
    else
    {
        Com_TxIPduRunTimeState[TxPduId].ipduTxMode = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse->ComTxModeMode;
        *RptNum  = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse->ComTxModeNumberOfRepetitions;
        if(TRUE == oldTMS)
        {
            Com_TxIPduRunTimeState[TxPduId].PeriodCnt = Com_ConfigStd->ComTxIPdu[TxPduId].ComTxModeFalse->ComTxModeTimeOffset;
        }
    }
    return;
}
#endif/*(0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_GWMAPPING_NUMBER)*/
/******************************************************************************/
/*
 * Brief               Called by Com_SendSignal,Com_SendDynSignal,
 *                     Com_SendSignalGroup,Com_SendSignalGroupArray.
 *                     set tx signal update bit to 1
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TxIpduId,UpdateBitPosition
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (0u < COM_TXIPDU_NUMBER)
FUNC(void , COM_CODE)
Com_SetSignalUpdateBit(PduIdType TxIpduId,uint32 UpdateBitPosition)
{
    uint16 ipduBufferIndex;
    if(COM_UNUSED_UINT32 != UpdateBitPosition)
    {
        /*If the update bit is configured, the corresponding bit is set to 1*/
        ipduBufferIndex = Com_ConfigStd->ComTxIPdu[TxIpduId].ComTxIPduBufIndex;
        #if(0u < COM_TXIPDUBUFF_SIZE)
        Com_TxIPduRuntimeBuff[ipduBufferIndex + (uint16)(UpdateBitPosition/8u)] |=
                (uint8)(0x01u << (((uint8)UpdateBitPosition) & 0x07u));
        #endif
    }
    return;
}
#endif
/******************************************************************************/
/*
 * Brief               Called by Com_MainFunctionTx,Com_RxPduCounterValid,
 *                     Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,
 *                     Com_RxPduReplicationValid.
 *                     update the expected counter
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      IsTxPdu,IpduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TXIPDU_NUMBER) || (0u < COM_RXIPDU_NUMBER))
FUNC(void , COM_CODE)
Com_PduCounterIncrease(boolean IsTxPdu,PduIdType IpduId)
{
    uint8 maxCounter;
    uint8 activeCounter;
    if(TRUE == IsTxPdu)
    {
        /*Update TxPdu's counter value*/
        #if(0u < COM_TXIPDU_NUMBER)
        maxCounter = Com_ConfigStd->ComTxIPdu[IpduId].ComIPduCounter->ComIPduMaxCounter;
        activeCounter = Com_TxIPduRunTimeState[IpduId].TxIpduCounter;
        if(maxCounter == activeCounter)
        {
            Com_TxIPduRunTimeState[IpduId].TxIpduCounter = 0u;
        }
        else
        {
            (Com_TxIPduRunTimeState[IpduId].TxIpduCounter) += 1u;
        }
        #endif
    }
    else
    {
        /*Update RxPdu's counter value*/
        #if(0u < COM_RXIPDU_NUMBER)
        maxCounter = Com_ConfigStd->ComRxIPdu[IpduId].ComIPduCounter->ComIPduMaxCounter;
        activeCounter = Com_RxIPduRunTimeState[IpduId].RxIpduCounter;
        if(maxCounter == activeCounter)
        {
            Com_RxIPduRunTimeState[IpduId].RxIpduCounter = 0u;
        }
        else
        {
            (Com_RxIPduRunTimeState[IpduId].RxIpduCounter) += 1u;
        }
        #endif
    }
    return;
}
#endif/*(0u < COM_TXIPDU_NUMBER) && (0u < COM_RXIPDU_NUMBER)*/
/******************************************************************************/
/*
 * Brief               Called by Com_TxSignalTMCCalculate,
 *                     Com_RxSignalFilter.
 *                     when FilterAlgorithm is MASKED_NEW_DIFFERS_MASKED_OLD,
 *                     calculate the signal's TMC
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalType,SignalValue,ComFilter,SignalBufferId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((0u < COM_TMCTXSIGNAL_NUMBER) || (0u < COM_TMCTXGROUPSIGNAL_NUMBER) || (COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER > 0u))
FUNC(boolean,COM_CODE)
Com_MaskedNewDiffersMaskedOldFilterCalculate(
        Com_SignalType SignalType,
        uint64 SignalValue,
        P2CONST(Com_FilterType, AUTOMATIC, COM_APPL_CONST) ComFilter,
        uint16 SignalBufferId)
{
    boolean filterOk = FALSE;
    switch(SignalType)
    {
        /*According to the signal type,
         *Calculated the signal value is filtered or not?*/
        #if (0u < COM_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            if((((boolean)(SignalValue)) & ((boolean)(ComFilter->ComFilterMask)))
                != (Com_SignalBoolRuntimeBuff[SignalBufferId] & ((boolean)(ComFilter->ComFilterMask))))
            {
                filterOk = TRUE;
            }
            break;
        #endif
        #if (0u < COM_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            if((((uint8)(SignalValue)) & ((uint8)(ComFilter->ComFilterMask)))
                != (Com_Signal8BitRuntimeBuff[SignalBufferId] & ((uint8)(ComFilter->ComFilterMask))))
            {
                filterOk = TRUE;
            }
            break;
        #endif
        #if (0u < COM_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            if((((uint16)(SignalValue)) & ((uint16)(ComFilter->ComFilterMask)))
                != (Com_Signal16BitRuntimeBuff[SignalBufferId] & ((uint16)(ComFilter->ComFilterMask))))
            {
                filterOk = TRUE;
            }
            break;
        #endif
        #if (0u < COM_SIGNAL_32BITBUFF_SIZE)
        case COM_UINT32:
        case COM_SINT32:
            if((((uint32)(SignalValue)) & ((uint32)(ComFilter->ComFilterMask)))
                != (Com_Signal32BitRuntimeBuff[SignalBufferId] & ((uint32)(ComFilter->ComFilterMask))))
            {
                filterOk = TRUE;
            }
            break;
        #endif
        #if (0u < COM_SIGNAL_64BITBUFF_SIZE)
        case COM_UINT64:
        case COM_SINT64:
            if(((SignalValue) & (ComFilter->ComFilterMask))
                != (Com_Signal64BitRuntimeBuff[SignalBufferId] & (ComFilter->ComFilterMask)))
            {
                filterOk = TRUE;
            }
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    return filterOk;
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/*******************************************************************************
**                            General Notes                                   **
*******************************************************************************/




