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
**  FILENAME    : CanTp_Cbk.h                                                **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Call-back function declarations                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef CANTP_CBK_H
#define CANTP_CBK_H

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTp_Types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*
 * Brief               This function is called by the CAN Interface after a successful
 *                     reception of a RX CAN L-PDU.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpRxPduId the received N-PDU ID
 *                     CanTpRxPduPtr indicator of structure with received
 *                                   L-SDU(payload) and data length
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface Receiving handling
 */
#define CANTP_START_SEC_CANTPRXINDICATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
extern FUNC(void, CANTP_APPL_CODE)
CanTp_RxIndication(
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr
    );
#define CANTP_STOP_SEC_CANTPRXINDICATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               All transmitted CAN frames belonging to the CAN Transport
 *                     Layer will be confirmed by this function.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxPduId ID of CAN L-PDU that has been transmitted
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface transmitting confirmation
 */
#define CANTP_START_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
extern FUNC(void, CANTP_APPL_CODE)
CanTp_TxConfirmation(PduIdType TxPduId);
#define CANTP_STOP_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_CBK_H */

/*=======[E N D   O F   F I L E]==============================================*/
