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
**  FILENAME    : Dem_EventDebounce.c                                         **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : DEM EventDebounce API definitions                           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2018-4-20  tao.yu    Initial version                         */
/*  V1.0.1       2019-9-16  tao.yu    fix some bug,change event callback      */
/*  V1.0.2       2019-12-25  tao.yu    QAC fix     							  */
/*  V1.0.3       2020-1-7   tao.yu    Commercial project problem modification */
/*  V1.0.4       2020-3-10   tao.yu    enable condition has no impact on      */
/* Dem_ResetEventDebounceStatus,  Dem_ResetEventStatus and Dem_<...>ClearDTC. */
/* 	V2.0.0		 2020-07-15  pengfei.zhu	fix bugs by PIM; optimized code   */
/* 	V2.0.1		 2020-09-15  pengfei.zhu	QAC test 						  */
/*******************************************************************************
**                       Version  information                                 **
*******************************************************************************/
#define DEM_EVENTDEBOUNCE_C_AR_MAJOR_VERSION   4U
#define DEM_EVENTDEBOUNCE_C_AR_MINOR_VERSION   2U
#define DEM_EVENTDEBOUNCE_C_AR_PATCH_VERSION   2U
#define DEM_EVENTDEBOUNCE_C_SW_MAJOR_VERSION   2U
#define DEM_EVENTDEBOUNCE_C_SW_MINOR_VERSION   0U
#define DEM_EVENTDEBOUNCE_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"
#if(STD_ON == DEM_TRIGGER_FIM_REPORTS)
#include "FiM.h"
#endif

/*******************************************************************************
**                       Version  check                                       **
*******************************************************************************/
#if (DEM_EVENTDEBOUNCE_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTDEBOUNCE_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_EventDebounce.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                      macros  define                                        **
*******************************************************************************/
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
/* Debounce Counter Base Info Define */

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DebounceCounterInfoType,AUTOMATIC) DemDebounceCounterInfo[DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM];
VAR(FDCInfoType,AUTOMATIC) FDCInfo[DEM_DTC_NUM];
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
VAR(Dem_DebounceTimeInfoType,AUTOMATIC) DemDebounceTimerInfo[DEM_DEBOUNCE_TIME_BASED_EVENT_NUM];
#endif
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void,DEM_CODE)
Dem_DebouceProcessPrePassedCounter(
	P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
	P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter
);
static FUNC(void,DEM_CODE)
Dem_DebouceProcessPreFailedCounter(
	P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
	P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter
);
static FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessCounter(
	P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
	P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg
);
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
static FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessTimer(
    P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg);
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif



/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Dem_DebounceInit
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceInit(void)
{
    uint16 iloop;

#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    iloop = 0;
    while (iloop < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM)
    {
    	DemDebounceCounterInfo[iloop].InternalDebounceCounter = 0;
    	DemDebounceCounterInfo[iloop].CurStatus = DEM_EVENT_STATUS_PREPASSED;
        iloop++;
    }
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    iloop = 0;
    while (iloop < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
    {
    	DemDebounceTimerInfo[iloop].Timeout = 0x00;
    	DemDebounceTimerInfo[iloop].Triggered = FALSE;
    	DemDebounceTimerInfo[iloop].CurStatus = DEM_EVENT_STATUS_PREPASSED;
        iloop++;
    }
#endif
    iloop = 0;
    while (iloop < DEM_DTC_NUM)
    {
        FDCInfo[iloop].FDC = 0;
        FDCInfo[iloop].MaxFDCDuringCurrentCycle = 0;
        FDCInfo[iloop].MaxFDCSinceLastClear = 0;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_DebounceReset
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceReset(Dem_EventIdType IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];

#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer;
#endif

    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:
        if( pEventCfg->AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM )
        {
            pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            pCounter->CurStatus = DEM_EVENT_STATUS_PREPASSED;
            pCounter->InternalDebounceCounter = 0;
        }
        break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:
        if( pEventCfg->AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM )
        {
            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimer->Timeout = 0;
            pTimer->Triggered = FALSE;
            pTimer->CurStatus = DEM_EVENT_STATUS_PREPASSED;
        }
        break;
#endif
        default:
        break;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_GetInteralFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              sint8
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_GetInteralFDC(
		Dem_EventIdType IntId,P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) FaultDetectionCounter)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    Std_ReturnType ret = E_NOT_OK;
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter;
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer;
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST) pTimerCfg;
#endif
#if(DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
    Dem_GetFDCFncType GetFnc;
#endif
    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:/*SWS_Dem_00415*/
            if(  ((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
               &&( (pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM ) )
            {
            pCfgCounter = &DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
            if (pCounter->InternalDebounceCounter > 0)
            {
                *FaultDetectionCounter = (sint8)((pCounter->InternalDebounceCounter * 127)
                    / (sint32)pCfgCounter->DemDebounceCounterFailedThreshold);
            }
            else
            {
                *FaultDetectionCounter = (sint8)((pCounter->InternalDebounceCounter * (-128))
                    / (sint32)pCfgCounter->DemDebounceCounterPassedThreshold);
            }
            ret = E_OK;
            }
            break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:/*SWS_Dem_00427*/
            if(  ((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
               &&( (pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM ) )
            {
            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemPbCfgPtr->DemDebounceTimeBaseClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            switch (pTimer->CurStatus)
            {
                case DEM_EVENT_STATUS_FAILED:
                case DEM_EVENT_STATUS_PREFAILED:
                    *FaultDetectionCounter = (sint8)((pTimer->Timeout * 127UL)
                        / (uint64)pTimerCfg->DemDebounceTimeFailedThreshold);
                    break;
                default:
                    *FaultDetectionCounter = ((sint8)(-1))*((sint8)((pTimer->Timeout * 128UL)
                        / (uint64)pTimerCfg->DemDebounceTimePassedThreshold));
                    break;
            }
            ret = E_OK;
            }
            break;
#endif
#if(DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM > 0)
        case DEM_DEBOUNCE_MONITOR_INTERNAL:/*SWS_Dem_00264] */
            if( (DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_MONITOR_INTERNAL_EVENT_NUM )
            {
                GetFnc = DemGetFDCFncs[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
                *FaultDetectionCounter = 0;
                if (GetFnc != NULL_PTR)
                {/*SWS_Dem_00439] */
                    ret = GetFnc(FaultDetectionCounter);
                    if (E_OK != ret)
                    {
                        *FaultDetectionCounter = 0;
                    }
                }
                else
                {
                    ret = DEM_E_NO_FDC_AVAILABLE;
                }
            }
            break;
#endif
        default:

            break;
    }
    return ret;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_DebounceProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcess(
    P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg =
    &DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEventBuffer->ExtId)];
    Dem_EventStatusType Status = pEventBuffer->Status;

    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:
        Status = Dem_DebounceProcessCounter(pEventBuffer, pEventCfg);
        break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:
        Status = Dem_DebounceProcessTimer(pEventBuffer, pEventCfg);
        break;
#endif
        default:
        break;
    }
    return Status;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_DebounceFreeze
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceFreeze(Dem_EventIdType IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    /*[SWS_Dem_00654]  [SWS_Dem_00677] */
    if((pEventCfg->AlgorithmType == DEM_DEBOUNCE_TIME_BASE)
       &&(pEventCfg->AlgorithmRef < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
       &&(pEventCfg->AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM))
    {
        if (DemPbCfgPtr->DemDebounceTimeBaseClass[pEventCfg->AlgorithmRef].DemDebounceBehavior == DEM_DEBOUNCE_FREEZE)
        {
            DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].IsFreezing = TRUE;
        }
        else
        {
            DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].CurStatus = DEM_EVENT_STATUS_PREPASSED;
            DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].Timeout = 0;
            DemDebounceTimerInfo[pEventCfg->AlgorithmIndex].Triggered = FALSE;
        }
    }
    else if((pEventCfg->AlgorithmType == DEM_DEBOUNCE_COUNTER_BASED)
            &&(pEventCfg->AlgorithmRef < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
            &&(pEventCfg->AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM))
    {
        if (DemPbCfgPtr->DemDebounceCounterBasedClass[pEventCfg->AlgorithmRef].DemDebounceBehavior == DEM_DEBOUNCE_RESET)
        {
            DemDebounceCounterInfo[pEventCfg->AlgorithmIndex].InternalDebounceCounter = 0;
        }
    }
    else
    {
        /*idle*/
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_DebounceTimerMain
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DebounceTimerMain(void)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST) pTimerCfg;

    uint16 iloop = 0;
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer = DemDebounceTimerInfo;

#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
    uint8 OldStatus;
#endif

    while((iloop < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
          &&(DemPbCfgPtr->DemEventParameter[pTimer->IntId].AlgorithmRef < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM))
    {
        if ((pTimer->Triggered == TRUE) && (pTimer->IsFreezing == FALSE))
        {
#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
        	OldStatus = pTimer->CurStatus;
#endif
            if ((pTimer->CurStatus != DEM_EVENT_STATUS_FAILED) && (pTimer->CurStatus != DEM_EVENT_STATUS_PASSED))
            {
                pTimer->Timeout += DEM_TASK_TIME;
            }
            pTimerCfg = &DemPbCfgPtr->DemDebounceTimeBaseClass[DemPbCfgPtr->DemEventParameter[pTimer->IntId].AlgorithmRef];
            pEventCfg = &DemPbCfgPtr->DemEventParameter[pTimer->IntId];
            if ((pTimer->Timeout >= pTimerCfg->DemDebounceTimeFailedThreshold)
                    && ((pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED)))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_FAILED;
                pTimer->Triggered = FALSE;
                if((0x00u != DEM_BITS_ISSET(DemOperationCycleStatus,pEventCfg->DemOperationCycleRef))
                    || (DemOperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType == DEM_OPCYC_OBD_DCY))
                {
                	Dem_EventQueueAddDebounceFailed(&DemEventRelateInformation[pTimer->IntId]);
                	Dem_EventTestFailed(pTimer->IntId, &(pTimer->Dem_EventBuffer));
                }
            }
            else if ((pTimer->Timeout >= pTimerCfg->DemDebounceTimePassedThreshold)
                    && ((pTimer->CurStatus == DEM_EVENT_STATUS_PREPASSED)))
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;
                pTimer->Triggered = FALSE;
                if((0x00u != DEM_BITS_ISSET(DemOperationCycleStatus,pEventCfg->DemOperationCycleRef))
                    || (DemOperationCycle[pEventCfg->DemOperationCycleRef].DemOperationCycleType == DEM_OPCYC_OBD_DCY))
                {
                	Dem_EventTestPassed(pTimer->IntId, &(pTimer->Dem_EventBuffer));
                }
            }
            else
            {
                /*idle*/
            }
            Dem_UpdateFDC(pTimer->IntId);
            Dem_UpdateInternalData( pTimer->IntId );

#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
        if (OldStatus != pTimer->CurStatus)
        {
        	(void)FiM_DemTriggerOnEventStatus(pTimer->IntId + 1u,OldStatus,pTimer->CurStatus);
        }
#endif
        }
        pTimer++;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)

static FUNC(void,DEM_CODE)
Dem_DebouceProcessPrePassedCounter(
P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter
)
{
    switch (pCounter->CurStatus)
    {
        case DEM_EVENT_STATUS_FAILED: /* Failed => PrePassed */
        case DEM_EVENT_STATUS_PREFAILED: /* PreFailed => PrePassed */
            if (pCfgCounter->DemDebounceCounterJumpDown == TRUE)/*SWS_Dem_00423*/
            {
                if(pCounter->InternalDebounceCounter > pCfgCounter
                                     ->DemDebounceCounterJumpDownValue)
                {
                    pCounter->InternalDebounceCounter = pCfgCounter
                                     ->DemDebounceCounterJumpDownValue;
                }
            }
        case DEM_EVENT_STATUS_PREPASSED: /* PrePassed => PrePassed SWS_Dem_00419*/
            if ((pCfgCounter->DemDebounceCounterPassedThreshold +
            		((sint16)pCfgCounter->DemDebounceCounterDecrementStepSize))
                >= (sint16)pCounter->InternalDebounceCounter)
            {
                pCounter->InternalDebounceCounter = pCfgCounter
                    ->DemDebounceCounterPassedThreshold;
            }
            else
            {
                pCounter->InternalDebounceCounter -= (sint16)pCfgCounter
                    ->DemDebounceCounterDecrementStepSize;
            }
            break;
        default: /* Passed do nothing */
            break;
    }
}


static FUNC(void,DEM_CODE)
Dem_DebouceProcessPreFailedCounter(
P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter,
P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter
)
{
    switch (pCounter->CurStatus)
    {
        case DEM_EVENT_STATUS_PASSED: /* Passed => Prefailed */
        case DEM_EVENT_STATUS_PREPASSED: /* PrePassed => Prefailed */
            if (pCfgCounter->DemDebounceCounterJumpUp == TRUE)/*SWS_Dem_00425*/
            {
                if(pCounter->InternalDebounceCounter < pCfgCounter
                                        ->DemDebounceCounterJumpUpValue)
                {
                    pCounter->InternalDebounceCounter = pCfgCounter
                                        ->DemDebounceCounterJumpUpValue;
                }
            }
        case DEM_EVENT_STATUS_PREFAILED: /* Prefailed => Prefailed SWS_Dem_00418 */
            if ((pCfgCounter->DemDebounceCounterFailedThreshold -
            		((sint16)pCfgCounter->DemDebounceCounterIncrementStepSize))
                <= (sint16)pCounter->InternalDebounceCounter)
            {
                pCounter->InternalDebounceCounter = pCfgCounter
                    ->DemDebounceCounterFailedThreshold;
            }
            else
            {
                pCounter->InternalDebounceCounter += (sint16)pCfgCounter
                    ->DemDebounceCounterIncrementStepSize;
            }
            break;
        default: /* Failed do nothing */
            break;
    }
}


/*************************************************************************/
/*
 * Brief               Dem_DebounceProcessCounter
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer && pEventCfg
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_EventStatusType, DEM_CODE)
Dem_DebounceProcessCounter(
P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter =
        &DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEventBuffer->ExtId)].AlgorithmRef];
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter=&DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
    Dem_EventStatusType Status = pEventBuffer->Status;

    switch (Status)
    {
        case DEM_EVENT_STATUS_PASSED:/*SWS_Dem_00421*/
            pCounter->InternalDebounceCounter = pCfgCounter
                ->DemDebounceCounterPassedThreshold;
            break;
        case DEM_EVENT_STATUS_FAILED:/*SWS_Dem_00420] */
            pCounter->InternalDebounceCounter = pCfgCounter
                ->DemDebounceCounterFailedThreshold;
            break;
        case DEM_EVENT_STATUS_PREPASSED:
            {
            	Dem_DebouceProcessPrePassedCounter(pCfgCounter,pCounter);
            }
            break;
        default: /* DEM_EVENT_STATUS_PREFAILED */
        	{
        		Dem_DebouceProcessPreFailedCounter(pCfgCounter,pCounter);
        	}
            break;
    }

    if (pCounter->InternalDebounceCounter
        >= pCfgCounter->DemDebounceCounterFailedThreshold)
    {
        pCounter->InternalDebounceCounter = pCfgCounter
            ->DemDebounceCounterFailedThreshold;
        pCounter->CurStatus = DEM_EVENT_STATUS_FAILED;
        Status = DEM_EVENT_STATUS_FAILED;
    }
    else if (pCounter->InternalDebounceCounter
        <= pCfgCounter->DemDebounceCounterPassedThreshold)
    {
        pCounter->InternalDebounceCounter = pCfgCounter
            ->DemDebounceCounterPassedThreshold;
        pCounter->CurStatus = DEM_EVENT_STATUS_PASSED;
        Status = DEM_EVENT_STATUS_PASSED;
    }
    else
    {
        pCounter->CurStatus = Status;
    }
    Dem_UpdateFDC(Dem_GetEventInternalId(pEventBuffer->ExtId));
    Dem_UpdateInternalData( Dem_GetEventInternalId(pEventBuffer->ExtId) );

    return Status;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               Dem_DebounceProcessTimer
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEventBuffer && pEventCfg
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_EventStatusType, DEM_CODE) Dem_DebounceProcessTimer(
    P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg)
{
    Dem_EventStatusType Status = pEventBuffer->Status;
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer=
        &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST) pTimerCfg=
            &DemPbCfgPtr->DemDebounceTimeBaseClass[DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEventBuffer->ExtId)].AlgorithmRef];


    uint8 chkmask = DEM_EVENT_STATUS_ENABLED_CONDICTION
        | DEM_EVENT_STATUS_DTC_SETTING;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;

    pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEventBuffer->ExtId));
    if (pTimer->IsFreezing != FALSE)
    {
        /*SWS_Dem_00656*/
        if (chkmask == DEM_FLAGS_ISSET(pEvent->Status, chkmask))
        {
            pTimer->IsFreezing = FALSE;
        }
    }
    switch (Status)
    {
        case DEM_EVENT_STATUS_PASSED:
            pTimer->CurStatus = DEM_EVENT_STATUS_PASSED;/*SWS_Dem_00435*/
            pTimer->Timeout = pTimerCfg->DemDebounceTimePassedThreshold;
            break;
        case DEM_EVENT_STATUS_FAILED:
            pTimer->CurStatus = DEM_EVENT_STATUS_FAILED;/*SWS_Dem_00431*/
            pTimer->Timeout = pTimerCfg->DemDebounceTimeFailedThreshold;
            break;
        case DEM_EVENT_STATUS_PREPASSED:/*SWS_Dem_00432*/
            if ((pTimer->CurStatus == DEM_EVENT_STATUS_FAILED)
                || (pTimer->CurStatus == DEM_EVENT_STATUS_PREFAILED)
				|| (pTimer->CurStatus == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED)
                || (pTimer->Triggered == FALSE))/*SWS_Dem_00433*/
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_PREPASSED;
                pTimer->Triggered = TRUE;
                pTimer->Timeout = 0;
            }
            break;
        case DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED:
        	break;
        case DEM_EVENT_STATUS_PREFAILED: /* Prefailed */
            if ((pTimer->CurStatus == DEM_EVENT_STATUS_PASSED)
                || (pTimer->CurStatus == DEM_EVENT_STATUS_PREPASSED)
                || (pTimer->Triggered == FALSE))/*SWS_Dem_00429*/
            {
                pTimer->CurStatus = DEM_EVENT_STATUS_PREFAILED;
                pTimer->Triggered = TRUE;
                pTimer->Timeout = 0;
            }
            break;
        default:
        	break;
    }
    Dem_MemCopy(&(pTimer->Dem_EventBuffer),pEventBuffer,sizeof(Dem_EventDataBufferType));
    Dem_UpdateFDC(pTimer->IntId);
    Dem_UpdateInternalData( pTimer->IntId );
    return Status;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * Brief               Dem_UpdateFDC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Dem_EventStatusType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_UpdateFDC(Dem_EventIdType IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
    P2VAR(Dem_DebounceCounterInfoType, AUTOMATIC, DEM_VAR) pCounter;
    P2CONST(Dem_DebounceCounterBasedClassType, AUTOMATIC, DEM_CONST) pCfgCounter;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
    P2VAR(Dem_DebounceTimeInfoType, AUTOMATIC, DEM_VAR) pTimer;
    P2CONST(Dem_DebounceTimeBaseClassType, AUTOMATIC, DEM_CONST) pTimerCfg;
#endif
    sint8 FDC = 0;

    switch (pEventCfg->AlgorithmType)
    {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
        case DEM_DEBOUNCE_COUNTER_BASED:

            if(  ((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM)
               &&( (pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM ) )
            {
                pCfgCounter = &DemPbCfgPtr->DemDebounceCounterBasedClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
                pCounter = &DemDebounceCounterInfo[pEventCfg->AlgorithmIndex];
                if (pCounter->InternalDebounceCounter == 0)
                {
                    FDC = 0;
                }
                else if (pCounter->InternalDebounceCounter > 0)
                {
                    FDC = (sint8)((pCounter->InternalDebounceCounter * 127)
                        / pCfgCounter->DemDebounceCounterFailedThreshold);
                }
                else
                {
                    FDC = (sint8)((pCounter->InternalDebounceCounter * ((sint8)(-128)))
                        / pCfgCounter->DemDebounceCounterPassedThreshold);
                }
            }
            break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
        case DEM_DEBOUNCE_TIME_BASE:
        if(  ((DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef) < DEM_DEBOUNCE_TIME_BASE_CLASS_NUM)
           &&( (pEventCfg->AlgorithmIndex) < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM ) )
        {
            pTimer = &DemDebounceTimerInfo[pEventCfg->AlgorithmIndex];
            pTimerCfg = &DemPbCfgPtr->DemDebounceTimeBaseClass[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmRef];
            switch (pTimer->CurStatus)
            {
                case DEM_EVENT_STATUS_PASSED:
                    FDC = -128;
                    break;
                case DEM_EVENT_STATUS_FAILED:
                    FDC = 127;
                    break;
                case DEM_EVENT_STATUS_PREFAILED:
                    FDC = (sint8)((pTimer->Timeout * 127UL)
                        / (uint64)pTimerCfg->DemDebounceTimeFailedThreshold);
                    break;
                default:
                    FDC = ((sint8)(-1))*((sint8)((pTimer->Timeout * 128UL)
                        / (uint64)pTimerCfg->DemDebounceTimePassedThreshold));
                    break;
				 }
            }
            break;
#endif
        default:
            break;
    }

	if (pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
	{
		if(FDC > 0)/* SWS_Dem_00795 */
		{
			if ((uint8)FDC > FDCInfo[pEventCfg->DemDTCRef].MaxFDCSinceLastClear)
			{
				FDCInfo[pEventCfg->DemDTCRef].MaxFDCSinceLastClear = (uint8)FDC;
			}
			if ((uint8)FDC > FDCInfo[pEventCfg->DemDTCRef].MaxFDCDuringCurrentCycle)
			{
				FDCInfo[pEventCfg->DemDTCRef].MaxFDCDuringCurrentCycle = (uint8)FDC;
			}
		}
		FDCInfo[pEventCfg->DemDTCRef].FDC = FDC;
	}
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
