/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dcm.c>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:44>
 */
/*============================================================================*/

#include "Rte_Dcm.h"
#include "Dcm.h"
#include "Dem.h"
#include "Nvm_Test2.h"
boolean Dcm_NeedReset = FALSE;

#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8  Buffer_FalseAcessCount_DcmDspSecurityRow_0  =  0x0;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"	
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8  Buffer_FalseAcessCount_DcmDspSecurityRow_1  =  0x0;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"	
/***************************Security Part****************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_CompareKey( const  uint8*  Key,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	if (Key[0] == 0x11 &&
		Key[1] == 0x22 &&
		Key[2] == 0x33 &&
		Key[3] == 0x44)
	{
		return E_OK;
	}
	else
	{
		return ((Std_ReturnType)11);
	}
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_1_CompareKey( const  uint8*  Key,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	if (Key[0] == 0x11 &&
		Key[1] == 0x22 &&
		Key[2] == 0x33 &&
		Key[3] == 0x44)
	{
		return E_OK;
	}
	else
	{
		return ((Std_ReturnType)11);
	}
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_GetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,uint8*  AttemptCounter )
{
	(*AttemptCounter) = Buffer_FalseAcessCount_DcmDspSecurityRow_0;
	
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_GetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,uint8*  AttemptCounter )
{
	(*AttemptCounter) = Buffer_FalseAcessCount_DcmDspSecurityRow_1;
	
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_GetSeed(
		Dcm_OpStatusType OpStatus,uint8* Seed,Dcm_NegativeResponseCodeType* ErrorCode)
{
	Seed[0] = 0x00;
	Seed[1] = 0x32;
	Seed[2] = 0x01;
	Seed[3] = 0xf4;
    return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_1_GetSeed(
		Dcm_OpStatusType OpStatus,uint8* Seed,Dcm_NegativeResponseCodeType* ErrorCode)
{
	Seed[0] = 0x01;
	Seed[1] = 0x32;
	Seed[2] = 0x01;
	Seed[3] = 0xf4;
    return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_SetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,  uint8  AttemptCounter )
{
	Buffer_FalseAcessCount_DcmDspSecurityRow_0 = AttemptCounter;
	
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_SetSecurityAttemptCounter( Dcm_OpStatusType  OpStatus,  uint8  AttemptCounter )
{
	Buffer_FalseAcessCount_DcmDspSecurityRow_1 = AttemptCounter;
	
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/***************************Did Part****************************************/


#define  DataLength_DcmDspData_0x1111 1
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 Buffer_DcmDspData_0x1111[ DataLength_DcmDspData_0x1111 ] ={0x20};

uint8 Buffer_DcmDspData_0x1111_Default[ DataLength_DcmDspData_0x1111 ] = {0x0} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
#define  DataLength_DcmDspData_0xf198 6
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 Buffer_DcmDspData_0xf198[ DataLength_DcmDspData_0xf198 ] = {0x20} ;
uint8 Buffer_DcmDspData_0xf198_Default[ DataLength_DcmDspData_0xf198 ] = {0x0} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
#define  DataLength_DcmDspData_0xf298 1
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 Buffer_DcmDspData_0xf298[ DataLength_DcmDspData_0xf298 ] = {0x20} ;
uint8 Buffer_DcmDspData_0xf298_Default[ DataLength_DcmDspData_0xf298 ] = {0x0} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
#define  DataLength_DcmDspData_f286 100
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 Buffer_DcmDspData_f286[ DataLength_DcmDspData_f286 ] = {0x20} ;
uint8 Buffer_DcmDspData_f286_Default[ DataLength_DcmDspData_f286 ] = {0x0} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
#define  DataLength_DcmDspData_F183 300
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 Buffer_DcmDspData_F183[ DataLength_DcmDspData_F183 ] = {0x20} ;
uint8 Buffer_DcmDspData_F183_Default[ DataLength_DcmDspData_F183 ] = {0x0} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
#define  DataLength_DcmDspData_0xf184 9
#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 Buffer_DcmDspData_0xf184[ DataLength_DcmDspData_0xf184 ] = {0x20} ;
uint8 Buffer_DcmDspData_0xf184_Default[ DataLength_DcmDspData_0xf184 ] = {0x0} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataConditionCheckReadFnc_0x1111( Dcm_OpStatusType  OpStatus,  Dcm_NegativeResponseCodeType*  ErrorCode )
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataConditionCheckReadFnc_0xf198( Dcm_OpStatusType  OpStatus,  Dcm_NegativeResponseCodeType*  ErrorCode )
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataConditionCheckReadFnc_0xf298( Dcm_OpStatusType  OpStatus,  Dcm_NegativeResponseCodeType*  ErrorCode )
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataFreezeCurrentStateFnc_0x1111(
		Dcm_OpStatusType OpStatus,
	    P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
	    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataGetScalingInfoFnc_0x1111(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)ScalingInfo,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadDataLengthFnc_0x1111( Dcm_OpStatusType  OpStatus,  uint16*  DataLength ) 
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadDataLengthFnc_0xf298( Dcm_OpStatusType  OpStatus,  uint16*  DataLength ) 
{
	*DataLength = 1u;
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadFnc_0x1111( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_0x1111;index++)
    {
    	#ifdef  DCM_DDDID_STORAGE_BLOCKID
		#ifdef  Offset_DcmDspData_0x1111
		        Data[index] = *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_0x1111 + index);
		#endif
		#else
		        //Buffer_DcmDspData_0x1111[index]=CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend;
		        Data[index] = (uint8)0x00;
		#endif
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadFnc_0xf198( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_0xf198;index++)
    {
    	#ifdef  DCM_DDDID_STORAGE_BLOCKID
		#ifdef  Offset_DcmDspData_0xf198
		        Data[index] = *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_0xf198 + index);
		#endif
		#else
		        Data[index] = Buffer_DcmDspData_0xf198[index];
		#endif
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadFnc_0xf298( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_0xf298;index++)
    {
    	#ifdef  DCM_DDDID_STORAGE_BLOCKID
		#ifdef  Offset_DcmDspData_0xf298
		        Data[index] = *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_0xf298 + index);
		#endif
		#else
		        Data[index] = Buffer_DcmDspData_0xf298[index];
		#endif
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadFnc_0xf286( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_f286;index++)
    {
    	#ifdef  DCM_DDDID_STORAGE_BLOCKID
		#ifdef  Offset_DcmDspData_f286
		        Data[index] = *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_f286 + index);
		#endif
		#else
		        Data[index] = Buffer_DcmDspData_f286[index];
		#endif
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadFnc_0xf183( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	uint16  index;
    for(index = 0;index < DataLength_DcmDspData_F183;index++)
    {
    	#ifdef  DCM_DDDID_STORAGE_BLOCKID
		#ifdef  Offset_DcmDspData_F183
		        Data[index] = *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_F183 + index);
		#endif
		#else
		        Data[index] = Buffer_DcmDspData_F183[index];
		#endif
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReadFnc_0xf184( Dcm_OpStatusType  OpStatus,uint8*  Data,Dcm_NegativeResponseCodeType*  ErrorCode )
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_0xf184;index++)
    {
    	#ifdef  DCM_DDDID_STORAGE_BLOCKID
		#ifdef  Offset_DcmDspData_0xf184
		        Data[index] = *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_0xf184 + index);
		#endif
		#else
		        Data[index] = Buffer_DcmDspData_0xf184[index];
		#endif
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataResetToDefaultFnc_0x1111(
		Dcm_OpStatusType OpStatus,
	    P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
	    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_0x1111;index++)
    {
        Buffer_DcmDspData_0x1111[index] = Buffer_DcmDspData_0x1111_Default[index];
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataReturnControlToEcuFnc_0x1111(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
	    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataShortTermAdjustmentFnc_0x1111( 
		 	P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlOptionRecord,
		    uint16 DataLength,
		    Dcm_OpStatusType OpStatus,
		    P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
		    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	uint8  index;
    for(index = 0;index < DataLength_DcmDspData_0x1111;index++)
    {
    	Buffer_DcmDspData_0x1111[index] = ControlOptionRecord[index];
    }
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_1111_WriteData( const  uint8*  Data,uint16  DataLength,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode ) 
{
	uint8  index = 0;
	boolean triggered = FALSE;
    for(index = 0; index < DataLength_DcmDspData_0x1111; index++)
    {
		#ifdef DCM_DDDID_STORAGE_BLOCKID
		#ifdef Offset_DcmDspData_0x1111
		        *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_0x1111 + index) = Data[index] ;
		#endif
		#else
		        if ((Buffer_DcmDspData_0x1111[index] != Data[index]) &&
		        	(FALSE == triggered))
		        {
		        	Dcm_TriggerOnEvent(1);
		        	triggered = TRUE;
		        }
		        
		        Buffer_DcmDspData_0x1111[index]= Data[index];
		#endif
    }
    triggered = FALSE;
	#ifdef DCM_DDDID_STORAGE_BLOCKID
	    NvM_WriteBlock(DCM_DDDID_STORAGE_BLOCKID, NULL_PTR);
	#endif
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  DcmDspDataWriteFnc_0xf184( const  uint8*  Data,uint16  DataLength,Dcm_OpStatusType  OpStatus,Dcm_NegativeResponseCodeType*  ErrorCode ) 
{
	uint8  index = 0;
    for(index = 0; index < DataLength_DcmDspData_0xf184; index++)
    {
		#ifdef DCM_DDDID_STORAGE_BLOCKID
		#ifdef Offset_DcmDspData_0xf184
		        *(NvM_BlockDescriptor[DCM_DDDID_STORAGE_BLOCKID - 1].NvmRamBlockDataAddress + Offset_DcmDspData_0xf184 + index) = Data[index] ;
		#endif
		#else
		        Buffer_DcmDspData_0xf184[index] = Data[index];
		#endif
    }
	#ifdef DCM_DDDID_STORAGE_BLOCKID
	    NvM_WriteBlock(DCM_DDDID_STORAGE_BLOCKID, NULL_PTR);
	#endif
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/***************************Routine Part****************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_RequestResults(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{

	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"		

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_Start(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	uint8  index;
	for(index = 0;index < DataLength_DcmDspData_0x1111;index++)
	{
		OutBuffer[index] = InBuffer[index];
	}
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_1_Start(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_Stop(
		P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
		P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode)
{
	uint8  index;
	for(index = 0;index < DataLength_DcmDspData_0x1111;index++)
		{
			OutBuffer[index] = InBuffer[index];
		}
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/**************function for ClearDTC**************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Rte_ClearDTCCheck(uint32 GoDTC,Dcm_NegativeResponseCodeType* ErrorCode)
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*******function for Protocol Start and stop***********/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType  Dcm_DcmDslCallbackDCMRequestService_0_StartProtocol = E_OK;
Std_ReturnType  DcmDslCallbackDCMRequestService_0_StartProtocol(Dcm_ProtocolType  ProtocolID)
{
    return(Dcm_DcmDslCallbackDCMRequestService_0_StartProtocol);
}

Std_ReturnType  Dcm_DcmDslCallbackDCMRequestService_0_StopProtocol = E_OK;
Std_ReturnType  DcmDslCallbackDCMRequestService_0_StopProtocol(Dcm_ProtocolType  ProtocolID)
{
    return(Dcm_DcmDslCallbackDCMRequestService_0_StopProtocol);
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*PreConditon Check*/
Std_ReturnType  Dcm_Rte_PreConditonCheck = E_OK;
Std_ReturnType RTE_PreConditonCheck(void)
{
	return Dcm_Rte_PreConditonCheck;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"



#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType SchM_PerformReset(Rte_ModeType_DcmEcuReset Reset)
{
	Dem_Shutdown();

	if (Reset == RTE_MODE_DcmEcuReset_EXECUTE)
	{
		 Dcm_NeedReset = TRUE;
	}
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_DidRangeIsDidAvailable(
    uint16 DID,
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_DidSupportedType,AUTOMATIC,DCM_VAR)supported)
{
	*supported = (Dcm_DidSupportedType)0x00;
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
	
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_DidRangeReadDataLengthFnc(
   uint16 DID,
   Dcm_OpStatusType OpStatus,
   P2VAR(uint16,AUTOMATIC,DCM_VAR)DataLength)
{
	*DataLength  = 1;
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
	
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_DidRangeReadDidFnc(
        uint16 DID,
       P2VAR(uint8,AUTOMATIC,DCM_VAR) Data,
       Dcm_OpStatusType OpStatus,
       uint16 DataLength,
       P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode)
{
	return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
	


