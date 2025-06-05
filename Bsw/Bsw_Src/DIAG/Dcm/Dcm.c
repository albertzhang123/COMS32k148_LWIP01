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
**  FILENAME    :  Dcm.c                                                     **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Implementation of Dcm                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm.h"
#include "UDS.h"
#if(DCM_BSWM_ENABLE == STD_ON)
#include "BswM_Dcm.h"
#endif
#include "ComM_Dcm.h"
/*============================================================================*/
/*=======[R E V I S I O N   H I S T O R Y]====================================*
*  <VERSION>    <DATE>       <AUTHOR>    <REVISION LOG>
*  V1.0.0       2018-3-20    shushi      Initial version
*  V1.0.1       2019-12-24   tao.yu      QAC check fix
*  V1.0.2       2020-1-7     tao.yu      Commercial project problem modification
*  V2.0.0       2021-8-25    tao.yu      release version
============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define  DCM_C_AR_MAJOR_VERSION   (0x04u)
#define  DCM_C_AR_MINOR_VERSION   (0x02u)
#define  DCM_C_AR_PATCH_VERSION   (0x02u)
#define  DCM_C_SW_MAJOR_VERSION   (0x02u)    /*Major Version*/
#define  DCM_C_SW_MINOR_VERSION   (0x00u)    /*Minor Version*/
#define  DCM_C_SW_PATCH_VERSION   (0x00u)    /*Patch version*/
/*=======[V E R S I O N  C H E C K]===========================================*/
#if (DCM_C_AR_MAJOR_VERSION != DCM_H_AR_MAJOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_AR_MINOR_VERSION != DCM_H_AR_MINOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_AR_PATCH_VERSION != DCM_H_AR_PATCH_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_SW_MAJOR_VERSION != DCM_H_SW_MAJOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_SW_MINOR_VERSION != DCM_H_SW_MINOR_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DCM_C_SW_PATCH_VERSION != DCM_H_SW_PATCH_VERSION)
  #error "Dcm.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef struct
{
    uint8 Dcm_VIN[17];
    boolean Flag;
}Dcm_VINType;

#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
static VAR(Dcm_VINType,DCM_VAR_POWER_ON_INIT)Dcm_VIN;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_PendingManage(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_P2ServerTimer(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_S3Timer(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_SecTimer(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if(DCM_PAGEDBUFFER_ENABLED == STD_ON)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_PageBufferTimer(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_GetUDSPhyPduID(P2VAR(PduIdType,AUTOMATIC,DCM_VAR)rxPhyPduId);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_BootloaderResponse(void);
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/



/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_MkCtrlType, DCM_VAR_NOINIT) Dcm_MkCtrl;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
VAR(boolean, DCM_VAR_POWER_ON_INIT) gAppl_UpdataOK_ResponseFlag = FALSE;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
VAR(boolean, DCM_VAR_POWER_ON_INIT) ReProgramingRequest = FALSE;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_ProgConditionsType, DCM_VAR_NOINIT) ProgConditions;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#if(STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_PageBufferDataType,DCM_VAR_NOINIT)Dcm_PageBufferData;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif

#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(Dcm_DspProgramType, DCM_VAR_NOINIT) Dcm_DspProgram;
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#if(STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
#define  DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
VAR(SchedulerQueueTransmitTypes,DCM_VAR_NOINIT)
Scheduler_0x2A_Transmit[DCM_PERIODICCONNECTION_NUM];
#define  DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif
/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#if(STD_ON == DCM_PAGEDBUFFER_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void,DCM_CODE)DslInternal_InitPageBuffer(void)
{
    SchM_Enter_Dcm(Dcm_PageBufferData);
    Dcm_PageBufferData.AlreadyPageSize = 0u;
    Dcm_PageBufferData.IloopOne = 0u;
    Dcm_PageBufferData.TotalDtcCount = 0u;
    Dcm_PageBufferData.TotalSize = 0UL;
    Dcm_PageBufferData.Filled = FALSE;
    Dcm_PageBufferData.PageTxOK = FALSE;
    Dcm_PageBufferData.PageIndex = 0u;
    Dcm_PageBufferData.LastFilled = TRUE;
    Dcm_PageBufferData.ThisPageSize = 0u;
    Dcm_PageBufferData.ReqOffset = 0u;
    Dcm_PageBufferData.LastFilledSize = 0u;
    Dcm_PageBufferData.ThisPageTxSize = 0u;
    Dcm_PageBufferData.LastNotTxDataSize = 0u;
    Dcm_PageBufferData.TimerStart = FALSE;
    Dcm_PageBufferData.TimeOut = FALSE;
    Dcm_PageBufferData.CurTimer = 0UL;
    Dcm_PageBufferData.ExpiredTimer = 0UL;
    SchM_Exit_Dcm(Dcm_PageBufferData);
}
#define DCM_STOP_SEC_CODE
#include  "Dcm_MemMap.h"
#endif

#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void,DCM_CODE)DslInternal_InitDspProgramInfo(void)
{
    SchM_Enter_Dcm(Dcm_DspProgram);
    Dcm_DspProgram.address = 0UL;
    Dcm_DspProgram.blockId = 0x00;
    Dcm_DspProgram.reqBlockId = 0x00;
    Dcm_DspProgram.Status = DCM_UDS0X36_INIT;
    Dcm_DspProgram.MemoryIdInfoIndex = 0xFF;
    Dcm_DspProgram.BlockLength = 0;
    Dcm_DspProgram.FirstFlag = FALSE;
    SchM_Exit_Dcm(Dcm_DspProgram);
}
#define DCM_STOP_SEC_CODE
#include  "Dcm_MemMap.h"

/*************************************************************************/
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
FUNC(void,DCM_CODE)Dcm_Init(P2CONST(Dcm_CfgType, DCM_CONST, DCM_CONST_PBCFG)ConfigPtr)
{
    uint16  Index;
#if (STD_ON == DCM_DSP_DATA_FUNC_ENABLED)
    uint16  Offest = 0;
    P2VAR(uint8, AUTOMATIC, DCM_VAR)VIN_Data;
    Dcm_NegativeResponseCodeType ErrorCode;
#endif
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(NULL_PTR == ConfigPtr)
    {
        DCM_DET_REPORTERROR(DCM_INIT_ID,DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        for(Index=0;Index<DCM_CHANNEL_NUM;Index++)
        {
            DslInternal_InitChannelCtrl((uint8)Index);
        }
        for(Index=0;Index<DCM_MSG_NUM;Index++)
        {
            DslInternal_InitMsgCtrl((uint8)Index);
        }
        for(Index=0;Index<DCM_DSLPROTOCOLROW_NUM_MAX;Index++)
        {
            DslInternal_InitProtocolCtrl((uint8)Index);
        }
        for(Index=0;Index<DCM_CONNECTION_NUM;Index++)
        {
            DslInternal_InitConnectionCtrl((uint8)Index);
        }
        /*Clear Buffer*/
        SchM_Enter_Dcm(Dcm_Channel);
        for(Index=0;Index<DCM_CHANNEL_LENGTH;Index++)
        {
            Dcm_Channel[Index] = 0x00;
        }
        SchM_Exit_Dcm(Dcm_Channel);

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
        DslInternal_InitSesCtrl();
#endif

#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
        DslInternal_InitSecCtrl();
#endif

#if ((STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED) && (DCM_DSP_ROUTINE_MAX_NUM > 0))
        Dsp_InitRoutineStates();
#endif
        DslInternal_InitComMCtrl();
        DslInternal_InitMkCtrl();

#if(STD_ON == DCM_PAGEDBUFFER_ENABLED)
        DslInternal_InitPageBuffer();
#endif
        DslInternal_InitDspProgramInfo();

#if (DCM_DDDID_STORAGE == STD_ON)
        Dcm_UDS0x2C_Init();
#endif

#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
        Dcm_UDS0x86_Init();
#endif

#if(STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
        Dcm_Scheduler_0x2A_Init();
#endif

        /*get VIN*/
        Dcm_VIN.Flag = FALSE;
#if (STD_ON == DCM_DSP_DATA_FUNC_ENABLED)
        if (NULL_PTR != Dcm_GeneralCfg.DcmVinRef)
        {
            Dcm_VIN.Flag = TRUE;
            for (Index = 0; Index < 17u; Index++)
            {
                Dcm_VIN.Dcm_VIN[Index] = 0xFF;
            }
            for (Index = 0;
                 (Index < Dcm_GeneralCfg.DcmVinRef->DcmDspDidSignalNum)
                         && (Dcm_VIN.Flag == TRUE); Index++)
            {
                VIN_Data = &(Dcm_VIN.Dcm_VIN[Offest]);
                if (NULL_PTR != Dcm_GeneralCfg.DcmVinRef->pDcmDspDidSignal[Index].
                        pDcmDspDidData->DcmDspDataReadFnc)
                {
                    if(E_OK != Dcm_GeneralCfg.DcmVinRef->pDcmDspDidSignal[Index].
                            pDcmDspDidData->DcmDspDataReadFnc(DCM_INITIAL,VIN_Data,&ErrorCode))
                    {
                        Dcm_VIN.Flag = FALSE;
                    }
                    else
                    {
                        Offest += ((Dcm_GeneralCfg.DcmVinRef->pDcmDspDidSignal[Index].
                                pDcmDspDidData->DcmDspDataSize +7u) >> 3u);
                    }
                }
            }
        }
#endif
    }
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"


#if(STD_ON == DCM_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <0x24>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <VersionInfo:Pointer to
 * where to store the version information of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void,DCM_CODE)Dcm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_VAR)VersionInfo)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
   if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
   {
       DCM_DET_REPORTERROR(DCM_GETVERSIONINFO_ID,DCM_E_UNINIT);
   }
   else if(NULL_PTR == VersionInfo)
   {
       DCM_DET_REPORTERROR(DCM_GETVERSIONINFO_ID,DCM_E_PARAM_POINTER);
   }
   else
#endif
   {
       VersionInfo->vendorID = DCM_VENDOR_ID;
       VersionInfo->moduleID = DCM_MODULE_ID;
       VersionInfo->sw_major_version = DCM_H_SW_MAJOR_VERSION;
       VersionInfo->sw_minor_version = DCM_H_SW_MINOR_VERSION;
       VersionInfo->sw_patch_version = DCM_H_SW_PATCH_VERSION;
   }
}
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetVin(P2VAR(uint8, AUTOMATIC, DCM_VAR) Data)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETVIN_ID,DCM_E_UNINIT);
    }
    else if(NULL_PTR == Data)
    {
        DCM_DET_REPORTERROR(DCM_GETVIN_ID,DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        if (Dcm_VIN.Flag == TRUE)
        {
            Dcm_MemoryCopy(Dcm_VIN.Dcm_VIN,Data,17);
            ret = E_OK;
        }
    }
	return  ret;
}
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemTriggerOnDTCStatus(
		uint32 DTC,
		Dem_UdsStatusByteType DTCStatusOld,
		Dem_UdsStatusByteType DTCStatusNew)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_DEMTRIGGERONDTCSTATUS_ID,DCM_E_UNINIT);
    }
    else
#endif
    {
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
        Dcm_UDS0x86_DTCStatusChange(DTCStatusOld, DTCStatusNew);
#else
        DCM_UNUSED(DTCStatusNew);
        DCM_UNUSED(DTCStatusOld);
#endif
        DCM_UNUSED(DTC);
    }
	return  E_OK;
}
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
FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSecurityLevel(
        P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_VAR)SecLevel )
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETSECURITYLEVEL_ID,DCM_E_UNINIT);
    }
    else if( NULL_PTR == SecLevel)
    {
        DCM_DET_REPORTERROR(DCM_GETSECURITYLEVEL_ID,DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        (*SecLevel) = Dcm_MkCtrl.Dcm_ActiveSec;
        ret = E_OK;
    }
    return  ret;
}
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
FUNC(Std_ReturnType,DCM_CODE)Dcm_GetSesCtrlType(P2VAR(Dcm_SesCtrlType,AUTOMATIC,DCM_VAR)SesType)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETSESCTRLTYPE_ID,DCM_E_UNINIT);
    }
    else if( NULL_PTR == SesType)
    {
        DCM_DET_REPORTERROR(DCM_GETSESCTRLTYPE_ID,DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        (*SesType) = Dcm_MkCtrl.Dcm_ActiveSes;
        ret = E_OK;
    }
    return  ret;
}
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
FUNC(Std_ReturnType,DCM_CODE)Dcm_GetActiveProtocol(
        P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_VAR)ActiveProtocol )
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_GETACTIVEPROTOCOL_ID,DCM_E_UNINIT);
    }
    else if(NULL_PTR == ActiveProtocol)
    {
        DCM_DET_REPORTERROR(DCM_GETACTIVEPROTOCOL_ID,DCM_E_PARAM_POINTER);
    }
    else
#endif
    {
        *ActiveProtocol=Dcm_MkCtrl.Dcm_ActiveProtocol;
        if ((DCM_OBD_ON_CAN == *ActiveProtocol)
                ||(DCM_OBD_ON_FLEXRAY == *ActiveProtocol)
                ||(DCM_OBD_ON_IP == *ActiveProtocol))
        {
            *ActiveProtocol = DCM_NO_ACTIVE_PROTOCOL;
        }
        ret = E_OK;
    }
    return ret;
}
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
 * Return              <Std_ReturnType E_OK: this value is always returned>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)Dcm_ResetToDefaultSession(void)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_RESETTODEFAULTSESSION_ID,DCM_E_UNINIT);
    }
    else
#endif
    {
        SchM_Enter_Dcm(Dcm_SesCtrl);
        Dcm_SesCtrl.Dcm_NewSes = DCM_DEFAULT_SESSION;
        SchM_Exit_Dcm(Dcm_SesCtrl);
        DslInternal_SesRefresh(Dcm_SesCtrl.Dcm_NewSes);
        SchM_Switch_DcmDiagnosticSessionControl(RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION);
    }
	return E_OK;
}
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
FUNC(Std_ReturnType,DCM_CODE)Dcm_TriggerOnEvent(uint8 RoeEventId)
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_TRIGGERONEVENT_ID,DCM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else
#endif
    {
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
        ret = Dcm_UDS0x86_ChangeOfDataIdentifier(RoeEventId);
#else
        DCM_UNUSED(RoeEventId);
#endif
    }
    return ret;
}
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
 * Return              <Std_ReturnType E_OK: this value is always returned>
 * PreCondition        <>
 * CallByAPI           <>
 */
/*************************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType,DCM_CODE)Dcm_SetActiveDiagnostic(boolean active)
{
    uint8 Index;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
    {
        DCM_DET_REPORTERROR(DCM_SETACTIVEDIAGNOSTIC_ID,DCM_E_UNINIT);
    }
    else
#endif
    {
        if (FALSE == active)
        {
            for (Index = 0; Index < DCM_MAINCONNECTION_NUM; Index++)
            {
                SchM_Enter_Dcm(Dcm_CommCtrl);
                Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic = DCM_COMM_NOT_ACTIVE;
                SchM_Exit_Dcm(Dcm_CommCtrl);
            }
        }
        else
        {
            for (Index = 0; Index < DCM_MAINCONNECTION_NUM; Index++)
            {
                SchM_Enter_Dcm(Dcm_CommCtrl);
                Dcm_CommCtrl[Index].Dcm_ActiveDiagnostic = DCM_COMM_ACTIVE;
                SchM_Exit_Dcm(Dcm_CommCtrl);
                ComM_DCM_ActiveDiagnostic(Dcm_CommCtrl[Index].DcmDslProtocolComMChannelId);
            }
        }
    }

	return E_OK;
}
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
FUNC(void,DCM_CODE) Dcm_MainFunction(void)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
   if(DCM_ON !=Dcm_MkCtrl.Dcm_MkState)
   {
       DCM_DET_REPORTERROR(DCM_MAIN_FUNCTION_ID,DCM_E_UNINIT);
   }
   else
#endif
   {
       /**************************************************/
       /*P2Server Timer background processing function*/
       Dcm_MainFunction_P2ServerTimer();
       /***************************************************/
       /*The Pending message background processing function*/
       Dcm_MainFunction_PendingManage();
       /**************************************************/
       /*S3Timer background processing function*/
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
       Dcm_MainFunction_S3Timer();
#endif
       /*************************************************/
       /*SecTimer background processing function*/
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
       Dcm_MainFunction_SecTimer();
#endif
       /**************************************************/
       /*Page_Buffer Timer background processing function*/
#if(DCM_PAGEDBUFFER_ENABLED == STD_ON)
       Dcm_MainFunction_PageBufferTimer();
#endif
       /**************************************************/
       /*ROE Timer check*/
#if (STD_ON == DCM_UDS_SERVICE0X86_ENABLED)
       Dcm_UDS0x86_CheckTimer();
#endif
       /*************************************************/
       Dcm_BootloaderResponse();
   }
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*************************************************************************/
/*
 * Brief               <The Pending message background processing function>
 * ServiceId           <None>
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
static FUNC(void,DCM_CODE)Dcm_MainFunction_PendingManage(void)
{
	uint8  ProtocolCtrlId;
	uint8  MsgCtrlId;
	uint16 SidTabCfgIndex;
	uint16 SidTabServiceCfgIndex;
    Std_ReturnType ret = E_OK;
    Dcm_NegativeResponseCodeType errorCode = DCM_E_RESERVED;
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_DiagnosticServicex)(
    		Dcm_OpStatusType OpStatus,
			uint8  ProtocolCtrlId,
    		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode);

	/*Pending manage*/
	for(ProtocolCtrlId = 0; ProtocolCtrlId < DCM_DSLPROTOCOLROW_NUM_MAX; ProtocolCtrlId++)
	{
		MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
		if(DCM_INVALID_UINT8 != MsgCtrlId)
		{
			if(DCM_MSG_RECEIVED == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
			{
				if(DCM_PROTOCOL_INVALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
				{
					/*the protocol not start yet*/
					/*DCM indication App start the protocol,App return E_PENDING*/
					(void)DslInternal_RxIndication(ProtocolCtrlId);
				}
				else
				{
					/*DCM indication Application to receive a new request,App return E_PENDING*/
					(void)DsdInternal_RxIndication(ProtocolCtrlId);
				}
			}
			else if(DCM_MSG_PROCESSED == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState)
			{
				/*when Message is "DCM_MSG_PROCESSED" status,App return E_PENDING"*/
				(void)DsdInternal_SearchSidTabServiceIndex( Dcm_MsgCtrl[MsgCtrlId].SID,
								ProtocolCtrlId,&SidTabCfgIndex,&SidTabServiceCfgIndex);
	            Dcm_DiagnosticServicex = ((Dcm_DsdCfg.pDcmDsdServiceTable)[SidTabCfgIndex].
	                    pDcmDsdService)[SidTabServiceCfgIndex].DcmDsdSidTabFnc;
			   /*Enter the specific service handler*/
	            if (
#if(STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
	            		(DCM_E_FORCE_RCRRP == Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret) &&
#endif
						(DCM_FORCE_RCRRP_OK == Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus))
	            {
	                SchM_Enter_Dcm(Dcm_MsgCtrl);
					Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = E_NOT_OK;
                    SchM_Exit_Dcm(Dcm_MsgCtrl);
	            }
	            else if ((DCM_PENDING == Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus))
	            {
	            	/*do nothing, not return.*/
	            }
	            else
	            {
	            	/*DCM_CANCEL*/
	                ret = E_NOT_OK;
	            }

	            if (ret == E_OK)
	            {
                    ret = (*Dcm_DiagnosticServicex)(Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus,
                            ProtocolCtrlId, &errorCode);
                    if (ret == E_NOT_OK)
                    {
                        if (DCM_E_RESERVED == errorCode)
                        {
                            DslInternal_ResetResource(ProtocolCtrlId);
                        }
                        else
                        {
                            DsdInternal_SetNrc(ProtocolCtrlId,errorCode);
                            DsdInternal_ProcessingDone(ProtocolCtrlId);
                        }
                    }
	            }
			}
			else if(  (FALSE == Dcm_MsgCtrl[MsgCtrlId].SendFlag)
	                 &&(DCM_MSG_TRANSMISSION == Dcm_MsgCtrl[MsgCtrlId].Dcm_MsgState))
			{
				DslInternal_ProcessingDone(ProtocolCtrlId);
			}
			else
			{
				/*Avoid Misra error*/
			}
		}
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
        if (DCM_PROTOCOL_VALID == Dcm_ProtocolCtrl[ProtocolCtrlId].Dcm_ProtocolState)
        {
		    Dcm_MainFunction_Scheduler_0x2A(ProtocolCtrlId);
        }
#endif
	}
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <P2Server Timer background processing function>
 * ServiceId           <None>
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
static FUNC(void,DCM_CODE)Dcm_MainFunction_P2ServerTimer(void)
{
	uint8  ProtocolCtrlId;
	uint8  MsgCtrlId;
	uint32 OldTimer;
	uint32 ExpireTimer;
	uint32 Timer;

	for(ProtocolCtrlId = 0; ProtocolCtrlId < DCM_DSLPROTOCOLROW_NUM_MAX; ProtocolCtrlId++)
	{
		MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
		if(DCM_INVALID_UINT8 !=  MsgCtrlId)
		{
			if(DCM_P2TIMER_ON == (Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2State))
			{
				OldTimer    = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2CurTimer;
				ExpireTimer = Dcm_MsgCtrl[MsgCtrlId].Dcm_P2Ctrl.Dcm_P2ExpiredTimer;
			    Dcm_GetTimeSpan(OldTimer,&Timer);
				if(Timer >= ExpireTimer)
				{/*P2Server Timer timeout*/
					DslInternal_P2ServerTimeout(ProtocolCtrlId);
				}
			}
		}
	}
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*************************************************************************/
/*
 * Brief               <S3Timer background processing function>
 * ServiceId           <None>
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
#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_S3Timer(void)
{
	uint32 OldTimer;
	uint32 ExpireTimer;
	uint32 Timer;

	/*S3Server timer*/
	if(DCM_S3TIMER_ON == (Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3State))
	{
		OldTimer    = Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3CurTimer;
		ExpireTimer = Dcm_SesCtrl.Dcm_S3Ctrl.Dcm_S3ExpiredTimer;
	    Dcm_GetTimeSpan(OldTimer,&Timer);
		if(Timer >= ExpireTimer)
		{
			/*S3Server Timer timeout*/
			DslInternal_S3ServerTimeout();
		}
	}
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               <Read SecTimer background processing function>
 * ServiceId           <None>
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
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_ReadSecTimer(void)
{
    uint8    SecCfgIndex;
    P2CONST(Dcm_DspSecurityRowType,AUTOMATIC,DCM_CONST)pSecurityRow;

    /***************************************/
#if (DCM_SECURITY_MAX_READOUT_TIME != 0)
    Std_ReturnType ret;

    if (Dcm_SecCtrl.Dcm_MaxReadoutTime < DCM_SECURITY_MAX_READOUT_TIME)
    {
        for (SecCfgIndex = Dcm_SecCtrl.Dcm_SecCfgIndex;
                (SecCfgIndex < DCM_SECURITY_NUM);SecCfgIndex++)
        {
            pSecurityRow = &(Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
            if (pSecurityRow != NULL_PTR)
            {
                if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                   &&((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                   ||(USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
                {
                    if (pSecurityRow->Dcm_GetSecurityAttemptCounterFnc != NULL_PTR)
                    {
                        ret = pSecurityRow->Dcm_GetSecurityAttemptCounterFnc(
                                DCM_INITIAL,&Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]);
                        if (DCM_E_PENDING == ret)
                        {
                            break;
                        }
                        else
                        {
                            Dcm_SecCtrl.Dcm_SecCfgIndex++;
                        }
                        if (E_NOT_OK == ret )
                        {
                            Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex] =
                                    pSecurityRow->DcmDspSecurityNumAttDelay;
                        }
                        if(Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex] >=
                                pSecurityRow->DcmDspSecurityNumAttDelay)
                        {
                            Dcm_ResetTime(
                                    &Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTimer[SecCfgIndex]);
                            Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTimer[SecCfgIndex] =
                            (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].
                            DcmDspSecurityDelayTime;
                            Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] =
                                    DCM_SECTIMER_ON;
                        }
                    }
                }
            }
        }
        if (SecCfgIndex == DCM_SECURITY_NUM)
        {
            Dcm_SecCtrl.Dcm_SecFlag |= DCM_SEC_CALL_OVER_MASK;
        }
    }
    else
#endif
    {
        SecCfgIndex = Dcm_SecCtrl.Dcm_SecCfgIndex;
        pSecurityRow = &(Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
        pSecurityRow->Dcm_GetSecurityAttemptCounterFnc(
                DCM_CANCEL,&Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]);

        for (SecCfgIndex = Dcm_SecCtrl.Dcm_SecCfgIndex;
                (SecCfgIndex < DCM_SECURITY_NUM);SecCfgIndex++)
        {
            pSecurityRow = &(Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
            if (pSecurityRow != NULL_PTR)
            {
                if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                   &&((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                   ||(USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
                {
                    Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex] =
                            pSecurityRow->DcmDspSecurityNumAttDelay;
                    Dcm_ResetTime(
                            &Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTimer[SecCfgIndex]);
                    Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTimer[SecCfgIndex] =
                    (Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow)[SecCfgIndex].
                    DcmDspSecurityDelayTime;
                    Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] =
                            DCM_SECTIMER_ON;
                }
            }
        }
        Dcm_SecCtrl.Dcm_SecFlag |= DCM_SEC_CALL_OVER_MASK;
    }
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif


/*************************************************************************/
/*
 * Brief               <SecTimer background processing function>
 * ServiceId           <None>
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
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_MainFunction_SecTimer(void)
{
	uint32 OldTimer;
	uint32 ExpireTimer;
	uint32 Timer;
	uint8    SecCfgIndex;
    P2CONST(Dcm_DspSecurityRowType,AUTOMATIC,DCM_CONST)pSecurityRow;

	/***************************************/
    if (0u == (Dcm_SecCtrl.Dcm_SecFlag & DCM_SEC_CALL_OVER_MASK))
    {
    	Dcm_MainFunction_ReadSecTimer();
    }
	/*security timer*/
	for (SecCfgIndex = 0; SecCfgIndex < DCM_SECURITY_NUM; SecCfgIndex++)
	{
        if(DCM_SECTIMER_ON == (Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex]))
        {
            OldTimer     = Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecCurTimer[SecCfgIndex];
            ExpireTimer  = Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecExpiredTimer[SecCfgIndex];
            Dcm_GetTimeSpan(OldTimer,&Timer);
            if(Timer >= ExpireTimer)
            {
                /*security timeout*/
                SchM_Enter_Dcm(Dcm_SecCtrl);
                Dcm_SecCtrl.Dcm_RunDlyCtrl.Dcm_SecTimerState[SecCfgIndex] = DCM_SECTIMER_OFF;
                /*********************************/
                Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex] = 0;
                Dcm_SecCtrl.Dcm_SubfunctionForSeed = 0;
                Dcm_SecCtrl.Dcm_SecServiceState = DCM_SERVICE_IDLE;
                SchM_Exit_Dcm(Dcm_SecCtrl);
                pSecurityRow = &(Dcm_DspCfg.pDcm_DspSecurity->pDcm_DspSecurityRow[SecCfgIndex]);
                if (pSecurityRow != NULL_PTR)
                {
                    if ((TRUE == pSecurityRow->DcmDspSecurityAttemptCounterEnabled)
                       &&((USE_ASYNCH_FNC == pSecurityRow->DcmDspSecurityUsePort)
                         || (USE_ASYNCH_CLIENT_SERVER == pSecurityRow->DcmDspSecurityUsePort)))
                    {
                        if (pSecurityRow->Dcm_SetSecurityAttemptCounterFnc != NULL_PTR)
                        {
                            (void)pSecurityRow->Dcm_SetSecurityAttemptCounterFnc(
                                    Dcm_SecCtrl.Dcm_OpStatus,
                                    Dcm_SecCtrl.Dcm_FalseAcessCount[SecCfgIndex]);
                        }
                    }
                }
            }
        }
	}
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_GetUDSPhyPduID(P2VAR(PduIdType,AUTOMATIC,DCM_VAR)rxPhyPduId)
{
    uint8  protocolIndex;
    uint8  rxpduidindex;
    P2CONST(Dcm_DslProtocolRxType,AUTOMATIC,DCM_CONST)pDslProtocolRx;

    if(NULL_PTR != rxPhyPduId)
    {
        protocolIndex = (Dcm_DslCfg.pDcmDslProtocol)->DcmDslProtocolRow_Num;
        while(protocolIndex > 0u)
        {
            protocolIndex--;
            if(DCM_UDS_ON_CAN == (Dcm_DslCfg.pDcmDslProtocol)->
                                 pDcmDslProtocolRow[protocolIndex].DcmDslProtocolID)
            {
                /** regard a protocol only have a connection here */
                pDslProtocolRx = (Dcm_DslCfg.pDcmDslProtocol)->
                                  pDcmDslProtocolRow[protocolIndex].
                                  pDcmDslConnection[0].pDcmDslMainConnection->
                                  pDcmDslProtocolRx;
                rxpduidindex = (Dcm_DslCfg.pDcmDslProtocol)->
                                pDcmDslProtocolRow[protocolIndex].pDcmDslConnection[0].
                                pDcmDslMainConnection->DcmDslProtocolRx_Num;
                while(rxpduidindex > 0u)
                {
                    rxpduidindex--;
                    if(DCM_PHYSICAL == pDslProtocolRx[rxpduidindex].DcmDslProtocolRxAddrType)
                    {
                        *rxPhyPduId = pDslProtocolRx[rxpduidindex].DcmDslProtocolRxPduId;
                    }
                }
                break;
            }
        }
    }
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if(DCM_PAGEDBUFFER_ENABLED == STD_ON)
/*************************************************************************/
/*
 * Brief               <Page_Buffer Timer background processing function>
 * ServiceId           <None>
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
static FUNC(void,DCM_CODE)Dcm_MainFunction_PageBufferTimer(void)
{
    uint8  ProtocolCtrlId;
    uint8  MsgCtrlId;
    uint32 OldTimer;
    uint32 ExpireTimer;
    uint32 Timer;

    for(ProtocolCtrlId = 0; ProtocolCtrlId < DCM_DSLPROTOCOLROW_NUM_MAX; ProtocolCtrlId++)
    {
        MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
        if(DCM_INVALID_UINT8 !=  MsgCtrlId)
        {
            if(TRUE == Dcm_PageBufferData.TimerStart)
            {
                OldTimer    = Dcm_PageBufferData.CurTimer;
                ExpireTimer = Dcm_PageBufferData.ExpiredTimer;
                Dcm_GetTimeSpan(OldTimer,&Timer);
                if(Timer >= ExpireTimer)
                {/*PageBuffer Timer timeout*/
                    SchM_Enter_Dcm(Dcm_PageBufferData);
                    Dcm_PageBufferData.TimeOut = TRUE;
                    SchM_Exit_Dcm(Dcm_PageBufferData);
                }
            }
        }
    }
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#define  DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(void,DCM_CODE)Dcm_BootloaderResponse(void)
{
   PduInfoType  PduBuffer;
   PduInfoType  *PduInfo = &PduBuffer;
   PduIdType pduRxId = 0;
   PduLengthType availabeBufferLength;
   uint8 Data[2] = {0};

   if(DCM_WARM_START == Dcm_GetProgConditions(&ProgConditions))
   {
      if (ProgConditions.ApplUpdated == TRUE)
      {
          gAppl_UpdataOK_ResponseFlag = TRUE;
          ProgConditions.ApplUpdated = FALSE;
          /*receive App updata flag,response 0x51 01*/
#if(DCM_BSWM_ENABLE == STD_ON)
          BswM_Dcm_ApplicationUpdated();
#endif

          Dcm_GetUDSPhyPduID(&pduRxId);

          PduInfo->SduLength = 0;
          (void)Dcm_StartOfReception(pduRxId,PduInfo,2,&availabeBufferLength);
          Data[0] = 0x10;
          Data[1] = 0x83;
          PduInfo->SduDataPtr = Data;
          PduInfo->SduLength = 2;
          (void)Dcm_CopyRxData(pduRxId,PduInfo,&availabeBufferLength);
          Dcm_TpRxIndication(pduRxId,E_OK);

          PduInfo->SduLength = 0;
          (void)Dcm_StartOfReception(pduRxId,PduInfo,2,&availabeBufferLength);
          Data[0] = 0x11;
          Data[1] = 0x01;
          PduInfo->SduDataPtr = Data;
          PduInfo->SduLength = 2;
          (void)Dcm_CopyRxData(pduRxId,PduInfo,&availabeBufferLength);
          Dcm_TpRxIndication(pduRxId,E_OK);
      }
   }
}

extern uint32 Diag_Timer;
FUNC(void, DCM_CODE)
Dcm_ResetTime(
P2VAR(uint32, AUTOMATIC, DCM_VAR) TimerPtr
)
{
    *TimerPtr = Diag_Timer;
}

FUNC(void, DCM_CODE)
Dcm_GetTimeSpan(
uint32 TimerPtr,
P2VAR(uint32, AUTOMATIC, DCM_VAR) TimeSpanPtr
)
{
    if (Diag_Timer >= TimerPtr)
    {
        *TimeSpanPtr = Diag_Timer - TimerPtr;
    }
    else
    {
        *TimeSpanPtr = (0xFFFFFFFF - TimerPtr) + Diag_Timer;
    }
}
#define  DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*******************************************************************************
**                            General Notes                                   **
*******************************************************************************/
