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
**  FILENAME    : OBD.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration of OBD                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

#ifndef OBD_H
#define OBD_H

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm_Types.h"

/*******************************************************************************
**                      macros                                                **
*******************************************************************************/
#define OBD_BITS_SET(_p,_n) ((_p)[(_n)>>3u] |= (uint8)(1u<<((_n)&7u)))

/* Service request packet length defined */
#define  DCM_UDS0X01_REQ_DATA_LENGTH_MIN                (2U)
/*0x01 Service request packet min length defined*/
#define  DCM_UDS0X01_REQ_DATA_LENGTH_MAX                (7U)
/*0x01 Service request packet max length defined*/
#define  DCM_UDS0X02_REQ_DATA_LENGTH_MIN                (3U)
/*0x02 Service request packet min length defined*/
#define  DCM_UDS0X02_REQ_DATA_LENGTH_MAX                (7U)
/*0x02 Service request packet max length defined*/
#define  DCM_UDS0X03_REQ_DATA_LENGTH                    (1U)
/*0x03 Service request packet length defined*/
#define  DCM_UDS0X04_REQ_DATA_LENGTH                    (1U)
/*0x04 Service request packet length defined*/
#define  DCM_UDS0X06_REQ_DATA_LENGTH_MIN                (2U)
/*0x06 Service request packet min length defined*/
#define  DCM_UDS0X06_REQ_DATA_LENGTH_MAX                (7U)
/*0x06 Service request packet max length defined*/
#define  DCM_UDS0X07_REQ_DATA_LENGTH                    (1U)
/*0x07 Service request packet length defined*/
#define  DCM_UDS0X08_REQ_DATA_LENGTH_MIN                (2U)
/*0x08 Service request packet min length defined*/
#define  DCM_UDS0X08_REQ_DATA_LENGTH_MAX                (7U)
/*0x08 Service request packet max length defined*/
#define  DCM_UDS0X09_REQ_DATA_LENGTH_MIN                (2U)
/*0x09 Service request packet min length defined*/
#define  DCM_UDS0X09_REQ_DATA_LENGTH_MAX                (7U)
/*0x09 Service request packet max length defined*/
#define  DCM_UDS0X0A_REQ_DATA_LENGTH                    (1U)
/*0x0A Service request packet length defined*/

/*SID Table*/
#define SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA                       0x01u
#define SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA                             0x02u
#define SID_REQUEST_EMISSION_RELATED_DTC                                     0x03u
#define SID_CLEAR_EMISSION_RELATED_DIAGNOSTIC_INFORMATION                    0x04u
#define SID_REQUEST_OXYGEN_SENSOR_MONITORING_TEST_RESULTS                    0x05u
#define SID_REQUEST_OBD_MONITORING_TEST_RESULTS                              0x06u
#define SID_REQUEST_EMISSION_RELATED_DTC_DURING_CURRENT_DC                   0x07u
#define SID_REQUEST_CONTROL                                                  0x08u
#define SID_REQUEST_VEHICLE_INFORMATION                                      0x09u
#define SID_REQUEST_PERMANENT_EMISSION_RELATED_DTC                           0x0Au

/*******************************************************************************
**                      Global Function declaration                           **
*******************************************************************************/
#if(STD_ON == DCM_OBD_FUNC_ENABLED)

#if(STD_ON == DCM_OBD_SERVICE0X01_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x01): Request current powertrain diagnostic data>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x01(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X01_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X02_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x02):  Request Power Train FreezeFrame Data>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x02(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X02_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X03_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x03):  Request emission-related diagnostic trouble codes>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x03(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X03_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X04_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x04):  Clear/reset emission-related diagnostic information>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x04(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X04_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X05_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x05):  Request oxygen sensor monitoring test results>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
extern Std_ReturnType  Dcm_OBD0x05(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#endif

#if(STD_ON == DCM_OBD_SERVICE0X06_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x06): Request On-Board Monitoring Test-results
 *                                           for Specific Monitored Systems>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x06(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X06_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X07_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x07):  Request emission-related diagnostic trouble
 *                          codes detected during current or last completed driving cycle>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x07(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X07_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X08_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x08): Request Control of On-Board System, Test or Component>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x08(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X08_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X09_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x09): Request Vehicle Information>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x09(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X09_ENABLED*/

#if(STD_ON == DCM_OBD_SERVICE0X0A_ENABLED)
/******************************************************************************/
/*
 * Brief               <OBD service (0x0A):   Request emission-related diagnostic
 *                                              trouble codes with permanent status>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_OBD0x0A(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X0A_ENABLED*/

#endif /*STD_ON == DCM_OBD_FUNC_ENABLED*/

#endif /* OBD_H */
/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
