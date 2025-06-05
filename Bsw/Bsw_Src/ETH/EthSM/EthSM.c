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
**  FILENAME    : EthSM.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>   <REVISION LOG> 
 * V2.0.0      20200527   HuRongbo   Initial version
 * V2.0.1      20210513   HuRongbo   Bug fix
 */
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

#define ETHSM_C_AR_MAJOR_VERSION  4
#define ETHSM_C_AR_MINOR_VERSION  2
#define ETHSM_C_AR_PATCH_VERSION  2
#define ETHSM_C_SW_MAJOR_VERSION  2
#define ETHSM_C_SW_MINOR_VERSION  0
#define ETHSM_C_SW_PATCH_VERSION  1

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthSM.h"
/*If it hasnot the Runtime det error*/
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#if (STD_ON == ETHSM_DEM_SUPPORT)
#include "Dem.h"
#endif
//#include "BswM_EthSM.h"
//#include "ComM_BusSM.h"
#include "EthIf.h"
#include "TcpIp.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (ETHSM_C_AR_MAJOR_VERSION != ETHSM_H_AR_MAJOR_VERSION)
  #error "EthSM.c : Mismatch in Specification Major Version"
#endif
#if (ETHSM_C_AR_MINOR_VERSION != ETHSM_H_AR_MINOR_VERSION)
  #error "EthSM.c : Mismatch in Specification _MINOR Version"
#endif
#if (ETHSM_C_AR_PATCH_VERSION != ETHSM_H_AR_PATCH_VERSION)
  #error "EthSM.c : Mismatch in Specification PATCH Version"
#endif
#if (ETHSM_C_SW_MAJOR_VERSION != ETHSM_H_SW_MAJOR_VERSION)
  #error "EthSM.c : Mismatch in Software Major Version"
#endif
#if (ETHSM_C_SW_MINOR_VERSION != ETHSM_H_SW_MINOR_VERSION)
  #error "EthSM.c : Mismatch in Software MINOR Version"
#endif
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#if (STD_ON == ETHSM_DEV_ERROR_DETECT)
#define ETHSM_DET_REPORT(ApiId,ErrorId)     (\
    (void)Det_ReportError(\
            ETHSM_MODULE_ID,\
            ETHSM_INSTANCE_ID,\
            (ApiId),\
            (ErrorId)))
#else
#define ETHSM_DET_REPORT(ApiId,ErrorId)
#endif

#define EthSM_GetComMNetworkId(ch) \
            (EthSM_ConfigPtr->EthSMNetwork[ch].EthSMComMNetworkHandleRef)

#define EthSM_GetEthIfControllerId(ch) \
            (EthSM_ConfigPtr->EthSMNetwork[ch].EthSMEthIfControllerRef)

#if (STD_ON == ETHSM_DEM_SUPPORT)
#define EthSM_GetDemEventParameterRef(ch) \
            (EthSM_ConfigPtr->EthSMNetwork[ch].EthSMDemEventParameterRefs)
#endif

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
/* Sub State define,Intermediate state when state change to another */
typedef enum
{
    ETHSM_SUBSTATE_NONE,
    ETHSM_SUBSTATE_WAIT_CTRL_ACTIVE,
    ETHSM_SUBSTATE_WAIT_CTRL_DOWN,
    ETHSM_SUBSTATE_WAIT_TCPIP_ONLINE,
    ETHSM_SUBSTATE_WAIT_TCPIP_OFFLINE,
    ETHSM_SUBSTATE_WAIT_TCPIP_ONHOLD,
    ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE
}EthSM_SubState;

/*Define the values of the internal states of the EthSM module*/
typedef enum
{
    ETHSM_UNINITED,
    ETHSM_INIT
}EthSM_StateType;

/* Define runtime value */
typedef struct
{
    /* ComM requested communication mode */
    ComM_ModeType requestComMode;
    /* Ethernet Interface Controller mode */
    Eth_ModeType ctrlMode;
    /* Actual transceiver link state of the specific network handle */
    EthTrcv_LinkStateType trcvLinkState;
    /* Actual TcpIp state of the specific network handle */
    TcpIp_StateType tcpipState;
    /* NetWork current BSM state */
    EthSM_NetworkModeStateType curBsmState;
    /* Sub State */
    EthSM_SubState subState;
}EthSM_RunTimeType;

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"

static FUNC(void,ETHSM_CODE) EthSM_StateOffline(
    NetworkHandleType ch
);

static FUNC(void,ETHSM_CODE) EthSM_StateWaitTrcvLink(
    NetworkHandleType ch
);

static FUNC(void,ETHSM_CODE) EthSM_StateWaitOnline(
    NetworkHandleType ch
);

static FUNC(void,ETHSM_CODE) EthSM_StateOnline(
    NetworkHandleType ch
);

static FUNC(void,ETHSM_CODE) EthSM_StateOnhold(
    NetworkHandleType ch
);

static FUNC(void,ETHSM_CODE) EthSM_StateWaitOffline(
    NetworkHandleType ch
);

static FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByNetworkHandle(
    NetworkHandleType NetworkHandle
);

static FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByCtrlIdx(
    uint8 CtrlIdx
);

#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define ETHSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthSM_MemMap.h"
/* EthSM runtime data */
static VAR(EthSM_RunTimeType,ETHSM_VAR) EthSM_RTData[ETHSM_CHANNEL_NUM];
#define ETHSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthSM_MemMap.h"


#define ETHSM_START_SEC_VAR_INIT_PTR
#include "EthSM_MemMap.h"
/* Pointer to configuration data */
static P2CONST(EthSM_ConfigType,ETHSM_VAR,ETHSM_CONST)
EthSM_ConfigPtr = NULL_PTR;
#define ETHSM_STOP_SEC_VAR_INIT_PTR
#include "EthSM_MemMap.h"


#define ETHSM_START_SEC_VAR_INIT_UNSPECIFIED
#include "EthSM_MemMap.h"
/* Variable store EthSM initialization status */
static VAR(EthSM_StateType,ETHSM_VAR_POWER_ON_INIT)
EthSM_Status = ETHSM_UNINITED;
#define ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "EthSM_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service initializes the EthSM module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_Init( void )
{
    uint8 ch;

    /* Initialize run time structure variable for every network */
    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++)
    {
        EthSM_RTData[ch].requestComMode = COMM_NO_COMMUNICATION;
        EthSM_RTData[ch].ctrlMode = ETH_MODE_DOWN;
        EthSM_RTData[ch].trcvLinkState = ETHTRCV_LINK_STATE_DOWN;
        EthSM_RTData[ch].tcpipState = TCPIP_STATE_OFFLINE;
        /* @req SWS_EthSM_00025 */
        /* After the initialization of the EthSM the state machine shall have a
         * transition to ETHSM_STATE_OFFLINE.
         */
        EthSM_RTData[ch].curBsmState = ETHSM_STATE_OFFLINE;
        EthSM_RTData[ch].subState = ETHSM_SUBSTATE_NONE;
    }

    /* Initialize config data pointer*/
    EthSM_ConfigPtr = &EthSM_Config;

    EthSM_Status = ETHSM_INIT;
}

/******************************************************************************/
/*
 * Brief               Output the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfo: Pointer to where to store the version
 *                                  information of this module
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#if(STD_ON == ETHSM_VERSION_INFO_API)
FUNC(void,ETHSM_CODE) EthSM_GetVersionInfo(
    P2VAR(Std_VersionInfoType,AUTOMATIC,AUTOMATIC) VersionInfo
)
{
    #if(STD_ON == ETHSM_DEV_ERROR_DETECT)
    if(NULL_PTR == VersionInfo)
    {
        ETHSM_DET_REPORT(ETHSM_SID_GETVERSIONINFO, ETHSM_E_PARAM_POINTER);
    }
    else
    #endif
    {
        VersionInfo->vendorID = ETHSM_VENDOR_ID;
        VersionInfo->moduleID = ETHSM_MODULE_ID;
        VersionInfo->sw_major_version = ETHSM_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = ETHSM_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = ETHSM_C_SW_PATCH_VERSION;
    }
}
#endif/*STD_ON == EthSM_VERSION_INFO_API*/

/******************************************************************************/
/*
 * Brief               Handles the communication mode and sets the Ethernet 
 *                     network active or passive.
 * ServiceId           0x05
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle: Handle of destinated communication network
 *                                    for request.
 *                     ComM_Mode: Requested communication mode.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Service accepted
 *                     E_NOT_OK: Service denied
 */
/******************************************************************************/
FUNC(Std_ReturnType,ETHSM_CODE) EthSM_RequestComMode(
    NetworkHandleType NetworkHandle,
    ComM_ModeType ComM_Mode
)
{
    Std_ReturnType ret = E_NOT_OK;
    NetworkHandleType ch = EthSM_GetChannelByNetworkHandle(NetworkHandle);
    
    #if(STD_ON == ETHSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00054 */
        ETHSM_DET_REPORT(ETHSM_SID_REQUESTCOMMODE, ETHSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if(ch >= ETHSM_CHANNEL_NUM)
    {
        /* @req SWS_EthSM_00051,SWS_EthSM_00052 */
        ETHSM_DET_REPORT(ETHSM_SID_REQUESTCOMMODE,
                         ETHSM_E_INVALID_NETWORK_HANDLE);
        detNoErr = FALSE;
    }
    if((COMM_NO_COMMUNICATION != ComM_Mode) &&
       (COMM_SILENT_COMMUNICATION != ComM_Mode) &&
       (COMM_FULL_COMMUNICATION != ComM_Mode))
    {
        /* @req SWS_EthSM_00095 */
        ETHSM_DET_REPORT(ETHSM_SID_REQUESTCOMMODE,ETHSM_E_INVALID_NETWORK_MODE);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        /* @req ECUC_EthSM_00079 */
        #if(STD_OFF == ETHSM_DUMMY_MODE)
        /* @req SWS_EthSM_00053 */
        /*Store the communication mode for the network handle except SilentCom*/
        if(COMM_SILENT_COMMUNICATION != ComM_Mode)
        {
            EthSM_RTData[ch].requestComMode = ComM_Mode;
        }
        #endif

        /* @req SWS_EthSM_00051,SWS_EthSM_00199 */
        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               This service shall put out the current communication mode
 *                     of a Ethernet network.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NetworkHandle: Network handle whose current communication
 *                                    mode shall be put out
 * Param-Name[out]     ComM_ModePtr: Pointer where to put out the current
 *                                   communication mode
 * Param-Name[in/out]  None
 * Return              E_OK: Service accepted
 *                     E_NOT_OK: Service denied
 */
/******************************************************************************/
FUNC(Std_ReturnType,ETHSM_CODE) EthSM_GetCurrentComMode(
    NetworkHandleType NetworkHandle,
    P2VAR(ComM_ModeType, AUTOMATIC, AUTOMATIC) ComM_ModePtr
)
{
    Std_ReturnType ret = E_NOT_OK;
    NetworkHandleType ch = EthSM_GetChannelByNetworkHandle(NetworkHandle);
    
    #if(STD_ON == ETHSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00060 */
        ETHSM_DET_REPORT(ETHSM_SID_GETCURRENTCOMMODE, ETHSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if(ch >= ETHSM_CHANNEL_NUM)
    {
        /* @req SWS_EthSM_00058 */
        ETHSM_DET_REPORT(ETHSM_SID_GETCURRENTCOMMODE,
                         ETHSM_E_INVALID_NETWORK_HANDLE);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        /* @req SWS_EthSM_00057,SWS_EthSM_00059 */
        /* Output the current communication mode for the network handle */
        if((ETHSM_STATE_ONLINE == EthSM_RTData[ch].curBsmState) ||
           (ETHSM_STATE_ONHOLD == EthSM_RTData[ch].curBsmState) ||
           (ETHSM_STATE_WAIT_OFFLINE == EthSM_RTData[ch].curBsmState))
        {
            *ComM_ModePtr = COMM_FULL_COMMUNICATION;
        }
        else
        {
            *ComM_ModePtr = COMM_NO_COMMUNICATION;
        }

        /* @req SWS_EthSM_00057 */
        ret = E_OK;
    }

    return ret;
}

#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called when mode has been read out.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant (only for different Ethernet controllers)
 * Param-Name[in]      CtrlIdx: Ethernet Interface Controller whose mode has
 *                              changed.
 *                     CtrlMode: Notified Ethernet Interface Controller mode.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_CtrlModeIndication (
    uint8 CtrlIdx,
    Eth_ModeType CtrlMode
)
{
    NetworkHandleType ch = EthSM_GetChannelByCtrlIdx(CtrlIdx);
    
    #if(STD_ON == ETHSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00192 */
        ETHSM_DET_REPORT(ETHSM_SID_CTRLMODEINDICATION, ETHSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if(ch >= ETHSM_CHANNEL_NUM)
    {
        /* @req SWS_EthSM_00191 */
        ETHSM_DET_REPORT(ETHSM_SID_CTRLMODEINDICATION,ETHSM_E_PARAM_CONTROLLER);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        EthSM_RTData[ch].ctrlMode = CtrlMode;
    }
}
#define ETHSM_STOP_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"


#define ETHSM_START_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the Ethernet Interface to 
 *                     report a transceiver link state change.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: Index of the Ethernet controller within the 
 *                              context of the Ethernet Interface
 *                     TransceiverLinkState: Actual transceiver link state of 
 *                                           the specific network handle
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_TrcvLinkStateChg (
    uint8 CtrlIdx,
    EthTrcv_LinkStateType TransceiverLinkState
)
{
    NetworkHandleType ch = EthSM_GetChannelByCtrlIdx(CtrlIdx);
    
    #if(STD_ON == ETHSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00115 */
        ETHSM_DET_REPORT(ETHSM_SID_TRCVLINKSTATECHG, ETHSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if(ch >= ETHSM_CHANNEL_NUM)
    {
        /* @req SWS_EthSM_00112 */
        ETHSM_DET_REPORT(ETHSM_SID_TRCVLINKSTATECHG, ETHSM_E_PARAM_CONTROLLER);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        /* @req SWS_EthSM_00114 */
        /* Store the transceiver link state for the affected network handle */
        EthSM_RTData[ch].trcvLinkState = TransceiverLinkState;
    }
}
#define ETHSM_STOP_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
#include "EthSM_MemMap.h"


#define ETHSM_START_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               This service is called by the TcpIp to report the actual
 *                     TcpIp state (e.g. online,offline).
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CtrlIdx: EthIf controller index to identify the 
 *                     communication network where the TcpIp state is changed.
 *                     TcpIpState: Actual TcpIp state of the specific network
 *                                 handle.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void,ETHSM_CODE) EthSM_TcpIpModeIndication (
    uint8 CtrlIdx,
    TcpIp_StateType TcpIpState
)
{
    NetworkHandleType ch = EthSM_GetChannelByCtrlIdx(CtrlIdx);
    
    #if(STD_ON == ETHSM_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(ETHSM_UNINITED == EthSM_Status)
    {
        /* @req SWS_EthSM_00120 */
        ETHSM_DET_REPORT(ETHSM_SID_TCPIPMODEINDICATION, ETHSM_E_UNINIT);
        detNoErr = FALSE;
    }
    if((TCPIP_STATE_ONLINE != TcpIpState) &&
       (TCPIP_STATE_ONHOLD != TcpIpState) &&
       (TCPIP_STATE_OFFLINE != TcpIpState) &&
       (TCPIP_STATE_STARTUP != TcpIpState) &&
       (TCPIP_STATE_SHUTDOWN != TcpIpState))
    {
        /* @req SWS_EthSM_00118 */
        ETHSM_DET_REPORT(ETHSM_SID_TCPIPMODEINDICATION,
                         ETHSM_E_INVALID_TCP_IP_MODE);
        detNoErr = FALSE;
    }
    if(ch >= ETHSM_CHANNEL_NUM)
    {
        /* @req SWS_EthSM_00112 */
        ETHSM_DET_REPORT(ETHSM_SID_TCPIPMODEINDICATION,
                         ETHSM_E_PARAM_CONTROLLER);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        /* @req SWS_EthSM_00119 */
        /* Store the TcpIp state for the affected network handle */
        EthSM_RTData[ch].tcpipState = TcpIpState;
    }
}
#define ETHSM_STOP_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
#include "EthSM_MemMap.h"

#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Cyclic Main Function which is called from the Scheduler.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, ETHSM_CODE) EthSM_MainFunction( void )
{
    NetworkHandleType ch;

    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++)
    {
        switch(EthSM_RTData[ch].curBsmState)
        {
            case ETHSM_STATE_OFFLINE:
                EthSM_StateOffline(ch);
                break;
            case ETHSM_STATE_WAIT_TRCVLINK:
                EthSM_StateWaitTrcvLink(ch);
                break;
            case ETHSM_STATE_WAIT_ONLINE:
                EthSM_StateWaitOnline(ch);
                break;
            case ETHSM_STATE_ONLINE:
                EthSM_StateOnline(ch);
                break;
            case ETHSM_STATE_ONHOLD:
                EthSM_StateOnhold(ch);
                break;
            case ETHSM_STATE_WAIT_OFFLINE:
                EthSM_StateWaitOffline(ch);
                break;
            default:
                /* Other state */
                break;
        }
    }
}
#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ETHSM_START_SEC_CODE
#include "EthSM_MemMap.h"
/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_OFFLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,ETHSM_CODE) EthSM_StateOffline(
    NetworkHandleType ch
)
{
    P2VAR(EthSM_RunTimeType, AUTOMATIC, ETHSM_APPL_DATA) chPtr = &EthSM_RTData[ch];
    Std_ReturnType ret;
    NetworkHandleType network;
    uint8 ctrlIdx;

    if (ETHSM_SUBSTATE_NONE == chPtr->subState)
    {
        /* @req SWS_EthSM_00026 */
        /* Transition from substate OFFLINE to WAIT_TRCVLINK */
        if (COMM_FULL_COMMUNICATION == chPtr->requestComMode)
        {
            /* @req SWS_EthSM_00088 */
            ctrlIdx = EthSM_GetEthIfControllerId(ch);
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_ACTIVE);
            if (E_OK == ret)
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_ACTIVE;
            }
        }
    }
    else if(ETHSM_SUBSTATE_WAIT_CTRL_ACTIVE == chPtr->subState)
    {
        if (ETH_MODE_ACTIVE == chPtr->ctrlMode)
        {
            chPtr->curBsmState = ETHSM_STATE_WAIT_TRCVLINK;
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            /* @req SWS_EthSM_00097 */
            network = EthSM_GetComMNetworkId(ch);
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_TRCVLINK);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_WAIT_TRCVLINK
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,ETHSM_CODE) EthSM_StateWaitTrcvLink(
    NetworkHandleType ch
)
{
    P2VAR(EthSM_RunTimeType, AUTOMATIC, ETHSM_APPL_DATA) chPtr = &EthSM_RTData[ch];
    Std_ReturnType ret;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);

    if (ETHSM_SUBSTATE_NONE == chPtr->subState)
    {
        /* @req SWS_EthSM_00127 */
        /* Transition from substate WAIT_TRCVLINK to OFFLINE */
        if (COMM_NO_COMMUNICATION == chPtr->requestComMode)
        {
            /* @req SWS_EthSM_00128 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);
            if (E_OK == ret)
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_DOWN;
            }
        }
        else
        {
            /* @req SWS_EthSM_00132 */
            /* Transition from substate WAIT_TRCVLINK to WAIT_ONLINE */
            if (ETHTRCV_LINK_STATE_ACTIVE == chPtr->trcvLinkState)
            {
                /* @req SWS_EthSM_00133 */
                ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_ONLINE);

                if (E_OK == ret)
                {
                    chPtr->curBsmState = ETHSM_STATE_WAIT_ONLINE;

                    /* @req SWS_EthSM_00134 */
                    //BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_ONLINE);
                }
            }
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_DOWN == chPtr->subState)
    {
        if (ETH_MODE_DOWN == chPtr->ctrlMode)
        {
            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            /* @req SWS_EthSM_00130 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_WAIT_ONLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,ETHSM_CODE) EthSM_StateWaitOnline(
    NetworkHandleType ch
)
{
    P2VAR(EthSM_RunTimeType, AUTOMATIC, ETHSM_APPL_DATA) chPtr = &EthSM_RTData[ch];
    Std_ReturnType ret;
    Std_ReturnType ret2;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);

    if (ETHSM_SUBSTATE_NONE == chPtr->subState)
    {
        /* @req SWS_EthSM_00140*/
        /* Transition from substate WAIT_ONLINE to OFFLINE */
        if (COMM_NO_COMMUNICATION == chPtr->requestComMode)
        {
            /* @req SWS_EthSM_00141 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);

            /* @req SWS_EthSM_00143 */
            ret2 = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if ((E_OK == ret) && (E_OK == ret2))
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE;
            }
        }
        else if (ETHTRCV_LINK_STATE_DOWN == chPtr->trcvLinkState)
        {
            /* @req SWS_EthSM_00136 */
            /* Transition from substate WAIT_ONLINE to WAIT_TRCVLINK */

            /* @req SWS_EthSM_00137 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if (E_OK == ret)
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_TCPIP_OFFLINE;
            }
        }
        else if (TCPIP_STATE_ONLINE == chPtr->tcpipState)
        {
            /* @req SWS_EthSM_00146 */
            /* Transition from substate WAIT_ONLINE to ONLINE */
            chPtr->curBsmState = ETHSM_STATE_ONLINE;

            /* @req SWS_EthSM_00148 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_ONLINE);
            /* @req SWS_EthSM_00150 */
            ComM_BusSM_ModeIndication(network, COMM_FULL_COMMUNICATION);
        }
        else
        {
            /* Do nothing,Just for MISRA-C check */
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE == chPtr->subState)
    {
        if ((ETH_MODE_DOWN == chPtr->ctrlMode) && (TCPIP_STATE_OFFLINE == chPtr->tcpipState))
        {
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            /* @req SWS_EthSM_00144 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_TCPIP_OFFLINE == chPtr->subState)
    {
        if (TCPIP_STATE_OFFLINE == chPtr->tcpipState)
        {
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            chPtr->curBsmState = ETHSM_STATE_WAIT_TRCVLINK;
            /* @req SWS_EthSM_00138 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_TRCVLINK);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_ONLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,ETHSM_CODE) EthSM_StateOnline(
    NetworkHandleType ch
)
{
    P2VAR(EthSM_RunTimeType, AUTOMATIC, ETHSM_APPL_DATA) chPtr = &EthSM_RTData[ch];
    Std_ReturnType ret;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);
    #if (STD_ON == ETHSM_DEM_SUPPORT)
    P2CONST(EthSM_DemEventParameterRefType, AUTOMATIC, ETHSM_CONST) demEventPtr;
    #endif

    if (ETHSM_SUBSTATE_NONE == chPtr->subState)
    {
        /* @req SWS_EthSM_00155 */
        /* Transition from substate ONLINE to WAIT_OFFLINE */
        if (COMM_NO_COMMUNICATION == chPtr->requestComMode)
        {
            /* @req SWS_EthSM_00157 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if (E_OK == ret)
            {
                chPtr->curBsmState = ETHSM_STATE_WAIT_OFFLINE;

                /* @req SWS_EthSM_00158 */
                //BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_OFFLINE);
            }
        }
        else if (ETHTRCV_LINK_STATE_DOWN == chPtr->trcvLinkState)
        {
            /* @req SWS_EthSM_00166 */
            /* Transition from substate ONLINE to ONHOLD */

            /* @req SWS_EthSM_00167 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_ONHOLD);
            if (E_OK == ret)
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_TCPIP_ONHOLD;
            }
        }
        else if (TCPIP_STATE_OFFLINE == chPtr->tcpipState)
        {
            /* @req SWS_EthSM_00151 */
            /* Transition from substate ONLINE to WAIT_ONLINE */
            chPtr->curBsmState = ETHSM_STATE_WAIT_ONLINE;

            /* @req SWS_EthSM_00152 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_ONLINE);
            /* @req SWS_EthSM_00154 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
        else
        {
            /* Do nothing,Just for MISRA-C check */
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_TCPIP_ONHOLD == chPtr->subState)
    {
        if (TCPIP_STATE_ONHOLD == chPtr->tcpipState)
        {
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            chPtr->curBsmState = ETHSM_STATE_ONHOLD;
            /* @req SWS_EthSM_00168 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_ONHOLD);

            /* @req SWS_EthSM_00188 */
            #if(STD_ON == ETHSM_DEM_SUPPORT)
            demEventPtr = EthSM_GetDemEventParameterRef(ch);
            if (NULL_PTR != demEventPtr)
            {
                (void)Dem_SetEventStatus(demEventPtr->EthSMDemEventId,
                    DEM_EVENT_STATUS_FAILED);
            }
            #endif
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_ONHOLD
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,ETHSM_CODE) EthSM_StateOnhold(
    NetworkHandleType ch
)
{
    P2VAR(EthSM_RunTimeType, AUTOMATIC, ETHSM_APPL_DATA) chPtr = &EthSM_RTData[ch];
    Std_ReturnType ret;
    Std_ReturnType ret2;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);
    #if (STD_ON == ETHSM_DEM_SUPPORT)
    P2CONST(EthSM_DemEventParameterRefType, AUTOMATIC, ETHSM_CONST) demEventPtr;
    #endif

    if (ETHSM_SUBSTATE_NONE == chPtr->subState)
    {
        /* @req SWS_EthSM_00178 */
        /* Transition from substate ONHOLD to OFFLINE */
        if (COMM_NO_COMMUNICATION == chPtr->requestComMode)
        {
            /* @req SWS_EthSM_00179 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);

            /* @req SWS_EthSM_00181 */
            ret2 = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_OFFLINE);
            if ((E_OK == ret) && (E_OK == ret2))
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE;
            }
        }
        else if (ETHTRCV_LINK_STATE_ACTIVE == chPtr->trcvLinkState)
        {
            /* @req SWS_EthSM_00170 */
            /* Transition from substate ONHOLD to ONLINE */

            /* @req SWS_EthSM_00171 */
            ret = TcpIp_RequestComMode(ctrlIdx, TCPIP_STATE_ONLINE);
            if (E_OK == ret)
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_TCPIP_ONLINE;
            }
        }
        else if (TCPIP_STATE_OFFLINE == chPtr->tcpipState)
        {
            /* @req SWS_EthSM_00174 */
            /* Transition from substate ONHOLD to WAIT_TRCVLINK */
            chPtr->curBsmState = ETHSM_STATE_WAIT_TRCVLINK;

            /* @req SWS_EthSM_00175 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_WAIT_TRCVLINK);
            /* @req SWS_EthSM_00177 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
        else
        {
            /* Do nothing,Just for MISRA-C check */
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_TCPIP_CLOSE == chPtr->subState)
    {
        if ((ETH_MODE_DOWN == chPtr->ctrlMode) && (TCPIP_STATE_OFFLINE == chPtr->tcpipState))
        {
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            /* @req SWS_EthSM_00182 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
            /* @req SWS_EthSM_00184 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_TCPIP_ONLINE == chPtr->subState)
    {
        if (TCPIP_STATE_ONLINE == chPtr->tcpipState)
        {
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            chPtr->curBsmState = ETHSM_STATE_ONLINE;
            /* @req SWS_EthSM_00172 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_ONLINE);

            /* @req SWS_EthSM_00196 */
            #if(STD_ON == ETHSM_DEM_SUPPORT)
            demEventPtr = EthSM_GetDemEventParameterRef(ch);
            if (NULL_PTR != demEventPtr)
            {
                (void)Dem_SetEventStatus(demEventPtr->EthSMDemEventId,
                    DEM_EVENT_STATUS_PASSED);
            }
            #endif
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Handle state transition in ETHSM_STATE_WAIT_OFFLINE
 * Sync/Async          Synchronous
 * Reentrancy          None Reentrant
 * Param-Name[in]      ch: EthSM Channel(EthSM Network)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void,ETHSM_CODE) EthSM_StateWaitOffline(
    NetworkHandleType ch
)
{
    P2VAR(EthSM_RunTimeType, AUTOMATIC, ETHSM_APPL_DATA) chPtr = &EthSM_RTData[ch];
    Std_ReturnType ret;
    NetworkHandleType network = EthSM_GetComMNetworkId(ch);
    uint8 ctrlIdx = EthSM_GetEthIfControllerId(ch);

    if (ETHSM_SUBSTATE_NONE == chPtr->subState)
    {
        /* @req SWS_EthSM_00160 */
        /* Transition from substate WAIT_OFFLINE to OFFLINE */
        if (TCPIP_STATE_OFFLINE == chPtr->tcpipState)
        {
            /* @req SWS_EthSM_00161 */
            ret = EthIf_SetControllerMode(ctrlIdx, ETH_MODE_DOWN);
            if (E_OK == ret)
            {
                chPtr->subState = ETHSM_SUBSTATE_WAIT_CTRL_DOWN;
            }
        }
    }
    else if (ETHSM_SUBSTATE_WAIT_CTRL_DOWN == chPtr->subState)
    {
        if (ETH_MODE_DOWN == chPtr->ctrlMode)
        {
            chPtr->subState = ETHSM_SUBSTATE_NONE;

            chPtr->curBsmState = ETHSM_STATE_OFFLINE;
            /* @req SWS_EthSM_00163 */
            //BswM_EthSM_CurrentState(network, ETHSM_STATE_OFFLINE);
            /* @req SWS_EthSM_00165 */
            ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
        }
    }
    else
    {
        /* Do nothing here */
    }
}

/******************************************************************************/
/*
 * Brief               Get EthSM channel index by NetworkHandle
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      NetworkHandle: NetworkHandle from ComM
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              If find return EthSM channel index
 *                     If not find return ETHSM_CHANNEL_NUM
 */
/******************************************************************************/
static FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByNetworkHandle(
    NetworkHandleType NetworkHandle
)
{
    NetworkHandleType ch;

    /* loop for every channel, search corresponding network */
    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++)
    {
        if (EthSM_GetComMNetworkId(ch) == NetworkHandle)
        {
            return ch;
        }
    }

    return ch;
}

/******************************************************************************/
/*
 * Brief               Get EthSM channel index by EthIf controller index
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      network: NetworkHandle from ComM
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              If find return EthSM channel index
 *                     If not find return ETHSM_CHANNEL_NUM
 */
/******************************************************************************/
static FUNC(NetworkHandleType, ETHSM_CODE) EthSM_GetChannelByCtrlIdx(
    uint8 CtrlIdx
)
{
    NetworkHandleType ch;

    /* loop for every channel, search corresponding network */
    for (ch = 0u; ch < ETHSM_CHANNEL_NUM; ch++)
    {
        if (EthSM_GetEthIfControllerId(ch) == CtrlIdx)
        {
            return ch;
        }
    }

    return ch;
}

#define ETHSM_STOP_SEC_CODE
#include "EthSM_MemMap.h"
