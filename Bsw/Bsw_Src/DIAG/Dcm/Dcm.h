
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
**  FILENAME    :  Dcm.h                                                     **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : declaration of Dcm                                         **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef DCM_H
#define DCM_H
/*============================================================================*/
/*=======[R E V I S I O N   H I S T O R Y]====================================*
*  <VERSION>    <DATE>       <AUTHOR>    <REVISION LOG>
*  V1.0.0       2018-3-20    shushi      Initial version
*  V1.0.1       2019-12-24   tao.yu      QAC check fix
*  V1.0.2       2020-1-7     tao.yu      Commercial project problem modification
*  V2.0.0       2021-8-25    tao.yu      release version
*=======[V E R S I O N  I N F O R M A T I O N]===============================*/

#define  DCM_H_AR_MAJOR_VERSION   (0x04u)
#define  DCM_H_AR_MINOR_VERSION   (0x02u)
#define  DCM_H_AR_PATCH_VERSION   (0x02u)
#define  DCM_H_SW_MAJOR_VERSION   (0x02u)    /*Major Version*/
#define  DCM_H_SW_MINOR_VERSION   (0x00u)    /*Minor Version*/
#define  DCM_H_SW_PATCH_VERSION   (0x00u)    /*Patch version*/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Dcm_Types.h"
#include "Dcm_Internal.h"
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/


/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/*
 * Brief                <initialization of DCM module.>
 * ServiceId            <0x01>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_Init(P2CONST(Dcm_CfgType, DCM_CONST, DCM_CONST_PBCFG)ConfigPtr);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <0x24>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <VersionInfo:Pointer to where to store
 * the version information of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#if(STD_ON == DCM_VERSION_INFO_API)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dcm_GetVersionInfo(
        P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_VAR)VersionInfo);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
/*************************************************************************/
/*
 * Brief               <Function to get the VIN (as defined in SAE J1979-DA)>
 * ServiceId           <0x07>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <Data: Pointer to where to store the VIN>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType:E_OK: The Data pointer has been filled with valid VIN
                                       E_NOT_OK: The default VIN will be used in the DoIP>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetVin(P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <Triggers on changes of the UDS DTC status byte.
 *                      Allows to trigger on ROE Event for subservice OnDTCStatusChanged.>
 * ServiceId           <0x2B>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <DTC: This is the DTC the change trigger is assigned to.
                        DTCStatusOld: DTC status before change
                        DTCStatusNew: DTC status after change>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType E_OK: this value is always returned>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DemTriggerOnDTCStatus(
		uint32 DTC,
		Dem_UdsStatusByteType DTCStatusOld,
		Dem_UdsStatusByteType DTCStatusNew);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"


/*************************************************************************/
/*
 * Brief               <This function provides the active security level value.>
 * ServiceId           <0x0d>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SecLevel:Pointer to Active Security Level value >
 * Param-Name[in/out]   <None>
 * Return               <E_OK:
 * 						E_NOT_OK:>
 * PreCondition         <...>
 * CallByAPI            <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSecurityLevel(
        P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_VAR)SecLevel);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function provides the active session control type value. >
 * ServiceId           <0x06>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <SesType:Active Session Control Type value>
 * Param-Name[in/out]  <None>
 * Return              <E_OK,E_NOT_OK>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSesCtrlType(
        P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_VAR)SesType);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This function returns the active protocol name. >
 * ServiceId           <0x0f>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <description...>
 * Param-Name[out]     <ActiveProtocol:Active protocol type value >
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  FUNC(Std_ReturnType,DCM_CODE)Dcm_GetActiveProtocol(
        P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_VAR)ActiveProtocol);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <The call to this function allows the application
 *                      to reset the current session to Default session.>
 * ServiceId           <0x2a>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  FUNC(Std_ReturnType,DCM_CODE)Dcm_ResetToDefaultSession(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <The call to this function allows to trigger an
 *                      event linked to a ResponseOnEvent request.>
 * ServiceId           <0x2D>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <RoeEventId: Identifier of the event that is triggered>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  FUNC(Std_ReturnType,DCM_CODE)Dcm_TriggerOnEvent(uint8 RoeEventId);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <Allows to activate and deactivate the call
 *                      of ComM_DCM_ActiveDiagnostic() function.>
 * ServiceId           <0x56>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <active If false Dcm shall not call ComM_DCM_ActiveDiagnostic().
 *                      If true Dcm will call ComM_DCM_ActiveDiagnostic().>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  FUNC(Std_ReturnType,DCM_CODE)Dcm_SetActiveDiagnostic(boolean active);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <This service is used for processing the tasks of the main loop. >
 * ServiceId           <0x25>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  FUNC(void,DCM_CODE) Dcm_MainFunction(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"


#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_SetProgConditions(
Dcm_OpStatusType OpStatus,
Dcm_ProgConditionsType * ProgConditions
);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Dcm_EcuStartModeType Dcm_GetProgConditions(
Dcm_ProgConditionsType * ProgConditions
);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif /* CANTP_H */
