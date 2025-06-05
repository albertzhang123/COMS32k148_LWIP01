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
**  FILENAME    : PduR_Internal.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : PDUR internal API definitions                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Internal.h"
#if(STD_OFF == PDUR_ZERO_COST_OPERATION)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if(STD_ON == PDUR_TP_COPYTXDATA)
/*copy the tx data from the tp buffer gateway on the fly*/
#if(PDUR_TP_BUFFER_SUM > 0u)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyTxDataFromTpBufferFly(
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);
#endif/*PDUR_TP_BUFFER_SUM > 0u*/

/*copy the tx data from the tx buffer*/
#if(PDUR_TX_BUFFER_SUM > 0u)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyTxDataFromTxBuffer(
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);
#endif/*PDUR_TX_BUFFER_SUM > 0u*/
#endif/*STD_ON == PDUR_TP_COPYTXDATA*/

/*copy the rx pdu data to the tp buffer*/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyRxTpDataToTpBuffer(
        uint8 TpBufferId,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) Info,
        P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr);
#endif/*0u < PDUR_TP_BUFFER_SUM*/

/*find one unused tp buffer.if no unused tp buffer,return 0xff*/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(uint8, PDUR_CODE)
PduR_FindNextValidTpBuffer(void);
#endif/*0u < PDUR_TP_BUFFER_SUM*/

/*gateway MF pdu to Tp mode by LoTp_Transmit*/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_GateWayMFToTp(PduIdType PduRDestPduId);
#endif/*0u < PDUR_TP_BUFFER_SUM*/

/*gateway SF pdu to Tp mode by LoTp_Transmit*/
#if((STD_ON == PDUR_TP_RXINDICATION) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_GateWaySFToTp(PduIdType PduRDestPduId);
#endif

/*find one valid tp buffer.if not find,return 0xff*/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(uint8, PDUR_CODE)
PduR_FindValidTpBuffer(uint16 PduRSrcPduId,PduLengthType PduLength);
/*find the rx tp pdu active store tp buffer*/
static FUNC(uint8, PDUR_CODE)
PduR_FindTpBuffer(uint16 PduRSrcPduId);
#endif/*0u < PDUR_TP_BUFFER_SUM*/
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif/*0u < PDUR_DEST_GATEWAY_TP_PDU_SUM*/
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               Rx Tp Pdu gateway to only one Tp Module Pdu handle,
 *                     when start of reception.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SrcPduId,info,Length
 * Param-Name[out]     bufferSizePtr
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
#if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
#if(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_StartOfReceptionToOneTpHandle
(
    PduIdType SrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    PduLengthType Length,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_OK;
    #if (0u < PDUR_TP_BUFFER_SUM)
    uint8 findTpBuffer;
    #endif
    #if(PDUR_TX_BUFFER_SUM > 0u)
    uint16 pduRDestTxBuffeId;
    #endif
    PduIdType gatewayTpRunTimeId;
    PduIdType destPduId;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 metaDataLength;
    metaDataLength = PDUR_SRCEPDU_CFG(SrcPduId).MetaDataLength;
    #endif
    destPduId = PDUR_ROUTINGPATH_CFG(SrcPduId).PduRDestPduIdRef[0u];
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(destPduId).GateWayTpRunTimeIndex;
    if(TRUE == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation)
    {
        #if(PDUR_TX_BUFFER_SUM > 0u)
        pduRDestTxBuffeId = PDUR_DESTPDU_CFG(destPduId).PduRDestTxBufferRef;
        /*SF tp pdu gateway,need store in dedicated buffer(BufferDepth is 1)*/
        if((PDUR_UNUSED_UINT16 != pduRDestTxBuffeId)
                && (PduR_TxBuffer[pduRDestTxBuffeId].PduRPduMaxLength >= Length))
        {
            if(FALSE == PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].used)
            {
                PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].used = TRUE;
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                if((0u < metaDataLength) && (NULL_PTR != info) && (NULL_PTR != info->MetaDataPtr))
                {
                    PduR_Memcpy(
                            PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].MetaData,
                            info->MetaDataPtr,
                            (PduLengthType)metaDataLength);
                }
                #endif
                if((NULL_PTR != info) && (NULL_PTR != info->SduDataPtr))
                {
                    PduR_Memcpy(
                            PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].TxBufferData,
                            info->SduDataPtr,
                            info->SduLength);
                    PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].SduLength = info->SduLength;
                }
                else
                {
                    PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].SduLength = 0u;
                }
                PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset = 0u;
                PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation = FALSE;
                *bufferSizePtr = (PduR_TxBuffer[pduRDestTxBuffeId].PduRPduMaxLength
                        - PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].SduLength);
            }
            else
            {
                bufQeqReturn = BUFREQ_E_OVFL;
            }
        }
        /*MF tp pdu gateway,need store in tp buffer*/
        else
        #endif/*PDUR_TX_BUFFER_SUM > 0u*/
        {
            #if (0u < PDUR_TP_BUFFER_SUM)
            findTpBuffer = PduR_FindValidTpBuffer(SrcPduId,Length);
            /*find the valid tp buffer*/
            if(PDUR_UNUSED_UINT8 != findTpBuffer)
            {
                PduR_TpBuffer[findTpBuffer].used = TRUE;
                PduR_TpBuffer[findTpBuffer].RxBufferOffset = 0u;
                PduR_TpBuffer[findTpBuffer].SduLength = Length;
                PduR_TpBuffer[findTpBuffer].PduHandleId = SrcPduId;
                PduR_TpBuffer[findTpBuffer].NextTpBufferId = PDUR_UNUSED_UINT8;
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                if((0u < metaDataLength) && (NULL_PTR != info) && (NULL_PTR != info->MetaDataPtr))
                {
                    PduR_Memcpy(PduR_TpBuffer[findTpBuffer].MetaData,info->MetaDataPtr,(PduLengthType)metaDataLength);
                }
                #endif
                if((NULL_PTR != info) && (NULL_PTR != info->SduDataPtr))
                {
                    bufQeqReturn = PduR_CopyRxTpDataToTpBuffer(findTpBuffer,info,bufferSizePtr);
                }
                else
                {
                    *bufferSizePtr = PduR_TpBuffer[findTpBuffer].TpBufferLength;
                }
                if(BUFREQ_OK == bufQeqReturn)
                {
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId = findTpBuffer;
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation = FALSE;
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset = 0u;
                    /*gateway-on-the-fly*/
                    if(TRUE == PDUR_ROUTINGPATH_CFG(SrcPduId).GatewayOnTheFly)
                    {
                        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly = TRUE;
                        if(PduR_TpBuffer[findTpBuffer].RxBufferOffset >= PDUR_DESTPDU_CFG(destPduId).PduRTpThreshold)
                        {
                            /*gateway MF pdu to Tp mode by LoTp_Transmit*/
                            if(E_NOT_OK == PduR_GateWayMFToTp(destPduId))
                            {
                                PduR_ClearBufferAndStateOfGateWayTpPdu(SrcPduId);
                                bufQeqReturn = BUFREQ_E_NOT_OK;
                            }
                            PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly = FALSE;
                        }
                    }
                }
                else
                {
                    PduR_ClearBufferAndStateOfGateWayTpPdu(SrcPduId);
                    bufQeqReturn = BUFREQ_E_NOT_OK;
                }
            }
            else
            #endif/*0u < PDUR_TP_BUFFER_SUM*/
            {
                bufQeqReturn = BUFREQ_E_OVFL;
            }
        }
    }
    else
    {
        bufQeqReturn = BUFREQ_E_NOT_OK;
    }
    return bufQeqReturn;
}
#endif/*STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT*/
/******************************************************************************/
/*
 * Brief               Rx Tp Pdu gateway to only one Tp Module Pdu handle,
 *                     when copy Rx Data.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SrcPduId,DestPduId,info,BufferSizePtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == PDUR_TP_COPYRXDATA)
FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyRxDataToOneTpHandle(
        #if (0u < PDUR_TP_BUFFER_SUM)
        PduIdType SrcPduId,
        #endif
        PduIdType DestPduId,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
        P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    #if(PDUR_TX_BUFFER_SUM > 0u)
    uint16 pduRDestTxBuffeId;
    PduLengthType rxOffset;
    #endif
    #if (0u < PDUR_TP_BUFFER_SUM)
    uint8 tpBufferId;
    PduIdType gatewayTpRunTimeId;
    uint16 pduRTpThreshold;
    #endif
    #if(PDUR_TX_BUFFER_SUM > 0u)
    pduRDestTxBuffeId = PDUR_DESTPDU_CFG(DestPduId).PduRDestTxBufferRef;
    /*the tp data is stored in Dedicated buffer(SF)*/
    if((PDUR_UNUSED_UINT16 != pduRDestTxBuffeId)
            && (TRUE == PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].used))
    {
        rxOffset = PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].SduLength;
        if((rxOffset + info->SduLength)
                <= PduR_TxBuffer[pduRDestTxBuffeId].PduRPduMaxLength)
        {
            PduR_Memcpy(
                &(PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].TxBufferData[rxOffset]),
                info->SduDataPtr,
                info->SduLength);
            PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].SduLength += info->SduLength;
            *BufferSizePtr = PduR_TxBuffer[pduRDestTxBuffeId].PduRPduMaxLength - (rxOffset + info->SduLength);
            bufQeqReturn = BUFREQ_OK;
        }
    }
    /*the tp data is stored in Tp buffer(FF and CF)*/
    else
    #endif/*PDUR_TX_BUFFER_SUM > 0u*/
    {
        #if (0u < PDUR_TP_BUFFER_SUM)
        /*find the valid tp buffer to copy the data*/
        tpBufferId = PduR_FindTpBuffer(SrcPduId);
        if(PDUR_UNUSED_UINT8 != tpBufferId)
        {
            /*copy the rx pdu data to the tp buffer*/
            bufQeqReturn = PduR_CopyRxTpDataToTpBuffer(tpBufferId,info,BufferSizePtr);
            if((BUFREQ_OK == bufQeqReturn)
                    && (TRUE == PDUR_ROUTINGPATH_CFG(SrcPduId).GatewayOnTheFly))
            {
                pduRTpThreshold = PDUR_DESTPDU_CFG(DestPduId).PduRTpThreshold;
                gatewayTpRunTimeId = PDUR_DESTPDU_CFG(DestPduId).GateWayTpRunTimeIndex;
                if((TRUE == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly)
                        && (PduR_TpBuffer[tpBufferId].RxBufferOffset >= pduRTpThreshold))
                {
                    /*gateway MF pdu to Tp mode by LoTp_Transmit*/
                    if(E_NOT_OK == PduR_GateWayMFToTp(DestPduId))
                    {
                        bufQeqReturn = BUFREQ_E_NOT_OK;
                        PduR_ClearBufferAndStateOfGateWayTpPdu(SrcPduId);
                    }
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly = FALSE;
                }
            }
        }
        #endif/*0u < PDUR_TP_BUFFER_SUM*/
    }
    return bufQeqReturn;
}
#endif/*STD_ON == PDUR_TP_COPYRXDATA*/
/******************************************************************************/
/*
 * Brief               Rx Tp Pdu gateway to only one Tp Module Pdu handle,
 *                     when Rx Indication.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SrcPduId,DestPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == PDUR_TP_RXINDICATION)
FUNC(void, PDUR_CODE)
PduR_RxIndicationToOneTpHandle(PduIdType SrcPduId,PduIdType DestPduId)
{
    PduIdType gatewayTpRunTimeId;
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(DestPduId).GateWayTpRunTimeIndex;
    /*pdu data stored in dedicated buffer(SF)*/
    if(PDUR_UNUSED_UINT8 == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId)
    {
        #if(PDUR_TX_BUFFER_SUM > 0u)
        /*gateway SF pdu to Tp mode by LoTp_Transmit*/
        if(E_NOT_OK == PduR_GateWaySFToTp(DestPduId))
        {
            PduR_ClearBufferAndStateOfGateWayTpPdu(SrcPduId);
        }
        #endif
    }
    #if (0u < PDUR_TP_BUFFER_SUM)
    /*pdu data stored in tp buffer*/
    else
    {
        /*gateway-on-the-fly mode,but the PduRTpThreshold is large than the pdu length*/
        if(TRUE == PDUR_ROUTINGPATH_CFG(SrcPduId).GatewayOnTheFly)
        {
            gatewayTpRunTimeId = PDUR_DESTPDU_CFG(DestPduId).GateWayTpRunTimeIndex;
            if(TRUE == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly)
            {
                /*gateway MF pdu to Tp mode by LoTp_Transmit*/
                if(E_OK == PduR_GateWayMFToTp(DestPduId))
                {
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly = FALSE;
                }
                else
                {
                    PduR_ClearBufferAndStateOfGateWayTpPdu(SrcPduId);
                }
            }
        }
        /*Receive all pdu data,then gateway to other tp module*/
        else
        {
            /*gateway MF pdu to Tp mode by LoTp_Transmit*/
            if(E_NOT_OK == PduR_GateWayMFToTp(DestPduId))
            {
                PduR_ClearBufferAndStateOfGateWayTpPdu(SrcPduId);
            }
        }
    }
    #endif/*0u < PDUR_TP_BUFFER_SUM*/
    return;
}
#endif/*STD_ON == PDUR_TP_RXINDICATION*/
/******************************************************************************/
/*
 * Brief               one Tp Pdu route to one Tp Pdu,the dest pdu copy tx
 *                     data handle.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DestPduId,info,retry,availableDataPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == PDUR_TP_COPYTXDATA)
FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_OneDestCopyTxDataFromTpHandle
(
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    PduIdType gatewayTpRunTimeId;
    uint8 tpBufferId;
    #if(PDUR_TP_BUFFER_SUM > 0u)
    PduIdType srcPduId;
    srcPduId = PDUR_DESTPDU_CFG(DestPduId).PduRSrcPduRef;
    #endif
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(DestPduId).GateWayTpRunTimeIndex;
    if(FALSE == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation)
    {
        tpBufferId = PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId;
        /*Gateway Single-Frame I-PDU*/
        #if(PDUR_TX_BUFFER_SUM > 0u)
        if(PDUR_UNUSED_UINT8 == tpBufferId)
        {
            bufQeqReturn = PduR_CopyTxDataFromTxBuffer(DestPduId,info,retry,availableDataPtr);
        }
        /*Gateway Multi-Frame I-PDU*/
        else
        #endif/*PDUR_TX_BUFFER_SUM > 0u*/
        {
            #if(PDUR_TP_BUFFER_SUM > 0u)
            #if(STD_ON == PDUR_META_DATA_SUPPORT)
            if((0u < PDUR_DESTPDU_CFG(DestPduId).MetaDataLength)
                && (NULL_PTR != info->MetaDataPtr))
            {
                PduR_Memcpy
                (
                    info->MetaDataPtr,
                    PduR_TpBuffer[tpBufferId].MetaData,
                    (PduLengthType)PDUR_DESTPDU_CFG(DestPduId).MetaDataLength
                );
            }
            #endif/*STD_ON == PDUR_META_DATA_SUPPORT*/
            /*gateway-on-the-fly mode*/
            if(TRUE == PDUR_ROUTINGPATH_CFG(srcPduId).GatewayOnTheFly)
            {
                if((NULL_PTR == retry) || (TP_DATACONF == retry->TpDataState))
                {
                    bufQeqReturn = PduR_CopyTxDataFromTpBufferFly(DestPduId,info,availableDataPtr);
                }
                else
                {
                    bufQeqReturn = BUFREQ_E_NOT_OK;
                }
            }
            /*not gateway-on-the-fly mode*/
            else
            {
                if((NULL_PTR != retry) && (TP_DATARETRY == retry->TpDataState))
                {
                    if(PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset >= retry->TxTpDataCnt)
                    {
                        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset -= retry->TxTpDataCnt;
                    }
                    /*will not happen general,TP module guarantee*/
                    else
                    {
                        /*re-copy data from 0*/
                        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset = 0u;
                    }
                }
                /*don't copy data,just update the available data length and tell Lo Tp Module */
                if(0u == info->SduLength)
                {
                    *availableDataPtr = PduR_TpBuffer[tpBufferId].RxBufferOffset
                            - PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset;
                    bufQeqReturn = BUFREQ_OK;
                }
                /*copy data and tell Lo Tp Module the available data length*/
                else
                {
                    if((PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset + info->SduLength)
                            <= PduR_TpBuffer[tpBufferId].RxBufferOffset)
                    {
                        PduR_Memcpy
                        (
                            info->SduDataPtr,
                            &(PduR_TpBuffer[tpBufferId].TpBufferData[PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset]),
                            info->SduLength
                        );
                        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset += info->SduLength;
                        *availableDataPtr = PduR_TpBuffer[tpBufferId].RxBufferOffset
                                - PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset;
                        bufQeqReturn = BUFREQ_OK;
                    }
                    /*will not happen general,TP module guarantee*/
                    else
                    {
                        bufQeqReturn = BUFREQ_E_NOT_OK;
                    }
                }
            }
            #endif/*PDUR_TP_BUFFER_SUM > 0u*/
        }
    }
    return bufQeqReturn;
}
#endif/*STD_ON == PDUR_TP_COPYTXDATA*/
/******************************************************************************/
/*
 * Brief               clear buffer and gateway state for gateway tp pdu.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduRSrcPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT) || (STD_ON == PDUR_TP_RXINDICATION) || (STD_ON == PDUR_TP_TXCONFIRMATION))
FUNC(void, PDUR_CODE)
PduR_ClearBufferAndStateOfGateWayTpPdu(PduIdType PduRSrcPduId)
{
    #if (0u < PDUR_TP_BUFFER_SUM)
    uint8 cnt;
    #endif
    PduIdType pduRDestPduId;
    #if(PDUR_TX_BUFFER_SUM > 0u)
    uint16 pduRDestTxBuffeIndex;
    #endif
    PduIdType gatewayTpRunTimeId;
    if(1u == PDUR_ROUTINGPATH_CFG(PduRSrcPduId).PduDestSum)
    {
        pduRDestPduId = PDUR_ROUTINGPATH_CFG(PduRSrcPduId).PduRDestPduIdRef[0u];
        #if(PDUR_TX_BUFFER_SUM > 0u)
        pduRDestTxBuffeIndex = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestTxBufferRef;
        /*clear dedicated buffer*/
        if((PDUR_UNUSED_UINT16 != pduRDestTxBuffeIndex)
            && (TRUE == PduR_TxBuffer[pduRDestTxBuffeIndex].PduRTxBufferRef[0u].used))
        {
            PduR_TxBuffer[pduRDestTxBuffeIndex].PduRTxBufferRef[0u].SduLength = 0u;
            PduR_TxBuffer[pduRDestTxBuffeIndex].PduRTxBufferRef[0u].used = FALSE;
        }
        else
        #endif/*PDUR_TX_BUFFER_SUM > 0u*/
        {
            #if (0u < PDUR_TP_BUFFER_SUM)
            /*clear tp buffer*/
            for(cnt = 0u;cnt < PDUR_TP_BUFFER_SUM;cnt++)
            {
                if((TRUE == PduR_TpBuffer[cnt].used) && (PduRSrcPduId == PduR_TpBuffer[cnt].PduHandleId))
                {
                    PduR_TpBuffer[cnt].NextTpBufferId = PDUR_UNUSED_UINT8;
                    PduR_TpBuffer[cnt].PduHandleId = PDUR_UNUSED_UINT16;
                    PduR_TpBuffer[cnt].RxBufferOffset = 0u;
                    PduR_TpBuffer[cnt].SduLength = 0u;
                    PduR_TpBuffer[cnt].used = FALSE;
                }
            }
            #endif/*0u < PDUR_TP_BUFFER_SUM*/
        }
        gatewayTpRunTimeId = PDUR_DESTPDU_CFG(pduRDestPduId).GateWayTpRunTimeIndex;
        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId = PDUR_UNUSED_UINT8;
        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation = TRUE;
        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset = 0u;
        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].NeedGateWayOnTheFly = FALSE;
    }
    return;
}
#endif/*(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT) || (STD_ON == PDUR_TP_RXINDICATION) || (STD_ON == PDUR_TP_TXCONFIRMATION)*/
#endif/*0u < PDUR_DEST_GATEWAY_TP_PDU_SUM*/
/******************************************************************************/
/*
 * Brief               copy data from source to dest.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      dest,source,length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if((PDUR_TP_BUFFER_SUM > 0u) || (PDUR_TX_BUFFER_SUM > 0u))
FUNC(void, PDUR_CODE)
PduR_Memcpy
(
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) dest,
    P2CONST(uint8, AUTOMATIC, AUTOMATIC) source,
    PduLengthType length
)
{
    PduLengthType dataLength;
    dataLength = length;
    while(dataLength > 0u)
    {
        *dest = *source;
        dest++;
        source++;
        dataLength--;
    }
    return;
}
#endif/*(PDUR_TP_BUFFER_SUM > 0u) || (PDUR_TX_BUFFER_SUM > 0u)*/
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if(STD_ON == PDUR_TP_COPYTXDATA)
/******************************************************************************/
/*
 * Brief               copy the tx data from the tp buffer gateway on the fly.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DestPduId,info
 * Param-Name[out]     availableDataPtr
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
#if(PDUR_TP_BUFFER_SUM > 0u)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyTxDataFromTpBufferFly(
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
    BufReq_ReturnType bufQeqReturn;
    uint8 tpBufferId;
    uint8 nextTpBufferId;
    PduLengthType nextTpBufferNeedCopyLength;
    PduIdType gatewayTpRunTimeId;
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(DestPduId).GateWayTpRunTimeIndex;
    tpBufferId = PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId;
    nextTpBufferId = PduR_TpBuffer[tpBufferId].NextTpBufferId;
    /*the active tp buffer have stored full and other tp buffer have start store Rx Tp Pdu data*/
    if(PDUR_UNUSED_UINT8 != nextTpBufferId)
    {
        /*don't copy data,just update the available data length and tell Lo Tp Module */
        if(0u == info->SduLength)
        {
            *availableDataPtr = (PduR_TpBuffer[tpBufferId].RxBufferOffset
                    + PduR_TpBuffer[nextTpBufferId].RxBufferOffset
                    - PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset);
            bufQeqReturn = BUFREQ_OK;
        }
        /*copy data and tell Lo Tp Module the available data length*/
        else
        {
            /*the active buffer have not copy completely*/
            if((PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset + info->SduLength)
                    <= PduR_TpBuffer[tpBufferId].RxBufferOffset)
            {
                PduR_Memcpy
                (
                    info->SduDataPtr,
                    &PduR_TpBuffer[tpBufferId].TpBufferData[PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset],
                    info->SduLength
                );
                PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset += info->SduLength;
                *availableDataPtr = (PduR_TpBuffer[tpBufferId].RxBufferOffset
                        + PduR_TpBuffer[nextTpBufferId].RxBufferOffset
                        - PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset);
                bufQeqReturn = BUFREQ_OK;
            }
            /*the active buffer have copy completely,and next tp buffer also copy some data*/
            else
            {
                nextTpBufferNeedCopyLength = (PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset
                        + info->SduLength
                        - PduR_TpBuffer[tpBufferId].RxBufferOffset);
                /*don't have enough rx valid data for copy*/
                if(nextTpBufferNeedCopyLength > PduR_TpBuffer[nextTpBufferId].RxBufferOffset)
                {
                    bufQeqReturn = BUFREQ_E_BUSY;
                }
                /*have enough rx valid data for copy*/
                else
                {
                    PduR_Memcpy
                    (
                        info->SduDataPtr,
                        &(PduR_TpBuffer[tpBufferId].TpBufferData[PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset]),
                        (info->SduLength - nextTpBufferNeedCopyLength)
                    );
                    PduR_Memcpy
                    (
                        &(info->SduDataPtr[info->SduLength - nextTpBufferNeedCopyLength]),
                        &(PduR_TpBuffer[nextTpBufferId].TpBufferData[0u]),
                        nextTpBufferNeedCopyLength
                    );
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset = nextTpBufferNeedCopyLength;
                    PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId = nextTpBufferId;
                    *availableDataPtr = PduR_TpBuffer[nextTpBufferId].RxBufferOffset - nextTpBufferNeedCopyLength;
                    bufQeqReturn = BUFREQ_OK;
                    /*clear the tp buffer which have been copyed all data*/
                    PduR_TpBuffer[tpBufferId].NextTpBufferId = PDUR_UNUSED_UINT8;
                    PduR_TpBuffer[tpBufferId].PduHandleId = PDUR_UNUSED_UINT16;
                    PduR_TpBuffer[tpBufferId].RxBufferOffset = 0u;
                    PduR_TpBuffer[tpBufferId].SduLength = 0u;
                    PduR_TpBuffer[tpBufferId].used = FALSE;
                }
            }
        }
    }
    /*the active tp buffer have stored not full*/
    else
    {
        /*don't copy data,just update the available data length and tell Lo Tp Module */
        if(0u == info->SduLength)
        {
            *availableDataPtr = PduR_TpBuffer[tpBufferId].RxBufferOffset
                    - PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset;
            bufQeqReturn = BUFREQ_OK;
        }
        /*copy data and tell Lo Tp Module the available data length*/
        else
        {
            if((PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset + info->SduLength)
                    <= PduR_TpBuffer[tpBufferId].RxBufferOffset)
            {
                PduR_Memcpy
                (
                    info->SduDataPtr,
                    &(PduR_TpBuffer[tpBufferId].TpBufferData[PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset]),
                    info->SduLength
                );
                PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset += info->SduLength;
                *availableDataPtr = PduR_TpBuffer[tpBufferId].RxBufferOffset
                        - PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset;
                bufQeqReturn = BUFREQ_OK;
            }
            else
            {
                bufQeqReturn = BUFREQ_E_BUSY;
            }
        }
    }
    return bufQeqReturn;
}
#endif/*PDUR_TP_BUFFER_SUM > 0u*/
/******************************************************************************/
/*
 * Brief               copy the tx data from the tx buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DestPduId,info,retry
 * Param-Name[out]     availableDataPtr
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
#if(PDUR_TX_BUFFER_SUM > 0u)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyTxDataFromTxBuffer(
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
    BufReq_ReturnType bufQeqReturn;
    PduIdType gatewayTpRunTimeId;
    uint16 pduRDestTxBuffeId;
    PduLengthType copyOffset;
    PduLengthType pduTotalLength;
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(DestPduId).GateWayTpRunTimeIndex;
    if((NULL_PTR != retry) && (TP_DATARETRY == retry->TpDataState))
    {
        if(PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset >= retry->TxTpDataCnt)
        {
            PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset -= retry->TxTpDataCnt;
        }
        /*will not happen general,TP module guarantee*/
        else
        {
            /*re-copy data from 0*/
            PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset = 0u;
        }
    }
    copyOffset = PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset;
    pduRDestTxBuffeId = PDUR_DESTPDU_CFG(DestPduId).PduRDestTxBufferRef;
    pduTotalLength = PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].SduLength;
    if(((copyOffset + info->SduLength) <= pduTotalLength)
        && (TRUE == PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].used))
    {
        PduR_Memcpy
        (
            info->SduDataPtr,
            &(PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].TxBufferData[copyOffset]),
            info->SduLength
        );
        #if(STD_ON == PDUR_META_DATA_SUPPORT)
        if((0u < PDUR_DESTPDU_CFG(DestPduId).MetaDataLength)
            && (NULL_PTR != info->MetaDataPtr))
        {
            PduR_Memcpy(
                info->MetaDataPtr,
                PduR_TxBuffer[pduRDestTxBuffeId].PduRTxBufferRef[0u].MetaData,
                (PduLengthType)PDUR_DESTPDU_CFG(DestPduId).MetaDataLength);
        }
        #endif/*STD_ON == PDUR_META_DATA_SUPPORT*/
        *availableDataPtr = (pduTotalLength - copyOffset - info->SduLength);
        PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TxBufferOffset += info->SduLength;
        bufQeqReturn = BUFREQ_OK;
    }
    else
    {
        bufQeqReturn = BUFREQ_E_NOT_OK;
    }
    return bufQeqReturn;
}
#endif/*PDUR_TX_BUFFER_SUM > 0u*/
#endif/*STD_ON == PDUR_TP_COPYTXDATA*/
/******************************************************************************/
/*
 * Brief               copy the rx pdu data to the tp buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TpBufferId,Info
 * Param-Name[out]     BufferSizePtr
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType
 */
/******************************************************************************/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyRxTpDataToTpBuffer(
    uint8 TpBufferId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) Info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr)
{
    BufReq_ReturnType bufQeqReturn;
    PduLengthType rxBufferOffset;
    uint8 nextValidTpBufferId;
    PduLengthType nextTpBufferStoreLength;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 metaDataLength;
    metaDataLength = PDUR_SRCEPDU_CFG(PduR_TpBuffer[TpBufferId].PduHandleId).MetaDataLength;
    #endif
    rxBufferOffset = PduR_TpBuffer[TpBufferId].RxBufferOffset;
    if((rxBufferOffset + Info->SduLength) < PduR_TpBuffer[TpBufferId].TpBufferLength)
    {
        PduR_Memcpy(
                &PduR_TpBuffer[TpBufferId].TpBufferData[rxBufferOffset],
                Info->SduDataPtr,
                Info->SduLength);
        PduR_TpBuffer[TpBufferId].RxBufferOffset += Info->SduLength;
        *BufferSizePtr = PduR_TpBuffer[TpBufferId].TpBufferLength
                - PduR_TpBuffer[TpBufferId].RxBufferOffset;
        bufQeqReturn = BUFREQ_OK;
    }
    /*need another tp buffer to store the pdu(used for gateway-on-the-fly)*/
    else if((rxBufferOffset + Info->SduLength) == PduR_TpBuffer[TpBufferId].TpBufferLength)
    {
        PduR_Memcpy(
                &PduR_TpBuffer[TpBufferId].TpBufferData[rxBufferOffset],
                Info->SduDataPtr,
                Info->SduLength);
        PduR_TpBuffer[TpBufferId].RxBufferOffset = PduR_TpBuffer[TpBufferId].TpBufferLength;
        /*find one unused tp buffer.if no unused tp buffer,return 0xff*/
        nextValidTpBufferId = PduR_FindNextValidTpBuffer();
        if(PDUR_UNUSED_UINT8 != nextValidTpBufferId)
        {
            PduR_TpBuffer[TpBufferId].NextTpBufferId = nextValidTpBufferId;
            PduR_TpBuffer[nextValidTpBufferId].PduHandleId = PduR_TpBuffer[TpBufferId].PduHandleId;
            PduR_TpBuffer[nextValidTpBufferId].RxBufferOffset = 0u;
            PduR_TpBuffer[nextValidTpBufferId].SduLength = PduR_TpBuffer[TpBufferId].SduLength;
            PduR_TpBuffer[nextValidTpBufferId].used = TRUE;
            #if(STD_ON == PDUR_META_DATA_SUPPORT)
            if(0u < metaDataLength)
            {
                PduR_Memcpy(
                        PduR_TpBuffer[nextValidTpBufferId].MetaData,
                        Info->MetaDataPtr,
                        (PduLengthType)metaDataLength);
            }
            #endif
            *BufferSizePtr = PduR_TpBuffer[nextValidTpBufferId].TpBufferLength;
        }
        else
        {
            *BufferSizePtr = 0u;
        }
        bufQeqReturn = BUFREQ_OK;
    }
    /*need another tp buffer to store the pdu(used for gateway-on-the-fly)*/
    else
    {
        /*find one unused tp buffer.if no unused tp buffer,return 0xff*/
        nextValidTpBufferId = PduR_FindNextValidTpBuffer();
        if(PDUR_UNUSED_UINT8 != nextValidTpBufferId)
        {
            nextTpBufferStoreLength =
                    (Info->SduLength - (PduR_TpBuffer[TpBufferId].TpBufferLength - rxBufferOffset));
            if(nextTpBufferStoreLength <= PduR_TpBuffer[nextValidTpBufferId].TpBufferLength)
            {
                PduR_Memcpy(
                        &PduR_TpBuffer[TpBufferId].TpBufferData[rxBufferOffset],
                        Info->SduDataPtr,
                        Info->SduLength - nextTpBufferStoreLength);
                PduR_TpBuffer[TpBufferId].RxBufferOffset = PduR_TpBuffer[TpBufferId].TpBufferLength;
                PduR_TpBuffer[TpBufferId].NextTpBufferId = nextValidTpBufferId;
                PduR_Memcpy(
                        &PduR_TpBuffer[nextValidTpBufferId].TpBufferData[0u],
                        &Info->SduDataPtr[Info->SduLength - nextTpBufferStoreLength],
                        nextTpBufferStoreLength);
                PduR_TpBuffer[nextValidTpBufferId].RxBufferOffset = nextTpBufferStoreLength;
                PduR_TpBuffer[nextValidTpBufferId].PduHandleId = PduR_TpBuffer[TpBufferId].PduHandleId;
                PduR_TpBuffer[nextValidTpBufferId].SduLength = PduR_TpBuffer[TpBufferId].SduLength;
                PduR_TpBuffer[nextValidTpBufferId].used = TRUE;
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                if(0u < metaDataLength)
                {
                    PduR_Memcpy(
                            PduR_TpBuffer[nextValidTpBufferId].MetaData,
                            Info->MetaDataPtr,
                            (PduLengthType)metaDataLength);
                }
                #endif
                *BufferSizePtr = PduR_TpBuffer[nextValidTpBufferId].TpBufferLength
                        - nextTpBufferStoreLength;
                bufQeqReturn = BUFREQ_OK;
            }
            else
            {
                bufQeqReturn = BUFREQ_E_NOT_OK;
            }
        }
        else
        {
            bufQeqReturn = BUFREQ_E_NOT_OK;
        }
    }
    return bufQeqReturn;
}
#endif/*0u < PDUR_TP_BUFFER_SUM*/
/******************************************************************************/
/*
 * Brief               find one unused tp buffer.
 *                     if no unused tp buffer,return 0xff.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(uint8, PDUR_CODE)
PduR_FindNextValidTpBuffer(void)
{
    uint8 findTpBuffer = PDUR_UNUSED_UINT8;
    uint8 tpBufferIndex;
    for(tpBufferIndex = 0u;
            (tpBufferIndex < PDUR_TP_BUFFER_SUM)&&(PDUR_UNUSED_UINT8 == findTpBuffer);
            tpBufferIndex++)
    {
        if(FALSE == PduR_TpBuffer[tpBufferIndex].used)
        {
            findTpBuffer = tpBufferIndex;
        }
    }
    return findTpBuffer;
}
#endif/*PDUR_TP_BUFFER_SUM*/
/******************************************************************************/
/*
 * Brief               gateway MF pdu to Tp mode by LoTp_Transmit.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduRDestPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_GateWayMFToTp(PduIdType PduRDestPduId)
{
    Std_ReturnType loTpTxReturn = E_NOT_OK;
    PduIdType gatewayTpRunTimeId;
    uint8 destModuleIndex;
    uint8 destModule;
    PduIdType loPduId;
    uint8 tpBufferId;
    PduInfoType pduRInfo;
    destModuleIndex = PDUR_DESTPDU_CFG(PduRDestPduId).BswModuleIndex;
    destModule = PduR_BswModuleConfigData[destModuleIndex].PduRBswModuleRef;
    loPduId = PDUR_DESTPDU_CFG(PduRDestPduId).PduRDestModulePduIndex;
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(PduRDestPduId).GateWayTpRunTimeIndex;
    tpBufferId = PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].ActiveTpBufferId;
    if(FALSE == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation)
    {
        pduRInfo.SduLength = PduR_TpBuffer[tpBufferId].SduLength;
        #if(STD_ON == PDUR_META_DATA_SUPPORT)
        if(PDUR_DESTPDU_CFG(PduRDestPduId).MetaDataLength > 0u)
        {
            pduRInfo.MetaDataPtr = PduR_TpBuffer[tpBufferId].MetaData;
        }
        else
        #endif
        {
            pduRInfo.MetaDataPtr = NULL_PTR;
        }
        pduRInfo.SduDataPtr = NULL_PTR;
        switch(destModule)
        {
            #if(STD_ON == PDUR_CANTP_SUPPORT)
            case PDUR_CANTP:
                loTpTxReturn = CanTp_Transmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_J1939TP_SUPPORT)
            case PDUR_J1939TP:
                loTpTxReturn = J1939Tp_Transmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_LINTP_SUPPORT)
            case PDUR_LINIF:
                loTpTxReturn = LinTp_Transmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_DOIP_SUPPORT)
            case PDUR_DOIP:
                loTpTxReturn = DoIP_TpTransmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_SOAD_SUPPORT)
            case PDUR_SOAD:
                loTpTxReturn = SoAd_TpTransmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_SECOC_SUPPORT)
            case PDUR_SECOC:
                loTpTxReturn = SecOC_TpTransmit(loPduId,&pduRInfo);
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    return loTpTxReturn;
}
#endif/*0u < PDUR_TP_BUFFER_SUM*/
/******************************************************************************/
/*
 * Brief               gateway SF pdu to Tp mode by LoTp_Transmit.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduRDestPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
#if((STD_ON == PDUR_TP_RXINDICATION) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_GateWaySFToTp(PduIdType PduRDestPduId)
{
    Std_ReturnType loTpTxReturn = E_NOT_OK;
    PduIdType gatewayTpRunTimeId;
    uint8 destModuleIndex;
    uint8 destModule;
    PduIdType loPduId;
    uint16 txBufferId;
    PduInfoType pduRInfo;
    destModuleIndex = PDUR_DESTPDU_CFG(PduRDestPduId).BswModuleIndex;
    destModule = PduR_BswModuleConfigData[destModuleIndex].PduRBswModuleRef;
    loPduId = PDUR_DESTPDU_CFG(PduRDestPduId).PduRDestModulePduIndex;
    txBufferId = PDUR_DESTPDU_CFG(PduRDestPduId).PduRDestTxBufferRef;
    gatewayTpRunTimeId = PDUR_DESTPDU_CFG(PduRDestPduId).GateWayTpRunTimeIndex;
    if(FALSE == PduR_GateWayDestTpRTSate[gatewayTpRunTimeId].TpTxConfirmation)
    {
        pduRInfo.SduLength = PduR_TxBuffer[txBufferId].PduRTxBufferRef[0u].SduLength;
        #if(STD_ON == PDUR_META_DATA_SUPPORT)
        if(PDUR_DESTPDU_CFG(PduRDestPduId).MetaDataLength > 0u)
        {
            pduRInfo.MetaDataPtr = PduR_TxBuffer[txBufferId].PduRTxBufferRef[0u].MetaData;
        }
        else
        #endif
        {
            pduRInfo.MetaDataPtr = NULL_PTR;
        }
        pduRInfo.SduDataPtr = NULL_PTR;
        switch(destModule)
        {
            #if(STD_ON == PDUR_CANTP_SUPPORT)
            case PDUR_CANTP:
                loTpTxReturn = CanTp_Transmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_J1939TP_SUPPORT)
            case PDUR_J1939TP:
                loTpTxReturn = J1939Tp_Transmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_LINTP_SUPPORT)
            case PDUR_LINIF:
                loTpTxReturn = LinTp_Transmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_DOIP_SUPPORT)
            case PDUR_DOIP:
                loTpTxReturn = DoIP_TpTransmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_SOAD_SUPPORT)
            case PDUR_SOAD:
                loTpTxReturn = SoAd_TpTransmit(loPduId,&pduRInfo);
                break;
            #endif
            #if(STD_ON == PDUR_SECOC_SUPPORT)
            case PDUR_SECOC:
                loTpTxReturn = SecOC_TpTransmit(loPduId,&pduRInfo);
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    return loTpTxReturn;
}
#endif/*(STD_ON == PDUR_TP_RXINDICATION) && (PDUR_TX_BUFFER_SUM > 0u)*/
/******************************************************************************/
/*
 * Brief               find one valid tp buffer.if not find,return 0xff.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduRSrcPduId,PduLength
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
#if (0u < PDUR_TP_BUFFER_SUM)
static FUNC(uint8, PDUR_CODE)
PduR_FindValidTpBuffer(uint16 PduRSrcPduId,PduLengthType PduLength)
{
    uint8 findTpBuffer = PDUR_UNUSED_UINT8;
    uint8 tpBufferIndex;
    boolean rxPduGateWayNow = FALSE;
    for(tpBufferIndex = 0u;(tpBufferIndex < PDUR_TP_BUFFER_SUM)&&(FALSE == rxPduGateWayNow);tpBufferIndex++)
    {
        if((PduR_TpBuffer[tpBufferIndex].PduHandleId == PduRSrcPduId)
                && (TRUE == PduR_TpBuffer[tpBufferIndex].used))
        {
            rxPduGateWayNow = TRUE;
        }
    }
    if(FALSE == rxPduGateWayNow)
    {
        for(tpBufferIndex = 0u;tpBufferIndex < PDUR_TP_BUFFER_SUM;tpBufferIndex++)
        {
            if((FALSE == PduR_TpBuffer[tpBufferIndex].used)
                    && (PduLength <= PduR_TpBuffer[tpBufferIndex].TpBufferLength))
            {
                if(PDUR_UNUSED_UINT8 == findTpBuffer)
                {
                    findTpBuffer = tpBufferIndex;
                }
                else
                {
                    if(PduR_TpBuffer[findTpBuffer].TpBufferLength > PduR_TpBuffer[tpBufferIndex].TpBufferLength)
                    {
                        findTpBuffer = tpBufferIndex;
                    }
                }
            }
        }
        if((TRUE == PDUR_ROUTINGPATH_CFG(PduRSrcPduId).GatewayOnTheFly)
                && (PDUR_UNUSED_UINT8 == findTpBuffer))
        {
            for(tpBufferIndex = 0u;(tpBufferIndex < PDUR_TP_BUFFER_SUM)&&(PDUR_UNUSED_UINT8 == findTpBuffer);tpBufferIndex++)
            {
                if((PDUR_ROUTINGPATH_CFG(PduRSrcPduId).PduRTpMaxThreshold <= PduR_TpBuffer[tpBufferIndex].TpBufferLength)
                        && (FALSE == PduR_TpBuffer[tpBufferIndex].used))
                {
                    findTpBuffer = tpBufferIndex;
                }
            }
        }
    }
    return findTpBuffer;
}
/******************************************************************************/
/*
 * Brief               find the rx tp pdu active store tp buffer.
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduRSrcPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
static FUNC(uint8, PDUR_CODE)
PduR_FindTpBuffer(uint16 PduRSrcPduId)
{
    uint8 tpBufferId = PDUR_UNUSED_UINT8;
    uint8 cnt;
    for(cnt = 0u;(cnt < PDUR_TP_BUFFER_SUM)&&(PDUR_UNUSED_UINT8 == tpBufferId);cnt++)
    {
        if((TRUE == PduR_TpBuffer[cnt].used) && (PduRSrcPduId == PduR_TpBuffer[cnt].PduHandleId))
        {
            if(PDUR_UNUSED_UINT8 == PduR_TpBuffer[cnt].NextTpBufferId)
            {
                tpBufferId = cnt;
            }
        }
    }
    return tpBufferId;
}
#endif/*0u < PDUR_TP_BUFFER_SUM*/

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif/*0u < PDUR_DEST_GATEWAY_TP_PDU_SUM*/

#endif/*STD_OFF == PDUR_ZERO_COST_OPERATION*/


