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
**  FILENAME    : ComM.h                                                      **
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
 *  V2.0.0       2020-07-24  Wanglili       R19_11 ComM.h initial version.
 */
#ifndef COMM_H
#define COMM_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComM_Cfg.h"
#if (STD_ON == COMM_COM_USED)
#include "Com.h"
#endif /*end of #if (STD_ON == COMM_COM_USED)*/
#include "Rte_ComM_Type.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/


#define COMM_VENDOR_ID                   62u /* Replace with a symbolic name once available */
#define COMM_MODULE_ID                   12u /* from AUTOSAR_BasicSoftwareModules.pdf */
#define COMM_H_AR_RELEASE_MAJOR_VERSION  19u
#define COMM_H_AR_RELEASE_MINOR_VERSION  11u
#define COMM_H_AR_RELEASE_PATCH_VERSION  0u
#define COMM_H_SW_MAJOR_VERSION          2u
#define COMM_H_SW_MINOR_VERSION          1u
#define COMM_H_SW_PATCH_VERSION          0u

#define COMM_INSTANCE_ID    0u

/*Error Code*/
#define COMM_E_NOT_INITED                           0x01U
#define COMM_E_WRONG_PARAMETERS                     0x02U
#define COMM_E_PARAM_POINTER                        0x03U
#define COMM_E_INIT_FAILED                          0x04U

/*Service Id*/
#define COMM_SERVICEID_INIT                         0x01U
#define COMM_SERVICEID_DEINIT                       0x02U
#define COMM_SERVICEID_GETSTATE                     0x34U
#define COMM_SERVICEID_GETSTATUS                    0x03U
#define COMM_SERVICEID_GETINHIBITIONSTATUS          0x04U
#define COMM_SERVICEID_REQUESTCOMMODE               0x05U
#define COMM_SERVICEID_GETMAXCOMMODE                0x06U
#define COMM_SERVICEID_GETREQUESTEDCOMMODE          0x07U
#define COMM_SERVICEID_GETCURRENTCOMMODE            0x08U
#define COMM_SERVICEID_PREVENTWAKEUP                0x09U
#define COMM_SERVICEID_LIMITCHANNELTONOCOMMODE      0x0bU
#define COMM_SERVICEID_LIMITECUTONOCOMMODE          0x0cU
#define COMM_SERVICEID_READINHIBITCOUNTER           0x0dU
#define COMM_SERVICEID_RESETINHIBITCOUNTER          0x0eU
#define COMM_SERVICEID_SETECUGROUPCLASSIFICATION    0x0fU
#define COMM_SERVICEID_GETVERSIONINFO               0x10U
#define COMM_SERVICEID_NM_NETWORKSTARTINDICATION    0x15U
#define COMM_SERVICEID_NM_NETWORKMODE               0x18U
#define COMM_SERVICEID_NM_PREPAREBUSSLEEPMODE       0x19U
#define COMM_SERVICEID_NM_BUSSLEEPMODE              0x1aU
#define COMM_SERVICEID_NM_RESTARTINDICATION         0x1bU
#define COMM_SERVICEID_DCM_ACTIVEDIAGNOSTIC         0x1fU
#define COMM_SERVICEID_DCM_INACTIVEDIAGNOSTIC       0x20U
#define COMM_SERVICEID_COMMUNICATIONALLOWED         0x35U
#define COMM_SERVICEID_ECUM_WAKEUPINDICATION        0x2aU
#define COMM_SERVICEID_ECUM_PNCWAKEUPINDICATION     0x37U
#define COMM_SERVICEID_BUSSM_MODEINDICATION         0x33U
#define COMM_SERVICEID_BUSSM_BUSSLEEPMODE           0x34U
#define COMM_SERVICEID_MAINFUNCATION                0x60U
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* Define the RTE's notification */
#if (STD_ON == COMM_RTE_NOTIFY_USED)
typedef void (*ComM_RteNotifyType)(uint8 Mode);
#endif /*end of #if (STD_ON == COMM_RTE_NOTIFY_USED)*/

/*State and sub-state of ComM state machine*/
typedef uint8 ComM_StateType;
#define COMM_NO_COM_NO_PENDING_REQUEST  (ComM_StateType)0
#define COMM_NO_COM_REQUEST_PENDING     (ComM_StateType)1u
#define COMM_FULL_COM_NETWORK_REQUESTED (ComM_StateType)2u
#define COMM_FULL_COM_READY_SLEEP       (ComM_StateType)3u
#define COMM_SILENT_COM                 (ComM_StateType)4u

/*Initialization status of ComM.*/
typedef enum
{
    COMM_UNINIT = 0u,
    COMM_INIT
} ComM_InitStatusType;

/*Current mode of a PNC*/
typedef enum
{
    COMM_PNC_NO_COMMUNICATION = 0u, /*PNC does not communicate*/
    COMM_PNC_PREPARE_SLEEP,    /*PNC is active with no deadline monitoring*/
    COMM_PNC_READY_SLEEP,      /*PNC is requested by a remote ComM user*/
    COMM_PNC_REQUESTED         /*PNC is requested by a local ComM user*/
} ComM_PncModeType;

typedef enum
{
    COMM_GATEWAY_TYPE_ACTIVE = 0u,
    COMM_GATEWAY_TYPE_PASSIVE,
    COMM_GATEWAY_TYPE_NULL
} ComM_PncGatewayType;

#if (STD_ON == COMM_VERSION_R19_11)
typedef enum
{
    COMM_CHANNEL_TYPE_MANAGED = 0u,
    COMM_CHANNEL_TYPE_MANAGING,
    COMM_CHANNEL_TYPE_NULL
} ComM_ChannelManageType;
#endif /*end of  #if (STD_ON == COMM_VERSION_R19_11)*/

/* Defines the bus type of the channel.*/
typedef enum
{
    COMM_BUS_TYPE_CAN = 0u,
    COMM_BUS_TYPE_CDD,
    COMM_BUS_TYPE_ETH,
    COMM_BUS_TYPE_FR,
    COMM_BUS_TYPE_INTERNAL,
    COMM_BUS_TYPE_LIN
} ComM_BusType;

/* Defines the functionality of the networkmanagement.Shall be harmonized with NM configuration.*/
typedef enum
{
    COMM_FULL = 0u,  /*AUTOSAR NM available (default).*/
    COMM_LIGHT,      /*No AUTOSAR NM available but functionality to shut down a channel.*/
    COMM_NONE,       /*No NM available.*/
    COMM_PASSIVE,     /*AUTOSAR NM running in passive mode available.*/
    #if (STD_ON == COMM_VERSION_R19_11)
    COMM_LINSLAVE
    #endif /*end of  #if (STD_ON == COMM_VERSION_R19_11)*/
} ComM_NmVariantType;

#if (STD_ON == COMM_PNC_SUPPORT)
typedef enum
{
    SIG_RX = 0u,
    SIG_TX
} ComM_PncComSignalDirectionType;

typedef enum
{
    EIRA = 0u,
    ERA  = 1u,
    Invalid = 2
} ComM_PncComSignalKindType;

typedef struct
{
    ComM_PncComSignalDirectionType ComMPncComSignalDirection;

    ComM_PncComSignalKindType ComMPncComSignalKind;/*Multiplicity 0..1*/

    #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
    uint8 ComMPncComSignalChannelId;
    #endif/*end of #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)*/

    /*Symbolic name reference to [ ComSignal ] from Com module*/
    #if (STD_ON == COMM_COM_USED)
    Com_SignalIdType ComMPncComSignalId;
    #endif/*end of #if (STD_ON == COMM_COM_USED)*/

    uint8 TxSignalBufferIndex;
} ComM_PncComSignalType;
#endif/*end of #if (STD_ON == COMM_PNC_SUPPORT)*/

/* This container contains the configuration (parameters) of the bus channel(s).
 * The channel parameters shall be harmonized within the whole communication stack.*/
typedef struct
{
    /* Identifies the bus type of the channel. */
    /**@req ComM567*/
    ComM_BusType ComMBusType;

    /*COMM_BUS_TYPE_CDD == ComMBusType, this menber valid*/
    uint8 ComMCDDBusPrefix;

    /*Shall be stored none volatile (value must be kept during a reset).*/
    boolean ComMFullCommRequestNotificationEnabled;

    /* Specifies the period in seconds that the MainFunction has to be triggered with.4~100ms*/
    /**@req ComM556*/
    uint16_least ComMMainFunctionPeriod;

    #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
    /* ECU is not allowed to change state of the channel to "Silent Communication" or
     * "Full Communication".*/
    /**@req ComM571*/
    boolean ComMNoCom;
    #endif /*end of #if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/

    /* true, a valid Nvm block reference must be given in the (existing, i.e. multiplicity 1)
    ComMGlobalNvmBlockDescriptor pointing to a sufficiently big Nvm block*/
    boolean ComMNoWakeUpInhibitionNvmStorage;

    /* Defines if an ECU is not allowed to wake-up the channel.*/
    /**@req ComM569*/
    #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
    boolean ComMNoWakeup;
    #endif /*end of #if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/

    #if (STD_ON == COMM_PNC_SUPPORT)
    #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
    ComM_PncGatewayType ComMPncGateway;/*Multiplicity 0...1*/
    #endif/* STD_ON == COMM_PNC_SUPPORT*/
    #endif/* STD_ON == COMM_PNC_GATEWAY_ENABLE*/

    #if (STD_ON == COMM_VERSION_R19_11)
    /*Defines channel is managed channel or managing channel*/
    ComM_ChannelManageType ComMManageType;

    uint8 ComMManageChannelNum;

    /*Define managed channel or managing channel ref to which channel*/
    P2CONST(uint8, TYPEDEF, ComM_CONFIG_DATA)ComMManageReference;
    #endif /*end of #if (STD_ON == COMM_VERSION_R19_11)*/

    /* Defines the functionality of the networkmanagement.Shall be harmonized with NM configuration.*/
    /**@req ComM568*/
    ComM_NmVariantType ComMNmVariant;

    /* Defines the timeout (in seconds) after state "ready sleep" is left.*/
    /**@req ComM606*/
    uint16 ComMNmLightTimeout;/*Multiplicity 0...1*/

    #if (STD_ON == COMM_PNC_SUPPORT)
    boolean ComMPncNmRequest;
    #endif/*end of #if (STD_ON == COMM_PNC_SUPPORT)*/

    /*Ref to this channel's user array elements number */
    uint8 NumofUserRefToChannel;

    /*Ref to this channel's user array*/
    P2CONST(uint8, COMM_CONST, COMM_CONST) UserRefToChannelMap;

    /*Ref to this channel's Pnc array elements number */
    uint8 NumofPncRefToChannel;

    /*Ref to this channel's Pnc array*/
    P2CONST(uint8, COMM_CONST, COMM_CONST) PncRefToChannelMap;

    /*Reference to EcucPartition, where the according ComMChannel is
    assigned to.*/
    P2CONST(uint8, TYPEDEF, ComM_CONFIG_DATA)ChannelPartitionRef;

} ComM_ChannelCfgType;

/* This container contains a list of identifiers that are needed to refer to
 * a user in the system which is designated to request Communication modes.*/
typedef struct
{
    /*Ref to this user's channel array elements number */
    uint8 NumofChannelRefToUser;

    /*Ref to this user's channel array */
    P2CONST(uint8, COMM_CONST, COMM_CONST) ChannelRefToUserMap;

#if (STD_ON == COMM_RTE_NOTIFY_USED)
    /* Mode switches notification */
    ComM_RteNotifyType RteNotify;

#endif /*end of #if (STD_ON == COMM_RTE_NOTIFY_USED)*/

    /*Ref to this user's Pnc array elements number */
    uint8 NumofPncRefToUser;

    /*Ref to this user's Pnc array */
    P2CONST(uint8, COMM_CONST, COMM_CONST) PncRefToUserMap;

    /*Denotes in which "EcucPartition" the requester is executed. */
    P2CONST(uint8, TYPEDEF, ComM_CONFIG_DATA) UserEcucPartitionRef;
} ComM_UserCfgType;

#if (STD_ON == COMM_PNC_SUPPORT)
typedef struct
{
    uint8 ComM_PncId;

    /*Ref to this Pnc's channel array elements number */
    uint8 NumOfChannelRefToPnc;

    /* Ref to this Pnc's channel array */
    P2CONST(uint8, COMM_CONST, COMM_CONST) ChannelRefToPncMap;

    /*Ref to this Pnc's user array elements number*/
    uint8 NumOfUserRefToPnc;

    /*Ref to this Pnc's user array */
    P2CONST(uint8, COMM_CONST, COMM_CONST) UserRefToPncMap;

    /* Ref to this Pnc's signal array elements number*/
    uint8 NumOfSignalRefToPnc;

    /* Ref to this Pnc's signal array */
    P2CONST(uint8, COMM_CONST, COMM_CONST)SignalRefToPncMap;

    /*Reference to the PortGroups that correspond to this PNC. */
    P2CONST(uint8, COMM_CONST, COMM_CONST) PncEthIfSwitchPortGroupRef;

} ComM_PncCfgType;
#endif

/*This type contains the implementation-specific post build configuration structure.*/
typedef struct
{
    P2CONST(boolean, TYPEDEF, COMM_CONST) pComMPncEnabled;
#if (STD_ON == COMM_PNC_SUPPORT)
    P2CONST(ComM_PncCfgType, TYPEDEF, COMM_CONST) pComMPncCfg;/*Multiplicity 0...56*/
#endif
} ComM_ConfigType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
extern CONST(ComM_ChannelCfgType, COMM_CONST) ComM_ChannelConfig[COMM_NUMBER_OF_CHANNELS];
extern CONST(ComM_UserCfgType, COMM_CONST) ComM_UserConfig[COMM_NUMBER_OF_USERS];
#if (STD_ON == COMM_PNC_SUPPORT)
extern CONST(ComM_PncComSignalType, COMM_CONST) ComM_PncComSignalConfig[COMM_NUMBER_OF_PNC_SIGNALS];
#endif
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
/*post-build configurable parameters*/
extern CONST(ComM_ConfigType, COMM_CONST) ComM_Config;
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
/*******************************************************************************
**                      Global Functions                                      **
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
FUNC(void, COMM_CODE)
ComM_Init(
    P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_DATA) ConfigPtr
);

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
FUNC(void, COMM_CODE)
ComM_DeInit(
    void
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetState(
    NetworkHandleType Channel,
    P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_DATA) State
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetStatus(
    P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetInhibitionStatus(
    NetworkHandleType Channel,
    P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_RequestComMode(
    ComM_UserHandleType User,
    ComM_ModeType ComMode
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetMaxComMode(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetRequestedComMode(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_GetCurrentComMode(
    ComM_UserHandleType User,
    P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_PreventWakeUp(
    NetworkHandleType Channel,
    boolean Status
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_LimitChannelToNoComMode(
    NetworkHandleType Channel,
    boolean Status
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_LimitECUToNoComMode(
    boolean Status
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_ReadInhibitCounter(
    P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_ResetInhibitCounter(
    void
);

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
FUNC(Std_ReturnType, COMM_CODE)
ComM_SetECUGroupClassification(
    ComM_InhibitionStatusType Status
);
#endif /*end of #if ((STD_ON==COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON==COMM_MODE_LIMITATION_ENABLED)) */

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
);
#endif/* end of #if (STD_ON == COMM_VERSION_INFO_API) */

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
FUNC(void, COMM_CODE)
ComM_MainFunction(
    NetworkHandleType Channel
);

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif /* COMM_H */
