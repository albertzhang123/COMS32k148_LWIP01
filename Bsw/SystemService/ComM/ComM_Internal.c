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
**  FILENAME    : ComM_Internal.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V2.0.0       2020-07-24  Wanglili       R19_11 initial version.
 *  V2.1.1       2022-03-17  darren.zhang   
 *      1.delete invalid function returns
 *      2.add nm state macro switch and schm protect
*/
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComM_Cfg.h"
#include "ComM.h"
#include "SchM_ComM.h"
#include "ComM_Internal.h"
#if (STD_ON == COMM_BUS_CAN_USED)
#include "CanSM_ComM.h"
#endif /*end of #if (STD_ON == COMM_BUS_CAN_USED)*/
#if (STD_ON == COMM_BUS_ETH_USED)
#include "EthSM.h"
#endif /*end of #if (STD_ON == COMM_BUS_ETH_USED)*/
#if (STD_ON == COMM_BUS_LIN_USED)
#include "LinSM.h"
#endif /*end of #if (STD_ON == COMM_BUS_LIN_USED)*/
#if (STD_ON == COMM_BUS_FLEXRAY_USED)
#include "FrSM_ComM.h"
#endif /*end of #if (STD_ON == COMM_BUS_FLEXRAY_USED)*/
#if (STD_ON == COMM_BSWM_USED)
#include "BswM_ComM.h"
#endif
#if (STD_ON == COMM_NM_USED)
#include "ComM_Nm.h"
#include "Nm.h"
#endif /*end of #if (STD_ON == COMM_NM_USED)*/
#if (STD_ON == COMM_DCM_USED)
#include "Dcm_Cbk.h"
#endif /* STD_ON == COMM_DCM_USED */

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define MINFULLCOMMODE_MAINFUNCTIONTIME(Channel) \
    COMM_T_MIN_FULL_COM_MODE_DURATION/(ComM_ChannelConfig[Channel].ComMMainFunctionPeriod)

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* Request internal communication mode. */
static FUNC(Std_ReturnType, COMM_CODE)
ComM_IntRequestComMode(NetworkHandleType Channel, ComM_ModeType ComMode);

/* Get internal commnication mode currently. */
static FUNC(Std_ReturnType, COMM_CODE) ComM_IntGetCurrentComMode
(
    NetworkHandleType Channel,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComModePtr
);

#if (STD_ON == COMM_PNC_SUPPORT)
/* Handle Pnc mode.*/
static FUNC(void, COMM_CODE) ComM_PncNoComHandler(PNCHandleType Pnc, NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_PncRequestedHandler(PNCHandleType Pnc, NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_PncReadySleepHandler(PNCHandleType Pnc, NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_PncPrepareSleepHandler(PNCHandleType Pnc, NetworkHandleType Channel);

/* Enter to Pnc mode*/
static FUNC(void, COMM_CODE) ComM_EnterPncNoCom(PNCHandleType Pnc, NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_EnterPncRequested(PNCHandleType Pnc, NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_EnterPncReadySleep(PNCHandleType Pnc, NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_EnterPncPrepareSleep(PNCHandleType Pnc, NetworkHandleType Channel);
#endif/*end of  #if (STD_ON == COMM_PNC_SUPPORT)*/

/* Handle different mode. */
static FUNC(void, COMM_CODE) ComM_NetworkRequestedHandler(NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_ReadySleepHandler(NetworkHandleType Channel);
#if (STD_ON == COMM_NM_USED)
static FUNC(void, COMM_CODE) ComM_SilentComHandler(NetworkHandleType Channel);
#endif /*end of #if (STD_ON == COMM_NM_USED)*/
static FUNC(void, COMM_CODE) ComM_NoComHandler(NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_NoComPendingHandler(NetworkHandleType Channel);

/* Enter to different mode. */
static FUNC(void, COMM_CODE) ComM_EnterNoCom(NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_EnterNoComPending(NetworkHandleType Channel);
#if (STD_ON == COMM_NM_USED)
static FUNC(void, COMM_CODE) ComM_EnterSilentCom(NetworkHandleType Channel);
#endif /*end of #if (STD_ON == COMM_NM_USED)*/
static FUNC(void, COMM_CODE) ComM_EnterNetworkRequested(NetworkHandleType Channel);
static FUNC(void, COMM_CODE) ComM_EnterReadySleep(NetworkHandleType Channel);

#if (STD_ON == COMM_PNC_SUPPORT)
static FUNC(boolean, COMM_CODE) ComM_AllPncUserRequestNoCom(PNCHandleType Pnc);
static FUNC(boolean, COMM_CODE) ComM_CheckPncPrepareSleepTimeExpired(PNCHandleType Pnc);
#endif/*end of  #if (STD_ON == COMM_PNC_SUPPORT) */

#if ((STD_ON == COMM_0PNC_VECTOR_AVOIDANCE) && (STD_ON == COMM_VERSION_R19_11))
FUNC(void, COMM_CODE) ComM_0PncVector_Avoidance(
    NetworkHandleType Channel);
#endif /*end of #if ((STD_ON == COMM_0PNC_VECTOR_AVOIDANCE) && (STD_ON == COMM_VERSION_R19_11))*/

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
#if (STD_ON == COMM_PNC_SUPPORT)
/* Defines run-time variable of pnc mode handler. */
CONST(ComM_PncModeHandleType, COMM_CONST) ComM_PncModeHandle[4u] =
{
    &ComM_PncNoComHandler,
    &ComM_PncPrepareSleepHandler,
    &ComM_PncReadySleepHandler,
    &ComM_PncRequestedHandler
};
#endif/*end of  #if (STD_ON == COMM_PNC_SUPPORT)*/

/* Defines run-time variable of state manager. */
CONST(ComM_SmHandleType, COMM_CONST) ComM_SmHandle[6u] =
{
    /* COMM_BUS_TYPE_CAN = 0 */
#if (STD_ON == COMM_BUS_CAN_USED)
    {&CanSM_RequestComMode, &CanSM_GetCurrentComMode},
#else
    {NULL_PTR, NULL_PTR},
#endif /*end of #if (STD_ON == COMM_BUS_CAN_USED)*/

    /*COMM_BUS_TYPE_CDD = 1*/
#if (STD_ON == COMM_BUS_CDD_USED)
    {&CddSM_RequestComMode, &CddSM_GetCurrentComMode},
#else
    {NULL_PTR, NULL_PTR},
#endif /*end of #if (STD_ON == COMM_BUS_CDD_USED)*/

    /*COMM_BUS_TYPE_ETH = 2*/
#if (STD_ON == COMM_BUS_ETH_USED)
    {&EthSM_RequestComMode, &EthSM_GetCurrentComMode},
#else
    {NULL_PTR, NULL_PTR},
#endif /*end of #if (STD_ON == COMM_BUS_ETH_USED)*/

    /* COMM_BUS_TYPE_FR = 3*/
#if (STD_ON == COMM_BUS_FLEXRAY_USED)
    {&FrSM_RequestComMode, &FrSM_GetCurrentComMode},
#else
    {NULL_PTR, NULL_PTR},
#endif /*end of #if (STD_ON == COMM_BUS_FLEXRAY_USED)*/

    /* COMM_BUS_TYPE_INTERNAL = 4 */
    {&ComM_IntRequestComMode, &ComM_IntGetCurrentComMode},

    /* COMM_BUS_TYPE_LIN = 5 */
#if (STD_ON == COMM_BUS_LIN_USED)
    {&LinSM_RequestComMode, &LinSM_GetCurrentComMode}
#else
    {NULL_PTR, NULL_PTR}
#endif /*end of #if (STD_ON == COMM_BUS_LIN_USED)*/
};

/* Defines run-time variable of mode handler. */
CONST(ComM_ModeHandleType, COMM_CONST) ComM_ModeHandle[5u] =
{
    &ComM_NoComHandler,
    &ComM_NoComPendingHandler,
    &ComM_NetworkRequestedHandler,
    &ComM_ReadySleepHandler,
#if (STD_ON == COMM_NM_USED)
    &ComM_SilentComHandler
#else
    NULL_PTR
#endif /*end of #if (STD_ON == COMM_NM_USED)*/
};
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"
/* Defines global run-time variable */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
VAR(ComM_GlobalType, COMM_VAR) ComM_Global;
#endif /*end of #if ((COMM_WAKEUP_INHIBITION_ENABLED==STD_ON) || (COMM_MODE_LIMITATION_ENABLED==STD_ON))*/

/* Defines run-time variable of all users */
VAR(ComM_UserType, COMM_VAR) ComM_User[COMM_NUMBER_OF_USERS];

/* Defines run-time variable of all channels */
VAR(ComM_ChannelType, COMM_VAR) ComM_Channel[COMM_NUMBER_OF_CHANNELS];

/* bus nm indication */
VAR(ComM_ModeType, COMM_VAR) ComM_BusNMModInd[COMM_NUMBER_OF_CHANNELS];

VAR(boolean, COMM_VAR) ComM_EcuToNoCom;

#if (STD_ON == COMM_PNC_SUPPORT)
/* Defines run-time variable of all Pncs */
VAR(ComM_PncType, COMM_VAR) ComM_Pnc[COMM_NUMBER_OF_PNCS];

VAR(ComM_PncSignalType, COMM_VAR) ComM_PncSignalbuffer[COMM_NUMBER_OF_TXSIGNAL];
#endif/*end of #if (STD_ON == COMM_PNC_SUPPORT)*/


#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"
P2CONST(ComM_ConfigType, AUTOMATIC, COMM_CONST) ComM_ConfigPtr;
#define COMM_STOP_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Determine whether the current channel can be requested
 *                     by user FULL_COM due to communication suppression.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)||(STD_ON == COMM_MODE_LIMITATION_ENABLED))
FUNC(Std_ReturnType, COMM_CODE) ComM_UserRequestFullCom(NetworkHandleType Channel)
{
    Std_ReturnType ret = E_OK;
    #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
    switch(ComM_Channel[Channel].State)
    {

        case COMM_NO_COM_NO_PENDING_REQUEST:
        case COMM_NO_COM_REQUEST_PENDING:
        case COMM_SILENT_COM:
            if (COMM_IS_INHIBIT_WAKEUP(Channel))
            {
                ret = COMM_E_MODE_LIMITATION;
                if (ComM_Global.InhibitCounter < COMM_MAX_REJECT_COUNTER)
                {
                    ComM_Global.InhibitCounter++;
                }
            }
            break;
        default:
        /*do nothing*/
            break;
    }
    #endif /*end of #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/
    #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
    if (
        (COMM_FULL_COM_NETWORK_REQUESTED != ComM_Channel[Channel].State)
        &&COMM_IS_LIMIT_TO_NOCOM(Channel))
    {
        ret = COMM_E_MODE_LIMITATION;
        if(ComM_Global.InhibitCounter < COMM_MAX_REJECT_COUNTER)
        {
            ComM_Global.InhibitCounter++;
        }
    }
    #endif /*end of #if (STD_ON == COMM_MODE_LIMITATION_ENABLED) */
    return ret;
}
#endif

/******************************************************************************/
/*
 * Brief               Receive the notification of the success of the lower-level
 *                     state switch, notify the DCM.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE)  ComM_BusModeIndication(NetworkHandleType Channel)
{
    switch(ComM_Channel[Channel].CurrentMode)
    {
        case COMM_NO_COMMUNICATION:
            #if (STD_ON == COMM_DCM_USED)
            Dcm_ComM_NoComModeEntered((uint8)Channel);
            #endif /*end of #if (COMM_DCM_USED == STD_ON)*/
        #if (COMM_RESET_AFTER_FORCING_NO_COMM == STD_ON)
        #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
        if(COMM_IS_LIMIT_TO_NOCOM(Channel))
        {
            (void)BswM_ComM_InitiateReset();
        }
        #endif /* end of if (STD_ON == COMM_MODE_LIMITATION_ENABLED) */
        #endif /* end of #if (COMM_RESET_AFTER_FORCING_NO_COMM == STD_ON)*/
            break;
        #if (COMM_DCM_USED == STD_ON)
        case COMM_SILENT_COMMUNICATION:
            Dcm_ComM_SilentComModeEntered((uint8)Channel);
            break;
        case COMM_FULL_COMMUNICATION:
            Dcm_ComM_FullComModeEntered((uint8)Channel);
            break;
        #endif /* end of #if (COMM_DCM_USED == STD_ON)*/
        default:
            /*This logic does not require any processing*/
            break;
    }
}

/******************************************************************************/
/*
* Brief               Mode switches with the NM.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different channel.
* Param-Name[in]      Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
#if (STD_ON == COMM_NM_USED)
FUNC(void, COMM_CODE) ComM_NmModeProcessing(NetworkHandleType Channel)
{
    switch(ComM_Channel[Channel].CurrentMode)
    {
        case COMM_SILENT_COMMUNICATION:
            if(ComM_Channel[Channel].specNmMode == NM_MODE_NETWORK)
            {
                ComM_EnterReadySleep(Channel);
            }
            else if(ComM_Channel[Channel].specNmMode == NM_MODE_BUS_SLEEP)
                {
                    ComM_EnterNoCom(Channel);
                }
            else
            {
                /* nothing */
            }
            break;
        case COMM_FULL_COMMUNICATION:
            if (((COMM_FULL == ComM_ChannelConfig[Channel].ComMNmVariant)
                 || (COMM_PASSIVE == ComM_ChannelConfig[Channel].ComMNmVariant))
                 && (ComM_Channel[Channel].specNmMode == NM_MODE_PREPARE_BUS_SLEEP))
                {
                    ComM_EnterSilentCom(Channel);
                }
            else if(ComM_Channel[Channel].specNmMode == NM_MODE_BUS_SLEEP)
                {
                    ComM_EnterNoCom(Channel);
                }
            else
            {
                /* nothing */
            }
            break;
        default:
            /*This logic does not require any processing*/
            break;
    }
}
#endif

/******************************************************************************/
/*
 * Brief               use the corresponding function provided by RTE to indicate
 *                     modes to the users.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if (STD_ON == COMM_RTE_NOTIFY_USED)
FUNC(void, COMM_CODE) ComM_UserModeNotify(NetworkHandleType Channel)
{
    uint8 user;
    P2VAR(ComM_UserType, AUTOMATIC, COMM_APPL_DATA) pUser;
    ComM_ModeType NewMode;
    uint8 ArrayIndex;
    for(user = 0; user < ComM_ChannelConfig[Channel].NumofUserRefToChannel; user++)
    {
        ArrayIndex = ComM_ChannelConfig[Channel].UserRefToChannelMap[user];
        pUser = &ComM_User[ArrayIndex];
        /*because mode change,old mode num--,new mode num++*/
        pUser->ChCounter[ComM_Channel[Channel].PreviousMode]--;
        pUser->ChCounter[ComM_Channel[Channel].CurrentMode]++;
        if (pUser->ChCounter[COMM_NO_COMMUNICATION] > 0x00u)
        {
            NewMode = COMM_NO_COMMUNICATION;
        }
        else if (pUser->ChCounter[COMM_SILENT_COMMUNICATION] > 0x00u)
        {
            NewMode = COMM_SILENT_COMMUNICATION;
        }
        else
        {
            NewMode = COMM_FULL_COMMUNICATION;
        }
        if (pUser->OldMode != NewMode)
        {
            pUser->OldMode = NewMode;
            if (NULL_PTR != ComM_UserConfig[ArrayIndex].RteNotify)
            {
                ComM_UserConfig[ArrayIndex].RteNotify((uint8)NewMode);
            }
        }
    }
    ComM_Channel[Channel].PreviousMode = ComM_Channel[Channel].CurrentMode;
}
#endif

/******************************************************************************/
/*
* Brief               Forward the signal received on the active channel to the
*                     passive channel.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc.
* Param-Name[in]      Pnc
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
#if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
FUNC(void, COMM_CODE) 
ComM_PncGatewaySignalExchange(PNCHandleType Pnc)
{
    if ((ComM_Pnc[Pnc].UserFullCounter > 0u)
        || ((COMM_GATEWAY_TYPE_ACTIVE == ComM_ChannelConfig[ComM_Pnc[Pnc].PncSignalChannelId].ComMPncGateway)
            && (ComM_Pnc[Pnc].ERACounter > 0u)))
    {
        uint32 chIndex,signalIndex;
        uint8 ChArrayIndex;
        for(chIndex=0u; chIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfChannelRefToPnc; chIndex++)
        {
            ChArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].ChannelRefToPncMap[chIndex];
            if(COMM_GATEWAY_TYPE_PASSIVE == ComM_ChannelConfig[ChArrayIndex].ComMPncGateway)
            {
                for (signalIndex = 0u; signalIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfSignalRefToPnc; signalIndex++)
                {
                    uint8 SglArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].SignalRefToPncMap[signalIndex];
                    if (SIG_TX == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalDirection)
                    {
                        COMM_SET_PNC_IN_SIGNAL(
                            ComM_ConfigPtr->pComMPncCfg[Pnc].ComM_PncId,
                            ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer);
                        #if (STD_ON == COMM_COM_USED)
                        (void)Com_SendSignal(ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalId,
                                &ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer[0]);
                        #endif
                    }
                }
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief              judge if the ComMPncGatewayType of all channels associated
 *                    with this pnc is not set,because although master switch
 *                    COMM_PNC_GATEWAY_ENABLED = STD_ON, not all channels
 *                    ComMPncGatewayType is set
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc.
* Param-Name[in]      Pnc
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              boolean
*/
/******************************************************************************/
FUNC(boolean, COMM_CODE)
ComM_JudgePncGatewayIsSet(PNCHandleType Pnc)
{
    uint8 Channel,ChArrayIndex;
    boolean Gateway_IsSet = FALSE;
    for(Channel = 0; Channel < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfChannelRefToPnc; Channel++)
    {
        ChArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].ChannelRefToPncMap[Channel];
        if(COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[ChArrayIndex].ComMPncGateway)
        {
            Gateway_IsSet = TRUE;
            break;
        }
    }
    return Gateway_IsSet;
}
#endif/* STD_ON == COMM_PNC_GATEWAY_ENABLED */

/******************************************************************************/
/*
* Brief               Request internal communication mode.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Channel.
* Param-Name[in]      Channel,ComMode
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              Std_ReturnType
*/
/******************************************************************************/
static FUNC(Std_ReturnType, COMM_CODE) ComM_IntRequestComMode
(
    NetworkHandleType Channel,
    ComM_ModeType ComMode
)
{
    (void)ComMode;
    COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_REQUEST);
    COMM_EVENT_SET(Channel, COMM_EVENT_BUS_OK);
    return E_OK;
}

/******************************************************************************/
/*
* Brief               Get internal communication mode currently.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Channel.
* Param-Name[in]      Channel,ComModePtr
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              Std_ReturnType
*/
/******************************************************************************/
static FUNC(Std_ReturnType, COMM_CODE) ComM_IntGetCurrentComMode
(
    NetworkHandleType Channel,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComModePtr
)
{
    *ComModePtr = ComM_Channel[Channel].CurrentMode;
    return E_OK;
}

/******************************************************************************/
/*
* Brief               All users under the Pnc are in NO_COM.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc.
* Param-Name[in]      Pnc
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              boolean
*/
/******************************************************************************/
#if (STD_ON == COMM_PNC_SUPPORT)
static FUNC(boolean, COMM_CODE) ComM_AllPncUserRequestNoCom(PNCHandleType Pnc)
{
    uint8 index;
    boolean ret = TRUE;
    uint8 ArrayIndex;
    for (index = 0u; index < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfUserRefToPnc; index++)
    {
        ArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].UserRefToPncMap[index];
        if (COMM_NO_COMMUNICATION != ComM_User[ArrayIndex].RequestMode)
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}

/******************************************************************************/
/*
* Brief               Check if the PncPrepareSleep timer expires.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc.
* Param-Name[in]      Pnc
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              boolean
*/
/******************************************************************************/
static FUNC(boolean, COMM_CODE) ComM_CheckPncPrepareSleepTimeExpired(PNCHandleType Pnc)
{
    boolean flag = FALSE;
    if(ComM_Pnc[Pnc].PncPrepareSleepTime > 0u)
    {
        ComM_Pnc[Pnc].PncPrepareSleepTime--;
        if (((boolean)TRUE == ComM_Pnc[Pnc].PncPrepareSleepTimeStart)
            &&(0u == ComM_Pnc[Pnc].PncPrepareSleepTime))
        {
            flag = TRUE;
            ComM_Pnc[Pnc].PncPrepareSleepTimeStart = FALSE;
        }
    }
    if(0u == ComM_Pnc[Pnc].PncPrepareSleepTime)
    {
        flag = TRUE;
        ComM_Pnc[Pnc].PncPrepareSleepTimeStart = FALSE;
    }
    return flag;
}



#if ((STD_ON == COMM_0PNC_VECTOR_AVOIDANCE) && (STD_ON == COMM_VERSION_R19_11))
/******************************************************************************/
/*
* Brief               passive channel shall release the channel if
*                     ComM0PncVectorAvoidance = TRUE
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
FUNC(void, COMM_CODE)
ComM_0PncVector_Avoidance(
    NetworkHandleType Channel)
{
    boolean AllPncRefToChannelNoSgl = TRUE;
    uint8 PncIndex,PncArrayIndex;
    for(PncIndex = 0; PncIndex < ComM_ChannelConfig[Channel].NumofPncRefToChannel; PncIndex++)
    {
        PncArrayIndex = ComM_ChannelConfig[Channel].PncRefToChannelMap[PncIndex];
        if(0u != ComM_Pnc[PncArrayIndex].ERACounter)
        {
            AllPncRefToChannelNoSgl = FALSE;
            break;
        }
    }
    if((boolean)TRUE == AllPncRefToChannelNoSgl)
    {
        /*@req R19-11 <SWS_ComM_00966>
         *if all signals are set to '0' for the referenced ComMChannel,release the channel */
        ComM_Channel[Channel].UserFullCounter = 0;
    }
}
#endif /*end of #if ((STD_ON == COMM_0PNC_VECTOR_AVOIDANCE) && (STD_ON == COMM_VERSION_R19_11))*/

/******************************************************************************/
/*
* Brief               PNC_NO_COMMUNICATION state handler function
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_PncNoComHandler(PNCHandleType Pnc, NetworkHandleType Channel)
{
    if ((ComM_Pnc[Pnc].UserFullCounter > 0u)
    /*@req  R19-11 <SWS_ComM_00934>
     *add ComMPncGatewayType is set*/
        #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
        || ((ComM_Pnc[Pnc].ERACounter > 0u)
                && (COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[Channel].ComMPncGateway))
        #endif
       )
    {
        ComM_EnterPncRequested(Pnc, Channel);
    }
    else if (ComM_Pnc[Pnc].EIRACounter > 0u)
    {
        ComM_EnterPncReadySleep(Pnc, Channel);
    }
    #if (STD_ON == COMM_SYNCHRONOUS_WAKE_UP)
    else if (COMM_PNC_EVENT_IS_SET(Pnc, COMM_EVENT_ECUM_WAKEUP))
    {
        /*@req <SWS_ComM_00931>  @req <SWS_ComM_00990>*/
        ComM_EnterPncPrepareSleep(Pnc,Channel);
    }
    #endif/*end of #if (STD_ON == COMM_SYNCHRONOUS_WAKE_UP)*/
    #if (STD_ON == COMM_VERSION_R19_11)
    else if(COMM_PNC_EVENT_IS_SET(Pnc,COMM_EVENT_ECUM_PNCWAKEUP))
    {
        /*@req R19-11 <SWS_ComM_00964>*/
        ComM_EnterPncPrepareSleep(Pnc,Channel);
    }
    #endif /*ennd of #if (STD_ON == COMM_VERSION_R19_11)*/
    else
    {
        /*do nothing*/
    }
}

/******************************************************************************/
/*
* Brief               PNC_REQUESTED state handler function
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_PncRequestedHandler(PNCHandleType Pnc, NetworkHandleType Channel)
{
    #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
    if(COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[ComM_Pnc[Pnc].PncSignalChannelId].ComMPncGateway)
    {
        uint8 signalIndex;
        /*SWS_ComM_00164*/
        if ((ComM_Pnc[Pnc].UserFullCounter > 0u)
            || ((COMM_GATEWAY_TYPE_ACTIVE == ComM_ChannelConfig[ComM_Pnc[Pnc].PncSignalChannelId].ComMPncGateway)
                && (ComM_Pnc[Pnc].ERACounter > 0u)))
        {
            uint8 chIndex;
            for(chIndex = 0; chIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfChannelRefToPnc; chIndex++)
            {
                if ((Channel == ComM_ConfigPtr->pComMPncCfg[Pnc].ChannelRefToPncMap[chIndex])
                    && (COMM_GATEWAY_TYPE_PASSIVE == ComM_ChannelConfig[Channel].ComMPncGateway))
                {
                    for (signalIndex = 0u; signalIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfSignalRefToPnc; signalIndex++)
                    {
                        uint8 SglArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].SignalRefToPncMap[signalIndex];
                        if (SIG_TX == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalDirection)
                        {
                            COMM_SET_PNC_IN_SIGNAL(
                                ComM_ConfigPtr->pComMPncCfg[Pnc].ComM_PncId,
                                    ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer);
                            #if (STD_ON == COMM_COM_USED)
                            (void)Com_SendSignal(ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalId,
                                &ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer[0]);
                            #endif/* STD_ON == COMM_COM_USED*/
                        }
                    }
                    break;
                }
            }
        }
        /*SWS_ComM_00959*/
        if (((boolean)TRUE == ComM_AllPncUserRequestNoCom(Pnc))
            && (0u == ComM_Pnc[Pnc].ERACounter)
            && (COMM_GATEWAY_TYPE_ACTIVE == ComM_ChannelConfig[ComM_Pnc[Pnc].PncSignalChannelId].ComMPncGateway))
        {
            uint8 chIndex;
            for(chIndex = 0; chIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfChannelRefToPnc; chIndex++)
            {
                if ((Channel == ComM_ConfigPtr->pComMPncCfg[Pnc].ChannelRefToPncMap[chIndex])
                    && (COMM_GATEWAY_TYPE_PASSIVE == ComM_ChannelConfig[Channel].ComMPncGateway))
                {
                    for (signalIndex = 0u; signalIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfSignalRefToPnc; signalIndex++)
                    {
                        uint8 SglArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].SignalRefToPncMap[signalIndex];
                        if (SIG_TX == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalDirection)
                        {
                            COMM_SET_PNC_IN_SIGNAL(
                                ComM_ConfigPtr->pComMPncCfg[Pnc].ComM_PncId,
                                    ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer);
                            #if (STD_ON == COMM_COM_USED)
                            (void)Com_SendSignal(ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalId,
                                    &ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer[0]);
                            #endif/*end of #if (STD_ON == COMM_COM_USED)*/
                            #if ((STD_ON == COMM_0PNC_VECTOR_AVOIDANCE) && (STD_ON == COMM_VERSION_R19_11))
                            ComM_0PncVector_Avoidance(Channel);
                            #endif
                        }
                    }
                }
            }
        }
        /*@req R19-11 update <SWS_ComM_00991>*/
        if (((boolean)TRUE == ComM_AllPncUserRequestNoCom(Pnc))
            && (0u == ComM_Pnc[Pnc].ERACounter)
             &&(COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[Channel].ComMPncGateway))
        {
            ComM_EnterPncReadySleep(Pnc, Channel);
        }
        /*@req R19-11 update <SWS_ComM_00938>
         *ComMPncGatewayEnabled = TRUE and on all channels the PNC belongs to the ComMPncGatewayType is not set
         *enter Pnc Ready Sleep state*/
        if (((boolean)TRUE == ComM_AllPncUserRequestNoCom(Pnc)) &&
               ((boolean)FALSE == ComM_JudgePncGatewayIsSet(Pnc)))
        {
             ComM_EnterPncReadySleep(Pnc, Channel);
        }
    }
    #else
    /*SWS_ComM_00938*/
    if (ComM_AllPncUserRequestNoCom(Pnc))
    {
        ComM_EnterPncReadySleep(Pnc, Channel);
    }
    #endif/*end of #if(STD_ON == COMM_PNC_GATEWAY_ENABLED)*/
}

/******************************************************************************/
/*
* Brief               PNC_READY_SLEEP state handler function
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_PncReadySleepHandler(PNCHandleType Pnc, NetworkHandleType Channel)
{
    /*@req R19-11 update <SWS_ComM_00165>*/
    if ((ComM_Pnc[Pnc].UserFullCounter > 0u)
    #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
        || ((ComM_Pnc[Pnc].ERACounter > 0u)
              && (COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[Channel].ComMPncGateway))
    #endif
       )
    {
        ComM_EnterPncRequested(Pnc, Channel);
    }
    else if (0u == ComM_Pnc[Pnc].EIRACounter)
    {
        /*SWS_ComM_00940*/
        ComM_EnterPncPrepareSleep(Pnc,Channel);
    }
    else
    {
        /*do nothing*/
    }
}

/******************************************************************************/
/*
* Brief               PNC_PREPARE_SLEEP state handler function
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_PncPrepareSleepHandler(PNCHandleType Pnc, NetworkHandleType Channel)
{
    /*SWS_ComM_00948,SWS_ComM_00951*/
    if ((ComM_Pnc[Pnc].UserFullCounter > 0u)
    #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
        || ((ComM_Pnc[Pnc].ERACounter > 0u)
               && (COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[Channel].ComMPncGateway))
    #endif
       )
    {
        ComM_Pnc[Pnc].PncPrepareSleepTime = 0u;
        ComM_Pnc[Pnc].PncPrepareSleepTimeStart = FALSE;
        ComM_EnterPncRequested(Pnc, Channel);
    }
    else if (ComM_Pnc[Pnc].EIRACounter > 0u)
    {
        /*SWS_ComM_00950*/
        ComM_Pnc[Pnc].PncPrepareSleepTime = 0;
        ComM_Pnc[Pnc].PncPrepareSleepTimeStart = FALSE;
        ComM_EnterPncReadySleep(Pnc, Channel);
    }
    else if((boolean)TRUE == ComM_CheckPncPrepareSleepTimeExpired(Pnc))
    {
        /*SWS_ComM_00947*/
        ComM_EnterPncNoCom(Pnc, Channel);
    }
    else
    {
        /*This logic does not require any processing*/
    }
}

/******************************************************************************/
/*
* Brief               Status switch to PNC_NO_COMMUNICATION
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterPncNoCom(PNCHandleType Pnc, NetworkHandleType Channel)
{
    (void)Channel;
    ComM_Pnc[Pnc].PreviousPncMode = ComM_Pnc[Pnc].CurrentPncMode;
    ComM_Pnc[Pnc].CurrentPncMode = COMM_PNC_NO_COMMUNICATION;
    ComM_Pnc[Pnc].PncState = COMM_STATE_PNC_NO;
    #if (STD_ON == COMM_BSWM_USED)
    BswM_ComM_CurrentPNCMode(Pnc, ComM_Pnc[Pnc].CurrentPncMode);
    #endif /*end of if (STD_ON == COMM_BSWM_USED) */
}

/******************************************************************************/
/*
* Brief               Status switch to PNC_REQUESTED
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterPncRequested(PNCHandleType Pnc, NetworkHandleType Channel)
{
    uint8 signalIndex;
    uint8 ChIndex;

    ComM_Pnc[Pnc].PreviousPncMode = ComM_Pnc[Pnc].CurrentPncMode;
    ComM_Pnc[Pnc].CurrentPncMode = COMM_PNC_REQUESTED;
    for (signalIndex = 0u; signalIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfSignalRefToPnc; signalIndex++)
    {
        boolean SetPncSingalFlag = FALSE;
        uint8 SglArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].SignalRefToPncMap[signalIndex];
        if (SIG_TX == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalDirection)
        {
            /*@req R19-11 <SWS_ComM_00930> ComMPncGatewayEnabled = FALSE  set pnc signal*/
            #if (STD_OFF == COMM_PNC_GATEWAY_ENABLED)
            /*Set the bit to '1' in ComM_PncSignalbuffer.SignalBuffer*/
            SetPncSingalFlag = TRUE;
            #else
            /*@req R19-11 <SWS_ComM_00930> ComMPncGatewayEnabled = TRUE but on all
            ComMChannels the PNC belongs to the ComMPncGatewayType is not set,set pnc signal*/
            if((boolean)FALSE == ComM_JudgePncGatewayIsSet(Pnc))
            {
                /*Set the bit to '1' in ComM_PncSignalbuffer.SignalBuffer*/
                SetPncSingalFlag = TRUE;
            }
            else/*ERA*/
            {
                uint8 PNCIndex;
                for(PNCIndex = 0; PNCIndex < ComM_ChannelConfig[Channel].NumofPncRefToChannel; PNCIndex++)
                {
                    /*@req <SWS_ComM_00992> ERA set active channel signal*/
                    if ((Pnc == ComM_ChannelConfig[Channel].PncRefToChannelMap[PNCIndex])
                       && (ComM_ChannelConfig[Channel].ComMPncGateway == COMM_GATEWAY_TYPE_ACTIVE))
                    {
                        SetPncSingalFlag = TRUE;
                    }
                }
            }
            #endif/*end of #if (STD_OFF == COMM_PNC_GATEWAY_ENABLED)*/
            if((boolean)TRUE == SetPncSingalFlag)
            {
                COMM_SET_PNC_IN_SIGNAL(
                    ComM_ConfigPtr->pComMPncCfg[Pnc].ComM_PncId,
                        ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer);
                #if (STD_ON == COMM_COM_USED)
                (void)Com_SendSignal(ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalId,
                    &ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer[0]);
                #endif/*end of #if (STD_ON == COMM_COM_USED)*/
            }
        }
    }
    for(ChIndex = 0u; ChIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfChannelRefToPnc; ChIndex++)
    {
        uint8 ArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].ChannelRefToPncMap[ChIndex];
        ComM_Channel[ArrayIndex].UserFullCounter++;
        if(((boolean)TRUE == ComM_ChannelConfig[Channel].ComMPncNmRequest)
        &&(COMM_FULL_COM_NETWORK_REQUESTED == ComM_Channel[Channel].State))
        {
            if(COMM_FULL == ComM_ChannelConfig[Channel].ComMNmVariant)
            {
#if (STD_ON == COMM_NM_USED)
                (void)Nm_NetworkRequest(Channel);
#endif/* (STD_ON == COMM_NM_USED) */
            }
        }
    }
    ComM_Pnc[Pnc].PncState = COMM_STATE_PNC_REQ;
    #if (STD_ON == COMM_BSWM_USED)
    BswM_ComM_CurrentPNCMode(Pnc, ComM_Pnc[Pnc].CurrentPncMode);
    #endif /*end of if (STD_ON == COMM_BSWM_USED) */
}

/******************************************************************************/
/*
* Brief               Status switch to PNC_READY_SLEEP
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterPncReadySleep(PNCHandleType Pnc, NetworkHandleType Channel)
{
    (void)Channel;
    uint8 signalIndex;
    uint8 ChannelIndex;
    ComM_Pnc[Pnc].PreviousPncMode = ComM_Pnc[Pnc].CurrentPncMode;
    ComM_Pnc[Pnc].CurrentPncMode = COMM_PNC_READY_SLEEP;
    /*SWS_ComM_00960*/
    if (COMM_PNC_REQUESTED == ComM_Pnc[Pnc].PreviousPncMode)
    {
        for (signalIndex = 0u; signalIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfSignalRefToPnc; signalIndex++)
        {
            uint8 SglArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].SignalRefToPncMap[signalIndex];
            if (SIG_TX == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalDirection)
            {
                COMM_CLEAR_PNC_IN_SIGNAL( 
                        ComM_ConfigPtr->pComMPncCfg[Pnc].ComM_PncId,
                            ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer);
                #if (STD_ON == COMM_COM_USED)
                (void)Com_SendSignal(ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalId,
                        &ComM_PncSignalbuffer[ComM_PncComSignalConfig[SglArrayIndex].TxSignalBufferIndex].SignalBuffer[0]);
                #endif/*end of #if (STD_ON == COMM_COM_USED)*/
            }
        }
        /*[SWS_ComM_00961]*/
        for(ChannelIndex = 0u; ChannelIndex < ComM_ConfigPtr->pComMPncCfg[Pnc].NumOfChannelRefToPnc; ChannelIndex++)
        {
            uint8 ArrayIndex;
            ArrayIndex = ComM_ConfigPtr->pComMPncCfg[Pnc].ChannelRefToPncMap[ChannelIndex];
            if (0U < ComM_Channel[ArrayIndex].UserFullCounter)
            {
                ComM_Channel[ArrayIndex].UserFullCounter--;
            }
        }
    }
    ComM_Pnc[Pnc].PncState = COMM_STATE_PNC_READY_SLEEP;
    #if (STD_ON == COMM_BSWM_USED)
    BswM_ComM_CurrentPNCMode(Pnc, ComM_Pnc[Pnc].CurrentPncMode);
    #endif /*end of if (STD_ON == COMM_BSWM_USED) */
}

/******************************************************************************/
/*
* Brief               Status switch to PNC_PREPARE_SLEEP
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Pnc and Channel.
* Param-Name[in]      Pnc,Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              None
*/
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterPncPrepareSleep(PNCHandleType Pnc, NetworkHandleType Channel)
{
    ComM_Pnc[Pnc].PreviousPncMode = ComM_Pnc[Pnc].CurrentPncMode;
    ComM_Pnc[Pnc].CurrentPncMode = COMM_PNC_PREPARE_SLEEP;
    ComM_Pnc[Pnc].PncPrepareSleepTime =
            COMM_PNC_PREPARE_SLEEP_TIMER/ComM_ChannelConfig[Channel].ComMMainFunctionPeriod;
    /*SWS_ComM_00952*/
    ComM_Pnc[Pnc].PncPrepareSleepTimeStart = TRUE;
    #if (STD_ON == COMM_SYNCHRONOUS_WAKE_UP)
    COMM_PNC_EVENT_CLR(Pnc, COMM_EVENT_ECUM_WAKEUP
            #if (STD_ON == COMM_VERSION_R19_11)
            |COMM_EVENT_ECUM_PNCWAKEUP
            #endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/
                );
    #endif
    ComM_Pnc[Pnc].PncState = COMM_STATE_PNC_PREPARE_SLEEP;
    #if (STD_ON == COMM_BSWM_USED)
    BswM_ComM_CurrentPNCMode(Pnc, ComM_Pnc[Pnc].CurrentPncMode);
    #endif /*end of if (STD_ON == COMM_BSWM_USED) */
}
#endif /*end of if (STD_ON == COMM_PNC_SUPPORT) */

#if (STD_ON == COMM_VERSION_R19_11)
/******************************************************************************/
/*
* Brief               managed channel request network,request managing
*                     channel handler function.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Channel.
* Param-Name[in]      Channel,NeedJudgePncRef,ManagedHandler
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              boolean
*/
/******************************************************************************/
FUNC(void, COMM_CODE) ComM_ManagedChannelHandler(
    NetworkHandleType Channel,
    boolean NeedJudgePncRef,
    ManagedHandlerType ManagedHandler)
{
    /*@req R19-11 <SWS_ComM_00875>
     *If the requested ComM channel is a managed channel that is not referenced by a ComMPnc, then also the
     *ComM channel state machine of the referencing managing channel shall be requested with FULL_COM.*/

    boolean ChannelRefbyPnc_Flag = FALSE;
    #if (STD_ON == COMM_PNC_SUPPORT)
    if((boolean)TRUE == NeedJudgePncRef)
    {
        /*judge this channel ref to a Pnc or not*/
        if(0u != ComM_ChannelConfig[Channel].NumofPncRefToChannel)
        {
            ChannelRefbyPnc_Flag = TRUE;
        }
    }
    #else
        (void)NeedJudgePncRef;
    #endif/*end of #if (STD_ON == COMM_PNC_SUPPORT)*/
    if((boolean)TRUE != ChannelRefbyPnc_Flag)
    {
        if(ECUMPNC_REQUEST == ManagedHandler)
        {
            /*if managed channel not ref Pnc,set managing channel network request flag*/
            COMM_EVENT_SET(ComM_ChannelConfig[Channel].ComMManageReference[0u],
                    COMM_EVENT_ECUM_PNCWAKEUP);
        }
        if(ECUM_REQUEST == ManagedHandler)
        {
            /*if managed channel not ref Pnc,set managing channel network request flag*/
            COMM_EVENT_SET(ComM_ChannelConfig[Channel].ComMManageReference[0u],
                    COMM_EVENT_ECUM_WAKEUP);
        }
        if(USER_REQUESTFULLCOM == ManagedHandler)
        {
            ComM_Channel[ComM_ChannelConfig[Channel].ComMManageReference[0u]].UserFullCounter++;
        }
        if(USER_REQUESTNOCOM == ManagedHandler)
        {
            ComM_Channel[ComM_ChannelConfig[Channel].ComMManageReference[0u]].UserFullCounter--;
        }
    }
}

/******************************************************************************/
/*
* Brief               Whether to allow managing channel release.
* Sync/Async          Synchronous
* Reentrancy          Reentrant for different Channel.
* Param-Name[in]      Channel
* Param-Name[out]     None
* Param-Name[in/out]  None
* Return              boolean
*/
/******************************************************************************/
FUNC(boolean, COMM_CODE) ComM_ManagingChannelHandler(
    NetworkHandleType Channel)
{
    boolean ManagedChannelNoUsed = TRUE;
    if(COMM_CHANNEL_TYPE_MANAGING == ComM_ChannelConfig[Channel].ComMManageType)
    {
        /*@req R19-11 <SWS_ComM_00897> note
        *A managing channel shall additionally only switch back to default sub-state
        *COMM_NO_COM_NO_PENDING_REQUEST, if also all its referenced managed channels
        *that are not referenced by a ComMPnc have no longer any valid pending request for
        COMM_FULL_COMMUNICATION.*/

        uint8 ChannelIndex;
        for(ChannelIndex = 0; ChannelIndex < ComM_ChannelConfig[Channel].ComMManageChannelNum; ChannelIndex++)
        {
            if((boolean)TRUE == ManagedChannelNoUsed)
            {
                uint8 ManagedChannelId =
                        ComM_ChannelConfig[Channel].ComMManageReference[ChannelIndex];/*managed channel ID*/
                #if (STD_ON == COMM_PNC_SUPPORT)
                /*judge this channel ref to a Pnc or not*/
                if(0u != ComM_ChannelConfig[ManagedChannelId].NumofPncRefToChannel)
                {
                    /*ref to pnc,do nothing*/
                }
                else
                {
                    if(0u != ComM_Channel[ManagedChannelId].UserFullCounter)
                    {
                        ManagedChannelNoUsed = FALSE;
                        break;
                    }
                }
                #else
                if(0u != ComM_Channel[ManagedChannelId].UserFullCounter)
                {
                    ManagedChannelNoUsed = FALSE;
                    break;
                }
                #endif/*end of #if (STD_ON == COMM_PNC_SUPPORT)*/
            }
        }
    }
    return ManagedChannelNoUsed;
}
#endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/

/******************************************************************************/
/*
 * Brief               Entered into the "No Com" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterNoCom(NetworkHandleType Channel)
{
    ComM_Channel[Channel].Events = 0u;
    ComM_Channel[Channel].State = COMM_NO_COM_NO_PENDING_REQUEST;
    ComM_Channel[Channel].LightModeTimer = 0u;

    if (ComM_Channel[Channel].CurrentMode != COMM_NO_COMMUNICATION)
    {
        ComM_Channel[Channel].UserFullCounter = 0u;
        COMM_SET_BUS_MODE(Channel, COMM_NO_COMMUNICATION);
        #if (STD_ON == COMM_RESET_AFTER_FORCING_NO_COMM)
        #if (STD_ON == COMM_BSWM_USED)
        #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
        if(COMM_IS_LIMIT_TO_NOCOM(Channel))
        {
            (void)BswM_ComM_InitiateReset();
        }
        #endif /*end of if (STD_ON == COMM_MODE_LIMITATION_ENABLED) */
        #endif /*end of  if (STD_ON == COMM_BSWM_USED) */
        #endif
    }

    if (COMM_FULL == ComM_ChannelConfig[Channel].ComMNmVariant)
    {
        #if ((STD_ON == COMM_NM_USED) && (STD_OFF == COMM_NM_PASSIVE_MODE_ENABLED))
        (void)Nm_NetworkRelease(Channel);
        #endif/* ((STD_ON == COMM_NM_USED) && (STD_OFF == COMM_NM_PASSIVE_MODE_ENABLED)) */
    }
}

/******************************************************************************/
/*
 * Brief               Handle of the "No Com" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_NoComHandler(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_BSWM_USED)
    (void)BswM_ComM_CurrentMode(Channel, COMM_NO_COMMUNICATION);
    #endif /*end of if (STD_ON == COMM_BSWM_USED) */

    if (((ComM_Channel[Channel].UserFullCounter > 0u)
    #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
         && (!COMM_IS_LIMIT_TO_NOCOM(Channel))
    #endif /*end of #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        || ((COMM_PASSIVE != ComM_ChannelConfig[Channel].ComMNmVariant)
            && (COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE)))
        #endif /*end of  #if (STD_ON == COMM_DCM_USED)*/
        #if ((STD_ON == COMM_NM_USED) || (STD_ON == COMM_ECUM_USED))
        || (COMM_EVENT_IS_SET(Channel, COMM_EVENT_PASSIVE_STARTUP))
        #endif /*end of  #if ((STD_ON == COMM_NM_USED) || (STD_ON == COMM_ECUM_USED))*/
       )
    {
        ComM_EnterNoComPending(Channel);
    }

    /*@req R19-11 <SWS_ComM_01015>*/
    #if ((STD_ON == COMM_SYNCHRONOUS_WAKE_UP)&&(STD_ON == COMM_ECUM_USED))
    if(COMM_EVENT_IS_SET(Channel, COMM_EVENT_ECUM_WAKEUP
            #if (STD_ON == COMM_VERSION_R19_11)
            |COMM_EVENT_ECUM_PNCWAKEUP
            #endif /*end of #if(STD_ON == COMM_VERSION_R19_11)*/
                 ))
    {
        uint8 index;
        for (index = 0u; index < COMM_NUMBER_OF_CHANNELS; index++)
        {
            if(Channel != index)
            {
                ComM_EnterNoComPending(index);
            }
        }
    }
    #endif/*end of #if ((STD_ON == COMM_SYNCHRONOUS_WAKE_UP)&&(STD_ON == COMM_ECUM_USED))*/
}

/******************************************************************************/
/*
 * Brief               Entered into the "No Pending" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterNoComPending(NetworkHandleType Channel)
{
    if ((boolean)TRUE == ComM_Channel[Channel].CommunicationAllowed)
    {
        ComM_EnterNetworkRequested(Channel);
    }
    else
    {
        ComM_Channel[Channel].State = COMM_NO_COM_REQUEST_PENDING;
    }
}

/******************************************************************************/
/*
 * Brief               Handle of the "No Pending" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_NoComPendingHandler(NetworkHandleType Channel)
{
    if ((boolean)TRUE == ComM_Channel[Channel].CommunicationAllowed)
    {
        if (ComM_Channel[Channel].CurrentMode != COMM_FULL_COMMUNICATION)
        {
            COMM_SET_BUS_MODE(Channel, COMM_FULL_COMMUNICATION);
        }
        if (COMM_FULL_COMMUNICATION == ComM_BusNMModInd[Channel])
        {
            ComM_EnterNetworkRequested(Channel);
        }
    }
    else if ((ComM_Channel[Channel].UserFullCounter == 0u)
    #if (STD_ON == COMM_DCM_USED)
                  || (!COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
    #endif /*end of #if (STD_ON == COMM_DCM_USED)*/
            )
    {
        #if (STD_ON == COMM_VERSION_R19_11)
        /*@req R19-11 <SWS_ComM_00897> note*/
        if(COMM_CHANNEL_TYPE_MANAGING == ComM_ChannelConfig[Channel].ComMManageType)
        {
            if((boolean)TRUE == ComM_ManagingChannelHandler(Channel))
            {
                ComM_EnterNoCom(Channel);
            }
        }
        else
        #endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/
        {
            ComM_EnterNoCom(Channel);
        }
    }
    else
    {
        /* Do nothing. */
    }
}

/******************************************************************************/
/*
 * Brief               Entered into the "NetworkRequested" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterNetworkRequested(NetworkHandleType Channel)
{
    if (ComM_Channel[Channel].CurrentMode != COMM_FULL_COMMUNICATION)
    {
        COMM_SET_BUS_MODE(Channel, COMM_FULL_COMMUNICATION);
    }


    switch (ComM_ChannelConfig[Channel].ComMNmVariant)
    {
        case COMM_FULL:
            #if (STD_ON == COMM_NM_USED)
            if ((ComM_Channel[Channel].UserFullCounter > 0u)
                     #if (STD_ON == COMM_DCM_USED)
                     || (COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
                     #endif /*end of #if (STD_ON == COMM_DCM_USED)*/
               )
            {
                #if (STD_OFF == COMM_NM_PASSIVE_MODE_ENABLED)
                if ((COMM_FULL_COMMUNICATION == ComM_BusNMModInd[Channel])
                   ||(COMM_FULL_COM_READY_SLEEP == ComM_Channel[Channel].State))
                {
#if (STD_ON == COMM_NM_USED)
                    (void)Nm_NetworkRequest(Channel);
#endif/* (STD_ON == COMM_NM_USED) */
                }
                #endif
            }

            if ((COMM_EVENT_IS_SET(Channel, COMM_EVENT_PASSIVE_STARTUP)) &&
                    (ComM_Channel[Channel].UserFullCounter == 0u))
            {
                if (COMM_FULL_COMMUNICATION == ComM_BusNMModInd[Channel])
                {
#if (STD_ON == COMM_NM_USED)
                    (void)Nm_PassiveStartUp(Channel);
#endif/* (STD_ON == COMM_NM_USED) */

                }
            }
            #endif/* STD_ON == COMM_NM_USED*/
            break;
        case COMM_PASSIVE:
        #if (STD_ON == COMM_VERSION_R19_11)
        case COMM_LINSLAVE:
        #endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/
            break;
        case COMM_LIGHT:
        case COMM_NONE:
            /*SWS_ComM_00886*/
            ComM_Channel[Channel].MinFullComModeTimer = MINFULLCOMMODE_MAINFUNCTIONTIME(Channel);
            break;
        default:
            /*This logic does not require any processing*/
            break;
    }

    if ((COMM_FULL_COMMUNICATION == ComM_BusNMModInd[Channel])
        ||(COMM_FULL_COM_READY_SLEEP == ComM_Channel[Channel].State))
    {
        ComM_Channel[Channel].State = COMM_FULL_COM_NETWORK_REQUESTED;
        ComM_BusNMModInd[Channel] = COMM_INVALID_UINT8;
        COMM_EVENT_CLR(Channel, (uint16)COMM_EVENT_PASSIVE_STARTUP);
    }
}

/******************************************************************************/
/*
 * Brief               Handle of the "NetworkRequested" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_NetworkRequestedHandler(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DCM_USED)
     if((boolean)TRUE == ComM_Channel[Channel].Dcm_ClearInd)
     {
         COMM_EVENT_CLR(Channel, (uint16)COMM_EVENT_DCM_ACTIVE);
         ComM_Channel[Channel].Dcm_ClearInd = FALSE;
     }
    #endif/*end of #if STD_ON == COMM_DCM_USED */
    /*SWS_ComM_00890*/
    #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
    if (COMM_IS_LIMIT_TO_NOCOM(Channel))
    {
        #if (STD_ON == COMM_DCM_USED)
        if (!COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        #endif /*end of #if (STD_ON == COMM_DCM_USED)*/
        {
            ComM_EnterReadySleep(Channel);
        }
    }
    #endif
    switch (ComM_ChannelConfig[Channel].ComMNmVariant)
    {
        case COMM_FULL:
            if ((ComM_Channel[Channel].UserFullCounter == 0u)

        #if (STD_ON == COMM_DCM_USED)
                        && (!COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        #endif /*end of #if(STD_ON == COMM_DCM_USED)*/
               )
            {
                #if (STD_ON == COMM_VERSION_R19_11)
                /*@req R19-11 SWS_ComM_00888
                  only all managed channel have no user request,managing channel can change to ready sleep state*/
                if((boolean)TRUE == ComM_ManagingChannelHandler(Channel))
                {
                    ComM_EnterReadySleep(Channel);
                }
                else
                #endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/
                {
                    ComM_EnterReadySleep(Channel);
                }
            }
            break;
        case COMM_PASSIVE:
            /*SWS_ComM_00915*/
            ComM_EnterReadySleep(Channel);
            break;
        #if (STD_ON == COMM_VERSION_R19_11)
        case COMM_LINSLAVE:
            /*@req R19-11 <SWS_ComM_01017>*/
            if(ComM_Channel[Channel].UserFullCounter == 0u)
            {
                ComM_EnterReadySleep(Channel);
            }
            break;
        #endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/
        case COMM_LIGHT:
        case COMM_NONE:
            if (ComM_Channel[Channel].MinFullComModeTimer > 0u)
            {
                ComM_Channel[Channel].MinFullComModeTimer--;
            }
            /*SWS_ComM_00889*/
            if ((ComM_Channel[Channel].MinFullComModeTimer == 0u)
                && (ComM_Channel[Channel].UserFullCounter == 0u)
                #if (STD_ON == COMM_DCM_USED)
                && (!COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
                #endif /*end of #if (STD_ON == COMM_DCM_USED)*/
               )
            {
                ComM_EnterReadySleep(Channel);
            }
            break;
        default:
            /*This logic does not require any processing*/
            break;
    }
}

/******************************************************************************/
/*
 * Brief               Entered into the "ReadySleep" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterReadySleep(NetworkHandleType Channel)
{
    if (ComM_Channel[Channel].CurrentMode != COMM_FULL_COMMUNICATION)
    {
        COMM_SET_BUS_MODE(Channel, COMM_FULL_COMMUNICATION);
    }
    ComM_Channel[Channel].State = COMM_FULL_COM_READY_SLEEP;
    switch (ComM_ChannelConfig[Channel].ComMNmVariant)
    {
        #if ((STD_ON == COMM_NM_USED) && (STD_OFF == COMM_NM_PASSIVE_MODE_ENABLED))
        case COMM_FULL:
            (void)Nm_NetworkRelease(Channel);
            break;
        #endif /* ((STD_ON == COMM_NM_USED) && (STD_OFF == COMM_NM_PASSIVE_MODE_ENABLED)) */
        case COMM_LIGHT:
            ComM_Channel[Channel].LightModeTimer =
                    ComM_ChannelConfig[Channel].ComMNmLightTimeout/ComM_ChannelConfig[Channel].ComMMainFunctionPeriod;
            break;
        default:
            /*This logic does not require any processing*/
            break;
    }
}

/******************************************************************************/
/*
 * Brief               Handle of the "ReadySleep" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_ReadySleepHandler(NetworkHandleType Channel)
{
    if (((ComM_Channel[Channel].UserFullCounter > 0u)
        #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        && (!COMM_IS_LIMIT_TO_NOCOM(Channel))
        #endif /*end of #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)*/
        )
        #if (STD_ON == COMM_DCM_USED)
        ||((COMM_PASSIVE != ComM_ChannelConfig[Channel].ComMNmVariant)
            &&(COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE)))
        #endif /*end of #if (STD_ON == COMM_DCM_USED)*/
        )
    {
        ComM_EnterNetworkRequested(Channel);
    }
    else
    {
        if (ComM_Channel[Channel].LightModeTimer > 0u)
        {
            ComM_Channel[Channel].LightModeTimer--;
        }
        if (((COMM_LIGHT == ComM_ChannelConfig[Channel].ComMNmVariant)
                &&(ComM_Channel[Channel].LightModeTimer == 0u))
            ||(COMM_BUS_TYPE_INTERNAL == ComM_ChannelConfig[Channel].ComMBusType)
            ||(COMM_LINSLAVE == ComM_ChannelConfig[Channel].ComMNmVariant))
        {
            ComM_EnterNoCom(Channel);
        }
        
    }

}

#if (STD_ON == COMM_NM_USED)
/******************************************************************************/
/*
 * Brief               Entered into the "Silent Com" status..
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_EnterSilentCom(NetworkHandleType Channel)
{
    COMM_SET_BUS_MODE(Channel, COMM_SILENT_COMMUNICATION);
    ComM_Channel[Channel].State = COMM_SILENT_COM;
    #if (STD_ON == COMM_BSWM_USED)
    (void)BswM_ComM_CurrentMode(Channel, COMM_SILENT_COMMUNICATION);
    #endif /*end of if (STD_ON == COMM_BSWM_USED) */
}

/******************************************************************************/
/*
 * Brief               Handle of the "Silent Com" status.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, COMM_CODE) ComM_SilentComHandler(NetworkHandleType Channel)
{
    if (((ComM_Channel[Channel].UserFullCounter > 0u)
    #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
         && (!COMM_IS_LIMIT_TO_NOCOM(Channel))
    #endif /*end of #if (COMM_MODE_LIMITATION_ENABLED == STD_ON)*/
        )
    #if (STD_ON == COMM_DCM_USED)
        || ((COMM_PASSIVE != ComM_ChannelConfig[Channel].ComMNmVariant)
            && (COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE)))
    #endif /*end of #if (STD_ON == COMM_DCM_USED)*/
       )
    {
        ComM_EnterNetworkRequested(Channel);
    }
}
#endif /*end of #if (STD_ON == COMM_NM_USED)*/

/******************************************************************************/
/*
 * Brief               judge BusSM indication Mode switch result and process.
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different channel.
 * Param-Name[in]      Channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE) ComM_BusSMIndProcess(NetworkHandleType Channel)
{
    if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_IND))
    {
        /*BusSM indication Mode switch success*/
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_OK))
        {
            #if (STD_ON == COMM_RTE_NOTIFY_USED)
            ComM_UserModeNotify(Channel);
            #endif /* end of STD_ON == COMM_RTE_NOTIFY_USED */
            ComM_BusModeIndication(Channel);
        }
        else
        {
            COMM_SET_BUS_MODE(Channel, ComM_Channel[Channel].CurrentMode);
        }
        COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_IND);
    }
}

#if (STD_ON == COMM_PNC_SUPPORT)
/******************************************************************************/
/*
 * Brief               process Rx Pnc signal,include EIRA and ERA kinds.
 * Sync/Async          Synchronous
 * Reentrancy          not Reentrant
 * Param-Name[in]      signalId,SglArrayIndex,PncIndex
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE) ComM_RxPncSignalProcess(
    uint16 signalId,
    uint8 SglArrayIndex,
    uint8 PncIndex)
{
    if(EIRA == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalKind)
    {
        ComM_Pnc[PncIndex].PncKind = EIRA;
        if ((uint8)E_OK == Com_ReceiveSignal(signalId, &ComM_Pnc[PncIndex].EIRA[0]))
        {
            if(COM_IS_PNC_SET_RXSIGNAL(
            ComM_ConfigPtr->pComMPncCfg[PncIndex].ComM_PncId,
                ComM_Pnc[PncIndex].EIRA))
            {
                ComM_Pnc[PncIndex].EIRACounter++;
            }
            else
            {
                if (ComM_Pnc[PncIndex].EIRACounter > 0u)
                {
                    ComM_Pnc[PncIndex].EIRACounter--;
                }
            }
        }
    }
    else
    {
        /*ERA*/
        ComM_Pnc[PncIndex].PncKind = ERA;
        if ((uint8)E_OK == Com_ReceiveSignal(signalId, &ComM_Pnc[PncIndex].ERA[0]))
        {
            if(COM_IS_PNC_SET_RXSIGNAL(
                ComM_ConfigPtr->pComMPncCfg[PncIndex].ComM_PncId,
                    ComM_Pnc[PncIndex].ERA))
            {
                ComM_Pnc[PncIndex].ERACounter++;
            }
            else
            {
                if (ComM_Pnc[PncIndex].ERACounter > 0u)
                {
                    ComM_Pnc[PncIndex].ERACounter--;
                }
            }
        }
    }/*else*/
}
#endif/* STD_ON == COMM_PNC_SUPPORT */
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
