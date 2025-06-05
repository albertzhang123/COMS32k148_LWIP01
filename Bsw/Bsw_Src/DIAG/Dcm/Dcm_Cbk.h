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
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */


#ifndef COMMUNICATION_DCM_DCM_CBK_H_
#define COMMUNICATION_DCM_DCM_CBK_H_

#include "ComStack_Types.h"
/*************************************************************************/
/*
 * Brief               <This function is called at the start
                        of receiving an N-SDU. The N-SDU might be
						fragmented into multiple N-PDUs
						(FF with one or more following CFs) or might
						consist of a single N-PDU (SF)>
 * ServiceId           <0x46>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 *                      TpSduLength: This length identifies the overall
 *                      number of bytes to be received.>
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType
 * containing data pointer and length of a receive buffe>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK:
 *                      BUFREQ_E_NOT_OK:
 *                      BUFREQ_E_OVFL:
 *                      BUFREQ_E_BUSY:>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern  FUNC(BufReq_ReturnType,DCM_CODE)Dcm_StartOfReception(
		PduIdType id,
		P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
		PduLengthType TpSduLength,
		P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr);
#define  DCM_STOP_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <0x44>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType
 * containing data pointer and length of a receive buffe>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK:
 *                      BUFREQ_E_NOT_OK:
 *                      BUFREQ_E_OVFL:
 *                      BUFREQ_E_BUSY:>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMCOPYRXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern  FUNC(BufReq_ReturnType,DCM_CODE)Dcm_CopyRxData(
		PduIdType id,
		P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
		P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) bufferSizePtr);
#define  DCM_STOP_SEC_DCMCOPYRXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <Called after an I-PDU has been received via the
 *                      TP API, the result indicates whether the
 *                      transmission was successful or not.>
 * ServiceId           <0x45>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:ID of DCM I-PDU that has been received
 *                      Result: The result of the diagnostic request message received>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after
 * call of Dcm_StartOfReception() and Dcm_CopyRxData()>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMTPRXINDICATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_TpRxIndication(PduIdType id, Std_ReturnType result);
#define  DCM_STOP_SEC_DCMTPRXINDICATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <This function is called to provide the received data
 *                      of an I-PDU segment (N-PDU) to the upper layer.Each
 *                      call to this function provides the next part of the
 *                      I-PDU data.The size of the remaining data is written
 *                      to the position indicated by bufferSizePtr>
 * ServiceId           <0x43>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmRxPduId:  the received data PduId;
 * Param-Name[out]     <PduInfoPtr:  Pointer to pointer to PduInfoType
 * containing data pointer and length of a receive buffe>
 *                     <retry:This parameter is used to acknowledge transmitted
 *                      data or to retransmit data after transmission problems.>
 * Param-Name[in/out]  <None>
 * Return              <BUFREQ_OK:
 *                      BUFREQ_E_NOT_OK:
 *                      BUFREQ_E_OVFL:
 *                      BUFREQ_E_BUSY:>
 * PreCondition        <None>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMCOPYTXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(BufReq_ReturnType,DCM_CODE)Dcm_CopyTxData(
		PduIdType id,
		P2CONST(PduInfoType, AUTOMATIC, DCM_CONST) info,
		P2CONST(RetryInfoType, AUTOMATIC, DCM_VAR) retry,
		P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) availableDataPtr);
#define  DCM_STOP_SEC_DCMCOPYTXDATA_CALLBACK_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <This is called by the PduR to confirm a Transmit>
 * ServiceId           <0x48>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM I-PDU that has been transmitted.
 *                      Result: NTFRSLT_OK: the complete N-PDU has been transmitted.
 *                              NTFRSLT_E_CANCELATION_OK: the N-PDU has been successfully
 *                              cancelled.
 *                              NTFRSLT_E_CANCELATION_NOT_OK: an error occurred when
 *                              cancelling the N-PDU.
 *                              any other value: an error occurred during transmission>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <this function must be called after call of DcmProvideTxBuffer().>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_TpTxConfirmation(PduIdType id, Std_ReturnType result);
#define  DCM_STOP_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <The lower layer communication interface module
 *                       confirms the transmission of an IPDU.>
 * ServiceId           <0x40>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <DcmTxPduId:ID of DCM I-PDU that has been transmitted.
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_TxConfirmation(PduIdType  DcmTxPduId);
#define  DCM_STOP_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_NO_COMMUNICATION>
 * ServiceId           <0x21>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_ComM_NoComModeEntered(uint8 NetworkId);
#define  DCM_STOP_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_SILENT_COMMUNICATION>
 * ServiceId           <0x22>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_ComM_SilentComModeEntered(uint8 NetworkId);
#define  DCM_STOP_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <ComM module notice DCM modules,
 * network communication mode is DCM_COMM_FULL_COMMUNICATION>
 * ServiceId           <0x23>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_ComM_FullComModeEntered(uint8 NetworkId);
#define  DCM_STOP_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
#include "Dcm_MemMap.h"


#endif /* COMMUNICATION_DCM_DCM_CBK_H_ */
