/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dcm.h>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:44>
 */
/*============================================================================*/


#ifndef RTEDCM_H
#define RTEDCM_H

#include "Rte_Dcm_Type.h"
#include "Dcm_Types.h"
extern boolean Dcm_NeedReset;

/***************************Security Part****************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_CompareKey( const  uint8*  Key,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_1_CompareKey( const  uint8*  Key,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_GetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,  uint8*  AttemptCounter );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_GetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,  uint8*  AttemptCounter );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_GetSeed(
		Dcm_OpStatusType OpStatus,uint8* Seed,Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_1_GetSeed(
		Dcm_OpStatusType OpStatus,uint8* Seed,Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_SetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,  uint8  AttemptCounter );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_SetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,  uint8  AttemptCounter );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/***************************Did Part****************************************/


#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataConditionCheckReadFnc_0x1111( Dcm_OpStatusType  OpStatus,  Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataConditionCheckReadFnc_0xf198( Dcm_OpStatusType  OpStatus,  Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataConditionCheckReadFnc_0xf298( Dcm_OpStatusType  OpStatus,  Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataFreezeCurrentStateFnc_0x1111(
		Dcm_OpStatusType OpStatus,
        P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataGetScalingInfoFnc_0x1111(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)ScalingInfo,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadDataLengthFnc_0x1111( Dcm_OpStatusType  OpStatus,  uint16*  DataLength );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadDataLengthFnc_0xf298( Dcm_OpStatusType  OpStatus,  uint16*  DataLength );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadFnc_0x1111( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadFnc_0xf198( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadFnc_0xf298( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadFnc_0xf286( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadFnc_0xf183( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReadFnc_0xf184( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataResetToDefaultFnc_0x1111(
		Dcm_OpStatusType OpStatus,
	    P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
	    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataReturnControlToEcuFnc_0x1111(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataShortTermAdjustmentFnc_0x1111(
		 	P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlOptionRecord,
		    uint16 DataLength,
		    Dcm_OpStatusType OpStatus,
		    P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
		    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_1111_WriteData( const  uint8*  Data,uint16  DataLength,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  DcmDspDataWriteFnc_0xf184( const  uint8*  Data,uint16  DataLength,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/***************************Routine Part****************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_RequestResults(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_Start(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_1_Start(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_Stop(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/**************function for ClearDTC**************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType Rte_ClearDTCCheck(uint32 GoDTC,Dcm_NegativeResponseCodeType* ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*******function for Protocol Start and stop***********/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern  Std_ReturnType  Dcm_DcmDslCallbackDCMRequestService_0_StartProtocol;
extern  Std_ReturnType  Dcm_DcmDslCallbackDCMRequestService_0_StopProtocol;
extern  Std_ReturnType  DcmDslCallbackDCMRequestService_0_StartProtocol(Dcm_ProtocolType  ProtocolID);
extern  Std_ReturnType  DcmDslCallbackDCMRequestService_0_StopProtocol(Dcm_ProtocolType  ProtocolID);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/********* PreConditon Check ***********************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_Rte_PreConditonCheck;
extern Std_ReturnType RTE_PreConditonCheck(void);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType SchM_PerformReset(Rte_ModeType_DcmEcuReset Reset);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_DidRangeIsDidAvailable(
    uint16 DID,
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_DidSupportedType,AUTOMATIC,DCM_VAR)supported);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
	
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_DidRangeReadDataLengthFnc(
   uint16 DID,
   Dcm_OpStatusType OpStatus,
   P2VAR(uint16,AUTOMATIC,DCM_VAR)DataLength);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
	
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_DidRangeReadDidFnc(
        uint16 DID,
       P2VAR(uint8,AUTOMATIC,DCM_VAR) Data,
       Dcm_OpStatusType OpStatus,
       uint16 DataLength,
       P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
	


#endif /*RTEDCM_H*/
