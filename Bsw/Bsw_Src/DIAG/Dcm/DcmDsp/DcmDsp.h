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

#ifndef DCMDSP_H
#define DCMDSP_H

/****************************** declarations *********************************/
#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
	#define DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
	#include "Dcm_MemMap.h"
	extern  VAR(Dcm_RoutineControlStateType,DCM_VAR_NOINIT)
	Dcm_RoutineControlState[DCM_DSP_ROUTINE_MAX_NUM];
	#define DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
	#include "Dcm_MemMap.h"
#endif

/*************************************************************************/
/*
* Brief               <The Dsp layer response to the confirmation>
* ServiceId           <None>
* Sync/Async          <Synchronous>
* Reentrancy          <Reentrant>
* Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number>
* Param-Name[out]     <None>
* Param-Name[in/out]  <None>
* Return              <None>
* PreCondition        <None>
* CallByAPI           <APIName>
*/
/*************************************************************************/

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)DspInternal_DcmConfirmation(uint8 ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"


#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)Dsp_InitRoutineStates(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(Std_ReturnType, DCM_CODE)Dcm_UdsSubServicesCheck( uint8 ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if(STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern   FUNC(void,DCM_CODE)DspInternal_DcmUpdatePage(uint8 ProtocolCtrlId);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
/****************************** definitions *********************************/
#endif /* DCMDSP_H_ */
