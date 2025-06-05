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
**  FILENAME    : LdCom_Cbk.h                                                **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_CBK_H_
#define LDCOM_CBK_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "LdCom_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define LDCOM_CBK_H_AR_RELEASE_MAJOR_VERSION            4u
#define LDCOM_CBK_H_AR_RELEASE_MINOR_VERSION            2u
#define LDCOM_CBK_H_AR_RELEASE_REVISION_VERSION         2u

#define LDCOM_CBK_H_SW_MAJOR_VERSION                    2u
#define LDCOM_CBK_H_SW_MINOR_VERSION                    0u
#define LDCOM_CBK_H_SW_PATCH_VERSION                    1u
/*******************************************************************************
**                      Public API Declarations                               **
*******************************************************************************/
#define LDCOM_START_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(BufReq_ReturnType, LDCOM_COPYTXDATA_CODE)
LdCom_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) availableDataPtr
);
#define LDCOM_STOP_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(void, LDCOM_TPTXCONFIRMATION_CODE)
LdCom_TpTxConfirmation(
    PduIdType id,
    Std_ReturnType result
);
#define LDCOM_STOP_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(BufReq_ReturnType, LDCOM_STARTOFRECEPTION_CODE)
LdCom_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr
);
#define LDCOM_STOP_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(BufReq_ReturnType, LDCOM_COPYRXDATA_CODE)
LdCom_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr
);
#define LDCOM_STOP_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(void, LDCOM_TPRXINDICATION_CODE)
LdCom_TpRxIndication(
    PduIdType id,
    Std_ReturnType result
);
#define LDCOM_STOP_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(void, LDCOM_RXINDICATION_CODE)
LdCom_RxIndication(
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);
#define LDCOM_STOP_SEC_LDCOMRXINDICATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(void, LDCOM_TXCONFIRMATION_CODE)
LdCom_TxConfirmation(
    PduIdType TxPduId
);
#define LDCOM_STOP_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
#include "LdCom_MemMap.h"

#define LDCOM_START_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
#include "LdCom_MemMap.h"
extern FUNC(Std_ReturnType, LDCOM_TRIGGERTRANSMIT_CODE)
LdCom_TriggerTransmit(
    PduIdType TxPduId,
    P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);
#define LDCOM_STOP_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
#include "LdCom_MemMap.h"

#endif/* LDCOM_CBK_H_ */


