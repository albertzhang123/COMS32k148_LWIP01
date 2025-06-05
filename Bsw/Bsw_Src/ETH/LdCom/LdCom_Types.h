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
**  FILENAME    : LdCom_Types.h                                              **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_TYPES_H_
#define LDCOM_TYPES_H_
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
#include "LdCom_Cfg.h"
#include "PduR_LdCom.h"

#define LDCOM_TYPES_H_AR_MAJOR_VERSION  4u
#define LDCOM_TYPES_H_AR_MINOR_VERSION  2u
#define LDCOM_TYPES_H_AR_PATCH_VERSION  2u
#define LDCOM_TYPES_H_SW_MAJOR_VERSION  2u
#define LDCOM_TYPES_H_SW_MINOR_VERSION  0u
#define LDCOM_TYPES_H_SW_PATCH_VERSION  1u

#if (STD_ON == LDCOM_DEV_ERROR_DETECT)
/* Error classification */
#define LDCOM_E_PARAM                                       ((uint8)0x01u)
#define LDCOM_E_UNINIT                                      ((uint8)0x02u)
#define LDCOM_E_PARAM_POINTER                               ((uint8)0x03u)
#define LDCOM_E_INVALID_PDU_SDU_ID                          ((uint8)0x04u)
#define LDCOM_E_INVALID_SIGNAL_ID                           ((uint8)0x05u)

/* Service ID[hex] */
#define LDCOM_SERVICE_ID_INIT                               ((uint8)0x00u)
#define LDCOM_SERVICE_ID_DEINIT                             ((uint8)0x02u)
#define LDCOM_SERVICE_ID_GETVERSIONINFO                     ((uint8)0x03u)
#define LDCOM_SERVICE_ID_TRANSMIT                           ((uint8)0x05u)
#define LDCOM_SERVICE_ID_COPYTXDATA                         ((uint8)0x43u)
#define LDCOM_SERVICE_ID_TPTXCONFIRMATION                   ((uint8)0x48u)
#define LDCOM_SERVICE_ID_STARTOFRECEPTION                   ((uint8)0x46u)
#define LDCOM_SERVICE_ID_COPYRXDATA                         ((uint8)0x44u)
#define LDCOM_SERVICE_ID_TPRXINDICATION                     ((uint8)0x45u)
#define LDCOM_SERVICE_ID_RXINDICATION                       ((uint8)0x42u)
#define LDCOM_SERVICE_ID_TXCONFIRMATION                     ((uint8)0x40u)
#define LDCOM_SERVICE_ID_TRIGGERTRANSMIT                    ((uint8)0x41u)
#endif/*STD_ON == LDCOM_DEV_ERROR_DETECT*/

#define LDCOM_UNUSED_UINT16                   (0xFFFFu)

/* Contains the configuration parameters of the IPdu inside LdCom. */
typedef struct
{
    PduIdType       LdComPduId;
    PduIdType       DestModulePduId;
#if (STD_ON == LDCOM_SUPPORT_SEND_TP)
    P2FUNC(BufReq_ReturnType,LDCOM_APPL_CODE,LdComTxCopyTxDataFuncPtr)(
            const PduInfoType* info,
            const RetryInfoType* retry,
            PduLengthType* availableDataPtr);
    P2FUNC(void,LDCOM_APPL_CODE,LdComTpTxConfirmationFuncPtr)(
            Std_ReturnType result);
#endif/* STD_ON == LDCOM_SUPPORT_SEND_TP */

#if (STD_ON == LDCOM_SUPPORT_RECEIVER_TP)
    P2FUNC(BufReq_ReturnType,LDCOM_APPL_CODE,LdComRxStartOfReceptionFuncPtr)(
            const PduInfoType* info,
            PduLengthType TpSduLength,
            PduLengthType* bufferSizePtr);
    P2FUNC(BufReq_ReturnType,LDCOM_APPL_CODE,LdComRxCopyRxDataFuncPtr)(
            const PduInfoType* info,
            PduLengthType* bufferSizePtr);
    P2FUNC(void,LDCOM_APPL_CODE,LdComTpRxIndicationFuncPtr)(
            Std_ReturnType result);
#endif/* STD_ON == LDCOM_SUPPORT_RECEIVER_TP */

#if (STD_ON == LDCOM_SUPPORT_SEND_IF)
    P2FUNC(void,LDCOM_APPL_CODE,LdComTxConfirmationFuncPtr)(void);
#endif/* STD_ON == LDCOM_SUPPORT_SEND_IF */

#if (STD_ON == LDCOM_SUPPORT_RECEIVER_IF)
    P2FUNC(void,LDCOM_APPL_CODE,LdComRxIndicationFuncPtr)(
            const PduInfoType* PduInfoPtr);
#endif/* STD_ON == LDCOM_SUPPORT_RECEIVER_IF */
#if (STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT)
    P2FUNC(Std_ReturnType,LDCOM_APPL_CODE,LdComTxTriggerTransmitFuncPtr)(
            PduInfoType* PduInfoPtr);
#endif/* STD_ON == LDCOM_SUPPORT_TRIGGER_TRANSMIT */
} LdCom_IPduType;

typedef struct
{
    P2CONST(LdCom_IPduType, AUTOMATIC, LDCOM_CONST_PBCFG) LdComIPduPtr;
} LdCom_ConfigType;

#endif/* LDCOM_TYPES_H_ */

