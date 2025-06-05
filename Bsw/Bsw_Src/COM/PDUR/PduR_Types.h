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
**  FILENAME    : PduR_Types.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type definitions of PDUR                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_TYPES_H
#define  PDUR_TYPES_H
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_TYPES_H_AR_MAJOR_VERSION  4u
#define PDUR_TYPES_H_AR_MINOR_VERSION  2u
#define PDUR_TYPES_H_AR_PATCH_VERSION  2u
#define PDUR_TYPES_H_SW_MAJOR_VERSION  2u
#define PDUR_TYPES_H_SW_MINOR_VERSION  0u
#define PDUR_TYPES_H_SW_PATCH_VERSION  0u
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Cfg.h"
#if(STD_OFF == PDUR_ZERO_COST_OPERATION)
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef enum
{
    PDUR_DIRECT = 0u,
    PDUR_TRIGGERTRANSMIT
}PduR_DestPduDataProvisionType;

typedef uint16 PduR_PBConfigIdType;
typedef uint16 PduR_RoutingPathGroupIdType;

typedef enum
{
    PDUR_UNINIT = 0u,
    PDUR_ONLINE
} PduR_StateType;

typedef Std_ReturnType (*PduR_LoIfTransmit_FuncPtrType)(
    PduIdType SoAdSrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
);

typedef void (*PduR_UpIfTxConfirmation_FuncPtrType)(PduIdType TxPduId);
/*****************************************************************************************************************/
typedef struct
{
    PduLengthType DefaultValueStart;
    PduLengthType DefaultValueLength;
}PduRDefaultValueType;

typedef struct
{
    PduIdType PduRSourcePduHandleId;
    boolean PduRSrcPduUpTxConf;
    PduIdType UpTxconfirmStateIndex;
    PduIdType PduRDestModulePduIndex;
    uint8 BswModuleIndex;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaDataLength;
    #endif
}PduRSrcPduType;

typedef struct
{
    PduIdType PduRDestPduHandleId;
    PduR_DestPduDataProvisionType PduRDestPduDataProvision;
    PduIdType PduRGatewayDirectTxStateIndex;
    uint16 PduRTpThreshold;
    PduIdType GateWayTpRunTimeIndex;
    boolean PduRTransmissionConfirmation;
    PduIdType PduRSrcPduRef;
    PduIdType PduRDestModulePduIndex;
    uint8 BswModuleIndex;
    uint16 PduRDestTxBufferRef;
    P2CONST(PduRDefaultValueType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDefaultValueRef;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaDataLength;
    #endif
}PduRDestPduType;

typedef struct
{
    PduIdType PduRSrcPduId;
    uint8 PduDestSum;
    P2CONST(PduIdType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDestPduIdRef;
    boolean TpRoute;
    boolean GatewayOnTheFly;
    uint16 PduRTpMaxThreshold;
}PduRRoutingPathType;

typedef struct
{
    P2CONST(PduRRoutingPathType, AUTOMATIC, PDUR_CONST_PBCFG) PduRRoutingPathRef;
}PduRRoutingTableType;
/*****************************************************************************************************************/
typedef struct
{
    boolean PduRIsEnabledAtInit;
    PduR_RoutingPathGroupIdType PduRRoutingPathGroupId;
    PduIdType PduRDestPduRefNumber;
    P2CONST(PduIdType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDestPduRef;
}PduRRoutingPathGroupType;
/*****************************************************************************************************************/
typedef struct
{
    PduR_PBConfigIdType PduRConfigId;
    P2CONST(PduRRoutingPathGroupType, AUTOMATIC, PDUR_CONST_PBCFG) PduRRoutingPathGroupRef;
    P2CONST(PduRRoutingTableType, AUTOMATIC, PDUR_CONST_PBCFG) PduRRoutingTableRef;
    P2CONST(PduRSrcPduType, AUTOMATIC, PDUR_CONST_PBCFG) PduRSrcPduRef;
    P2CONST(PduRDestPduType, AUTOMATIC, PDUR_CONST_PBCFG) PduRDestPduRef;
}PduR_PBConfigType;

typedef struct
{
    uint8   PduRBswModuleRef;
    boolean PduRCancelReceive;
    boolean PduRCancelTransmit;
    boolean PduRChangeParameterApi;
    boolean PduRCommunicationInterface;/*unused*/
    boolean PduRTransportProtocol;/*unused*/
    boolean PduRLowerModule;/*unused*/
    boolean PduRUpperModule;/*unused*/
    boolean PduRRetransmission;/*unused*/
    boolean PduRTriggertransmit;
    boolean PduRTxConfirmation;/*unused*/
    boolean PduRUseTag;/*unused*/
}PduRBswModuleType;

typedef struct
{
    PduLengthType RxBufferOffset;
    PduLengthType TpBufferLength;
    PduLengthType SduLength;
    P2VAR(uint8, AUTOMATIC, PDUR_CONST_PBCFG) TpBufferData;
    PduIdType PduHandleId;
    boolean used;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaData[8u];
    #endif
    uint8 NextTpBufferId;
}PduR_TpBufferTableType;

typedef struct
{
    PduLengthType SduLength;
    P2VAR(uint8, AUTOMATIC, PDUR_CONST_PBCFG) TxBufferData;
    boolean used;
    #if(STD_ON == PDUR_META_DATA_SUPPORT)
    uint8 MetaData[8u];
    #endif
}PduR_TxBufferType;

typedef struct
{
    PduLengthType PduRPduMaxLength;
    uint8 PduRTxBufferDepth;
    P2VAR(PduR_TxBufferType, AUTOMATIC, PDUR_CONST_PBCFG) PduRTxBufferRef;
}PduR_TxBufferTableType;
#endif/*STD_OFF == PDUR_ZERO_COST_OPERATION*/

#endif  /* end of PDUR_TYPES_H */


