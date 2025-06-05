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
**  FILENAME    : ComM.c                                                     **
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
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V1.0.0    20200113  zhengfei.li   Initial version(1.30)
 *  V1.0.1    20200114  lili.wang
 *    1.Modify the function that calls network management in ComM_Internal.c is not
 *    encapsulated by the network management macro switch.
 *    2.Ethernet compatible.
 *    3.Modified the problem that the 3e80 service request is cleared before the
 *   network is successfully opened.
 *  V1.0.2    20200521  lili.wang
 *    1. QAC check code modify
  * V1.0.3    20200522  lili.wang
 *    1. modify the bug:when one user ref to multiple channel or pnc,ComM_RequestComMode only set the
 *    first channel or pnc's UserFullCounter++.It will lead to the other channel or pnc failed to switch modes.
 *  V2.0.0    20200723  lili.wang
 *    1. Update code according AUTOSAR R19-11 Specification
 *    2. Modify code with the new coding rule
 *    3. Fix bugs find in V1.0.3
 *  V2.0.1    20201019  lili.wang
 *    1. QAC check,code update
 *  V2.0.2    20201211  lili.wang
 *    1. cantata check,code update
 *  V2.0.3    20210201  lili.wang
 *    1. delete the macro switch COMM_PNC_SUPPORT of ComM_ManagedChannelHandler and ComM_ManagingChannelHandler
 *    2. Modify the timing mechanism for MainFunction timing.
 *    3. ComM_BusSM_ModeIndication parameter ComMode is converted from pointer to variable
 *    4. Add "Rte_ComM_Type.h"
 *  V2.0.4    20210510  lili.wang
 *    1. Code update after integration test
 *  V2.0.5    20210629  lili.wang
 *    1. Solve the bug that ComMPncPrepareSleepTimer is 0 and the Pnc state machine is maintained in the
 *    state of COMM_PNC_PREPARE_SLEEP
 *  V2.0.6    20210707  lili.wang
 *    1. ComM_EnterNoComPending modify.
 *  V2.0.7    20210715  lili.wang
 *    1. Solve the bug if ComMTMinFullComModeDuration timeout,the channel state machine is maintained in the
 *    state of COMM_FULL_COM_NETWORK_REQUESTED.
 *    2.Improve the code logic for accessing the configuration parameters of the gateway when the gateway
 *    and non-gateway exist at the same time.
 *  V2.0.8    20210805  lili.wang
 *    1. ComM.Internal.c grammatical errors modify.
 *  V2.0.9    20210809  lili.wang
 *    1. When the ComM type is set to LINSLAVE, NO_COM is requested, but the request cannot be passed down.
 *  V2.1.0    20210927 darren.zhang
 *    1. modify bus nm notify comm used var cache,to solve bus nm notify slinet and normal but comm not had time to
 *    process it.then comm clear all bus notfiy state.
 *  V2.1.1    20220317 darren.zhang
 *    1.Adjust the macro switch of the interface
 *  V2.1.2    20220424 darren.zhang
 *    1.Adjust the header file inclusion order
*/
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define COMM_C_AR_RELEASE_MAJOR_VERSION  19u
#define COMM_C_AR_RELEASE_MINOR_VERSION  11u
#define COMM_C_AR_RELEASE_PATCH_VERSION  0u
#define COMM_C_SW_MAJOR_VERSION  2u
#define COMM_C_SW_MINOR_VERSION  1u
#define COMM_C_SW_PATCH_VERSION  2u
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComM.h"
#include "ComM_Internal.h"
#include "ComM_BusSM.h"
#include "SchM_ComM.h"
#if (STD_ON == COMM_COM_USED)
#include "ComM_Com.h"
#endif/* STD_ON == COMM_COM_USED */
#if (STD_ON == COMM_DCM_USED)
#include "ComM_Dcm.h"
#include "Dcm.h"
#include "Dcm_Cbk.h"
#endif /* STD_ON == COMM_DCM_USED */
#if (STD_ON == COMM_ECUM_USED)
#include "ComM_EcuM.h"
#endif/* STD_ON == COMM_ECUM_USED */
#if (STD_ON == COMM_BSWM_USED)
#include "ComM_BswM.h"
#endif/* STD_ON == COMM_BSWM_USED */
#if (STD_ON == COMM_BSWM_USED)
#include "BswM.h"
#endif/* STD_ON == COMM_BSWM_USED */
#if (STD_ON == COMM_NM_USED)
#include "ComM_Nm.h"
#include "Nm.h"
#endif /* STD_ON == COMM_NM_USED */
#if (STD_ON == COMM_NVM_USED)
#include "Nvm.h"
#endif /* STD_ON == COMM_NVM_USED */
#if (STD_ON == COMM_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == COMM_DEV_ERROR_DETECT */
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (COMM_C_AR_RELEASE_MAJOR_VERSION != COMM_H_AR_RELEASE_MAJOR_VERSION)
  #error "ComM.c : Mismatch in Specification Major Version"
#endif
#if (COMM_C_AR_RELEASE_MINOR_VERSION != COMM_H_AR_RELEASE_MINOR_VERSION)
  #error "ComM.c : Mismatch in Specification _MINOR Version"
#endif
#if (COMM_C_AR_RELEASE_PATCH_VERSION != COMM_H_AR_RELEASE_PATCH_VERSION)
  #error "ComM.c : Mismatch in Specification PATCH Version"
#endif
#if (COMM_C_SW_MAJOR_VERSION != COMM_H_SW_MAJOR_VERSION)
  #error "ComM.c : Mismatch in Software Major Version"
#endif
#if (COMM_C_SW_MINOR_VERSION != COMM_H_SW_MINOR_VERSION)
  #error "ComM.c : Mismatch in Software MINOR Version"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#define COMM_START_SEC_VAR_INIT_UNSPECIFIED
#include "ComM_MemMap.h"
/* Defines status variable of the COMM */
static VAR(ComM_InitStatusType, COMM_VAR_POWER_ON_INIT) ComM_Status = COMM_UNINIT;
#define COMM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "ComM_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the AUTOSAR Communication Manager and restarts
 *                     the internal state machines.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/* @req ComM313 ComM146 ComM793*/
FUNC(void, COMM_CODE) ComM_Init(
    P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_DATA) ConfigPtr)
{
    if (COMM_UNINIT == ComM_Status)
    {
        NetworkHandleType index;

        #if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)
        for (index = 0u; index < COMM_NUMBER_OF_CHANNELS; index++)
        {
            #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
            ComM_Global.ComMNoWakeup[index] = ComM_ChannelConfig[index].ComMNoWakeup;
            #else
            ComM_Global.ComMNoWakeup[index] = 0;
            #endif
            ComM_Global.inhStu[index] = 0x0u;
        }
        ComM_Global.InhibitCounter = 0x0u;
        ComM_Global.EcuGroupClassification = COMM_ECU_GROUP_CLASSIFICATION;
        #if (STD_ON == COMM_NVM_USED)
        /*  @SWS_ComM_00864
        ComM shall read non-volatile parameters
        specified in SWS_ComM_00103 from NVRAM. If no parameters are
        available, ComM shall use the default values in the ComM
        configuration */
        NvM_ReadBlock(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, &ComM_Global);
        #endif /* STD_ON == COMM_NVM_USED */
        #endif /* COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED */
        #if (STD_ON == COMM_DEV_ERROR_DETECT)
        if(NULL_PTR == ConfigPtr)
        {
            Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_INIT,
                    COMM_E_PARAM_POINTER);
        }
        else
        #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
        {
            ComM_ConfigPtr = ConfigPtr;
        }

        #if (STD_ON == COMM_PNC_SUPPORT)
        if ((ComM_ConfigPtr->pComMPncEnabled != NULL_PTR)
            && ((boolean)TRUE == *(ComM_ConfigPtr->pComMPncEnabled)))
        {
            for (index = 0u; index < COMM_NUMBER_OF_PNCS; index++)
            {
                ComM_Pnc[index].CurrentPncMode = COMM_PNC_NO_COMMUNICATION;
                ComM_Pnc[index].PreviousPncMode = COMM_PNC_NO_COMMUNICATION;
                ComM_Pnc[index].PncEvents = 0;
                ComM_Pnc[index].PncState = COMM_STATE_PNC_NO;
                ComM_Pnc[index].UserFullCounter = 0;
                ComM_Pnc[index].EIRACounter = 0;
                ComM_Pnc[index].ERACounter = 0;
                ComM_Pnc[index].PncPrepareSleepTime = 0;
                ComM_Pnc[index].PncPrepareSleepTimeStart = FALSE;
            }
        }
        #endif/* STD_ON == COMM_PNC_SUPPORT */
        for (index = 0u; index < COMM_NUMBER_OF_USERS; index++)
        {
            /**@req ComM485*/
            ComM_User[index].RequestMode = COMM_NO_COMMUNICATION;
            #if (STD_ON == COMM_RTE_NOTIFY_USED)
            ComM_User[index].ChCounter[0u] = ComM_UserConfig[index].NumofChannelRefToUser;
            ComM_User[index].ChCounter[1u] = 0u;
            ComM_User[index].ChCounter[2u] = 0u;
            ComM_User[index].OldMode = COMM_NO_COMMUNICATION;
            #endif /* STD_ON == COMM_RTE_NOTIFY_USED */
        }
        for (index = 0u; index < COMM_NUMBER_OF_CHANNELS; index++)
        {
            ComM_Channel[index].PreviousMode = COMM_NO_COMMUNICATION;
            ComM_Channel[index].CurrentMode = COMM_NO_COMMUNICATION;
            ComM_Channel[index].State = COMM_NO_COM_NO_PENDING_REQUEST;
            ComM_Channel[index].MinFullComModeTimer = 0u;
            ComM_Channel[index].LightModeTimer = 0u;
            ComM_Channel[index].Events = 0u;
            ComM_Channel[index].UserFullCounter = 0u;
            ComM_Channel[index].CommunicationAllowed = FALSE;
#if (STD_ON == COMM_NM_USED)
            ComM_Channel[index].specNmMode = NM_MODE_BUS_SLEEP;
#endif/* (STD_ON == COMM_NM_USED) */
            #if (STD_ON == COMM_DCM_USED)
            ComM_Channel[index].Dcm_ClearInd = FALSE;
            #endif /* STD_ON == COMM_DCM_USED */
            #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
            /*ECUC_ComM_00569:This is the default/init value of a runtime variable
             that can be changed during runtime using ComM_PreventWakeUp().*/
            if ((boolean)TRUE == ComM_Global.ComMNoWakeup[index])
            {
                ComM_Global.inhStu[index] |= COMM_MASK_INHIBIT_WAKEUP;
            }
            #endif /* STD_ON == COMM_WAKEUP_INHIBITION_ENABLED */
            #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
            if ((boolean)TRUE == ComM_ChannelConfig[index].ComMNoCom)
            {
                ComM_Global.inhStu[index] |= COMM_MASK_LIMIT_TO_NOCOM;
            }
            #endif /* STD_ON == COMM_MODE_LIMITATION_ENABLED */
            ComM_BusNMModInd[index] = COMM_INVALID_UINT8;
        }
        /* Set mode status */
        ComM_Status = COMM_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               De-initializes (terminates) the AUTOSAR Communication Manager.
 * ServiceId           0x02
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM147*/
/**@req ComM794*/
FUNC(void, COMM_CODE) ComM_DeInit(void)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        /**@req ComM580*/
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DEINIT, COMM_E_NOT_INITED);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        NetworkHandleType index;
    boolean AllowDeinit = TRUE;
        /* Check whether all channels are in No Communication mode */
        for (index = 0u; index < COMM_NUMBER_OF_CHANNELS; index++)
        {
            if (COMM_NO_COMMUNICATION != ComM_Channel[index].CurrentMode)
            {
                AllowDeinit = FALSE;
            }
        }
    if((boolean)TRUE == AllowDeinit)
    {
            /*  @SWS_ComM_00865,shall store non-volatile
                parameters specified in SWS_ComM_00103 to NVRAM */
            /* Done, if all channel is in No Communication. */
        #if (STD_ON == COMM_NVM_USED)
            NvM_WriteBlock(COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR, &ComM_Global);
        #endif
            ComM_Status = COMM_UNINIT;
    }
    }
}

/*this api is delete in R19-11 */
/******************************************************************************/
/*
 * Brief               Return current state, including sub-state, of the ComM
 *                     channel state machine.
 * ServiceId           0x34
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  State
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetState(
    NetworkHandleType Channel,
    P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_DATA) State)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETSTATE,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETSTATE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if (NULL_PTR == State)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETSTATE,
                        COMM_E_PARAM_POINTER);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        *State = ComM_Channel[Channel].State;
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Returns the initialization status of the AUTOSAR Communication
 *                     Manager.
 * ServiceId           0x03
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  Status
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM242*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus
(
    P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (NULL_PTR == Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETSTATUS,
                        COMM_E_PARAM_POINTER);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        *Status = ComM_Status;
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Returns the inhibition status of a ComM channel.
 * ServiceId           0x04
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  Status
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM619*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetInhibitionStatus
(
    NetworkHandleType Channel,
    P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETINHIBITIONSTATUS,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETINHIBITIONSTATUS,
                        COMM_E_PARAM_POINTER);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETINHIBITIONSTATUS,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
    #if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)
        *Status = ComM_Global.inhStu[Channel];
    #else
        *Status = 0u;
    #endif /* COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED */
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Requesting of a communication mode by a user.
 * ServiceId           0x05
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      User,
 *                     ComMode
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM283*^**@req ComM191*^**@req ComM471*^**@req ComM500*^**@req ComM129*/
/**@req ComM110*^**@req ComM795*^**@req ComM441*^**@req ComM439*^**@req ComM441*/
/**@req ComM442*^**@req ComM68*/
FUNC(Std_ReturnType, COMM_CODE) ComM_RequestComMode
(
    ComM_UserHandleType User,
    ComM_ModeType ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_REQUESTCOMMODE,
                        COMM_E_NOT_INITED);
    }
    /**@req ComM151*/
    else if ((COMM_NO_COMMUNICATION != ComMode) && (COMM_FULL_COMMUNICATION != ComMode))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_REQUESTCOMMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_REQUESTCOMMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        #if (STD_ON == COMM_PNC_SUPPORT)
        PNCHandleType pncIndex;
        #endif/* STD_ON == COMM_PNC_SUPPORT */
        NetworkHandleType index;
        uint8 ArrayIndex;
        ret = E_OK;
        /**@req ComM343*/
            
        #if (STD_ON == COMM_PNC_SUPPORT)
        if ((ComM_ConfigPtr->pComMPncEnabled != NULL_PTR)
            && ((boolean)TRUE == *(ComM_ConfigPtr->pComMPncEnabled)))
        {
            for(pncIndex = 0; pncIndex < ComM_UserConfig[User].NumofPncRefToUser; pncIndex++)
            {
                ArrayIndex = ComM_UserConfig[User].PncRefToUserMap[pncIndex];
                if(ComMode!=ComM_User[User].RequestMode)
                {
                    if (COMM_FULL_COMMUNICATION == ComMode)
                    {
                        ComM_Pnc[ArrayIndex].UserFullCounter++;
                    }
                    else
                    {
                        if (ComM_Pnc[ArrayIndex].UserFullCounter > 0u)
                        {
                            ComM_Pnc[ArrayIndex].UserFullCounter--;
                        }
                    }
                }
            }
        }
        #endif/* STD_ON == COMM_PNC_SUPPORT */

        for (index = 0; index < ComM_UserConfig[User].NumofChannelRefToUser; index++)
        {
            ArrayIndex = ComM_UserConfig[User].ChannelRefToUserMap[index];
            if (COMM_FULL_COMMUNICATION == ComMode)
            {
                #if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) \
                    || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
                ret = ComM_UserRequestFullCom(ArrayIndex);
                if ((uint8)E_OK == ret)
                #endif/* STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) \
                    || (STD_ON == COMM_MODE_LIMITATION_ENABLED */
                {
                    if(ComMode != ComM_User[User].RequestMode)
                    {
                        ComM_Channel[ArrayIndex].UserFullCounter++;
                        #if (STD_ON == COMM_VERSION_R19_11)
                        /*@req R19-11 <SWS_ComM_00875>*/
                        if(COMM_CHANNEL_TYPE_MANAGED == ComM_ChannelConfig[ArrayIndex].ComMManageType)
                        {
                            ComM_ManagedChannelHandler(ArrayIndex,TRUE,USER_REQUESTFULLCOM);
                        }
                        #endif /* STD_ON == COMM_VERSION_R19_11 */
                    }
                }
            }
            else
            {
                if(ComMode!=ComM_User[User].RequestMode)
                {
                    if (ComM_Channel[ArrayIndex].UserFullCounter > 0u)
                    {
                        ComM_Channel[ArrayIndex].UserFullCounter--;
                        #if (STD_ON == COMM_VERSION_R19_11)
                        /*@req R19-11 <SWS_ComM_00875>*/
                        if(COMM_CHANNEL_TYPE_MANAGED == ComM_ChannelConfig[ArrayIndex].ComMManageType)
                        {
                            ComM_ManagedChannelHandler(ArrayIndex,TRUE,USER_REQUESTNOCOM);
                        }
                        #endif /* STD_ON == COMM_VERSION_R19_11 */
                    }
                }
            }
        }
        ComM_User[User].RequestMode = ComMode;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Function to query the maximum allowed communication mode
 *                     of the corresponding user.
 * ServiceId           0x06
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      User
 * Param-Name[in/out]  ComMode
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM85*/
/**@req ComM374*/
/**@req ComM796*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetMaxComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETMAXCOMMODE,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETMAXCOMMODE,
                        COMM_E_PARAM_POINTER);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETMAXCOMMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        #if ((STD_ON == COMM_MODE_LIMITATION_ENABLED)||(STD_ON == COMM_WAKEUP_INHIBITION_ENABLED))
        NetworkHandleType index;
        uint8 ArrayIndex;
        *ComMode = COMM_FULL_COMMUNICATION;
        for (index = 0; index < ComM_UserConfig[User].NumofChannelRefToUser; index++)
        {
            ArrayIndex = ComM_UserConfig[User].ChannelRefToUserMap[index];
            if(*ComMode != COMM_NO_COMMUNICATION)
            {
                switch(ComM_Channel[ArrayIndex].CurrentMode)
                {
                    case COMM_NO_COMMUNICATION:
                        #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
                        if (COMM_IS_INHIBIT_WAKEUP(ArrayIndex))
                        {
                            *ComMode = COMM_NO_COMMUNICATION;
                        }
                        #endif /* STD_ON == COMM_WAKEUP_INHIBITION_ENABLED */
                        break;
                    case COMM_SILENT_COMMUNICATION:
                        #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
                        if (((COMM_IS_LIMIT_TO_NOCOM(ArrayIndex)) && (*ComMode == COMM_FULL_COMMUNICATION)))
                        {
                            *ComMode = COMM_SILENT_COMMUNICATION;
                        }
                        #endif /* STD_ON == COMM_MODE_LIMITATION_ENABLED */
                        break;
                    default:
                        /*This logic does not require any processing*/
                        break;
                }
            }
        }
        #else
        *ComMode = COMM_FULL_COMMUNICATION;
        #endif /* end of STD_ON == COMM_MODE_LIMITATION_ENABLED ||
                     STD_ON == COMM_WAKEUP_INHIBITION_ENABLED  */
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Function to query the currently requested communication
 *                     mode of the corresponding user.
 * ServiceId           0x07
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      User
 * Param-Name[in/out]  ComMode
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM80*/
/**@req ComM79*/
/**@req ComM797*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetRequestedComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETREQUESTEDCOMMODE,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETREQUESTEDCOMMODE,
                        COMM_E_PARAM_POINTER);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETREQUESTEDCOMMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        *ComMode = ComM_User[User].RequestMode;
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Function to query the Current communication mode. ComM
 *                     shall use the corresponding interfaces of the Bus State
 *                     Managers to get the current communication mode.
 * ServiceId           0x08
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      User
 * Param-Name[in/out]  ComMode
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM84*/
/**@req ComM83*/
/**@req ComM176*/
/**@req ComM798*/
FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentComMode
(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETCURRENTCOMMODE,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETCURRENTCOMMODE,
                        COMM_E_PARAM_POINTER);
    }
    else if (COMM_MAX_USER_ID < User)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETCURRENTCOMMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        NetworkHandleType index;
        uint8 ArrayIndex;
        ComM_ModeType mode = COMM_FULL_COMMUNICATION;
        *ComMode = COMM_FULL_COMMUNICATION;
        for (index = 0; index < ComM_UserConfig[User].NumofChannelRefToUser; index++)
        {
            ArrayIndex = ComM_UserConfig[User].ChannelRefToUserMap[index];
            if ((uint8)E_OK == COMM_SM_GET_COMMODE(ArrayIndex, mode))
            {
                if (COMM_NO_COMMUNICATION == mode)
                {
                    *ComMode = mode;
                    break;
                }
                else
                {
                    if (COMM_SILENT_COMMUNICATION == mode)
                    {
                        *ComMode = mode;
                    }
                }
            }
        }
        ret = E_OK;
    }
    return ret;
}

#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
/******************************************************************************/
/*
 * Brief               Changes the inhibition status ComMNoWakeup for the
 *                     corresponding channel.
 * ServiceId           0x09
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel,
 *                     Status
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM301*/
/**@req ComM488*/
/**@req ComM156*/
/**@req ComM799*/
FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp
(
    NetworkHandleType Channel,
    boolean Status
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_PREVENTWAKEUP,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_PREVENTWAKEUP,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if((COMM_IS_INHIBIT_WAKEUP(Channel)))
        {
            if ((boolean)TRUE == Status)
            {
                ComM_Global.inhStu[Channel] |= COMM_MASK_INHIBIT_WAKEUP;
            }
            else
            {
               ComM_Global.inhStu[Channel] &= ~COMM_MASK_INHIBIT_WAKEUP;
            }
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif /* STD_ON == COMM_WAKEUP_INHIBITION_ENABLED */

#if ((STD_ON == COMM_RESET_AFTER_FORCING_NO_COMM) && (STD_ON == COMM_MODE_LIMITATION_ENABLED))
/******************************************************************************/
/*
 * Brief               Changes the inhibition status to ComMNoCom for the
 *                     corresponding channel.
 * ServiceId           0x0B
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel,
 *                     Status
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM301*/
/**@req ComM488*/
/**@req ComM163*/
/**@req ComM800*/
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitChannelToNoComMode
(
    NetworkHandleType Channel,
    boolean Status
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_LIMITCHANNELTONOCOMMODE,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_LIMITCHANNELTONOCOMMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if(COMM_IS_LIMIT_TO_NOCOM(Channel))
        {
            if ((boolean)TRUE == Status)
            {
                ComM_Global.inhStu[Channel] |= COMM_MASK_LIMIT_TO_NOCOM;
            }
            else
            {
                ComM_Global.inhStu[Channel] &= (uint8) (~COMM_MASK_LIMIT_TO_NOCOM);
            }
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Changes the inhibition status to ComMNoCom.
 * ServiceId           0x0C
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Status
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM301*/
/**@req ComM124*/
/**@req ComM801*/
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitECUToNoComMode(boolean Status)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_LIMITECUTONOCOMMODE,
                            COMM_E_NOT_INITED);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        ComM_EcuToNoCom = Status;
        ret = E_OK;
    }
    return ret;
}
#endif /* end of STD_ON==COMM_RESET_AFTER_FORCING_NO_COMM && STD_ON==COMM_MODE_LIMITATION_ENABLED */

#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
#if defined (COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR)
/******************************************************************************/
/*
 * Brief               This function returns the amount of rejected Full
 *                     Communication user requests.
 * ServiceId           0x0D
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  CounterValue
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM224*/
/**@req ComM802*/
FUNC(Std_ReturnType, COMM_CODE) ComM_ReadInhibitCounter
(
    P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_READINHIBITCOUNTER,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == CounterValue)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_READINHIBITCOUNTER,
                        COMM_E_PARAM_POINTER);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        *CounterValue = ComM_Global.InhibitCounter;
        ret = E_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function resets the Inhibited COMM_FULL_COMMUNICATION
 *                     request Counter.
 * ServiceId           0x0E
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM108*/
/**@req ComM803*/
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_RESETINHIBITCOUNTER,
                        COMM_E_NOT_INITED);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        ComM_Global.InhibitCounter = 0u;
        ret = E_OK;
    }
    return ret;
}
#endif /* defined COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR */
#endif /* STD_ON == COMM_MODE_LIMITATION_ENABLED */

#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
/******************************************************************************/
/*
 * Brief               Changes the ECU Group Classification status.
 * ServiceId           0x0F
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Status
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
/**@req ComM43*/
/**@req ComM552*/
FUNC(Std_ReturnType, COMM_CODE) ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status)
{
    Std_ReturnType ret = E_NOT_OK;
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_SETECUGROUPCLASSIFICATION,
                        COMM_E_NOT_INITED);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        ComM_Global.EcuGroupClassification = Status;
        ret = E_OK;
    }
    return ret;
}
#endif /*  (end of STD_ON==COMM_WAKEUP_INHIBITION_ENABLED || STD_ON==COMM_MODE_LIMITATION_ENABLED) */

#if (STD_ON == COMM_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               This function returns the published information.
 * ServiceId           0x10U
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  Versioninfo
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE)
ComM_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, COMM_APPL_DATA) Versioninfo
)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETVERSIONINFO,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == Versioninfo)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_GETVERSIONINFO,
                        COMM_E_PARAM_POINTER);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT  */
    {
        Versioninfo->vendorID = COMM_VENDOR_ID;
        Versioninfo->moduleID = COMM_MODULE_ID;
        Versioninfo->sw_major_version = COMM_C_SW_MAJOR_VERSION;
        Versioninfo->sw_minor_version = COMM_C_SW_MINOR_VERSION;
        Versioninfo->sw_patch_version = COMM_C_SW_PATCH_VERSION;
    }
}
#endif/* STD_ON == COMM_VERSION_INFO_API */
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"


#if (STD_ON == COMM_NM_USED)
#define COMM_START_SEC_COMMNMNETWORKSTARTINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication that a NM-message has been received in the Bus
 *                     Sleep Mode, what indicates that some nodes in the network
 *                     have already entered the Network Mode.
 * ServiceId           0x15
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM383*/
/**@req ComM804*/
/**@req ComM805*/
FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKSTARTINDICATION,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKSTARTINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
             && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKSTARTINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        /*SWS_ComM_00583*/
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_START);
    }
}
#define COMM_STOP_SEC_COMMNMNETWORKSTARTINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_COMMNMNETWORKMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification that the network management has entered
 *                     Network Mode.
 * ServiceId           0x18
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM390*/
/**@req ComM806*/
/**@req ComM807*/
FUNC(void, COMM_CODE) ComM_Nm_NetworkMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKMODE,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
             && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_NETWORKMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_NETMODE);
        SCHM_ENTER_COMM_CH(Channel);
        ComM_Channel[Channel].specNmMode = NM_MODE_NETWORK;
        SCHM_EXIT_COMM_CH(Channel);
    }
}
#define COMM_STOP_SEC_COMMNMNETWORKMODE_CALLBACK_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_COMMNMPREPAREBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification that the network management has entered
 *                     Prepare Bus-Sleep Mode.
 * ServiceId           0x19
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM391*/
/**@req ComM808*/
/**@req ComM809*/
FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
             && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_PRESLEEP);
        SCHM_ENTER_COMM_CH(Channel);
        ComM_Channel[Channel].specNmMode = NM_MODE_PREPARE_BUS_SLEEP;
        SCHM_EXIT_COMM_CH(Channel);
    }
}
#define COMM_STOP_SEC_COMMNMPREPAREBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_COMMNMBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification that the network management has entered
 *                     Bus-Sleep Mode.
 * ServiceId           0x1A
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM392*/
/**@req ComM810*/
/**@req ComM811*/
FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_BUSSLEEPMODE,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_BUSSLEEPMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
             && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_BUSSLEEPMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_BUSSLEEP);
        SCHM_ENTER_COMM_CH(Channel);
        ComM_Channel[Channel].specNmMode = NM_MODE_BUS_SLEEP;
        SCHM_EXIT_COMM_CH(Channel);
    }
}
#define COMM_STOP_SEC_COMMNMBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_COMMNMRESTARTINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               If NmIf has started to shut down the coordinated busses,
 *                     AND not all coordinated busses have indicated bus sleep
 *                     state, AND on at least on one of the coordinated busses
 *                     NM is restarted, THEN the NM Interface shall call the
 *                     callback function ComM_Nm_RestartIndication with the
 *                     nmNetworkHandle of the channels which have already
 *                     indicated bus sleep state.
 * ServiceId           0x1B
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM792*/
/**@req ComM812*/
/**@req ComM813*/
FUNC(void, COMM_CODE) ComM_Nm_RestartIndication(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_RESTARTINDICATION,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_RESTARTINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if ((ComM_ChannelConfig[Channel].ComMNmVariant != COMM_FULL)
             && (ComM_ChannelConfig[Channel].ComMNmVariant != COMM_PASSIVE))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_NM_RESTARTINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        COMM_EVENT_SET(Channel, COMM_EVENT_NM_RESTART);
    }
}
#define COMM_STOP_SEC_COMMNMRESTARTINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif /* STD_ON == COMM_NM_USED */

#if (STD_ON == COMM_DCM_USED)
#define COMM_START_SEC_COMMDCMACTIVEDIAGNOSTIC_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of active diagnostic by the DCM.
 * ServiceId           0x1F
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM346*/
/**@req ComM362*/
FUNC(void, COMM_CODE) ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_ACTIVEDIAGNOSTIC,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_ACTIVEDIAGNOSTIC,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if (COMM_PASSIVE != ComM_ChannelConfig[Channel].ComMNmVariant)
        {
            COMM_EVENT_SET(Channel, COMM_EVENT_DCM_ACTIVE);
            if((boolean)TRUE == ComM_Channel[Channel].Dcm_ClearInd)
            {
                ComM_Channel[Channel].Dcm_ClearInd = FALSE;
            }
        }
    }
}
#define COMM_STOP_SEC_COMMDCMACTIVEDIAGNOSTIC_CALLBACK_CODE
#include "ComM_MemMap.h"

#define COMM_START_SEC_COMMDCMINACTIVEDIAGNOSTIC_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of inactive diagnostic by the DCM.
 * ServiceId           0x20
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM364*/
FUNC(void, COMM_CODE) ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_INACTIVEDIAGNOSTIC,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_DCM_INACTIVEDIAGNOSTIC,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if(COMM_EVENT_IS_SET(Channel, COMM_EVENT_DCM_ACTIVE))
        {
            if((boolean)FALSE == ComM_Channel[Channel].CommunicationAllowed)
            {
                COMM_EVENT_CLR(Channel, (uint16)COMM_EVENT_DCM_ACTIVE);
            }
            else
            {
                ComM_Channel[Channel].Dcm_ClearInd = TRUE;
            }
        }
    }
}
#define COMM_STOP_SEC_COMMDCMINACTIVEDIAGNOSTIC_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif /* STD_ON == COMM_DCM_USED */

#if (STD_ON == COMM_ECUM_USED)
#define COMM_START_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification of a wake up on the corresponding channel.
 * ServiceId           0x2A
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM694*/
/**@req ComM275*/
/**@req ComM814*/
/**@req ComM815*/
/**@req ComM316*/
FUNC(void, COMM_CODE) ComM_EcuM_WakeUpIndication(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_WAKEUPINDICATION,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_WAKEUPINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        #if (STD_ON == COMM_SYNCHRONOUS_WAKE_UP)
        #if (STD_ON == COMM_PNC_SUPPORT)
        if ((ComM_ConfigPtr->pComMPncEnabled != NULL_PTR)
            && (TRUE == *(ComM_ConfigPtr->pComMPncEnabled)))
        {
            PNCHandleType PncIndex;
            uint8 ArrayIndex;
            for (PncIndex = 0; PncIndex < ComM_ChannelConfig[Channel].NumofPncRefToChannel; PncIndex++)
            {
                ArrayIndex = ComM_ChannelConfig[Channel].PncRefToChannelMap[PncIndex];
                COMM_PNC_EVENT_SET(ArrayIndex, COMM_EVENT_ECUM_WAKEUP);
            }
        }
        #endif/* STD_ON == COMM_PNC_SUPPORT */
        for (Channel = 0u; Channel < COMM_NUMBER_OF_CHANNELS; Channel++)
        #endif /* STD_ON == COMM_SYNCHRONOUS_WAKE_UP */
        {
            COMM_EVENT_SET(Channel, COMM_EVENT_ECUM_WAKEUP);
            #if (STD_ON == COMM_VERSION_R19_11)
            /*@req R19-11 <SWS_ComM_00893>*/
            if(COMM_CHANNEL_TYPE_MANAGED == ComM_ChannelConfig[Channel].ComMManageType)
            {
                ComM_ManagedChannelHandler(Channel,FALSE,ECUM_REQUEST);
            }
            #endif /* STD_ON == COMM_VERSION_R19_11 */
        }
    }
}
#define COMM_STOP_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"

#if (STD_ON == COMM_VERSION_R19_11)
#if (STD_ON == COMM_PNC_SUPPORT)
#define COMM_START_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification of a wake up on the corresponding partial
 *                     network cluster.
 * ServiceId           0x37
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      PNCid
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE) ComM_EcuM_PNCWakeUpIndication(PNCHandleType PNCid)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_ECUM_PNCWAKEUPINDICATION,
                        COMM_E_NOT_INITED);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if ((ComM_ConfigPtr->pComMPncEnabled != NULL_PTR)
                    && ((boolean)TRUE == *(ComM_ConfigPtr->pComMPncEnabled)))
        {
            PNCHandleType PncIndex;
            NetworkHandleType channel;
            for (PncIndex = 0; PncIndex < COMM_NUMBER_OF_PNCS; PncIndex++)
            {
                if(PNCid == ComM_ConfigPtr->pComMPncCfg[PncIndex].ComM_PncId)
                {
                    COMM_PNC_EVENT_SET(PncIndex, COMM_EVENT_ECUM_PNCWAKEUP);
                    /*@req R19_11 <SWS_ComM_01015> */
                    #if (STD_OFF == COMM_SYNCHRONOUS_WAKE_UP)
                    for (channel = 0; channel < ComM_ConfigPtr->pComMPncCfg[PncIndex].NumOfChannelRefToPnc; channel++)
                    {
                        uint8 ArrayIndex = ComM_ConfigPtr->pComMPncCfg[PncIndex].ChannelRefToPncMap[channel];
                        /*@req R19_11 <SWS_ComM_01014> */
                        COMM_EVENT_SET(ArrayIndex, COMM_EVENT_ECUM_PNCWAKEUP);
                        /*@req R19-11 <SWS_ComM_01014> note*/
                        if(COMM_CHANNEL_TYPE_MANAGED == ComM_ChannelConfig[channel].ComMManageType)
                        {
                            ComM_ManagedChannelHandler(channel,FALSE,ECUMPNC_REQUEST);
                        }
                    }
                    #else
                    for (channel = 0; channel < COMM_NUMBER_OF_CHANNELS; channel++)
                    {
                        /*@req R19_11 <SWS_ComM_01014> */
                        COMM_EVENT_SET(channel, COMM_EVENT_ECUM_PNCWAKEUP);
                        /*@req R19-11 <SWS_ComM_01014> note*/
                        if(COMM_CHANNEL_TYPE_MANAGED == ComM_ChannelConfig[channel].ComMManageType)
                        {
                            ComM_ManagedChannelHandler(channel,FALSE,ECUMPNC_REQUEST);
                        }
                    }
                    #endif /* STD_OFF == COMM_SYNCHRONOUS_WAKE_UP */
                }
            }
        }
    }
}
#endif/* STD_ON == COMM_PNC_SUPPORT */
#endif /* STD_ON == COMM_VERSION_R19_11 */
#define COMM_STOP_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif/* STD_ON == COMM_ECUM_USED */

#if (( STD_ON == COMM_BSWM_USED)||( STD_ON == COMM_ECUM_USED))
#define COMM_START_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               EcuM or BswM shall indicate to ComM when communication
 *                     is allowed.
 * ServiceId           0x35
 * ServiceId           Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel,
 *                     Allowed
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_ECUMBSWM_CODE) ComM_CommunicationAllowed(
    NetworkHandleType Channel,
    boolean Allowed)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_COMMUNICATIONALLOWED,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_COMMUNICATIONALLOWED,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        ComM_Channel[Channel].CommunicationAllowed = Allowed;
    }
}
#define COMM_STOP_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif /* end of STD_ON == COMM_BSWM_USED || STD_ON == COMM_ECUM_USED */

#define COMM_START_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of the actual bus mode by the corresponding
 *                     Bus State Manager. ComM shall propagate the indicated
 *                     state to the users with means of the RTE.
 * ServiceId           0x33
 * ServiceId           Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel,
 *                     ComMode
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM675*/
/**@req ComM816*/
/**@req ComM817*/
#if (STD_OFF == COMM_VERSION_R19_11)
FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication
(
    NetworkHandleType Channel,
    P2CONST(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_NOT_INITED);
    }
    else if (NULL_PTR == ComMode)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_PARAM_POINTER);
    }
    else if ((COMM_NO_COMMUNICATION != *ComMode) && (COMM_SILENT_COMMUNICATION != *ComMode)
             && (COMM_FULL_COMMUNICATION != *ComMode))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_REQUEST))
        {
            COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_REQUEST);
            if (ComM_Channel[Channel].CurrentMode == *ComMode)
            {
                COMM_EVENT_SET(Channel, COMM_EVENT_BUS_OK);
            }
            else
            {
                COMM_EVENT_SET(Channel, COMM_EVENT_BUS_FAIL);
            }
        }
        ComM_BusNMModInd[Channel] = *ComMode;
    }
}
#else
FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication
(
    NetworkHandleType Channel,
    VAR(ComM_ModeType, AUTOMATIC) ComMode
)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_NOT_INITED);
    }
    else if ((COMM_NO_COMMUNICATION != ComMode) && (COMM_SILENT_COMMUNICATION != ComMode)
             && (COMM_FULL_COMMUNICATION != ComMode))
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_MODEINDICATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_BUS_REQUEST))
        {
            COMM_EVENT_CLR(Channel, COMM_EVENT_BUS_REQUEST);
            if (ComM_Channel[Channel].CurrentMode == ComMode)
            {
                COMM_EVENT_SET(Channel, COMM_EVENT_BUS_OK);
            }
            else
            {
                COMM_EVENT_SET(Channel, COMM_EVENT_BUS_FAIL);
            }
        }
        ComM_BusNMModInd[Channel] = ComMode;
    }
}
#endif
#define COMM_STOP_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
#include "ComM_MemMap.h"

#if (STD_ON == COMM_VERSION_R19_11)
#define COMM_START_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification of the corresponding Bus State Manager
 *                     that the actual bus mode is Bus-Sleep.
 * Only applicable for LIN slave nodes.
 * ServiceId           0x34
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_CODE) ComM_BusSM_BusSleepMode(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_BUSSLEEPMODE,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_BUSSM_BUSSLEEPMODE,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* end of STD_ON == COMM_DEV_ERROR_DETECT */
    {
    /*@req R19-11 <SWS_ComM_01018>*/
    COMM_EVENT_SET(Channel, COMM_EVENT_NM_BUSSLEEP);
    }
}
#define COMM_STOP_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif /* STD_OFF == COMM_VERSION_R19_11 */

#if (STD_ON == COMM_PNC_SUPPORT)
#define COMM_START_SEC_COMMCOMCBK_CALLBACK_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This callback is called when the EIRA or ERA was updated
 *                     in COM.
 * ServiceId           0x36
 * ServiceId           Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, COMM_COM_CODE) ComM_COMCbk(uint16 signalId)
{
    uint8 signalIndex;
    uint8 PncIndex;

    for (PncIndex = 0; PncIndex < COMM_NUMBER_OF_PNCS; PncIndex++)
    {
        for(signalIndex = 0; signalIndex < ComM_ConfigPtr->pComMPncCfg[PncIndex].NumOfSignalRefToPnc; signalIndex++)
        {
            uint8 SglArrayIndex = ComM_ConfigPtr->pComMPncCfg[PncIndex].SignalRefToPncMap[signalIndex];
            if(signalId == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalId)
            {
                if (SIG_RX == ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalDirection)
                {
                    ComM_RxPncSignalProcess(signalId,SglArrayIndex,PncIndex);
                    #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
                    if(0xFF != ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalChannelId)
                    {
                        ComM_Pnc[PncIndex].PncSignalChannelId =
                                ComM_PncComSignalConfig[SglArrayIndex].ComMPncComSignalChannelId;

                        if(COMM_GATEWAY_TYPE_NULL != ComM_ChannelConfig[ComM_Pnc[PncIndex].PncSignalChannelId].ComMPncGateway)
                        {
                            ComM_PncGatewaySignalExchange(PncIndex);
                        }
                    }
                    #endif/* STD_ON == COMM_PNC_GATEWAY_ENABLED */
                }
            }
        }
    }
}
#define COMM_STOP_SEC_COMMCOMCBK_CALLBACK_CODE
#include "ComM_MemMap.h"
#endif/* STD_ON == COMM_PNC_SUPPORT */

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function shall perform the processing of the AUTOSAR
 *                     ComM activities that are not directly initiated by the calls.
 * ServiceId           0x60
 * ServiceId           Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Channel
 * Param-Name[in/out]  None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
/**@req ComM429*/
/**@req ComM818*/
FUNC(void, COMM_CODE) ComM_MainFunction(NetworkHandleType Channel)
{
    #if (STD_ON == COMM_DEV_ERROR_DETECT)
    if (COMM_UNINIT == ComM_Status)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_MAINFUNCATION,
                        COMM_E_NOT_INITED);
    }
    else if (COMM_MAX_CHANNEL_ID < Channel)
    {
        Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_SERVICEID_MAINFUNCATION,
                        COMM_E_WRONG_PARAMETERS);
    }
    else
    #endif /* STD_ON == COMM_DEV_ERROR_DETECT */
    {
        ComM_ModeHandleType pHandle = COMM_MODE_HANDLER(Channel);
        #if (STD_ON == COMM_PNC_SUPPORT)
        if ((ComM_ConfigPtr->pComMPncEnabled != NULL_PTR)
            && ((boolean)TRUE == *(ComM_ConfigPtr->pComMPncEnabled)))
        {
            uint8 PncIndex,ArrayIndex;
            ComM_PncModeHandleType pPncHandle = NULL_PTR;
            for (PncIndex = 0; PncIndex < ComM_ChannelConfig[Channel].NumofPncRefToChannel; PncIndex++)
            {   
                ArrayIndex = ComM_ChannelConfig[Channel].PncRefToChannelMap[PncIndex];              
                pPncHandle = COMM_PNC_MODE_HANDLER(ArrayIndex);
                if (NULL_PTR != pPncHandle)
                {
                    pPncHandle(ArrayIndex, Channel);
                }
            }
        }
        #endif/* STD_ON == COMM_PNC_SUPPORT */

        /*judge BusSM indication Mode switch result*/
        ComM_BusSMIndProcess(Channel);

        #if (STD_ON == COMM_NM_USED)
        if (COMM_EVENT_IS_SET(Channel, COMM_EVENT_NM_IND))
        {
            ComM_NmModeProcessing(Channel);
            COMM_EVENT_CLR(Channel, (uint16)COMM_EVENT_NM_IND);
        }
        #endif /* STD_ON == COMM_NM_USED */
        if (NULL_PTR != pHandle)
        {
            pHandle(Channel);
        }
    }
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
