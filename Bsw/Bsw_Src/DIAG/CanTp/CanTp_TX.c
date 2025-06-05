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
**  FILENAME    : CanTp_TX.c                                                 **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Tx process                                                 **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*=======[I N C L U D E S]====================================================*/
#include "CanTp.h"
#include "CanTp_Internal.h"

/*=======[M A C R O S]========================================================*/
#define DLC_NUM 8u
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/*construct FF PCI information in local buffer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructFFPci(uint8 ChannelIdx);

/*construct SF PCI information in local buffer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructSFPci(uint8 ChannelIdx);

/*construct CF PCI information in local buffer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructCFPci(uint8 ChannelIdx);

/*Get the information of received FC frame.*/
static FUNC(void, CANTP_CODE)
CanTp_GetChannelFCInfo(
    uint8 ChannelIdx,
    P2VAR(CanTp_FCInfoType, AUTOMATIC, AUTOMATIC) FcInfoPtr);

#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/*Get TX-SDU configuration by TX-NSduId.*/
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxSduId(PduIdType TxSduId, uint8 *channelId)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    P2CONST(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) channelPtr;
    uint8 sduIdx;
    uint8 chnIdx;

    for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CANTP_CHANNEL_NUMBER); chnIdx++)
    {
    	channelPtr = &CanTp_CfgData.CanTpChannel[chnIdx];
    	for (sduIdx = 0; (NULL_PTR == result) && (sduIdx < channelPtr->TxNsduNum); sduIdx++)
    	{
    		if (TxSduId == channelPtr->TxNSdus[sduIdx].TxNSduId)
    		{
    			result = &channelPtr->TxNSdus[sduIdx];
    			*channelId = chnIdx;
    		}
    	}
    }

    return result;
}

#if (STD_ON == CANTP_FD)
/*
 * Brief               match CanFd Data length.
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
CanTp_MatchDLC(PduLengthType frameDL)
{
	uint8 i;
	PduLengthType len = 0;

	/*ISO 11898-1:2014 define the DLC table*/
	const uint8 canDL[DLC_NUM] = {8, 12, 16, 20, 24, 32, 48, 64};

	for (i = 0; i < DLC_NUM; i++)
	{
		if (frameDL <= canDL[i])
		{
			len = canDL[i];
			break;
		}
	}
	/**/
	return len;
}
#endif
/*
 * Brief               Padding By Frame.
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
static FUNC(PduLengthType, CANTP_CODE)
CanTp_PaddingByFrameDL(uint8 ChannelIdx, PduLengthType frameDL)
{
	PduLengthType position;

	position = CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition +
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataLen;
	CanTp_MemorySet(
		&CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[position],
		CANTP_PADDING_BYTE,
		frameDL - position);

	return frameDL;
}
/*
 * Brief               Tx CF Padding.
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
CanTp_TxCFPadding(
	uint8 ChannelIdx,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr,
	P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr
	)
{
    PduLengthType frameDL;

	/*STmin timeout, send the CF immediately*/
	if (CAN20 == txSduCfgPtr->CanFrameType)
	{
        if (CANTP_PADDING_ON == txSduCfgPtr->TxPaddingActivation)
        {
            if (0u == CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining)
            {
                frameDL = CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength;
                pduInfoPtr->SduLength = CanTp_PaddingByFrameDL(ChannelIdx, frameDL);
            }
        }
	}
	#if (STD_ON == CANTP_FD)
	else
	{

        if (0u == CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining)
        {
            frameDL = CanTp_MatchDLC(pduInfoPtr->SduLength);
            pduInfoPtr->SduLength = CanTp_PaddingByFrameDL(ChannelIdx, frameDL);
        }
	}
	#endif
}
/*
 * Brief               Tx SF Padding.
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
CanTp_TxSFPadding(
	uint8 ChannelIdx,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr,
	P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr
	)
{
    PduLengthType frameDL;
#if (STD_ON == CANTP_FD)
    PduLengthType can20DataFieldLen = 0;
    PduLengthType canFdDataOffset = 0;


	switch (txSduCfgPtr->AddressingFormat)
	{
		case CANTP_STANDARD:
		{
			can20DataFieldLen = CANTP_SF_LEN_MAX_STD;
			canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_STD;
			break;
		}
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_EXTENDED:
		{
			can20DataFieldLen = CANTP_SF_LEN_MAX_EX;
			canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_EX;
			break;
		}
		#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_NORMALFIXED:
		{
			can20DataFieldLen = CANTP_SF_LEN_MAX_NF;
			canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_NF;
			break;
		}
		#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED:
		{
			can20DataFieldLen = CANTP_SF_LEN_MAX_MIX11;
			canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_MIX11;
			break;
		}
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED29BIT:
		{
			can20DataFieldLen = CANTP_SF_LEN_MAX_MIX29;
			canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_MIX29;
			break;
		}
		#endif
		default:
		    /*idle*/
			break;
	}

	if ((CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount > can20DataFieldLen)
		&&( CAN_FD == txSduCfgPtr->CanFrameType))
	{
		/*CANFD frame length >8*/
		frameDL = CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount + canFdDataOffset;
		/*frameDL here is less than or equal to the DLC which configuration in the CANFI module*/
		frameDL = CanTp_MatchDLC(frameDL);
		pduInfoPtr->SduLength = CanTp_PaddingByFrameDL(ChannelIdx, frameDL);
	}
	else
#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	{
        if (CANTP_PADDING_ON == txSduCfgPtr->TxPaddingActivation)
        {
            /*CAN2.0 or small CAN FD*/
            frameDL = CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength;
            pduInfoPtr->SduLength = CanTp_PaddingByFrameDL(ChannelIdx, frameDL);
        }
	}
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               store Tx meta data info.
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
FUNC(void, CANTP_CODE)CanTp_SaveTxMetaDataInfo(
	uint8 channelId,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
	)
{
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
		|| (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
	P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

	channelPtr = &CanTp_Channels[channelId];
#endif
	switch (TxNSduCfgPtr->AddressingFormat)
	{
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_EXTENDED:
            #if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
            channelPtr->TxConnection.MetaDataN_TA =
                    #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_MSB];
                    #else/* Little endian bit ordering  */
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_LSB];
                    #endif
            #else
                    channelPtr->TxConnection.MetaDataN_TA = *(TxNSduCfgPtr->NTa);
            #endif
			break;
		#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED:
            #if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
            channelPtr->TxConnection.MetaDataN_AE = CanTpTxInfoPtr->MetaDataPtr[CANTP_AE_FOR_DCM];
            #else
                    channelPtr->TxConnection.MetaDataN_AE = *(TxNSduCfgPtr->NAe);
            #endif
			break;
		#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_NORMALFIXED:
            #if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
            #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
		    channelPtr->TxConnection.MetaDataN_SA =
		            CanTpTxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_MSB];
            channelPtr->TxConnection.MetaDataN_TA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_MSB];
            #else/* Little endian bit ordering  */
            channelPtr->TxConnection.MetaDataN_SA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_LSB];
            channelPtr->TxConnection.MetaDataN_TA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_LSB];
            #endif
            #else
            channelPtr->TxConnection.MetaDataN_SA = *(TxNSduCfgPtr->NSa);
            channelPtr->TxConnection.MetaDataN_TA = *(TxNSduCfgPtr->NTa);
            #endif
			break;
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED29BIT:
            #if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
            #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
            channelPtr->TxConnection.MetaDataN_SA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_MSB];
            channelPtr->TxConnection.MetaDataN_TA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_MSB];
            #else/* Little endian bit ordering  */
            channelPtr->TxConnection.MetaDataN_SA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_SA_FOR_DCM_LSB];
            channelPtr->TxConnection.MetaDataN_TA =
                    CanTpTxInfoPtr->MetaDataPtr[CANTP_TA_FOR_DCM_LSB];
            #endif
            channelPtr->TxConnection.MetaDataN_AE = CanTpTxInfoPtr->MetaDataPtr[CANTP_AE_FOR_DCM];
            #else
            channelPtr->TxConnection.MetaDataN_SA = *(TxNSduCfgPtr->NSa);
            channelPtr->TxConnection.MetaDataN_TA = *(TxNSduCfgPtr->NTa);
            channelPtr->TxConnection.MetaDataN_AE = *(TxNSduCfgPtr->NAe);
            #endif
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
 * Brief               Construct Tx meta data info.
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
CanTp_ConstructTxMetaDataInfo(
	uint8 channelId,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr
	)
{
	switch (TxNSduCfgPtr->AddressingFormat)
	{
		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_EXTENDED:
		    pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] =
		            CanTp_Channels[channelId].TxConnection.MetaDataN_TA;
			break;
		#endif
		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED:
            pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] =
                    CanTp_Channels[channelId].TxConnection.MetaDataN_AE;
			break;
		#endif
		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_NORMALFIXED:
            if ((CanTp_Channels[channelId].TxConnection.MetaDataLength >= 4u)
                && (CANTP_CAN_ID_32_MASK == (TxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_MSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_SA;
                pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_MSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_TA;
                #else/* Little endian bit ordering  */
                pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_LSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_SA;
                pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_LSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_TA;
                #endif
            }
			break;
		#endif
		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
		case CANTP_MIXED29BIT:
            if ((CanTp_Channels[channelId].TxConnection.MetaDataLength >= 4u)
                && (CANTP_CAN_ID_32_MASK == (TxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
            {
                #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_MSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_SA;
                pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_MSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_TA;
                #else/* Little endian bit ordering  */
                pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_LSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_SA;
                pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_LSB] =
                        CanTp_Channels[channelId].TxConnection.MetaDataN_TA;
                #endif
            }
            pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] =
                    CanTp_Channels[channelId].TxConnection.MetaDataN_AE;
			break;
		#endif
		default:
            /*idle*/
			break;
	}
}
/*
 * Brief               check RxFc meta data.
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
FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckRxFCMetaData(
	uint8 channelIdx,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxInfoPtr
	)
{
	   Std_ReturnType result = E_OK;
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
	|| (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
	|| (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
	|| (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
	   P2CONST(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;
	   
	   channelPtr = &CanTp_Channels[channelIdx];
#endif
	   switch (TxNSduCfgPtr->AddressingFormat)
	   	{
	   		#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
	   		case CANTP_EXTENDED:
	   			if (channelPtr->TxConnection.MetaDataN_SA !=
	   			        CanTpRxInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX])
	   			{
	   				result = E_NOT_OK;
	   			}
	   			break;
	   		#endif
	   		#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	   		case CANTP_MIXED:
	   			if (channelPtr->TxConnection.MetaDataN_AE !=
	   			        CanTpRxInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
				{
					result = E_NOT_OK;
				}
	   			break;
	   		#endif
	   		#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	   		case CANTP_NORMALFIXED:
	            if ((channelPtr->TxConnection.MetaDataLength >= 4u)
	                && (CANTP_CAN_ID_32_MASK ==
	                        (TxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
	            {
	                if (
                    #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
	                    (channelPtr->TxConnection.MetaDataN_SA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_MSB])
                            || (channelPtr->TxConnection.MetaDataN_TA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_MSB])
                    #else/* Little endian bit ordering  */
                    (channelPtr->TxConnection.MetaDataN_SA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_LSB])
                            || (channelPtr->TxConnection.MetaDataN_TA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_LSB])
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
	   		#endif
	   		#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	   		case CANTP_MIXED29BIT:
                if ((channelPtr->TxConnection.MetaDataLength >= 4u) &&
                    (CANTP_CAN_ID_32_MASK == (TxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
                {
                    if (
                    #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* Big endian bit ordering*/
                        (channelPtr->TxConnection.MetaDataN_SA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_MSB])
                            || (channelPtr->TxConnection.MetaDataN_TA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_MSB])
                    #else/* Little endian bit ordering  */
                    (channelPtr->TxConnection.MetaDataN_SA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_NF_LSB])
                            || (channelPtr->TxConnection.MetaDataN_TA !=
                            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_NF_LSB])
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
                if (channelPtr->TxConnection.MetaDataN_AE !=
                        CanTpRxInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX29])
                {
                    result = E_NOT_OK;
                }
	   			break;
	   		#endif
	   		default:
	            /*idle*/
	   			break;
	   	}
	   return result;
}
#endif
/*
 * Brief               check Tx CanFD Length.
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
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckTxCanFDLength(
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	PduLengthType sduTotalLength,
	PduLengthType dataFieldLength
	)
{
	Std_ReturnType ret = E_NOT_OK;

	if (CAN_FD == TxNSduCfgPtr->CanFrameType)
	{
		if (dataFieldLength > sduTotalLength)
		{
			/*transmit each sdu length should less than DLC which configuration in Canif module*/
			ret = E_OK;
		}
		else
		{
			ret = E_NOT_OK;
		}
	}
	return ret;
}
/*
 * Brief               check Tx Length In Address Mode.
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
FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckTxLengthInAddressMode(
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
	)
{
	Std_ReturnType ret = E_NOT_OK;
	PduLengthType len;

	switch (TxNSduCfgPtr->TxTaType)
	{
	/*functional addressing mode can only use SF*/
	    case CANTP_FUNCTIONAL_TX:
        switch (TxNSduCfgPtr->AddressingFormat)
        {
            case CANTP_STANDARD:
                if (CANTP_SF_LEN_MAX_STD >= CanTpTxInfoPtr->SduLength)
                {
                    /*CAN2.0 or small CAN FD can send this single frame*/
                    ret = E_OK;
                }
                else
                {
                    /*large CAN FD only*/
                    len = TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_STD;
                    ret = CanTp_CheckTxCanFDLength(TxNSduCfgPtr, CanTpTxInfoPtr->SduLength, len);
                }
                break;
            #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            case CANTP_EXTENDED:
                if (CANTP_SF_LEN_MAX_EX >= CanTpTxInfoPtr->SduLength)
                {
                    /*CAN2.0 or small CAN FD*/
                    ret = E_OK;
                }
                else
                {
                    /*large CAN FD only*/
                    len = TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_EX;
                    ret = CanTp_CheckTxCanFDLength(TxNSduCfgPtr, CanTpTxInfoPtr->SduLength, len);
                }
                break;
            #endif
            #if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
            case CANTP_NORMALFIXED:
                if (CANTP_SF_LEN_MAX_NF >= CanTpTxInfoPtr->SduLength)
                {
                    /*CAN2.0 or small CAN FD*/
                    ret = E_OK;
                }
                else
                {
                    /*large CAN FD only*/
                    len = TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_NF;
                    ret = CanTp_CheckTxCanFDLength(TxNSduCfgPtr, CanTpTxInfoPtr->SduLength, len);
                }
                break;
            #endif
            #if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
            case CANTP_MIXED:
                if (CANTP_SF_LEN_MAX_MIX11 >= CanTpTxInfoPtr->SduLength)
                {
                    /*CAN2.0 or small CAN FD*/
                    ret = E_OK;
                }
                else
                {
                    /*large CAN FD only*/
                    len = TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_MIX11;
                    ret = CanTp_CheckTxCanFDLength(TxNSduCfgPtr, CanTpTxInfoPtr->SduLength, len);
                }
                break;
            #endif
            #if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
            case CANTP_MIXED29BIT:
                if (CANTP_SF_LEN_MAX_MIX29 >= CanTpTxInfoPtr->SduLength)
                {
                    /*CAN2.0 or small CAN FD*/
                    ret = E_OK;
                }
                else
                {
                    /*large CAN FD only*/
                    len = TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_MIX29;
                    ret = CanTp_CheckTxCanFDLength(TxNSduCfgPtr, CanTpTxInfoPtr->SduLength, len);
                }
                break;
            #endif
            default:
                /*idle*/
                break;
        }
        break;
        case CANTP_PHYSICAL_TX:
            ret = E_OK;
            break;
        default:
            /*idle*/
            break;
    }
	return ret;
}
/*
 * Brief               Get TX-SDU configuration by received FC NPDU information.
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
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByFCNpdu(	PduIdType CanTpRxPduId,
    						P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
							P2VAR(uint8, AUTOMATIC, AUTOMATIC) channelIdx)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
    P2CONST(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) channelPtr;
    uint8 txNSduIdx;
    uint8 chnIdx;
    CanTp_FramePCIType frameType;

	for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CANTP_CHANNEL_NUMBER); chnIdx++)
	{
		channelPtr = &CanTp_CfgData.CanTpChannel[chnIdx];
		for (txNSduIdx = 0; (NULL_PTR == result) &&
		(txNSduIdx < channelPtr->TxNsduNum); txNSduIdx++)
		{
			/*only physical TX NSDU need configure RX FCNPDUID*/
			if ((CANTP_PHYSICAL_TX == channelPtr->TxNSdus[txNSduIdx].TxTaType)
			    && (channelPtr->TxNSdus[txNSduIdx].RxFcNPduId == CanTpRxPduId))
			{
				switch (channelPtr->TxNSdus[txNSduIdx].AddressingFormat)
				{
					case CANTP_STANDARD:
                    case CANTP_NORMALFIXED:
						/*check the standard addressing TX-NSDU, only FC accepted*/
						frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] &
						CANTP_FTYPE_MASK;
						if (CANTP_FTYPE_FC == frameType)
						{
							result = &channelPtr->TxNSdus[txNSduIdx];
							*channelIdx = chnIdx;
						}
						break;
					#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || \
						(CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || \
						(CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
					case CANTP_MIXED:
                    case CANTP_MIXED29BIT:
                    case CANTP_EXTENDED:
						if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_MIX11)
						{
							/*check the mix11 addressing TX-NSDU, only FC accepted*/
							frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_MIX11] &
							        CANTP_FTYPE_MASK;
							if (CANTP_FTYPE_FC == frameType)
							{
                                result = &channelPtr->TxNSdus[txNSduIdx];
                                *channelIdx = chnIdx;
							}
						}
						else
						{
							/*configured as extended addressing format, but the
							 * received NPDU not has enough data space*/
						}
						break;
					#endif/*ENDOF #if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)*/
					default:
			            /*idle*/
						break;
				}/*END OF switch*/
			} /*END OF if (CANTP_PHYSICAL == CanTp_CfgData.TxNSdus[txNSduIdx].TxTaType)*/
		}/*END OF for loop*/
	}/*END OF for loop*/

    return result;
}
/*
 * Brief               Get Tx-SDU configuration by Tx NPDUID.
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
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxNPdu(PduIdType CanTpTxPduId, uint8 *channelId)
{
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) result = NULL_PTR;
	P2CONST(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) channelPtr;
	uint8 sduIdx;
	uint8 chnIdx;

	for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CANTP_CHANNEL_NUMBER); chnIdx++)
	{
		channelPtr = &CanTp_CfgData.CanTpChannel[chnIdx];
		for (sduIdx = 0; (NULL_PTR == result) && (sduIdx < channelPtr->TxNsduNum); sduIdx++)
		{
			if (CanTpTxPduId == channelPtr->TxNSdus[sduIdx].TxNPduId)
			{
				result = &channelPtr->TxNSdus[sduIdx];
				*channelId = chnIdx;
			}
		}
	}

	return result;
}
/*
 * Brief               Handle received FC.
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
CanTp_TxHandleFCReception(uint8 ChannelIdx)
{
    CanTp_FCInfoType fcInfo;
    PduIdType nSduId;
    PduIdType iPduId;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    uint32 N_Bs;
    uint32 N_Cs;
    uint8 channelId;

    CanTp_GetChannelFCInfo(ChannelIdx, &fcInfo);
    nSduId = CanTp_Channels[ChannelIdx].TxConnection.NSduId;
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId, &channelId);
    iPduId = txNSduCfgPtr->TxIPduId;

    switch (fcInfo.FcFs)
    {
        case CANTP_FC_FS_CTS:
            /*Clear to send. Get the channel ready to send CF, and make STmin timer timeout*/
            CanTp_ConstructCFPci(ChannelIdx);
            CanTp_Channels[ChannelIdx].TxConnection.CurrentBs = fcInfo.FcBS;
			CanTp_Channels[ChannelIdx].TxConnection.HandledCfCount = 0;
            CanTp_Channels[ChannelIdx].TxConnection.STminTimer.Started = TRUE;
            CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime = fcInfo.FcSTMin;
            CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.STminTimer.StartTime));
            CanTp_Channels[ChannelIdx].TxConnection.STminTimer.FcSTMin = fcInfo.FcSTMin;
            /*transit to state SENDCF*/
            if (txNSduCfgPtr->Ncs != NULL_PTR)
            {
            	N_Cs = *(txNSduCfgPtr->Ncs);
                CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_NC;
                CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = N_Cs;
                CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
            }
            else
            {
            	CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
            	        CANTP_TIMER_NONE;
            }

            CanTp_Channels[ChannelIdx].TxConnection.HandleType = CANTP_FTYPE_CF;
            CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_START;
            break;
        case CANTP_FC_FS_WT:
            /*reset the BS timer*/
            if (txNSduCfgPtr->Nbs != NULL_PTR)
            {
            	N_Bs = *(txNSduCfgPtr->Nbs);
                CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_NB;
                CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = N_Bs;
                CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
            }
            else
            {
            	 CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
            	         CANTP_TIMER_NONE;
            }
            break;
        case CANTP_FC_FS_OVFLW:
        default:
            /*release resource of channel*/
        	CanTp_ReleaseTxChannel(ChannelIdx);
            /*notify upper with NO_BUFFER*/
            PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
            break;
    } /*ENDOF switch (fcInfo.FcFs)*/

    /*clear event flag*/
    CanTp_Channels[ChannelIdx].TxConnection.EventFlags =
    		CanTp_Channels[ChannelIdx].TxConnection.EventFlags &
    		((uint8)~CANTP_CHANNEL_EVENT_FC_RECEIVED);
}
/*
 * Brief               Handle transmit request.
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
CanTp_TxHandleTransmitReq(uint8 ChannelIdx)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    PduLengthType len = 0;
    uint8 channelId;
    uint32 N_Cs;

    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(
            CanTp_Channels[ChannelIdx].TxConnection.NSduId, &channelId);

    if (CAN20 == txNSduCfgPtr->CanFrameType)
    {
    	/*Can2.0 or small Can Fd*/
		switch (txNSduCfgPtr->AddressingFormat)
		{
			case CANTP_STANDARD:
            case CANTP_NORMALFIXED:
				len = txNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_STD;
				break;
			#if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || \
			        (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || \
			        (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
			case CANTP_MIXED:
            case CANTP_MIXED29BIT:
            case CANTP_EXTENDED:
				len = txNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_MIX11;
				break;
			#endif
			default:
	            /*idle*/
				break;
		}

    	if (len < CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount)
		{
			/*larger SDU transmit request*/
			CanTp_Channels[ChannelIdx].TxConnection.HandleType = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.RootState = CANTP_LARGE_TRANSMITTING;
			CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_START;
		}
		else
		{
			/*SF transmit request*/
			CanTp_Channels[ChannelIdx].TxConnection.HandleType  = CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.RootState = CANTP_SF_TRANSIMITTING;
			CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_SF_START;
		}
    }
	#if (STD_ON == CANTP_FD)
    else
    {
    	/*CANFD*/
    	switch (txNSduCfgPtr->AddressingFormat)
		{
			case CANTP_STANDARD:
            case CANTP_NORMALFIXED:
				/*small Can FD data offset same as Can 2.0,
				 * CANTP_SF_DATA_OFFSET_STD,small Can FD data field length
				 * less than large Can FD, so len = bigger one*/
			    if (CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount > 7u)
			    {
			        len = txNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_STD;
			    }
			    else
			    {
                    len = txNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_STD;
			    }
				break;
			case CANTP_EXTENDED:
            case CANTP_MIXED29BIT:
            case CANTP_MIXED:
                if (CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount > 6u)
                {
                    len = txNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_EX;
                }
                else
                {
                    len = txNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_MIX11;
                }
                break;
			default:
			    /*idle*/
				break;
		}

    	if (len < CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount)
		{
			/*larger SDU transmit request*/
			CanTp_Channels[ChannelIdx].TxConnection.HandleType = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.RootState = CANTP_LARGE_TRANSMITTING;
			CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_START;
		}
		else
		{
			/*SF transmit request*/
			CanTp_Channels[ChannelIdx].TxConnection.HandleType  = CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.RootState = CANTP_SF_TRANSIMITTING;
			CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_SF_START;
		}
    }
	#endif /*ENDOF #if (STD_ON == CANTP_FD)*/

    /*start CS timer*/
    if (txNSduCfgPtr->Ncs != NULL_PTR)
    {
    	N_Cs = *(txNSduCfgPtr->Ncs);
        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_NC;
        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = N_Cs;
        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
    }
    else
    {
    	CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
    }
    /*construct PCI information*/
    if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].TxConnection.HandleType)
    {
        /*construct FF PCI information*/
        CanTp_ConstructFFPci(ChannelIdx);
    }
    else
    {
        /*construct SF PCI information*/
        CanTp_ConstructSFPci(ChannelIdx);
    }

    /*clear event flag*/
    CanTp_Channels[ChannelIdx].TxConnection.EventFlags = 0;
}

/*Handle transmit cancel request event.*/
FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCancel(uint8 ChannelIdx)
{
    PduIdType sduId;
    PduIdType iPduId;
    uint8 channelId;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;

    sduId = CanTp_Channels[ChannelIdx].TxConnection.NSduId;
	txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(sduId, &channelId);
    iPduId = txSduCfgPtr->TxIPduId;

    /*Release the resource related with the channel*/
    CanTp_ReleaseTxChannel(ChannelIdx);
    /*Notify the upper*/
    PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
}

/*Handle confirmation for data transmit.*/
FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCfm(uint8 ChannelIdx)
{
    PduIdType nSduId;
    PduIdType iPduId;
    uint8 channelId;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;
    uint32 N_Bs;
    uint32 N_Cs;

    nSduId = CanTp_Channels[ChannelIdx].TxConnection.NSduId;
    txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId, &channelId);
    iPduId = txSduCfgPtr->TxIPduId;

    if (CANTP_TX_SF_V4SF_CFM == CanTp_Channels[ChannelIdx].TxConnection.Substate)
    {
        /*Confirmation for SF data transmit. The end of the SF transmitting*/
        /*Release the channel resource*/
    	CanTp_ReleaseTxChannel(ChannelIdx);
        /*Notify the upper*/
        PduR_CanTpTxConfirmation(iPduId, E_OK);
    }
    else /*Confirmation for large SDU data transmit*/
    {
        /*update next SN number*/
        CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn =
        		(uint8)(CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn + 1u) &
        		CANTP_CF_SN_MASK;

        if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].TxConnection.HandleType)
        {
            /*confirmation for FF, transit to state V4FC and start BS timer*/
            CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_V4FC;
            if (txSduCfgPtr->Nbs != NULL_PTR)
            {
            	N_Bs = *(txSduCfgPtr->Nbs);
                CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_NB;
                CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = N_Bs;
                CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
            }
            else
            {
            	CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
            	        CANTP_TIMER_NONE;
            }
        }
        else /*confirmation for CF*/
        {
            if (0u == CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining)
            {
                /*SDU transmitting finished*/
                /*release resource of channel*/
            	CanTp_ReleaseTxChannel(ChannelIdx);
                /*notify the upper*/
                PduR_CanTpTxConfirmation(iPduId, E_OK);
            }
            else /*whole SDU transmitting not finished*/
            {
                /*update handled CF count*/
                if (0xFFu > CanTp_Channels[ChannelIdx].TxConnection.HandledCfCount)
                {
                    CanTp_Channels[ChannelIdx].TxConnection.HandledCfCount++;
                }

                if (CanTp_Channels[ChannelIdx].TxConnection.HandledCfCount ==
                        CanTp_Channels[ChannelIdx].TxConnection.CurrentBs)
                {
                    /*current block finished*/
                    /*wait another FC*/
                    CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_V4FC;
                    if (txSduCfgPtr->Nbs != NULL_PTR)
                    {
                    	N_Bs = *(txSduCfgPtr->Nbs);
                        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
                                CANTP_NB;
                        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = N_Bs;
                        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
                    }
                    else
                    {
                    	CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
                    	        CANTP_TIMER_NONE;
                    }
                }
                else /*current block not finished yet*/
                {
                    /*construct the next CF PCI, and transit to SENDCF, start STmin and CS*/
                    CanTp_ConstructCFPci(ChannelIdx);
                    CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_START;
                    if (txSduCfgPtr->Ncs != NULL_PTR)
                    {
                    	N_Cs = *(txSduCfgPtr->Ncs);
                        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
                                CANTP_NC;
                        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = N_Cs;
                        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
                    }
                    else
                    {
                    	CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
                    	        CANTP_TIMER_NONE;
                    }

                    CanTp_Channels[ChannelIdx].TxConnection.STminTimer.Started = TRUE;
                    CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime =
                    		CanTp_Channels[ChannelIdx].TxConnection.STminTimer.FcSTMin;
                    CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.STminTimer.StartTime));
                }
            } /*ENDOF whole SDU transmitting not finished*/
        } /*END OF confirmation for CF*/
    } /*END OF Confirmation for large SDU data transmit*/

    /*clear event flag*/
    CanTp_Channels[ChannelIdx].TxConnection.EventFlags =
    		CanTp_Channels[ChannelIdx].TxConnection.EventFlags &
    		((uint8)~CANTP_CHANNEL_EVENT_TX_CONFIRMED);
}

/*Back ground handling for SF transmitting.*/
FUNC(void, CANTP_CODE)
CanTp_TxHandleSFStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;
    BufReq_ReturnType bufRslt;
    Std_ReturnType rslt;
    PduLengthType position;
    PduIdType nSduId;
    PduIdType iPduId;
    uint8 channelId;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[5];

    pduInfo.MetaDataPtr = Metadata;
#endif
    nSduId = CanTp_Channels[ChannelIdx].TxConnection.NSduId;
	txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId, &channelId);
    iPduId = txSduCfgPtr->TxIPduId;
    position = CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition;
    pduInfo.SduDataPtr = &CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[position];
	if (CAN20 == txSduCfgPtr->CanFrameType)
	{
		/*CAN2.0*/
		if (txSduCfgPtr->TxNPduDLC > CANTP_CAN20_FRAME_LEN_MAX)
		{
			CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength = CANTP_CAN20_FRAME_LEN_MAX;
		}
		else
		{
			CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength = txSduCfgPtr->TxNPduDLC;
		}
	}
	#if (STD_ON == CANTP_FD)
	else
	{
		CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength =
				CanTp_MatchDLC(CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition
				        + CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount);
	}
	#endif
	pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength -
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition;

    /*SF may less than the length of CAN Frame*/
	if (pduInfo.SduLength > CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining)
	{
		pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining;
	}

    /*upperBufRemaining is upper layer data left after copy data*/
    bufRslt = PduR_CanTpCopyTxData(iPduId, pduInfoPtr, NULL_PTR,
            &CanTp_Channels[ChannelIdx].TxConnection.UpperBufRemaining);

    /*Handle according to the request buffer result*/
    switch (bufRslt)
    {
        case BUFREQ_OK:
        {
            /*get length of the data copy*/
        	CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining =
        			CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining - pduInfo.SduLength;

            CanTp_Channels[ChannelIdx].TxConnection.LocalDataLen = pduInfo.SduLength;

            if (0u == CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining)
            {
                /*SF data copy finished, send it*/
                /*construct CANIF transmit request*/
                pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].TxConnection.LocalBuf;
                pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition +
                		CanTp_Channels[ChannelIdx].TxConnection.LocalDataLen;
        		/*pduInfo.SduLength will be set in the api below, if padding*/
                CanTp_TxSFPadding(ChannelIdx,
                        txSduCfgPtr,
                        &pduInfo);
				#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                CanTp_ConstructTxMetaDataInfo(ChannelIdx, txSduCfgPtr, pduInfoPtr);
				#endif

                CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_SF_V4SF_CFM;
                /*Request CANIF to transmit the SF frame*/
                rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, pduInfoPtr);
                if (E_NOT_OK == rslt)
                {
                	 /*Release resources related to the channel*/
                	CanTp_ReleaseTxChannel(ChannelIdx);
                    /*Notify the upper*/
                    PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
                }
                else
                {
                	/*Transit channel to wait for confirmation of SF*/
					CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_SF_V4SF_CFM;
					CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_NA;
					CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime =
					        txSduCfgPtr->Nas;
		            CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
                }
            } /*END OF SF data copy finished*/
            break;
        } /*ENDOF case BUFREQ_OK*/
        case BUFREQ_E_NOT_OK:
        {
            /*Fail to get TX buffer*/
            /*Release resources related to the channel*/
        	CanTp_ReleaseTxChannel(ChannelIdx);
            /*Notify the upper*/
            PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
            break;
        }
        case BUFREQ_E_BUSY:
            /*The upper is busy, no buffer provided. Try to get buffer next round*/
            break;
        default:
            /*invalid return value for PduR_CanTpProvideTxBuffer*/
            break;
    } /*END OF switch*/
}

/* Back ground handling for large SDU transmitting, trying to get TX buffer.*/
FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeStart(uint8 ChannelIdx)
{
    PduInfoType pduInfo;
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr = &pduInfo;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;
    BufReq_ReturnType bufRslt;
    Std_ReturnType rslt;
    PduLengthType position;
    PduIdType nSduId;
    PduIdType iPduId;
    uint8 channelId;
    static VAR(boolean, CANTP_VAR_POWER_ON_INIT) CopyFlg = FALSE;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[5];
    pduInfo.MetaDataPtr = Metadata;
#endif
    nSduId = CanTp_Channels[ChannelIdx].TxConnection.NSduId;
    txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId, &channelId);
    iPduId = txSduCfgPtr->TxIPduId;
    pduInfo.SduLength = 0;
    /*calculate the length which need to be copied*/
    if (CAN20 == txSduCfgPtr->CanFrameType)
	{/*CAN2.0*/
		if (txSduCfgPtr->TxNPduDLC > CANTP_CAN20_FRAME_LEN_MAX)
		{
			CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength = CANTP_CAN20_FRAME_LEN_MAX;
		}
		else
		{
			CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength = txSduCfgPtr->TxNPduDLC;
		}
	}
	#if (STD_ON == CANTP_FD)
	else
	{
		CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength = txSduCfgPtr->TxNPduDLC;
	}
	#endif
	if ((CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].TxConnection.HandleType)
		|| ((CopyFlg == FALSE) &&
		        (CANTP_FTYPE_FF != CanTp_Channels[ChannelIdx].TxConnection.HandleType)))
	{
		pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.CanFrameLength -
				CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition;
		/*the last CF may less than the length of CAN Frame*/
		if (pduInfo.SduLength > CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining)
		{
			pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining;
		}
		position = CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition;
		pduInfo.SduDataPtr = &CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[position];
		bufRslt = PduR_CanTpCopyTxData(iPduId, pduInfoPtr, NULL_PTR,
		        &CanTp_Channels[ChannelIdx].TxConnection.UpperBufRemaining);
	}
	else
	{
		bufRslt = BUFREQ_OK;
	}
    switch (bufRslt)
    {
        case BUFREQ_OK:
        	if ((CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].TxConnection.HandleType)
        	    || ((CopyFlg == FALSE) && (CANTP_FTYPE_FF !=
        	            CanTp_Channels[ChannelIdx].TxConnection.HandleType)))
        	{
				/*update channel runtime information*/
				/*get length of the data copy*/
				CanTp_Channels[ChannelIdx].TxConnection.LocalDataLen = pduInfo.SduLength;
				CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining =
						CanTp_Channels[ChannelIdx].TxConnection.SduDataRemaining -
						pduInfo.SduLength;
        	}
	        /*Set CopyFlg*/
	        CopyFlg = TRUE;
			if (CANTP_FTYPE_FF == CanTp_Channels[ChannelIdx].TxConnection.HandleType)
			{/*send the FF immediately*/
				pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].TxConnection.LocalBuf;
				/*the length include pci info*/
				pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.LocalDataLen +
						CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition;
				#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                CanTp_ConstructTxMetaDataInfo(ChannelIdx, txSduCfgPtr, pduInfoPtr);
				#endif
				CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_V4DATA_CFM;
				rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, pduInfoPtr);
	        	/*Clear CopyFlg*/
	        	CopyFlg = FALSE;
				if (E_NOT_OK == rslt)
				{
					CanTp_ReleaseTxChannel(ChannelIdx);
                    /*Notify the upper*/
                    PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
				}
				else
				{
					/*Transit channel to wait for confirmation of FF*/
					CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_V4DATA_CFM;
					CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer = CANTP_NA;
					CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime =
					        txSduCfgPtr->Nas;
		            CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
				}
			}
			else /*handle the CF transmitting data copy*/
			{
				if ((TRUE == CanTp_Channels[ChannelIdx].TxConnection.STminTimer.Started)
					&& (0u == CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime))
				{
					pduInfo.SduDataPtr = CanTp_Channels[ChannelIdx].TxConnection.LocalBuf;
					pduInfo.SduLength = CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition
					        + CanTp_Channels[ChannelIdx].TxConnection.LocalDataLen;
	        		/*pduInfo.SduLength will be set in the api below*/
                    CanTp_TxCFPadding(ChannelIdx, txSduCfgPtr, pduInfoPtr);

					#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                    CanTp_ConstructTxMetaDataInfo(ChannelIdx, txSduCfgPtr, pduInfoPtr);
					#endif
					CanTp_Channels[ChannelIdx].TxConnection.Substate = CANTP_TX_LARGE_V4DATA_CFM;
					rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, pduInfoPtr);
		        	/*Clear CopyFlg*/
		        	CopyFlg = FALSE;
					if (E_NOT_OK == rslt)
					{
						CanTp_ReleaseTxChannel(ChannelIdx);
	                    /*Notify the upper*/
	                    PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
					}
					else
					{
						/*Transit channel to wait for confirmation of CF*/
						CanTp_Channels[ChannelIdx].TxConnection.Substate =
						        CANTP_TX_LARGE_V4DATA_CFM;
						CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer =
						        CANTP_NA;
						CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime =
						        txSduCfgPtr->Nas;
			            CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));
					}

					CanTp_Channels[ChannelIdx].TxConnection.STminTimer.Started = FALSE;
				}
				else /*STmin not time out*/
				{
					/*transit to state SENDCF, wait for sending next round*/
				}
			} /*END OF handle the CF transmitting data copy*/
            break;
        case BUFREQ_E_NOT_OK:
            /*Fail to get TX buffer*/
            /*Release resources related to the channel*/
        	CanTp_ReleaseTxChannel(ChannelIdx);
            /*Notify the upper*/
            PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
            break;
        case BUFREQ_E_BUSY:
    		if ((CANTP_NC == CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer)
    				&&(0u == CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime))
			{
        		/*STmin timeout, abort send*/
        		/*Release resources related to the channel*/
    			CanTp_ReleaseTxChannel(ChannelIdx);
        		/*Notify the upper*/
				PduR_CanTpTxConfirmation(iPduId, E_NOT_OK);
			}/*The upper is busy, no buffer provided. Try to get buffer next round*/
            break;
        default:/*invalid return value*/
            break;
    } /*END OF switch (bufRslt)*/
}

/*============Internal static Function Implementation*/
/*construct FF PCI information in local buffer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructFFPci(uint8 ChannelIdx)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    uint32 pciDl;
    uint8 channelId;

    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(
            CanTp_Channels[ChannelIdx].TxConnection.NSduId, &channelId);
    pciDl = CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount;
    if (CANTP_STANDARD == txNSduCfgPtr->AddressingFormat)
    {/*standard FF*/
    	if (pciDl <= CANTP_FF_DL_12bit_LENGTH)
    	{/*FF_DL less than or equal to 4095*//*SDU length low nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_STD_FF_FFDL_LOW_OFFSET_LT4095] = (uint8)pciDl;
			/*SDU length high nibble and Frame type*//*update local information*/
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_STD] = CANTP_FTYPE_FF|(uint8)pciDl;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_FF_DATA_OFFSET_LT4095_STD;
    	}
    	else
    	{/*FF_DL bigger than 4095*//*SDU length from low to high nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_STD_FF_FFDL_LOW_OFFSET_BT4095] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_STD_FF_FFDL_LOW_OFFSET_BT4095 - 1u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_STD_FF_FFDL_LOW_OFFSET_BT4095 - 2u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_STD_FF_FFDL_LOW_OFFSET_BT4095 - 3u] = (uint8)pciDl;
			/*SDU Frame type*//*update local information*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_STD] = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_STD + 1u] = 0x00;
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_BT4095_STD;
    	}
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == txNSduCfgPtr->AddressingFormat)
    {/*extended FF*/
    	if (pciDl <= CANTP_FF_DL_12bit_LENGTH)
		{/*FF_DL less than or equal to 4095*//*SDU length low nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_EX_FF_FFDL_LOW_OFFSET_LT4095] = (uint8)pciDl;
			/*SDU length high nibble and Frame type*//*update local information*/
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_EX] = CANTP_FTYPE_FF|(uint8)pciDl;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_FF_DATA_OFFSET_LT4095_EX;
		}
		else
		{/*FF_DL bigger than 4095*//*SDU length from low to high nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_EX_FF_FFDL_LOW_OFFSET_BT4095] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_EX_FF_FFDL_LOW_OFFSET_BT4095 - 1u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_EX_FF_FFDL_LOW_OFFSET_BT4095 - 2u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_EX_FF_FFDL_LOW_OFFSET_BT4095 - 3u] = (uint8)pciDl;
			/*SDU Frame type*//*update local information*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_EX] = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_EX + 1u] = 0x00u;
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_FF_DATA_OFFSET_BT4095_EX;
		}
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == txNSduCfgPtr->AddressingFormat)
	{
		if (pciDl <= CANTP_FF_DL_12bit_LENGTH)
		{/*FF_DL less than or equal to 4095*//*SDU length low nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_NF_FF_FFDL_LOW_OFFSET_LT4095] = (uint8)pciDl;
			/*SDU length high nibble and Frame type*//*update local information*/
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_NF] = CANTP_FTYPE_FF|(uint8)pciDl;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_FF_DATA_OFFSET_LT4095_NF;
		}
		else
		{/*FF_DL bigger than 4095*//*SDU length from low to high nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_NF_FF_FFDL_LOW_OFFSET_BT4095] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_NF_FF_FFDL_LOW_OFFSET_BT4095 - 1u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_NF_FF_FFDL_LOW_OFFSET_BT4095 - 2u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_NF_FF_FFDL_LOW_OFFSET_BT4095 - 3u] = (uint8)pciDl;
			/*SDU Frame type*//*update local information*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_NF] = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_NF + 1u] = 0x00u;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_FF_DATA_OFFSET_BT4095_NF;
		}
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == txNSduCfgPtr->AddressingFormat)
	{
		if (pciDl <= CANTP_FF_DL_12bit_LENGTH)
		{/*FF_DL less than or equal to 4095*//*SDU length low nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_MIX11_FF_FFDL_LOW_OFFSET_LT4095] = (uint8)pciDl;
			/*SDU length high nibble and Frame type*//*update local information*/
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_MIX11] = CANTP_FTYPE_FF|(uint8)pciDl;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_FF_DATA_OFFSET_LT4095_MIX11;
		}
		else
		{/*FF_DL bigger than 4095*//*SDU length from low to high nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_11MIX_FF_FFDL_LOW_OFFSET_BT4095] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_11MIX_FF_FFDL_LOW_OFFSET_BT4095 - 1u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_11MIX_FF_FFDL_LOW_OFFSET_BT4095 - 2u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_11MIX_FF_FFDL_LOW_OFFSET_BT4095 - 3u] = (uint8)pciDl;
			/*SDU Frame type*//*update local information*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_MIX11] = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_MIX11 + 1u] = 0x00u;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_FF_DATA_OFFSET_BT4095_MIX11;
		}
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == txNSduCfgPtr->AddressingFormat)
	{
		if (pciDl <= CANTP_FF_DL_12bit_LENGTH)
		{/*FF_DL less than or equal to 4095*//*SDU length low nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_MIX29_FF_FFDL_LOW_OFFSET_LT4095] = (uint8)pciDl;
			/*SDU length high nibble and Frame type*//*update local information*/
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_MIX29] = CANTP_FTYPE_FF|(uint8)pciDl;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_MIX29_FF_DATA_OFFSET_LT4095;
		}
		else
		{/*FF_DL bigger than 4095*//*SDU length from low to high nibble*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_29MIX_FF_FFDL_LOW_OFFSET_BT4095] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_29MIX_FF_FFDL_LOW_OFFSET_BT4095 - 1u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_29MIX_FF_FFDL_LOW_OFFSET_BT4095 - 2u] = (uint8)pciDl;
			pciDl >>= CANTP_OFFSET_BYTE_PART;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_29MIX_FF_FFDL_LOW_OFFSET_BT4095 - 3u] = (uint8)pciDl;
			/*SDU Frame type*//*update local information*/
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_MIX29] = CANTP_FTYPE_FF;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalBuf[CANTP_PCI_OFFSET_MIX29 + 1u] = 0x00u;
			CanTp_Channels[ChannelIdx].TxConnection.
			LocalDataPosition = CANTP_MIX29_FF_DATA_OFFSET_BT4095;
		}
	}
	else
	{}
	#endif
}

static FUNC(PduLengthType, CANTP_CODE) CanTp_packSFandLastCF(
        PduLengthType SF_DL,PduLengthType Offset)
{
	PduLengthType len;
	if ((SF_DL >= (8u - Offset)) && (SF_DL <= (10u - Offset)))
	{
		len = 12;
	}
	else if ((SF_DL >= (11u - Offset)) && (SF_DL <= (14u - Offset)))
	{
		len = 16;
	}
	else if ((SF_DL >= (15u - Offset)) && (SF_DL <= (18u - Offset)))
	{
		len = 20;
	}
	else if ((SF_DL >= (19u - Offset)) && (SF_DL <= (22u - Offset)))
	{
		len = 24;
	}
	else if ((SF_DL >= (23u - Offset)) && (SF_DL <= (30u - Offset)))
	{
		len = 32;
	}
	else if ((SF_DL >= (31u - Offset)) && (SF_DL <= (46u - Offset)))
	{
		len = 48;
	}
	else if ((SF_DL >= (47u - Offset)) && (SF_DL <= (62u - Offset)))
	{
		len = 64;
	}
	else
	{
		len = 8;
	}
    return len;
}
/*construct SF PCI information in local buffer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructSFPci(uint8 ChannelIdx)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    uint16 pciDl;
    uint8 channelId;

    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(
            CanTp_Channels[ChannelIdx].TxConnection.NSduId, &channelId);
    pciDl = CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount;

    if (CANTP_STANDARD == txNSduCfgPtr->AddressingFormat)
    {
        /*standard SF*/
    	if (CanTp_packSFandLastCF(
    	        CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount,0) <= 8u)
    	{
    		/*SF_DL less than or equal to 7*/
    		/*Frame type and SF DL*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_STD] =
					((uint8)pciDl & CANTP_SF_DL_MASK);/*CANTP_FTYPE_SF*/
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_SF_DATA_OFFSET_STD;
    	}
		#if (STD_ON == CANTP_FD)
    	else
    	{
    		/*SF_DL bigger than 7*/
    		/*Frame type and SF DL*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_STD] =
			        CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_STD + 1u] =
			        (uint8)pciDl;
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_SF_DATA_OFFSET_CANFD_STD;
    	}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == txNSduCfgPtr->AddressingFormat)
    {
    	/*extended SF*/
    	if (CanTp_packSFandLastCF(
    	        CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount,0) <= 8u)
    	{
    		/*SF_DL less than or equal to 7*/
    		/*Frame type and SF DL ,shall be CANTP_FTYPE_SF | ((uint8)pciDl & CANTP_SF_DL_MASK),
    		 * but for QAC CANTP_FTYPE_SF is remove*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_EX] =
					((uint8)pciDl & CANTP_SF_DL_MASK);
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_SF_DATA_OFFSET_EX;
    	}
		#if (STD_ON == CANTP_FD)
    	else
    	{
    		/*SF_DL bigger than 7*/
    		/*Frame type and SF DL*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_EX] = CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_EX + 1u] =
			        (uint8)pciDl;
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_SF_DATA_OFFSET_CANFD_EX;
    	}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == txNSduCfgPtr->AddressingFormat)
	{
		if (CanTp_packSFandLastCF(
		        CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount,0) <= 8u)
		{
			/*SF_DL less than or equal to 7*/
			/*Frame type and SF DL,shall be CANTP_FTYPE_SF | ((uint8)pciDl & CANTP_SF_DL_MASK),
             * but for QAC CANTP_FTYPE_SF is remove*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_NF] =
					((uint8)pciDl & CANTP_SF_DL_MASK);
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_SF_DATA_OFFSET_NF;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*SF_DL bigger than 7*/
			/*Frame type and SF DL*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_NF] = CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_NF + 1u] =
			        (uint8)pciDl;
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_SF_DATA_OFFSET_CANFD_NF;
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == txNSduCfgPtr->AddressingFormat)
	{
		if (CanTp_packSFandLastCF(
		        CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount,0) <= 8u)
		{
			/*SF_DL less than or equal to 7*/
			/*Frame type and SF DL,shall be CANTP_FTYPE_SF | ((uint8)pciDl & CANTP_SF_DL_MASK),
             * but for QAC CANTP_FTYPE_SF is remove*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX11] =
					((uint8)pciDl & CANTP_SF_DL_MASK);
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_SF_DATA_OFFSET_MIX11;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*SF_DL bigger than 7*/
			/*Frame type and SF DL*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX11] =
			        CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX11 + 1u] =
			        (uint8)pciDl;
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_SF_DATA_OFFSET_CANFD_MIX11;
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == txNSduCfgPtr->AddressingFormat)
	{
		if (CanTp_packSFandLastCF(
		        CanTp_Channels[ChannelIdx].TxConnection.SduDataTotalCount,0) <= 8u)
		{
			/*SF_DL less than or equal to 7*/
			/*Frame type and SF DL,shall be CANTP_FTYPE_SF | ((uint8)pciDl & CANTP_SF_DL_MASK),
             * but for QAC CANTP_FTYPE_SF is remove*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX29] =
					((uint8)pciDl & CANTP_SF_DL_MASK);
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_SF_DATA_OFFSET_MIX29;
		}
		#if (STD_ON == CANTP_FD)
		else
		{
			/*SF_DL bigger than 7*/
			/*Frame type and SF DL*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX29] =
			        CANTP_FTYPE_SF;
			CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX29 + 1u] =
			        (uint8)pciDl;
			/*update local buffer information*/
			CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition =
			        CANTP_SF_DATA_OFFSET_CANFD_MIX29;
		}
		#endif/*ENDOF #if (STD_ON == CANTP_FD)*/
	}
	#endif
	else
	{}
}

/*construct CF PCI information in local buffer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_ConstructCFPci(uint8 ChannelIdx)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    uint8 channelId;

    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(
            CanTp_Channels[ChannelIdx].TxConnection.NSduId, &channelId);

    if (CANTP_STANDARD == txNSduCfgPtr->AddressingFormat)
    {
        /*Standard SDU*/
        CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_STD] =
        		CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn;
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_STD;
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == txNSduCfgPtr->AddressingFormat)
    {
        CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_EX] =
        		CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn;
        /*update local buffer information*/
        CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_EX;
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == txNSduCfgPtr->AddressingFormat)
	{
		CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_NF] =
				CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn;
		/*update local buffer information*/
		CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_NF;
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == txNSduCfgPtr->AddressingFormat)
	{
		CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX11] =
				CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn;
		/*update local buffer information*/
		CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_MIX11;
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == txNSduCfgPtr->AddressingFormat)
	{
		CanTp_Channels[ChannelIdx].TxConnection.LocalBuf[CANTP_PCI_OFFSET_MIX29] =
				CANTP_FTYPE_CF | CanTp_Channels[ChannelIdx].TxConnection.CurrentCfSn;
		/*update local buffer information*/
		CanTp_Channels[ChannelIdx].TxConnection.LocalDataPosition = CANTP_CF_DATA_OFFSET_MIX29;
	}
	#endif
	else
	{}
}

/*Get the information of received FC frame.*/
static FUNC(void, CANTP_CODE)
CanTp_GetChannelFCInfo(
    uint8 ChannelIdx,
    P2VAR(CanTp_FCInfoType, AUTOMATIC, AUTOMATIC) FcInfoPtr)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
	uint32 stMinUs;
    uint8 channelId;
    PduLengthType offset = 0;
    uint8 stMinRaw;


    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(
            CanTp_Channels[ChannelIdx].TxConnection.NSduId, &channelId);

    if (CANTP_STANDARD == txNSduCfgPtr->AddressingFormat)
    {
        /*standard SDU*/
        offset = CANTP_PCI_OFFSET_STD;
    }
	#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    else if (CANTP_EXTENDED == txNSduCfgPtr->AddressingFormat)
    {
        /*extended SDU*/
        offset = CANTP_PCI_OFFSET_EX;
    }
	#endif
	#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_NORMALFIXED == txNSduCfgPtr->AddressingFormat)
	{
		/*extended SDU*/
		offset = CANTP_PCI_OFFSET_NF;
	}
	#endif
	#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED == txNSduCfgPtr->AddressingFormat)
	{
		/*extended SDU*/
		offset = CANTP_PCI_OFFSET_MIX11;
	}
	#endif
	#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
	else if (CANTP_MIXED29BIT == txNSduCfgPtr->AddressingFormat)
	{
		/*extended SDU*/
		offset = CANTP_PCI_OFFSET_MIX29;
	}
	#endif
	else
	{
	    /*idle*/
	}

    FcInfoPtr->FcFs = CanTp_Channels[ChannelIdx].TxConnection.FcBuf[offset] & CANTP_FC_FS_MASK;
    FcInfoPtr->FcBS = CanTp_Channels[ChannelIdx].TxConnection.FcBuf[offset + 1u];

    /*calculate the STMin in tick unit. Round the value to the nearest integer, that is
     * if it is 0.49, then the integer will be 0, 0.50 will be 1.*/
    stMinRaw = CanTp_Channels[ChannelIdx].TxConnection.FcBuf[offset + 2u];

    if (CANTP_FC_STMIN_MS_LIMIT >= stMinRaw)
    {
        /*ms per bit*/
        stMinUs = (uint32)stMinRaw * CANTP_FC_STMIN_MS_UNIT;
    }
    else if ((CANTP_FC_STMIN_US_LOW <= stMinRaw)
        && (CANTP_FC_STMIN_US_UPPER >= stMinRaw))
    {
        /*F1-F9 100us per step*/
        stMinUs = ((uint32)((uint32)stMinRaw - 0xF0UL)) * CANTP_FC_STMIN_US_UNIT;
    }
    else
    {
        /*invalid value, use 7F*/
        stMinUs = (uint32)CANTP_FC_STMIN_MS_LIMIT * CANTP_FC_STMIN_MS_UNIT;
    }

    /*transit to value in unit tick*/
    FcInfoPtr->FcSTMin = (stMinUs + (uint32)((uint32)1000 >> (uint32)1)) / (uint32)1000;
}

#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
