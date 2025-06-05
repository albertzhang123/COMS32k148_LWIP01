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
**  FILENAME    : ComM_Internal.h                                             **
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
 *  V2.1.0       2021-09-27  darren.zhang   chanel struct add bus nm state
 *  V2.1.1       2022-03-17  darren.zhang   Adjust the data type macro switch
 */
#ifndef COMM_INTERNAL_H
#define COMM_INTERNAL_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Rte_ComM_Type.h"
#if (STD_ON == COMM_NM_USED)
#include "NmStack_Types.h"
#endif/* (STD_ON == COMM_NM_USED) */
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define COMM_INVALID_UINT8             0xFFu

/* Defines internal pnc state */
#define COMM_STATE_PNC_NO              0u
#define COMM_STATE_PNC_PREPARE_SLEEP   1u
#define COMM_STATE_PNC_READY_SLEEP     2u
#define COMM_STATE_PNC_REQ             3u

#define COMM_MAX_REJECT_COUNTER     0xFFFFu
/*#define COMM_MAX_NVM_TIMEOUT        0xFFFFu*/

/* Other module notify event */
#define COMM_EVENT_DCM_ACTIVE       0x0001u
#define COMM_EVENT_ECUM_WAKEUP      0x0002u
#define COMM_EVENT_ECUM_RUNMODE     0x0004u
#define COMM_EVENT_NM_START         0x0008u
#define COMM_EVENT_NM_RESTART       0x0010u
#define COMM_EVENT_NM_NETMODE       0x0020u
#define COMM_EVENT_NM_PRESLEEP      0x0040u
#define COMM_EVENT_NM_BUSSLEEP      0x0080u
#if (STD_ON == COMM_VERSION_R19_11)
#define COMM_EVENT_ECUM_PNCWAKEUP   0x0100u
#endif /* #if (STD_ON == COMM_VERSION_R19_11)*/
/* Bus mode indication event */
#define COMM_EVENT_BUS_REQUEST      0x1000u
#define COMM_EVENT_BUS_OK           0x2000u
#define COMM_EVENT_BUS_FAIL         0x4000u
/*bussm indication happen,the result need to judge further*/
#define COMM_EVENT_BUS_IND          0x6000u

/*Std_ReturnType extend: Function call has been successfully but mode can not be
 * granted because of mode inhibition.*/
#define COMM_E_MODE_LIMITATION   (Std_ReturnType)0x02

/* Defines event of all network requested passively in case of the NM is used. */
#if (STD_ON == COMM_VERSION_R19_11)
#define COMM_EVENT_PASSIVE_STARTUP \
    (COMM_EVENT_ECUM_WAKEUP|COMM_EVENT_NM_START|COMM_EVENT_NM_RESTART|COMM_EVENT_ECUM_PNCWAKEUP)
#else
#define COMM_EVENT_PASSIVE_STARTUP \
    (COMM_EVENT_ECUM_WAKEUP|COMM_EVENT_NM_START|COMM_EVENT_NM_RESTART)
#endif /* #if (STD_ON == COMM_VERSION_R19_11)*/

/* Defines the NM mode indication */
#define COMM_EVENT_NM_IND \
    (COMM_EVENT_NM_NETMODE|COMM_EVENT_NM_PRESLEEP|COMM_EVENT_NM_BUSSLEEP)

/* Maximum identification of the user */
#define COMM_MAX_USER_ID        (COMM_NUMBER_OF_USERS - 1u)

/* Maximum identification of the channel */
#define COMM_MAX_CHANNEL_ID     (COMM_NUMBER_OF_CHANNELS - 1u)

/* Set event status flag */
#define COMM_EVENT_SET(_ch,_mask) \
    do{\
            SCHM_ENTER_COMM_CH(_ch);\
            ComM_Channel[_ch].Events |= (_mask);\
            SCHM_EXIT_COMM_CH(_ch);\
        }while(0)

/* Clear event status flag */
#define COMM_EVENT_CLR(_ch,_mask) \
    do{\
            SCHM_ENTER_COMM_CH(_ch);\
            ComM_Channel[_ch].Events &= ((uint16)~(_mask));\
            SCHM_EXIT_COMM_CH(_ch);\
        }while(0)

/* Get event status */
#define COMM_EVENT_IS_SET(_ch,_mask) \
    (0u != (ComM_Channel[_ch].Events & (_mask)))

#if (STD_ON == COMM_PNC_SUPPORT)
/* Set pnc event*/
#define COMM_PNC_EVENT_SET(_pnc, _mask) \
    do{ComM_Pnc[_pnc].PncEvents |= (_mask);}while(0)

/* Clear pnc event */
#define COMM_PNC_EVENT_CLR(_pnc,_mask) \
    do{ComM_Pnc[_pnc].PncEvents &= ((uint16)~(_mask));}while(0)

/* Judge pnc event is set or not*/
#define COMM_PNC_EVENT_IS_SET(_pnc, _mask) \
    (0u != (ComM_Pnc[_pnc].PncEvents & (_mask)))

#define COMM_PNC_BYTE_INDEX(_pncId)     (((_pncId)>>3u)-COMM_PNC_VECTOR_OFFSET)
#define COMM_PNC_BIT_INDEX(_pncId)     ((_pncId)&7u)/*_pncId div 8,get the bitindex */

/* Set bit corresponding the pnc in signal to '1' */
#define COMM_SET_PNC_IN_SIGNAL(_pncId,_arry) \
    do{(_arry)[COMM_PNC_BYTE_INDEX(_pncId)] |= \
    (uint8)(1u << COMM_PNC_BIT_INDEX(_pncId));}while(0)

/* Clear bit corresponding the pnc in signal to '0'*/
#define COMM_CLEAR_PNC_IN_SIGNAL(_pncId,_arry) \
    do{(_arry)[COMM_PNC_BYTE_INDEX(_pncId)] &=~ \
    (uint8)(1u << COMM_PNC_BIT_INDEX(_pncId));}while(0)

/* judge pnc rx signal bit is '1' or not */
#define COM_IS_PNC_SET_RXSIGNAL(_pncId,_arry) \
     (0u != ((uint8)((1u << COMM_PNC_BIT_INDEX(_pncId)) & \
     (_arry)[COMM_PNC_BYTE_INDEX(_pncId)])))
#endif/* STD_ON == COMM_PNC_SUPPORT*/

/* Check whether wakeup inhibition is enabled or not. */
#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)
#define COMM_IS_INHIBIT_WAKEUP(_ch) \
    ((ComM_Global.EcuGroupClassification & ComM_Global.inhStu[_ch])==COMM_MASK_INHIBIT_WAKEUP)
#endif /*#if (STD_ON == COMM_WAKEUP_INHIBITION_ENABLED)*/

/* Check whether limitation is enabled or not. */
#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)
#define COMM_IS_LIMIT_TO_NOCOM(_ch) \
    ((COMM_PASSIVE == ComM_ChannelConfig[_ch].ComMNmVariant) || \
     (((ComM_Global.EcuGroupClassification & COMM_MASK_LIMIT_TO_NOCOM) != 0u) && \
      ((ComM_EcuToNoCom != 0u) || ((ComM_Global.inhStu[_ch] & COMM_MASK_LIMIT_TO_NOCOM) != 0u))))
#endif /*#if (STD_ON == COMM_MODE_LIMITATION_ENABLED)*/

#if (STD_ON == COMM_PNC_SUPPORT)
#define COMM_PNC_MODE_HANDLER(_pnc) \
    (ComM_PncModeHandle[ComM_Pnc[_pnc].PncState])
#endif

/* Get handles of mode processing.  */
#define COMM_MODE_HANDLER(_ch) \
    (ComM_ModeHandle[ComM_Channel[_ch].State])

/* Get handles of requestes communication mode. */
#define COMM_SM_REQ_COMMODE(_ch,_mode) \
    (ComM_SmHandle[ComM_ChannelConfig[_ch].ComMBusType].RequestComMode((_ch),(_mode)))

/* Get handles of read communication mode. */
#define COMM_SM_GET_COMMODE(_ch,_mode) \
    (ComM_SmHandle[ComM_ChannelConfig[_ch].ComMBusType].GetCurrentComMode((_ch),&(_mode)))

/* Set bus mode depending on the channel. */
#define COMM_SET_BUS_MODE(_ch, _mode) \
    do {\
        ComM_Channel[_ch].CurrentMode = (_mode);\
        ComM_BusNMModInd[Channel] = COMM_INVALID_UINT8;\
        COMM_EVENT_SET((_ch), COMM_EVENT_BUS_REQUEST);\
        if ((uint8)E_OK != COMM_SM_REQ_COMMODE((_ch), (_mode))) {\
            COMM_EVENT_CLR((_ch), COMM_EVENT_BUS_REQUEST);\
            COMM_EVENT_SET((_ch), COMM_EVENT_BUS_FAIL);\
        }\
    }while(0)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#if (STD_ON == COMM_PNC_SUPPORT)
typedef P2FUNC(void, COMM_APPL_CODE, ComM_PncModeHandleType)(PNCHandleType Pnc, NetworkHandleType Channel);
#endif

/* Defines handler of switches communication mode. */
typedef P2FUNC(void, COMM_APPL_CODE, ComM_ModeHandleType)(NetworkHandleType Channel);

typedef struct
{
    /* Defines handler of requests communication mode. */
    P2FUNC(Std_ReturnType, COMM_APPL_CODE, RequestComMode)(NetworkHandleType, ComM_ModeType);

    /* Defines handler of gets current communication mode. */
    P2FUNC(Std_ReturnType, COMM_APPL_CODE, GetCurrentComMode)
    (
        NetworkHandleType,
        P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA)
    );

} ComM_SmHandleType;

/* Defines run-time structure of a channel. */
typedef struct
{
    /* Previous mode of a channel */
    ComM_ModeType PreviousMode;

    /* Current mode of a channel */
    ComM_ModeType CurrentMode;

    /* Status of a channel */
    ComM_StateType State;
#if (STD_ON == COMM_NM_USED)
    /* special bus nm state */
    Nm_ModeType specNmMode;
#endif/* (STD_ON == COMM_NM_USED) */

    /*this timer is used for ComMTMinFullComModeDuration*/
    uint32 MinFullComModeTimer;

    /*this timer is used for ComMNmLightTimeout*/
    uint32 LightModeTimer;

    /* Other module flag events */
    uint16 Events;

    /* User counter of requests "full-communication" */
    uint8 UserFullCounter;

    boolean CommunicationAllowed;

#if (STD_ON == COMM_DCM_USED)
    boolean Dcm_ClearInd;
#endif
} ComM_ChannelType;

#if (STD_ON == COMM_PNC_SUPPORT)
typedef struct
{
    /*one signal to more pnc, one signal to one channel. receive signal reference channel, and pnc reference signal*/
    uint8 PncSignalChannelId;

    uint8 PncState;

    uint16 PncEvents;

    uint32 PncPrepareSleepTime;

    boolean PncPrepareSleepTimeStart;

    /* Previous mode of a channel */
    ComM_PncModeType PreviousPncMode;

    /* Current Pnc mode of a channel */
    ComM_PncModeType CurrentPncMode;

    ComM_PncComSignalKindType PncKind;

    /* User counter of requests "full-communication" */
    uint8 UserFullCounter;

    /* this pnc represent bit is set to 1, then counter++*/
    uint8 EIRACounter;

    uint8 EIRA[8];

    uint8 ERACounter;

    uint8 ERA[8];

} ComM_PncType;
#endif

typedef struct
{
    /*the same signal used the same Tx SignalBuffer*/
    uint8 SignalBuffer[8];
}ComM_PncSignalType;

/* Defines run-time structure of a user. */
typedef struct
{
    /* User request mode currently */
    ComM_ModeType RequestMode;

#if (STD_ON == COMM_RTE_NOTIFY_USED)
    /* Old mode */
    ComM_ModeType OldMode;

    /* Channel counter of the mode(eg. full-com, silent-com, no-com). */
    uint8 ChCounter[3u];
#endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/

} ComM_UserType;

/* Defines global run-time structure. */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
typedef struct
{
    boolean ComMNoWakeup[COMM_NUMBER_OF_CHANNELS];

    ComM_InhibitionStatusType inhStu[COMM_NUMBER_OF_CHANNELS];

    /* Inhibit counter of requests "full-communication" */
    uint16 InhibitCounter;

    /* Whether enable "wakeup-inhibition" and "communicate-limitiation" or not */
    ComM_InhibitionStatusType EcuGroupClassification;
} ComM_GlobalType;
#endif /*#if (COMM_WAKEUP_INHIBITION_ENABLED || COMM_MODE_LIMITATION_ENABLED)*/

typedef enum
{
    ECUM_REQUEST = 0u,
    ECUMPNC_REQUEST,
    USER_REQUESTFULLCOM,
    USER_REQUESTNOCOM
}ManagedHandlerType;
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

extern P2CONST(ComM_ConfigType, AUTOMATIC, COMM_CONST) ComM_ConfigPtr;

#if (STD_ON == COMM_PNC_SUPPORT)
/* Defines run-time variable of pnc mode handler. */
extern CONST(ComM_PncModeHandleType, COMM_CONST) ComM_PncModeHandle[4u];
#endif

extern CONST(ComM_SmHandleType, COMM_CONST) ComM_SmHandle[6u];

/* Defines run-time variable of mode handler. */
extern CONST(ComM_ModeHandleType, COMM_CONST) ComM_ModeHandle[5u];



/* Defines global run-time variable */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
extern VAR(ComM_GlobalType, COMM_VAR) ComM_Global;
#endif /*#if ((COMM_WAKEUP_INHIBITION_ENABLED==STD_ON) || (COMM_MODE_LIMITATION_ENABLED==STD_ON))*/

/* Defines run-time variable of all users */
extern VAR(ComM_UserType, COMM_VAR) ComM_User[COMM_NUMBER_OF_USERS];

/* Defines run-time variable of all channels */
extern VAR(ComM_ChannelType, COMM_VAR) ComM_Channel[COMM_NUMBER_OF_CHANNELS];

/* bus nm indication */
extern VAR(ComM_ModeType, COMM_VAR) ComM_BusNMModInd[COMM_NUMBER_OF_CHANNELS];

extern VAR(boolean, COMM_VAR) ComM_EcuToNoCom;

#if (STD_ON == COMM_PNC_SUPPORT)
/* Defines run-time variable of all Pncs */
extern VAR(ComM_PncType, COMM_VAR) ComM_Pnc[COMM_NUMBER_OF_PNCS];

extern VAR(ComM_PncSignalType, COMM_VAR) ComM_PncSignalbuffer[COMM_NUMBER_OF_TXSIGNAL];
#endif/* (STD_ON == COMM_PNC_SUPPORT) */

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/* Handle "Full Com" request of the user. */
#if ((STD_ON == COMM_WAKEUP_INHIBITION_ENABLED) || (STD_ON == COMM_MODE_LIMITATION_ENABLED))
extern FUNC(Std_ReturnType, COMM_CODE) ComM_UserRequestFullCom(NetworkHandleType Channel);
#endif /*#if ((STD_ON==COMM_WAKEUP_INHIBITION_ENABLED)||(STD_ON==COMM_MODE_LIMITATION_ENABLED))*/

/* Handle the DCM state indication. */
extern FUNC(void, COMM_CODE)  ComM_BusModeIndication(NetworkHandleType Channel);

/* Handle the NM state indication. */
#if (STD_ON == COMM_NM_USED)
extern FUNC(void, COMM_CODE) ComM_NmModeProcessing(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_NM_USED)*/

/* Handle upper-layer(RTE) state indication. */
#if (STD_ON == COMM_RTE_NOTIFY_USED)
extern FUNC(void, COMM_CODE) ComM_UserModeNotify(NetworkHandleType Channel);
#endif /*#if (STD_ON == COMM_RTE_NOTIFY_USED)*/

#if (STD_ON == COMM_VERSION_R19_11)
FUNC(void, COMM_CODE) ComM_ManagedChannelHandler(
    NetworkHandleType Channel,
    boolean NeedJudgePncRef,
    ManagedHandlerType ManagedHandler);

FUNC(boolean, COMM_CODE) ComM_ManagingChannelHandler(
    NetworkHandleType Channel);
#endif /* #if (STD_ON == COMM_VERSION_R19_11)*/

#if (STD_ON == COMM_PNC_GATEWAY_ENABLED)
FUNC(void, COMM_CODE)
ComM_PncGatewaySignalExchange(PNCHandleType Pnc);

FUNC(boolean, COMM_CODE)
ComM_JudgePncGatewayIsSet(PNCHandleType Pnc);
#endif/* #if (STD_ON == COMM_PNC_GATEWAY_ENABLED)*/

FUNC(void, COMM_CODE) ComM_BusSMIndProcess(NetworkHandleType Channel);

#if (STD_ON == COMM_PNC_SUPPORT)
FUNC(void, COMM_CODE) ComM_RxPncSignalProcess(
    uint16 signalId,uint8 SglArrayIndex,uint8 PncIndex);
#endif/* STD_ON == COMM_PNC_SUPPORT */


#endif/*COMM_INTERNAL_H*/
