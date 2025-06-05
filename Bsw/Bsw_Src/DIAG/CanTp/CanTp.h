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
**  FILENAME    : CanTp.h                                                    **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Public interfaces declared by CanTp module                 **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V2.0.0       2020-5-6  tao.yu         create
 *
 */
/*============================================================================*/

#ifndef CANTP_H
#define CANTP_H

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTp_Types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*========[V E R S I O N  I N F O R M A T I O N]=========*/
#define CANTP_H_AR_MAJOR_VERSION  4U
#define CANTP_H_AR_MINOR_VERSION  2U
#define CANTP_H_AR_PATCH_VERSION  2U
#define CANTP_H_SW_MAJOR_VERSION  2U
#define CANTP_H_SW_MINOR_VERSION  0U
#define CANTP_H_SW_PATCH_VERSION  0U

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"
extern CONST(CanTp_ConfigType, CANTP_CONST) CanTp_CfgData;
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*
 * Brief               This function initializes the CanTp module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(void, CANTP_APPL_CODE)
CanTp_Init(
		P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr
		);
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This function to shutdown the CanTp module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(void, CANTP_APPL_CODE)
CanTp_Shutdown(void);
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This service is used to request the transfer of segmented data.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be transmitted.
 *                     CanTpTxInfoPtr indicator of a structure with CAN N-SDU related
 *                                    data:indicator of a CAN N-SDU buffer and the length
 *                                    of this buffer.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:request can be started successfully
 *                     E_NOT_OK:request cannot be started
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_Transmit(
    PduIdType CanTpTxSduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
    );
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"


#if (CANTP_TC == STD_ON)
/*
 * Brief               This service is used to cancel the transfer of pending
 *                     CAN N-SDUs. The connection is identified by CanTpTxPduId.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be canceled.
 *                     CanTpCancelReason The reason for cancelation.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_CancelTransmit(
	PduIdType CanTpTxSduId
    );
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This service is used to cancel the reception of an ongoing N-SDU.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      This parameter contains the unique CanTp module identifier
 *                     of the N-SDU to be cancelled for transmission.
 *                     Range: 0..(maximum number of L-PDU IDs received ) - 1
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_CancelReceive(
	PduIdType CanTpRxSduId
    );
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif

#if (CANTP_CHANGE_PARAMETER == STD_ON)
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_ChangeParameter(
	PduIdType id,
	TPParameterType parameter,
	uint16 value
);
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif

#if (CANTP_READ_PARAMETER == STD_ON)
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_ReadParameter(
	PduIdType id,
	TPParameterType parameter,
	uint16* value
);
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif
/*
 * Brief               The main function for scheduling the CANTP.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp module initialized
 * CallByAPI           SchM
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(void, CANTP_APPL_CODE)
CanTp_MainFunction(void);
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_VERSION_INFO_API == STD_ON)
/*
 * Brief               This function return the version information of the CANTP module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     versioninfo indicator as to where to store the version information of this module.
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
extern FUNC(void, CANTP_APPL_CODE)
CanTp_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) versioninfo
    );
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_H */

/*=======[E N D   O F   F I L E]==============================================*/
