/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2016)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Dcm_OBDIternal.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Internal API declaration of OBD                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef DCM_OBDINTERNAL_H
#define DCM_OBDINTERNAL_H
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm_Types.h"
#include "Dcm_Internal.h"
#include "Dcm_Cfg.h"
#include "DcmDsl_ProtocolManage.h"
#include "Dem.h"
#include "Dem_Dcm.h"

/*******************************************************************************
**                      macros                                                **
*******************************************************************************/
#define OBD_DATA_LSB_MASK                   ((uint32)0x000000FFu)

typedef struct
{
    boolean UDSCall;
    uint8 SID;
    uint8 pid;
}OBD_ReadPidValuesTypes;
/*******************************************************************************
**                      Global Function declaration                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               MemSet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Val && Size
 * Param-Name[out]     none
 * Param-Name[in/out]  Dest
 * Return              none
 */
/*************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(void, DCM_CODE) OBD_MemSet(
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Dest,
    uint8 Val,
    uint32 Size);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Read PID values>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ObdMidCfgIndex && ptxPos && pnoFindPIDsNum>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
 #define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE) OBD_ReadPidValues(
        OBD_ReadPidValuesTypes* OBD_ReadPid,
        P2VAR(uint16, AUTOMATIC, DCM_VAR) pBufOffset,
        P2VAR(uint8, AUTOMATIC, DCM_VAR) pNoFindPidNum
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/******************************************************************************/
/*
 * Brief               <Check supported Parameter id>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE)
OBD_CheckSupportedPIDs(uint8 pid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf,uint8 SID);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
/******************************************************************************/
/*
 * Brief               <Check supported test id>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <none>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE)
OBD_CheckSupportedTIDs(uint8 Tid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if (DCM_DSP_REQUESTCONTROL_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <find config index number of TID>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pCfgIndex>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE) OBD_FindCfgIndexOfTestID(uint8 testID, P2VAR(
        uint8, AUTOMATIC, DCM_VAR) pCfgIndex);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
#if (DCM_DSP_VEHINFO_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Set Availability InfoType Value>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <InfoType && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE)
OBD_SetAvailabilityInfoTypeValue(uint8 InfoType, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/******************************************************************************/
/*
 * Brief               <find config index number of VehInfo>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pCfgIndex>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE) OBD_FindCfgIndexOfVehInfo(uint8 vehInfo,
        P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(void, DCM_CODE)Dcm_FillTo4bytes(P2VAR(uint8, AUTOMATIC, DCM_VAR) pDataBuf,
        P2CONST(uint32, AUTOMATIC, DCM_VAR) pData);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif /* DCM_OBDINTERNAL_H */
