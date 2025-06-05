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
**  FILENAME    : CanTp_Cbk.c                                                **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Call-back function implementation                          **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*=======[I N C L U D E S]====================================================*/
#include "CanTp.h"
#include "CanTp_Cbk.h"
#include "CanTp_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

typedef struct
{
    CanTp_FramePCIType FrameType;
    uint8 channelIdx;
    P2VAR(boolean, AUTOMATIC, CANTP_CONST) FcFlag;
}CanTp_StoreRxNPduEvtType;
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/



/*This function is called by the CanTp_RxIndication after a successful
reception of a RX CAN L-PDU, put the PDU into pool*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
static FUNC(void, CANTP_CODE)
CanTp_FillRxPdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr);

/*Handle Confirm event occurred to the specific channel*/
static FUNC(void, CANTP_CODE)
CanTp_DispatchCfmEvent(uint8 ChannelIdx);

/*Store the received RXNPDU when indicated*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvt(
        PduIdType CanTpRxPduId,
        P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt
        );

/*find RxIPduId*/
static FUNC(PduIdType, CANTP_CODE)
        CanTp_GetRxIPduId(PduIdType RcvdNPduId,uint8 channelIdx);

/*store the RxNpdu info*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduInfo(
        PduIdType CanTpRxPduId,
        P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt
        );
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/*
 * Brief               This function is called by the CAN Interface after a successful
 *                     reception of a RX CAN L-PDU.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId the received N-PDU ID
 *                     CanTpRxPduPtr indicator of structure with received
 *                                   L-SDU(payload) and data length
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface Receiving handling
 */
#define CANTP_START_SEC_CANTPRXINDICATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE)
CanTp_RxIndication(
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr
    )
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the reception indication only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_UNINIT);
    }
    else if ((NULL_PTR == PduInfoPtr)
            || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        /*INVALID pointer parameter*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PARAM_POINTER);
    }
    else
#endif
    { /*pointer not null*/
        /*fill the received NPDU into channel*/
        CanTp_FillRxPdu(RxPduId, PduInfoPtr);
    } /*END OF pointer not null*/
}

#define CANTP_STOP_SEC_CANTPRXINDICATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               All transmitted CAN frames belonging to the CAN Transport
 *                     Layer will be confirmed by this function.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxPduId ID of CAN L-PDU that has been transmitted
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface transmitting confirmation
 */
#define CANTP_START_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE)
CanTp_TxConfirmation(PduIdType TxPduId)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;
    volatile boolean eventConsumed = FALSE;
    uint8 chnIdx;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the reception indication only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_UNINIT);
    }
    else
#endif
    {
        /*first check if it is a transmitted data NPDU*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByTxNPdu(TxPduId, &chnIdx);
        if (NULL_PTR != txNSduCfgPtr)
        {
           /*A transmitted data PDU confirmed*/
           /*Store the confirmation event only when the channel related with this TX SDU
            * is transmitting this SDU and it is waiting for the confirmation*/
           channelPtr = &CanTp_Channels[chnIdx];

           if (((CANTP_TX_SF_V4SF_CFM == channelPtr->TxConnection.Substate)
                   || (CANTP_TX_LARGE_V4DATA_CFM == channelPtr->TxConnection.Substate))
               && (txNSduCfgPtr->TxNSduId == channelPtr->TxConnection.NSduId))
           {
               channelPtr->TxConnection.EventFlags |= CANTP_CHANNEL_EVENT_TX_CONFIRMED;
               eventConsumed = TRUE;
           }
        } /*END OF handling data frame confirmation*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        else
        {
           /*invalid confirmation*/
           CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
        }
#endif
        if (FALSE == eventConsumed)
        {
           /*then check if it is a transmitted FC NPDU*/
           rxNSduCfgPtr = CanTp_GetRxSduCfgByTxFCNPdu(TxPduId, &chnIdx);

           if (NULL_PTR != rxNSduCfgPtr)
           {
               channelPtr = &CanTp_Channels[chnIdx];
               /*A transmitted FC PDU confirmed*/
               /*Store the confirmation event only when the channel related with this RX SDU
                * is receiving this SDU and it is waiting for the FC confirmation*/
               if (((CANTP_RX_LARGE_V4FCCTS_CFM == channelPtr->RxConnection.Substate)
                       || (CANTP_RX_LARGE_V4FCWT_CFM == channelPtr->RxConnection.Substate))
                   && (rxNSduCfgPtr->RxNSduId == channelPtr->RxConnection.NSduId))
               {
                   channelPtr->RxConnection.EventFlags |= CANTP_CHANNEL_EVENT_TXFC_CONFIRMED;
                   eventConsumed = TRUE;
               }
           }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
           else
           {
               /*invalid confirmation*/
               CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
           }
#endif
        } /*END OF FC confirmation*/

        if (TRUE == eventConsumed)
        {
           /*handle confirmation events*/
           CanTp_DispatchCfmEvent(chnIdx);
        }
    }
}
#define CANTP_STOP_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

/*========[Internal STATIC Function Implementation]==================================*/
/*find RxIPduId*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
static FUNC(PduIdType, CANTP_CODE)
        CanTp_GetRxIPduId(PduIdType RcvdNPduId,uint8 channelIdx)
{
    PduIdType RxIPduId = 0xFF;
    uint8 iloop;
    for (iloop = 0; ((iloop < CanTp_CfgData.CanTpChannel[channelIdx].RxNSduNum)
            && (RxIPduId == 0xFFu)); iloop++)
    {
        if (CanTp_CfgData.CanTpChannel[channelIdx].RxNSdus[iloop].RxNPduId == RcvdNPduId)
        {
            RxIPduId = CanTp_CfgData.CanTpChannel[channelIdx].RxNSdus[iloop].RxIPduId;
        }
    }
    return RxIPduId;
}

/*Utility function:copy memory data.*/
static FUNC(void, CANTP_CODE)
CanTp_MemoryCopy(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) Source,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Dest,
    uint16 Length)
{
    uint16 DL;
    for (DL = 0; DL < Length; DL++)
    {
        Dest[DL] = Source[DL];
    }
}

/*store the RxNpdu info*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduInfo(
	PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt
    )
{
	Std_ReturnType result = E_OK;
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;
    PduIdType RxIPduId;

    channelPtr = &CanTp_Channels[CanTp_RxNPduEvt.channelIdx];
    /*Before Store Rxinfo shall clear the older Rx event*/
    /*CANTP_CHANNEL_EVENT_FF_RECEIVED,CANTP_CHANNEL_EVENT_CF_RECEIVED,
     * CANTP_CHANNEL_EVENT_FC_RECEIVED,CANTP_CHANNEL_EVENT_SF_RECEIVED,
     * all equal CANTP_CHANNEL_EVENT_RECEIVED_MASK*/
	if ((channelPtr->RxConnection.EventFlags & CANTP_CHANNEL_EVENT_RECEIVED_MASK) != 0u)
	{
	    RxIPduId = CanTp_GetRxIPduId(channelPtr->RxConnection.RcvdNPduId,CanTp_RxNPduEvt.channelIdx);
		PduR_CanTpRxIndication(RxIPduId, E_NOT_OK);
        channelPtr->RxConnection.EventFlags =
        		channelPtr->RxConnection.EventFlags
        		& ((uint16)~CANTP_CHANNEL_EVENT_RECEIVED_MASK);
	}
    CanTp_MemoryCopy(	CanTpRxPduPtr->SduDataPtr,
    					channelPtr->RxConnection.LocalBuf,
						CanTpRxPduPtr->SduLength);
    channelPtr->RxConnection.RcvdNPduId = CanTpRxPduId;
    channelPtr->RxConnection.RcvdNSduId = RxNSduCfgPtr->RxNSduId;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    channelPtr->RxConnection.MetaDataLength =
            (RxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
#endif
    channelPtr->RxConnection.DataLen = CanTpRxPduPtr->SduLength;

    switch(CanTp_RxNPduEvt.FrameType)
    {
    case CANTP_FTYPE_CF:
        #if (CANTP_DYN_ID_SUPPORT == STD_ON)
        /*SWS_CanTp_00333,check the addressing information contained in the MetaData of
         * the N-PDU (at the end of the data referenced by PduInfoPtr) against the
         * stored values from the FF.*/
        result = CanTp_CheckRxCFMetaData(CanTp_RxNPduEvt.channelIdx, RxNSduCfgPtr, CanTpRxPduPtr);
        #endif
        if (E_OK == result)
        {
            channelPtr->RxConnection.EventFlags |= CANTP_CHANNEL_EVENT_CF_RECEIVED;
        }
        break;
    case CANTP_FTYPE_FF:
        /*no break,because do same save metadata*/
    case CANTP_FTYPE_SF:
        /*transit the sub-state in order to ignore the following events related
         * with the last reception*/
#if 0 /*no affect?*/
        channelPtr->RxConnection.Substate = CANTP_SUBSTATE_NONE;
#endif
        #if (CANTP_DYN_ID_SUPPORT == STD_ON)
        result = CanTp_SaveRxMetaDataInfo(CanTp_RxNPduEvt.channelIdx, RxNSduCfgPtr, CanTpRxPduPtr);
        #endif
        if (E_OK == result)
        {
            if (CanTp_RxNPduEvt.FrameType == CANTP_FTYPE_SF)
            {
                channelPtr->RxConnection.EventFlags |= CANTP_CHANNEL_EVENT_SF_RECEIVED;
            }
            else
            {
                channelPtr->RxConnection.EventFlags |= CANTP_CHANNEL_EVENT_FF_RECEIVED;
            }
        }
        break;
    default:
        /*idle*/
        break;
    }
	return result;
}

static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvtForCF(
        PduIdType CanTpRxPduId,
        P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        uint8 channelIdx
)
{
    Std_ReturnType result = E_NOT_OK;
    CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt;
#if (STD_ON == CANTP_FD)
    PduLengthType MatchDLC = CanTp_MatchDLC(CanTpRxPduPtr->SduLength);
#endif

    /*min-length check*/
    if ((CanTpRxPduPtr->SduLength < 1u)
    #if (STD_ON == CANTP_FD)
        || (((CanTpRxPduPtr->SduLength != MatchDLC)
            && (CanTpRxPduPtr->SduLength >= 8u)))
        || (CanTpRxPduPtr->SduLength > RxNSduCfgPtr->RxNPduDLC)
    #endif
            )
    {
        /*idle*/
    }
    else
    {
        /*only accept it when the channel is receiving a large SDU related with this CF,
     * and it is waiting a CF.*/
        if ((CANTP_FUNCTIONAL_RX == RxNSduCfgPtr->RxTaType)
            #if (STD_ON == CANTP_FD)
            ||(CANTP_CANFD_FUNCTIONAL == RxNSduCfgPtr->RxTaType)
            #endif
            )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /*invalid TAType with CF frame, wrong configuration*/
        	CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_TATYPE);
#endif
        }
        else if ((RxNSduCfgPtr->RxNSduId == CanTp_Channels[channelIdx].RxConnection.NSduId)
                && (CANTP_LARGE_RECEIVING == CanTp_Channels[channelIdx].RxConnection.RootState)
                && (CANTP_RX_LARGE_V4CF == CanTp_Channels[channelIdx].RxConnection.Substate))
        {
            if ((8u > CanTpRxPduPtr->SduLength)
                    && (RxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON))
            {
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION,CANTP_E_PADDING);
                PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
            }
            else
            {
                CanTp_RxNPduEvt.FrameType = CANTP_FTYPE_CF;
                CanTp_RxNPduEvt.channelIdx = channelIdx;
                result = CanTp_StoreRxNPduInfo(
                        CanTpRxPduId,
                        CanTpRxPduPtr,
                        RxNSduCfgPtr,
                        CanTp_RxNPduEvt);
            }
        }
        else
        {
            /*CF not expected*/
        }
    }
    return result;
}

static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvtForFF(
        PduIdType CanTpRxPduId,
        P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        uint8 channelIdx
)
{
    Std_ReturnType result = E_NOT_OK;
    PduLengthType Dl = 0;
    PduLengthType frameDl;
    CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt;
#if (STD_ON == CANTP_FD)
    PduLengthType MatchDLC = CanTp_MatchDLC(CanTpRxPduPtr->SduLength);
#endif

    /*min-length check*/
    /*half duplex, if Tx is in processing, the same channel can not
     * receiving data at the same time*/
    /*Only accept FF when the channel is IDLE or it is handling SDU receiving*/
    /*Function addressing mode not support multiple frames*/
    if ((CanTpRxPduPtr->SduLength < 8u)
            || ((CanTp_Channels[channelIdx].ChannelMode == CANTP_MODE_HALF_DUPLEX)
            &&(CanTp_Channels[channelIdx].TxConnection.RootState != CANTP_IDLE ))
    #if (STD_ON == CANTP_FD)
        || (((CanTpRxPduPtr->SduLength != MatchDLC)
            && (CanTpRxPduPtr->SduLength > 8u)))
            || (CanTpRxPduPtr->SduLength > RxNSduCfgPtr->RxNPduDLC)
    #endif
        )
    {
        /*idle*/
    }
    else
    {
        if ((CANTP_FUNCTIONAL_RX == RxNSduCfgPtr->RxTaType)
                #if (STD_ON == CANTP_FD)
                ||(CANTP_CANFD_FUNCTIONAL == RxNSduCfgPtr->RxTaType)
                #endif
                )
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /*invalid TAType with FF frame, wrong configuration*/
        	CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_TATYPE);
#endif
        }
        else if ((CANTP_IDLE == CanTp_Channels[channelIdx].RxConnection.RootState)
            || (CANTP_LARGE_RECEIVING == CanTp_Channels[channelIdx].RxConnection.RootState))
        {
            /*check data length of the FF, ignore the frame if invalid length*/
            frameDl = CanTp_RxGetFFDl(CanTpRxPduPtr->SduDataPtr, RxNSduCfgPtr);
            if (CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat)
            {
                /*normal addressing mode*/
                if (frameDl > 4095u)
                {
                    Dl = CANTP_FF_DATA_OFFSET_BT4095_STD;
                }
                else
                {
                    Dl = CANTP_FF_DATA_OFFSET_LT4095_STD;
                }
            }
            #if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
            else if (CANTP_EXTENDED == RxNSduCfgPtr->AddressingFormat)
            {
                /*extended addressing mode*/
                if (frameDl > 4095u)
                {
                    Dl = CANTP_FF_DATA_OFFSET_BT4095_EX;
                }
                else
                {
                    Dl = CANTP_FF_DATA_OFFSET_LT4095_EX;
                }
            }
            #endif
            #if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
            else if (CANTP_NORMALFIXED == RxNSduCfgPtr->AddressingFormat)
            {
                /*normal fixed addressing mode*/
                if (frameDl > 4095u)
                {
                    Dl = CANTP_FF_DATA_OFFSET_BT4095_NF;
                }
                else
                {
                    Dl = CANTP_FF_DATA_OFFSET_LT4095_NF;
                }
            }
            #endif
            #if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
            else if ((CANTP_MIXED == RxNSduCfgPtr->AddressingFormat) ||
                    (CANTP_MIXED29BIT == RxNSduCfgPtr->AddressingFormat))
            {
                /*mix11 addressing mode*/
                if (frameDl > 4095u)
                {
                    Dl = CANTP_FF_DATA_OFFSET_BT4095_MIX11;
                }
                else
                {
                    Dl = CANTP_FF_DATA_OFFSET_LT4095_MIX11;
                }
            }
            #endif
            else
            {}
            /*+ 1u means this FF can be send by SF*/
            if(frameDl > (CanTpRxPduPtr->SduLength - Dl + 1u))
            {
                CanTp_RxNPduEvt.FrameType = CANTP_FTYPE_FF;
                CanTp_RxNPduEvt.channelIdx = channelIdx;
                /*store event information*/
                result = CanTp_StoreRxNPduInfo(CanTpRxPduId,CanTpRxPduPtr,RxNSduCfgPtr,CanTp_RxNPduEvt);
                CanTp_Channels[channelIdx].RxConnection.ChannelTimer.EnabledTimer = CANTP_NB;
                CanTp_Channels[channelIdx].RxConnection.ChannelTimer.RemainTime = RxNSduCfgPtr->Nbr;
                CanTp_ResetTime(&(CanTp_Channels[channelIdx].RxConnection.ChannelTimer.StartTime));
            }
        }
        else
        {
            /*idle*/
        }
    }
    return result;
}

#if (STD_ON == CANTP_FD)
FUNC(boolean, CANTP_CODE) CanTp_checkSF(
        PduLengthType SF_DL,PduLengthType CAN_DL,PduLengthType Offset)
{
    boolean ret = FALSE;
    switch(CAN_DL)
    {
    case 12u:
        if ((SF_DL >= (8u - Offset)) && (SF_DL <= (10u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 16u:
    case 20u:
    case 24u:
        if ((SF_DL >= ((CAN_DL - 5u) - Offset)) && (SF_DL <= ((CAN_DL - 2u) - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 32u:
        if ((SF_DL >= (23u - Offset)) && (SF_DL <= (30u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 48u:
        if ((SF_DL >= (31u - Offset)) && (SF_DL <= (46u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 64u:
        if ((SF_DL >= (47u - Offset)) && (SF_DL <= (62u - Offset)))
        {
            ret = TRUE;
        }
        break;
    default:
        if (CAN_DL <= 8u)
        {
            ret = TRUE;
        }
        break;
    }
    return ret;
}

FUNC(boolean, CANTP_CODE) CanTp_checkLastCF(
        PduLengthType CF_DL,PduLengthType CAN_DL,PduLengthType Offset)
{
    boolean ret = FALSE;
    switch(CAN_DL)
    {
    case 12u:
        if ((CF_DL >= (8u - Offset)) && (CF_DL <= (11u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 16u:
    case 20u:
    case 24u:
        if ((CF_DL >= ((CAN_DL - 4u) - Offset)) && (CF_DL <= ((CAN_DL - 1u) - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 32u:
        if ((CF_DL >= (24u - Offset)) && (CF_DL <= (31u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 48u:
        if ((CF_DL >= (32u - Offset)) && (CF_DL <= (47u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 64u:
        if ((CF_DL >= (48u - Offset)) && (CF_DL <= (63u - Offset)))
        {
            ret = TRUE;
        }
        break;
    default:
        if (CAN_DL <= 8u)
        {
            ret = TRUE;
        }
        break;
    }
    return ret;
}
#endif
static FUNC(void, CANTP_CODE)
CanTp_StoreRxNPduEvtForSFGetDl(
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        P2VAR(PduLengthType, AUTOMATIC, CANTP_CONST) Dl
#if (STD_ON == CANTP_FD)
        ,P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2VAR(PduLengthType, AUTOMATIC, CANTP_CONST) offset
#endif
)
{
    if ((CANTP_STANDARD == RxNSduCfgPtr->AddressingFormat) ||
            (CANTP_NORMALFIXED == RxNSduCfgPtr->AddressingFormat))
    {
        /*normal addressing mode*/
        if (CAN20 == RxNSduCfgPtr->CanFrameType)
        {
            *Dl = CANTP_SF_DATA_OFFSET_STD;
        }
        #if (STD_ON == CANTP_FD)
        else if ((CAN_FD == RxNSduCfgPtr->CanFrameType) &&
                (CanTpRxPduPtr->SduLength > 8u))
        {
            *Dl = CANTP_SF_DATA_OFFSET_CANFD_STD;
        }
        else
        {}
        *offset = 0;
        #endif
    }
    #if ((CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)||(CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)||(CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    else if ((CANTP_MIXED == RxNSduCfgPtr->AddressingFormat) ||
            (CANTP_MIXED29BIT == RxNSduCfgPtr->AddressingFormat) ||
            (CANTP_EXTENDED == RxNSduCfgPtr->AddressingFormat))
    {
        /*mix11 addressing mode*//*mix29 addressing mode*/
        if (CAN20 == RxNSduCfgPtr->CanFrameType)
        {
            *Dl = CANTP_SF_DATA_OFFSET_MIX11;
        }
        #if (STD_ON == CANTP_FD)
        else if ((CAN_FD == RxNSduCfgPtr->CanFrameType) &&
                (CanTpRxPduPtr->SduLength > 8u))
        {
            *Dl = CANTP_SF_DATA_OFFSET_CANFD_MIX11;
        }
        else
        {}
        *offset = 1;
        #endif
    }
    #endif
    else
    {
        /*idle*/
    }
}

static FUNC(void, CANTP_CODE)
CanTp_StoreRxSFDealWith3E80(uint8 ChannelIdx,
P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr)
{
    PduInfoType pduInfo;
    const PduInfoType *pduInfoPtr = &pduInfo;
    BufReq_ReturnType bufRslt;
    PduLengthType bufferSize;
    PduIdType iPduId;
    uint8 data[2] = {0x3E,0x80};
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 metadata[5];

    pduInfo.MetaDataPtr = metadata;
#endif

    pduInfo.SduLength = 0;
    iPduId = RxNSduCfgPtr->RxIPduId;

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    if (((RxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK) > 0u)
       && ((CANTP_ADDRESS_EXTENSION_8_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_ADDRESS_EXTENSION_8_MASK))
       || (CANTP_SA16_AND_TA16_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))))
    {
        CanTp_ConstructForwardRxMetaData(ChannelIdx, RxNSduCfgPtr, pduInfoPtr);
    }
#endif

    bufRslt = PduR_CanTpStartOfReception(iPduId, pduInfoPtr, 2u, &bufferSize);

    /*handle according the result of request RX buffer*/
    switch (bufRslt)
    {
        case BUFREQ_OK:
            if (bufferSize >= 2u)
            {
                pduInfo.SduDataPtr = data;
                pduInfo.SduLength = 2;
                bufRslt = PduR_CanTpCopyRxData(iPduId, pduInfoPtr, &bufferSize);
                if (bufRslt == BUFREQ_OK)
                {
                    PduR_CanTpRxIndication(iPduId, E_OK);
                }
                else
                {
                    PduR_CanTpRxIndication(iPduId, E_NOT_OK);
                    CanTp_ReleaseRxChannel(ChannelIdx);
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

static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvtForSF(
        PduIdType CanTpRxPduId,
        P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        uint8 channelIdx
)
{
    Std_ReturnType result = E_NOT_OK;
    PduLengthType Dl = 0;
    PduLengthType frameDl;
    CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt;
#if (STD_ON == CANTP_FD)
    PduLengthType offset = 0;
    PduLengthType MatchDLC = CanTp_MatchDLC(CanTpRxPduPtr->SduLength);
#endif

    /*min-length check*/
    if (CanTpRxPduPtr->SduLength >= 1u)
    {
        /*Handle FF and SF with the same strategy*/
        /*Only accept SF when the channel is IDLE or it is handling SDU receiving*/
        if((CanTp_Channels[channelIdx].ChannelMode == CANTP_MODE_HALF_DUPLEX)
            &&(CanTp_Channels[channelIdx].TxConnection.RootState != CANTP_IDLE ))
        {
            /*half duplex, if Tx is in processing, the same channel can not
             * receiving data at the same time*/
        }
        else if((CANTP_IDLE == CanTp_Channels[channelIdx].RxConnection.RootState)
                || (CANTP_LARGE_RECEIVING == CanTp_Channels[channelIdx].RxConnection.RootState))
        {
            /*check data length of the SF, ignore the frame if invalid length*/
            frameDl = CanTp_RxGetSFDl(CanTpRxPduPtr->SduDataPtr,RxNSduCfgPtr
#if (STD_ON == CANTP_FD)
            		,CanTpRxPduPtr->SduLength
#endif
					);

#if (STD_ON == CANTP_FD)
           if (((CanTpRxPduPtr->SduLength != MatchDLC)
                && (CanTpRxPduPtr->SduLength >= 8u))
               || (CanTpRxPduPtr->SduLength > RxNSduCfgPtr->RxNPduDLC))
            {
               result = E_NOT_OK;
            }
           else
#endif
           {
                if ((8u > CanTpRxPduPtr->SduLength)
                        && (RxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON))
                {
                    CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION,CANTP_E_PADDING);
                    result = E_NOT_OK;
                }
                else
                {
                    CanTp_StoreRxNPduEvtForSFGetDl(RxNSduCfgPtr,&Dl
                    #if (STD_ON == CANTP_FD)
                    		,CanTpRxPduPtr,&offset
                    #endif
);
                    /*frameDl valid*/
                    if (frameDl != 0x0u)
                    {
#if (STD_ON == CANTP_FD)
                        if (FALSE == CanTp_checkSF(
                        frameDl,CanTp_MatchDLC(CanTpRxPduPtr->SduLength),offset))
                        {}
                        else
#endif
                        if(frameDl <= (CanTpRxPduPtr->SduLength - Dl))
                        {/*store event information*/
                            if ((CanTpRxPduPtr->SduDataPtr[Dl] == 0x3Eu)
                                 && (CanTpRxPduPtr->SduDataPtr[Dl + 1u] == 0x80u)
                                 && ((RxNSduCfgPtr->RxTaType == CANTP_FUNCTIONAL_RX)
                                     || (RxNSduCfgPtr->RxTaType == CANTP_CANFD_FUNCTIONAL))
                                 && (frameDl == 2u))
                            {
                                CanTp_StoreRxSFDealWith3E80(channelIdx,RxNSduCfgPtr);
                            }
                            else
                            {
                            	if (CANTP_LARGE_RECEIVING == CanTp_Channels[channelIdx].RxConnection.RootState)
                            	{
                            		PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                                    CanTp_ReleaseRxChannel(channelIdx);
                            	}
                                CanTp_RxNPduEvt.FrameType = CANTP_FTYPE_SF;
                                CanTp_RxNPduEvt.channelIdx = channelIdx;
                                result = CanTp_StoreRxNPduInfo(CanTpRxPduId,CanTpRxPduPtr,
                                        RxNSduCfgPtr,CanTp_RxNPduEvt);
                            }
                        }
                        else
                        {}
                    }
                }
           }
        }
        else
        {
            /*idle*/
        }
    }
    return result;
}
/*Store the received RXNPDU when indicated*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxNPduEvt(
        PduIdType CanTpRxPduId,
        P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
        CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt
        )
{
    Std_ReturnType result = E_NOT_OK;

    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
    *(CanTp_RxNPduEvt.FcFlag) = FALSE;
    switch (CanTp_RxNPduEvt.FrameType)
    {
        case CANTP_FTYPE_CF:
            result = CanTp_StoreRxNPduEvtForCF(
                    CanTpRxPduId,
                    CanTpRxPduPtr,
                    RxNSduCfgPtr,
                    CanTp_RxNPduEvt.channelIdx);
            break;
        case CANTP_FTYPE_FF:
            result = CanTp_StoreRxNPduEvtForFF(
                    CanTpRxPduId,
                    CanTpRxPduPtr,
                    RxNSduCfgPtr,
                    CanTp_RxNPduEvt.channelIdx);
            break;
        case CANTP_FTYPE_SF:
            result = CanTp_StoreRxNPduEvtForSF(
                    CanTpRxPduId,
                    CanTpRxPduPtr,
                    RxNSduCfgPtr,
                    CanTp_RxNPduEvt.channelIdx);
            break;
        default:
            *(CanTp_RxNPduEvt.FcFlag) = TRUE;/*this will never happen*/
            break;
    } /*END OF switch (frameType) */
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/
    return result;
}

/*Store the received FC NPDU when indicated.*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_StoreRxFCNPduEvt(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    uint8 channelIdx
    )
{
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;
    PduIdType SduId;
    Std_ReturnType result = E_OK;
#if (STD_ON == CANTP_FD)
    PduLengthType MatchDLC = CanTp_MatchDLC(CanTpRxPduPtr->SduLength);
#endif

    SduId = TxNSduCfgPtr->TxNSduId;

    /*min-length check*/
    if ((CanTpRxPduPtr->SduLength < 3u)
#if (STD_ON == CANTP_FD)
       || (((MatchDLC != CanTpRxPduPtr->SduLength)
             && (CanTpRxPduPtr->SduLength >= 8u)))
             || (CanTpRxPduPtr->SduLength > TxNSduCfgPtr->TxNPduDLC)
#endif
                    )
    {
        result = E_NOT_OK;
    }

    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
    channelPtr = &CanTp_Channels[channelIdx];
    /*only accepted the received FC when the channel is
     * transmitting a large SDU related with the FC NPDU,
     * and it is waiting for a FC. Otherwise ignore the frame*/
    if ((result == E_OK) && (SduId == channelPtr->TxConnection.NSduId)
        && (CANTP_TX_LARGE_V4FC == channelPtr->TxConnection.Substate)
        && (CANTP_LARGE_TRANSMITTING == channelPtr->TxConnection.RootState)
        && ((channelPtr->TxConnection.EventFlags & CANTP_CHANNEL_EVENT_FC_RECEIVED) == 0u))
    {
        if ((8u > CanTpRxPduPtr->SduLength) &&
                (TxNSduCfgPtr->TxPaddingActivation == CANTP_PADDING_ON))
        {
            CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION,CANTP_E_PADDING);
            result = E_NOT_OK;
        }
        else
        {
            CanTp_MemoryCopy(CanTpRxPduPtr->SduDataPtr,
                            channelPtr->TxConnection.FcBuf,
                            CanTpRxPduPtr->SduLength);
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
            /*SWS_CanTp_00336,the CanTp module shall check the addressing information contained in the
             * MetaData (at the end of the data referenced by PduInfoPtr) against the stored values.*/
            result = CanTp_CheckRxFCMetaData(channelIdx, TxNSduCfgPtr, CanTpRxPduPtr);
#endif
            if (result == E_OK)
            {
                channelPtr->TxConnection.EventFlags |= CANTP_CHANNEL_EVENT_FC_RECEIVED;
            }
        }
    }
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

    return result;
}



/*This function is called by the CanTp_RxIndication after a successful
reception of a RX CAN L-PDU, put the PDU into pool.*/
static FUNC(void, CANTP_CODE)
CanTp_FillRxPdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    CanTp_FramePCIType sduFrameType;
    uint8 chnIdx;
    boolean fc = TRUE;
    CanTp_StoreRxNPduEvtType CanTp_RxNPduEvt;

    /*first try to check if the received NPDU whether is CF SF or FF*/
	rxNSduCfgPtr = CanTp_GetRxSduCfgByNpdu(
	        CanTpRxPduId,
	        CanTpRxPduPtr,
	        &sduFrameType,
	        &chnIdx);
	if (NULL_PTR != rxNSduCfgPtr)
	{
	    CanTp_RxNPduEvt.FcFlag = &fc;
	    CanTp_RxNPduEvt.FrameType = sduFrameType;
	    CanTp_RxNPduEvt.channelIdx = chnIdx;
		(void)CanTp_StoreRxNPduEvt(
			CanTpRxPduId,
			CanTpRxPduPtr,
			rxNSduCfgPtr,
			CanTp_RxNPduEvt);
	}

	if (TRUE == fc)
	{
		/*then check whether it is FC, if FC received that means this is sender*/
		txNSduCfgPtr = CanTp_GetTxSduCfgByFCNpdu(
		        CanTpRxPduId,
		        CanTpRxPduPtr,
		        &chnIdx);
		if (NULL_PTR != txNSduCfgPtr)
		{
			/*A FC received*/
			(void)CanTp_StoreRxFCNPduEvt(
				CanTpRxPduPtr,
				txNSduCfgPtr,
				chnIdx);
		}
	}
}

/*Handle Confirm event occurred to the specific channel*/
static FUNC(void, CANTP_CODE)
CanTp_DispatchCfmEvent(uint8 ChannelIdx)
{
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[ChannelIdx];

    if ((channelPtr->TxConnection.EventFlags
            & CANTP_CHANNEL_EVENT_TX_CONFIRMED) != 0u)
    {
        if ((CANTP_TX_SF_V4SF_CFM == channelPtr->TxConnection.Substate)
            || (CANTP_TX_LARGE_V4DATA_CFM == channelPtr->TxConnection.Substate))
        {
            /*A data frame transmit confirmed*/
            CanTp_TxHandleTransmitCfm(ChannelIdx);
        }
        else
        {
            channelPtr->TxConnection.EventFlags =
            		channelPtr->TxConnection.EventFlags
            		& ((uint8)~CANTP_CHANNEL_EVENT_TX_CONFIRMED);
        }
    }

    if ((channelPtr->RxConnection.EventFlags
            & CANTP_CHANNEL_EVENT_TXFC_CONFIRMED) != 0u)
    {
        if ((CANTP_RX_LARGE_V4FCCTS_CFM == channelPtr->RxConnection.Substate)
            || (CANTP_RX_LARGE_V4FCWT_CFM == channelPtr->RxConnection.Substate))
        {
            /*A data frame transmit confirmed*/
            CanTp_RxHandleFCCfm(ChannelIdx);
        }
        else
        {
            channelPtr->RxConnection.EventFlags =
            		channelPtr->RxConnection.EventFlags
            		& ((uint8)~CANTP_CHANNEL_EVENT_TXFC_CONFIRMED);
        }
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
