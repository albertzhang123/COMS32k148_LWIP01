/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2020)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    : CanTp_RX.c                                                 **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Rx process                                                 **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*=======[I N C L U D E S]====================================================*/
#include "CanTp.h"
#include "CanTp_Internal.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/*Construct Flow Control frame according to the parameter.*/
static FUNC(PduLengthType, CANTP_CODE)
CanTp_ConstructFCPci(
	    uint8 ChannelIdx,
	    CanTp_FCInfoType CanTp_FC
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)))
	    ,
	    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Metadata
#endif
	    );

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
/*Construct Rx meta data*/
FUNC(void, CANTP_CODE)CanTp_ConstructForwardRxMetaData(
	uint8 channelId,
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_VAR) CanTpRxInfoPtr);
#endif

/*Compare Buffer With Bs*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CompareBufferWithBs(uint8 ChannelIdx);

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*Construct FC meta data*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructFCMetaData(
		uint8 channelId,
		P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr
	#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
		,
	    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Metadata
	#endif
		);
#endif

/*Calculate the BS value according to the buffer length and remain data length.*/
static FUNC(void, CANTP_CODE)
CanTp_CalcBS(uint8 ChannelIdx);

/*Start a block by sending FC_CTS if needed.*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_RxBlockStart(uint8 ChannelIdx);

#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               Get RX-SDU configuration by received NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2VAR(CanTp_FramePCIType, AUTOMATIC, AUTOMATIC) SduFrameType,
	P2VAR(uint8, AUTOMATIC, AUTOMATIC) channelIdx
    )
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    P2CONST(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) channelPtr;
    uint8 rxNSduIdx;
    uint8 chnIdx;
    CanTp_FramePCIType frameType;

    for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CANTP_CHANNEL_NUMBER); chnIdx++)
    {
    	channelPtr = &CanTp_CfgData.CanTpChannel[chnIdx];
		for (rxNSduIdx = 0; (NULL_PTR == result) &&
		(rxNSduIdx < channelPtr->RxNSduNum); rxNSduIdx++)
		{
			switch (channelPtr->RxNSdus[rxNSduIdx].AddressingFormat)
			{
				case CANTP_STANDARD:
					frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] & CANTP_FTYPE_MASK;
					if ((channelPtr->RxNSdus[rxNSduIdx].RxNPduId == CanTpRxPduId)
						&& ((CANTP_FTYPE_CF == frameType)||(CANTP_FTYPE_SF == frameType)
						        ||(CANTP_FTYPE_FF == frameType)))
					{
						result = &channelPtr->RxNSdus[rxNSduIdx];
						*SduFrameType = frameType;
						*channelIdx = chnIdx;
					}
					break;
				#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
				case CANTP_EXTENDED:
					if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_EX)
					{
						/*check this RX-SDU, which is configured as extended addressing
						 * format, and the CAN data at least includes TA and PCIType*/
						/*check its TA and frame type*/
						frameType  = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_EX] &
						        CANTP_FTYPE_MASK;

						if ((channelPtr->RxNSdus[rxNSduIdx].RxNPduId == CanTpRxPduId)
							&& ((CANTP_FTYPE_CF == frameType)||(CANTP_FTYPE_SF == frameType)
							        ||(CANTP_FTYPE_FF == frameType)))
						{
							result = &channelPtr->RxNSdus[rxNSduIdx];
							*SduFrameType = frameType;
							*channelIdx = chnIdx;
						}
					}
					break;
				#endif/*ENDOF extended addressing mode*/
				#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
				case CANTP_NORMALFIXED:
					frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_NF] & CANTP_FTYPE_MASK;
					if ((channelPtr->RxNSdus[rxNSduIdx].RxNPduId == CanTpRxPduId)
						&& ((CANTP_FTYPE_CF == frameType)||(CANTP_FTYPE_SF == frameType)
						        ||(CANTP_FTYPE_FF == frameType)))
					{
						result = &channelPtr->RxNSdus[rxNSduIdx];
						*SduFrameType = frameType;
						*channelIdx = chnIdx;
					}
					break;
				#endif/*ENDOF normal fixed addressing mode*/
				#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || \
					(CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
				case CANTP_MIXED:
                case CANTP_MIXED29BIT:
					if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_MIX11)
					{
						/*check this RX-SDU, which is configured as mix11 addressing
						 * format, and the CAN data at least includes AE and PCIType*/
						/*check its TA and frame type*/
						frameType  = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_MIX11] &
						        CANTP_FTYPE_MASK;

						if ((channelPtr->RxNSdus[rxNSduIdx].RxNPduId == CanTpRxPduId)
							&& ((CANTP_FTYPE_CF == frameType)||(CANTP_FTYPE_SF == frameType)
							        ||(CANTP_FTYPE_FF == frameType)))
						{
							result = &channelPtr->RxNSdus[rxNSduIdx];
							*SduFrameType = frameType;
							*channelIdx = chnIdx;
						}
					}
					break;
				#endif/*ENDOF mix11 addressing mode*/
				default:
				    /*idle*/
					break;
			}/*END OF switch*/
       } /*END OF for loop*/
    } /*END OF for loop*/
    return result;
}
/*
 * Brief               Get Rx-SDU configuration by Tx-FC NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByTxFCNPdu(PduIdType CanTpTxPduId, uint8 *channelId)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    P2CONST(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) channelPtr;
    uint8 rxNSduIdx;
    uint8 chnIdx;

    for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CANTP_CHANNEL_NUMBER); chnIdx++)
    {
    	channelPtr = &CanTp_CfgData.CanTpChannel[chnIdx];
		for (rxNSduIdx = 0; (NULL_PTR == result) &&
		(rxNSduIdx < channelPtr->RxNSduNum); rxNSduIdx++)
		{
			if ((channelPtr->RxNSdus[rxNSduIdx].TxFcNPduId == CanTpTxPduId)
				&&((CANTP_PHYSICAL_RX == channelPtr->RxNSdus[rxNSduIdx].RxTaType)
					#if (STD_ON == CANTP_FD)
					||(CANTP_CANFD_PHYSICAL == channelPtr->RxNSdus[rxNSduIdx].RxTaType)
					#endif
					))
			{
				result = &channelPtr->RxNSdus[rxNSduIdx];
				*channelId = chnIdx;
			}
		}
    }
    return result;
}
/*
 * Brief               Handle received CF..
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleCFReception(uint8 ChannelIdx)
{
    uint8 cfSN = 0;
    uint8 chnIdx;
    PduIdType nSduId;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
	boolean finished = FALSE;
	PduInfoType pduInfo;
	P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
	PduIdType iPduId;
	uint32 N_Cr;
	PduLengthType position;
	Std_ReturnType rslt;
	BufReq_ReturnType bufRslt;

    nSduId = CanTp_Channels[ChannelIdx].RxConnection.NSduId;
    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId, &chnIdx);
    iPduId = rxNSduCfgPtr->RxIPduId;/*update local buffer information*/
    if ((CANTP_STANDARD == rxNSduCfgPtr->AddressingFormat) ||
            (CANTP_NORMALFIXED == rxNSduCfgPtr->AddressingFormat))
    {/*standard SDU*/
		cfSN = CanTp_Channels[ChannelIdx].RxConnection.LocalBuf[CANTP_PCI_OFFSET_STD] &
		        CANTP_CF_SN_MASK;
		CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_STD;
    }
#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
	else if ((CANTP_MIXED == rxNSduCfgPtr->AddressingFormat) ||
	        (CANTP_MIXED29BIT == rxNSduCfgPtr->AddressingFormat) ||
            (CANTP_EXTENDED == rxNSduCfgPtr->AddressingFormat))
	{/*extended SDU*/
		cfSN = CanTp_Channels[ChannelIdx].RxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX11] &
		        CANTP_CF_SN_MASK;
		CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_MIX11;
	}
#endif
	else
	{/*idle*/}
	CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen =
	        CanTp_Channels[ChannelIdx].RxConnection.DataLen -
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition;
    if (cfSN != CanTp_Channels[ChannelIdx].RxConnection.CurrentCfSn)
    { /*check SN value*//*wrong SN*//*release resource of channel*/
        CanTp_ReleaseRxChannel(ChannelIdx);
        PduR_CanTpRxIndication(iPduId, E_NOT_OK);
    }
    else
    { /*It is the expected CF*/
        CanTp_Channels[ChannelIdx].RxConnection.HandleType = CANTP_FTYPE_CF;
        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
        if (0xFFu > CanTp_Channels[ChannelIdx].RxConnection.HandledCfCount)
        {
            CanTp_Channels[ChannelIdx].RxConnection.HandledCfCount++;
        }/*consider rx padding*/
        if (CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen >
            CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining)
        {
            CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen =
                    CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining;
        }
        position = CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition;
        pduInfo.SduDataPtr = &(CanTp_Channels[ChannelIdx].RxConnection.LocalBuf[position]);
        pduInfo.SduLength = CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen;
        bufRslt = PduR_CanTpCopyRxData(iPduId, pduInfoPtr,
                &CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining);
        if (BUFREQ_E_NOT_OK == bufRslt)
        {/*release resource of channel*/
            CanTp_ReleaseRxChannel(ChannelIdx);
			PduR_CanTpRxIndication(iPduId, E_NOT_OK);
        }
        else
        {
        	CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining =
					CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining -
					CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen;
	        if (((CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen < (8u - CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition))
	        		&& (0u != CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining))
#if (STD_ON == CANTP_FD)
	            || ((CanTp_Channels[ChannelIdx].RxConnection.DataLen !=
                        CanTp_Channels[ChannelIdx].RxConnection.FDDlc) &&
	                    (0u != CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining))
#endif
	        )
	        {/*release resource of channel*/
	            CanTp_ReleaseRxChannel(ChannelIdx);
				PduR_CanTpRxIndication(iPduId, E_NOT_OK);
	        }
			if (0u == CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining)
			{/*the whole SDU reception finished*//*release resource of channel*/
		        CanTp_ReleaseRxChannel(ChannelIdx);
#if (STD_ON == CANTP_FD)
                if (FALSE == CanTp_checkLastCF(
                        (CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen),
                        CanTp_MatchDLC(CanTp_Channels[ChannelIdx].RxConnection.DataLen),
                        (CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition - 1u)))
                {
                    PduR_CanTpRxIndication(iPduId, E_NOT_OK);
                }
                else
#endif
                {
                    PduR_CanTpRxIndication(iPduId, E_OK);
                }
				finished = TRUE;
			}
			else /*SDU whole reception not finished*/
			{
                if (CanTp_Channels[ChannelIdx].RxConnection.CurrentBs ==
                        CanTp_Channels[ChannelIdx].RxConnection.HandledCfCount)
                {/*current block finished, check upper buffer is enough or not*/
                    rslt = CanTp_CompareBufferWithBs(ChannelIdx);
                    if (E_NOT_OK == rslt)
                    {/*the upper buffer is not enough to store the next block,
                         * so need to get more buffer*/
                        CanTp_Channels[ChannelIdx].RxConnection.Substate =CANTP_RX_LARGE_START;
                        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer =
                                CANTP_NB;
                        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime =
                                rxNSduCfgPtr->Nbr;
                        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].
                                RxConnection.ChannelTimer.StartTime));
                    }
                    else
                    {/*buffer enough, start a new block*//*calculate block size*/
                        CanTp_CalcBS(ChannelIdx);
                        rslt = CanTp_RxBlockStart(ChannelIdx);
                        if (E_NOT_OK == rslt)
                        {
                            CanTp_ReleaseRxChannel(ChannelIdx);
                            PduR_CanTpRxIndication(iPduId, E_NOT_OK);
                        }
                    }
                }
                else
                {/*continue to wait another CF*/
                    CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_LARGE_V4CF;
                    if (rxNSduCfgPtr->Ncr != NULL_PTR)
                    {
                        N_Cr = *(rxNSduCfgPtr->Ncr);
                        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer =
                                CANTP_NC;
                        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime = N_Cr;
                        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].
                                RxConnection.ChannelTimer.StartTime));
                    }
                    else
                    {
                        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer =
                                CANTP_TIMER_NONE;
                    }
                }
                /*update SN,expect next CF*/
                CanTp_Channels[ChannelIdx].RxConnection.CurrentCfSn =
                        (uint8)(CanTp_Channels[ChannelIdx].RxConnection.CurrentCfSn + 1u) &
                        CANTP_CF_SN_MASK;
			} /*END OF SDU whole reception not finished*/
        } /*END OF if (BUFREQ_E_NOT_OK == bufRslt)*/
    } /*END OF handling expected CF*/
	if (FALSE == finished)/*clear the event*/
	{
        CanTp_Channels[ChannelIdx].RxConnection.EventFlags =
        		CanTp_Channels[ChannelIdx].RxConnection.EventFlags &
        		((uint8)~CANTP_CHANNEL_EVENT_CF_RECEIVED);
    }
}
/*
 * Brief               FF come, just execute one time of PduR_CanTpStartOfReception.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleFFStart(uint8 ChannelIdx)
{
	PduIdType iPduId;
    PduIdType nSduId;
	PduInfoType pduInfo;
	P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    BufReq_ReturnType bufRslt;
    Std_ReturnType rslt;
    PduLengthType position;
    uint8 chnIdx;
    CanTp_FCInfoType CanTp_FC;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[5];

    pduInfo.MetaDataPtr = Metadata;
#endif

    pduInfo.SduLength = 0;
    nSduId = CanTp_Channels[ChannelIdx].RxConnection.NSduId;
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId, &chnIdx);
    iPduId = rxSduCfgPtr->RxIPduId;
	#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    if ((CanTp_Channels[ChannelIdx].RxConnection.MetaDataLength > 0u)
            && ((CANTP_ADDRESS_EXTENSION_8_MASK ==
                    (rxSduCfgPtr->CanTpPduFlag & CANTP_ADDRESS_EXTENSION_8_MASK))
                    || (CANTP_SA16_AND_TA16_MASK ==
                            (rxSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))))
    {
    	CanTp_ConstructForwardRxMetaData(ChannelIdx, rxSduCfgPtr, pduInfoPtr);
    }
	#endif

    bufRslt = PduR_CanTpStartOfReception(
    		iPduId,
			pduInfoPtr,
			CanTp_Channels[ChannelIdx].RxConnection.SduDataTotalCount,
			&CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining);

	switch (bufRslt)
	{
		case BUFREQ_OK:
			if (CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen >
			CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining)
			{
		        CanTp_ReleaseRxChannel(ChannelIdx);
				PduR_CanTpRxIndication(iPduId, E_NOT_OK);
			}
			else
			{
				position = CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition;
				pduInfo.SduDataPtr = &CanTp_Channels[ChannelIdx].RxConnection.LocalBuf[position];
				pduInfo.SduLength = CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen;
				bufRslt = PduR_CanTpCopyRxData(iPduId, pduInfoPtr,
				        &CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining);
				if (BUFREQ_E_NOT_OK == bufRslt)
				{
					CanTp_ReleaseRxChannel(ChannelIdx);
					PduR_CanTpRxIndication(iPduId, E_NOT_OK);
				}
				else
				{
					CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining =
							CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining -
							CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen;
					/*calculate block size*/
					CanTp_CalcBS(ChannelIdx);
					/*SWS_CanTp_00082,check upper layer buffer is enough or not for next block*/
					rslt = CanTp_CompareBufferWithBs(ChannelIdx);
					if (E_NOT_OK == rslt)
					{
						/*the upper buffer is not enough to store the next block,
						 * so need to get more buffer*/
						CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_LARGE_START;
						CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer =
						        CANTP_NB;
						CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime =
						        rxSduCfgPtr->Nbr;
                        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].
                                RxConnection.ChannelTimer.StartTime));
					}
					else
					{
						/*buffer enough, start a new block*/
						rslt = CanTp_RxBlockStart(ChannelIdx);
						if (E_NOT_OK == rslt)
						{
							CanTp_ReleaseRxChannel(ChannelIdx);
							PduR_CanTpRxIndication(iPduId, E_NOT_OK);
						}
					}
				}
	    		/*update expected next SN*/
				CanTp_Channels[ChannelIdx].RxConnection.CurrentCfSn =
						(uint8)(CanTp_Channels[ChannelIdx].RxConnection.CurrentCfSn + 1u) &
						CANTP_CF_SN_MASK;
			}
			break;
		case BUFREQ_E_NOT_OK:
		    CanTp_InitRxChannel(ChannelIdx);
			break;
		case BUFREQ_E_OVFL:
			/*the CanTp module shall send a Flow Control N-PDU
			 * with overflow status (FC(OVFLW)) and abort the N-SDU reception*/
			/*when handling FF, send FC_OVFL*/
		    CanTp_FC.FcFs = CANTP_FC_FS_OVFLW;
		    CanTp_FC.FcBS = 0;
		    CanTp_FC.FcSTMin = 0;
			pduInfo.SduLength = CanTp_ConstructFCPci(ChannelIdx,CanTp_FC
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)))
                                            ,
                                            pduInfo.MetaDataPtr
#endif
                                            );
			pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].RxConnection.FcBuf;
			(void)CanIf_Transmit(rxSduCfgPtr->TxFcLPduId, pduInfoPtr);
			/*release resource of the channel*/
			CanTp_ReleaseRxChannel(ChannelIdx);
			break;
		default:
		    /*idle*/
			break;
	}
}
/*
 * Brief               Handle received FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleFFReception(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    PduLengthType ffDl;
    uint8 ChannelId;

    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.RcvdNSduId, &ChannelId);

    if ((CANTP_SF_RECEIVING == CanTp_Channels[ChannelIdx].RxConnection.RootState)
        || (CANTP_LARGE_RECEIVING == CanTp_Channels[ChannelIdx].RxConnection.RootState))
    {
        /*Another SF received when the channel is receiving a SDU*/
        /*Notify error for old SDU reception*/
        PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
    }

    ffDl = CanTp_RxGetFFDl(CanTp_Channels[ChannelIdx].RxConnection.LocalBuf, rxNSduCfgPtr);
    if (0u == ffDl)
    {
        PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
    }

    /*update channel runtime information*/
    CanTp_Channels[ChannelIdx].RxConnection.RootState = CANTP_LARGE_RECEIVING;
    CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_FF_START;
    CanTp_Channels[ChannelIdx].RxConnection.NSduId =
            CanTp_Channels[ChannelIdx].RxConnection.RcvdNSduId;
    /*SWS_CanTp_00166,the CanTp module shall start a time-out N_Br
     * before calling PduR_CanTpStartOfReception*/
    CanTp_Channels[ChannelIdx].RxConnection.RcvdNSduId = 0;
    CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_NB;
    CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime = rxNSduCfgPtr->Nbr;
    CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.StartTime));
    CanTp_Channels[ChannelIdx].RxConnection.HandleType = CANTP_FTYPE_FF;

    if (CANTP_STANDARD == rxNSduCfgPtr->AddressingFormat)
    {
    	/*standard FF*/
    	if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
    	{
    		/*FF_DL less than or equal to 4095*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_LT4095_STD;
    	}
    	else
    	{
    		/*FF_DL bigger than 4095*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_BT4095_STD;
    	}
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == rxNSduCfgPtr->AddressingFormat)
    {
        /*extended FF*/
    	if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
		{
			/*FF_DL less than or equal to 4095*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_LT4095_EX;
		}
		else
		{
			/*FF_DL bigger than 4095*/
			/*update local information*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_BT4095_EX;
		}
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == rxNSduCfgPtr->AddressingFormat)
	{
		if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
		{
			/*FF_DL less than or equal to 4095*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_LT4095_NF;
		}
		else
		{
			/*FF_DL bigger than 4095*/
			/*update local information*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_BT4095_NF;
		}
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == rxNSduCfgPtr->AddressingFormat)
	{
		if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
		{
			/*FF_DL less than or equal to 4095*/
			/*update local information*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_LT4095_MIX11;
		}
		else
		{
			/*FF_DL bigger than 4095*/
			/*update local information*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_BT4095_MIX11;
		}
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == rxNSduCfgPtr->AddressingFormat)
	{
		if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
		{
			/*FF_DL less than or equal to 4095*/
			/*update local information*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_MIX29_FF_DATA_OFFSET_LT4095;
		}
		else
		{
			/*FF_DL bigger than 4095*/
			/*update local information*/
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
			        CANTP_MIX29_FF_DATA_OFFSET_BT4095;
		}
	}
	#endif
	else
	{}

    /*initialize SDU information*/
	CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen =
	        CanTp_Channels[ChannelIdx].RxConnection.DataLen -
			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition;
    CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining = ffDl;
    CanTp_Channels[ChannelIdx].RxConnection.SduDataTotalCount = ffDl;

    /*initialize buffer information*/
    CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining = 0;

    /*initialize Flow control information*/
    CanTp_Channels[ChannelIdx].RxConnection.CurrentBs = 0;
#if (STD_ON == CANTP_FD)
    CanTp_Channels[ChannelIdx].RxConnection.FDDlc =
            CanTp_Channels[ChannelIdx].RxConnection.DataLen;
#endif
    CanTp_Channels[ChannelIdx].RxConnection.CurrentCfSn = 0;
    CanTp_Channels[ChannelIdx].RxConnection.HandledCfCount = 0;
    CanTp_Channels[ChannelIdx].RxConnection.SentWftCount = 0;

    /*clear the event*/
    CanTp_Channels[ChannelIdx].RxConnection.EventFlags =
    		CanTp_Channels[ChannelIdx].RxConnection.EventFlags &
    		((uint8)~CANTP_CHANNEL_EVENT_FF_RECEIVED);
}
/*
 * Brief               Handle received SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleSFReception(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    PduLengthType sfDl;
    uint8 chnIdx;

    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.RcvdNSduId, &chnIdx);

    if ((CANTP_SF_RECEIVING == CanTp_Channels[ChannelIdx].RxConnection.RootState)
        || (CANTP_LARGE_RECEIVING == CanTp_Channels[ChannelIdx].RxConnection.RootState))
    {
        /*Another SF received when the channel is receiving a SDU*/
        /*Notify error for old SDU reception*/
        PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
    }
    sfDl = CanTp_RxGetSFDl(CanTp_Channels[ChannelIdx].RxConnection.LocalBuf, rxNSduCfgPtr
#if (STD_ON == CANTP_FD)
    		,CanTp_Channels[ChannelIdx].RxConnection.DataLen
#endif
			);
    /*SF data length valid*/
    if (sfDl != 0x0u)
    {
        /*update channel runtime information*/
        CanTp_Channels[ChannelIdx].RxConnection.RootState = CANTP_SF_RECEIVING;
        CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_SF_START;
        CanTp_Channels[ChannelIdx].RxConnection.NSduId =
                CanTp_Channels[ChannelIdx].RxConnection.RcvdNSduId;
        CanTp_Channels[ChannelIdx].RxConnection.RcvdNSduId = 0;
        CanTp_Channels[ChannelIdx].RxConnection.HandleType = CANTP_FTYPE_SF;

        /*initialize SDU information*/
        if (CANTP_STANDARD == rxNSduCfgPtr->AddressingFormat)
        {
            /*standard SF*/
        	if (CanTp_Channels[ChannelIdx].RxConnection.DataLen <= 8u)
        	{
        		/*SF_DL less than or equal to 7*/
    			/*update local buffer information*/
    			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
    			        CANTP_SF_DATA_OFFSET_STD;
        	}
    		#if (STD_ON == CANTP_FD)
        	else
        	{
        		/*SF_DL bigger than 7*/
    			/*update local buffer information*/
    			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
    			        CANTP_SF_DATA_OFFSET_CANFD_STD;
        	}
    		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
        }
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
        else if (CANTP_EXTENDED == rxNSduCfgPtr->AddressingFormat)
        {
        	/*extended SF*/
        	if (CanTp_Channels[ChannelIdx].RxConnection.DataLen <= 8u)
        	{
        		/*SF_DL less than or equal to 7*/
    			/*update local buffer information*/
    			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
    			        CANTP_SF_DATA_OFFSET_EX;
        	}
    		#if (STD_ON == CANTP_FD)
        	else
        	{
        		/*SF_DL bigger than 7*/
    			/*update local buffer information*/
    			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
    			        CANTP_SF_DATA_OFFSET_CANFD_EX;
        	}
    		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
        }
    	#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
        else if (CANTP_NORMALFIXED == rxNSduCfgPtr->AddressingFormat)
        {
    		if (CanTp_Channels[ChannelIdx].RxConnection.DataLen <= 8u)
    		{
    			/*SF_DL less than or equal to 7*/
    			/*update local buffer information*/
    			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
    			        CANTP_SF_DATA_OFFSET_NF;
    		}
    		#if (STD_ON == CANTP_FD)
    		else
    		{
    			/*SF_DL bigger than 7*/
    			/*update local buffer information*/
    			CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
    			        CANTP_SF_DATA_OFFSET_CANFD_NF;
    		}
    		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
        }
    	#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		else if (CANTP_MIXED == rxNSduCfgPtr->AddressingFormat)
		{
			if (CanTp_Channels[ChannelIdx].RxConnection.DataLen <= 8u)
			{
				/*SF_DL less than or equal to 7*/
				/*update local buffer information*/
				CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
				        CANTP_SF_DATA_OFFSET_MIX11;
			}
			#if (STD_ON == CANTP_FD)
			else
			{
				/*SF_DL bigger than 7*/
				/*update local buffer information*/
				CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
				        CANTP_SF_DATA_OFFSET_CANFD_MIX11;
			}
			#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
		}
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		else if (CANTP_MIXED29BIT == rxNSduCfgPtr->AddressingFormat)
		{
			if (CanTp_Channels[ChannelIdx].RxConnection.DataLen <= 8u)
			{
				/*SF_DL less than or equal to 7*/
				/*update local buffer information*/
				CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
				        CANTP_SF_DATA_OFFSET_MIX29;
			}
			#if (STD_ON == CANTP_FD)
			else
			{
				/*SF_DL bigger than 7*/
				/*update local buffer information*/
				CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition =
				        CANTP_SF_DATA_OFFSET_CANFD_MIX29;
			}
			#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
		}
		#endif
		else
		{}

        CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen = sfDl;
        CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining = sfDl;
        CanTp_Channels[ChannelIdx].RxConnection.SduDataTotalCount = sfDl;

        /*initialize buffer information*/
        CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining = 0;

        /*clear the event*/
        CanTp_Channels[ChannelIdx].RxConnection.EventFlags =
        		CanTp_Channels[ChannelIdx].RxConnection.EventFlags &
        		((uint8)~CANTP_CHANNEL_EVENT_SF_RECEIVED);
    }
}
/*
 * Brief               Cancel Receive.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleReceiveCancel(uint8 ChannelIdx)
{
    PduIdType sduId;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    uint8 chnIdx;

    sduId = CanTp_Channels[ChannelIdx].RxConnection.NSduId;
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(sduId, &chnIdx);
    /*Release the resource related with the channel*/
    CanTp_ReleaseRxChannel(ChannelIdx);
    /*Notify the upper*/
    PduR_CanTpRxIndication(rxSduCfgPtr->RxIPduId, E_NOT_OK);
}
/*
 * Brief               Handle confirmation for FC.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleFCCfm(uint8 ChannelIdx)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    uint8 chnIdx;
    uint32 N_Cr;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.NSduId, &chnIdx);

    if (CANTP_RX_LARGE_V4FCWT_CFM == CanTp_Channels[ChannelIdx].RxConnection.Substate)
    {
        /*transit to RX_LARGE_START, which will try to get buffer again*/
        CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_LARGE_START;
        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_NB;
        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime = rxSduCfgPtr->Nbr;
        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.StartTime));
    }
    else if (CANTP_RX_LARGE_V4FCCTS_CFM == CanTp_Channels[ChannelIdx].RxConnection.Substate)
    {
        /*FC_CTS sent, start to waiting for CFs*/
        CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_LARGE_V4CF;
        if (rxSduCfgPtr->Ncr != NULL_PTR)
        {
        	N_Cr = *(rxSduCfgPtr->Ncr);
            CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_NC;
            CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime = N_Cr;
            CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.StartTime));
        }
        else
        {
        	CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
        }
    }
    else
    {
        /*will not happen with handling of filtering when indicated by CanIf*/
    }

    /*clear the event*/
    CanTp_Channels[ChannelIdx].RxConnection.EventFlags =
    		CanTp_Channels[ChannelIdx].RxConnection.EventFlags &
    		((uint8)~CANTP_CHANNEL_EVENT_TXFC_CONFIRMED);
}
/*
 * Brief               Back ground handling for receiving SF,in this state the channel
 * is trying to get buffer to store received SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleSFStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    uint8 chnIdx;
    const PduInfoType *pduInfoPtr = &pduInfo;
    BufReq_ReturnType bufRslt;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    PduLengthType bufferSize;
    PduIdType nSduId;
    PduIdType iPduId;
    PduLengthType pduLength;
    PduLengthType pduPosition;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 data[5];

    pduInfo.MetaDataPtr = data;
#endif
    
    pduInfo.SduLength = 0;
    nSduId = CanTp_Channels[ChannelIdx].RxConnection.NSduId;
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId, &chnIdx);
    iPduId = rxSduCfgPtr->RxIPduId;

	#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    if ((CanTp_Channels[ChannelIdx].RxConnection.MetaDataLength > 0u)
            && ((CANTP_ADDRESS_EXTENSION_8_MASK ==
                    (rxSduCfgPtr->CanTpPduFlag & CANTP_ADDRESS_EXTENSION_8_MASK))
                    || (CANTP_SA16_AND_TA16_MASK ==
                            (rxSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))))
    {
    	CanTp_ConstructForwardRxMetaData(ChannelIdx, rxSduCfgPtr, pduInfoPtr);
    }
	#endif

    pduLength = CanTp_Channels[ChannelIdx].RxConnection.SduDataTotalCount;
    bufRslt = PduR_CanTpStartOfReception(iPduId, pduInfoPtr, pduLength, &bufferSize);

    /*handle according the result of request RX buffer*/
    switch (bufRslt)
    {
        case BUFREQ_OK:
        	if (bufferSize >= pduLength)
        	{
        		pduPosition = CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition;
				pduInfo.SduDataPtr =
				        &CanTp_Channels[ChannelIdx].RxConnection.LocalBuf[pduPosition];
				pduInfo.SduLength = CanTp_Channels[ChannelIdx].RxConnection.LocalDataLen;
        		bufRslt = PduR_CanTpCopyRxData(iPduId, pduInfoPtr, &bufferSize);
        		CanTp_ReleaseRxChannel(ChannelIdx);
        		if (bufRslt == BUFREQ_OK)
        		{
        			PduR_CanTpRxIndication(iPduId, E_OK);
        		}
        		else
        		{
        			PduR_CanTpRxIndication(iPduId, E_NOT_OK);
        		}
        	}
        	else
        	{
        		/*SWS_CanTp_00339,a smaller available buffer size
        		 * than needed for the already received data*/
        		/*abort the reception of the N-SDU*/
        		CanTp_ReleaseRxChannel(ChannelIdx);
        		PduR_CanTpRxIndication(iPduId, E_NOT_OK);
        	}
            break;
        case BUFREQ_E_NOT_OK:
        	/*Release the resource of the channel*/
            CanTp_InitRxChannel(ChannelIdx);
        	break;
        case BUFREQ_E_OVFL:
        default:
            /*Release the resource of the channel*/
        	CanTp_ReleaseRxChannel(ChannelIdx);
            /*Notify the upper for error*/
            PduR_CanTpRxIndication(iPduId, E_NOT_OK);
            break;
    } /*END OF switch (bufRslt)*/
}
/*
 * Brief               Back ground handling for large SDU receiving, trying to get RX buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
CanTp_RxHandleLargeStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    uint8 chnIdx;
    uint8 wtfMax;
    P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    Std_ReturnType rslt;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    PduIdType nSduId;
    PduIdType iPduId;
    uint32 N_Ar;
    CanTp_FCInfoType CanTp_FC;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[4];

    pduInfo.MetaDataPtr = Metadata;
#endif

    nSduId = CanTp_Channels[ChannelIdx].RxConnection.NSduId;
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId, &chnIdx);
    iPduId = rxSduCfgPtr->RxIPduId;

    if (CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer == CANTP_NB)
    {
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 0;
        /*query the upper layer about the available buffer size*/
    	(void)PduR_CanTpCopyRxData(iPduId, pduInfoPtr,
    	        &CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining);
    }

    /*if or not have enough buffer size in the upper layer to store the next block*/
    rslt = CanTp_CompareBufferWithBs(ChannelIdx);
    if ( E_NOT_OK == rslt)
    {
    	/*upper buffer smaller than next block size, can not receive*/
		if ((CANTP_NB == CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer)
				&&(0u == CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime))
		{
			if (rxSduCfgPtr->RxWftMax != NULL_PTR)
			{
				wtfMax = *(rxSduCfgPtr->RxWftMax);
			}
			else
			{
				wtfMax = 0xFF;
			}

			/*Nbr expires*/
			if (0u != wtfMax)
			{
				/*If the whole SDU will be send in more than one
				 * single block(I.E. BS is not zero) and it the end of current block
				 * but not the last block, or if currently it is
				 * handling First Frame, it is allowed to send FC_WAIT*/
				if ((CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].RxConnection.HandleType)
					|| ((CanTp_Channels[ChannelIdx].RxConnection.CurrentBs ==
					        CanTp_Channels[ChannelIdx].RxConnection.HandledCfCount)
						&& (CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining > 0u)))
				{
					/*Try to send FC_WAIT*/
					if (wtfMax <= CanTp_Channels[ChannelIdx].RxConnection.SentWftCount)
					{
						/*Can not send any more FC_WAIT. Abort this reception*/
						/*release resource of this channel*/
						CanTp_ReleaseRxChannel(ChannelIdx);
						PduR_CanTpRxIndication(iPduId, E_NOT_OK);
					}
					else
					{
						/*Send FC_WAIT and then wait for the confirmation*/
			            CanTp_FC.FcFs = CANTP_FC_FS_WT;
			            CanTp_FC.FcBS = 0;
			            CanTp_FC.FcSTMin = 0;
						pduInfo.SduLength = CanTp_ConstructFCPci(ChannelIdx,CanTp_FC
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)))
                                            ,
                                            pduInfo.MetaDataPtr
#endif
                                            );
						pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].RxConnection.FcBuf;

						rslt = CanIf_Transmit(rxSduCfgPtr->TxFcLPduId, pduInfoPtr);
						if (E_OK == rslt)
						{
							/*send FC OK*/
							CanTp_Channels[ChannelIdx].RxConnection.SentWftCount++;
							/*transit to waiting for confirmation of FC_WAIT*/
							CanTp_Channels[ChannelIdx].RxConnection.Substate =
							        CANTP_RX_LARGE_V4FCWT_CFM;
						    if (rxSduCfgPtr->Nar != NULL_PTR)
						    {
						    	N_Ar = *(rxSduCfgPtr->Nar);
								CanTp_Channels[ChannelIdx].RxConnection.
								ChannelTimer.EnabledTimer = CANTP_NA;
								CanTp_Channels[ChannelIdx].RxConnection.
								ChannelTimer.RemainTime = N_Ar;
		                        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].
		                                RxConnection.ChannelTimer.StartTime));
						    }
						    else
						    {
						    	CanTp_Channels[ChannelIdx].RxConnection.
						    	ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
						    }
						}
						else
						{
							CanTp_ReleaseRxChannel(ChannelIdx);
							PduR_CanTpRxIndication(iPduId, E_NOT_OK);
						}
					}
				} /*END OF try to send FC_WAIT*/
			}
		}/*End of Nbr expires*/
    }
    else
    {
    	/*upper buffer bigger than next block size*/
		/*FF reception finished or current BS reception finished, send FC_CTS to start a block*/
    	rslt = CanTp_RxBlockStart(ChannelIdx);
    	if (E_NOT_OK == rslt)
    	{
    		CanTp_ReleaseRxChannel(ChannelIdx);
			PduR_CanTpRxIndication(iPduId, E_NOT_OK);
    	}
    }
}
/*
 * Brief               Get SDU configuration by N-SDU ID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNSduId(PduIdType CanTpRxSduId, uint8* channelId)
{
	uint8 chnIdx;
    uint8 rxNSduIdx;
    P2CONST(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) channelPtr;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;

    for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CANTP_CHANNEL_NUMBER); chnIdx++)
    {
    	channelPtr = &CanTp_CfgData.CanTpChannel[chnIdx];
		for (rxNSduIdx = 0; (NULL_PTR == result) &&
		(rxNSduIdx < channelPtr->RxNSduNum); rxNSduIdx++)
		{
			if (channelPtr->RxNSdus[rxNSduIdx].RxNSduId == CanTpRxSduId)
			{
				result = &channelPtr->RxNSdus[rxNSduIdx];
				*channelId = chnIdx;
			}
		}
    }
    return result;
}
/*
 * Brief               Get FF_DL information from the data content of FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetFFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr)
{
    /*because PduLengthType is uint16, so FFDL can not bigger than 65535*/
	PduLengthType ffDl = 0;

    /*check FF DL*/
    if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
    {
        /*PCI starts at byte[0]*/
        ffDl = (PduLengthType)((PduLengthType)CanTpRxPduPtr[CANTP_PCI_OFFSET_STD] &
                (PduLengthType)CANTP_FF_DL_MASK);
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 1u];
        /*FFDL > 4095*/
        if (0x0u == ffDl)
        {
        	ffDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 2u];
        	ffDl <<= CANTP_OFFSET_BYTE_PART;
        	ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 3u];
        	ffDl <<= CANTP_OFFSET_BYTE_PART;
        	ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 4u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 5u];
			if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
			{
			    ffDl = 0;
			}
        }
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == RxNSduCfgPtr->AddressingFormat)
    {
        /*PCI starts at byte[1]*/
        ffDl = (PduLengthType)((PduLengthType)CanTpRxPduPtr[CANTP_PCI_OFFSET_EX] &
                (PduLengthType)CANTP_FF_DL_MASK);
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 1u];
		/*FFDL > 4095*/
		if (0x0u == ffDl)
		{
			ffDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 2u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 3u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 4u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 5u];
            if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
            {
                ffDl = 0;
            }
		}
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_NORMALFIXED == RxNSduCfgPtr->AddressingFormat)
    {
        /*PCI starts at byte[0]*/
        ffDl = (PduLengthType)((PduLengthType)CanTpRxPduPtr[CANTP_PCI_OFFSET_NF] &
                (PduLengthType)CANTP_FF_DL_MASK);
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_NF + 1u];
		/*FFDL > 4095*/
		if (0x0u == ffDl)
		{
			ffDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_NF + 2u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_NF + 3u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_NF + 4u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_NF + 5u];
            if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
            {
                ffDl = 0;
            }
		}
    }
	#endif
	#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)||(CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
    else if ((CANTP_MIXED == RxNSduCfgPtr->AddressingFormat) ||
            (CANTP_MIXED29BIT == RxNSduCfgPtr->AddressingFormat))
    {
        /*PCI starts at byte[1]*/
        ffDl = (PduLengthType)((PduLengthType)CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11] &
                (PduLengthType)CANTP_FF_DL_MASK);
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11 + 1u];
		/*FFDL > 4095*/
		if (0x0u == ffDl)
		{
			ffDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11 + 2u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11 + 3u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11 + 4u];
			ffDl <<= CANTP_OFFSET_BYTE_PART;
			ffDl = ffDl + CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11 + 5u];
            if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
            {
                ffDl = 0;
            }
		}
    }
	#endif
    else
    {}

    return ffDl;
}
/*
 * Brief               Get SFF_DL information from the data content of SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetSFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr
#if (STD_ON == CANTP_FD)
	,PduLengthType CanTpDLC
#endif
	)
{
    uint8 sfDl = 0;

    /*check SF DL*/
    if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
    {
        /*PCI is byte[0]*/
        sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_STD] & CANTP_SF_DL_MASK;
		#if (STD_ON == CANTP_FD)
        if ((0x0u == sfDl) && (8u < CanTpDLC))
        {
        	/*CanFD only*/
        	if (CAN_FD == RxNSduCfgPtr->CanFrameType)
        	{
        		sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_STD + 1u];
        	}
        }
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == RxNSduCfgPtr->AddressingFormat)
    {
        /*PCI is byte[1]*/
        sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_EX] & CANTP_SF_DL_MASK;
		#if (STD_ON == CANTP_FD)
        if ((0x0u == sfDl) && (8u < CanTpDLC))
        {
        	/*CanFD only*/
        	if (CAN_FD == RxNSduCfgPtr->CanFrameType)
        	{
        		sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_EX + 1u];
        	}
        }
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == RxNSduCfgPtr->AddressingFormat)
	{
		/*PCI is byte[0]*/
		sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_NF] & CANTP_SF_DL_MASK;
		#if (STD_ON == CANTP_FD)
        if ((0x0u == sfDl) && (8u < CanTpDLC))
		{
			/*CanFD only*/
			if (CAN_FD == RxNSduCfgPtr->CanFrameType)
			{
				sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_NF + 1u];
			}
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)||(CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
	else if ((CANTP_MIXED == RxNSduCfgPtr->AddressingFormat) ||
	        (CANTP_MIXED29BIT == RxNSduCfgPtr->AddressingFormat))
	{
		/*PCI is byte[1]*/
		sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11] & CANTP_SF_DL_MASK;
		#if (STD_ON == CANTP_FD)
        if ((0x0u == sfDl) && (8u < CanTpDLC))
		{
			/*CanFD only*/
			if (CAN_FD == RxNSduCfgPtr->CanFrameType)
			{
				sfDl = CanTpRxPduPtr[CANTP_PCI_OFFSET_MIX11 + 1u];
			}
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	else
	{}

    return sfDl;
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               store Rx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)CanTp_SaveRxMetaDataInfo(
	uint8 channelId,
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxInfoPtr
	)
{
    Std_ReturnType ret = E_OK;
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
	P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[channelId];
#endif

	switch (RxNSduCfgPtr->AddressingFormat)
	{
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_EXTENDED:
            channelPtr->RxConnection.MetaDataN_TA =
                    CanTpRxInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX];
            #if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
            if ((RxNSduCfgPtr->NTa == NULL_PTR)
                    || (channelPtr->RxConnection.MetaDataN_TA != *(RxNSduCfgPtr->NTa)))
            {
                ret = E_NOT_OK;
            }
            #endif
			break;
		#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED:
            channelPtr->RxConnection.MetaDataN_AE =
                    CanTpRxInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX];
            #if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
            if ((RxNSduCfgPtr->NAe == NULL_PTR)
                    || (channelPtr->RxConnection.MetaDataN_AE != *(RxNSduCfgPtr->NAe)))
            {
                ret = E_NOT_OK;
            }
            #endif
			break;
		#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
		case  CANTP_NORMALFIXED:
	        if ((channelPtr->RxConnection.MetaDataLength >= 4u)
	            && (CANTP_CAN_ID_32_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
		    {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                channelPtr->RxConnection.MetaDataN_SA =
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_MSB];
                channelPtr->RxConnection.MetaDataN_TA =
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_MSB];
                #else/* Little endian bit ordering  */
                channelPtr->RxConnection.MetaDataN_SA =
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_LSB];
                channelPtr->RxConnection.MetaDataN_TA =
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_LSB];
                #endif
                #if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
                if ((RxNSduCfgPtr->NSa == NULL_PTR)
                        || (RxNSduCfgPtr->NTa == NULL_PTR)
                        || (channelPtr->RxConnection.MetaDataN_SA != *(RxNSduCfgPtr->NSa))
                        || (channelPtr->RxConnection.MetaDataN_TA != *(RxNSduCfgPtr->NTa)))
                {
                    ret = E_NOT_OK;
                }
                #endif
	        }
	        else
	        {
                ret = E_NOT_OK;
	        }
			break;
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		case  CANTP_MIXED29BIT:
            channelPtr->RxConnection.MetaDataN_AE =
                    CanTpRxInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX29];
            #if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
            if ((RxNSduCfgPtr->NAe == NULL_PTR)
                    || (channelPtr->RxConnection.MetaDataN_AE != *(RxNSduCfgPtr->NAe)))
            {
                ret = E_NOT_OK;
            }
            else
            #endif
            {
                if ((channelPtr->RxConnection.MetaDataLength >= 4u)
                    && (CANTP_CAN_ID_32_MASK ==
                            (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
                {
                    #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                    channelPtr->RxConnection.MetaDataN_SA =
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_MSB];
                    channelPtr->RxConnection.MetaDataN_TA =
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_MSB];
                    #else/* Little endian bit ordering  */
                    channelPtr->RxConnection.MetaDataN_SA =
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_LSB];
                    channelPtr->RxConnection.MetaDataN_TA =
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_LSB];
                    #endif
                    #if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
                    if ((RxNSduCfgPtr->NSa == NULL_PTR)
                            || (RxNSduCfgPtr->NTa == NULL_PTR)
                            || (channelPtr->RxConnection.MetaDataN_SA != *(RxNSduCfgPtr->NSa))
                            || (channelPtr->RxConnection.MetaDataN_TA != *(RxNSduCfgPtr->NTa)))
                    {
                        ret = E_NOT_OK;
                    }
                    #endif
                }
                else
                {
                    ret = E_NOT_OK;
                }
            }
			break;
		#endif
		default:
		    /*idle*/
			break;
	}
	return ret;
}
#endif

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
/*
 * Brief               Construct Rx meta data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)CanTp_ConstructForwardRxMetaData(
	uint8 channelId,
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_VAR) CanTpRxInfoPtr
	)
{
    uint8 i;
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[channelId];
#endif

	for(i = 0;i < 5u; i++)
	{
	    CanTpRxInfoPtr->MetaDataPtr[i] = 0;
	}
	switch (RxNSduCfgPtr->AddressingFormat)
	{
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_EXTENDED:
            if (CANTP_SA16_AND_TA16_MASK ==
                    (RxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_MSB] =
                        channelPtr->RxConnection.MetaDataN_TA;
                #else/* Little endian bit ordering  */
                CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_LSB] =
                        channelPtr->RxConnection.MetaDataN_TA;
                #endif
            }
			break;
		#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_NORMALFIXED:
		    if (CANTP_SA16_AND_TA16_MASK ==
		            (RxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))
		    {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_MSB] =
                        channelPtr->RxConnection.MetaDataN_SA;
                CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_MSB] =
                        channelPtr->RxConnection.MetaDataN_TA;
                #else/* Little endian bit ordering  */
                CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_LSB] =
                        channelPtr->RxConnection.MetaDataN_SA;
                CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_LSB] =
                        channelPtr->RxConnection.MetaDataN_TA;
                #endif
		    }
			break;
		#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED:
			if (CANTP_ADDRESS_EXTENSION_8_MASK ==
			        (RxNSduCfgPtr->CanTpPduFlag & CANTP_ADDRESS_EXTENSION_8_MASK))
			{
                CanTpRxInfoPtr->MetaDataPtr[CANTP_AE_FOR_DCM] =
                        channelPtr->RxConnection.MetaDataN_AE;
			}
			break;
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED29BIT:
            if (CANTP_SA16_AND_TA16_MASK ==
                    (RxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_MSB] =
                        channelPtr->RxConnection.MetaDataN_SA;
                CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_MSB] =
                        channelPtr->RxConnection.MetaDataN_TA;
                #else/* Little endian bit ordering  */
                CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_LSB] =
                        channelPtr->RxConnection.MetaDataN_SA;
                CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_LSB] =
                        channelPtr->RxConnection.MetaDataN_TA;
                #endif
            }
            if (CANTP_ADDRESS_EXTENSION_8_MASK ==
                    (RxNSduCfgPtr->CanTpPduFlag & CANTP_ADDRESS_EXTENSION_8_MASK))
            {
                CanTpRxInfoPtr->MetaDataPtr[CANTP_AE_FOR_DCM] =
                        channelPtr->RxConnection.MetaDataN_AE;
            }
			break;
		#endif
		default:
            /*idle*/
			break;
	}
}
#endif
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               Construct FC meta data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
static FUNC(void, CANTP_CODE)
CanTp_ConstructFCMetaData(
	uint8 channelId,
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
	,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Metadata
#endif
	)
{
	switch (RxNSduCfgPtr->AddressingFormat)
	{
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_EXTENDED:
		    CanTp_Channels[channelId].RxConnection.FcBuf[CANTP_TA_OFFSET_META_EX] =
		            CanTp_Channels[channelId].RxConnection.MetaDataN_TA;
			break;
		#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED:
            CanTp_Channels[channelId].RxConnection.FcBuf[CANTP_AE_OFFSET_META_MIX] =
                    CanTp_Channels[channelId].RxConnection.MetaDataN_AE;
			break;
		#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_NORMALFIXED:
		    if ((CanTp_Channels[channelId].RxConnection.MetaDataLength >= 4u)
		         && (CANTP_CAN_ID_32_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
		    {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                Metadata[CANTP_SA_OFFSET_META_MIX29_MSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_TA;
                Metadata[CANTP_TA_OFFSET_META_MIX29_MSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_SA;
                #else/* Little endian bit ordering  */
                Metadata[CANTP_SA_OFFSET_META_MIX29_LSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_TA;
                Metadata[CANTP_TA_OFFSET_META_MIX29_LSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_SA;
                #endif
		    }
			break;
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED29BIT:
            if ((CanTp_Channels[channelId].RxConnection.MetaDataLength >= 4u)
                 && (CANTP_CAN_ID_32_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                Metadata[CANTP_SA_OFFSET_META_MIX29_MSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_TA;
                Metadata[CANTP_TA_OFFSET_META_MIX29_MSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_SA;
                #else/* Little endian bit ordering  */
                Metadata[CANTP_SA_OFFSET_META_MIX29_LSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_TA;
                Metadata[CANTP_TA_OFFSET_META_MIX29_LSB] =
                        CanTp_Channels[channelId].RxConnection.MetaDataN_SA;
                #endif
            }
            CanTp_Channels[channelId].RxConnection.FcBuf[CANTP_AE_OFFSET_META_MIX29] =
                    CanTp_Channels[channelId].RxConnection.MetaDataN_AE;
			break;
		#endif
		default:
            /*idle*/
			break;
	}
}
#endif

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*check RxCF meta data*/
FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckRxCFMetaData(
	uint8 channelIdx,
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxInfoPtr
	)
{
    Std_ReturnType result = E_OK;
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
        P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

        channelPtr = &CanTp_Channels[channelIdx];
#endif

    switch (RxNSduCfgPtr->AddressingFormat)
    {
        #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
        case CANTP_EXTENDED:
        {
            if (CanTpRxInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] !=
                    channelPtr->RxConnection.MetaDataN_TA)
            {
                result = E_NOT_OK;
            }
            break;
        }
        #endif
        #if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
        case CANTP_MIXED:
        {
            if (CanTpRxInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] !=
                    channelPtr->RxConnection.MetaDataN_AE)
            {
                result = E_NOT_OK;
            }
            break;
        }
        #endif
        #if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
        case CANTP_NORMALFIXED:
        {
            if ((channelPtr->RxConnection.MetaDataLength >= 4u)
                    && (CANTP_CAN_ID_32_MASK ==
                            (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                if ((channelPtr->RxConnection.MetaDataN_SA !=
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_MIX29_MSB])||
                    (channelPtr->RxConnection.MetaDataN_TA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_MIX29_MSB]))
                #else/* Little endian bit ordering  */
                if ((channelPtr->RxConnection.MetaDataN_SA !=
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_MIX29_LSB])||
                    (channelPtr->RxConnection.MetaDataN_TA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_MIX29_LSB]))
                #endif
                {
                    result = E_NOT_OK;
                }
            }
            else
            {
                result = E_NOT_OK;
            }
            break;
        }
        #endif
        #if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
        case CANTP_MIXED29BIT:
        {
            if (CanTpRxInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX29] !=
                    channelPtr->RxConnection.MetaDataN_AE)
            {
                result = E_NOT_OK;
            }
            if ((channelPtr->RxConnection.MetaDataLength >= 4u)
                    && (CANTP_CAN_ID_32_MASK ==
                            (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                if((channelPtr->RxConnection.MetaDataN_SA !=
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_MIX29_MSB])
                    || (channelPtr->RxConnection.MetaDataN_TA !=
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_MIX29_MSB])
                #else/* Little endian bit ordering  */
                if ((channelPtr->RxConnection.MetaDataN_SA !=
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_MIX29_LSB])
                    || (channelPtr->RxConnection.MetaDataN_TA !=
                        CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_MIX29_LSB])
                #endif
                )
                {
                    result = E_NOT_OK;
                }
            }
            else
            {
                result = E_NOT_OK;
            }
            break;
        }
        #endif
        default:
            /*idle*/
            break;
    }
    return result;
}
#endif

/*Construct Flow Control frame according to the parameter.*/
static FUNC(PduLengthType, CANTP_CODE)
CanTp_ConstructFCPci(
    uint8 ChannelIdx,
    CanTp_FCInfoType CanTp_FC
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)))
    ,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Metadata
#endif
    )
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    PduLengthType fcFrameLen = 0u;
    uint8 chnIdx;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.NSduId, &chnIdx);

    if (CANTP_STANDARD == rxSduCfgPtr->AddressingFormat)
    {
        /*STANDARD SDU*/
        CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_PCI_OFFSET_STD] =
                CANTP_FTYPE_FC | CanTp_FC.FcFs;
        CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_BS_STD] = CanTp_FC.FcBS;
        CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_STMIN_STD] = (uint8)(uint8)CanTp_FC.FcSTMin;
        fcFrameLen = CANTP_FC_LEN_STD;
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == rxSduCfgPtr->AddressingFormat)
    {
        /*EXTENDED SDU*/
        /*first byte OEM dependent*/
        CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_PCI_OFFSET_EX] =
                CANTP_FTYPE_FC | CanTp_FC.FcFs;
        CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_BS_EX] = CanTp_FC.FcBS;
        CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_STMIN_EX] = (uint8)CanTp_FC.FcSTMin;
        fcFrameLen = CANTP_FC_LEN_EX;
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == rxSduCfgPtr->AddressingFormat)
	{
		/*normal fixed SDU*/
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_PCI_OFFSET_NF] =
		        CANTP_FTYPE_FC | CanTp_FC.FcFs;
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_BS_NF] = CanTp_FC.FcBS;
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_STMIN_NF] = (uint8)CanTp_FC.FcSTMin;
		fcFrameLen = CANTP_FC_LEN_NF;
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == rxSduCfgPtr->AddressingFormat)
	{
		/*mix11 SDU*/
		/*first byte OEM dependent*/
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_PCI_OFFSET_MIX11] =
		        CANTP_FTYPE_FC | CanTp_FC.FcFs;
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_BS_MIX11] = CanTp_FC.FcBS;
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_STMIN_MIX11] = (uint8)CanTp_FC.FcSTMin;
		fcFrameLen = CANTP_FC_LEN_MIX11;
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == rxSduCfgPtr->AddressingFormat)
	{
		/*mix29 SDU*/
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_PCI_OFFSET_MIX29] =
		        CANTP_FTYPE_FC | CanTp_FC.FcFs;
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_BS_MIX29] = CanTp_FC.FcBS;
		CanTp_Channels[ChannelIdx].RxConnection.FcBuf[CANTP_FC_OFFSET_STMIN_MIX29] = (uint8)CanTp_FC.FcSTMin;
		fcFrameLen = CANTP_FC_LEN_MIX29;
	}
	#endif
	else
	{}

    CanTp_Channels[ChannelIdx].RxConnection.LocalDataPosition = fcFrameLen;
    if (rxSduCfgPtr->CanFrameType == CAN20)
    {
        /*SWS_CanTp_00347*/
        if (CANTP_PADDING_ON == rxSduCfgPtr->RxPaddingActivation)
        {
            CanTp_MemorySet(
                &CanTp_Channels[ChannelIdx].RxConnection.FcBuf[fcFrameLen],
                CANTP_PADDING_BYTE,
                CANTP_CAN20_FRAME_LEN_MAX - fcFrameLen);
            fcFrameLen = CANTP_CAN20_FRAME_LEN_MAX;
        }
    }
    else
    {
        CanTp_MemorySet(
            &CanTp_Channels[ChannelIdx].RxConnection.FcBuf[fcFrameLen],
            CANTP_PADDING_BYTE,
            (8u - fcFrameLen));
        fcFrameLen = 8u;
    }

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    CanTp_ConstructFCMetaData(ChannelIdx, rxSduCfgPtr
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
    		,Metadata
#endif
			);
#endif

    /*return the FC frame length contain meta data length if it have meta data*/
    return fcFrameLen;
}

/*Compare Buffer With Bs*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CompareBufferWithBs(uint8 ChannelIdx)
{
    PduLengthType blockDataLen = 0xFFFFu;
    uint8 chnIdx;
    Std_ReturnType rslt;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.NSduId, &chnIdx);
	if (CANTP_STANDARD == rxSduCfgPtr->AddressingFormat)
	{/*CF composed of this block,CAN2.0 and CANFD is the same format,
	just different with length*/
		if (CAN20 == rxSduCfgPtr->CanFrameType)
		{
			blockDataLen =
					(PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs *
					CANTP_CF_DATALEN_STD;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*CAN_FD*/
			blockDataLen =
					(PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs
					* (CanTp_Channels[ChannelIdx].RxConnection.FDDlc - CANTP_CF_DATA_OFFSET_STD);
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_EXTENDED == rxSduCfgPtr->AddressingFormat)
	{/*CF composed of this block, CAN2.0 and CANFD is the same format, just different with length*/
		if (CAN20 == rxSduCfgPtr->CanFrameType)
		{
			blockDataLen =
					(PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs *
					CANTP_CF_DATALEN_EX;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*CAN_FD*/
			blockDataLen =
                    (PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs
                    * (CanTp_Channels[ChannelIdx].RxConnection.FDDlc - CANTP_CF_DATA_OFFSET_EX);
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == rxSduCfgPtr->AddressingFormat)
	{/*CF composed of this block, CAN2.0 and CANFD is the same format, just different with length*/
		if (CAN20 == rxSduCfgPtr->CanFrameType)
		{
			blockDataLen =
					(PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs *
					CANTP_CF_DATALEN_NF;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*CAN_FD*/
			blockDataLen =
                    (PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs
                    * (CanTp_Channels[ChannelIdx].RxConnection.FDDlc - CANTP_CF_DATA_OFFSET_NF);
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == rxSduCfgPtr->AddressingFormat)
	{/*CF composed of this block, CAN2.0 and CANFD is the same format, just different with length*/
		if (CAN20 == rxSduCfgPtr->CanFrameType)
		{
			blockDataLen =
					(PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs *
					CANTP_CF_DATALEN_MIX11;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*CAN_FD*/
			blockDataLen =
                    (PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs
                    * (CanTp_Channels[ChannelIdx].RxConnection.FDDlc - CANTP_CF_DATA_OFFSET_MIX11);
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == rxSduCfgPtr->AddressingFormat)
	{/*CF composed of this block, CAN2.0 and CANFD is the same format, just different with length*/
		if (CAN20 == rxSduCfgPtr->CanFrameType)
		{
			blockDataLen =
					(PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs *
					CANTP_CF_DATALEN_MIX29;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*CAN_FD*/
			blockDataLen =
                    (PduLengthType)CanTp_Channels[ChannelIdx].RxConnection.CurrentBs
                    * (CanTp_Channels[ChannelIdx].RxConnection.FDDlc - CANTP_CF_DATA_OFFSET_MIX29);
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	else
	{}

	if (CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining > blockDataLen)
	{
		rslt = E_OK;
	}
	else
	{
	    /*TODO:added for Limit buffer,just test*/
	    if (CanTp_Channels[ChannelIdx].RxConnection.UpperBufRemaining >=
	            CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining)
	    {
	        rslt = E_OK;
	    }
	    else
	    {
	        rslt = E_NOT_OK;
	    }
	}
	return rslt;
}

/*Calculate the BS value according to the buffer length and remain data length.*/
static FUNC(void, CANTP_CODE)
CanTp_CalcBS(uint8 ChannelIdx)
{
    PduLengthType blockDataLen;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    uint8 bs;
    uint8 chnIdx;

    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.NSduId, &chnIdx);
    /*check changParameter of BS is done or not*/
    if ((CanTp_Channels[ChannelIdx].RxConnection.EventFlags &
            CANTP_CHANNEL_EVENT_RXBSCHANGED) != 0u)
    {
    	bs = CanTp_Channels[ChannelIdx].RxConnection.ChangedBs;
    }
    else
    {
    	/*bs is not changed by api*/
    	if (rxSduCfgPtr->Bs != NULL_PTR)
    	{
    		bs = *(rxSduCfgPtr->Bs);
    	}
    	else
    	{
    		bs = 0xFF;
    	}
    }

    if(0u == bs)
    {
        /*BS is 0, no more FC need from now on*/
        CanTp_Channels[ChannelIdx].RxConnection.CurrentBs = 0;
    }
    else
    {
        /*get the data length of this block using
         * the minimum value between PDUR buffer length and
         * SDU remain data length. If the minimum
         * value is zero, I.E. the PDUR remain buffer is 0, use 1
         * as the BS*/
        blockDataLen = CanTp_Channels[ChannelIdx].RxConnection.SduDataRemaining;

        if (0u == blockDataLen)
        {
            /*no more upper buffer space left */
            CanTp_Channels[ChannelIdx].RxConnection.CurrentBs = 1U;
        }
        else
        {
            if (CANTP_STANDARD == rxSduCfgPtr->AddressingFormat)
            {
            	/*Standard SDU*/
            	/*calculate CF block,CAN2.0 and CANFD is the same format,
            	 * just different with length*/
            	if (CAN20 == rxSduCfgPtr->CanFrameType)
            	{
            		CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
            				(uint8)((blockDataLen + CANTP_CF_DATALEN_STD) /
            				        CANTP_CF_DATALEN_STD);
            	}
				#if (STD_ON == CANTP_FD)
            	else
            	{
            		/*CAN_FD*/
            		CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
            				(uint8)((blockDataLen + CanTp_Channels[ChannelIdx].RxConnection.FDDlc
            				        - CANTP_CF_DATA_OFFSET_STD)
            				        / CanTp_Channels[ChannelIdx].RxConnection.FDDlc);
            	}
				#endif /*ENDOF #if (STD_ON == CANTP_FD)*/
            }
			#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            else if (CANTP_EXTENDED == rxSduCfgPtr->AddressingFormat)
            {
            	/*extended SDU*/
            	/*calculate CF block,CAN2.0 and CANFD is the same format,
            	 * just different with length*/
            	if (CAN20 == rxSduCfgPtr->CanFrameType)
            	{
            		CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
            				(uint8)((blockDataLen + CANTP_CF_DATALEN_EX) /
            				        CANTP_CF_DATALEN_EX);
            	}
				#if (STD_ON == CANTP_FD)
            	else
            	{
            		/*CAN_FD*/
                    CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
                            (uint8)((blockDataLen + CanTp_Channels[ChannelIdx].RxConnection.FDDlc
                                    - CANTP_CF_DATA_OFFSET_EX)
                                    / CanTp_Channels[ChannelIdx].RxConnection.FDDlc);
            	}
				#endif /*ENDOF #if (STD_ON == CANTP_FD)*/
            }
			#endif
			#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
			else if (CANTP_NORMALFIXED == rxSduCfgPtr->AddressingFormat)
			{
				/*extended SDU*/
				/*calculate CF block,CAN2.0 and CANFD is the same format,
				 * just different with length*/
				if (CAN20 == rxSduCfgPtr->CanFrameType)
				{
					CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
							(uint8)((blockDataLen + CANTP_CF_DATALEN_NF) /
							        CANTP_CF_DATALEN_NF);
				}
				#if (STD_ON == CANTP_FD)
				else
				{
					/*CAN_FD*/
					CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
                            (uint8)((blockDataLen + CanTp_Channels[ChannelIdx].RxConnection.FDDlc
                                    - CANTP_CF_DATA_OFFSET_NF)
                                    / CanTp_Channels[ChannelIdx].RxConnection.FDDlc);
				}
				#endif /*ENDOF #if (STD_ON == CANTP_FD)*/
			}
			#endif
			#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || \
            (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
			else if ((CANTP_MIXED == rxSduCfgPtr->AddressingFormat) ||
			        (CANTP_MIXED29BIT == rxSduCfgPtr->AddressingFormat))
			{
				/*extended SDU*/
				/*calculate CF block,CAN2.0 and CANFD is the same format,
				 * just different with length*/
				if (CAN20 == rxSduCfgPtr->CanFrameType)
				{
					CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
							(uint8)((blockDataLen + CANTP_CF_DATALEN_MIX11) /
							        CANTP_CF_DATALEN_MIX11);
				}
				#if (STD_ON == CANTP_FD)
				else
				{
					/*CAN_FD*/
					CanTp_Channels[ChannelIdx].RxConnection.CurrentBs =
                            (uint8)((blockDataLen + CanTp_Channels[ChannelIdx].RxConnection.FDDlc
                                    - CANTP_CF_DATA_OFFSET_MIX11)
                                    / CanTp_Channels[ChannelIdx].RxConnection.FDDlc);
				}
				#endif /*ENDOF #if (STD_ON == CANTP_FD)*/
			}
			#endif
			else
			{}

            /*use the minimum value between calculated and the configured maximum value*/
            if (CanTp_Channels[ChannelIdx].RxConnection.CurrentBs > bs)
            {
                CanTp_Channels[ChannelIdx].RxConnection.CurrentBs = bs;
            }
        }
    } /*END OF BS needed*/
}

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/*check Rx ChangeParameter*/
FUNC(boolean, CANTP_CODE)
CanTP_CheckRxChangeParameter(
		TPParameterType canTpParam,
		uint16 value)
{
	boolean paramValid = FALSE;

	if (((TP_BS == canTpParam)
			&&(value < 256u))
		||((TP_STMIN == canTpParam)
			&&((value > 0x0u) && (value < 0xFFu))))
	{
		paramValid = TRUE;
	}

	return paramValid;
}
#endif

/*Start a block by sending FC_CTS if needed.*/
static FUNC(Std_ReturnType , CANTP_CODE)
CanTp_RxBlockStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;
    Std_ReturnType  rslt;
    uint8 stMin;
    uint8 chnIdx;
    uint32 N_Ar;
    CanTp_FCInfoType CanTp_FC;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[4];

    pduInfo.MetaDataPtr = Metadata;
#endif
    rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(
            CanTp_Channels[ChannelIdx].RxConnection.NSduId, &chnIdx);

    /*check changParameter of STmin is done or not*/
	if ((CanTp_Channels[ChannelIdx].RxConnection.EventFlags &
	        CANTP_CHANNEL_EVENT_RXSTMINCHANGED) != 0u)
	{
		stMin = CanTp_Channels[ChannelIdx].RxConnection.ChangedSTmin;
	}
	else
	{
		/*stmin is not changed by api*/
		if (rxSduCfgPtr->STmin != NULL_PTR)
		{
			stMin = *(rxSduCfgPtr->STmin);
		}
		else
		{
			stMin = 0;
		}
	}
    /*send FC_CTS*/
    CanTp_FC.FcFs = CANTP_FC_FS_CTS;
    CanTp_FC.FcBS = CanTp_Channels[ChannelIdx].RxConnection.CurrentBs;
    CanTp_FC.FcSTMin = stMin;
    pduInfo.SduLength = CanTp_ConstructFCPci(ChannelIdx,CanTp_FC
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)))
											,
											pduInfo.MetaDataPtr
#endif
											);
    pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].RxConnection.FcBuf;
      
    CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_LARGE_V4FCCTS_CFM;
    rslt = CanIf_Transmit(rxSduCfgPtr->TxFcLPduId,  &pduInfo);
    /*transit to waiting for confirmation of FC_CTS*/
    CanTp_Channels[ChannelIdx].RxConnection.Substate = CANTP_RX_LARGE_V4FCCTS_CFM;
    if (rxSduCfgPtr->Nar != NULL_PTR)
    {
    	N_Ar = *(rxSduCfgPtr->Nar);
        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_NA;
        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime = N_Ar;
        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.StartTime));
    }
    else
    {
    	CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
    }

    CanTp_Channels[ChannelIdx].RxConnection.SentWftCount = 0;
    CanTp_Channels[ChannelIdx].RxConnection.HandledCfCount = 0;

    return rslt;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
