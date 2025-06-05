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
**  FILENAME    : PduR.c                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for PDUR                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V2.0.0    20200622  zhengfei.li   Initial version
 *                                    (Upgrade according to the R19_11 standards)
 *  V2.0.1    20211103  zhengfei.li   add "PDUR_" to some Macro Name
 ******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Internal.h"
#if(STD_OFF == PDUR_ZERO_COST_OPERATION)
#include "Det.h"
#if(STD_ON == PDUR_CANIF_SUPPORT)
#include "PduR_CanIf.h"
#endif
#if(STD_ON == PDUR_CANTP_SUPPORT)
#include "PduR_CanTp.h"
#endif
#if(STD_ON == PDUR_J1939TP_SUPPORT)
#include "PduR_J1939Tp.h"
#endif
#if(STD_ON == PDUR_COM_SUPPORT)
#include "PduR_Com.h"
#endif
#if(STD_ON == PDUR_DCM_SUPPORT)
#include "PduR_Dcm.h"
#endif
#if(STD_ON == PDUR_J1939DCM_SUPPORT)
#include "PduR_J1939Dcm.h"
#endif
#if(STD_ON == PDUR_IPDUM_SUPPORT)
#include "PduR_IpduM.h"
#endif
#if(STD_ON == PDUR_J1939RM_SUPPORT)
#include "PduR_J1939Rm.h"
#endif
#if(STD_ON == PDUR_LDCOM_SUPPORT)
#include "PduR_LdCom.h"
#endif
#if(STD_ON == PDUR_DLT_SUPPORT)
#include "PduR_Dlt.h"
#endif
#if(STD_ON == PDUR_SECOC_SUPPORT)
#include "PduR_SecOC.h"
#endif
#if(STD_ON == PDUR_DBG_SUPPORT)
#include "PduR_Dbg.h"
#endif
#if(STD_ON == PDUR_CANNM_SUPPORT)
#include "PduR_CanNm.h"
#endif
#if(STD_ON == PDUR_OSEKNM_SUPPORT)
#include "PduR_OsekNm.h"
#endif
#if(STD_ON == PDUR_UDPNM_SUPPORT)
#include "PduR_UdpNm.h"
#endif
#if(STD_ON == PDUR_LINIF_SUPPORT)
#include "PduR_LinIf.h"
#endif
#if(STD_ON == PDUR_LINTP_SUPPORT)
#include "PduR_LinTp.h"
#endif
#if(STD_ON == PDUR_FRIF_SUPPORT)
#include "PduR_FrIf.h"
#endif
#if(STD_ON == PDUR_FRNM_SUPPORT)
#include "PduR_FrNm.h"
#endif
#if(STD_ON == PDUR_DOIP_SUPPORT)
#include "PduR_DoIP.h"
#endif
#if(STD_ON == PDUR_SOAD_SUPPORT)
#include "PduR_SoAd.h"
#endif
#if(STD_ON == PDUR_SOMEIPTP_SUPPORT)
#include "PduR_SomeIpTp.h"
#endif
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define PDUR_C_AR_MAJOR_VERSION  4u
#define PDUR_C_AR_MINOR_VERSION  2u
#define PDUR_C_AR_PATCH_VERSION  2u
#define PDUR_C_SW_MAJOR_VERSION  2u
#define PDUR_C_SW_MINOR_VERSION  0u
#define PDUR_C_SW_PATCH_VERSION  1u
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (PDUR_C_AR_MAJOR_VERSION != PDUR_H_AR_MAJOR_VERSION)
  #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_C_AR_MINOR_VERSION != PDUR_H_AR_MINOR_VERSION)
  #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_C_AR_PATCH_VERSION != PDUR_H_AR_PATCH_VERSION)
  #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_C_SW_MAJOR_VERSION != PDUR_H_SW_MAJOR_VERSION)
  #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_C_SW_MINOR_VERSION != PDUR_H_SW_MINOR_VERSION)
  #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_C_SW_PATCH_VERSION != PDUR_H_SW_PATCH_VERSION)
  #error "PduR.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if(STD_ON == PDUR_DEV_ERROR_DETECT)
#define PDUR_START_SEC_VAR_INIT_8
#include "PduR_MemMap.h"
static VAR(PduR_StateType,PDUR_VAR_POWER_ON_INIT)
PduR_Status = PDUR_UNINIT;
#define PDUR_STOP_SEC_VAR_INIT_8
#include "PduR_MemMap.h"
#endif

/*If gateway:dest pdu(PduRDestPduDataProvision) is configuration PDUR_DIRECT,
 * and PduRDestTxBufferRef is configuration*/
#if(PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u)
#define PDUR_START_SEC_VAR_NO_INIT_8
#include "PduR_MemMap.h"
static VAR(PduR_DestStateType,PDUR_VAR)
PduR_DestPduState[PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM];
#define PDUR_STOP_SEC_VAR_NO_INIT_8
#include "PduR_MemMap.h"
#endif

/*up module Tx 1:n(only If Pdu)*/
#if(PDUR_SRC_UP_MULTICAST_TX_IF_SUM > 0u)
#define PDUR_START_SEC_VAR_NO_INIT_BOOLEAN
#include "PduR_MemMap.h"
static VAR(boolean,PDUR_VAR)
PduR_UpTxState[PDUR_SRC_UP_MULTICAST_TX_IF_SUM];
#define PDUR_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "PduR_MemMap.h"
#endif

/*all route dest pdus is enable or disable*/
#if(PDUR_DEST_PDU_SUM > 0u)
#define PDUR_START_SEC_VAR_NO_INIT_BOOLEAN
#include "PduR_MemMap.h"
static VAR(boolean,PDUR_VAR)
PduRIsEnabled[PDUR_DEST_PDU_SUM];
#define PDUR_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "PduR_MemMap.h"
#else
#define PDUR_START_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"
static P2VAR(boolean, PDUR_VAR, PDUR_APPL_DATA) PduRIsEnabled = NULL_PTR;
#define PDUR_STOP_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"
#endif
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

#if(STD_ON == PDUR_TRANSMIT_SUPPORT)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeTransmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif/*STD_ON == PDUR_TRANSMIT_SUPPORT*/

#if(STD_ON == PDUR_CANCEL_TRANSMIT)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeCancelTransmit(PduIdType id);
#endif/*STD_ON == PDUR_CANCEL_TRANSMIT*/

#if(STD_ON == PDUR_CHANGE_PARAMETER)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeChangeParameter(PduIdType id,TPParameterType parameter,uint16 value);
#endif/*STD_ON == PDUR_CHANGE_PARAMETER*/

#if(STD_ON == PDUR_CANCEL_RECEIVE)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeCancelReceive(PduIdType id);
#endif/*STD_ON == PDUR_CANCEL_RECEIVE*/

#if(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_LoTpStartOfReception
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
);
#endif/*STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT*/

#if(STD_ON == PDUR_TP_COPYRXDATA)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_LoTpCopyRxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
);
#endif/*STD_ON == PDUR_TP_COPYRXDATA*/

#if(STD_ON == PDUR_TP_RXINDICATION)
static FUNC(void, PDUR_CODE)
PduR_LoTpRxIndication
(
    PduIdType id,
    Std_ReturnType result
);
#endif/*STD_ON == PDUR_TP_RXINDICATION*/

#if(STD_ON == PDUR_TP_COPYTXDATA)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_LoTpCopyTxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr
);
#endif/*STD_ON == PDUR_TP_COPYTXDATA*/

#if(STD_ON == PDUR_TP_TXCONFIRMATION)
static FUNC(void, PDUR_CODE)
PduR_LoTpTxConfirmation(PduIdType id,Std_ReturnType result);
#endif/*STD_ON == PDUR_TP_TXCONFIRMATION*/

/*Rx If pdu gateway to other If Pdus*/
#if(STD_ON == PDUR_RX_INDICATION)
static FUNC(void, PDUR_CODE)
PduR_GateWayIfPdu(
    uint8 DestModule,
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) InfoPtr
);

/*Called by PduR_GateWayIfPdu.
 *If pdu gateway(config buffer and direct) handle*/
#if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_GateWayDirectBufferHandle(
        PduR_LoIfTransmit_FuncPtrType pduR_LoIfTransmitApi,
        PduIdType DestPduId,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) InfoPtr);
#endif
#endif/*STD_ON == PDUR_RX_INDICATION*/

/*PDU(store in the buffer) gateway handle in the LoIfTxConfirmation API*/
#if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_IfTxConfirmationGatewayHandle(
    PduIdType TxPduId,
    PduR_LoIfTransmit_FuncPtrType PduR_LoIfTransmitApi);
#endif

#if((STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_CopyRxPduToTxBuffer(
        #if(STD_ON == PDUR_META_DATA_SUPPORT)
        uint8 MetaDataLength,
        #endif
        P2VAR(PduR_TxBufferType, AUTOMATIC, PDUR_APPL_DATA) TxBufferPtr,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA)PduInfo,
        PduLengthType PduLength);
#endif/*(STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u)*/

/*store the If Rx Pdu data to buffer*/
#if((STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_EnQueueBuffer(
    PduIdType PduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA)PduInfo
);
#endif/*(STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u)*/

#if((STD_ON == PDUR_TRIGGER_TRANSMIT) || (PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u))
#if(PDUR_TX_BUFFER_SUM > 0u)
/*clear the buffer(the buffer data have transmit,Whether or not it succeeds)*/
static FUNC(void, PDUR_CODE)
PduR_DeQueueBuffer(PduIdType PduId);

/*Called by PduR_IfTxConfirmation,PduR_IfTriggerTransmitGwHandle.
 *Get the lower module's IfTransmit Api*/
static FUNC(void, PDUR_CODE)
PduR_GetLoIfTransmitApi(
        PduIdType TxPduId,
        PduR_LoIfTransmit_FuncPtrType* PduR_LoIfTransmitApi);
#endif/*PDUR_TX_BUFFER_SUM > 0u*/
#endif/*(STD_ON == PDUR_TRIGGER_TRANSMIT) || (PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u)*/

/*Called by PduR_IfTriggerTransmit.
 *IfTriggerTransmit GW handle*/
#if((STD_ON == PDUR_TRIGGER_TRANSMIT) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_IfTriggerTransmitGwHandle(
        PduIdType TxPduId,
        P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr);
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/*PduR PB Configuration Run Time point parameter*/
#define PDUR_START_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"
P2CONST(PduR_PBConfigType, PDUR_CONST, PDUR_CONST_PBCFG) PduR_ConfigStd = NULL_PTR;
#define PDUR_STOP_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"

/*gateway Tp routing path:all dest pdu(SF/MF,route to Tp or Up)*/
#if(PDUR_DEST_GATEWAY_TP_PDU_SUM > 0u)
#define PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
VAR(PduR_GateWayDestTpRunTimeType,PDUR_VAR)
PduR_GateWayDestTpRTSate[PDUR_DEST_GATEWAY_TP_PDU_SUM];
#define PDUR_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
#endif
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               Initializes the PDU Router
 * ServiceId           0xf0
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to Post build configuration data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, PDUR_CODE)
PduR_Init(P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_CONST_PBCFG) ConfigPtr)
{
    #if (PDUR_ROUTING_PATH_GROUP_SUM > 0u)
    PduIdType cnt,destPduNumber,destIndex;
    #endif
    uint16 index;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(NULL_PTR == ConfigPtr)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_INIT_ID, PDUR_E_INIT_FAILED);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        PduR_ConfigStd = ConfigPtr;
        #if (0u < PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM)
        /*if source pdu gateway to other if dest pdus which configed PDUR_DIRECT */
        for(index = 0u;index < PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM;index++)
        {
            PduR_DestPduState[index] = PDUR_IDLE;
        }
        #endif/* 0u < PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM */
        /* init all dest pdu state base on RoutingPathGroup parameter PduRIsEnabledAtInit.
         * default state is enabled for dest pdus not included any path group */
        for(index = 0u;index < PDUR_DEST_PDU_SUM;index++)
        {
            PduRIsEnabled[index] = TRUE;
        }
        #if (0u < PDUR_ROUTING_PATH_GROUP_SUM)
        for(index = 0u;index < PDUR_ROUTING_PATH_GROUP_SUM;index++)
        {
            if(FALSE == PDUR_ROUTEPATHGROUP_CFG(index).PduRIsEnabledAtInit)
            {
                destPduNumber = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduRefNumber;
                for(cnt = 0u;cnt < destPduNumber;cnt++)
                {
                    destIndex = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduRef[cnt];
                    PduRIsEnabled[destIndex] = FALSE;
                }
            }
        }
        #endif/*(0u < PDUR_ROUTING_PATH_GROUP_SUM)*/

        #if (0u < PDUR_SRC_UP_MULTICAST_TX_IF_SUM)
        /*up module 1:n(n>1)transmit,used to handle up TxConfirmation*/
        for(index = 0u;index < PDUR_SRC_UP_MULTICAST_TX_IF_SUM;index++)
        {
            PduR_UpTxState[index] = TRUE;
        }
        #endif

        #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
        /*gateway tp routing path,Run Time State of all dest pdu(include up pdu and tp pdu)*/
        for(index = 0u;index < PDUR_DEST_GATEWAY_TP_PDU_SUM;index++)
        {
            PduR_GateWayDestTpRTSate[index].ActiveTpBufferId = PDUR_UNUSED_UINT8;
            PduR_GateWayDestTpRTSate[index].NeedGateWayOnTheFly = FALSE;
            PduR_GateWayDestTpRTSate[index].TpTxConfirmation = TRUE;
            PduR_GateWayDestTpRTSate[index].TxBufferOffset = 0u;
        }
        #endif
        /*Init TP Buffer*/
        #if(0u < PDUR_TP_BUFFER_SUM)
        for(index = 0u;index < PDUR_TP_BUFFER_SUM;index++)
        {
            PduR_TpBuffer[index].used = FALSE;
        }
        #endif
        /*Init Tx Buffer*/
        #if(0u < PDUR_TX_BUFFER_SUM)
        for(index = 0u;index < PDUR_TX_BUFFER_SUM;index++)
        {
            uint8 depthCnt,depth;
            depth = PduR_TxBuffer[index].PduRTxBufferDepth;
            for(depthCnt = 0u;depthCnt < depth;depthCnt++)
            {
                PduR_TxBuffer[index].PduRTxBufferRef[depthCnt].used = FALSE;
            }
        }
        #endif
        #if(STD_ON == PDUR_DEV_ERROR_DETECT)
        PduR_Status = PDUR_ONLINE;
        #endif
    }
    return;
}
/******************************************************************************/
/*
 * Brief               Returns the unique identifier of the post-build time
 *                     configuration of the PDU Router
 * ServiceId           0xf2
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              PduR_PBConfigIdType: Identifier of the post-build time configuration
 */
/******************************************************************************/
FUNC(PduR_PBConfigIdType, PDUR_CODE)
PduR_GetConfigurationId(void)
{
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_GETCONFIGURATIONID_ID, PDUR_E_UNINIT);
    }
    #endif
    return PduR_ConfigStd->PduRConfigId;
}
/******************************************************************************/
/*
 * Brief               Enables a routing path table
 * ServiceId           0xf3
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the routing path group.
 *                     Routing path groups are defined in the PDU router configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, PDUR_CODE)
PduR_EnableRouting(PduR_RoutingPathGroupIdType id)
{
    #if (0u < PDUR_ROUTING_PATH_GROUP_SUM)
    PduIdType destPduId;
    PduIdType destPduNumber;
    PduIdType index;
    #endif
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_ENABLEROUTING_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if (0u < PDUR_ROUTING_PATH_GROUP_SUM)
            && (id >= PDUR_ROUTING_PATH_GROUP_SUM)
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_ENABLEROUTING_ID, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if (0u < PDUR_ROUTING_PATH_GROUP_SUM)
        destPduNumber = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduRefNumber;
        for(index = 0u;index < destPduNumber;index++)
        {
            destPduId = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduRef[index];
            PduRIsEnabled[destPduId] = TRUE;
        }
        #endif
    }
    PDUR_NOUSED(id);
    return;
}
/******************************************************************************/
/*
 * Brief               Disables a routing path table
 * ServiceId           0xf4
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the routing path group.
 *                     Routing path groups are defined in the PDU router configuration
 *                     initialize (true,false)
 *                     true: initialize single buffers to the default value
 *                     false: retain current value of single buffers
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, PDUR_CODE)
PduR_DisableRouting(PduR_RoutingPathGroupIdType id,boolean initialize)
{
    #if (0u < PDUR_ROUTING_PATH_GROUP_SUM)
    PduIdType destPduNumber;
    PduIdType index;
    PduIdType destPduId;
    #if(PDUR_TX_BUFFER_SUM > 0u)
    uint16 txBufferId;
    uint8 cnt,txBufferDepth;
    boolean clearTxBuffer;
    #endif
    #endif
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_DISABLEROUTING_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_ROUTING_PATH_GROUP_SUM)
            && (id >= PDUR_ROUTING_PATH_GROUP_SUM)
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_DISABLEROUTING_ID, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if (0u < PDUR_ROUTING_PATH_GROUP_SUM)
        destPduNumber = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduRefNumber;
        for(index = 0u;index < destPduNumber;index++)
        {
            destPduId = PDUR_ROUTEPATHGROUP_CFG(id).PduRDestPduRef[index];
            if(TRUE == PduRIsEnabled[destPduId])
            {
                #if(PDUR_TX_BUFFER_SUM > 0u)
                txBufferId = PDUR_DESTPDU_CFG(destPduId).PduRDestTxBufferRef;
                if(PDUR_UNUSED_UINT16 != txBufferId)
                {
                    txBufferDepth = PduR_TxBuffer[txBufferId].PduRTxBufferDepth;
                    if(1u == txBufferDepth)
                    {
                        if(PDUR_TRIGGERTRANSMIT == PDUR_DESTPDU_CFG(destPduId).PduRDestPduDataProvision)
                        {
                            /*the according buffer shall be set to the default value*/
                            if(TRUE == initialize)
                            {
                                #if(PDUR_DEFAULT_VALUE_LENGTH > 0u)
                                if(NULL_PTR != PDUR_DESTPDU_CFG(destPduId).PduRDefaultValueRef)
                                {
                                    PduR_TxBuffer[txBufferId].PduRTxBufferRef[0u].SduLength =
                                            PDUR_DESTPDU_CFG(destPduId).PduRDefaultValueRef->DefaultValueLength;
                                    PduR_Memcpy(
                                        PduR_TxBuffer[txBufferId].PduRTxBufferRef[0u].TxBufferData,
                                        &PduR_Default_value[PDUR_DESTPDU_CFG(destPduId).PduRDefaultValueRef->DefaultValueStart],
                                        PDUR_DESTPDU_CFG(destPduId).PduRDefaultValueRef->DefaultValueLength);
                                    PduR_TxBuffer[txBufferId].PduRTxBufferRef[0u].used = TRUE;
                                }
                                #endif/*PDUR_DEFAULT_VALUE_LENGTH > 0u*/
                            }
                            clearTxBuffer = FALSE;
                        }
                        else
                        {
                            clearTxBuffer = TRUE;
                        }
                    }
                    else
                    {
                        clearTxBuffer = TRUE;
                    }
                    /*clear and the buffer*/
                    if(TRUE == clearTxBuffer)
                    {
                        if(TRUE == PduR_TxBuffer[txBufferId].PduRTxBufferRef[0u].used)
                        {
                            (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_DISABLEROUTING_ID, PDUR_E_PDU_INSTANCES_LOST);
                        }
                        for(cnt = 0u;cnt < txBufferDepth;cnt++)
                        {
                            PduR_TxBuffer[txBufferId].PduRTxBufferRef[cnt].used = FALSE;
                        }
                    }
                }
                #endif/*PDUR_TX_BUFFER_SUM > 0u*/
                PduRIsEnabled[destPduId] = FALSE;
            }
        }
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(initialize);
    return;
}
/******************************************************************************/
/*
 * Brief               Requests transmission of an I-PDU.
 * ServiceId           0x49
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info: Length and pointer to the buffer of the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType (E_OK,E_NOT_OK)
 *                     E_OK - request is accepted by the destination module; transmission is continued.
 *                     E_NOT_OK - request is not accepted by the destination module;transmission is aborted.
 */
/******************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE)
PduR_Transmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
    Std_ReturnType result = E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TRANSMIT_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (NULL_PTR == info))
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TRANSMIT_ID, PDUR_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && (id >= PDUR_SRC_PDU_SUM)
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TRANSMIT_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TRANSMIT_SUPPORT)
        result = PduR_UpModeTransmit(id,info);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(info);
    return result;
}
/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of an I-PDU in a lower
 *                     layer communication interface or transport protocol module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType (E_OK,E_NOT_OK)
 *                     E_OK - Cancellation was executed successfully by the destination module.
 *                     E_NOT_OK - Cancellation was rejected by the destination module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE)
PduR_CancelTransmit(PduIdType id)
{
    Std_ReturnType result = E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANCELTRANSMIT_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && (id >= PDUR_SRC_PDU_SUM)
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANCELTRANSMIT_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_CANCEL_TRANSMIT)
        result = PduR_UpModeCancelTransmit(id);
        #endif
    }
    PDUR_NOUSED(id);
    return result;
}
/******************************************************************************/
/*
 * Brief               Request to change a specific transport protocol parameter (e.g. block size).
 * ServiceId           0x4b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identifiaction of the I-PDU which the parameter change shall affect.
 *                     parameter: The parameter that shall change.
 *                     value: The new value of the parameter
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType (E_OK,E_NOT_OK)
 *                     E_OK: The parameter was changed successfully.
 *                     E_NOT_OK: The parameter change was rejected.
 */
/******************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE)
PduR_ChangeParameter(PduIdType id,TPParameterType parameter,uint16 value)
{
    Std_ReturnType result = E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CHANGEPARAMETER_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && ((id >= PDUR_SRC_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CHANGEPARAMETER_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_CHANGE_PARAMETER)
        result = PduR_UpModeChangeParameter(id,parameter,value);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(parameter);
    PDUR_NOUSED(value);
    return result;
}
/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing reception of an I-PDU in a lower layer transport protocol module.
 * ServiceId           0x4c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU to be cancelled.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType (E_OK,E_NOT_OK)
 *                     E_OK: Cancellation was executed successfully by the destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE)
PduR_CancelReceive(PduIdType id)
{
    Std_ReturnType result = E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANCELRECEIVE_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && ((id >= PDUR_SRC_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_CANCELRECEIVE_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_CANCEL_RECEIVE)
        result = PduR_UpModeCancelReceive(id);
        #endif
    }
    PDUR_NOUSED(id);
    return result;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received I-PDU from a lower layer communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      RxPduId: ID of the received I-PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the received I-PDU and a pointer to a buffer (SduDataPtr) containing the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(void, PDUR_CODE)
PduR_IfRxIndication(
        PduIdType RxPduId,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr)
{
    #if(STD_ON == PDUR_RX_INDICATION)
    uint8 cnt,pduDestSum;
    PduIdType pduRDestPduId;
    PduIdType destModulePduId;
    uint8 destModuleIndex,destModule;
    #endif
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFRXINDICATION_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)&&((NULL_PTR == PduInfoPtr)||(NULL_PTR == PduInfoPtr->SduDataPtr)
            #if(STD_ON == PDUR_META_DATA_SUPPORT)
            || ((0u < PDUR_SRCEPDU_CFG(RxPduId).MetaDataLength) && (NULL_PTR == PduInfoPtr->MetaDataPtr))
            #endif
            ))
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFRXINDICATION_ID, PDUR_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) &&
            #if(0u < PDUR_SRC_PDU_SUM)
            ((RxPduId >= PDUR_SRC_PDU_SUM) || (TRUE == PDUR_ROUTINGPATH_CFG(RxPduId).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFRXINDICATION_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_RX_INDICATION)
        pduDestSum = PDUR_ROUTINGPATH_CFG(RxPduId).PduDestSum;
        for(cnt = 0u;cnt < pduDestSum;cnt++)
        {
            pduRDestPduId = PDUR_ROUTINGPATH_CFG(RxPduId).PduRDestPduIdRef[cnt];
            destModulePduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
            destModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
            destModule = PduR_BswModuleConfigData[destModuleIndex].PduRBswModuleRef;
            if(TRUE == PduRIsEnabled[pduRDestPduId])
            {
                switch(destModule)
                {
                    #if(STD_ON == PDUR_COM_SUPPORT)
                    case PDUR_COM:
                        Com_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_LDCOM_SUPPORT)
                    case PDUR_LDCOM:
                        LdCom_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_DLT_SUPPORT)
                    case PDUR_DLT:
                        Dlt_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_SECOC_SUPPORT)
                    case PDUR_SECOC:
                        SecOC_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_IPDUM_SUPPORT)
                    case PDUR_IPDUM:
                        IpduM_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_J1939DCM_SUPPORT)
                    case PDUR_J1939DCM:
                        J1939Dcm_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_J1939RM_SUPPORT)
                    case PDUR_J1939RM:
                        J1939Rm_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    #if(STD_ON == PDUR_SOMEIPTP_SUPPORT)
                    case PDUR_SOMEIPTP:
                        SomeIpTp_RxIndication(destModulePduId, PduInfoPtr);
                        break;
                    #endif
                    default:
                        PduR_GateWayIfPdu(destModule,pduRDestPduId,PduInfoPtr);
                        break;
                }
            }
        }
        #endif/*STD_ON == PDUR_RX_INDICATION*/
    }
    PDUR_NOUSED(RxPduId);
    PDUR_NOUSED(PduInfoPtr);
    return;
}
#define PDUR_STOP_SEC_IFRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms the transmission of an IPDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the I-PDU that has been transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(void, PDUR_CODE)
PduR_IfTxConfirmation(PduIdType TxPduId)
{
    #if(STD_ON == PDUR_TX_CONFIRMATION)
    PduIdType srcUpPduId;
    PduIdType srcPduId;
    uint8 srcPduModuleIndex;
    uint8 srcPduModule;
    PduR_UpIfTxConfirmation_FuncPtrType pduR_UpIfTxConfirmationApi = NULL_PTR;
    #endif
    #if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
    PduR_LoIfTransmit_FuncPtrType PduR_LoIfTransmitApi = NULL_PTR;
    #endif
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFTXCONFIRMATION_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_DEST_PDU_SUM)
            && (TxPduId >= PDUR_DEST_PDU_SUM)
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFTXCONFIRMATION_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TX_CONFIRMATION)
        srcPduId = PDUR_DESTPDU_CFG(TxPduId).PduRSrcPduRef;
        if(FALSE == PDUR_ROUTINGPATH_CFG(srcPduId).TpRoute)
        {
            srcPduModuleIndex = PDUR_SRCEPDU_CFG(srcPduId).BswModuleIndex;
            srcPduModule = PduR_BswModuleConfigData[srcPduModuleIndex].PduRBswModuleRef;
            switch(srcPduModule)
            {
                #if(STD_ON == PDUR_COM_SUPPORT)
                case PDUR_COM:
                    pduR_UpIfTxConfirmationApi = Com_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_LDCOM_SUPPORT)
                case PDUR_LDCOM:
                    pduR_UpIfTxConfirmationApi = LdCom_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_DLT_SUPPORT)
                case PDUR_DLT:
                    pduR_UpIfTxConfirmationApi = Dlt_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_SECOC_SUPPORT)
                case PDUR_SECOC:
                    pduR_UpIfTxConfirmationApi = SecOC_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_IPDUM_SUPPORT)
                case PDUR_IPDUM:
                    pduR_UpIfTxConfirmationApi = IpduM_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_DCM_SUPPORT)
                case PDUR_DCM:
                    pduR_UpIfTxConfirmationApi = Dcm_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_J1939DCM_SUPPORT)
                case PDUR_J1939DCM:
                    pduR_UpIfTxConfirmationApi = J1939Dcm_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_J1939RM_SUPPORT)
                case PDUR_J1939RM:
                    pduR_UpIfTxConfirmationApi = J1939Rm_TxConfirmation;
                    break;
                #endif
                #if(STD_ON == PDUR_SOMEIPTP_SUPPORT)
                case PDUR_SOMEIPTP:
                    pduR_UpIfTxConfirmationApi = SomeIpTp_TxConfirmation;
                    break;
                #endif
                default:/*If layer:Gateway pdu*/
                    #if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
                    PduR_GetLoIfTransmitApi(TxPduId,&PduR_LoIfTransmitApi);
                    if(NULL_PTR != PduR_LoIfTransmitApi)
                    {
                        PduR_IfTxConfirmationGatewayHandle(
                            TxPduId,
                            PduR_LoIfTransmitApi);
                    }
                    #endif
                    break;
            }
            if((NULL_PTR != pduR_UpIfTxConfirmationApi)
                    && (TRUE == PDUR_SRCEPDU_CFG(srcPduId).PduRSrcPduUpTxConf))
            {
                srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                /*route 1:1 Pdu from up module to lo If module*/
                if(PDUR_UNUSED_UINT16 == PDUR_SRCEPDU_CFG(srcPduId).UpTxconfirmStateIndex)
                {
                    pduR_UpIfTxConfirmationApi(srcUpPduId);
                }
                /*route 1:n Pdu from up module to lo If module,only the first Lo TxConfirm call Up TxConfirm*/
                #if(PDUR_SRC_UP_MULTICAST_TX_IF_SUM > 0u)
                else
                {
                    if(FALSE == PduR_UpTxState[PDUR_SRCEPDU_CFG(srcPduId).UpTxconfirmStateIndex])
                    {
                        pduR_UpIfTxConfirmationApi(srcUpPduId);
                        PduR_UpTxState[PDUR_SRCEPDU_CFG(srcPduId).UpTxconfirmStateIndex] = TRUE;
                    }
                }
                #endif/*PDUR_SRC_UP_MULTICAST_TX_IF_SUM > 0u*/
            }
        }
        #endif/*STD_ON == PDUR_TX_CONFIRMATION*/
    }
    PDUR_NOUSED(TxPduId);
    return;       
}
#define PDUR_STOP_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module) shall check whether the
 *                     available data fits into the buffer size reported by PduInfoPtr->SduLength.
 *                     If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr
 *                     and update the length of the actual copied data in PduInfoPtr->SduLength.
 *                     If not, it returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the SDU that is requested to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU
 *                     data shall be copied, and the available buffer size in SduLengh.
 *                     On return, the service will indicate the length of the copied SDU
 *                     data in SduLength.
 * Return              None
 */
/******************************************************************************/
#define PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(Std_ReturnType, PDUR_CODE)
PduR_IfTriggerTransmit(
        PduIdType TxPduId,
        P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
    #if(STD_ON == PDUR_TRIGGER_TRANSMIT)
    PduIdType srcPduId;
    PduIdType srcUpPduId;
    uint8 srcModuleIndex;
    uint8 srcModule;
    #endif
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_DEST_PDU_SUM)
            && (TxPduId >= PDUR_DEST_PDU_SUM)
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TRIGGER_TRANSMIT)
        srcPduId = PDUR_DESTPDU_CFG(TxPduId).PduRSrcPduRef;
        if((TRUE == PduRIsEnabled[TxPduId]) && (FALSE == PDUR_ROUTINGPATH_CFG(srcPduId).TpRoute))
        {
            srcPduId = PDUR_DESTPDU_CFG(TxPduId).PduRSrcPduRef;
            srcModuleIndex = PDUR_SRCEPDU_CFG(srcPduId).BswModuleIndex;
            srcModule = PduR_BswModuleConfigData[srcModuleIndex].PduRBswModuleRef;
            switch(srcModule)
            {
                #if(STD_ON == PDUR_COM_TRIGGER_TRANSMIT)
                case PDUR_COM:
                    srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                    result = Com_TriggerTransmit(srcUpPduId,PduInfoPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_LDCOM_TRIGGER_TRANSMIT)
                case PDUR_LDCOM:
                    srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                    result = LdCom_TriggerTransmit(srcUpPduId,PduInfoPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DLT_TRIGGER_TRANSMIT)
                case PDUR_DLT:
                    srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                    result = Dlt_TriggerTransmit(srcUpPduId,PduInfoPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_SECOC_TRIGGER_TRANSMIT)
                case PDUR_SECOC:
                    srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                    result = SecOC_TriggerTransmit(srcUpPduId,PduInfoPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_IPDUM_TRIGGER_TRANSMIT)
                case PDUR_IPDUM:
                    srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                    result = IpduM_TriggerTransmit(srcUpPduId,PduInfoPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_SOMEIPTP_TRIGGER_TRANSMIT)
                case PDUR_SOMEIPTP:
                    srcUpPduId = PDUR_SRCEPDU_CFG(srcPduId).PduRDestModulePduIndex;
                    result = SomeIpTp_TriggerTransmit(srcUpPduId,PduInfoPtr);
                    break;
                #endif
                default:
                    /*If layer:Gateway pdu*/
                    #if(PDUR_TX_BUFFER_SUM > 0u)
                    result = PduR_IfTriggerTransmitGwHandle(TxPduId,PduInfoPtr);
                    #endif/*PDUR_TX_BUFFER_SUM > 0u*/
                    break;
            }
        }
        #endif/*STD_ON == PDUR_TRIGGER_TRANSMIT*/
    }
    PDUR_NOUSED(TxPduId);
    PDUR_NOUSED(PduInfoPtr);
    return result;
}
#define PDUR_STOP_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called at the start of receiving an N-SDU. The N-SDU might be
 *                     fragmented into multiple N-PDUs (FF with one or more following CFs) or might
 *                     consist of a single N-PDU (SF).
 * ServiceId           0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the payload
 *                     data (without protocol information) and payload length of the
 *                     first frame or single frame of a transport protocol I-PDU
 *                     reception. Depending on the global parameter
 *                     MetaDataLength, additional bytes containing MetaData (e.g.
 *                     the CAN ID) are appended after the payload data, increasing
 *                     the length accordingly. If neither first/single frame data nor
 *                     MetaData are available, this parameter is set to NULL_PTR.
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving module. This
 *                     parameter will be used to compute the Block Size (BS) in the transport protocol module.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType(BUFREQ_OK,BUFREQ_E_NOT_OK,BUFREQ_E_OVFL)
 *                     BUFREQ_OK: Connection has been accepted. bufferSizePtr
 *                     indicates the available receive buffer; reception is continued.
 *                     If no buffer of the requested size is available, a receive buffer
 *                     size of 0 shall be indicated by bufferSizePtr.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected;
 *                     reception is aborted. bufferSizePtr remains unchanged.
 *                     BUFREQ_E_OVFL: No buffer of the required length can be
 *                     provided; reception is aborted. bufferSizePtr remains unchanged.
 */
/******************************************************************************/
#define PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_TpStartOfReception
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPSTARTOFRECEPTION_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && ((id >= PDUR_SRC_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPSTARTOFRECEPTION_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((NULL_PTR == bufferSizePtr)
            #if(STD_ON == PDUR_META_DATA_SUPPORT)
            || ((0u < PDUR_SRCEPDU_CFG(id).MetaDataLength) && (NULL_PTR == info->MetaDataPtr))
            #endif
            ))
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPSTARTOFRECEPTION_ID, PDUR_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
        bufQeqReturn = PduR_LoTpStartOfReception(id,info,TpSduLength,bufferSizePtr);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(info);
    PDUR_NOUSED(TpSduLength);
    PDUR_NOUSED(bufferSizePtr);
    return bufQeqReturn;
}
#define PDUR_STOP_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer.
 *                     Each call to this function provides the next part of the I-PDU data.
 *                     The size of the remaining data is written to the position indicated by bufferSizePtr.
 * ServiceId           0x44
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the received I-PDU.
 *                     info: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 *                     An SduLength of 0 can be used to query the current amount of available buffer in the upper
 *                     layer module. In this case, the SduDataPtr may be a NULL_PTR.
 * Param-Name[out]     bufferSizePtr: Available receive buffer after data has been copied.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType(BUFREQ_OK,BUFREQ_E_NOT_OK)
 *                     BUFREQ_OK: Data copied successfully
 *                     BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 */
/******************************************************************************/
#define PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_TpCopyRxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPCOPYRXDATA_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && ((id >= PDUR_SRC_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPCOPYRXDATA_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr)))
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPCOPYRXDATA_ID, PDUR_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TP_COPYRXDATA)
        bufQeqReturn = PduR_LoTpCopyRxData(id,info,bufferSizePtr);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(info);
    PDUR_NOUSED(bufferSizePtr);
    return bufQeqReturn;
}
#define PDUR_STOP_SEC_TPCOPYRXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               Called after an I-PDU has been received via the TP API, the result indicates
 *                     whether the transmission was successful or not.
 * ServiceId           0x45
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the received I-PDU.
 *                     result: Result of the reception.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(void, PDUR_CODE)
PduR_TpRxIndication
(
    PduIdType id,
    Std_ReturnType result
)
{
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPRXINDICATION_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_SRC_PDU_SUM)
            && ((id >= PDUR_SRC_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(id).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPRXINDICATION_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TP_RXINDICATION)
        PduR_LoTpRxIndication(id,result);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(result);
    return;
}
#define PDUR_STOP_SEC_TPRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *                     Each call to this function provides the next part of the I-PDU data unless retry-
 *                     >TpDataState is TP_DATARETRY. In this case the function restarts to copy the
 *                     data beginning at the offset from the current position indicated by retry-
 *                     >TxTpDataCnt. The size of the remaining data is written to the position indicated
 *                     by availableDataPtr.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     info: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 *                     If not enough transmit data is available, no data is copied by the upper layer module and
 *                     BUFREQ_E_BUSY is returned.The lower layer module may retry the call.An SduLength of 0 can be used to
 *                     indicate state changes in the retry parameter or to query the current amount of available data in the
 *                     upper layer module. In this case, the SduDataPtr may be a NULL_PTR.
 *                     retry: This parameter is used to acknowledge transmitted data or to retransmit data after transmission problems.
 *                     If the retry parameter is a NULL_PTR, it indicates that the transmit data can be removed from the buffer immediately
 *                     after it has been copied. Otherwise, the retry parameter must point to a valid RetryInfoType element.
 *                     If TpDataState indicates TP_CONFPENDING, the previously copied data must remain in the TP buffer to be available for
 *                     error recovery.TP_DATACONF indicates that all data that has been copied before this call is confirmed and can be
 *                     removed from the TP buffer. Data copied by this API call is excluded and will be confirmed later.TP_DATARETRY indicates
 *                     that this API call shall copy previously copied data in order to recover from an error. In this case TxTpDataCnt specifies
 *                     the offset in bytes from the current data copy position.
 * Param-Name[out]     availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer module's Tx buffer.
 *                     availableDataPtr can be used by TP modules that support dynamic payload lengths (e.g. FrIsoTp) to determine the size
 *                     of the following CFs.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType (BUFREQ_OK,BUFREQ_E_BUSY,BUFREQ_E_NOT_OK)
 *                     BUFREQ_OK: Data has been copied to the transmit buffer completely as requested.
 *                     BUFREQ_E_BUSY: Request could not be fulfilled, because the required amount of Tx data is not available. The lower layer
 *                     module may retry this call later on. No data has been copied.
 *                     BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 */
/******************************************************************************/
#define PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_TpCopyTxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    PduIdType srcPdu;
    srcPdu = PDUR_DESTPDU_CFG(id).PduRSrcPduRef;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPCOPYTXDATA_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_DEST_PDU_SUM)
            && ((id >= PDUR_DEST_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(srcPdu).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPCOPYTXDATA_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TP_COPYTXDATA)
        bufQeqReturn = PduR_LoTpCopyTxData(id,info,retry,availableDataPtr);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(info);
    PDUR_NOUSED(retry);
    PDUR_NOUSED(availableDataPtr);
    return bufQeqReturn;
}
#define PDUR_STOP_SEC_TPCOPYTXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function is called after the I-PDU has been transmitted on its network, the
 *                     result indicates whether the transmission was successful or not.
 * ServiceId           0x48
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     result: Result of the transmission of the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
#define PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
FUNC(void, PDUR_CODE)
PduR_TpTxConfirmation(
    PduIdType id,
    Std_ReturnType result)
{
    #if(STD_ON == PDUR_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    PduIdType srcPdu;
    srcPdu = PDUR_DESTPDU_CFG(id).PduRSrcPduRef;
    if(PDUR_ONLINE != PduR_Status)
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPTXCONFIRMATION_ID, PDUR_E_UNINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr)
            #if(0u < PDUR_DEST_PDU_SUM)
            && ((id >= PDUR_DEST_PDU_SUM) || (FALSE == PDUR_ROUTINGPATH_CFG(srcPdu).TpRoute))
            #endif
            )
    {
        (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_TPTXCONFIRMATION_ID, PDUR_E_PDU_ID_INVALID);
        detNoErr = FALSE;
    }
    if(TRUE == detNoErr)
    #endif
    {
        #if(STD_ON == PDUR_TP_TXCONFIRMATION)
        PduR_LoTpTxConfirmation(id,result);
        #endif
    }
    PDUR_NOUSED(id);
    PDUR_NOUSED(result);
    return;
}
#define PDUR_STOP_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if(STD_ON == PDUR_TRANSMIT_SUPPORT)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeTransmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
    Std_ReturnType result = E_NOT_OK;
    Std_ReturnType resultLo;
    PduIdType pduRDestPduId;
    PduIdType loModulePduId;
    uint8 loModuleIndex;
    uint8 loModule;
    uint8 pduDestSum;
    uint8 cnt;
    pduDestSum = PDUR_ROUTINGPATH_CFG(id).PduDestSum;
    /*route one up module pdu to tp module,just support 1:1*/
    if(TRUE == PDUR_ROUTINGPATH_CFG(id).TpRoute)
    {
        if(1u == pduDestSum)
        {
            pduRDestPduId = PDUR_ROUTINGPATH_CFG(id).PduRDestPduIdRef[0u];
            loModulePduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
            loModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
            loModule = PduR_BswModuleConfigData[loModuleIndex].PduRBswModuleRef;
            if(TRUE == PduRIsEnabled[pduRDestPduId])
            {
                switch(loModule)
                {
                    #if(STD_ON == PDUR_CANTP_SUPPORT)
                    case PDUR_CANTP:
                        result = CanTp_Transmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_CANTP_SUPPORT*/
                    #if(STD_ON == PDUR_J1939TP_SUPPORT)
                    case PDUR_J1939TP:
                        result = J1939Tp_Transmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_J1939TP_SUPPORT*/
                    #if(STD_ON == PDUR_LINTP_SUPPORT)
                    case PDUR_LINIF:
                        result = LinTp_Transmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_LINTP_SUPPORT*/
                    #if(STD_ON == PDUR_DOIP_SUPPORT)
                    case PDUR_DOIP:
                        result = DoIP_TpTransmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_DOIP_SUPPORT*/
                    #if(STD_ON == PDUR_SOAD_SUPPORT)
                    case PDUR_SOAD:
                        result = SoAd_TpTransmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_SOAD_SUPPORT*/
                    #if(STD_ON == PDUR_SOMEIPTP_SUPPORT)
                    case PDUR_SOMEIPTP:
                        result = SomeIpTp_Transmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_SOMEIPTP_SUPPORT*/
                    #if(STD_ON == PDUR_SECOC_SUPPORT)
                    case PDUR_SECOC:
                        result = SecOC_TpTransmit(loModulePduId,info);
                        break;
                    #endif/*STD_ON == PDUR_SECOC_SUPPORT*/
                    default:
                        /*do nothing*/
                        break;
                }
            }
        }
    }
    /*route one up module pdu to if modules (pdus)*/
    else
    {
        for(cnt = 0u;cnt < pduDestSum;cnt++)
        {
            pduRDestPduId = PDUR_ROUTINGPATH_CFG(id).PduRDestPduIdRef[cnt];
            loModulePduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
            loModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
            loModule = PduR_BswModuleConfigData[loModuleIndex].PduRBswModuleRef;
            if(TRUE == PduRIsEnabled[pduRDestPduId])
            {
                switch(loModule)
                {
                    #if(STD_ON == PDUR_CANIF_SUPPORT)
                    case PDUR_CANIF:
                        resultLo = CanIf_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_CANIF_SUPPORT*/
                    #if(STD_ON == PDUR_CANNM_SUPPORT)
                    case PDUR_CANNM:
                        resultLo = CanNm_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_CANNM_SUPPORT*/
                    #if(STD_ON == PDUR_OSEKNM_SUPPORT)
                    case PDUR_OSEKNM:
                        resultLo = OsekNm_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_OSEKNM_SUPPORT*/
                    #if(STD_ON == PDUR_UDPNM_SUPPORT)
                    case PDUR_UDPNM:
                        resultLo = UdpNm_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_UDPNM_SUPPORT*/
                    #if(STD_ON == PDUR_SECOC_SUPPORT)
                    case PDUR_SECOC:
                        resultLo = SecOC_IfTransmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_SECOC_SUPPORT*/
                    #if(STD_ON == PDUR_IPDUM_SUPPORT)
                    case PDUR_IPDUM:
                        resultLo = IpduM_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_IPDUM_SUPPORT*/
                    #if(STD_ON == PDUR_LINIF_SUPPORT)
                    case PDUR_LINIF:
                        resultLo = LinIf_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_LINIF_SUPPORT*/
                    #if(STD_ON == PDUR_FRIF_SUPPORT)
                    case PDUR_FRIF:
                        resultLo = FrIf_Transmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_FRIF_SUPPORT*/
                    #if(STD_ON == PDUR_SOAD_SUPPORT)
                    case PDUR_SOAD:
                        resultLo = SoAd_IfTransmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_SOAD_SUPPORT*/
                    #if(STD_ON == PDUR_DOIP_SUPPORT)
                    case PDUR_DOIP:
                        resultLo = DoIP_IfTransmit(loModulePduId,info);
                        if(E_OK == resultLo)
                        {
                            result = E_OK;
                        }
                        break;
                    #endif/*STD_ON == PDUR_DOIP_SUPPORT*/
                    default:
                        /*do nothing*/
                        break;
                }
            }
        }
        #if(PDUR_SRC_UP_MULTICAST_TX_IF_SUM > 0u)
        if((1u < pduDestSum) && (E_OK == result))
        {
            if(TRUE == PDUR_SRCEPDU_CFG(id).PduRSrcPduUpTxConf)
            {
                /*route 1:n,used to handle just the first dest TxConfirm will call up TxConfirm*/
                PduR_UpTxState[PDUR_SRCEPDU_CFG(id).UpTxconfirmStateIndex] = FALSE;
            }
        }
        #endif/*PDUR_SRC_UP_MULTICAST_TX_IF_SUM > 0u*/
    }
    return result;
}
#endif/*STD_ON == PDUR_TRANSMIT_SUPPORT*/

#if(STD_ON == PDUR_CANCEL_TRANSMIT)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeCancelTransmit(PduIdType id)
{
    Std_ReturnType result = E_NOT_OK;
    boolean resultOk = TRUE;
    PduIdType pduRDestPduId;
    PduIdType loModulePduId;
    uint8 loModuleIndex;
    uint8 loModule;
    uint8 pduDestSum;
    uint8 cnt;
    pduDestSum = PDUR_ROUTINGPATH_CFG(id).PduDestSum;
    /*Only all dest pdu cancel transmit OK,then return OK.*/
    for(cnt = 0u;(cnt < pduDestSum)&&(TRUE == resultOk);cnt++)
    {
        pduRDestPduId = PDUR_ROUTINGPATH_CFG(id).PduRDestPduIdRef[cnt];
        loModulePduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
        loModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
        loModule = PduR_BswModuleConfigData[loModuleIndex].PduRBswModuleRef;
        if((TRUE == PduRIsEnabled[pduRDestPduId])
                && (TRUE == PduR_BswModuleConfigData[loModuleIndex].PduRCancelTransmit))
        {
            switch(loModule)
            {
                #if(STD_ON == PDUR_CANIF_CANCEL_TRANSMIT)
                case PDUR_CANIF:
                    if(E_NOT_OK == CanIf_CancelTransmit(loModulePduId))
                    {
                        resultOk = FALSE;
                    }
                    break;
                #endif
                #if(STD_ON == PDUR_CANTP_CANCEL_TRANSMIT)
                case PDUR_CANTP:
                    if(E_NOT_OK == CanTp_CancelTransmit(loModulePduId))
                    {
                        resultOk = FALSE;
                    }
                    break;
                #endif
                #if(STD_ON == PDUR_J1939TP_CANCEL_TRANSMIT)
                case PDUR_J1939TP:
                    if(E_NOT_OK == J1939Tp_CancelTransmit(loModulePduId))
                    {
                        resultOk = FALSE;
                    }
                    break;
                #endif
                #if(STD_ON == PDUR_SECOC_CANCEL_TRANSMIT)
                case PDUR_SECOC:
                    if(TRUE == PDUR_ROUTINGPATH_CFG(id).TpRoute)
                    {
                        if(E_NOT_OK == SecOC_TpCancelTransmit(loModulePduId))
                        {
                            resultOk = FALSE;
                        }
                        break;
                    }
                    else
                    {
                        if(E_NOT_OK == SecOC_IfCancelTransmit(loModulePduId))
                        {
                            resultOk = FALSE;
                        }
                        break;
                    }
                    break;
                #endif
                #if(STD_ON == PDUR_LINIF_CANCEL_TRANSMIT)
                case PDUR_LINIF:
                    #if(STD_ON == PDUR_LINTP_SUPPORT)
                    if(TRUE == PDUR_ROUTINGPATH_CFG(id).TpRoute)
                    {
                        if(E_NOT_OK == LinTp_CancelTransmit(loModulePduId))
                        {
                            resultOk = FALSE;
                        }
                        break;
                    }
                    else
                    #endif
                    {
                        if(E_NOT_OK == LinIf_CancelTransmit(loModulePduId))
                        {
                            resultOk = FALSE;
                        }
                        break;
                    }
                #endif
                #if(STD_ON == PDUR_DOIP_CANCEL_TRANSMIT)
                case PDUR_DOIP:
                    if(TRUE == PDUR_ROUTINGPATH_CFG(id).TpRoute)
                    {
                        if(E_NOT_OK == DoIP_TpCancelTransmit(loModulePduId))
                        {
                            resultOk = FALSE;
                        }
                        break;
                    }
                    else
                    {
                        if(E_NOT_OK == DoIP_IfCancelTransmit(loModulePduId))
                        {
                            resultOk = FALSE;
                        }
                        break;
                    }
                #endif
                #if(STD_ON == PDUR_SOAD_CANCEL_TRANSMIT)
                case PDUR_SOAD:
                    if(E_NOT_OK == SoAd_TpCancelTransmit(loModulePduId))
                    {
                        resultOk = FALSE;
                    }
                    break;
                #endif
                #if(STD_ON == PDUR_FRIF_CANCEL_TRANSMIT)
                case PDUR_FRIF:
                    if(E_NOT_OK == FrIf_CancelTransmit(loModulePduId))
                    {
                        resultOk = FALSE;
                    }
                    break;
                #endif
                default:
                    /*do nothing*/
                    break;
            }
        }
        else
        {
            resultOk = FALSE;
        }
    }
    if(TRUE == resultOk)
    {
        result = E_OK;
    }
    return result;
}
#endif/*STD_ON == PDUR_CANCEL_TRANSMIT*/

#if(STD_ON == PDUR_CHANGE_PARAMETER)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeChangeParameter(PduIdType id,TPParameterType parameter,uint16 value)
{
    Std_ReturnType result = E_NOT_OK;
    PduIdType pduRSourcePduId;
    PduIdType loModulePduId;
    uint8 loModuleIndex;
    uint8 loModule;
    pduRSourcePduId = PDUR_DESTPDU_CFG(id).PduRSrcPduRef;
    loModulePduId = PDUR_SRCEPDU_CFG(pduRSourcePduId).PduRDestModulePduIndex;
    loModuleIndex = PDUR_SRCEPDU_CFG(pduRSourcePduId).BswModuleIndex;
    if((TRUE == PduRIsEnabled[id])
            && (TRUE == PduR_BswModuleConfigData[loModuleIndex].PduRChangeParameterApi))
    {
        loModule = PduR_BswModuleConfigData[loModuleIndex].PduRBswModuleRef;
        switch(loModule)
        {
            #if(STD_ON == PDUR_CANTP_CHANGE_PARAMETER)
            case PDUR_CANTP:
                result = CanTp_ChangeParameter(loModulePduId,parameter,value);
                break;
            #endif
            #if(STD_ON == PDUR_J1939TP_CHANGE_PARAMETER)
            case PDUR_J1939TP:
                result = J1939Tp_ChangeParameter(loModulePduId,parameter,value);
                break;
            #endif
            #if(STD_ON == PDUR_LINIF_CHANGE_PARAMETER)
            case PDUR_LINIF:
                result = LinTp_ChangeParameter(loModulePduId,parameter,value);
                break;
            #endif
            #if(STD_ON == PDUR_SOAD_CHANGE_PARAMETER)
            case PDUR_SOAD:
                result = SoAd_TpChangeParameter(loModulePduId,parameter,value);
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    return result;
}
#endif/*STD_ON == PDUR_CHANGE_PARAMETER*/

#if(STD_ON == PDUR_CANCEL_RECEIVE)
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_UpModeCancelReceive(PduIdType id)
{
    Std_ReturnType result = E_NOT_OK;
    PduIdType pduRSourcePduId;
    PduIdType loModulePduId;
    uint8 loModuleIndex;
    uint8 loModule;
    pduRSourcePduId = PDUR_DESTPDU_CFG(id).PduRSrcPduRef;
    loModulePduId = PDUR_SRCEPDU_CFG(pduRSourcePduId).PduRDestModulePduIndex;
    loModuleIndex = PDUR_SRCEPDU_CFG(pduRSourcePduId).BswModuleIndex;
    if((TRUE == PduRIsEnabled[id])
            && (TRUE == PduR_BswModuleConfigData[loModuleIndex].PduRCancelReceive))
    {
        loModule = PduR_BswModuleConfigData[loModuleIndex].PduRBswModuleRef;
        switch(loModule)
        {
            #if(STD_ON == PDUR_CANTP_CANCEL_RECEIVE)
            case PDUR_CANTP:
                result = CanTp_CancelReceive(loModulePduId);
                break;
            #endif
            #if(STD_ON == PDUR_J1939TP_CANCEL_RECEIVE)
            case PDUR_J1939TP:
                result = J1939Tp_CancelReceive(loModulePduId);
                break;
            #endif
            #if(STD_ON == PDUR_LINIF_CANCEL_RECEIVE)
            case PDUR_LINIF:
                result = LinTp_CancelReceive(loModulePduId);
                break;
            #endif
            #if(STD_ON == PDUR_DOIP_CANCEL_RECEIVE)
            case PDUR_DOIP:
                result = DoIP_TpCancelReceive(loModulePduId);
                break;
            #endif
            #if(STD_ON == PDUR_SOAD_CANCEL_RECEIVE)
            case PDUR_SOAD:
                result = SoAd_TpCancelReceive(loModulePduId);
                break;
            #endif
            #if(STD_ON == PDUR_SECOC_CANCEL_RECEIVE)
            case PDUR_SECOC:
                result = SecOC_TpCancelReceive(loModulePduId);
                break;
            #endif
            default:
                /*do nothing*/
                break;
        }
    }
    return result;
}
#endif/*STD_ON == PDUR_CANCEL_RECEIVE*/

#if(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_LoTpStartOfReception
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    uint8 pduDestSum;
    PduIdType pduRDestPduId;
    uint8 destModuleIndex;
    uint8 destModule;
    PduIdType upPduId;
    pduDestSum = PDUR_ROUTINGPATH_CFG(id).PduDestSum;
    /*route tp pdu 1:1*/
    if(1u == pduDestSum)
    {
        pduRDestPduId = PDUR_ROUTINGPATH_CFG(id).PduRDestPduIdRef[0u];
        if(TRUE == PduRIsEnabled[pduRDestPduId])
        {
            destModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
            destModule = PduR_BswModuleConfigData[destModuleIndex].PduRBswModuleRef;
            upPduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
            switch(destModule)
            {
                #if(STD_ON == PDUR_COM_SUPPORT)
                case PDUR_COM:
                    bufQeqReturn = Com_StartOfReception(upPduId,info,TpSduLength,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_LDCOM_SUPPORT)
                case PDUR_LDCOM:
                    bufQeqReturn = LdCom_StartOfReception(upPduId,info,TpSduLength,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DLT_SUPPORT)
                case PDUR_DLT:
                    bufQeqReturn = Dlt_StartOfReception(upPduId,info,TpSduLength,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_SECOC_SUPPORT)
                case PDUR_SECOC:
                    bufQeqReturn = SecOC_StartOfReception(upPduId,info,TpSduLength,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DCM_SUPPORT)
                case PDUR_DCM:
                    bufQeqReturn = Dcm_StartOfReception(upPduId,info,TpSduLength,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_J1939DCM_SUPPORT)
                case PDUR_J1939DCM:
                    bufQeqReturn = J1939Dcm_StartOfReception(upPduId,info,TpSduLength,bufferSizePtr);
                    break;
                #endif
                default:/*Tp layer:Gateway pdu*/
                    #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
                    bufQeqReturn = PduR_StartOfReceptionToOneTpHandle(id,info,TpSduLength,bufferSizePtr);
                    #endif
                    break;
            }
        }
    }
    return bufQeqReturn;
}
#endif/*STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT*/

#if(STD_ON == PDUR_TP_COPYRXDATA)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_LoTpCopyRxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    uint8 pduDestSum;
    PduIdType pduRDestPduId;
    uint8 destModuleIndex;
    uint8 destModule;
    PduIdType upPduId;
    #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
    PduIdType gateWayTpRunTimeId;
    #endif
    pduDestSum = PDUR_ROUTINGPATH_CFG(id).PduDestSum;
    /*one dest pdu*/
    if(1u == pduDestSum)
    {
        pduRDestPduId = PDUR_ROUTINGPATH_CFG(id).PduRDestPduIdRef[0u];
        if(TRUE == PduRIsEnabled[pduRDestPduId])
        {
            destModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
            destModule = PduR_BswModuleConfigData[destModuleIndex].PduRBswModuleRef;
            upPduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
            switch(destModule)
            {
                #if(STD_ON == PDUR_COM_SUPPORT)
                case PDUR_COM:
                    bufQeqReturn = Com_CopyRxData(upPduId,info,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_LDCOM_SUPPORT)
                case PDUR_LDCOM:
                    bufQeqReturn = LdCom_CopyRxData(upPduId,info,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DLT_SUPPORT)
                case PDUR_DLT:
                    bufQeqReturn = Dlt_CopyRxData(upPduId,info,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_SECOC_SUPPORT)
                case PDUR_SECOC:
                    bufQeqReturn = SecOC_CopyRxData(upPduId,info,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DCM_SUPPORT)
                case PDUR_DCM:
                    bufQeqReturn = Dcm_CopyRxData(upPduId,info,bufferSizePtr);
                    break;
                #endif
                #if(STD_ON == PDUR_J1939DCM_SUPPORT)
                case PDUR_J1939DCM:
                    bufQeqReturn = J1939Dcm_CopyRxData(upPduId,info,bufferSizePtr);
                    break;
                #endif
                default:/*Tp layer:Gateway pdu*/
                    #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
                    gateWayTpRunTimeId = PDUR_DESTPDU_CFG(pduRDestPduId).GateWayTpRunTimeIndex;
                    if((PDUR_UNUSED_UINT16 != gateWayTpRunTimeId)
                            && (FALSE == PduR_GateWayDestTpRTSate[gateWayTpRunTimeId].TpTxConfirmation))
                    {
                        bufQeqReturn = PduR_CopyRxDataToOneTpHandle(
                                #if (0u < PDUR_TP_BUFFER_SUM)
                                id,
                                #endif
                                pduRDestPduId,info,bufferSizePtr);
                    }
                    #endif
                    break;
            }
        }
    }
    return bufQeqReturn;
}
#endif/*STD_ON == PDUR_TP_COPYRXDATA*/

#if(STD_ON == PDUR_TP_RXINDICATION)
static FUNC(void, PDUR_CODE)
PduR_LoTpRxIndication
(
    PduIdType id,
    Std_ReturnType result
)
{
    uint8 pduDestSum;
    PduIdType pduRDestPduId;
    uint8 destModuleIndex;
    uint8 destModule;
    PduIdType destModulePduId;
    pduDestSum = PDUR_ROUTINGPATH_CFG(id).PduDestSum;
    /*rx tp pdu to only one dest pdu*/
    if(1u == pduDestSum)
    {
        pduRDestPduId = PDUR_ROUTINGPATH_CFG(id).PduRDestPduIdRef[0u];
        destModuleIndex = PDUR_DESTPDU_CFG(pduRDestPduId).BswModuleIndex;
        destModule = PduR_BswModuleConfigData[destModuleIndex].PduRBswModuleRef;
        destModulePduId = PDUR_DESTPDU_CFG(pduRDestPduId).PduRDestModulePduIndex;
        switch(destModule)
        {
            #if(STD_ON == PDUR_COM_SUPPORT)
            case PDUR_COM:
                Com_TpRxIndication(destModulePduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_LDCOM_SUPPORT)
            case PDUR_LDCOM:
                LdCom_TpRxIndication(destModulePduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_DLT_SUPPORT)
            case PDUR_DLT:
                Dlt_TpRxIndication(destModulePduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_SECOC_SUPPORT)
            case PDUR_SECOC:
                SecOC_TpRxIndication(destModulePduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_DCM_SUPPORT)
            case PDUR_DCM:
                Dcm_TpRxIndication(destModulePduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_J1939DCM_SUPPORT)
            case PDUR_J1939DCM:
                J1939Dcm_TpRxIndication(destModulePduId,result);
                break;
            #endif
            default:/*Tp layer:Gateway pdu*/
                #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
                if((E_OK == result) && (TRUE == PduRIsEnabled[pduRDestPduId]))
                {
                    PduR_RxIndicationToOneTpHandle(id,pduRDestPduId);
                }
                else
                {
                    PduR_ClearBufferAndStateOfGateWayTpPdu(id);
                }
                #endif
                break;
        }
    }
    return;
}
#endif/*STD_ON == PDUR_TP_RXINDICATION*/

#if(STD_ON == PDUR_TP_COPYTXDATA)
static FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_LoTpCopyTxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr
)
{
    BufReq_ReturnType bufQeqReturn = BUFREQ_E_NOT_OK;
    PduIdType srcPdu;
    uint8 srcModuleIndex;
    uint8 srcModule;
    PduIdType upPduId;
    srcPdu = PDUR_DESTPDU_CFG(id).PduRSrcPduRef;
    srcModuleIndex = PDUR_SRCEPDU_CFG(srcPdu).BswModuleIndex;
    srcModule = PduR_BswModuleConfigData[srcModuleIndex].PduRBswModuleRef;
    upPduId = PDUR_SRCEPDU_CFG(srcPdu).PduRDestModulePduIndex;
    /*one dest pdu route*/
    if(1u == PDUR_ROUTINGPATH_CFG(srcPdu).PduDestSum)
    {
        if(TRUE == PduRIsEnabled[id])
        {
            switch(srcModule)
            {
                #if(STD_ON == PDUR_COM_SUPPORT)
                case PDUR_COM:
                    bufQeqReturn = Com_CopyTxData(upPduId,info,retry,availableDataPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_LDCOM_SUPPORT)
                case PDUR_LDCOM:
                    bufQeqReturn = LdCom_CopyTxData(upPduId,info,retry,availableDataPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DLT_SUPPORT)
                case PDUR_DLT:
                    bufQeqReturn = Dlt_CopyTxData(upPduId,info,retry,availableDataPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_SECOC_SUPPORT)
                case PDUR_SECOC:
                    bufQeqReturn = SecOC_CopyTxData(upPduId,info,retry,availableDataPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_DCM_SUPPORT)
                case PDUR_DCM:
                    bufQeqReturn = Dcm_CopyTxData(upPduId,info,retry,availableDataPtr);
                    break;
                #endif
                #if(STD_ON == PDUR_J1939DCM_SUPPORT)
                case PDUR_J1939DCM:
                    bufQeqReturn = J1939Dcm_CopyTxData(upPduId,info,retry,availableDataPtr);
                    break;
                #endif
                /*Tp layer:Gateway pdu*/
                default:
                    #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
                    bufQeqReturn = PduR_OneDestCopyTxDataFromTpHandle(id,info,retry,availableDataPtr);
                    #endif
                break;
            }
        }
    }
    return bufQeqReturn;
}
#endif/*STD_ON == PDUR_TP_COPYTXDATA*/

#if(STD_ON == PDUR_TP_TXCONFIRMATION)
static FUNC(void, PDUR_CODE)
PduR_LoTpTxConfirmation(PduIdType id,Std_ReturnType result)
{
    PduIdType srcPdu;
    uint8 srcModuleIndex;
    uint8 srcModule;
    PduIdType upPduId;
    srcPdu = PDUR_DESTPDU_CFG(id).PduRSrcPduRef;
    srcModuleIndex = PDUR_SRCEPDU_CFG(srcPdu).BswModuleIndex;
    srcModule = PduR_BswModuleConfigData[srcModuleIndex].PduRBswModuleRef;
    /*one dest pdu route*/
    if(1u == PDUR_ROUTINGPATH_CFG(srcPdu).PduDestSum)
    {
        upPduId = PDUR_SRCEPDU_CFG(srcPdu).PduRDestModulePduIndex;
        switch(srcModule)
        {
            #if(STD_ON == PDUR_COM_SUPPORT)
            case PDUR_COM:
                Com_TpTxConfirmation(upPduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_LDCOM_SUPPORT)
            case PDUR_LDCOM:
                LdCom_TpTxConfirmation(upPduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_DLT_SUPPORT)
            case PDUR_DLT:
                Dlt_TpTxConfirmation(upPduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_SECOC_SUPPORT)
            case PDUR_SECOC:
                SecOC_TpTxConfirmation(upPduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_DCM_SUPPORT)
            case PDUR_DCM:
                Dcm_TpTxConfirmation(upPduId,result);
                break;
            #endif
            #if(STD_ON == PDUR_J1939DCM_SUPPORT)
            case PDUR_J1939DCM:
                J1939Dcm_TpTxConfirmation(upPduId,result);
                break;
            #endif
            /*Tp layer:Gateway pdu*/
            default:
                #if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
                PduR_ClearBufferAndStateOfGateWayTpPdu(srcPdu);
                #endif
                break;
        }
    }
    return;
}
#endif/*STD_ON == PDUR_TP_TXCONFIRMATION*/

#if((STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_CopyRxPduToTxBuffer(
        #if(STD_ON == PDUR_META_DATA_SUPPORT)
        uint8 MetaDataLength,
        #endif
        P2VAR(PduR_TxBufferType, AUTOMATIC, PDUR_APPL_DATA) TxBufferPtr,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA)PduInfo,
        PduLengthType PduLength)
{
    TxBufferPtr->used = TRUE;
    TxBufferPtr->SduLength = PduLength;
    PduR_Memcpy(
        TxBufferPtr->TxBufferData,
        PduInfo->SduDataPtr,
        PduLength);
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    if(0u < MetaDataLength)
    {
        PduR_Memcpy(
            TxBufferPtr->MetaData,
            PduInfo->MetaDataPtr,
            (PduLengthType)MetaDataLength);
    }
    #endif
    return;
}
#endif/*(STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u)*/

#if((STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_EnQueueBuffer(
    PduIdType PduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA)PduInfo
)
{
    uint8 bufferDepth;
    uint16 bufferId;
    uint8 cnt;
    boolean findIdleBuffer = FALSE;
    PduLengthType pduMaxLength;
    PduR_TxBufferType* txBufferPtr;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 metaDataLength;
    metaDataLength = PDUR_DESTPDU_CFG(PduId).MetaDataLength;
    #endif
    bufferId = PDUR_DESTPDU_CFG(PduId).PduRDestTxBufferRef;
    pduMaxLength = PduR_TxBuffer[bufferId].PduRPduMaxLength;
    bufferDepth = PduR_TxBuffer[bufferId].PduRTxBufferDepth;
    if(pduMaxLength > PduInfo->SduLength)
    {
        pduMaxLength = PduInfo->SduLength;
    }
    if(1u == bufferDepth)
    {
        txBufferPtr = &(PduR_TxBuffer[bufferId].PduRTxBufferRef[0u]);
        PduR_CopyRxPduToTxBuffer(
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                metaDataLength,
                #endif
                txBufferPtr,PduInfo,pduMaxLength);
    }
    if(bufferDepth > 1u)
    {
        for(cnt = 0u;(cnt < bufferDepth)&&(FALSE == findIdleBuffer);cnt++)
        {
            if(FALSE == PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt].used)
            {
                findIdleBuffer = TRUE;
                txBufferPtr = &(PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt]);
                PduR_CopyRxPduToTxBuffer(
                        #if(STD_ON == PDUR_META_DATA_SUPPORT)
                        metaDataLength,
                        #endif
                        txBufferPtr,PduInfo,pduMaxLength);
            }
        }
        /*all buffer is used, the FIFO is flushed */
        if(FALSE == findIdleBuffer)
        {
            txBufferPtr = &(PduR_TxBuffer[bufferId].PduRTxBufferRef[0u]);
            PduR_CopyRxPduToTxBuffer(
                    #if(STD_ON == PDUR_META_DATA_SUPPORT)
                    metaDataLength,
                    #endif
                    txBufferPtr,PduInfo,pduMaxLength);
            for(cnt = 1u;cnt < bufferDepth;cnt++)
            {
                PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt].used = FALSE;
            }
            (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
    }
    return;
}
#endif/*(STD_ON == PDUR_RX_INDICATION) && (PDUR_TX_BUFFER_SUM > 0u)*/

#if(STD_ON == PDUR_RX_INDICATION)
static FUNC(void, PDUR_CODE)
PduR_GateWayIfPdu(
    uint8 DestModule,
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) InfoPtr
)
{
    PduR_DestPduDataProvisionType provisionType;
    #if(PDUR_TX_BUFFER_SUM > 0u)
    PduInfoType pduInfo;
    #endif
    uint16 bufferId;
    PduIdType destModulePduId;
    PduR_LoIfTransmit_FuncPtrType pduR_LoIfTransmitApi = NULL_PTR;
    switch(DestModule)
    {
        #if(STD_ON == PDUR_CANIF_SUPPORT)
        case PDUR_CANIF:
            pduR_LoIfTransmitApi = &CanIf_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_LINIF_SUPPORT)
        case PDUR_LINIF:
            pduR_LoIfTransmitApi = &LinIf_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_DOIP_SUPPORT)
        case PDUR_DOIP:
            pduR_LoIfTransmitApi = &DoIP_IfTransmit;
            break;
        #endif
        #if(STD_ON == PDUR_SOAD_SUPPORT)
        case PDUR_SOAD:
            pduR_LoIfTransmitApi = &SoAd_IfTransmit;
            break;
        #endif  
        #if(STD_ON == PDUR_FRIF_SUPPORT)
        case PDUR_FRIF:
            pduR_LoIfTransmitApi = &FrIf_Transmit;
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
    if(NULL_PTR != pduR_LoIfTransmitApi)
    {
        provisionType = PDUR_DESTPDU_CFG(DestPduId).PduRDestPduDataProvision;
        destModulePduId = PDUR_DESTPDU_CFG(DestPduId).PduRDestModulePduIndex;
        if(PDUR_TRIGGERTRANSMIT == provisionType)
        {
            #if(PDUR_TX_BUFFER_SUM > 0u)
            PduR_EnQueueBuffer(DestPduId,InfoPtr);
            pduInfo.SduDataPtr = NULL_PTR;
            pduInfo.SduLength = 0u;
            pduInfo.MetaDataPtr = InfoPtr->MetaDataPtr;
            (void)pduR_LoIfTransmitApi(destModulePduId,&pduInfo);
            #endif/*PDUR_TX_BUFFER_SUM > 0u*/
        }
        else
        {
            bufferId = PDUR_DESTPDU_CFG(DestPduId).PduRDestTxBufferRef;
            /*don't configuration buffer*/
            if(PDUR_UNUSED_UINT16 == bufferId)
            {
                (void)pduR_LoIfTransmitApi(destModulePduId,InfoPtr);
            }
            #if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
            else
            {
                PduR_GateWayDirectBufferHandle(pduR_LoIfTransmitApi,DestPduId,InfoPtr);
            }
            #endif/*(PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u)*/
        }
    }
    return;
}

/*Called by PduR_GateWayIfPdu.
 *If pdu gateway(config buffer and direct) handle*/
#if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_GateWayDirectBufferHandle(
        PduR_LoIfTransmit_FuncPtrType pduR_LoIfTransmitApi,
        PduIdType DestPduId,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) InfoPtr)
{

    PduInfoType pduInfo;
    PduIdType gateWayIfPduDirectStateId;
    uint16 bufferId;
    PduIdType destModulePduId;
    destModulePduId = PDUR_DESTPDU_CFG(DestPduId).PduRDestModulePduIndex;
    bufferId = PDUR_DESTPDU_CFG(DestPduId).PduRDestTxBufferRef;
    gateWayIfPduDirectStateId = PDUR_DESTPDU_CFG(DestPduId).PduRGatewayDirectTxStateIndex;
    /*only 1 buffer,"Last is best" behavior*/
    if(1u == PduR_TxBuffer[bufferId].PduRTxBufferDepth)
    {
        if(PDUR_IDLE == PduR_DestPduState[gateWayIfPduDirectStateId])
        {
            /*transmit the old data in buffer,store the new data to buffer*/
            if(TRUE == PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used)
            {
                pduInfo.SduDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].TxBufferData;
                pduInfo.SduLength = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].SduLength;
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                if(0u < PDUR_DESTPDU_CFG(DestPduId).MetaDataLength)
                {
                    pduInfo.MetaDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].MetaData;
                }
                else
                #endif
                {
                    pduInfo.MetaDataPtr = NULL_PTR;
                }
                if(E_OK == pduR_LoIfTransmitApi(destModulePduId,&pduInfo))
                {
                    PduR_DestPduState[gateWayIfPduDirectStateId] = PDUR_BUSY;
                }
                PduR_EnQueueBuffer(DestPduId,InfoPtr);
            }
            /*transmit the new data,don't store the data even the transmit not ok*/
            else
            {
                if(E_OK == pduR_LoIfTransmitApi(destModulePduId,InfoPtr))
                {
                    PduR_DestPduState[gateWayIfPduDirectStateId] = PDUR_BUSY;
                }
            }
        }
        /*gateway if pdu(direct),the pdu is busy.Store the data to buffer*/
        else
        {
            PduR_EnQueueBuffer(DestPduId,InfoPtr);
        }
    }
    /*FIFO*/
    else
    {
        if(PDUR_IDLE == PduR_DestPduState[gateWayIfPduDirectStateId])
        {
            /*transmit the oldest data in buffer,store the new data to buffer*/
            if(TRUE == PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used)
            {
                pduInfo.SduDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].TxBufferData;
                pduInfo.SduLength = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].SduLength;
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                if(0u < PDUR_DESTPDU_CFG(DestPduId).MetaDataLength)
                {
                    pduInfo.MetaDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].MetaData;
                }
                #endif
                if(E_OK == pduR_LoIfTransmitApi(destModulePduId,&pduInfo))
                {
                    PduR_DestPduState[gateWayIfPduDirectStateId] = PDUR_BUSY;
                }
                else
                {
                    (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
                }
                PduR_DeQueueBuffer(DestPduId);
                PduR_EnQueueBuffer(DestPduId,InfoPtr);
            }
            /*transmit the new data,don't store the data even the transmit not ok*/
            else
            {
                if(E_OK == pduR_LoIfTransmitApi(destModulePduId,InfoPtr))
                {
                    PduR_DestPduState[gateWayIfPduDirectStateId] = PDUR_BUSY;
                }
                else
                {
                    (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
                }
            }
        }
        /*gateway if pdu(direct),the pdu is busy.Store the data to buffer*/
        else
        {
            PduR_EnQueueBuffer(DestPduId,InfoPtr);
        }
    }
    return;
}
#endif/*(PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u)*/
#endif/*STD_ON == PDUR_RX_INDICATION*/

#if((PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(void, PDUR_CODE)
PduR_IfTxConfirmationGatewayHandle(
    PduIdType TxPduId,
    PduR_LoIfTransmit_FuncPtrType PduR_LoIfTransmitApi
)
{
    uint16 bufferId;
    PduInfoType pduInfo;
    PduIdType destModulePduId;
    Std_ReturnType result;
    bufferId = PDUR_DESTPDU_CFG(TxPduId).PduRDestTxBufferRef;
    /*the pdu config buffer and it's PDUR_DIRECT mode*/
    if((PDUR_UNUSED_UINT16 != bufferId)
            && (PDUR_DIRECT == PDUR_DESTPDU_CFG(TxPduId).PduRDestPduDataProvision))
    {
        PduR_DestPduState[PDUR_DESTPDU_CFG(TxPduId).PduRGatewayDirectTxStateIndex] = PDUR_IDLE;
        if(TRUE == PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used)
        {
            pduInfo.SduDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].TxBufferData;
            pduInfo.SduLength = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].SduLength;
            #if(STD_ON == PDUR_META_DATA_SUPPORT)
            if(0u < PDUR_DESTPDU_CFG(TxPduId).MetaDataLength)
            {
                pduInfo.MetaDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].MetaData;
            }
            else
            #endif
            {
                pduInfo.MetaDataPtr = NULL_PTR;
            }
            destModulePduId = PDUR_DESTPDU_CFG(TxPduId).PduRDestModulePduIndex;
            result = PduR_LoIfTransmitApi(destModulePduId,&pduInfo);
            if(E_OK == result)
            {
                PduR_DestPduState[PDUR_DESTPDU_CFG(TxPduId).PduRGatewayDirectTxStateIndex] = PDUR_BUSY;
            }
            else
            {
                /*transmit not ok(FIFO),report det error*/
                if(1u < PduR_TxBuffer[bufferId].PduRTxBufferDepth)
                {
                    (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_IFTXCONFIRMATION_ID, PDUR_E_PDU_INSTANCES_LOST);
                }
            }
            PduR_DeQueueBuffer(TxPduId);
        }
    }
    return;
}
#endif/*(PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u) && (PDUR_TX_BUFFER_SUM > 0u)*/

#if((STD_ON == PDUR_TRIGGER_TRANSMIT) || (PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u))
#if(PDUR_TX_BUFFER_SUM > 0u)
/*clear the buffer(the buffer data have transmit,Whether or not it succeeds)*/
static FUNC(void, PDUR_CODE)
PduR_DeQueueBuffer(PduIdType PduId)
{
    uint8 bufferDepth;
    uint16 bufferId;
    uint8 cnt;
    bufferId = PDUR_DESTPDU_CFG(PduId).PduRDestTxBufferRef;
    bufferDepth = PduR_TxBuffer[bufferId].PduRTxBufferDepth;
    if(1u == bufferDepth)
    {
        PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used = FALSE;
    }
    if(bufferDepth > 1u)
    {
        PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used = FALSE;
        for(cnt = 0u;cnt < (bufferDepth - 1u);cnt++)
        {
            if(PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt+1u].used == TRUE)
            {
                PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt].SduLength =
                        PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt+1u].SduLength;
                #if(STD_ON == PDUR_META_DATA_SUPPORT)
                PduR_Memcpy(
                     PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt].MetaData,
                     PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt+1u].MetaData,
                     8u);
                #endif
                PduR_Memcpy(
                     PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt].TxBufferData,
                     PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt+1u].TxBufferData,
                     PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt+1u].SduLength);
                PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt].used = TRUE;
                PduR_TxBuffer[bufferId].PduRTxBufferRef[cnt+1u].used = FALSE;
            }
        }
        PduR_TxBuffer[bufferId].PduRTxBufferRef[bufferDepth-1u].used = FALSE;
    }
   return;
}

/*Called by PduR_IfTxConfirmation,PduR_IfTriggerTransmitGwHandle.
 *Get the lower module's IfTransmit Api*/
static FUNC(void, PDUR_CODE)
PduR_GetLoIfTransmitApi(
        PduIdType TxPduId,
        PduR_LoIfTransmit_FuncPtrType* PduR_LoIfTransmitApi)
{
    uint8 moduleId,module;
    moduleId = PDUR_DESTPDU_CFG(TxPduId).BswModuleIndex;
    module = PduR_BswModuleConfigData[moduleId].PduRBswModuleRef;
    switch(module)
    {
        #if(STD_ON == PDUR_CANIF_SUPPORT)
        case PDUR_CANIF:
            *PduR_LoIfTransmitApi = CanIf_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_CANNM_SUPPORT)
        case PDUR_CANNM:
            *PduR_LoIfTransmitApi = CanNm_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_OSEKNM_SUPPORT)
        case PDUR_OSEKNM:
            *PduR_LoIfTransmitApi = OsekNm_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_UDPNM_SUPPORT)
        case PDUR_UDPNM:
            *PduR_LoIfTransmitApi = UdpNm_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_SECOC_SUPPORT)
        case PDUR_SECOC:
            *PduR_LoIfTransmitApi = SecOC_IfTransmit;
            break;
        #endif
        #if(STD_ON == PDUR_IPDUM_SUPPORT)
        case PDUR_IPDUM:
            *PduR_LoIfTransmitApi = IpduM_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_LINIF_SUPPORT)
        case PDUR_LINIF:
            *PduR_LoIfTransmitApi = LinIf_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_FRIF_SUPPORT)
        case PDUR_FRIF:
            *PduR_LoIfTransmitApi = FrIf_Transmit;
            break;
        #endif
        #if(STD_ON == PDUR_SOAD_SUPPORT)
        case PDUR_SOAD:
            *PduR_LoIfTransmitApi = SoAd_IfTransmit;
            break;
        #endif
        #if(STD_ON == PDUR_DOIP_SUPPORT)
        case PDUR_DOIP:
            *PduR_LoIfTransmitApi = DoIP_IfTransmit;
            break;
        #endif
        default:
            /*do nothing*/
            break;
    }
}
#endif/*PDUR_TX_BUFFER_SUM > 0u*/
#endif/*(STD_ON == PDUR_TRIGGER_TRANSMIT) || (PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u)*/

/*Called by PduR_IfTriggerTransmit.
 *IfTriggerTransmit GW handle*/
#if((STD_ON == PDUR_TRIGGER_TRANSMIT) && (PDUR_TX_BUFFER_SUM > 0u))
static FUNC(Std_ReturnType, PDUR_CODE)
PduR_IfTriggerTransmitGwHandle(
        PduIdType TxPduId,
        P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
    uint16 bufferId;
    PduIdType destModulePduId;
    PduInfoType pduInfo;
    PduR_LoIfTransmit_FuncPtrType PduR_LoIfTransmitApi = NULL_PTR;
    bufferId = PDUR_DESTPDU_CFG(TxPduId).PduRDestTxBufferRef;
    if((PduInfoPtr->SduLength >= PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].SduLength)
        && (TRUE == PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used))
    {
        PduInfoPtr->SduLength = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].SduLength;
        PduR_Memcpy(
                PduInfoPtr->SduDataPtr,
                PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].TxBufferData,
                PduInfoPtr->SduLength);
        #if(STD_ON == PDUR_META_DATA_SUPPORT)
        if(0u < PDUR_DESTPDU_CFG(TxPduId).MetaDataLength)
        {
            PduR_Memcpy(
                    PduInfoPtr->MetaDataPtr,
                    PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].MetaData,
                    (PduLengthType)PDUR_DESTPDU_CFG(TxPduId).MetaDataLength);
        }
        else
        #endif
        {
            PduInfoPtr->MetaDataPtr = NULL_PTR;
        }
        if(1u < PduR_TxBuffer[bufferId].PduRTxBufferDepth)
        {
            PduR_DeQueueBuffer(TxPduId);
            if(TRUE == PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].used)
            {
                PduR_GetLoIfTransmitApi(TxPduId,&PduR_LoIfTransmitApi);
                if(NULL_PTR != PduR_LoIfTransmitApi)
                {
                    destModulePduId = PDUR_DESTPDU_CFG(TxPduId).PduRDestModulePduIndex;
                    pduInfo.SduDataPtr = NULL_PTR;
                    pduInfo.SduLength = 0u;
                    #if(STD_ON == PDUR_META_DATA_SUPPORT)
                    if(0u < PDUR_DESTPDU_CFG(TxPduId).MetaDataLength)
                    {
                        pduInfo.MetaDataPtr = PduR_TxBuffer[bufferId].PduRTxBufferRef[0u].MetaData;
                    }
                    else
                    #endif
                    {
                        pduInfo.MetaDataPtr = NULL_PTR;
                    }
                    (void)PduR_LoIfTransmitApi(destModulePduId,&pduInfo);
                }
            }
        }
        result = E_OK;
    }
    return result;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* STD_OFF == PDUR_ZERO_COST_OPERATION */
/*=======[E N D   O F   F I L E]==============================================*/


