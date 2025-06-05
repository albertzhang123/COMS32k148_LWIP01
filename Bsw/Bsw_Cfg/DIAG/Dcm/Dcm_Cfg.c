/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dcm_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:44>
 */
/*============================================================================*/

/******************************* references ************************************/
#include "Dcm_Types.h"
#include "Rte_Dcm.h"
#include "Dcm_Cfg.h"
#if(STD_ON == DCM_UDS_FUNC_ENABLED)
#include "UDS.h"
#endif
#if(STD_ON == DCM_OBD_FUNC_ENABLED)
#include "OBD.h"
#endif

/**********************DID*************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static const boolean DcmDspDIDcheckPerSourceDID = FALSE;
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/**********************Clear DTC**************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspClearDTCType, DCM_CONST) Dcm_DspClearDTCCfg =
{
	Rte_ClearDTCCheck, /*DcmDsp_ClearDTCCheckFnc*/
	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*******************Control DTC Setting********************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspControlDTCSettingType, DCM_CONST) Dcm_DspControlDTCSettingCfg =	
{
	FALSE, /*DcmSupportDTCSettingControlOptionRecord*/
	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*******************Com Control********************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DspComControlAllChannelType, DCM_CONST) Dcm_DspComControlAllChannelCfg[1] = 
{
	{
		TRUE, /*DcmDspComControlAllChannelUsed*/
		0u, /*DcmDspComMChannelId*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspComControlType, DCM_CONST) Dcm_DspComControlCfg =
{
	1u, /*DcmDspComControlAllChannelNum*/
	&Dcm_DspComControlAllChannelCfg[0], /*DcmDspComControlAllChannel*/	
	NULL_PTR, /*DcmDspComControlSetting*/								
	0u, /*DcmDspComControlSpecificChannelNum*/								
	NULL_PTR, /*DcmDspComControlSpecificChannel*/	
	0u, /*DcmDspComControlSubNodeNum*/								
	NULL_PTR /*DcmDspComControlSubNode*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*******************Common Authorization********************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)DcmDspCommonAuthorization_0_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)DcmDspCommonAuthorization_0_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspCommonAuthorizationType, DCM_CONST) Dcm_DspCommonAuthorizationCfg[1] =
{
	{
		
		1u,	 /*DcmDspCommonAuthorizationSecurityLevelRefNum*/
		&DcmDspCommonAuthorization_0_SecRef[0], /*DcmDspCommonAuthorizationSecurityLevelRef*/
		1u, /*DcmDspCommonAuthorizationSessionRefNum*/
		&DcmDspCommonAuthorization_0_SesRef[0], /*DcmDspCommonAuthorizationSessionRef*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/********************Dsp Data******************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDataInfoType, DCM_CONST) Dcm_DspDataInfoCfg[1] =
{
	{
		1UL, /*DcmDspDataScalingInfoSize*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDataType, DCM_CONST) Dcm_DspDataCfg[6] =
{
	{
		DcmDspDataConditionCheckReadFnc_0x1111,	 /*DcmDspDataConditionCheckReadFnc*/	
		TRUE, /*DcmConditionCheckReadFncUsed*/	
		NULL_PTR,	 /*DcmDspDataEcuSignalFnc*/	
		NULL_PTR, /*DcmDspDataReadEcuSignalFnc*/	
		DCM_OPAQUE, /*DcmDspDataEndianness*/	
		DcmDspDataFreezeCurrentStateFnc_0x1111,	 /*DcmDspDataFreezeCurrentsStateFnc*/	
		DcmDspDataGetScalingInfoFnc_0x1111,	 /*DcmDspDataGetScalingInfoFnc*/	
		DcmDspDataReadDataLengthFnc_0x1111,  /*DcmDspDataReadDataLengthFnc*/		
		DcmDspDataReadFnc_0x1111, /*DcmDspDataReadFnc*/		
		DcmDspDataResetToDefaultFnc_0x1111, /*DcmDspDataResetToDefaultFnc*/	
		DcmDspDataReturnControlToEcuFnc_0x1111, /*DcmDspDataReturnControlToECUFnc*/		
		DcmDspDataShortTermAdjustmentFnc_0x1111, /*DcmDspDataShortTermAdjustmentFnc*/		
		Rte_1111_WriteData, /*DcmDspDataWriteFnc*/		
		8u, /*DcmDspDataSize*/	
		DCM_UINT8, /*DcmDspDataType*/	
		USE_DATA_ASYNCH_FNC, /*DcmDspDataUsePort*/	
		0u, /*DcmDspDataBlockId*/
		0u, /*DcmDspDataInfoIndex*/	
		NULL_PTR, /*DcmDspDiagnosisScaling*/	
		NULL_PTR /*DcmDspExternalSRDataElementClass*/	
	},
	{
		DcmDspDataConditionCheckReadFnc_0xf198,	 /*DcmDspDataConditionCheckReadFnc*/	
		TRUE, /*DcmConditionCheckReadFncUsed*/	
		NULL_PTR,	 /*DcmDspDataEcuSignalFnc*/	
		NULL_PTR, /*DcmDspDataReadEcuSignalFnc*/	
		DCM_OPAQUE, /*DcmDspDataEndianness*/	
		NULL_PTR,	 /*DcmDspDataFreezeCurrentsStateFnc*/	
		NULL_PTR,	 /*DcmDspDataGetScalingInfoFnc*/	
		NULL_PTR,  /*DcmDspDataReadDataLengthFnc*/		
		DcmDspDataReadFnc_0xf198, /*DcmDspDataReadFnc*/		
		NULL_PTR, /*DcmDspDataResetToDefaultFnc*/	
		NULL_PTR, /*DcmDspDataReturnControlToECUFnc*/		
		NULL_PTR, /*DcmDspDataShortTermAdjustmentFnc*/		
		NULL_PTR, /*DcmDspDataWriteFnc*/		
		48u, /*DcmDspDataSize*/	
		DCM_UINT8, /*DcmDspDataType*/	
		USE_DATA_ASYNCH_FNC, /*DcmDspDataUsePort*/	
		0u, /*DcmDspDataBlockId*/
		0xffu, /*DcmDspDataInfoIndex*/	
		NULL_PTR, /*DcmDspDiagnosisScaling*/	
		NULL_PTR /*DcmDspExternalSRDataElementClass*/	
	},
	{
		DcmDspDataConditionCheckReadFnc_0xf298,	 /*DcmDspDataConditionCheckReadFnc*/	
		TRUE, /*DcmConditionCheckReadFncUsed*/	
		NULL_PTR,	 /*DcmDspDataEcuSignalFnc*/	
		NULL_PTR, /*DcmDspDataReadEcuSignalFnc*/	
		DCM_OPAQUE, /*DcmDspDataEndianness*/	
		NULL_PTR,	 /*DcmDspDataFreezeCurrentsStateFnc*/	
		NULL_PTR,	 /*DcmDspDataGetScalingInfoFnc*/	
		DcmDspDataReadDataLengthFnc_0xf298,  /*DcmDspDataReadDataLengthFnc*/		
		DcmDspDataReadFnc_0xf298, /*DcmDspDataReadFnc*/		
		NULL_PTR, /*DcmDspDataResetToDefaultFnc*/	
		NULL_PTR, /*DcmDspDataReturnControlToECUFnc*/		
		NULL_PTR, /*DcmDspDataShortTermAdjustmentFnc*/		
		NULL_PTR, /*DcmDspDataWriteFnc*/		
		8u, /*DcmDspDataSize*/	
		DCM_UINT8_DYN, /*DcmDspDataType*/	
		USE_DATA_ASYNCH_FNC, /*DcmDspDataUsePort*/	
		0u, /*DcmDspDataBlockId*/
		0xffu, /*DcmDspDataInfoIndex*/	
		NULL_PTR, /*DcmDspDiagnosisScaling*/	
		NULL_PTR /*DcmDspExternalSRDataElementClass*/	
	},
	{
		NULL_PTR,	 /*DcmDspDataConditionCheckReadFnc*/	
		TRUE, /*DcmConditionCheckReadFncUsed*/	
		NULL_PTR,	 /*DcmDspDataEcuSignalFnc*/	
		NULL_PTR, /*DcmDspDataReadEcuSignalFnc*/	
		DCM_OPAQUE, /*DcmDspDataEndianness*/	
		NULL_PTR,	 /*DcmDspDataFreezeCurrentsStateFnc*/	
		NULL_PTR,	 /*DcmDspDataGetScalingInfoFnc*/	
		NULL_PTR,  /*DcmDspDataReadDataLengthFnc*/		
		DcmDspDataReadFnc_0xf286, /*DcmDspDataReadFnc*/		
		NULL_PTR, /*DcmDspDataResetToDefaultFnc*/	
		NULL_PTR, /*DcmDspDataReturnControlToECUFnc*/		
		NULL_PTR, /*DcmDspDataShortTermAdjustmentFnc*/		
		NULL_PTR, /*DcmDspDataWriteFnc*/		
		800u, /*DcmDspDataSize*/	
		DCM_UINT8, /*DcmDspDataType*/	
		USE_DATA_ASYNCH_FNC, /*DcmDspDataUsePort*/	
		0u, /*DcmDspDataBlockId*/
		0xffu, /*DcmDspDataInfoIndex*/	
		NULL_PTR, /*DcmDspDiagnosisScaling*/	
		NULL_PTR /*DcmDspExternalSRDataElementClass*/	
	},
	{
		NULL_PTR,	 /*DcmDspDataConditionCheckReadFnc*/	
		TRUE, /*DcmConditionCheckReadFncUsed*/	
		NULL_PTR,	 /*DcmDspDataEcuSignalFnc*/	
		NULL_PTR, /*DcmDspDataReadEcuSignalFnc*/	
		DCM_OPAQUE, /*DcmDspDataEndianness*/	
		NULL_PTR,	 /*DcmDspDataFreezeCurrentsStateFnc*/	
		NULL_PTR,	 /*DcmDspDataGetScalingInfoFnc*/	
		NULL_PTR,  /*DcmDspDataReadDataLengthFnc*/		
		DcmDspDataReadFnc_0xf183, /*DcmDspDataReadFnc*/		
		NULL_PTR, /*DcmDspDataResetToDefaultFnc*/	
		NULL_PTR, /*DcmDspDataReturnControlToECUFnc*/		
		NULL_PTR, /*DcmDspDataShortTermAdjustmentFnc*/		
		NULL_PTR, /*DcmDspDataWriteFnc*/		
		2400u, /*DcmDspDataSize*/	
		DCM_UINT8, /*DcmDspDataType*/	
		USE_DATA_ASYNCH_FNC, /*DcmDspDataUsePort*/	
		0u, /*DcmDspDataBlockId*/
		0xffu, /*DcmDspDataInfoIndex*/	
		NULL_PTR, /*DcmDspDiagnosisScaling*/	
		NULL_PTR /*DcmDspExternalSRDataElementClass*/	
	},
	{
		NULL_PTR,	 /*DcmDspDataConditionCheckReadFnc*/	
		TRUE, /*DcmConditionCheckReadFncUsed*/	
		NULL_PTR,	 /*DcmDspDataEcuSignalFnc*/	
		NULL_PTR, /*DcmDspDataReadEcuSignalFnc*/	
		DCM_OPAQUE, /*DcmDspDataEndianness*/	
		NULL_PTR,	 /*DcmDspDataFreezeCurrentsStateFnc*/	
		NULL_PTR,	 /*DcmDspDataGetScalingInfoFnc*/	
		NULL_PTR,  /*DcmDspDataReadDataLengthFnc*/		
		DcmDspDataReadFnc_0xf184, /*DcmDspDataReadFnc*/		
		NULL_PTR, /*DcmDspDataResetToDefaultFnc*/	
		NULL_PTR, /*DcmDspDataReturnControlToECUFnc*/		
		NULL_PTR, /*DcmDspDataShortTermAdjustmentFnc*/		
		DcmDspDataWriteFnc_0xf184, /*DcmDspDataWriteFnc*/		
		72u, /*DcmDspDataSize*/	
		DCM_UINT8, /*DcmDspDataType*/	
		USE_DATA_ASYNCH_FNC, /*DcmDspDataUsePort*/	
		0u, /*DcmDspDataBlockId*/
		0xffu, /*DcmDspDataInfoIndex*/	
		NULL_PTR, /*DcmDspDiagnosisScaling*/	
		NULL_PTR /*DcmDspExternalSRDataElementClass*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/********************Dsp Did******************************/
/******************************************
 *DcmDspDidRead container configration
 *****************************************/																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_0_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_1_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_2_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_9_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_10_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_11_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_12_ReadCfg =
{
    0u, /*DcmDspDidReadSecurityLevelRefNum*/
    NULL_PTR, /*pDcmDspDidReadSecurityLevelRow*/
    0u, /*DcmDspDidReadSessionRefNum*/
    NULL_PTR, /*pDcmDspDidReadSessionRow*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*******************************************
 *DcmDspDidWrite container configuration,
 which is in the DcmDspDidInfo container
 ******************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_DidInfo_0_Write_SecRefCfg[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_DidInfo_0_Write_SesRefCfg[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidWriteType,DCM_CONST)Dcm_DidInfo_0_WriteCfg= 
{
    1u,/*DcmDspDidWriteSecurityLevelRefNum*/
    &Dcm_DidInfo_0_Write_SecRefCfg[0],	/*pDcmDspDidWriteSecurityLevelRow*/
    1u,	/*DcmDspDidWriteSessionRefNum*/
    &Dcm_DidInfo_0_Write_SesRefCfg[0], /*pDcmDspDidWriteSessionRow*/						
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_DidInfo_11_Write_SecRefCfg[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_DidInfo_11_Write_SesRefCfg[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
																		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DspDidWriteType,DCM_CONST)Dcm_DidInfo_11_WriteCfg= 
{
    1u,/*DcmDspDidWriteSecurityLevelRefNum*/
    &Dcm_DidInfo_11_Write_SecRefCfg[0],	/*pDcmDspDidWriteSecurityLevelRow*/
    1u,	/*DcmDspDidWriteSessionRefNum*/
    &Dcm_DidInfo_11_Write_SesRefCfg[0], /*pDcmDspDidWriteSessionRow*/						
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*******************************************
 *DcmDspDidControl container configuration,
 which is in the DcmDspDidInfo container
 ******************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(uint8,DCM_CONST)Dcm_DidInfo_0_Control_SecRefCfg[1]= {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(uint8,DCM_CONST)Dcm_DidInfo_0_Control_SesRefCfg[1]={3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidControlType,DCM_CONST) Dcm_DidInfo_0_ControlCfg = 
{
	DCM_CONTROLMASK_NO, /*DcmDspDidControlMask*/
	0u, /*DcmDspDidControlMaskSize*/	
    1u, /*DcmDspDidControlSecurityLevelRefNum*/
    &Dcm_DidInfo_0_Control_SecRefCfg[0], /*pDcmDspDidControlSecurityLevelRow*/
    1u, /*DcmDspDidControlSessionRefNum*/
    &Dcm_DidInfo_0_Control_SesRefCfg[0], /*pDcmDspDidControlSessionRow*/
    TRUE, /*DcmDspDidFreezeCurrentState*/
	TRUE, /*DcmDspDidResetToDefault*/
    TRUE, /*DcmDspDidShortTermAdjustement*/	
	NULL_PTR, /*DcmDspDidControlEnableMask*/	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"	
/******************************************
 *DcmDspDidInfo container Configuration ***
 ******************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidInfoType,DCM_CONST)Dcm_DspDidInfoCfg[13] = 
{
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		&Dcm_DidInfo_0_ControlCfg, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_0_ReadCfg, /*pDcmDspDidRead*/	
		&Dcm_DidInfo_0_WriteCfg, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_1_ReadCfg, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        2u, /*DcmDspDDDIDMaxElements*/	
    	TRUE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_2_ReadCfg, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		NULL_PTR, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		NULL_PTR, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		NULL_PTR, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		NULL_PTR, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		NULL_PTR, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		NULL_PTR, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        2u, /*DcmDspDDDIDMaxElements*/	
    	TRUE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_9_ReadCfg, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_10_ReadCfg, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_11_ReadCfg, /*pDcmDspDidRead*/	
		&Dcm_DidInfo_11_WriteCfg, /*pDcmDspDidWrite*/	
    },
    {
        0u, /*DcmDspDDDIDMaxElements*/	
    	FALSE, /*DcmDspDidDynamicallyDefined*/	
		NULL_PTR, /*pDcmDspDidControl*/	
		&Dcm_DidInfo_12_ReadCfg, /*pDcmDspDidRead*/	
		NULL_PTR, /*pDcmDspDidWrite*/	
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidSignalType,DCM_CONST)Dcm_Did_1111_SignalCfg[1] = 
{
	{
		0u,                   /*DcmDspDidDataPos*/		
		&Dcm_DspDataCfg[0],	 /*pDcmDspDidData*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidSignalType,DCM_CONST)Dcm_Did_F198_SignalCfg[1] = 
{
	{
		0u,                   /*DcmDspDidDataPos*/		
		&Dcm_DspDataCfg[1],	 /*pDcmDspDidData*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidSignalType,DCM_CONST)Dcm_Did_F298_SignalCfg[1] = 
{
	{
		0u,                   /*DcmDspDidDataPos*/		
		&Dcm_DspDataCfg[2],	 /*pDcmDspDidData*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidSignalType,DCM_CONST)Dcm_Did_F286_SignalCfg[1] = 
{
	{
		0u,                   /*DcmDspDidDataPos*/		
		&Dcm_DspDataCfg[3],	 /*pDcmDspDidData*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidSignalType,DCM_CONST)Dcm_Did_F183_SignalCfg[1] = 
{
	{
		0u,                   /*DcmDspDidDataPos*/		
		&Dcm_DspDataCfg[4],	 /*pDcmDspDidData*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidSignalType,DCM_CONST)Dcm_Did_F184_SignalCfg[1] = 
{
	{
		0u,                   /*DcmDspDidDataPos*/		
		&Dcm_DspDataCfg[5],	 /*pDcmDspDidData*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/**********************************************
 *DcmDspDid container configration*************
 **********************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidType,DCM_CONST)Dcm_DspDidCfg[7] =
{
    { /* Did_0x1111 */
        0x1111u,	 /*DcmDspDidId*/	
		TRUE,	 /*DcmDspDidUsed*/	
        0u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        1u, /*DcmDspDidSignalNum*/	
        &Dcm_Did_1111_SignalCfg[0],	 /*pDcmDspDidSignal*/	
    },
    { /* Did_0xF198 */
        0xF198u,	 /*DcmDspDidId*/	
		TRUE,	 /*DcmDspDidUsed*/	
        1u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        1u, /*DcmDspDidSignalNum*/	
        &Dcm_Did_F198_SignalCfg[0],	 /*pDcmDspDidSignal*/	
    },
    { /* Did_0xF298 */
        0xF298u,	 /*DcmDspDidId*/	
		TRUE,	 /*DcmDspDidUsed*/	
        2u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        1u, /*DcmDspDidSignalNum*/	
        &Dcm_Did_F298_SignalCfg[0],	 /*pDcmDspDidSignal*/	
    },
    { /* Did_0x100 */
        0x100u,	 /*DcmDspDidId*/	
		FALSE,	 /*DcmDspDidUsed*/	
        6u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        0u, /*DcmDspDidSignalNum*/	
        NULL_PTR,	 /*pDcmDspDidSignal*/	
    },
    { /* Did_0xF286 */
        0xF286u,	 /*DcmDspDidId*/	
		TRUE,	 /*DcmDspDidUsed*/	
        9u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        1u, /*DcmDspDidSignalNum*/	
        &Dcm_Did_F286_SignalCfg[0],	 /*pDcmDspDidSignal*/	
    },
    { /* Did_0xF183 */
        0xF183u,	 /*DcmDspDidId*/	
		TRUE,	 /*DcmDspDidUsed*/	
        10u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        1u, /*DcmDspDidSignalNum*/	
        &Dcm_Did_F183_SignalCfg[0],	 /*pDcmDspDidSignal*/	
    },
    { /* Did_0xF184 */
        0xF184u,	 /*DcmDspDidId*/	
		TRUE,	 /*DcmDspDidUsed*/	
        11u,	 /*DcmDspDidInfoIndex*/		
        0u,	 /*DcmDspRefDidNum*/	
        NULL_PTR,	 /*pDcmDspRefDidIdArray*/	
        1u, /*DcmDspDidSignalNum*/	
        &Dcm_Did_F184_SignalCfg[0],	 /*pDcmDspDidSignal*/	
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/******************Dsp Did Range**************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspDidRangeType,DCM_CONST)Dcm_DspDidRangeCfg[1] =
{
	{
		TRUE,	 /*DcmDspDidRangeHasGaps*/	
		256u,	 /*DcmDspDidRangeIdentifierLowerLimit*/	
		272u,	 /*DcmDspDidRangeIdentifierUpperLimit*/	
		Dcm_DidRangeIsDidAvailable,	 /*DcmDspDidRangeIsDidAvailableFnc*/		
		Dcm_DidRangeReadDataLengthFnc,	 /*DcmDspDidRangeReadDataLengthFnc*/	
		Dcm_DidRangeReadDidFnc,	 /*DcmDspDidRangeReadDidFnc*/		
		NULL_PTR,	 /*DcmDspDidRangeWriteDidFnc*/		
		80u,	 /*DcmDspDidRangeMaxDataLength*/	
		FALSE,	 /*DcmDspDidRangeUsePort*/	
		12u,	 /*DcmDspDidRangeInfoIndex*/			
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/******************Dsp Memory**************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspAddressAndLengthFormatIdentifierType,DCM_CONST)Dcm_DspAddressAndLengthFormatIdentifierCfg[3] =
{
	{
		68u	 /*DcmDspSupportedAddressAndLengthFormatIdentifier*/	
	},
	{
		66u	 /*DcmDspSupportedAddressAndLengthFormatIdentifier*/	
	},
	{
		65u	 /*DcmDspSupportedAddressAndLengthFormatIdentifier*/	
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspReadMemoryRangeInfoType,DCM_CONST)Dcm_DspReadMemoryRangeInfoCfg[2] =
{
	{
		0x20000000UL, /*DcmDspReadMemoryRangeHigh*/
		0x10000000UL, /*DcmDspReadMemoryRangeLow*/
		0u, /*DcmDspReadMemorySessionLevelRefNum*/	
		NULL_PTR, /*pDcmDspReadMemorySessionLevelRow*/
		0u, /*DcmDspReadMemorySecurityLevelRefNum*/	
		NULL_PTR, /*pDcmDspReadMemorySecurityLevelRow*/
	},
	{
		0x50000UL, /*DcmDspReadMemoryRangeHigh*/
		0x0UL, /*DcmDspReadMemoryRangeLow*/
		0u, /*DcmDspReadMemorySessionLevelRefNum*/	
		NULL_PTR, /*pDcmDspReadMemorySessionLevelRow*/
		0u, /*DcmDspReadMemorySecurityLevelRefNum*/	
		NULL_PTR, /*pDcmDspReadMemorySecurityLevelRow*/
	},
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(uint8,DCM_CONST)Dcm_WriteMemoryRangeInfo_0_SesRefCfg[1]= {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(uint8,DCM_CONST)Dcm_WriteMemoryRangeInfo_0_SecRefCfg[1]= {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(uint8,DCM_CONST)Dcm_WriteMemoryRangeInfo_1_SesRefCfg[1]= {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
		
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(uint8,DCM_CONST)Dcm_WriteMemoryRangeInfo_1_SecRefCfg[1]= {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspWriteMemoryRangeInfoType,DCM_CONST)Dcm_DspWriteMemoryRangeInfoCfg[2] =
{
	{
		0x200000UL, /*DcmDspWriteMemoryRangeHigh*/
		0x100000UL, /*DcmDspWriteMemoryRangeLow*/
		1u, /*DcmDspWriteMemorySessionLevelRefNum*/
		&Dcm_WriteMemoryRangeInfo_0_SesRefCfg[0], /*pDcmDspWriteMemorySessionLevelRow*/
		1u, /*DcmDspWriteMemorySecurityLevelRefNum*/
		&Dcm_WriteMemoryRangeInfo_0_SecRefCfg[0], /*pDcmDspWriteMemorySecurityLevelRow*/
	},
	{
		0x50000UL, /*DcmDspWriteMemoryRangeHigh*/
		0x0UL, /*DcmDspWriteMemoryRangeLow*/
		1u, /*DcmDspWriteMemorySessionLevelRefNum*/
		&Dcm_WriteMemoryRangeInfo_1_SesRefCfg[0], /*pDcmDspWriteMemorySessionLevelRow*/
		1u, /*DcmDspWriteMemorySecurityLevelRefNum*/
		&Dcm_WriteMemoryRangeInfo_1_SecRefCfg[0], /*pDcmDspWriteMemorySecurityLevelRow*/
	},
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspMemoryIdInfoType,DCM_CONST)Dcm_DspMemoryIdInfoCfg[1] =
{
	{
		0u, /*DcmDspMemoryIdValue*/		
		2u,/*DcmDspReadMemoryRangeInfoNum*/
		&Dcm_DspReadMemoryRangeInfoCfg[0], /*DcmDspReadMemoryRangeInfo*/
		2u,/*DcmDspWriteMemoryRangeInfoNum*/
		&Dcm_DspWriteMemoryRangeInfoCfg[0], /*DcmDspWriteMemoryRangeInfo*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspMemoryType,DCM_CONST)Dcm_DspMemoryCfg =	
{
    3u, /*DcmDspAddressAndLengthFormatIdentifierNum*/
	&Dcm_DspAddressAndLengthFormatIdentifierCfg[0], /*DcmDspAddressAndLengthFormatIdentifier*/
	1u,/*DcmDspMemoryIdInfoNum*/
	&Dcm_DspMemoryIdInfoCfg[0], /*DcmDspMemoryIdInfo*/								
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"





/******************Dsp Routine**************/
/***********************************
 *DcmDspRequestRoutineResults container
 **********************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutSignalType,DCM_CONST)Dcm_Routine_0x1234_SignalCfg[6] = 
{
	{
		DCM_OPAQUE,	 /*DcmDspRoutineSignalEndianness*/
		0u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},
	{
		DCM_OPAQUE,	 /*DcmDspRoutineSignalEndianness*/
		8u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_UINT8, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},
	{
		DCM_OPAQUE, /*DcmDspRoutineSignalEndianness*/	
		1u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},
	{
		DCM_OPAQUE, /*DcmDspRoutineSignalEndianness*/	
		1u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},
	{
		DCM_OPAQUE, /*DcmDspRoutineSignalEndianness*/	
		1u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},
	{
		DCM_OPAQUE, /*DcmDspRoutineSignalEndianness*/	
		1u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	}	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutSignalType,DCM_CONST)Dcm_Routine_0x203_SignalCfg[2] = 
{
	{
		DCM_OPAQUE, /*DcmDspRoutineSignalEndianness*/	
		0u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},
	{
		DCM_OPAQUE, /*DcmDspRoutineSignalEndianness*/	
		1u, /*DcmDspRoutineSignalLength*/	
		0u, /*DcmDspRoutineSignalPos*/
		DCM_BOOLEAN, /*DcmDspRoutineSignalType*/	
		NULL_PTR, /*DcmDspArgumentScaling*/										
	},	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
/***********************************
 *DcmDspRequestRoutineResults container
 **********************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)Dcm_Routine_0x1234_RequestRoutineResultsInCfg =
{	
	1u,                                  /*RoutineInSignalNum*/	
	&Dcm_Routine_0x1234_SignalCfg[0],	/*DcmDspRoutineInSignal*/	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)Dcm_Routine_0x1234_RequestRoutineResultsOutCfg =
{	
	1u,                                  /*RoutineOutSignalNum*/	
	&Dcm_Routine_0x1234_SignalCfg[1],	/*DcmDspRoutineOutSignal*/	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRequestRoutineResultsType,DCM_CONST)Dcm_RoutineInfo_0x1234_ResultsCfg =
{
	Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_RequestResults, /*DcmDspRequestResultsRoutineFnc*/	
	NULL_PTR, /*DcmDspRequestRoutineResultsCommonAuthorizationRef*/	
	&Dcm_Routine_0x1234_RequestRoutineResultsInCfg, /*DcmDspRequestRoutineResultsIn*/	
	&Dcm_Routine_0x1234_RequestRoutineResultsOutCfg, /*DcmDspRequestRoutineResultsOut*/	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
/***********************************
 *DcmDspRoutineStart container
 **********************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)Dcm_Routine_0x1234_StartRoutineInCfg =
{	
	1u,                                  /*RoutineInOutSignalNum*/	
	&Dcm_Routine_0x1234_SignalCfg[2],	/*DcmDspRoutineInOutSignal*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)Dcm_Routine_0x1234_StartRoutineOutCfg = 
{	
	1u,                                  /*RoutineInOutSignalNum*/	
	&Dcm_Routine_0x1234_SignalCfg[3],	/*DcmDspRoutineInOutSignal*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspStartRoutineType,DCM_CONST)Dcm_RoutineInfo_0x1234_StartCfg =
{
	 
     Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_Start, /*DcmDspStartRoutineFnc*/
     NULL_PTR, /*DcmDspStartRoutineCommonAuthorizationRef*/
     &Dcm_Routine_0x1234_StartRoutineInCfg, /*DcmDspStartRoutineIn*/
     &Dcm_Routine_0x1234_StartRoutineOutCfg, /*DcmDspStartRoutineOut*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)Dcm_Routine_0x203_StartRoutineInCfg =
{	
	1u,                                  /*RoutineInOutSignalNum*/	
	&Dcm_Routine_0x203_SignalCfg[0],	/*DcmDspRoutineInOutSignal*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)Dcm_Routine_0x203_StartRoutineOutCfg = 
{	
	1u,                                  /*RoutineInOutSignalNum*/	
	&Dcm_Routine_0x203_SignalCfg[1],	/*DcmDspRoutineInOutSignal*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspStartRoutineType,DCM_CONST)Dcm_RoutineInfo_0x203_StartCfg =
{
	 
     Rte_Call_Dcm_RoutineServices_DcmDspRoutine_1_Start, /*DcmDspStartRoutineFnc*/
     NULL_PTR, /*DcmDspStartRoutineCommonAuthorizationRef*/
     &Dcm_Routine_0x203_StartRoutineInCfg, /*DcmDspStartRoutineIn*/
     &Dcm_Routine_0x203_StartRoutineOutCfg, /*DcmDspStartRoutineOut*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/***********************************
 *DcmDspRoutineStop container
 **********************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)	Dcm_Routine_0x1234_StopRoutineInCfg = 
{	
	1u,                                  /*RoutineInOutSignalNum*/	
	&Dcm_Routine_0x1234_SignalCfg[4],	/*DcmDspRoutineInOutSignal*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineInOutType,DCM_CONST)	Dcm_Routine_0x1234_StopRoutineOutCfg =
{	
	1u,	
	&Dcm_Routine_0x1234_SignalCfg[5],
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspStopRoutineType,DCM_CONST)Dcm_RoutineInfo_0x1234_StopCfg =
{
	Rte_Call_Dcm_RoutineServices_DcmDspRoutine_0_Stop, /*DcmDspRoutineStopFnc*/
	NULL_PTR, /*DcmDspStopRoutineCommonAuthorizationRef*/
    &Dcm_Routine_0x1234_StopRoutineInCfg, /*DcmDspStopRoutineIn*/
    &Dcm_Routine_0x1234_StopRoutineOutCfg, /*DcmDspStopRoutineIn*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
/***********************************
 *DcmDspRoutine container configration
 **********************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoutineType,DCM_CONST)Dcm_DspRoutineCfg[2] =
{
    { /* DcmDspRoutine_0 */
        0x1234u, /*DcmDspRoutineId*/
		0u, /*DcmDspRoutineInfoByte*/ 
		TRUE, /*DcmDspRoutineUsePort*/	
		TRUE,	 /*DcmDspRoutineUsed*/
		&Dcm_DspCommonAuthorizationCfg[0],	 /*DcmDspCommonAuthorizationRef*/	
		&Dcm_RoutineInfo_0x1234_ResultsCfg, /*DcmDspStopRoutineIn*/					
		&Dcm_RoutineInfo_0x1234_StartCfg, /*DcmDspRequestRoutineResults*/
		&Dcm_RoutineInfo_0x1234_StopCfg, /*DcmDspStopRoutine*/			
    },
    { /* DcmDspRoutine_1 */
        0x203u, /*DcmDspRoutineId*/
		0u, /*DcmDspRoutineInfoByte*/ 
		TRUE, /*DcmDspRoutineUsePort*/	
		TRUE,	 /*DcmDspRoutineUsed*/
		&Dcm_DspCommonAuthorizationCfg[0],	 /*DcmDspCommonAuthorizationRef*/	
		NULL_PTR, /*DcmDspStopRoutineIn*/					
		&Dcm_RoutineInfo_0x203_StartCfg, /*DcmDspRequestRoutineResults*/
		NULL_PTR, /*DcmDspStopRoutine*/			
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
/******************Dsp Security Row**************/
/************************************************
 ****DcmDspSecurityRow container(Multiplicity=0..31)****
 ************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspSecurityRowType,DCM_CONST)Dcm_DspSecurityRow[2] =
{
    { /* DcmDspSecurityRow_0 */	
        1u,      	/*DcmDspSecurityLevel*/			
        4u,      	/*DcmDspSecuritySeedSize*/		
        4u,      	/*DcmDspSecurityKeySize*/		
        0u,      	/*DcmDspSecurityADRSize*/	
		TRUE,		/*DcmDspSecurityAttemptCounterEnabled*/
        3u,    /*DcmDspSecurityNumAttDelay*/	
        10000u,  /*DcmDspSecurityDelayTime,10s */			
        10000u,/*DcmDspSecurityDelayTimeOnBoot*/				
        Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_GetSeed,	/*Dcm_GetSeedFnc*/	
		Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_CompareKey,	/*Dcm_CompareKeyFnc*/
		Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_GetSecurityAttemptCounter,	/*Dcm_GetSecurityAttemptCounterFnc*/
		Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_0_SetSecurityAttemptCounter,	/*DcmDspSecurityUsePort*/
		USE_ASYNCH_FNC,	/*DcmDspSecurityUsePort*/
    },
    { /* DcmDspSecurityRow_1 */	
        3u,      	/*DcmDspSecurityLevel*/			
        4u,      	/*DcmDspSecuritySeedSize*/		
        4u,      	/*DcmDspSecurityKeySize*/		
        0u,      	/*DcmDspSecurityADRSize*/	
		FALSE,		/*DcmDspSecurityAttemptCounterEnabled*/
        3u,    /*DcmDspSecurityNumAttDelay*/	
        10000u,  /*DcmDspSecurityDelayTime,10s */			
        10000u,/*DcmDspSecurityDelayTimeOnBoot*/				
        Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_1_GetSeed,	/*Dcm_GetSeedFnc*/	
		Rte_Call_Dcm_SecurityAccess_DcmDspSecurityRow_1_CompareKey,	/*Dcm_CompareKeyFnc*/
		NULL_PTR,	/*Dcm_GetSecurityAttemptCounterFnc*/
		NULL_PTR,	/*DcmDspSecurityUsePort*/
		USE_ASYNCH_FNC,	/*DcmDspSecurityUsePort*/
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/************************************************
 ****DcmDspSecurity container(Multiplicity=1)****
 ************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspSecurityType,DCM_CONST)Dcm_DspSecurity =
{
    &Dcm_DspSecurityRow[0],	/*pDcm_DspSecurityRow*/								
    2u,	/*DcmDspSecurityRow_Num*/	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/******************Dsp Session Row**************/
/************************************************
 ****DcmDspSessionRow container(Multiplicity=0..31)
 ************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspSessionRowType,DCM_CONST)Dcm_DspSessionRow[3] =
{
    { /* DcmDspSessionRow_0 */
        DCM_NO_BOOT,	/*DcmDspSessionForBoot*/
    	1u,	/*DcmDspSessionLevel*/
        50u,	/*DcmDspSessionP2ServerMax*/	
        5000u,	/*DcmDspSessionP2StarServerMax*/	
    },
    { /* DcmDspSessionRow_1 */
        DCM_SYS_BOOT,	/*DcmDspSessionForBoot*/
    	2u,	/*DcmDspSessionLevel*/
        50u,	/*DcmDspSessionP2ServerMax*/	
        5000u,	/*DcmDspSessionP2StarServerMax*/	
    },
    { /* DcmDspSessionRow_2 */
        DCM_NO_BOOT,	/*DcmDspSessionForBoot*/
    	3u,	/*DcmDspSessionLevel*/
        50u,	/*DcmDspSessionP2ServerMax*/	
        5000u,	/*DcmDspSessionP2StarServerMax*/	
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/************************************************
 *******Dcm_DspSession container(Multiplicity=1)*
 ************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspSessionType,DCM_CONST)Dcm_DspSession =
{
    &Dcm_DspSessionRow[0],	/*pDcmDspSessionRow*/								
    3u,		/*DcmDspSessionRow_Num*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspPeriodicTransmissionTypes,DCM_CONST)Dcm_DspPeriodicTransmission =
{
    10UL,/*DcmDspPeriodicTransmissionFastRate*/
    20UL,/*DcmDspPeriodicTransmissionMediumRate*/
    30UL,/*DcmDspPeriodicTransmissionSlowRate*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*****************************************************
 ****************ROE container configration********
 ****************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoeOnChangeOfDataIdentifierType,DCM_CONST)DcmDspRoeOnChangeOfDataIdentifier[1] =
{
	{
		0x1111,/*Did value*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoeOnDTCStatusChangeType,DCM_CONST)DcmDspRoeOnDTCStatusChange[1] =
{
	{
		0xFF,
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoeEventPropertiesType,DCM_CONST)DcmDspRoeEventProperties[DCM_DSP_ROE_NUM] =
{
	{
		NULL_PTR,/*DcmDspRoeOnChangeOfDataIdentifier*/
		&DcmDspRoeOnDTCStatusChange[0],/*DcmDspRoeOnDTCStatusChange*/
	},
	{
		&DcmDspRoeOnChangeOfDataIdentifier[0],/*DcmDspRoeOnChangeOfDataIdentifier*/
		NULL_PTR,/*DcmDspRoeOnDTCStatusChange*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoeEventWindowTimeTypes,DCM_CONST)DcmDspRoeEventWindowTime[3] =
{
	{
		DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE
	},
	{
		DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE
	},
	{
		DCM_ROE_EVENT_WINDOW_INFINITE
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoeEventType,DCM_CONST)DcmDspRoeEvent[2] =
{
	{
		0x0,/*DcmDspRoeEventId*/
		DCM_ROE_STOPPED,/*DcmDspRoeInitialEventStatus*/
		&DcmDspRoeEventProperties[0],/*DcmDspRoeEventProperties*/
	},
	{
		0x1,/*DcmDspRoeEventId*/
		DCM_ROE_STOPPED,/*DcmDspRoeInitialEventStatus*/
		&DcmDspRoeEventProperties[1],/*DcmDspRoeEventProperties*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static CONST(Dcm_DspRoeTypes,DCM_CONST)Dcm_DspRoe =
{
	10,/*DcmDspRoeInterMessageTime*/
	2,/*DcmDspRoeEventNum*/
    DcmDspRoeEvent,/*DcmDspRoeEvent*/
    3,/*DcmDspRoeEventWindowTimeNum*/
    DcmDspRoeEventWindowTime,/*DcmDspRoeEventWindowTime*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
/*****************************************************
 ****************DcmDsp container configration********
 ****************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DspCfgType,DCM_CONST) Dcm_DspCfg =
{
	&DcmDspDIDcheckPerSourceDID,	/*DcmDspDDDIDcheckPerSourceDID*/
	DCM_BIG_ENDIAN,	/*DcmDspDataDefaultEndianness*/								
	TRUE,/*DcmDspEnableObdMirror*/			
    10,/*DcmDspMaxDidToRead*/								
    DCM_DSP_MAX_PERIODIC_DID_TO_READ,/*DcmDspMaxPeriodicDidToRead*/						
	0u,/*DcmDspPowerDownTime*/								
	AFTER_RESET,	/*DcmResponseToEcuReset*/	

	&Dcm_DspClearDTCCfg,		/*pDcmDspClearDTC*/	
	&Dcm_DspComControlCfg,		/*pDcmDspComControl*/	
	&Dcm_DspCommonAuthorizationCfg[0],		/*pDcmDspCommonAuthorization*/	
	&Dcm_DspControlDTCSettingCfg,	/*pDcmDspControlDTCSetting*/	
	
	&Dcm_DspDataCfg[0],	/*pDcmDspData*/	
	
	&Dcm_DspDataInfoCfg[0],	/*pDcmDspDataInfo*/	
	
	
	7u,	/*DcmDspDidNum*/	
    &Dcm_DspDidCfg[0],		/*pDcmDspDid*/									
	13u,	/*DcmDspDidInfoNum*/	
    &Dcm_DspDidInfoCfg[0],		/*pDcmDspDidInfo*/								
	1u,	/*DcmDspDidRangeNum*/	
	&Dcm_DspDidRangeCfg[0],		/*pDcmDspDidRange*/								
	
	&Dcm_DspMemoryCfg, /*pDcmDspMemory*/
	
    NULL_PTR,	/*DcmDspRequestFileTransfer*/	
    
    2u,		/*DcmDspRoutineNum*/	
    &Dcm_DspRoutineCfg[0],	/*pDcmDspRoutine*/									
   
    &Dcm_DspSecurity,  /* pDcm_DspSecurity */				
    &Dcm_DspSession,  /* pDcm_DspSession */	
   					
    
    DCM_DSP_MAX_PERIODIC_DID_SCHEDULER,
    &Dcm_DspPeriodicTransmission,
    &Dcm_DspRoe
}; 
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*****************************************************************************************
 ********************************* DSD container configration*****************************
 *****************************************************************************************/

/*DcmDsdService_0x10 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x10_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x10[3] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		NULL_PTR,	/*DcmDsdSubServiceSessionLevelRef*/	
		0u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x10_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x3u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		NULL_PTR,	/*DcmDsdSubServiceSessionLevelRef*/	
		0u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x11 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x11_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x11[1] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x11_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x19 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_4_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_6_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_A_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_E_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x19[6] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x19_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x19_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x4u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x19_4_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x6u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x19_6_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0xAu,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x19_A_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0xEu,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x19_E_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x27 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x27_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x27_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x27_5_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x27_6_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x27[4] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x27_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x27_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x5u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x27_5_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x6u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x27_6_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x28 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x28_0_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x28_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x28_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x28_3_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x28[4] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x0u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x28_0_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x28_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x28_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x3u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x28_3_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x2C SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2C_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2C_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2C_3_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x2C[3] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x2C_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x2C_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x3u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x2C_3_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x31 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x31_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x31_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x31_3_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x31[3] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x31_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x31_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x3u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x31_3_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x3E SubService*/

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x3E[1] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x0u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		NULL_PTR,	/*DcmDsdSubServiceSessionLevelRef*/	
		0u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x85 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x85_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x85_2_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x85[2] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x85_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x2u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x85_2_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*DcmDsdService_0x86 SubService*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_0_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_1_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_3_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_4_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_5_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_6_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdSubServiceCfgType,DCM_CONST)Dcm_UDS_DsdSubService_UDS0x86[6] =	
{
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x0u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x86_0_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x1u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x86_1_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x3u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x86_3_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x4u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x86_4_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x5u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x86_5_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	},
	{
		NULL_PTR,	/*DcmDsdSubServiceFnc*/	
		0x6u,	/*DcmDsdSubServiceId*/	
		TRUE,	/*DcmDsdSubServiceUsed*/	
		NULL_PTR,	/*DcmDsdSubServiceModeRuleRef*/	
		NULL_PTR,	/*DcmDsdSubServiceSecurityLevelRef*/	
		0u,	/*DcmDsdSubServiceSecurityLevel_Num*/		
		&Dcm_UDS_UDS0x86_6_SesRef[0],	/*DcmDsdSubServiceSessionLevelRef*/	
		1u	/*DcmDsdSubServiceSessionLevel_Num*/		
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/*UDS Service session and security configuration*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x11_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x14_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x19_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x23_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x27_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x28_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2A_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2C_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2E_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x2F_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x31_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x34_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x34_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x35_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x35_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x36_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x36_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x37_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x37_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x38_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x38_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x3D_SecRef[1] = {1u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x3D_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x85_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(uint8,DCM_CONST)Dcm_UDS_UDS0x86_SesRef[1] = {3u};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
/*DcmDsdService UDS*/	
static  CONST(Dcm_DsdServiceCfgType,DCM_CONST)UDS_Service[23] =
{
	{ /*DiagnosticSessionControl*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x10,	/*DcmDsdSidTabFnc*/
		0x10u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		TRUE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		0u,	/*DcmDsdSessionLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSessionLevelRef*/
		3u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x10[0],	/*DcmDsdSubService*/
	},
	{ /*ECUReset*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x11,	/*DcmDsdSidTabFnc*/
		0x11u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		TRUE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x11_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		1u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x11[0],	/*DcmDsdSubService*/
	},
	{ /*ClearDiagnosticInformation*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x14,	/*DcmDsdSidTabFnc*/
		0x14u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x14_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*ReadDTCInformation*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x19,	/*DcmDsdSidTabFnc*/
		0x19u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x19_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		6u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x19[0],	/*DcmDsdSubService*/
	},
	{ /*ReadDataByIdentifier*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x22,	/*DcmDsdSidTabFnc*/
		0x22u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		0u,	/*DcmDsdSessionLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*ReadMemoryByAddress*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x23,	/*DcmDsdSidTabFnc*/
		0x23u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x23_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*ReadScalingDataByIdentifier*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x24,	/*DcmDsdSidTabFnc*/
		0x24u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		0u,	/*DcmDsdSessionLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*SecurityAccess*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x27,	/*DcmDsdSidTabFnc*/
		0x27u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x27_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		4u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x27[0],	/*DcmDsdSubService*/
	},
	{ /*CommunicationControl*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x28,	/*DcmDsdSidTabFnc*/
		0x28u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		TRUE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x28_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		4u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x28[0],	/*DcmDsdSubService*/
	},
	{ /*ReadDataByPeriodicIdentifier*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x2A,	/*DcmDsdSidTabFnc*/
		0x2Au,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x2A_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*DynamicallyDefineDataIdentifier*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x2C,	/*DcmDsdSidTabFnc*/
		0x2Cu,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x2C_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		3u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x2C[0],	/*DcmDsdSubService*/
	},
	{ /*WriteDataByIdentifier*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x2E,	/*DcmDsdSidTabFnc*/
		0x2Eu,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x2E_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*InputOutputControlByIdentifier*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x2F,	/*DcmDsdSidTabFnc*/
		0x2Fu,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x2F_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*RoutineControl*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x31,	/*DcmDsdSidTabFnc*/
		0x31u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x31_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		3u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x31[0],	/*DcmDsdSubService*/
	},
	{ /*RequestDownload*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x34,	/*DcmDsdSidTabFnc*/
		0x34u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		1u, /*DcmDsdSecurityLevel_Num*/	
		&Dcm_UDS_UDS0x34_SecRef[0],	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x34_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*RequestUpload*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x35,	/*DcmDsdSidTabFnc*/
		0x35u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		1u, /*DcmDsdSecurityLevel_Num*/	
		&Dcm_UDS_UDS0x35_SecRef[0],	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x35_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*TransferData*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x36,	/*DcmDsdSidTabFnc*/
		0x36u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		1u, /*DcmDsdSecurityLevel_Num*/	
		&Dcm_UDS_UDS0x36_SecRef[0],	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x36_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*RequestTransferExit*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x37,	/*DcmDsdSidTabFnc*/
		0x37u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		1u, /*DcmDsdSecurityLevel_Num*/	
		&Dcm_UDS_UDS0x37_SecRef[0],	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x37_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*RequestFileTransfer*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x38,	/*DcmDsdSidTabFnc*/
		0x38u,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		1u, /*DcmDsdSecurityLevel_Num*/	
		&Dcm_UDS_UDS0x38_SecRef[0],	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x38_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*WriteMemoryByAddress*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x3D,	/*DcmDsdSidTabFnc*/
		0x3Du,	/*DcmDsdServiceId*/
		FALSE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYSICAL, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		1u, /*DcmDsdSecurityLevel_Num*/	
		&Dcm_UDS_UDS0x3D_SecRef[0],	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x3D_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		0u,	/*DcmDsdSubService_Num*/
		NULL_PTR,	/*DcmDsdSubService*/
	},
	{ /*TesterPresent*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x3E,	/*DcmDsdSidTabFnc*/
		0x3Eu,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		TRUE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		0u,	/*DcmDsdSessionLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSessionLevelRef*/
		1u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x3E[0],	/*DcmDsdSubService*/
	},
	{ /*ControlDTCSetting*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x85,	/*DcmDsdSidTabFnc*/
		0x85u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		TRUE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x85_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		2u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x85[0],	/*DcmDsdSubService*/
	},
	{ /*ResponseOnEvent*/
		TRUE,	/*DcmDsdServiceUsed*/
		Dcm_UDS0x86,	/*DcmDsdSidTabFnc*/
		0x86u,	/*DcmDsdServiceId*/
		TRUE,	/*DcmDsdSubfuncAvial*/
		FALSE,  /*DcmDsdSuppressPosRsp*/
		DCM_PHYANDFUNC, /*DcmDsdSidTabAddressingFormat*/
		NULL_PTR,	/*DcmDsdModeRuleRef*/	
		0u, /*DcmDsdSecurityLevel_Num*/	
		NULL_PTR,	/*pDcmDsdSecurityLevelRef*/
		1u,	/*DcmDsdSessionLevel_Num*/	
		&Dcm_UDS_UDS0x86_SesRef[0],	/*pDcmDsdSessionLevelRef*/
		6u,	/*DcmDsdSubService_Num*/
		&Dcm_UDS_DsdSubService_UDS0x86[0],	/*DcmDsdSubService*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


/**********************************************************************/
/*DCM Support Service Table(Multiplicity=1..256)*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DsdServiceTableCfgType,DCM_CONST)Dcm_DsdServiceTable[DCM_SERVICE_TAB_NUM]=
{
    {
        0x0u,	/*DcmDsdSidTabId*/
        &UDS_Service[0],	/*pDcmDsdService*/
        23u	/*DcmDsdSidTab_ServiceNum*/
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/**********************************************************************/

/**********************************************************************/
/*Dsd container(Multiplicity=1)*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DsdCfgType,DCM_CONST)Dcm_DsdCfg =
{
		/*DcmDsdServiceReqManufacturerNoti_PortNum*/
		/*DcmDsdServiceReqSupplierNoti_PortNum*/
    &Dcm_DsdServiceTable[0],	/*pDcmDsdServiceTable*/	
    DCM_SERVICE_TAB_NUM		/*DcmDsdServiceTable_Num*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*****************************************************************************************
 ********************************* DSL container configration*****************************
 *****************************************************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
/*DcmDslBuffer container(Multiplicity=1..256)*/
static  CONST(Dcm_DslBufferType,DCM_CONST)Dcm_DslBufferCfg[DCM_CHANNEL_NUM] =
{	
    {/* DcmDslBuffer_RX_UDS*/
    	0x0u,	/*Dcm_DslBufferId*/
		1024u,	/*Dcm_DslBufferSize*/
		0u	/*offset*/
    },
    {/* DcmDslBuffer_TX_UDS*/
    	0x1u,	/*Dcm_DslBufferId*/
		1024u,	/*Dcm_DslBufferSize*/
		1024u	/*offset*/
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/***********************************/
/*DcmDslDiagResp container(Multiplicity=1)*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DslDiagRespType,DCM_CONST)Dcm_DslDiagRespCfg =
{	
	TRUE,		/*DcmDslDiagRespOnSecondDeclinedRequest*/
    10u		/*DcmDslDiagRespMaxNumRespPend*/	
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*****************************************************
 *DcmDslCallbackDCMRequestService port configuration(Multiplicity=1..*)
 *****************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DslCallbackDCMRequestServiceType,DCM_CONST)Dcm_DslCallbackDCMRequestServiceCfg[1] =	
{	
	{
		DcmDslCallbackDCMRequestService_0_StartProtocol,	/*StartProtocol*/
		DcmDslCallbackDCMRequestService_0_StopProtocol,	/*StopProtocol*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/********************UDS protocol Connection configuration*******************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DslProtocolRxType,DCM_CONST)Dsl_Protocol_Connection_RxCfg[DCM_DSL_RX_ID_NUM]=
{
	{
    	0x0u,      		/*DcmDslParentConnectionCtrlId*/
        DCM_PHYSICAL,	/*DcmDslProtocolRxAddrType*/
        DCM_DCM_UDS_RX_PHY,       /*DcmDslProtocolRxPduId*/	
		
    },
	{
    	0x0u,      		/*DcmDslParentConnectionCtrlId*/
        DCM_FUNCTIONAL,	/*DcmDslProtocolRxAddrType*/
        DCM_DCM_UDS_RX_FUN,       /*DcmDslProtocolRxPduId*/	
		
    },
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DslProtocolTxType,DCM_CONST)Dsl_Protocol_Connection_TxCfg[DCM_DSL_TX_ID_NUM]=
{
	{
    	0x0u,			/*parent connection id*/
        DCM_DCM_UDS_TX,       /*DcmDslProtocolTxPduId*/
		DCM_PDUR_DCM_UDS_TX,	/*DcmDslProtocolTx Pdu Id of PduR*/
    },
	
	{
    	0x0u,			/*parent connection id*/
        DCM_DCM_2A,       /*DcmDslProtocolTxPduId*/
		DCM_PDUR_DCM_2A,	/*DcmDslProtocolTx Pdu Id of PduR*/
    },
		
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
static  CONST(Dcm_DslPeriodicConnectionType,DCM_CONST)Dcm_DslPeriodicConnection0[1] =
{
    {DCM_DCM_2A, DCM_PDUR_DCM_2A},
};
static  CONST(Dcm_DslPeriodicTransmissionType,DCM_CONST)Dcm_DslPeriodicTransmission[DCM_PERIODICCONNECTION_NUM] =
{
    {
        Dcm_DslPeriodicConnection0,/*DcmDslPeriodicTxConfirmationPduId*/
        1u,/*DcmPeriodicTxPduIdNum*/
    },
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*Connection1,Mainconnection,ProtocolTx configration(Multiplicity=1..*)*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DslMainConnectionType,DCM_CONST) Dsl_Protocol_MainConnectionCfg[DCM_MAINCONNECTION_NUM] =
{
	{
		0x00000000u,							/*DcmDslProtocolRxTesterSourceAddr*/
		&Dcm_DslPeriodicTransmission[0],  /*pDcmDslPeriodicTranmissionConRef*/	
		0u, 									/*DcmDslProtocolComMChannelId*/	
		&Dsl_Protocol_Connection_TxCfg[0],  /*pDcmDslROEConnectionRef*/	
		&Dsl_Protocol_Connection_RxCfg[0],	/*pDcmDslProtocolRx*/
		2u,                   				/*DcmDslProtocolRx_Num*/
		&Dsl_Protocol_Connection_TxCfg[0],  /*pDcmDslProtocolTx*/	
		1u									/*DcmDslProtocolTx_Num*/
	}
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*Connection1 configration*/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DslConnectionType,DCM_CONST)Dsl_Protocol_ConnectionCfg[DCM_CONNECTION_NUM]=
{
    {
    	0x0u,        						/*parent protocolRow id*/
        &Dsl_Protocol_MainConnectionCfg[0],	/*pDcmDslMainConnection*/
        NULL_PTR,         					/*pDcmDslPeriodicTransmission*/
        NULL_PTR          					/*pDcmDslResponseOnEvent*/
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
/*****************************************************
 ****Dcm_DslProtocolRow container configration(Multiplicity=1..*)*******
 ****************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_DslProtocolRowType,DCM_CONST)Dsl_ProtocolRowCfg[DCM_DSLPROTOCOLROW_NUM_MAX] =
{	
    {
        DCM_UDS_ON_CAN,		/*DcmDslProtocolID*/
        0x1u,					/*This parameter is mandatory and defines the maximum length of the response message in case DcmPagedBufferEnabled == TRUE*/
        10u,					/*DcmDslProtocolPreemptTimes*/
        2u,					/*DcmDslProtocolPriority*/
		TRUE,				/*true-protocol is available*/
		DCM_PROTOCAL_TRAN_NOT_VALID,
		TRUE,				/*True-send 0x78 before transitioning to the bootloader */
		0u,	 				/*DcmTimStrP2ServerAdjust*/	
		0u,				/*DcmTimStrP2StarServerAdjust*/	
		&Dcm_DslBufferCfg[0],/*DcmDslProtocolRxBuffer*/
        &Dcm_DslBufferCfg[1],/*DcmDslProtocolTxBuffer*/
        0u, 				/*DcmDslServiceTableId*/
        &Dsl_Protocol_ConnectionCfg[0] /*DcmDslConnection*/
    }
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*****************************************************
 *DcmDslProtocol container configration(Multiplicity=1)
 ****************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h"
CONST(Dcm_DslProtocolType,DCM_CONST)Dcm_DslProtocol =
{
    &Dsl_ProtocolRowCfg[0],	/*pDcmDslProtocolRow*/
    DCM_DSLPROTOCOLROW_NUM_MAX,	/*DcmDslProtocolRow_Num*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/*****************************************************
 ****************DcmDsl container configration*****
 ****************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h"
CONST(Dcm_DslCfgType,DCM_CONST)Dcm_DslCfg =
{
	DCM_CHANNEL_NUM,		/*Number of Channel configration*/					
    &Dcm_DslBufferCfg[0],	/*DcmDslBuffer*/									
	1u,						/*Number of DslCallbackDCMRequestService port*/
    &Dcm_DslCallbackDCMRequestServiceCfg[0],	/*pDcmDslCallback_DCMRequestService*/									
    &Dcm_DslDiagRespCfg,   	/*reference to DcmDslDiagResp configration*/		
    &Dcm_DslProtocol,		/*reference to DcmDslProtocol configration*/		
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h"
CONST(Dcm_CfgType,DCM_CONST)Dcm_Cfg =
{
	&Dcm_DslCfg,	/*pDcmDslCfg*/
	&Dcm_DsdCfg,	/*pDcmDsdCfg*/
	&Dcm_DspCfg,	/*pDcmDspCfg*/
	NULL_PTR,	    /*pDcmPageBufferCfg*/
	NULL_PTR,	    /*pDcmProcessingConditionsCfg*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

/**********************************************************************
 ***********************DcmGeneral Container***************************
 **********************************************************************/
#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "Dcm_MemMap.h" 
CONST(Dcm_GeneralCfgType,DCM_CONST)Dcm_GeneralCfg =
{
    FALSE, /*DcmDDDIDStorage*/
	DCM_DEV_ERROR_DETECT, /*DcmDevErrorDetect*/			
	NULL_PTR, /*DcmHeaderFileInclusion*/							
	DCM_RESPOND_ALL_REQUEST, /*DcmRespondAllRequest*/			
	DCM_VERSION_INFO_API, /*DcmVersionInfoApi*/				
	1, /*DcmTaskTime*/
	NULL_PTR, /*DcmVinRef*/
};
#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
