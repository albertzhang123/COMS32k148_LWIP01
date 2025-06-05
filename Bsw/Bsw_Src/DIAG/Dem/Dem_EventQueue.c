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
**  FILENAME    : Dem_EventQueue.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for EventQueue                       **
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
#define DEM_EVENTQUEUE_C_AR_MAJOR_VERSION   4U
#define DEM_EVENTQUEUE_C_AR_MINOR_VERSION   2U
#define DEM_EVENTQUEUE_C_AR_PATCH_VERSION   2U
#define DEM_EVENTQUEUE_C_SW_MAJOR_VERSION   2U
#define DEM_EVENTQUEUE_C_SW_MINOR_VERSION   0U
#define DEM_EVENTQUEUE_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"
#if(STD_ON == DEM_TRIGGER_FIM_REPORTS)
#include "FiM.h"
#endif


/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_EVENTQUEUE_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EVENTQUEUE_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_EventQueue.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
typedef struct
{
    Dem_EventDataBufferType Queue[DEM_EVENT_QUEUE_SIZE];
    uint8 ReadIndex;
    uint8 WriteIndex;
} Dem_EventQueueType;

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_EventQueueType,AUTOMATIC) DemEventQueue;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Dem_EventQueueInit
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventQueueInit(void)
{
    P2VAR(Dem_EventQueueType, AUTOMATIC, DEM_VAR) pQueue = &DemEventQueue;
    P2VAR(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pBuffer = pQueue->Queue;
    uint16 iloop = 0;

    pQueue->ReadIndex = 0x00;
    pQueue->WriteIndex = 0x00;

    while(iloop < DEM_EVENT_QUEUE_SIZE)
    {
        pBuffer->ExtId = 0x00;
        pBuffer->Status = 0x00;
#if((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
        Dem_MemSet(pBuffer->FFData, 0xFFu,DEM_FREEZE_FRAME_MAX_LEN);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
        Dem_MemSet(pBuffer->ExtData, 0xFFu,DEM_EXTENDED_DATA_MAX_LEN);
#endif
        iloop++;
        pBuffer++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/*************************************************************************/
/*
 * Brief               Dem_EventQueueAdd
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId && Status
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_EventQueueAdd(Dem_EventIdType ExtId,
    Dem_EventStatusType Status)
{
    Std_ReturnType res = E_NOT_OK;
    P2VAR(Dem_EventQueueType, AUTOMATIC, DEM_VAR) pQueue = &DemEventQueue;
    P2VAR(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pBuffer = &(pQueue->Queue[pQueue->WriteIndex]);

    if (pBuffer->ExtId == 0x00u)
    {
        pBuffer->ExtId = ExtId;
        pBuffer->Status = Status;
#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_SYNCHRONOUS_TO_REPORTING)
#if((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))/*SWS_Dem_00805*/
        Dem_GetFreezeFrame(Dem_GetEventInternalId(ExtId), pBuffer->FFData);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)/*SWS_Dem_00812*/
        Dem_GetExtendedData(Dem_GetEventInternalId(ExtId), pBuffer->ExtData);
#endif
#endif
        pQueue->WriteIndex++;
        if (pQueue->WriteIndex >= DEM_EVENT_QUEUE_SIZE)
        {
            pQueue->WriteIndex = 0x00;
        }
        res = E_OK;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_EventQueueProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventQueueProcess(void)
{
    P2VAR(Dem_EventQueueType, AUTOMATIC, DEM_VAR) pQueue = &DemEventQueue;
    P2VAR(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pBuffer = &(pQueue->Queue[pQueue->ReadIndex]);
    Dem_EventIdType IntId;
    uint8 OperationCycleId;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
#if(DEM_DEAL_MAINFUNCTION_ENABLE == STD_ON)
    uint8 Deal_Cnt = 0;
#endif
#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
    uint8 OldStatus;
#endif
    /* Proccess Event */
    while((pBuffer->ExtId != 0x00u)
#if(DEM_DEAL_MAINFUNCTION_ENABLE == STD_ON)
    && (Deal_Cnt < DEM_DEAL_MAINFUNCTION_COUNTER)
#endif
    )
    {
#if(DEM_DEAL_MAINFUNCTION_ENABLE == STD_ON)
        Deal_Cnt++;
#endif
        IntId = Dem_GetEventInternalId(pBuffer->ExtId);
        pEvent = Dem_GetEventInfo(IntId);
        OperationCycleId = DemPbCfgPtr->DemEventParameter[IntId].DemOperationCycleRef;
#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
        OldStatus = pEvent->UdsStatus;
#endif
        if((0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
                && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE))
#if(DEM_ENABLE_CONDITION_NUM > 0)
                && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION))
#endif
            )
        {
            if(((0x00u != DEM_BITS_ISSET(DemOperationCycleStatus,OperationCycleId)) ||
               (DemOperationCycle[OperationCycleId].DemOperationCycleType == DEM_OPCYC_OBD_DCY))/*SWS_Dem_00700]*/)
            {/*SWS_Dem_00854] */
#if(DEM_ENVIRONMENT_DATA_CAPTURE == DEM_CAPTURE_ASYNCHRONOUS_TO_REPORTING)
#if((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))/*SWS_Dem_00461]*/
                Dem_GetFreezeFrame(IntId, pBuffer->FFData);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)/*SWS_Dem_00812]*/
                Dem_GetExtendedData(IntId, pBuffer->ExtData);
#endif
#endif
                if ((pBuffer->Status == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED)
                    && (pEvent->FDC_Trigger == FALSE))
                {
                    pEvent->FDC_Trigger = TRUE;	/*SWS_Dem_01068,SWS_Dem_01069*/
                    Dem_EventDataStorageTrigger(IntId,pBuffer,DEM_TRIGGER_ON_FDC_THRESHOLD);
                }
                else
                {
                    /* Debounce */
                    switch(Dem_DebounceProcess(pBuffer))
                    {
                        case DEM_EVENT_STATUS_PASSED:
                        Dem_EventTestPassed(IntId, pBuffer);
                        break;
                        case DEM_EVENT_STATUS_FAILED:
                        Dem_EventQueueAddDebounceFailed(pEvent);
                        Dem_EventTestFailed(IntId, pBuffer);
                        break;
                        default:
                        break;
                    }
                }
                    /*SWS_Dem_00786][SWS_Dem_01068][SWS_Dem_00811] [SWS_Dem_01069]*/
                    if((DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef != DEM_DTC_REF_INVALID)
                        && (DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef
                            ].DemDTCAttributesRef].DemEventMemoryEntryFdcThresholdStorageValue
                                <= (uint8)FDCInfo[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].FDC)
                                && (DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED) == 0x00u))
                    {/*SWS_Dem_00799]*/
                        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_FDC_TRIGGERED);
#if(DEM_TRIGGER_ON_FDC_THRESHOLD == DEM_EVENT_MEMORY_ENTRY_STORAGE_TRIGGER)
                        Dem_EventRetention(IntId,pBuffer);
#else
                        Dem_EventDataStorageTrigger(IntId,pBuffer,DEM_TRIGGER_ON_FDC_THRESHOLD);
#endif
                    }
            }
            else
            {/*debounce counter shall be reset to zero*/
                switch (DemPbCfgPtr->DemEventParameter[IntId].AlgorithmType)
                {
#if(DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM > 0)
                    case DEM_DEBOUNCE_COUNTER_BASED:
                        if(DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex < DEM_DEBOUNCE_COUNTER_BASED_EVENT_NUM)
                        {
                            DemDebounceCounterInfo[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex].InternalDebounceCounter = 0;
                            DemDebounceCounterInfo[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex].CurStatus = DEM_EVENT_STATUS_PREPASSED;
                        }
                    break;
#endif
#if(DEM_DEBOUNCE_TIME_BASE_CLASS_NUM > 0)
                    case DEM_DEBOUNCE_TIME_BASE:
                        if( DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex < DEM_DEBOUNCE_TIME_BASED_EVENT_NUM)
                        {
                            DemDebounceTimerInfo[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex].Timeout = 0u;
                            DemDebounceTimerInfo[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex].Triggered = FALSE;
                            DemDebounceTimerInfo[DemPbCfgPtr->DemEventParameter[IntId].AlgorithmIndex].CurStatus = DEM_EVENT_STATUS_PREPASSED;
                        }
                    break;
#endif
                    default:
                    break;
                }
            }
        }
        Dem_UpdateFDC(IntId);
#if(DEM_TRIGGER_FIM_REPORTS == STD_ON)
        if (OldStatus != pEvent->UdsStatus)
        {
        	(void)FiM_DemTriggerOnEventStatus(pEvent->IntId + 1u,OldStatus,pEvent->UdsStatus);
        }
#endif
        /* Clear Buffer */
        pBuffer->ExtId = 0x00;
        pBuffer->Status = 0x00;

        /* Move ReadIndex */
        pQueue->ReadIndex++;
        if(pQueue->ReadIndex >= DEM_EVENT_QUEUE_SIZE)
        {
            pQueue->ReadIndex = 0x00;
        }
        pBuffer = &(pQueue->Queue[pQueue->ReadIndex]);
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
