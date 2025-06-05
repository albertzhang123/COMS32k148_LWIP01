/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2020)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Dem_OBD.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for OBD                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2018-4-20  tao.yu    Initial version                         */
/*  V1.0.1       2019-9-17  tao.yu    fix some bug,change event callback      */
/*  V1.0.2       2019-12-25  tao.yu    QAC fix     							  */
/*  V1.0.3       2020-1-7   tao.yu    Commercial project problem modification */
/*  V1.0.4       2020-3-10   tao.yu    enable condition has no impact on      */
/* Dem_ResetEventDebounceStatus,  Dem_ResetEventStatus and Dem_<...>ClearDTC. */
/* 	V2.0.0		 2020-07-15  pengfei.zhu	fix bugs by PIM; optimized code   */
/* 	V2.0.1		 2020-09-15  pengfei.zhu	QAC test 						  */
/*******************************************************************************
**                       Version  information                                 **
*******************************************************************************/
#define DEM_OBD_C_AR_MAJOR_VERSION  	4U
#define DEM_OBD_C_AR_MINOR_VERSION  	2U
#define DEM_OBD_C_AR_PATCH_VERSION  	2U
#define DEM_OBD_C_SW_MAJOR_VERSION  	2U
#define DEM_OBD_C_SW_MINOR_VERSION  	0U
#define DEM_OBD_C_SW_PATCH_VERSION  	1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"
#include "Dem.h"
#include "Dem_Dcm.h"
#if(STD_ON == DEM_TRIGGER_FIM_REPORTS)
#include "FiM.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_OBD_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_OBD.c : Mismatch in Specification Major Version"
#endif
#if (DEM_OBD_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_OBD.c : Mismatch in Specification Major Version"
#endif
#if (DEM_OBD_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_OBD.c : Mismatch in Specification Major Version"
#endif
#if (DEM_OBD_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_OBD.c : Mismatch in Specification Major Version"
#endif
#if (DEM_OBD_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_OBD.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************************
**                      macros                                                **
*******************************************************************************/


/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if(DEM_RATIO_NUM > 0)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
IUMPRType IUMPRValue[DEM_RATIO_NUM];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif


#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"
Dem_IndicatorStatusType OBDMilStatus;
uint16 OBDDistanceMILOn;
uint16 OBDTimeMILOn;
uint16 OBDTimeDTCClear;
uint16 DistSinceDtcCleared;
uint16  IgnUpCycleCounter;
uint16 OBDDistanceMILLastOn;
uint16 OBDTimeMILLastOn;
uint16 OBDDistanceDTCClear;
uint16 OBDTimeDTCLastClear;
#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"
uint8 WarmUpCycleCounter;
#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Dem_MemMap.h"
boolean CurrentPTOStatus;
#define DEM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_INIT_16
#include "Dem_MemMap.h"
uint16 ContinuousMICounter = 0;
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
uint16 MasterContinuousMICounter = 0;
#endif
uint16 OBDB1Counter = 0;
#define DEM_STOP_SEC_VAR_INIT_16
#include "Dem_MemMap.h"


/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define DEM_START_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static boolean PFCStatu = FALSE;
static Dem_IumprDenomCondStatusType ConditionStatu[5] = {3,3,3,3,3};
#define DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#if (DEM_DTR_NUM > 0)
static DTRInfoType DTRInfo[DEM_DTR_NUM];
#endif
static boolean SetDataOfPid21;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
#if(DEM_RATIO_NUM > 0)
static FUNC(void, DEM_CODE) Dem_CalIUMPRValue(Dem_RatioIdType Dem_RatioId);
static FUNC(void, DEM_CODE) Dem_CalIUMPRDenValue(Dem_RatioIdType Dem_RatioId);
static FUNC(void, DEM_CODE) Dem_CalIUMPRNumValue(Dem_RatioIdType Dem_RatioId);
static FUNC(uint16, DEM_CODE) Dem_GetIUMPRDenValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup);
static FUNC(uint16, DEM_CODE) Dem_GetIUMPRNumValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup);
#endif
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
static FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_SelectOBDFreezeFrame(
	P2VAR(Dem_EventIdType,AUTOMATIC, DEM_APPL_DATA) IntId
);
#endif
#if((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
static FUNC(void, DEM_CODE) Dem_B1CounterProcess(void);
#endif
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Init the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_OBDInit(void)
{
    WarmUpCycleCounter = 0;
    /*distance related*/
    OBDDistanceMILOn = 0;
    OBDDistanceDTCClear = 0;
    DistSinceDtcCleared = 0;
    OBDDistanceMILLastOn = 0;
    /*time related*/
    OBDTimeMILOn = 0;
    OBDTimeDTCClear = 0;
    OBDTimeDTCLastClear = 0;
    OBDTimeMILLastOn = 0;
    SetDataOfPid21 = FALSE;
    PFCStatu = FALSE;
    OBDMilStatus = DEM_INDICATOR_OFF;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DTR_NUM > 0)
/*************************************************************************/
/*
 * Brief               Init the IUMPR.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DTRInit(void)
{
    uint16 iloop = 0;
    while (iloop < DEM_DTR_NUM)
    {
        DTRInfo[iloop].LowerLimit = 0;
        DTRInfo[iloop].TestResult = 0;
        DTRInfo[iloop].UpperLimit = 0;
        iloop++;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               Init the IUMPR.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_IUMPRInit(void)
{
    uint8 iloop = 0;
    IgnUpCycleCounter = 0;
    while (iloop < DEM_RATIO_NUM)
    {
        IUMPRValue[iloop].IUMPRDenStatus = TRUE;
        IUMPRValue[iloop].IUMPRNumStatus = FALSE;
        IUMPRValue[iloop].Denominator = 0;
        IUMPRValue[iloop].Numerator = 0;
        IUMPRValue[iloop].General_Denominator = 0;
        iloop++;
    }
    ConditionStatu[0] = 3;
    ConditionStatu[1] = 3;
    ConditionStatu[2] = 3;
    ConditionStatu[3] = 3;
    ConditionStatu[4] = 3;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               Update the Current OBD Mil Status.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_UpdateOBDMilStatus(uint8 indicatorRef,uint16 IntID)
{
    Dem_IndicatorStatusType currentMilStatus = DEM_INDICATOR_OFF;
    uint16 CurrentDistance = 0;
    uint16 CurrentTime = 0;
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    static Dem_IndicatorStatusType lastMilStatus = DEM_INDICATOR_OFF;
#endif

    if(indicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
    {
#if(DEM_INDICATOR_NUM > 0)
        currentMilStatus = DemWIRStatus[indicatorRef];
#endif
    }
    else
    {
        return;
    }
    if(currentMilStatus != DEM_INDICATOR_OFF)
    {
        /* MIL recently activated */
        if (((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemDtcValue < 0xFFFF33UL)
                && (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemDtcValue != DEM_DTC_CFG_INVALID))
                || (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
        {/*SWS_Dem_01139] */
            OBDMilStatus = currentMilStatus;
        }
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        /* Read current distance information */
        if (lastMilStatus == DEM_INDICATOR_OFF)
		{
			OBDDistanceMILLastOn = Dem_ReadDistanceInformation();
			OBDTimeMILLastOn = Dem_ReadTimeInformation();
        }
#endif
    }
    else
    {
        /* MIL recently de-activated */
        if (((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemDtcValue < 0xFFFF33UL)
                && (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemDtcValue != DEM_DTC_CFG_INVALID))
                || (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
        {/*SWS_Dem_01139] */
            OBDMilStatus = currentMilStatus;
        }
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        CurrentDistance = Dem_ReadDistanceInformation();
#endif
        if (CurrentDistance > OBDDistanceMILLastOn)
        {
            if ((uint16)(CurrentDistance - OBDDistanceMILLastOn + OBDDistanceMILOn) < 0xFFFFu)
            {
                OBDDistanceMILOn += CurrentDistance - OBDDistanceMILLastOn;
                OBDDistanceMILLastOn = CurrentDistance;
            }
            else
            {
                OBDDistanceMILOn = 0xFFFFu;
            }
        }
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
        CurrentTime = Dem_ReadTimeInformation();
#endif
        if (CurrentTime > OBDTimeMILLastOn)
        {
            if ((uint16)(CurrentTime - OBDTimeMILLastOn + OBDTimeMILOn) < 0xFFFFu)
            {
                OBDTimeMILOn += CurrentTime - OBDTimeMILLastOn;
                OBDTimeMILLastOn = CurrentTime;
            }
            else
            {
                OBDTimeMILOn = 0xFFFFu;
            }
        }
    }
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    lastMilStatus = currentMilStatus;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Clear the OBD Information on clearing dtc.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ClearOBDInfo(void)
{
    /* Clear counters if all OBD DTCs have been cleared */
    WarmUpCycleCounter = 0;
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    OBDDistanceDTCClear = Dem_ReadDistanceInformation();
    OBDTimeDTCLastClear = Dem_ReadTimeInformation();
#endif
    OBDTimeDTCClear = 0;
    ContinuousMICounter = 0;/*SWS_Dem_01146]*/
    OBDB1Counter = 0;
    OBDDistanceMILOn = 0;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
/*************************************************************************/
/*
 * Brief               Calculate the OBD related data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_CalOBDRelatedValue(uint8 OperationId)
{
    uint16 iloop = 0;
	static uint8 WarmUpCycleCounterForMI = 0;
	static uint8 CycleCounterForMI = 0;
	static uint8 CycleCounterForB1 = 0;
	static uint8 TimeCycleCounterForMI = 0;

#if(DEM_RATIO_NUM > 0)
    uint16 DemDTCRef = 0;
#endif
    if (OBDMilStatus == DEM_INDICATOR_OFF)
    {
        if (CycleCounterForMI < 0xFFu)
        {
            CycleCounterForMI++;
        }
    }
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if ((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B1)
            && ((DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TF) != 0x00u)
                 && (DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_CDTC) != 0x00u)))
        {
            CycleCounterForB1 = 0;
            break;
        }
        CycleCounterForB1++;

        if (CycleCounterForB1 > 200u)
        {/*SWS_Dem_01157]  */
            OBDB1Counter = 190u;
        }
        else if (CycleCounterForB1 > 3u)
        {/*SWS_Dem_01156] */
            OBDB1Counter = 0u;
        }
        else
        {

        }
        iloop++;
    }

    switch(DemOperationCycle[OperationId].DemOperationCycleType)
    {
      case DEM_OPCYC_IGNITION:
              IgnUpCycleCounter++;
          break;
      case DEM_OPCYC_OBD_DCY:
#if(DEM_RATIO_NUM > 0)
          iloop = 0;
            while (iloop < DEM_RATIO_NUM)
            {
                DemDTCRef = DemPbCfgPtr->DemEventParameter[DemRatio[iloop].DemDiagnosticEventRef].DemDTCRef;
                if ((DemDTCGeneralStatus[DemDTCRef].SuppressionStatus != TRUE)
                    && (0x00u != DEM_FLAGS_ISSET(DemEventRelateInformation[DemRatio[iloop].DemDiagnosticEventRef].Status, DEM_EVENT_STATUS_AVAILABLE)))/*SWS_Dem_01104] */
                {/*SWS_Dem_00709] */
                    Dem_CalIUMPRValue(iloop);
                }
                iloop++;
            }
#endif
          break;
      case DEM_OPCYC_WARMUP:
          if (WarmUpCycleCounter < 0xFFu)
          {
              WarmUpCycleCounter++;
          }
          if (OBDMilStatus == DEM_INDICATOR_OFF)
          {
              if (WarmUpCycleCounterForMI < 40u)
              {/*SWS_Dem_01146]*/
                  WarmUpCycleCounterForMI++;
              }
              else
              {
                  WarmUpCycleCounterForMI = 0u;
                  ContinuousMICounter = 0u;
              }
          }
          break;
      case DEM_OPCYC_TIME:
          if (OBDMilStatus != DEM_INDICATOR_OFF)
          {
              if (CycleCounterForMI > 3u)
              {/*SWS_Dem_01145][SWS_Dem_01144]*/
                  ContinuousMICounter = 0u;
              }
              if (ContinuousMICounter < 0xFFu)
              {/*SWS_Dem_01147] [SWS_Dem_01142] */
                  ContinuousMICounter++;
              }
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)
              if (MasterContinuousMICounter < 0xFFu)
              {/*SWS_Dem_01151] [SWS_Dem_01152]*/
                  MasterContinuousMICounter++;
              }
#endif
              WarmUpCycleCounterForMI = 0u;
              TimeCycleCounterForMI = 0u;
              CycleCounterForMI = 0u;
          }
          else
          {
              if (TimeCycleCounterForMI < 200u)
              {/*SWS_Dem_01146]*/
                  TimeCycleCounterForMI++;
              }
              else
              {
                  TimeCycleCounterForMI = 0u;
                  ContinuousMICounter = 0u;
              }
          }
          Dem_B1CounterProcess();
          break;
      default:
          break;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_RATIO_NUM > 0)
/*************************************************************************/
/*
 * Brief               Get the IUMPR calculated data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*SWS_Dem_01104  IUMPR ratios referring to an unavailable event shall neither be
computed nor reported.*/
static FUNC(void, DEM_CODE) Dem_CalIUMPRValue(Dem_RatioIdType Dem_RatioId)
{
#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
    boolean permission = TRUE;
    uint8 FimFunctionId = 0;
    uint16 iloop = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) FimFunctionIdIndex = NULL_PTR;
    FimFunctionId = DemRatio[Dem_RatioId].DemFunctionIdRef;
    FiM_GetFunctionPermission(FimFunctionId, &permission);
    if(permission == FALSE)  /*Consider the integration with the FIM*/
    {
        return;  /*Numerator and Denominator both are forbidden*/
    }
    else
    {
        if (DemRatio[Dem_RatioId].DemSecondaryFunctionIdRef != NULL_PTR)
        {
            FimFunctionIdIndex = DemRatio[Dem_RatioId].DemSecondaryFunctionIdRef->DemSecondaryFunctionIdRef;
            while(iloop < DemRatio[Dem_RatioId].DemSecondaryFunctionIdRef->DemSecondaryFunctionIdRefNum)
            {
                FiM_GetFunctionPermission(*(FimFunctionIdIndex), &permission);
                if(permission == FALSE)  /*Consider the integration with the FIM*/
                {
                    return;  /*Numerator and Denominator both are forbidden*/
                }
                iloop++;
                FimFunctionIdIndex++;
            }
        }
    }
#endif

	Dem_CalIUMPRDenValue(Dem_RatioId);
	Dem_CalIUMPRNumValue(Dem_RatioId);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Get the IUMPR calculated Denominator data.
 * ServiceId           Internal Function>
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_CalIUMPRDenValue(Dem_RatioIdType Dem_RatioId)
{
   /*get General Denominator*/
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint8 eventid = 0;

    eventid = DemRatio[Dem_RatioId].DemDiagnosticEventRef;
    pEvent = Dem_GetEventInfo(eventid);
    if(0u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
    {
        return;
    }
    else
    {
    	IUMPRValue[Dem_RatioId].General_Denominator++;
    }
    /*get Specific Diagnostic Denominator*/
    if(IUMPRValue[Dem_RatioId].IUMPRDenStatus == TRUE)/*SWS_Dem_00712]*/
    {
        if (IUMPRValue[Dem_RatioId].Denominator < 0xFFFFu)
        {
            IUMPRValue[Dem_RatioId].Denominator++;
        }
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Get the IUMPR calculated Numerator data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_CalIUMPRNumValue(Dem_RatioIdType Dem_RatioId)
{
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint8 eventId = 0;
    eventId = DemRatio[Dem_RatioId].DemDiagnosticEventRef;
    pEvent = Dem_GetEventInfo(eventId);
#if ((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) || (DEM_OBD_SUPPORT == DEM_OBD_PRIMARY_ECU))/*SWS_Dem_00710] */
    if(DemRatio[Dem_RatioId].DemRatioKind == DEM_RATIO_OBSERVER)/*SWS_Dem_00359][SWS_Dem_00361]*/
    {
        if(DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TFTOC) != 0x00u)
        {
            if (IUMPRValue[Dem_RatioId].Numerator < 0xFFFFu)
            {
                IUMPRValue[Dem_RatioId].Numerator++;
            }
        }
    }
    else
    {
        if(IUMPRValue[Dem_RatioId].IUMPRNumStatus == TRUE)
        {
            if (IUMPRValue[Dem_RatioId].Numerator < 0xFFFFu)
            {
                IUMPRValue[Dem_RatioId].Numerator++;
            }
            IUMPRValue[Dem_RatioId].IUMPRNumStatus = FALSE;
        }
    }
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Get the IUMPR Group Denomitor data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(uint16, DEM_CODE) Dem_GetIUMPRDenValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup)
{
    uint16 iloop = 0;
    while (iloop < DEM_RATIO_NUM)
    {
        if (DemRatio[iloop].DemIUMPRGroup == Dem_IUMPRGroup)
        {
            return IUMPRValue[iloop].Denominator;
        }
        iloop++;
    }
    return 0;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Get the IUMPR calculated Numerator data.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(uint16, DEM_CODE) Dem_GetIUMPRNumValueByGroup(Dem_IUMPRGroupType Dem_IUMPRGroup)
{
    uint16 iloop = 0;
    while (iloop < DEM_RATIO_NUM)
    {
        if (DemRatio[iloop].DemIUMPRGroup == Dem_IUMPRGroup)
        {
            return IUMPRValue[iloop].Numerator;
        }
        iloop++;
    }
    return 0;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 08.
 * ServiceId           0x6b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata08 Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata08BufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetInfoTypeValue08(
Dcm_OpStatusType OpStatus,
uint8* Iumprdata08,
uint8* Iumprdata08BufferSize
)
{

    uint8 tempData[32] = {0};
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE08, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if ((Iumprdata08BufferSize == NULL_PTR) || (Iumprdata08 == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE08, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    tempData[0U]  = (uint8)(IgnUpCycleCounter >> 8U);
    tempData[1U]  = (uint8)(IgnUpCycleCounter);
#if(DEM_RATIO_NUM > 0)
    tempData[2U]  = (uint8)(IUMPRValue[0U].General_Denominator >> 8U);
    tempData[3U]  = (uint8)(IUMPRValue[0U].General_Denominator);
#endif
#if(DEM_RATIO_NUM > 0)
    tempData[4U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[5U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1));
    tempData[6U]  = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[7U]  = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1));

    tempData[8U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[9U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2));
    tempData[10U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[11U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2));

    tempData[12U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[13U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1));
    tempData[14U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[15U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1));

    tempData[16U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[17U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2));
    tempData[18U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[19U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2));

    tempData[20U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[21U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR));
    tempData[22U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[23U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR));

    tempData[24U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[25U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR));
    tempData[26U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[27U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR));

    tempData[28U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[29U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP));
    tempData[30U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[31U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP));
#endif
    *Iumprdata08BufferSize = 32U;

    switch(OpStatus)
    {
        case 0x00u:
            Dem_MemCopy(Iumprdata08,tempData,32U);
            break;
        case 0x01u:
        case 0x02u:
        case 0x03u:
            break;
        default:
            break;
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service is used for requesting IUMPR data according to InfoType 0B.
 * ServiceId           0x6c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      OpStatus Only DCM_INITIAL will appear, because this API
                        behaves synchronous.
 * Param-Name[out]     Iumprdata0B Buffer containing the number of data elements (as
                        defined in ISO-15031-5) and contents of InfoType
                        08. The buffer is provided by the Dcm.
 * Param-Name[in/out]  Iumprdata0BBufferSize: The maximum number of data bytes that can be
                        written to the Iumprdata08 Buffer.
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetInfoTypeValue0B(
Dcm_OpStatusType OpStatus,
uint8* Iumprdata0B,
uint8* Iumprdata0BBufferSize
)
{
    uint8 tempData[32] = {0};
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE0B, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if ((Iumprdata0BBufferSize == NULL_PTR) || (Iumprdata0B == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETINFOTYPEVALUE0B, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    tempData[0U]  = (uint8)(IgnUpCycleCounter >> 8U);
    tempData[1U]  = (uint8)(IgnUpCycleCounter);
#if(DEM_RATIO_NUM > 0)
    tempData[2U]  = (uint8)(IUMPRValue[0].General_Denominator >> 8U);
    tempData[3U]  = (uint8)(IUMPRValue[0].General_Denominator);
#endif
#if(DEM_RATIO_NUM > 0)
    tempData[4U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[5U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT1));
    tempData[6U]  = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1) >> 8U);
    tempData[7U]  = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT1));

    tempData[8U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[9U]  = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_CAT2));
    tempData[10U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2) >> 8U);
    tempData[11U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_CAT2));

    tempData[12U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[13U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS1));
    tempData[14U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1) >> 8U);
    tempData[15U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS1));

    tempData[16U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[17U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_OXS2));
    tempData[18U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2) >> 8U);
    tempData[19U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_OXS2));

    tempData[20U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[21U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EGR));
    tempData[22U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR) >> 8U);
    tempData[23U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EGR));

    tempData[24U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[25U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_SAIR));
    tempData[26U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR) >> 8U);
    tempData[27U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_SAIR));

    tempData[28U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[29U] = (uint8)(Dem_GetIUMPRNumValueByGroup(DEM_IUMPR_EVAP));
    tempData[30U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP) >> 8U);
    tempData[31U] = (uint8)(Dem_GetIUMPRDenValueByGroup(DEM_IUMPR_EVAP));
#endif
    *Iumprdata0BBufferSize = 32U;

    switch(OpStatus)
    {
        case 0x00u:
            Dem_MemCopy(Iumprdata0B,tempData,32U);
            break;
        case 0x01u:
        case 0x02u:
        case 0x03u:
            break;
        default:
            break;
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Reports the value of a requested "availability-OBDMID" to the DCM upon a Service 06 request.
 * ServiceId           0xa3
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid Availablity OBDMID (00,20, 40...)
 * Param-Name[out]     Obdmidvalue Bit coded information on the support of OBDMIDs.
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetAvailableOBDMIDs(
		uint8 Obdmid,
		uint32* Obdmidvalue
)
{
#if (DEM_DTR_NUM > 0)
    uint16 iloop = 0;
    boolean Find = FALSE;
#endif
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETAVAILABLEOBDMIDS, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (Obdmidvalue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETAVAILABLEOBDMIDS, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    *Obdmidvalue = 0;
#if (DEM_DTR_NUM > 0)
    while (iloop < DEM_DTR_NUM)
    {
        if ((DemPbCfgPtr->DTR[iloop].DemDtrMid >= (Obdmid + 0x01u))
              && (DemPbCfgPtr->DTR[iloop].DemDtrMid <= (Obdmid + 0x20u)))
        {
            *Obdmidvalue |= (uint32)1u << (0x20u - (DemPbCfgPtr->DTR[iloop].DemDtrMid - Obdmid));/*SWS_Dem_00760] */
        }
        if((DemPbCfgPtr->DTR[iloop].DemDtrMid == Obdmid)||(Obdmid == 0x0u))
        {
        	Find = TRUE;
        }

        iloop++;
    }
    if(Find == FALSE)
    {
    	*Obdmidvalue = 0;
    }
#else
    DEM_UNUSED(Obdmid);
#endif
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the number of TIDs per (functional) OBDMID.
 * ServiceId           0xa4
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid OBDMID subject of the request to identify the number of assigned TIDs
 * Param-Name[out]     numberOfTIDs Number of assigned TIDs for the requested OBDMID.
 *                      Used as loop value for the DCM to retrieve all OBD/TID result data
 * Param-Name[in/out]  None
 * Return              E_OK: get number of TIDs successful
                        E_NOT_OK: get number of TIDs failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetNumTIDsOfOBDMID(
		uint8 Obdmid,
		uint8* numberOfTIDs
)
{
#if (DEM_DTR_NUM > 0)
    uint16 iloop = 0;
#endif
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNUMTIDSOFOBDMID, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (numberOfTIDs == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETNUMTIDSOFOBDMID, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    *numberOfTIDs = 0u;
#if (DEM_DTR_NUM > 0)
    while (iloop < DEM_DTR_NUM)
    {
        if (DemPbCfgPtr->DTR[iloop].DemDtrMid == Obdmid)
        {
            *numberOfTIDs += 1u;/*SWS_Dem_00761]*/
        }
        iloop++;
    }
#else
    DEM_UNUSED(Obdmid);
#endif
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 01 computed by the Dem.
 * ServiceId           0x61
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 01 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID01(
uint8* PID01value
)
{
    uint16 iloop = 0;
    uint8  ReadinessGroupIndex = 0;
    uint8  supportedGroups[2];
    uint8  notCompletedGroups[2];
    uint16 obdDtc;
    uint16  obdDtcRef;
    uint8  OBDDTCNum = 0;
    uint16  tempRef;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID01, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID01value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID01, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    Dem_MemSet(PID01value, 0u, (uint8)4U);
    Dem_MemSet(supportedGroups, 0u, (uint8)2U);
    Dem_MemSet(notCompletedGroups, 0u, (uint8)2U);
    /* Byte A */
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if (DemPbCfgPtr->DemEventParameter[iloop].DemEventAvailable == TRUE)/*SWS_Dem_01103] */
        {
            tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
            obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
            /*SWS_Dem_01101]*/
            if (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
            { /* 0xFFFF33 <= WWH-OBD <= 0xFFFFFF */
                if ((((DemPbCfgPtr->DemDTC[tempRef].DemDtcValue < 0xFFFF33UL) && (DemPbCfgPtr->DemDTC[tempRef].DemDtcValue > 0UL))
                        || (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                         && (obdDtcRef != DEM_OBD_DTC_INVALID))
                {/*SWS_Dem_01137]*/
                    if ((OBDDTCNum < 0x7Fu) && (0x00U != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_CDTC))) /*confirmedDTC 0x08*/
                    {/* ConfirmedDtcNum : Byte A, bit 0-6 */
                        OBDDTCNum++;  /*number of OBD confirmed faults [SWS_Dem_00351] */
                    }
                }
            }
        }
        iloop++;
    }
    PID01value[0] = OBDDTCNum;/*SWS_Dem_00351]*/
    if (OBDMilStatus != DEM_INDICATOR_OFF)
    {/* MIL status : Byte A, bit 7 */
        PID01value[0] |= (uint8)0x80U;/*SWS_Dem_01138] */
    }
    /* Byte B, C, D*/
    while (ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FOROBD)
    {/*SWS_Dem_00354]*/
        iloop = 0;
        while (iloop < DEM_EVENT_PARAMETER_NUM)
        {
            if (DemPbCfgPtr->DemEventParameter[iloop].DemEventAvailable == TRUE)/*SWS_Dem_01103] */
            {
                tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
                obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
                /*SWS_Dem_01101]*/
                if (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
                {
                    if (DemPbCfgPtr->DemObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex)
                    {
                        obdDtc = DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue;
                        if ((obdDtc != 0xFFFFU) && (obdDtc != 0u) && (obdDtcRef != DEM_OBD_DTC_INVALID))
                        {
                            DEM_BITS_SET(supportedGroups, ReadinessGroupIndex);
                            if (((0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TF))
                                    && (0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TNCSLC)))
                                    || (0x00u != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_WIR)))
                            {/*SWS_Dem_00354]*/
                                ;
                            }
                            else
                            {
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
            iloop++;
        }
        ReadinessGroupIndex++;
    }
    PID01value[1] = ((supportedGroups[0u] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
    PID01value[2] = ((uint8)((supportedGroups[0u] & 0xF0u) >> 4u) | (uint8)((supportedGroups[1u] & 0xFu) << 4u));
    PID01value[3] = ((uint8)((notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint8)((notCompletedGroups[1u] & 0xFu) << 4u));
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 1C computed by the Dem.
 * ServiceId           0x63
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 1C computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*OBD requirements to which vehicle is designed*/
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID1C(
uint8* PID1Cvalue
)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID1C, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID1Cvalue)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID1C, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    PID1Cvalue[0] = DemGeneralOBD.DemOBDCompliancy; /*SWS_Dem_00748]*/
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 21 computed by the Dem.
 * ServiceId           0x64
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 21 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID21(
uint8* PID21value
)
{
    uint16 DistWithMilOn = 0U;
    uint16 currentDistanceInformation = 0U;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID21, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID21value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID21, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    if (SetDataOfPid21 == TRUE)
    {
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)/*SWS_Dem_00704]*/
        /* MIL deactivated */
        if (DEM_INDICATOR_OFF != OBDMilStatus)       /*the indicator is on*/
        {
            currentDistanceInformation = Dem_ReadDistanceInformation();
            if (currentDistanceInformation > OBDDistanceMILLastOn)
            {
                DistWithMilOn = currentDistanceInformation - OBDDistanceMILLastOn;
                OBDDistanceMILLastOn = currentDistanceInformation;
            }
        }
        if ((OBDDistanceMILOn + DistWithMilOn) < 0xFFFFu)
        {
            OBDDistanceMILOn += DistWithMilOn;
        }
        else
        {
            OBDDistanceMILOn = 0xFFFFu;
        }
        PID21value[0] = (uint8)OBDDistanceMILOn;
        PID21value[1] = (uint8)(OBDDistanceMILOn >> 8u);
#endif
    }
    else
    {
        PID21value[0] = 0xFFu;
        PID21value[1] = 0xFFu;/*SWS_Dem_01099]*/
    }
    SetDataOfPid21 = FALSE;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Service to report the value of PID30 computed by the Dem.
 * ServiceId           0x65
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 30 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID30(
uint8* PID30value
)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID30, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID30value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID30, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    *PID30value = WarmUpCycleCounter;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Service to report the value of PID 31 computed by the Dem.
 * ServiceId           0x66
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 31 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID31(
uint8* PID31value
)
{
   uint16 currentDistanceInformation = 0U;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID31, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID31value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID31, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)/*SWS_Dem_00704]*/
    /* Read current distance information */
    currentDistanceInformation = Dem_ReadDistanceInformation();
    if (currentDistanceInformation > OBDDistanceDTCClear)
    {
        DistSinceDtcCleared = currentDistanceInformation - OBDDistanceDTCClear;
    }
    PID31value[0] = (uint8)DistSinceDtcCleared;
    PID31value[1] = (uint8)(DistSinceDtcCleared >> 8u);
#endif
   return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Service to report the value of PID 41 computed by the Dem.
 * ServiceId           0x67
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 41 computed by the Dem.>
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID41(
uint8* PID41value
)
{
    uint16 iloop = 0;
    uint16 ReadinessGroupIndex = 0;
    uint8  enabledGroups[2];
    uint8  notCompletedGroups[2];
    uint16 obdDtc;
    uint16  obdDtcRef;
    uint16  tempRef;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID41, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID41value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID41, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    Dem_MemSet(PID41value, 0u, (uint8)4U);
    enabledGroups[0] = 0xffU;
    enabledGroups[1] = 0x07U;
    notCompletedGroups[0] = 0;
    notCompletedGroups[1] = 0;

    while (ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FOROBD)
    {
        iloop = 0;
        while (iloop < DEM_EVENT_PARAMETER_NUM)
        {
            if (DemPbCfgPtr->DemEventParameter[iloop].DemEventAvailable == TRUE)/*SWS_Dem_01103] */
            {
                tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
                obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
                if (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
                {/*SWS_Dem_01101]*/
                    if (DemPbCfgPtr->DemObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex)
                    {
                        /* Only include emission related DTCs */
                        obdDtc = DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue;
                        if ((obdDtc != 0xFFFFU) && (obdDtc != 0u) &&(obdDtcRef != DEM_OBD_DTC_INVALID))
                        {
                            pEvent = Dem_GetEventInfo(iloop);
                            if ((0u != (DEM_BITS_ISSET(DemOperationCycleStatus,DemPbCfgPtr->DemEventParameter[iloop].DemOperationCycleRef)))
                                    && (DemOperationCycle[DemPbCfgPtr->DemEventParameter[iloop].DemOperationCycleRef].DemOperationCycleType == DEM_OPCYC_POWER))
                            {
                                if (0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
                                {
                                    /* [SWS_Dem_00356] [SWS_Dem_00348] */
                                    DEM_BITS_CLR(enabledGroups, ReadinessGroupIndex);
                                }
                            }
                            if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))
                            {/*SWS_Dem_00355]*/
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
            iloop++;
        }
        ReadinessGroupIndex++;
    }
    PID41value[0] = 0x00;
    PID41value[1] = ((enabledGroups[0] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
    PID41value[2] = ((uint8)((enabledGroups[0u] & 0xF0u) >> 4u) | (uint8)((enabledGroups[1u] & 0xFu) << 4u));
    PID41value[3] = ((uint8)((notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint8)((notCompletedGroups[1u] & 0xFu) << 4u));
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4D computed by the Dem.
 * ServiceId           0x68
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Buffer containing the contents of PID 4D computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID4D(
uint8* PID4Dvalue
)
{
	uint16 TimeWithMilOn  = 0U;
    uint16 CurrentTimeInformation = 0U;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4D, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID4Dvalue)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4D, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)/*SWS_Dem_00704]*/
    /* MIL deactivated */
    if (DEM_INDICATOR_OFF != OBDMilStatus)       /*the indicator is on*/
    {
        CurrentTimeInformation = Dem_ReadTimeInformation();
        if (CurrentTimeInformation > OBDTimeMILLastOn)
        {
            TimeWithMilOn = CurrentTimeInformation - OBDTimeMILLastOn;
            OBDTimeMILLastOn = CurrentTimeInformation;
        }
    }
    if ((OBDTimeMILOn + TimeWithMilOn) < 0xFFFFu)
    {
        OBDTimeMILOn += TimeWithMilOn;
    }
    else
    {
        OBDTimeMILOn = 0xFFFFu;
    }
    PID4Dvalue[0] = (uint8)OBDTimeMILOn;
    PID4Dvalue[1] = (uint8)(OBDTimeMILOn >> 8u);
#endif
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Service to report the value of PID 4E computed by the Dem.
 * ServiceId           0x69
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue Buffer containing the contents of PID 4E
                        computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID4E(
uint8* PID4Evalue
)
{
	uint16 CurrentTimeInformation = 0U;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4E, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID4Evalue)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID4E, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)/*SWS_Dem_00704]*/
    /* MIL still activated */
    CurrentTimeInformation = Dem_ReadTimeInformation();
    if (CurrentTimeInformation > OBDTimeDTCLastClear)
    {
        OBDTimeDTCClear = CurrentTimeInformation - OBDTimeDTCLastClear;
    }
    PID4Evalue[0] = (uint8)OBDTimeDTCClear;
    PID4Evalue[1] = (uint8)(OBDTimeDTCClear >> 8u);
#endif
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to report the value of PID 91 computed by the Dem.
 * ServiceId           0x6a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID91value Buffer containing the contents of PID 91
                        computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID91(
uint8* PID91value
)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID91, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID91value)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATDOFPID91, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    return DemReadPID91(PID91value);/*TODO: ECU OBD System Information (5 byte)*/
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets data element per PID and index of the most important freeze frame
                        being selected for the output of service 02. The function stores the data
                        in the provided DestBuffer.
 * ServiceId           0x52
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PID: This parameter is an identifier for a PID as defined
                        in ISO15031-5.
                        DataElementIndexOfPID: Data element index of this PID according
                        to the Dcm configuration of service 02. It is zero-based and
                        consecutive, and ordered by the data element  positions
                        (configured in Dcm, refer to SWS_Dem_00597).>
 * Param-Name[out]     None
 * Param-Name[in/out]  DestBuffer: This parameter contains a byte pointer that points to
                        the buffer, to which the data element of the PID shall
                        be written to. The format is raw hexadecimal values
                        and contains no header-information.
                        BufSize: When the function is called this parameter contains
                        the maximum number of data bytes that can be written
                        to the buffer.
                        The function returns the actual number of written
                        data bytes in this parameter.
 * Return              E_OK Freeze frame data was successfully reported
                        E_NOT_OK Freeze frame data was not successfully reported
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfOBDFreezeFrame(
uint8 PID,
uint8 DataElementIndexOfPID,
uint8* DestBuffer,
uint16* BufSize
)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (DEM_PID_CLASS_NUM > 0)
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer = NULL_PTR;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = NULL_PTR;
    uint16 IntID = 0;
    uint8 DataElementIndex = 0;
    uint8 pid_num = 0;
    P2CONST(Dem_PidClassType, AUTOMATIC, DEM_CONST) pPid;
    uint16 obdDtcRef;
    uint16 tempRef;
    uint8 cnt;
#endif
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATAOFOBDFREEZEFRAME, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMREADDATAOFOBDFREEZEFRAME, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
#if (DEM_PID_CLASS_NUM > 0)
    pEntry = Dem_SelectOBDFreezeFrame(&IntID);
    obdDtcRef = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemObdDTCRef;
    tempRef = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].DemDTCAttributesRef;
    if((pEntry != NULL_PTR)
            && ((obdDtcRef != DEM_OBD_DTC_INVALID) && ((DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)))
            && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemFreezeFrameClassRef == DEM_FREEZE_FRAME_INVALID)
            && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemWWHOBDFreezeFrameClassRef == DEM_WWHOBD_FREEZE_FRAME_INVALID)
            && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemJ1939FreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID)
            && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemJ1939ExpandedFreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID))
    {
        /*SWS_Dem_01101]*/
        if (DemDTCGeneralStatus[DemPbCfgPtr->DemEventParameter[IntID].DemDTCRef].SuppressionStatus == TRUE)
        {
            retVal = E_NOT_OK;
        }
        else
        {
			pBuffer = &(pEntry -> FFList[0]).Data[0];
			while (pid_num < DEM_PID_CLASS_NUM)
			{
				pPid = &DemPbCfgPtr->DemPidClass[pid_num];
				if (PID == pPid->DemPidIdentifier)
				{
					while (DataElementIndex < pPid->RefNum)
					{
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
						if (DataElementIndexOfPID == (pPid->StartIndex + DataElementIndex))
						{
							Dem_MemCopy(DestBuffer, pBuffer, DemDataElementClass[pPid->StartIndex + DataElementIndex].DemDataElementDataSize);
							retVal = E_OK;
							*BufSize += DemDataElementClass[pPid->StartIndex + DataElementIndex].DemDataElementDataSize;
						}
						for (cnt = 0;cnt<DemDataElementClass[pPid->StartIndex + DataElementIndex].DemDataElementDataSize;cnt++)
						{
							pBuffer++;
						}
#endif
						DataElementIndex++;
					}
				}
				else
				{
					for (cnt = 0;cnt<pPid->DataSize;cnt++)
					{
						pBuffer++;
					}
				}
				pid_num++;
			}
        }
    }
#endif
    return retVal;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Gets DTC by freeze frame record number.
 * ServiceId           0x53
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FrameNumber: Unique identifier for a freeze frame
                        record as defined in ISO 15031-5. The value 0x00
                        indicates the complete OBD freeze frame. Other values
                        are reserved for future functionality.
                        DTCFormat: Output format of the DTC value.
 * Param-Name[out]     DTC Diagnostic Trouble Code in ODB format. If the return
                        value of the function is other than E_OK this
                        parameter does not contain valid data.
 * Param-Name[in/out]  None
 * Return              E_OK: operation was successful E_NOT_OK: no DTC available
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetDTCOfOBDFreezeFrame(
uint8 FrameNumber,
uint32* DTC,
Dem_DTCFormatType DTCFormat
)
{
    Std_ReturnType retVal = E_NOT_OK;
    Dem_EventIdType IntId = 0;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;

    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (DTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    /* Only record number 0 is supported, SWS_Dem_00291 */
    if ((0u == FrameNumber) && (DEM_DTC_FORMAT_OBD == DTCFormat))
    {
        if (Dem_SelectOBDFreezeFrame(&IntId) != NULL_PTR)/*SWS_Dem_01172] [SWS_Dem_01173]*/
        {
            pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
           /* OBD Freeze frame stored, return DTC that caused storage */
            /*SWS_Dem_01101]*/
        	if (pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
        	{
				if (DemDTCGeneralStatus[pEventCfg->DemDTCRef].SuppressionStatus == TRUE)
				{
					retVal = E_NOT_OK;
				}
				else
				{
					*DTC = (uint32)DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].DemObdDTCRef].DemDtcValue;
					retVal = E_OK;
				}
        	}
        }
        else
        {
           /* No DTC found */
           *DTC = 0;
        }
    }
    return retVal;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Reports a DTR data along with TID-value, UaSID, test result with lower and upper limit.
 * ServiceId           0xa5
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Obdmid: Identification of a DTR element by assigned DTRId.
                        TIDindex: Index of the TID within the DEM. Runs from 0
                        to "numberOfTIDs" obtained in the call to Dem_
                        DcmGetNumTIDsOfOBDMID()
 * Param-Name[out]     TIDvalue: TID to be put on the tester reponse
                        UaSID: UaSID to be put on the tester reponse
                        Testvalue:Latest test result
                        Lowlimvalue:Lower limit value associated to the latest test result
                        Upplimvalue:Upper limit value associated to the latest test result
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetDTRData(
uint8 Obdmid,
uint8 TIDindex,
uint8* TIDvalue,
uint8* UaSID,
uint16* Testvalue,
uint16* Lowlimvalue,
uint16* Upplimvalue
)
{
#if (DEM_DTR_NUM > 0)
    uint16 iloop = 0;
#endif
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if ((TIDvalue == NULL_PTR) || (UaSID == NULL_PTR) || (Testvalue == NULL_PTR) || (Lowlimvalue == NULL_PTR) || (Upplimvalue == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCOFOBDFREEZEFRAME, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
#if (DEM_DTR_NUM > 0)
    while (iloop < DEM_DTR_NUM)
    {
        if ((Obdmid == DemPbCfgPtr->DTR[iloop].DemDtrMid) && (TIDindex == iloop))
        {/*SWS_Dem_00762]*/
            *TIDvalue = DemPbCfgPtr->DTR[iloop].DemDtrTid;
            *UaSID = DemPbCfgPtr->DTR[iloop].DemDtrUasid;
            *Lowlimvalue = (uint16)DTRInfo[iloop].LowerLimit;
            *Testvalue = (uint16)DTRInfo[iloop].TestResult;
            *Upplimvalue = (uint16)DTRInfo[iloop].UpperLimit;
        }
        iloop++;
    }
#else
    DEM_UNUSED(Obdmid);
    DEM_UNUSED(TIDindex);
#endif
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/*************************************************************************/
/*
 * Brief               Service for reporting the event as disabled to the
                        Dem for the PID 41 computation.
 * ServiceId           0x51
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different EventIds. Non reentrant for the same EventId.
 * Param-Name[in]      Identification of an event by assigned EventId.>
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK set of event to disabled was successfull.
                         E_NOT_OK set of event disabled failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventDisabled(
    Dem_EventIdType EventId
    )
{
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    EventId = Dem_GetEventInternalId(EventId);
    pEvent = Dem_GetEventInfo(EventId);
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTDISABLED, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (EventId >= DEM_EVENT_PARAMETER_NUM)
    {
        DEM_DET_REPORT(DEM_SID_SETEVENTDISABLED, DEM_E_WRONG_CONFIGURATION);
        return E_NOT_OK;
    }
    DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION);/*SWS_Dem_00294]*/
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service for reporting that faults are possibly found
                        because all conditions are fullfilled.
 * ServiceId           0x73
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Ratio Identifier reporting that a respective monitor
                        could have found a fault
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR result was successfully reported
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRFaultDetect(Dem_RatioIdType RatioID)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRFAULTDETECT, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (RatioID > DEM_RATIO_NUM)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRFAULTDETECT, DEM_E_WRONG_CONFIGURATION);
        return E_NOT_OK;
    }
#if(DEM_RATIO_NUM > 0)
    if (DemRatio[RatioID].DemRatioKind == DEM_RATIO_API)
    {
        IUMPRValue[RatioID].IUMPRNumStatus =TRUE;/*malfunction could have been found.*/
    }
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to forward
                        the condition status to a Dem of a particular ECU
 * ServiceId           0xae
 * Sync/Async          Synchronous /Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
                        ConditionStatus:Status of the IUMPR denominator condition
                        (Notreached, reached, not reachable / inhibited)
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: set of IUMPR denominator condition was successful
                        E_NOT_OK: set of IUMPR denominator condition
                        failed or could not be accepted.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetIUMPRDenCondition(
        Dem_IumprDenomCondIdType ConditionId,
        Dem_IumprDenomCondStatusType ConditionStatus
)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETIUMPRDENCONDITION, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (ConditionId >= 5u)
    {
        DEM_DET_REPORT(DEM_SID_SETIUMPRDENCONDITION, DEM_E_PARAM_DATA);
        return E_NOT_OK;
    }

    ConditionStatu[ConditionId] = ConditionStatus;/*SWS_Dem_00714] */
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               In order to communicate the status of the (additional) denominator
                        conditions among the OBD relevant ECUs, the API is used to retrieve
                        the condition status from the Dem of the ECU where the conditions are
                        computed.
 * ServiceId           0xaf
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ConditionId: Identification of a IUMPR denominator condition ID
 * Param-Name[out]     ConditionStatus:Status of the IUMPR denominator condition
 *                      (Notreached, reached, not reachable / inhibited)
 * Param-Name[in/out]  None
 * Return              E_OK: get of IUMPR denominator condition status was successful
                        E_NOT_OK: get of condition status failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetIUMPRDenCondition(
        Dem_IumprDenomCondIdType ConditionId,
        Dem_IumprDenomCondStatusType* ConditionStatus
)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_GETIUMPRDENCONDITION, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (ConditionId >= 5u)
    {
        DEM_DET_REPORT(DEM_SID_GETIUMPRDENCONDITION, DEM_E_PARAM_DATA);
        return E_NOT_OK;
    }
    *ConditionStatus = ConditionStatu[ConditionId];/*SWS_Dem_00966]*/
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service is used to lock a denominator of a specific monitor.
 * ServiceId           0x71
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is locked
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRDenLock(Dem_RatioIdType RatioID)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENLOCK, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (RatioID > DEM_RATIO_NUM)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENLOCK, DEM_E_WRONG_CONFIGURATION);
        return E_NOT_OK;
    }
#if(DEM_RATIO_NUM > 0)
    if (DEM_IUMPR_DEN_PHYS_API == DemRatio[RatioID].DemIUMPRDenGroup)/*SWS_Dem_00362] */
    {
        IUMPRValue[RatioID].IUMPRDenStatus = FALSE;
    }
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service is used to release a denominator of a
                        specific monitor.
 * ServiceId           0x72
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Ratio Identifier reporting that specific denominator
                        is released
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK report of IUMPR denominator status was successfully reported
                        E_NOK report of IUMPR denominator status was
                        not successfully reported
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRDenRelease(Dem_RatioIdType RatioID)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENRELEASE, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (RatioID > DEM_RATIO_NUM)
    {
        DEM_DET_REPORT(DEM_SID_REPIUMPRDENRELEASE, DEM_E_WRONG_CONFIGURATION);
        return E_NOT_OK;
    }
#if(DEM_RATIO_NUM > 0)
    if (DEM_IUMPR_DEN_PHYS_API == DemRatio[RatioID].DemIUMPRDenGroup)/*SWS_Dem_00362] */
    {
        IUMPRValue[RatioID].IUMPRDenStatus = TRUE;
    }
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_PTO_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Set the status of the PTO.
 * ServiceId           0x79
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PtoStatus sets the status of the PTO
                        TRUE:active    FALSE:inactive
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType Returns E_OK when the new PTO-status has been
                        adopted by the Dem; returns E_NOT_OK in all other cases.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetPtoStatus(boolean PtoStatus)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETPTOSTATUS, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    CurrentPTOStatus = PtoStatus;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*************************************************************************/
/*
 * Brief               Service to report the value of PID 01 computed by the Dem.
 * ServiceId           0xb3
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID01value:Buffer containing the contents of PID 01 computed by the Dem.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ReadDataOfPID01(uint8* PID01value)
{
    uint16 iloop = 0;
    uint8  ReadinessGroupIndex = 0;
    uint8  supportedGroups[2];
    uint8  notCompletedGroups[2];
    uint16 obdDtc;
    uint16  obdDtcRef;
    uint8  OBDDTCNum = 0;
    uint16  tempRef;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_READDATAOFPID01, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID01value)
    {
        DEM_DET_REPORT(DEM_SID_READDATAOFPID01, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    Dem_MemSet(PID01value, 0u, (uint8)4U);
    Dem_MemSet(supportedGroups, 0u, (uint8)2U);
    Dem_MemSet(notCompletedGroups, 0u, (uint8)2U);
    /* Byte A */
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if (DemPbCfgPtr->DemEventParameter[iloop].DemEventAvailable == TRUE)/*SWS_Dem_01103] */
        {
            tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
            obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
            /*SWS_Dem_01101]*/
            if (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
            {
                if ((((DemPbCfgPtr->DemDTC[tempRef].DemDtcValue < 0xFFFF33UL) && (DemPbCfgPtr->DemDTC[tempRef].DemDtcValue > 0UL))
                        || (DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                        && (obdDtcRef != DEM_OBD_DTC_INVALID))
                {/*SWS_Dem_01137]*/
                    if ((OBDDTCNum < 0x7Fu) && (0x00U != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_CDTC))) /*confirmedDTC 0x08*/
                    {
                        OBDDTCNum++;  /*number of dtcs [SWS_Dem_00351] */
                    }
                }
            }
        }
        iloop++;
    }
    PID01value[0] = OBDDTCNum;/*SWS_Dem_00351]*/
    if (OBDMilStatus != DEM_INDICATOR_OFF)
    {
        PID01value[0] |= (uint8)0x80U;/*SWS_Dem_01138] */
    }
    /* Byte B, C, D*/
    while (ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FOROBD)  /*in total 11 groups,n events assigned to the group*/
    {/*SWS_Dem_00354]*/
        iloop = 0;
        while (iloop < DEM_EVENT_PARAMETER_NUM)
        {
            if (DemPbCfgPtr->DemEventParameter[iloop].DemEventAvailable == TRUE)/*SWS_Dem_01103] */
            {
                tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
                obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
                /*SWS_Dem_01101]*/
                if (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
                {
                    if (DemPbCfgPtr->DemObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex)
                    {
                        obdDtc = DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue;
                        if ((obdDtc != 0xFFFFU) && (obdDtc != 0u) && (obdDtcRef != DEM_OBD_DTC_INVALID))
                        {
                            DEM_BITS_SET(supportedGroups, ReadinessGroupIndex);
                            if (((0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TF))
                                    && (0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TNCSLC)))
                                    || (0x00u != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_WIR)))
                            {/*SWS_Dem_00354]*/
                                ;
                            }
                            else
                            {
                                DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
                            }
                        }
                    }
                }
            }
            iloop++;
        }
        ReadinessGroupIndex++;
    }
    PID01value[1] = ((supportedGroups[0u] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
    PID01value[2] = ((uint8)((supportedGroups[0u] & 0xF0u) >> 4u) | (uint8)((supportedGroups[1u] & 0xFu) << 4u));
    PID01value[3] = ((uint8)((notCompletedGroups[0u] & 0xF0u) >> 4u) | (uint8)((notCompletedGroups[1u] & 0xFu) << 4u));
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 21 in the Dem by a software component.
 * ServiceId           0xa6
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PID21value:Buffer containing the contents of PID 21.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID21(uint8* PID21value)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID21value)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    OBDDistanceMILOn = (uint16)PID21value[0u] | (uint16)((uint16)PID21value[1u] << 8u);
    SetDataOfPid21 = TRUE;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 31 in the Dem by a software component
 * ServiceId           0xa7
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID31value:Buffer containing the contents of PID 31.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID31(uint8* PID31value)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID31, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID31value)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID31, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    DistSinceDtcCleared = (uint16)PID31value[0] | (uint16)((uint16)PID31value[1] << 8u);
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 4D in the Dem by a software component.
 * ServiceId           0xa8
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Dvalue:Buffer containing the contents of PID 4D.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID4D(uint8* PID4Dvalue)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4D, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID4Dvalue)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4D, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    OBDTimeMILOn = (uint16)PID4Dvalue[0u] | (uint16)((uint16)PID4Dvalue[1u] << 8u);
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Service to set the value of PID 4E in the Dem by a software component.
 * ServiceId           0xa9
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID4Evalue:Buffer containing the contents of PID 4E.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetDataOfPID4E(uint8* PID4Evalue)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4E, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID4Evalue)
    {
        DEM_DET_REPORT(DEM_SID_SETDATAOFPID4E, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    OBDTimeDTCClear = (uint16)PID4Evalue[0u] | (uint16)((uint16)PID4Evalue[1u] << 8u);
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Marks the current OBD driving cycle as having met the criteria for the PFC cycle.
 * ServiceId           0xaa
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetPfcCycleQualified(void)
{
    uint8 iloop = 0;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETPFCCYCLEQUALIFIED, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    PFCStatu = FALSE;
    while (iloop < DEM_OPERATION_CYCLE_NUM)
    {
        if (DemOperationCycle[iloop].DemOperationCycleType == DEM_OPCYC_OBD_DCY)
        {
            PFCStatu = TRUE;
        }
        iloop++;
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Returns TRUE if the criteria for the PFC cycle have been met during the current OBD driving cycle.
 * ServiceId           0xab
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     isqualified:TRUE: During the current OBD driving cycle the criteria for the PFC cycle have been met.
                        FALSE: During the current OBD driving cycle the criteria for the PFC cycle have not been met.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetPfcCycleQualified(boolean* isqualified)
{
    uint8 iloop = 0;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_GETPFCCYCLEQUALIFIED, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == isqualified)
    {
        DEM_DET_REPORT(DEM_SID_GETPFCCYCLEQUALIFIED, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    while (iloop < DEM_OPERATION_CYCLE_NUM)
    {
        if ((DemOperationCycle[iloop].DemOperationCycleType == DEM_OPCYC_OBD_DCY)
                && (0x00u != DEM_BITS_ISSET(DemOperationCycleStatus,iloop))
                && ((DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
                        && (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] != DEM_INDICATOR_OFF)))
        {
            PFCStatu = TRUE;
        }
        iloop++;
    }
    *isqualified = PFCStatu;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/*************************************************************************/
/*
 * Brief               API to inform the Dem (of a Dependend / Secondary ECU) about the
                       reception of service 04 execution by a software component.
 * ServiceId           0xac
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that has been
 *                     cleared from the event memory.
 *                       DTCFormat:Format of the provided DTC value.
 *                       DTCOrigin:Event memory (e.g. MIRROR)
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Return value unused - only for compatibility with according RTE operation.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_SetClearDTC(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin
    )
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr;

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_SETCLEARDTC, DEM_E_UNINIT);
        return DEM_CLEAR_FAILED;
    }

    pClr = &DemClearDTCInfo;
    if (DemClearDTCLock == DEM_CLEAR_LOCK)
    {
        if ( (pClr->SaveDTCIndex == Dem_GetDTCIndex(DTC, DTCFormat)) &&
        (pClr->SaveDTCFormat == DTCFormat) &&
        (pClr->SaveDTCOrigin == DTCOrigin) &&
        (pClr->SID == DEM_SID_SETCLEARDTC))
        {
            /*SWS_Dem_00663]*/
            return DEM_CLEAR_PENDING;
        }
        else
        {
            /*SWS_Dem_00662] [SWS_Dem_00664]*/
            return DEM_CLEAR_BUSY;
        }
    }
    else if (DemClearDTCLock == DEM_CLEAR_NOT_LOCK)
    {
        DemClearDTCLock = DEM_CLEAR_LOCK;/*SWS_Dem_00661]*/
        pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
        pClr->DTCFormat = DTCFormat;
        pClr->DTCOrigin = DTCOrigin;
        pClr->SaveDTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
        pClr->SaveDTCFormat = DTCFormat;
        pClr->SaveDTCOrigin = DTCOrigin;
        pClr->SID = DEM_SID_SETCLEARDTC;
    }
    else
    {
        ;
    }
    /*SWS_Dem_00670] */
#if (DEM_CLEAR_DTCLIMITATION == DEM_ONLY_CLEAR_ALL_DTCS)
    if (DTCFormat != DEM_DTC_FORMAT_UDS || (DTC & 0xFFFFFF) != 0xFFFFFF)
    {
        Dem_ResetDemClearDTCInfo();
        return  DEM_CLEAR_WRONG_DTC;
    }
#endif

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        Dem_ResetDemClearDTCInfo();
        return DEM_CLEAR_WRONG_DTCORIGIN;
    }

    if (((DTC & 0xFFFFFFUL) == 0xFFFFFFUL)
        || ((DTC & 0xFFFFFFUL) == 0xFFFF33UL))/*SWS_Dem_00723][SWS_Dem_01179] */
    {
        pClr->ClearAllGroup = TRUE;
    }
    else
    {
        Dem_ResetDemClearDTCInfo();
        return  DEM_CLEAR_WRONG_DTC;
    }
    pClr->memDest = DTCOrigin;
    Dem_Pending = TRUE;
    Dem_ClearDTCProcess();

    Dem_ResetDemClearDTCInfo();
    return DEM_CLEAR_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief              Reports a DTR result with lower and upper limit. The internal eventstatus
                        serves as master whether the DTR values are forwarded or ignored,
                        also taking the DTRUpdateKind into account.
 * ServiceId           0xa2
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTRId:Identification of a DTR element by assigned DTRId
 *                      TestResult:Test result of DTR
 *                      LowerLimit:Lower limit of DTR
 *                      UpperLimit:Upper limit of DTR
 *                      Ctrlval:Control value of the DTR to support its interpretation Dem-internally.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Report of DTR result successful
                        E_NOT_OK: Report of DTR result failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_SetDTR(
		uint16 DTRId,
        sint32 TestResult,
        sint32 LowerLimit,
        sint32 UpperLimit,
        Dem_DTRControlType Ctrlval
)
{
#if (DEM_DTR_NUM > 0)
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
#endif
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_SETDTR, DEM_E_UNINIT);
        return E_NOT_OK;
    }
#if (DEM_DTR_NUM > 0)
    pEvent = &DemEventRelateInformation[DemPbCfgPtr->DTR[DTRId].DemDtrEventRef];
    if ((DemPbCfgPtr->DTR[DTRId].DemDtrUpdateKind == DEM_DTR_UPDATE_ALWAYS)
        || ((DemPbCfgPtr->DTR[DTRId].DemDtrUpdateKind == DEM_DTR_UPDATE_STEADY)
                && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TNCTOC))))
    {
        switch(Ctrlval)
        {
        case DEM_DTR_CTL_NORMAL:
            DTRInfo[DTRId].TestResult = TestResult;
            DTRInfo[DTRId].LowerLimit = LowerLimit;
            DTRInfo[DTRId].UpperLimit = UpperLimit;
            break;
        case DEM_DTR_CTL_NO_MAX:
            DTRInfo[DTRId].TestResult = TestResult;
            DTRInfo[DTRId].LowerLimit = LowerLimit;
            break;
        case DEM_DTR_CTL_NO_MIN:
            DTRInfo[DTRId].TestResult = TestResult;
            DTRInfo[DTRId].UpperLimit = UpperLimit;
            break;
        case DEM_DTR_CTL_RESET:
            DTRInfo[DTRId].TestResult = 0;
            DTRInfo[DTRId].LowerLimit = 0;
            DTRInfo[DTRId].UpperLimit = 0;
            break;
        default:
            break;
        }
        return E_OK;
    }
#else
	DEM_UNUSED(DTRId);
	DEM_UNUSED(TestResult);
	DEM_UNUSED(LowerLimit);
	DEM_UNUSED(UpperLimit);
	DEM_UNUSED(Ctrlval);
#endif
    return E_NOT_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Get the event ID of FreezeFrame.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Identification of an event by assigned EventId.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR), DEM_CODE)
        Dem_SelectOBDFreezeFrame(P2VAR(Dem_EventIdType,AUTOMATIC, DEM_APPL_DATA) IntId)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) RetpEntry;
    uint16 iloop = 0;
    uint8 SelectTimeA = 0xFF;
    uint8 SelectTimeB1 = 0xFF;
    uint8 SelectTimeB2 = 0xFF;
    uint8 SelectTimeC = 0xFF;
    uint8 SelectTime = 0xFF;
    boolean ClassAFind = FALSE;
    boolean ClassB1Find = FALSE;
    boolean ClassB2Find = FALSE;
    boolean ClassCFind = FALSE;
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        pEntry = Dem_CheckEventMemEntryExistsAlready(iloop);
        if((pEntry != NULL_PTR) && (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemObdDTCRef != DEM_OBD_DTC_INVALID))
        {
            if (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_A)
            {
                if (pEntry->AbsTime < SelectTimeA)/*SWS_Dem_01173] */
                {
                    SelectTimeA = pEntry->AbsTime;
                    *IntId = iloop;
                    RetpEntry = pEntry;
                }
                ClassAFind = TRUE;
            }
            if (ClassAFind == FALSE)
            {
                if (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B1)
                {
                    if (pEntry->AbsTime < SelectTimeB1)/*SWS_Dem_01173] */
                    {
                        SelectTimeB1 = pEntry->AbsTime;
                        *IntId = iloop;
                        RetpEntry = pEntry;
                    }
                    ClassB1Find = TRUE;
                }
                if (ClassB1Find == FALSE)
                {
                    if (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B2)
                    {
                        if (pEntry->AbsTime < SelectTimeB2)/*SWS_Dem_01173] */
                        {
                            SelectTimeB2 = pEntry->AbsTime;
                            *IntId = iloop;
                            RetpEntry = pEntry;
                        }
                        ClassB2Find = TRUE;
                    }
                    if (ClassB2Find == FALSE)
                    {
                        if (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_C)
                        {
                            if (pEntry->AbsTime < SelectTimeC)/*SWS_Dem_01173] */
                            {
                                SelectTimeC = pEntry->AbsTime;
                                *IntId = iloop;
                                RetpEntry = pEntry;
                            }
                            ClassCFind = TRUE;
                        }
                        if (ClassCFind == FALSE)
                        {
                            if (pEntry->AbsTime < SelectTime)/*SWS_Dem_01173] */
                            {
                                SelectTime = pEntry->AbsTime;
                                *IntId = iloop;
                                RetpEntry = pEntry;
                            }
                        }
                    }

                }

            }
        }
        iloop++;
    }
    return RetpEntry;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#if((DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU) && (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON))
/*************************************************************************/
/*
 * Brief               Service to get the value of PID 21 from the Dem by a software component.
 * ServiceId           0xb1
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     PID21value:Content of PID 21 as raw hex value.
 * Param-Name[in/out]  None
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetDataOfPID21(uint8* PID21value)
{/*SWS_Dem_01098] */
    uint16 DistWithMilOn = 0U;
    uint16 currentDistanceInformation = 0U;
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == PID21value)
    {
        DEM_DET_REPORT(DEM_SID_GETDATAOFPID21, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
#if(DEM_OBD_SUPPORT == DEM_OBD_MASTER_ECU)/*SWS_Dem_00704]*/
    /* MIL deactivated */
    if (DEM_INDICATOR_OFF != OBDMilStatus)       /*the indicator is on*/
    {
        currentDistanceInformation = Dem_ReadDistanceInformation();
        if (currentDistanceInformation > OBDDistanceMILLastOn)
        {
            DistWithMilOn = currentDistanceInformation - OBDDistanceMILLastOn;
        }
    }
    if (OBDDistanceMILOn + DistWithMilOn < 0xFFFF)
    {
        OBDDistanceMILOn += DistWithMilOn;
    }
    else
    {
        OBDDistanceMILOn = 0xFFFF;
    }
    PID21value[0] = (uint8)OBDDistanceMILOn;
    PID21value[1] = (uint8)(OBDDistanceMILOn >> 8);
#endif
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * Brief               Gets the DTC Severity availability mask.
 * ServiceId           0xb2
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTCSeverityMask:The value DTCSeverityMask indicates the
 *                      supported DTC severity bits from the Dem.
 * Param-Name[in/out]  none
 * Return              E_OK: get of DTC severity mask was successful
                        E_NOT_OK: get of DTC severity mask failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE)Dem_DcmGetDTCSeverityAvailabilityMask(
        Dem_DTCSeverityType* DTCSeverityMask)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCSEVERITYAVAILABILITYMASK, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == DTCSeverityMask)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETDTCSEVERITYAVAILABILITYMASK, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    *DTCSeverityMask = DemDTCFilterInfo.DTCSeverityMask;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if (DEM_WWWOBD_NUM > 0)
/*************************************************************************/
/*
 * Brief               Service to report the value of the B1 counter computed by the Dem.
 * ServiceId           0xb4
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     B1Counter:Buffer containing the B1 counter.
 * Param-Name[in/out]  none
 * Return              Always E_OK is returned, as E_NOT_OK will never appear.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE) Dem_GetB1Counter(uint16* B1Counter)
{
    if (DEM_STATE_INIT != DemInitState)
    {
        DEM_DET_REPORT(DEM_SID_GETB1COUNTER, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (NULL_PTR == B1Counter)
    {
        DEM_DET_REPORT(DEM_SID_GETB1COUNTER, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    *B1Counter = OBDB1Counter;
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
/*************************************************************************/
/*
 * Brief               Get the Current Distance Information.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(uint16, DEM_CODE) Dem_ReadDistanceInformation(void)    /*read current distance*/
{
    uint8 data[2] = {0};
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
    if ((DemGeneralOBD.DemOBDInputDistanceInformation != DEM_DATA_ELEMENT_CLASS_NUM_INVALID)
        && (DemDataElementClass[DemGeneralOBD.DemOBDInputDistanceInformation].DemDataElementReadFnc != NULL_PTR))
    {
        DemDataElementClass[DemGeneralOBD.DemOBDInputDistanceInformation].DemDataElementReadFnc(data);
    }
#endif
    return ((uint16)data[0u] | (uint16)((uint16)data[1u] << 8u));
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Get the Current Time Since Engine Start.
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(uint16, DEM_CODE) Dem_ReadTimeInformation(void)
{
    uint8 data[2] = {0};
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
    if ((DemGeneralOBD.DemOBDTimeSinceEngineStart != DEM_DATA_ELEMENT_CLASS_NUM_INVALID)
         && (DemDataElementClass[DemGeneralOBD.DemOBDTimeSinceEngineStart].DemDataElementReadFnc != NULL_PTR))
    {
        DemDataElementClass[DemGeneralOBD.DemOBDTimeSinceEngineStart].DemDataElementReadFnc(data);
    }
#endif
    return ((uint16)data[0u] | (uint16)((uint16)data[1u] << 8u));
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if (DEM_DTR_NUM > 0)
/*************************************************************************/
/*
 * Brief               Clear DTRInfo By Clear Command
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ClearDTRInfoByEventID(Dem_EventIdType IntId)
{
    uint16 iloop = 0;
    while (iloop < DEM_DTR_NUM)
    {
        if (IntId == DemPbCfgPtr->DTR[iloop].DemDtrEventRef)
        {
            DTRInfo[iloop].LowerLimit = 0;
            DTRInfo[iloop].TestResult = 0;
            DTRInfo[iloop].UpperLimit = 0;
        }
        iloop++;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Clear DTRInfo By Clear Command
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ClearDTRInfoByDTRID(uint16 DTRID)
{
            DTRInfo[DTRID].LowerLimit = 0;
            DTRInfo[DTRID].TestResult = 0;
            DTRInfo[DTRID].UpperLimit = 0;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if((DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) || (DEM_J1939_SUPPORT == STD_ON))
/*************************************************************************/
/*
 * Brief               Process the B1 Counter
 * ServiceId           Internal Function
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_B1CounterProcess(void)
{
    uint16 iloop = 0;
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if ((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef].DemWWHOBDDTCClass == DEM_DTC_WWHOBD_CLASS_B1)
            && ((DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TF) != 0x00u)
                 && (DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_CDTC) != 0x00u)))
        {/*SWS_Dem_01154]*/
            if (OBDB1Counter < 0xFFu)
            {/*SWS_Dem_01158] */
                OBDB1Counter++;
            }
            if (OBDB1Counter > 200u)
            {
               ContinuousMICounter = 0u;/*SWS_Dem_01141]*/
            }
            break;
        }
        iloop++;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif/*#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)*/

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/

