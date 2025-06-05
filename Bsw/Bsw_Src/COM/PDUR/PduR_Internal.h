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
**  FILENAME    : PduR_Internal.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : PDUR internal header for internal API declarations          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_INTERNAL_H
#define  PDUR_INTERNAL_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
#if(STD_OFF == PDUR_ZERO_COST_OPERATION)
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef struct
{
    boolean NeedGateWayOnTheFly;
    uint8 ActiveTpBufferId;
    boolean TpTxConfirmation;
    PduLengthType TxBufferOffset;
}PduR_GateWayDestTpRunTimeType;

#if(PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM > 0u)
typedef enum
{
    PDUR_BUSY = 0u,
    PDUR_IDLE
} PduR_DestStateType;
#endif
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#if(PDUR_TP_BUFFER_SUM > 0u)
#define PDUR_START_SEC_VAR_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
extern VAR(PduR_TpBufferTableType,PDUR_VAR_POWER_ON_INIT)
PduR_TpBuffer[PDUR_TP_BUFFER_SUM];
#define PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
#endif

#if(PDUR_TX_BUFFER_SUM > 0u)
#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
extern CONST(PduR_TxBufferTableType,PDUR_CONST)
PduR_TxBuffer[PDUR_TX_BUFFER_SUM];
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
#endif

#if(PDUR_DEFAULT_VALUE_LENGTH > 0u)
#define PDUR_START_SEC_PBCONFIG_DATA_8
#include "PduR_MemMap.h"
extern CONST(uint8,PDUR_CONST)
PduR_Default_value[PDUR_DEFAULT_VALUE_LENGTH];
#define PDUR_STOP_SEC_PBCONFIG_DATA_8
#include "PduR_MemMap.h"
#endif

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
extern CONST(PduRBswModuleType,PDUR_CONST)
PduR_BswModuleConfigData[PDUR_BSW_MODULE_SUM];
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
/***********************
** define in PduR.c   **
************************/
/*PduR PB Configuration Run Time point parameter*/
#define PDUR_START_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"
extern P2CONST(PduR_PBConfigType, PDUR_VAR, PDUR_CONST) PduR_ConfigStd;
#define PDUR_STOP_SEC_VAR_INIT_PTR
#include "PduR_MemMap.h"

/*gateway Tp routing path:all dest pdu(SF/MF,route to Tp or Up)*/
#if(PDUR_DEST_GATEWAY_TP_PDU_SUM > 0u)
#define PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
extern VAR(PduR_GateWayDestTpRunTimeType,PDUR_VAR)
PduR_GateWayDestTpRTSate[PDUR_DEST_GATEWAY_TP_PDU_SUM];
#define PDUR_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
#endif
/*******************************************************************************
**                      External function declarations                        **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if (0u < PDUR_DEST_GATEWAY_TP_PDU_SUM)
/*Rx Tp Pdu gateway to only one Tp Module Pdu handle,when start of reception*/
#if(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT)
extern FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_StartOfReceptionToOneTpHandle
(
    PduIdType SrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    PduLengthType Length,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr
);
#endif/*STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT*/

/*Rx Tp Pdu gateway to only one Tp Module Pdu handle,when copy Rx Data*/
#if(STD_ON == PDUR_TP_COPYRXDATA)
extern FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_CopyRxDataToOneTpHandle
(
    #if (0u < PDUR_TP_BUFFER_SUM)
    PduIdType SrcPduId,
    #endif
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr
);
#endif/*STD_ON == PDUR_TP_COPYRXDATA*/

/*Rx Tp Pdu gateway to only one Tp Module Pdu handle,when Rx Indication*/
#if(STD_ON == PDUR_TP_RXINDICATION)
extern FUNC(void, PDUR_CODE)
PduR_RxIndicationToOneTpHandle(PduIdType SrcPduId,PduIdType DestPduId);
#endif/*STD_ON == PDUR_TP_RXINDICATION*/

/*one Tp Pdu route to one Tp Pdu,the dest pdu copy tx data handle*/
#if(STD_ON == PDUR_TP_COPYTXDATA)
extern FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_OneDestCopyTxDataFromTpHandle
(
    PduIdType DestPduId,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr
);
#endif/*STD_ON == PDUR_TP_COPYTXDATA*/

/*clear buffer and gateway state for gateway tp pdu*/
#if((STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT) || (STD_ON == PDUR_TP_RXINDICATION) || (STD_ON == PDUR_TP_TXCONFIRMATION))
extern FUNC(void, PDUR_CODE)
PduR_ClearBufferAndStateOfGateWayTpPdu(PduIdType PduRSrcPduId);
#endif/*(STD_ON == PDUR_TP_STARTOFRECEPTION_TRANSMIT) || (STD_ON == PDUR_TP_RXINDICATION) || (STD_ON == PDUR_TP_TXCONFIRMATION)*/
#endif/*0u < PDUR_DEST_GATEWAY_TP_PDU_SUM*/

#if((PDUR_TP_BUFFER_SUM > 0u) || (PDUR_TX_BUFFER_SUM > 0u))
/*copy data from source to dest*/
extern FUNC(void, PDUR_CODE)
PduR_Memcpy
(
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) dest,
    P2CONST(uint8, AUTOMATIC, AUTOMATIC) source,
    PduLengthType length
);
#endif
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif/*STD_OFF == PDUR_ZERO_COST_OPERATION*/

#endif/* end of PDUR_INTERNAL_H */


