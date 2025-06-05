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
**  FILENAME    : CanTp.c                                                    **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Public functions implementation                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V2.0.0       2020-5-6  tao.yu         create
 *  
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#include "CanTp.h"
#include "CanTp_Internal.h"


/*========[V E R S I O N  I N F O R M A T I O N]=========*/
#define CANTP_C_AR_MAJOR_VERSION  4U
#define CANTP_C_AR_MINOR_VERSION  2U
#define CANTP_C_AR_PATCH_VERSION  2U
#define CANTP_C_SW_MAJOR_VERSION  2U
#define CANTP_C_SW_MINOR_VERSION  0U
#define CANTP_C_SW_PATCH_VERSION  0U

/*=======[V E R S I O N  C H E C K]===========================================*/
/*check version information with CanTp.h*/

#if ((CANTP_C_AR_MAJOR_VERSION != CANTP_H_AR_MAJOR_VERSION)\
   ||(CANTP_C_AR_MINOR_VERSION != CANTP_H_AR_MINOR_VERSION)\
   ||(CANTP_C_AR_PATCH_VERSION != CANTP_H_AR_PATCH_VERSION)\
   ||(CANTP_C_SW_MAJOR_VERSION != CANTP_H_SW_MAJOR_VERSION)\
   ||(CANTP_C_SW_MINOR_VERSION != CANTP_H_SW_MINOR_VERSION)\
   ||(CANTP_C_SW_PATCH_VERSION != CANTP_H_SW_PATCH_VERSION))

#error " CanTp.c version mismatching with CanTp.h"

#endif
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANTP_START_SEC_VAR_POWER_ON_INIT_8
#include "CanTp_MemMap.h"
/*CANTP global variable for module state:CANTP_ON, CANTP_OFF*/
volatile VAR(CanTp_StateType, CANTP_VAR_POWER_ON_INIT) CanTp_ModuleState = CANTP_OFF;
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_8
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTp_MemMap.h"
/*CANTP global variable for runtime channel controlling information*/
VAR(CanTp_ConnectionChannelType, CANTP_VAR_NOINIT) CanTp_Channels[CANTP_CHANNEL_NUMBER];
#define CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTp_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

static FUNC(void, CANTP_CODE)
CanTp_GetTimeSpan(
uint32 TimerPtr,
P2VAR(uint32, AUTOMATIC, CANTP_APPL_DATA) TimeSpanPtr
);

/*Try to accept the transmit request.*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_AcceptTransmitRequest(
	uint8 channelId,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
    );
static FUNC(void, CANTP_CODE)
CanTp_InitChannel(uint8 ChannelIdx);

/*Handle TX event occurred to the specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_DispatchTxEvent(uint8 ChannelIdx);

/*Handle back ground states to the specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleBackGroudStates(uint8 ChannelIdx);

/*Handle RX event occurred to the specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_DispatchRxEvent(uint8 ChannelIdx);

/* Handle the timer for specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleTimers(uint8 ChannelIdx);

/*Handle the timer for RX channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleRxTimer(uint8 ChannelIdx);

/*Handle the timer for TX channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleTxTimer(uint8 ChannelIdx);

/*Update Rx ABC timer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_UpdateRxABCTimer(uint8 ChannelIdx);

/*Update Tx ABC timer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_UpdateTxABCTimer(uint8 ChannelIdx);

#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"


/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/*
 * Brief               This function initializes the CanTp module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_Init(
        P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr)
{
    uint8 channelIdx;
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        /*invalid transmit request information pointer*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_INIT, CANTP_E_PARAM_POINTER);
    }
    else
#endif
    {
        SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*lock state*/
        /*if the module is working, turn off it, then it can not receive or
         transmit new N-SDUs*/
        if (CANTP_ON == CanTp_ModuleState)
        {
            CanTp_ModuleState = CANTP_OFF;
        }
        SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*unlock state*/

        SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
        /*Initialize all the channels to stop all the working channels*/
        for (channelIdx = 0; channelIdx < CANTP_CHANNEL_NUMBER; channelIdx++)
        {
            CanTp_Channels[channelIdx].ChannelMode = CfgPtr->CanTpChannel[channelIdx].ChannelMode;
            CanTp_InitChannel(channelIdx);
        }

        SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

        /*turn the switch of module state on*/
        SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*lock state*/
        /*CanTp start to work*/
        CanTp_ModuleState = CANTP_ON;
        SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*unlock state*/
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This function to shutdown the CanTp module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_Shutdown(void)
{
    SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*lock state*/
    /*CanTp stops working*/
    CanTp_ModuleState = CANTP_OFF;
    SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_STATE); /*unlock state*/
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This service is used to request the transfer of segmented data.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be transmitted.
 *                     CanTpTxInfoPtr indicator of a structure with CAN N-SDU related
 *                                    data:indicator of a CAN N-SDU buffer and the length
 *                                    of this buffer.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:request can be started successfully
 *                     E_NOT_OK:request cannot be started
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_Transmit(
        PduIdType CanTpTxSduId,
P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;
    uint8 channelId;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_UNINIT);
    }
    else if (NULL_PTR == CanTpTxInfoPtr)
    {
        /*invalid transmit request information pointer*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_PARAM_POINTER);
    }
    else
#endif
    { /*information pointer OK*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTpTxSduId, &channelId);
        if (NULL_PTR != txNSduCfgPtr)
        {
            /*A configured TX SDU matched. Try to accepted the transmit request*/
            result = CanTp_AcceptTransmitRequest(channelId, txNSduCfgPtr, CanTpTxInfoPtr);
        } /*END OF if (NULL_PTR != txNSduCfgPtr)*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        else
        {
            /*No configured TX SDU matched with this TxSduId*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TX_ID);
        }
#endif
    } /* END OF if (CANTP_ON == CanTp_RuntimeControl.ModuleState)*/
    return result;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_TC == STD_ON)
/*
 * Brief               This service is used to cancel the transfer of pending
 *                     CAN N-SDUs. The connection is identified by CanTpTxPduId.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be canceled.
 *                     CanTpCancelReason The reason for cancelation.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_CancelTransmit(PduIdType CanTpTxSduId)
{
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;
    uint8 chnIdx;

    /*check module state, handle the request only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        /*Get configuration of the specified TX-NSDU*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(CanTpTxSduId, &chnIdx);
        if (NULL_PTR == txNSduCfgPtr)
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /*Invalid TxNSduId, no configuration matched*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_PARAM_ID);
#endif
        }
        else if (CANTP_TC_MASK == (txNSduCfgPtr->CanTpPduFlag & CANTP_TC_MASK))
        {
			if ((CANTP_LARGE_TRANSMITTING != CanTp_Channels[chnIdx].TxConnection.RootState)
				&& (CANTP_SF_TRANSIMITTING != CanTp_Channels[chnIdx].TxConnection.RootState))
			{
				CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST,
				        CANTP_E_OPER_NOT_SUPPORTED);
			}
			else
			{ /*A matched configuration matched*//*lock channel*/
				SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL);
				if (CanTp_Channels[chnIdx].TxConnection.NSduId == CanTpTxSduId)
				{
					/*The TX-NSDU is being transmitted*/
					CanTp_Channels[chnIdx].TxConnection.EventFlags =
					        CANTP_CHANNEL_EVENT_TXCANCELLED;
					/*turn off the sub-state to reject the
					 * following events for this channel transmitting*/
					CanTp_Channels[chnIdx].TxConnection.Substate = CANTP_SUBSTATE_NONE;
					result = E_OK;
				}
				SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL);/*unlock channel*/
			} /*END OF A matched configuration matched*/
        }
    } /* END OF if (CANTP_ON == CanTp_RuntimeControl.ModuleState)*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    else
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_UNINIT);
    }
#endif
    return result;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This service is used to cancel the reception of an ongoing N-SDU.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      This parameter contains the unique CanTp module identifier
 *                     of the N-SDU to be cancelled for transmission.
 *                     Range: 0..(maximum number of L-PDU IDs received ) - 1
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_CancelReceive(PduIdType CanTpRxSduId)
{
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;
    uint8 chnIdx;

    /*check module state, handle the request only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
        /*Get configuration of the specified TX-NSDU*/
        rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(CanTpRxSduId, &chnIdx);

        if (NULL_PTR == rxNSduCfgPtr)
        {
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            /*Invalid RxNSduId, no configuration matched*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_PARAM_ID);
#endif
        }
        else
        { /*A matched configuration matched*/
            SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/

            if (CANTP_LARGE_RECEIVING == CanTp_Channels[chnIdx].RxConnection.RootState)
            {
            	/*last Consecutive Frame can not cancel*/
				if ((CanTp_Channels[chnIdx].RxConnection.NSduId == CanTpRxSduId)
					&& (CanTp_Channels[chnIdx].RxConnection.SduDataRemaining > 0u))
				{
					CanTp_Channels[chnIdx].RxConnection.EventFlags =
					        CANTP_CHANNEL_EVENT_RXCANCELLED;
					/*turn off the sub-state to reject the
					 * following events for this channel receiving*/
					CanTp_Channels[chnIdx].RxConnection.Substate = CANTP_SUBSTATE_NONE;
					result = E_OK;
				}
            }
            else
            {
            	CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST,
            	        CANTP_E_OPER_NOT_SUPPORTED);
            }
            SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/
        } /*END OF A matched configuration matched*/
    } /* END OF if (CANTP_ON == CanTp_RuntimeControl.ModuleState)*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    else
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_UNINIT);
    }
#endif
    return result;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif

#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_CHANGE_PARAMETER == STD_ON)
FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_ChangeParameter(
	PduIdType id,
	TPParameterType parameter,
	uint16 value)
{
	Std_ReturnType result = E_NOT_OK;
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
	uint8 chnIdx;

	rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(id, &chnIdx);
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
	/*check module state, handle only when module started*/
	if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_UNINIT);
    }
	else if ((FALSE == CanTP_CheckRxChangeParameter(parameter, value))
			||(NULL_PTR == rxNSduCfgPtr))
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_PARAM_ID);
    }
    else /*All request parameters are valid*/
#endif
    {
        if (CanTp_Channels[chnIdx].RxConnection.RootState != CANTP_LARGE_RECEIVING)
        {
            if (TP_BS == parameter)
            {
                CanTp_Channels[chnIdx].RxConnection.ChangedBs = (uint8)value;
                CanTp_Channels[chnIdx].RxConnection.EventFlags |= CANTP_CHANNEL_EVENT_RXBSCHANGED;
                result = E_OK;
            }
            else if (TP_STMIN == parameter)
            {
                CanTp_Channels[chnIdx].RxConnection.ChangedSTmin = (uint8)value;
                CanTp_Channels[chnIdx].RxConnection.EventFlags |=
                        CANTP_CHANNEL_EVENT_RXSTMINCHANGED;
                result = E_OK;
            }
            else
            {
                /*idle*/
            }
        }
    }

	return result;
}
#endif

#if (CANTP_READ_PARAMETER == STD_ON)
FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_ReadParameter(
	PduIdType id,
	TPParameterType parameter,
	uint16* value)
{
	Std_ReturnType result = E_NOT_OK;
	P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxNSduCfgPtr;
	uint8 chnIdx;

	rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(id, &chnIdx);

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
	/*check module state, handle only when module started*/
	if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_UNINIT);
    }
	else if (((parameter != TP_STMIN)
				&&(parameter != TP_BS))
			||(NULL_PTR == rxNSduCfgPtr))
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_PARAM_ID);
    }
    else if (NULL_PTR == value)
    {
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_PARAM_POINTER);
    }
    else /*All request parameters are valid*/
#endif
    {
        if (TP_BS == parameter)
        {
            *value = (uint16)CanTp_Channels[chnIdx].RxConnection.CurrentBs;
            result = E_OK;
        }
        else if (TP_STMIN == parameter)/* TP_STMIN == parameter */
        {
            if ((CanTp_Channels[chnIdx].RxConnection.EventFlags &
                    CANTP_CHANNEL_EVENT_RXSTMINCHANGED) != 0u)
            {
                /*STmin have been changed*/
                *value = (uint16)CanTp_Channels[chnIdx].RxConnection.ChangedSTmin;
            }
            else
            {
                if (rxNSduCfgPtr->STmin != NULL_PTR)
                {
                    *value = *(rxNSduCfgPtr->STmin);
                }
                else
                {
                    *value = 0;
                }
            }
            result = E_OK;
        }
        else
        {
            /*idle*/
        }
    }

	return result;
}
#endif
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*
 * Brief               The main function for scheduling the CANTP.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp module initialized
 * CallByAPI           SchM
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_MainFunction(void)
{
    uint8 channelIdx;
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_UNINIT);
    }
    else
#endif
    {
        for (channelIdx = 0; channelIdx < CANTP_CHANNEL_NUMBER; channelIdx++)
        {
            SchM_Enter_CanTp(CANTP_MODULE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
            /*handle channel timers*/
            CanTp_HandleTimers(channelIdx);

            /*handle receive events*/
            CanTp_DispatchRxEvent(channelIdx);

            /*handle transmit events*/
            CanTp_DispatchTxEvent(channelIdx);

            /*handle back ground states*/
            CanTp_HandleBackGroudStates(channelIdx);
            SchM_Exit_CanTp(CANTP_MODULE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/
        }
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_VERSION_INFO_API == STD_ON)
/*
 * Brief               This function return the version information of the CANTP module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     versioninfo indicator as to where to store the
 * version information of this module.
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) versioninfo
    )
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        /*report development error*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_GETVERSIONINFO, CANTP_E_PARAM_POINTER);
    }
    else
#endif
    { /*NULL_PTR != versioninfo*/
        versioninfo->moduleID = CANTP_MODULE_ID;
        versioninfo->sw_major_version = CANTP_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANTP_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANTP_C_SW_PATCH_VERSION;
        versioninfo->vendorID = CANTP_VENDOR_ID;
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif

/*=============Module Internal Function Implementation====================*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/*Initialize the Rx specific channel.*/
FUNC(void, CANTP_CODE)
CanTp_InitRxChannel(uint8 ChannelIdx)
{
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[ChannelIdx];

    /*initialize channel state information of RxConnection*/
    channelPtr->RxConnection.RootState = CANTP_IDLE;
    channelPtr->RxConnection.Substate = CANTP_SUBSTATE_NONE;
    channelPtr->RxConnection.HandleType = CANTP_FTYPE_RESEVED;

    /*initialize channel ABC timer*/
    channelPtr->RxConnection.ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
    channelPtr->RxConnection.ChannelTimer.RemainTime = 0;
    channelPtr->RxConnection.ChannelTimer.StartTime = 0;

    /*initialize channel flow control parameter*/
    channelPtr->RxConnection.CurrentBs = 0;
#if (STD_ON == CANTP_FD)
    channelPtr->RxConnection.FDDlc = 0;
#endif
    channelPtr->RxConnection.ChangedBs = 0;
    channelPtr->RxConnection.ChangedSTmin = 0;
    channelPtr->RxConnection.CurrentCfSn = 0;
    channelPtr->RxConnection.HandledCfCount = 0;
    channelPtr->RxConnection.SentWftCount = 0;
    channelPtr->RxConnection.STminTimer.Started = FALSE;
    channelPtr->RxConnection.STminTimer.RemainTime = 0;
    channelPtr->RxConnection.STminTimer.StartTime = 0;
    channelPtr->RxConnection.STminTimer.FcSTMin = 0;

    /*initialize SDU information*/
    channelPtr->RxConnection.NSduId = 0;
    channelPtr->RxConnection.RcvdNPduId = 0;
    channelPtr->RxConnection.RcvdNSduId = 0;
    channelPtr->RxConnection.SduDataRemaining = 0;
    channelPtr->RxConnection.SduDataTotalCount = 0;

    /*initialize event flags*/
    channelPtr->RxConnection.EventFlags = 0;

    /*initialize buffer information*/
    channelPtr->RxConnection.LocalDataLen = 0;
    channelPtr->RxConnection.LocalDataPosition = 0;
    channelPtr->RxConnection.UpperBufRemaining = 0;

#if ((CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON) || (CANTP_DYN_ID_SUPPORT == STD_ON))
    /*initialize address information of SDU with MetaData*/
    channelPtr->RxConnection.MetaDataN_TA = 0;
    channelPtr->RxConnection.MetaDataN_SA = 0;
    channelPtr->RxConnection.MetaDataN_AE = 0;
    channelPtr->RxConnection.MetaDataLength = 0;
#endif
}

/*Initialize the Tx specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_InitTxChannel(uint8 ChannelIdx)
{
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[ChannelIdx];

	/*full duplex need to inital another connection*/
	/*initialize channel state information*/
	channelPtr->TxConnection.RootState = CANTP_IDLE;
	channelPtr->TxConnection.Substate = CANTP_SUBSTATE_NONE;
	channelPtr->TxConnection.HandleType = CANTP_FTYPE_RESEVED;

	/*initialize channel ABC timer*/
	channelPtr->TxConnection.ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
	channelPtr->TxConnection.ChannelTimer.RemainTime = 0;
	channelPtr->TxConnection.ChannelTimer.StartTime = 0;

	/*initialize channel flow control parameter*/
	channelPtr->TxConnection.CurrentBs = 0;
#if (STD_ON == CANTP_FD)
    channelPtr->TxConnection.FDDlc = 0;
#endif
	channelPtr->TxConnection.ChangedBs = 0;
	channelPtr->TxConnection.ChangedSTmin = 0;
	channelPtr->TxConnection.CurrentCfSn = 0;
	channelPtr->TxConnection.HandledCfCount = 0;
	channelPtr->TxConnection.SentWftCount = 0;
	channelPtr->TxConnection.STminTimer.Started = FALSE;
	channelPtr->TxConnection.STminTimer.RemainTime = 0;
	channelPtr->TxConnection.STminTimer.StartTime = 0;
	channelPtr->TxConnection.STminTimer.FcSTMin = 0;

	/*initialize SDU information*/
	channelPtr->TxConnection.NSduId = 0;
	channelPtr->TxConnection.RcvdNPduId = 0;
	channelPtr->TxConnection.RcvdNSduId = 0;
	channelPtr->TxConnection.SduDataRemaining = 0;
	channelPtr->TxConnection.SduDataTotalCount = 0;

	/*initialize event flags*/
	channelPtr->TxConnection.EventFlags = 0;

	/*initialize buffer information*/
	channelPtr->TxConnection.LocalDataLen = 0;
	channelPtr->TxConnection.LocalDataPosition = 0;
	channelPtr->TxConnection.UpperBufRemaining = 0;

#if ((CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON) || (CANTP_DYN_ID_SUPPORT == STD_ON))
	/*initialize address information of SDU with MetaData*/
	channelPtr->TxConnection.MetaDataN_TA = 0;
	channelPtr->TxConnection.MetaDataN_SA = 0;
    channelPtr->TxConnection.MetaDataN_AE = 0;
	channelPtr->TxConnection.MetaDataLength = 0;
#endif
}

/*Initialize the specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_InitChannel(uint8 ChannelIdx)
{
    CanTp_InitRxChannel(ChannelIdx);
    CanTp_InitTxChannel(ChannelIdx);
}
/*Utility function: Set memory with specific value.*/
FUNC(void, CANTP_CODE)
CanTp_MemorySet(
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Address,
    uint8 Value,
    uint16 Length)
{
    uint16 DL;
    for (DL = 0; DL < Length; DL++)
    {
        Address[DL] = Value;
    }
}
/*=================static Function Implementation==============================*/
/*Try to accept the transmit request.*/
static FUNC(Std_ReturnType, CANTP_CODE)
CanTp_AcceptTransmitRequest(
	uint8 channelId,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
    )
{
    Std_ReturnType result = E_NOT_OK;
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    /*check sdu data total length of the transmit request*/
    if (E_NOT_OK == CanTp_CheckTxLengthInAddressMode(TxNSduCfgPtr, CanTpTxInfoPtr))
    {
        /*check TxTaType type, invalid*/
    	CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TATYPE);
    }
    else /*All request parameters are valid*/
    {
        /*check if the channel is free*/
        channelPtr = &CanTp_Channels[channelId];

        SchM_Enter_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*lock channel*/
        if((channelPtr->ChannelMode == CANTP_MODE_HALF_DUPLEX)
			&&(channelPtr->RxConnection.RootState != CANTP_IDLE ))
		{
			/*half duplex, if Rx is in processing, the same channel can not
			 * transmit data at the same time*/
			result = E_NOT_OK;
		}
        else if((CANTP_IDLE == channelPtr->TxConnection.RootState)
        		&& (0u == channelPtr->TxConnection.EventFlags))
        {
            /*channel is free, and no event happened to it*/
            channelPtr->TxConnection.EventFlags = CANTP_CHANNEL_EVENT_TRANSMITREQUEST;
            channelPtr->TxConnection.NSduId = TxNSduCfgPtr->TxNSduId;
            channelPtr->TxConnection.SduDataRemaining = CanTpTxInfoPtr->SduLength;
            channelPtr->TxConnection.SduDataTotalCount = CanTpTxInfoPtr->SduLength;
            #if (CANTP_DYN_ID_SUPPORT == STD_ON)
            channelPtr->TxConnection.MetaDataLength =
                    (TxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
            #endif

			#if (CANTP_DYN_ID_SUPPORT == STD_ON)
            if ((channelPtr->TxConnection.MetaDataLength > 0u)
                    && ((CANTP_ADDRESS_EXTENSION_8_MASK ==
                            (TxNSduCfgPtr->CanTpPduFlag & CANTP_ADDRESS_EXTENSION_8_MASK))
                            || (CANTP_SA16_AND_TA16_MASK ==
                                    (TxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK))))
            {
            	CanTp_SaveTxMetaDataInfo(channelId, TxNSduCfgPtr, CanTpTxInfoPtr);
            }
			#endif

			/*set channel as sender to avoid unexpected
			 *  PDU reception,here can not make sure of SF*/
			channelPtr->TxConnection.RootState = CANTP_SF_TRANSIMITTING;

            result = E_OK;
        }
        else
        {
            /*idle*/
        }
        SchM_Exit_CanTp(CANTP_INSTANCE_ID, CANTP_EXCLUSIVE_AREA_CHANNEL); /*unlock channel*/

    }
    return result;
}

/*Handle back ground states to the specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleBackGroudStates(uint8 ChannelIdx)
{
    /*Handle background operation for channel ,if needed*/
    switch (CanTp_Channels[ChannelIdx].RxConnection.Substate)
    {
        case CANTP_RX_SF_START:
        {
            /*receiving SF*/
            CanTp_RxHandleSFStart(ChannelIdx);
            break;
        }
        case CANTP_RX_FF_START:
		{
			/*receiving large SDU, trying to get buffer*/
			CanTp_RxHandleFFStart(ChannelIdx);
			break;
		}
        case CANTP_RX_LARGE_START:
        {
            /*receiving large SDU, trying to get buffer*/
            CanTp_RxHandleLargeStart(ChannelIdx);
            break;
        }
        default:
            /*No special handling needed*/
            break;
    } /*END OF switch (CanTp_Channels[ChannelIdx].RxConnection->Substate)*/

    /*Handle background operation for channel ,if needed*/
	switch (CanTp_Channels[ChannelIdx].TxConnection.Substate)
	{
		case CANTP_TX_SF_START:
		{
			/*transmitting SF, trying to get buffer*/
			CanTp_TxHandleSFStart(ChannelIdx);
			break;
		}
		case CANTP_TX_LARGE_START:
		{
			/*transmitting large SDU, trying to get buffer*/
			CanTp_TxHandleLargeStart(ChannelIdx);
			break;
		}
		default:
			/*No special handling needed*/
			break;
	} /*END OF switch (CanTp_Channels[ChannelIdx].TxConnectionPtr->Substate)*/
}

/*************************************************************************/
/*Handle TX event occurred to the specific channel.*/
/*************************************************************************/
static FUNC(void, CANTP_CODE)
CanTp_DispatchTxEvent(uint8 ChannelIdx)
{
    P2CONST(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[ChannelIdx];

    if ((channelPtr->TxConnection.EventFlags & CANTP_CHANNEL_EVENT_TXCANCELLED) != 0u)
    {
        /*A transmit request canceled*/
        CanTp_TxHandleTransmitCancel(ChannelIdx);
    }

    if ((channelPtr->TxConnection.EventFlags & CANTP_CHANNEL_EVENT_TRANSMITREQUEST) != 0u)
    {
        /*A transmit request accepted*/
        CanTp_TxHandleTransmitReq(ChannelIdx);
    }
}


/*Handle RX event occurred to the specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_DispatchRxEvent(uint8 ChannelIdx)
{
    P2VAR(CanTp_ConnectionChannelType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) channelPtr;

    channelPtr = &CanTp_Channels[ChannelIdx];

    if ((channelPtr->RxConnection.EventFlags & CANTP_CHANNEL_EVENT_CF_RECEIVED) != 0u)
    {
        /*A CF frame received*/
        if ((CANTP_LARGE_RECEIVING == channelPtr->RxConnection.RootState)
            && (CANTP_RX_LARGE_V4CF == channelPtr->RxConnection.Substate))
        {
            /*A CF frame received*/
            CanTp_RxHandleCFReception(ChannelIdx);
        }
        else
        {
            channelPtr->RxConnection.EventFlags =
             channelPtr->RxConnection.EventFlags & ((uint16)~CANTP_CHANNEL_EVENT_CF_RECEIVED);
        }
    }

    if ((channelPtr->TxConnection.EventFlags & CANTP_CHANNEL_EVENT_FC_RECEIVED) != 0u)
    {
        if ((CANTP_LARGE_TRANSMITTING == channelPtr->TxConnection.RootState)
            && (CANTP_TX_LARGE_V4FC == channelPtr->TxConnection.Substate))
        {
            /*A FC frame received*/
            CanTp_TxHandleFCReception(ChannelIdx);
        }
        else
        {
            channelPtr->TxConnection.EventFlags =
            	channelPtr->TxConnection.EventFlags & ((uint16)~CANTP_CHANNEL_EVENT_FC_RECEIVED);
        }
    }

    if ((channelPtr->RxConnection.EventFlags & CANTP_CHANNEL_EVENT_FF_RECEIVED) != 0u)
    {
        /*A FF frame received*/
        CanTp_RxHandleFFReception(ChannelIdx);
    }

    if ((channelPtr->RxConnection.EventFlags & CANTP_CHANNEL_EVENT_SF_RECEIVED) != 0u)
    {
        /*A SF frame received*/
        CanTp_RxHandleSFReception(ChannelIdx);
    }

    if ((channelPtr->RxConnection.EventFlags & CANTP_CHANNEL_EVENT_RXCANCELLED) != 0u)
	{
    	/*A receive request canceled*/
    	CanTp_RxHandleReceiveCancel(ChannelIdx);
	}
}

/*Handle the timer for specific channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleTimers(uint8 ChannelIdx)
{
    if (CANTP_IDLE != CanTp_Channels[ChannelIdx].RxConnection.RootState)
    {
        if ((CANTP_SF_RECEIVING == CanTp_Channels[ChannelIdx].RxConnection.RootState)
            || (CANTP_LARGE_RECEIVING == CanTp_Channels[ChannelIdx].RxConnection.RootState))
        {
            /*channel is receiving SDU*/
            CanTp_HandleRxTimer(ChannelIdx);
        }
    } /*ENDOF if (CANTP_IDLE != CanTp_Channels[ChannelIdx].RxConnection->RootState)*/

    if (CANTP_IDLE != CanTp_Channels[ChannelIdx].TxConnection.RootState)
    {
        if((CANTP_SF_TRANSIMITTING == CanTp_Channels[ChannelIdx].TxConnection.RootState)
        	|| (CANTP_LARGE_TRANSMITTING == CanTp_Channels[ChannelIdx].TxConnection.RootState))
        {
            /*channel is transmitting SDU*/
            CanTp_HandleTxTimer(ChannelIdx);
        }
    }
}

/* Handle the timer for RX channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleRxTimer(uint8 ChannelIdx)
{
	Std_ReturnType ntfRslt = E_NOT_OK;
	uint8 chnIdx;
    PduIdType iPduId;
    PduIdType nSduId;
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) rxSduCfgPtr;

    /*Handle ABC timer*/
	if (CANTP_TIMER_NONE != CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer)
	{
		/*update the time information*/
		CanTp_UpdateRxABCTimer(ChannelIdx);

		if (0u == CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime)
		{
			/*Channel timer timeout occurred*/
			nSduId = CanTp_Channels[ChannelIdx].RxConnection.NSduId;
			rxSduCfgPtr = CanTp_GetRxSduCfgByNSduId(nSduId, &chnIdx);
			if (NULL_PTR != rxSduCfgPtr)
			{
			    iPduId = rxSduCfgPtr->RxIPduId;

                if (CANTP_NA == CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer)
                {
                    /*Nar timeout*/
                    ntfRslt = E_NOT_OK;
                }
                else if (CANTP_NC ==
                        CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer)
                {
                    /*Ncr timeout*/
                    ntfRslt = E_NOT_OK;
                }
                else
                {
                    /*other timer timeout, result is NTFRSLT_E_NOT_OK*/
                }

                /*CANTP_NB deal in LargeStart*/
                if (CANTP_NB != CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer)
                {
                    /*release resource of channel*/
                	CanTp_ReleaseRxChannel(ChannelIdx);
                    /*notify upper*/
                    /*receiving failed*/
                    PduR_CanTpRxIndication(iPduId, ntfRslt);
                }

                    /*report development error*/
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_COM);
			}
		} /*ENDOF channel timer timeout*/
	}
}

/*Handle the timer for TX channel.*/
static FUNC(void, CANTP_CODE)
CanTp_HandleTxTimer(uint8 ChannelIdx)
{
    uint32 elapsedTick;
    Std_ReturnType ntfRslt = E_NOT_OK;
    PduIdType iPduId;
    PduIdType nSduId;
    uint8 channelId;
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txSduCfgPtr;

    /*Handle ABC timer*/
	if (CANTP_TIMER_NONE != CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer)
	{
        /*update the time information*/
        CanTp_UpdateTxABCTimer(ChannelIdx);

        if (0u == CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime)
        {
            /*Channel timer timeout occurred*/
        	nSduId = CanTp_Channels[ChannelIdx].TxConnection.NSduId;
			txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(nSduId, &channelId);
			if (NULL_PTR != txSduCfgPtr)
			{
                iPduId = txSduCfgPtr->TxIPduId;

                if (CANTP_NA == CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer)
                {
                    /*Nas timeout*/
                    ntfRslt = E_NOT_OK;
                }
                else if (CANTP_NB ==
                        CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer)
                {
                    /*Nbs timeout*/
                    ntfRslt = E_NOT_OK;
                }
                else
                {
                    /*other timer timeout, result is NTFRSLT_E_NOT_OK*/
                }
                /*CANTP_NC deal in LargeStart*/
                if (CANTP_NC != CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer)
                {
                    /*release resource of channel*/
                    CanTp_ReleaseTxChannel(ChannelIdx);
                    /*notify upper*/
                    /*transmitting failed*/
                    PduR_CanTpTxConfirmation(iPduId, ntfRslt);
                }
                /*report development error*/
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_COM);
			}
        } /*ENDOF channel timer timeout*/
	}

    /*handle STmin timer for transmitter if needed*/
    if ((CANTP_LARGE_TRANSMITTING == CanTp_Channels[ChannelIdx].TxConnection.RootState)
        && (TRUE == CanTp_Channels[ChannelIdx].TxConnection.STminTimer.Started))
    {
        CanTp_GetTimeSpan(CanTp_Channels[ChannelIdx].TxConnection.STminTimer.StartTime,
                &elapsedTick);
        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.STminTimer.StartTime));
            
        if (CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime <= elapsedTick)
        {
            CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime = 0;
        }
        else
        {
            CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime =
            		CanTp_Channels[ChannelIdx].TxConnection.STminTimer.RemainTime -elapsedTick;
        }
    } /*END OF STMin timer handling*/
}

/*Update Rx ABC timer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_UpdateRxABCTimer(uint8 ChannelIdx)
{
    uint32 elapsedTick;

    /*update the time information*/
    if (CANTP_TIMER_NONE != CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.EnabledTimer)
    {
        CanTp_GetTimeSpan(CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.StartTime,
                &elapsedTick);
        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.StartTime));
        
        if (CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime <= elapsedTick)
        {
            CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime = 0;

        }
        else
        {
            CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime =
            		CanTp_Channels[ChannelIdx].RxConnection.ChannelTimer.RemainTime - elapsedTick;
        }
    } /*ENDOF update timer information*/
}

/*Update Tx ABC timer for channel.*/
static FUNC(void, CANTP_CODE)
CanTp_UpdateTxABCTimer(uint8 ChannelIdx)
{
    uint32 elapsedTick;

    /*update the time information*/
    if (CANTP_TIMER_NONE != CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.EnabledTimer)
    {
        CanTp_GetTimeSpan(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime,
                &elapsedTick);
        CanTp_ResetTime(&(CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.StartTime));

        if (CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime <= elapsedTick)
        {
            CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime = 0;
        }
        else
        {
            CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime =
            		CanTp_Channels[ChannelIdx].TxConnection.ChannelTimer.RemainTime - elapsedTick;
        }
    } /*ENDOF update timer information*/
}

extern uint32 Diag_Timer;

FUNC(void, CANTP_CODE)
CanTp_ResetTime(
P2VAR(uint32, AUTOMATIC, CANTP_APPL_DATA) TimerPtr
)
{
    *TimerPtr = Diag_Timer;
}

static FUNC(void, CANTP_CODE)
CanTp_GetTimeSpan(
uint32 TimerPtr,
P2VAR(uint32, AUTOMATIC, CANTP_APPL_DATA) TimeSpanPtr
)
{
    if (Diag_Timer >= TimerPtr)
    {
        *TimeSpanPtr = Diag_Timer - TimerPtr;
    }
    else
    {
        *TimeSpanPtr = (0xFFFFFFFF - TimerPtr) + Diag_Timer;
    }
}

FUNC(void, CANTP_CODE)
CanTp_ReleaseRxChannel(uint8 ChannelIdx)
{
    if (CanTp_Channels[ChannelIdx].ChannelMode != CANTP_MODE_FULL_DUPLEX)
    {
        CanTp_InitChannel(ChannelIdx);
    }
    else
    {
        CanTp_InitRxChannel(ChannelIdx);
    }
}

FUNC(void, CANTP_CODE)
CanTp_ReleaseTxChannel(uint8 ChannelIdx)
{
    if (CanTp_Channels[ChannelIdx].ChannelMode != CANTP_MODE_FULL_DUPLEX)
    {
        CanTp_InitChannel(ChannelIdx);
    }
    else
    {
        CanTp_InitTxChannel(ChannelIdx);
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
