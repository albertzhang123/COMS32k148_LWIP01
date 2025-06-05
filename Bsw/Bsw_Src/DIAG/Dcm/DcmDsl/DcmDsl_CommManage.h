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

#ifndef DCMDSL_COMMMANAGE_H
#define DCMDSL_COMMMANAGE_H
/*============================================================================*/

/**************************************************************/
#include "Dcm_Types.h"

/***************************************************************************************
 *************Communication Management sub-function data structure*****
 **************************************************************************************/
typedef  enum
{
   DCM_COMM_NO_COMMUNICATION      = 0, /* conmmunication is "NO Communication"status */
   DCM_COMM_SILENT_COMMUNICATION  = 1, /* conmmunication is "Silent Communication"status */
   DCM_COMM_FULL_COMMUNICATION    = 2  /* conmmunication is "Full Communication"status*/
}Dcm_CommStateType;

typedef  enum
{
	DCM_COMM_ACTIVE      = 0,
	DCM_COMM_NOT_ACTIVE  = 1,
}Dcm_ActiveDiagnosticType;

typedef struct
{
   Dcm_CommStateType  Dcm_CommState;
   Dcm_ActiveDiagnosticType Dcm_ActiveDiagnostic;
   NetworkHandleType DcmDslProtocolComMChannelId;
}Dcm_CommCtrlType;


#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
extern  VAR(Dcm_CommCtrlType,DCM_VAR_NOINIT)Dcm_CommCtrl[DCM_MAINCONNECTION_NUM];
/*Diagnostic Communication Control*/
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
/*********************************************************************************
 *****************Communication Management function declarations******************
 ********************************************************************************/
/*************************************************************************/
/*
 * Brief               <initialization of Comm Submodule>
 * ServiceId           <None>
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
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)DslInternal_InitComMCtrl(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif /* DCMDSL_COMMMANAGE_H_ */
