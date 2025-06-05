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
**  FILENAME    : Dem_FreezeFrame.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for FreezeFrame                      **
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
#define DEM_FREEZEFRAME_C_AR_MAJOR_VERSION   4U
#define DEM_FREEZEFRAME_C_AR_MINOR_VERSION   2U
#define DEM_FREEZEFRAME_C_AR_PATCH_VERSION   2U
#define DEM_FREEZEFRAME_C_SW_MAJOR_VERSION   2U
#define DEM_FREEZEFRAME_C_SW_MINOR_VERSION   0U
#define DEM_FREEZEFRAME_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"

#if((DEM_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0) || (DEM_PID_CLASS_NUM > 0))
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_FREEZEFRAME_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
#if (DEM_FREEZEFRAME_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_FreezeFrame.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_PreStoreFFInfoType,AUTOMATIC) DemPreStoreFFInfo[DEM_MAX_NUMBER_PRESTORED_FF];
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
#if(DEM_DID_CLASS_NUM > 0)
static FUNC(void,DEM_CODE)
Dem_GetGeneralFreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
);
static FUNC(void,DEM_CODE)
Dem_GetWWHOBDFreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
);
#endif

#if(DEM_SPN_CLASS_NUM > 0u)
static FUNC(void,DEM_CODE)
Dem_GetJ1939FreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
);
static FUNC(void,DEM_CODE)
Dem_GetJ1939ExtendedFreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
);
#endif

#if(DEM_PID_CLASS_NUM > 0u)
static FUNC(void,DEM_CODE)
Dem_GetObdFreezeFrame(
	uint8* pBuffer
);
#endif
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameInit
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
FUNC(void, DEM_CODE) Dem_FreezeFrameInit(void)
{
    uint8 iloop = 0u;
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = DemPreStoreFFInfo;

    while (iloop < DEM_MAX_NUMBER_PRESTORED_FF)
    {
        pPreFF->ExtId = 0x00;
        Dem_MemSet(pPreFF->FFData, 0xFFu, DEM_FREEZE_FRAME_MAX_LEN);
        pPreFF++;
        iloop++;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * Brief               Dem_GetFreezeFrame
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     pBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_GetFreezeFrame(
	Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer
)
{
    uint16 tempRef;
    uint16 DTCAttributesRef;
#if((DEM_PID_CLASS_NUM > 0) || (DEM_SPN_CLASS_NUM > 0))
    uint16 obdDtcRef;
#endif

    Dem_UpdateInternalData(IntId);

    /* DTCAttributesRef */
    DTCAttributesRef = DemPbCfg.DemDTC[DemPbCfg.DemEventParameter[IntId].DemDTCRef].DemDTCAttributesRef;
#if((DEM_PID_CLASS_NUM > 0) || (DEM_SPN_CLASS_NUM > 0))
    obdDtcRef = DemPbCfg.DemDTC[DemPbCfg.DemEventParameter[IntId].DemDTCRef].DemObdDTCRef;
#endif
#if(DEM_DID_CLASS_NUM > 0)
    /* FreezeFrameRef */
    tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemFreezeFrameClassRef;
    if (tempRef != DEM_FREEZE_FRAME_INVALID)
    {
    	Dem_GetGeneralFreezeFrame(tempRef,pBuffer);
    }
    else
    {
        /* WWHOBDFreezeFrameRef */
        tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemWWHOBDFreezeFrameClassRef;
        if (tempRef != DEM_WWHOBD_FREEZE_FRAME_INVALID)
        {
        	Dem_GetWWHOBDFreezeFrame(tempRef,pBuffer);
        }
        else
        {
#endif
#if(DEM_SPN_CLASS_NUM > 0u)
            /* J1939FreezeFrameRef */
            tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemJ1939FreezeFrameClassRef;
            if ((tempRef != DEM_J1939_FREEZE_FRAME_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                    && (DemPbCfg.DemObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
            {
            	Dem_GetJ1939FreezeFrame(tempRef,pBuffer);
            }
            else
            {
                /* J1939EXFreezeFrameRef */
                tempRef = DemPbCfg.DemDTCAttributes[DTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef;
                if ((tempRef != DEM_J1939_FREEZE_FRAME_INVALID) && (obdDtcRef != DEM_OBD_DTC_INVALID)
                        && (DemPbCfg.DemObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
                {
                	Dem_GetJ1939ExtendedFreezeFrame(tempRef,pBuffer);
                }
                else
                {
#endif
#if(DEM_PID_CLASS_NUM > 0u)
                    if((obdDtcRef != DEM_OBD_DTC_INVALID) && (DemPbCfg.DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
                    {
                        /*no other FreezeFrameRef ,take the obd FreezeFrame*/
                    	Dem_GetObdFreezeFrame(pBuffer);
                    }
#endif
#if(DEM_SPN_CLASS_NUM > 0u)
                }
            }
#endif
#if(DEM_DID_CLASS_NUM > 0u)
        }
    }
#endif
    return; 
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_CheckFreezeFrameIsStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              boolean
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(boolean, DEM_CODE) Dem_CheckFreezeFrameIsStorage(
#if ((DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)\
		&& (DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0))
		Dem_EventIdType IntId,
    uint8 Trigger
#else
	void
#endif
)
{
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    boolean res = FALSE;
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0)
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC, DEM_CONST) pFFRecordCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFFRecordIndex;
    uint8 iloop = 0;
    pFFRecordIndex =
        &DemFreezeFrameRecordClassRef[DemFreezeFrameRecNumClass[pDTCAttrCfg
            ->DemFreezeFrameRecNumClassRef].StartIndex];
    while ((iloop < DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].RefNum) && (res == FALSE))
    {
        if (*pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
        {
            pFFRecordCfg = &DemFreezeFrameRecordClass[*pFFRecordIndex];
            if (pFFRecordCfg->DemFreezeFrameRecordTrigger == Trigger)
            {
                res = TRUE;
            }
        }
        pFFRecordIndex++;
        iloop++;
    }
#endif
    return res;
#else
    return FALSE;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameStorage
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer && Trigger
 * Param-Name[out]     None
 * Param-Name[in/out]  pEntry
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_FreezeFrameStorage(
	VAR(Dem_EventIdType,DEM_CODE) IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
	P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
	VAR(uint8,DEM_CODE) Trigger)
{
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
    P2CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC, DEM_CONST) pFFRecordCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFFRecordIndex;
#endif
    uint8 iloop = 0u;
#endif
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
    uint16 index = 0u;
#endif
#if ((DEM_PID_CLASS_NUM > 0) && (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT) && (DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED))
    uint16 obdDtcRef;
    uint16 tempRef;
#endif
    if (pDTCAttrCfg == NULL_PTR)
    {
        return;
    }
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    if (pDTCAttrCfg->DemFreezeFrameRecNumClassRef
    								== DEM_FREEZE_FRAME_REC_NUM_INVALID)
    {
        return;
    }
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
    pFFRecordIndex = &DemFreezeFrameRecordClassRef[DemFreezeFrameRecNumClass[pDTCAttrCfg
																			 	->DemFreezeFrameRecNumClassRef].StartIndex];
    while (iloop < DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].RefNum)
    {
        if (*pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
        {
            pFFRecordCfg = &DemFreezeFrameRecordClass[*pFFRecordIndex];
            if (pFFRecordCfg->DemFreezeFrameRecordTrigger == Trigger)
            {/*SWS_Dem_00797]*/
#endif
                pFF = &(pEntry->FFList[iloop]);
                if (pFF->RecordNum == 0xFFu)
                {
                    pEntry->FFNum++;
                }
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
                if (((pFF->RecordNum == 0xFFu) && (pFFRecordCfg->DemFreezeFrameRecordUpdate == DEM_UPDATE_RECORD_NO))
                        || (pFFRecordCfg->DemFreezeFrameRecordUpdate == DEM_UPDATE_RECORD_YES))/*SWS_Dem_00804] [SWS_Dem_00803]*/
                {
#endif
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
#if(DEM_PID_CLASS_NUM > 0)
                    obdDtcRef = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef;
                    tempRef = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemDTCAttributesRef;
                    if(((obdDtcRef != DEM_OBD_DTC_INVALID) && ((DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID)))
                        && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemFreezeFrameClassRef == DEM_FREEZE_FRAME_INVALID)
                        && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemWWHOBDFreezeFrameClassRef == DEM_WWHOBD_FREEZE_FRAME_INVALID)
                        && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemJ1939FreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID)
                        && (DemPbCfgPtr->DemDTCAttributes[tempRef].DemJ1939ExpandedFreezeFrameClassRef == DEM_J1939_FREEZE_FRAME_INVALID))
                    {
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
                        Dem_MemCopy(pFF->Data, DemDTCDataBuffer[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].FFData, DEM_PID_TOTAL_SIZE);
                        DEM_UNUSED(pEventBuffer);
#else
                        Dem_FreezeFrameCopy(IntId, pFF->Data, pEventBuffer
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                        		,Trigger
#endif
								);
#endif
                        pFF->RecordNum = 0;
                    }
                    else
#endif
#endif
                    {
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
                        /*RecordNum the initialized value should be 0xff*/
                        pFF->RecordNum = pFFRecordCfg->DemFreezeFrameRecordNumber;/*SWS_Dem_00581]*/
#endif/*#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0)*/

#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
                        while (index < DEM_EVENT_PARAMETER_NUM)
                        {
                            if(DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef == DemPbCfgPtr->DemEventParameter[index].DemDTCRef)
                            {
                                Dem_MemCopy(pFF->Data, DemDTCDataBuffer[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].FFData,
                                        DEM_FREEZE_FRAME_MAX_LEN);
                            }
                            index++;
                        }
#else/*#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)*/
                        Dem_FreezeFrameCopy(IntId, pFF->Data, pEventBuffer
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                        		,Trigger
#endif/*#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)*/
								);
#endif/*#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)*/
                        pEntry->LatestFF = iloop;
                    }
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
                }
            }
        }
        pFFRecordIndex++;
        iloop++;
    }
#endif/*#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0)*/

#else/*#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)*/
    if (pDTCAttrCfg->DemMaxNumberFreezeFrameRecords != 0u)
    {
		if (pDTCAttrCfg->DemMaxNumberFreezeFrameRecords == 1u)
		{
			pEntry->FFNum = 1u;
			pFF = &pEntry->FFList[0u];
			pEntry->LatestFF = pEntry->FFNum;
			pFF->RecordNum = pEntry->LatestFF;
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
			Dem_MemCopy(pFF->Data, DemDTCDataBuffer[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].FFData, DEM_FREEZE_FRAME_MAX_LEN);
#else/*#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)*/
			Dem_FreezeFrameCopy(IntId, pFF->Data, pEventBuffer
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
					,Trigger
#endif/*#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)*/
					);
#endif/*#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)*/
		}
		else
		{
			if(pEntry->FFNum < pDTCAttrCfg->DemMaxNumberFreezeFrameRecords)
			{/*SWS_Dem_00585] */
				pFF = &pEntry->FFList[pEntry->FFNum];
				pEntry->LatestFF = pEntry->FFNum;
				pEntry->FFNum++;
				pFF->RecordNum = pEntry->FFNum;
#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
				Dem_MemCopy(pFF->Data, DemDTCDataBuffer[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].FFData, DEM_FREEZE_FRAME_MAX_LEN);
#else/*#if(DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)*/
				Dem_FreezeFrameCopy(IntId, pFF->Data, pEventBuffer
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
						,
						Trigger
#endif/*#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)*/
						);
#endif/*#if (DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)*/
			}
		}
    }
#endif/*#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)*/
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameGetByRecordNum
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_FreezeFrameInfoType*
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_FreezeFrameGetByRecordNum(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    uint8 RecordNum)
{
    uint8 iloop = 0;
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF = pEntry->FFList;
    if (pEntry->FFNum == 0u)
    {
        return NULL_PTR;
    }
    if (RecordNum == 0xFFu)
    {
        return &(pEntry->FFList[pEntry->LatestFF]);
    }
    while (iloop < DEM_MAX_NUMBER_FF_RECORDS)
    {
        if (pFF->RecordNum == RecordNum)
        {
            return pFF;
        }
        iloop++;
        pFF++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameGetFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEntry && RecordNum && Did
 * Param-Name[out]     None
 * Param-Name[in/out]  pBuffer && BufSize
 * Return              Std_ReturnType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_FreezeFrameGetFromEntry(
P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
uint8 RecordNum,
uint16 Did,
P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer,
uint16* BufSize)
{

    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF;
    uint16 Ref;
#if(DEM_DID_CLASS_NUM > 0)
    uint8 cnt;
    uint16 didRefNum;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) pDidIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pData;
#endif

    if (pEntry->FFNum == 0u)
    {
        return DEM_E_NODATAAVAILABLE;
    }
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CALCULATED)
    if (RecordNum == 0xFFu)/*SWS_Dem_00479]*/
    {
        Dem_MemCopy(pBuffer, pEntry->FFList[pEntry->LatestFF].Data,
        DEM_FREEZE_FRAME_MAX_LEN);
        *BufSize = DEM_FREEZE_FRAME_MAX_LEN;
        return E_OK;
    }
    else
    {
        return E_NOT_OK;
    }
#endif
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    if ((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef].DemWWHOBDDTCClass
            == DEM_DTC_WWHOBD_CLASS_NOCLASS) && (RecordNum == 0u))
    {/*SWS_Dem_01194] */
        return DEM_E_WRONG_RECORDNUMBER;
    }
#endif
    pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNum);
    if (pFF == NULL_PTR)
    {
        return DEM_E_WRONG_RECORDNUMBER;
    }
#if(DEM_DID_CLASS_NUM > 0)
    pData = pFF->Data;
    /* DtcRef */
    Ref = DemPbCfgPtr->DemEventParameter[Dem_GetEventInternalId(pEntry->EventId)].DemDTCRef;
    /* DTCAttributesRef */
    Ref = DemPbCfgPtr->DemDTC[Ref].DemDTCAttributesRef;
    /* FreezeFrameRef */
    Ref = DemPbCfgPtr->DemDTCAttributes[Ref].DemFreezeFrameClassRef;
    pDidIndex = &DemDidClassRef[DemFreezeFrameClass[Ref].StartIndex];

    for (didRefNum = 0; didRefNum < DemFreezeFrameClass[Ref].RefNum; didRefNum++)
    {
        pDid = &DemDidClass[*pDidIndex];
        if (pDid->DemDidIdentifier == Did)
        {
            Dem_MemCopy(pBuffer, pData, pDid->DataSize);
            *BufSize = pDid->DataSize;
            return E_OK;
        }
        else
        {
        	for (cnt = 0; cnt < pDid->DataSize;cnt++)
        	{
        		pData++;
        	}
        }
        pDidIndex++;
    }
#endif
    return DEM_E_WRONG_DIDNUMBER;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * Brief               Dem_PreStoreFF
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_PreStoreFF(Dem_EventIdType ExtId)
{
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = Dem_PreStoreFFGet(ExtId);

    if (pPreFF == NULL_PTR)
    {
        pPreFF = Dem_PreStoreFFGet(0x00);
        if (pPreFF == NULL_PTR)
        {
            return E_NOT_OK;/*SWS_Dem_00808] */
        }
        pPreFF->ExtId = ExtId;
    }
    /*SWS_Dem_00807]*/
    Dem_GetFreezeFrame(Dem_GetEventInternalId(ExtId), pPreFF->FFData);
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_ClearPreStoreFF
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ClearPreStoreFF(Dem_EventIdType ExtId)
{
    Std_ReturnType res = E_NOT_OK;
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = Dem_PreStoreFFGet(ExtId);

    if (pPreFF != NULL_PTR)
    {
        pPreFF->ExtId = 0x00;
        Dem_MemSet(pPreFF->FFData, 0xFFu, DEM_FREEZE_FRAME_MAX_LEN);
        res = E_OK;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
/*************************************************************************/
/*
 * Brief               Dem_PreStoreFFGet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Dem_PreStoreFFInfoType*
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR), DEM_CODE)
Dem_PreStoreFFGet(Dem_EventIdType ExtId)
{
    uint8 iloop = 0;
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = DemPreStoreFFInfo;
    while (iloop < DEM_MAX_NUMBER_PRESTORED_FF)
    {
        if (pPreFF->ExtId == ExtId)
        {
            return pPreFF;
        }
        pPreFF++;
        iloop++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

/*************************************************************************/
/*
 * Brief               Dem_FreezeFrameCopy
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId && pEventBuffer
 * Param-Name[out]     pFFBuffer
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_FreezeFrameCopy(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pFFBuffer,
    P2CONST(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
	,
	uint8 Trigger
#endif
	)
    {
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
    P2VAR(Dem_PreStoreFFInfoType, AUTOMATIC, DEM_VAR) pPreFF = NULL_PTR;
#endif
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0)
	boolean Find = FALSE;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC, DEM_CONST) pFFRecordCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFFRecordIndex;
    uint8 iloop = 0;

#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
    pFFRecordIndex = &DemFreezeFrameRecordClassRef[DemFreezeFrameRecNumClass[pDTCAttrCfg
    ->DemFreezeFrameRecNumClassRef].StartIndex];
    while (iloop < DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].RefNum)
    {
        if (*pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
        {
            pFFRecordCfg = &DemFreezeFrameRecordClass[*pFFRecordIndex];
            if (pFFRecordCfg->DemFreezeFrameRecordTrigger == Trigger)
            {
            	Find = TRUE;
            }
        }
        iloop++;
        pFFRecordIndex++;
    }
    if (Find == FALSE)
    {
    	return;
    }
#endif
#endif
#if(DEM_MAX_NUMBER_PRESTORED_FF > 0)
    if (DemPbCfgPtr->DemEventParameter[IntId].DemFFPrestorageSupported == TRUE)
    {
        pPreFF = Dem_PreStoreFFGet(Dem_GetEventExternalId(IntId));
    }
    if (pPreFF != NULL_PTR)
    {/*SWS_Dem_00464]*/
        Dem_MemCopy(pFFBuffer, pPreFF->FFData, DEM_FREEZE_FRAME_MAX_LEN);
        pPreFF->ExtId = 0x00;/*SWS_Dem_00969]*/
        Dem_MemSet(pPreFF->FFData, 0xFFu, DEM_FREEZE_FRAME_MAX_LEN);
        return;
    }
#endif
    if (pEventBuffer != NULL_PTR)
    {
        Dem_MemCopy(pFFBuffer, pEventBuffer->FFData, DEM_FREEZE_FRAME_MAX_LEN);
        return;
    }
    Dem_GetFreezeFrame(IntId, pFFBuffer);
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DID_CLASS_NUM > 0)
static FUNC(void,DEM_CODE)
Dem_GetGeneralFreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
)
{
	uint8 cnt = 0u;
    uint16 didRefNum = 0u;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) pIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;
    uint16 csStart;
    uint8 csIndex;

    pIndex = &DemDidClassRef[DemFreezeFrameClass[tempRef].StartIndex];
    while (didRefNum < DemFreezeFrameClass[tempRef].RefNum)
    {
        didRefNum++;
        pDid = &DemDidClass[*pIndex];
        csStart = pDid->StartIndex;
        csIndex = 0;
        while (csIndex < pDid->RefNum)
        {
            csIndex++;
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
            if (DemDataElementClass[csStart].DemDataElementReadFnc != NULL_PTR)
            {
                if (DemDataElementClass[csStart].DemDataElementReadFnc(pBuffer) != E_OK)
                {
                    Dem_MemSet(pBuffer, 0xFFu, DemDataElementClass[csStart].DemDataElementDataSize);
                    DEM_DET_REPORT(DEM_SID_MAINFUNCTION, DEM_E_NODATAAVAILABLE);
                }
            }
            for (cnt = 0; cnt < DemDataElementClass[csStart].DemDataElementDataSize;cnt++)
            {
                pBuffer++;
            }
#endif
            csStart++;
        }
        pIndex++;
    }
}

static FUNC(void,DEM_CODE)
Dem_GetWWHOBDFreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
)
{
    uint8 cnt;
    uint16 csStart;
    uint8 csIndex;
    uint16 didRefNum = 0;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) pIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;

    pIndex = &DemDidClassRef[DemFreezeFrameClass[tempRef].StartIndex];

    while (didRefNum < DemFreezeFrameClass[tempRef].RefNum)
    {
        didRefNum++;
        pDid = &DemDidClass[*pIndex];
        csStart = pDid->StartIndex;
        csIndex = 0;
        while (csIndex < pDid->RefNum)
        {
            csIndex++;
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
            if (DemDataElementClass[csStart].DemDataElementReadFnc != NULL_PTR)
            {
                if (DemDataElementClass[csStart].DemDataElementReadFnc(pBuffer) != E_OK)
                {
                    Dem_MemSet(pBuffer, 0xFFu, DemDataElementClass[csStart].DemDataElementDataSize);
                    DEM_DET_REPORT(DEM_SID_MAINFUNCTION, DEM_E_NODATAAVAILABLE);
                }
            }

            for (cnt = 0; cnt < DemDataElementClass[csStart].DemDataElementDataSize;cnt++)
            {
                pBuffer++;
            }
#endif
            csStart++;
        }
        pIndex++;
    }
}
#endif

#if(DEM_SPN_CLASS_NUM > 0u)
static FUNC(void,DEM_CODE)
Dem_GetJ1939FreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
)
{
	uint8 cnt;
    uint16 SpnRefNum = 0;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) SpnIndex;
    P2CONST(Dem_SPNClassType, AUTOMATIC, DEM_CONST) pSpn;

    SpnIndex = &DemSPNClassRef[DemJ1939FreezeFrameClass[tempRef].StartIndex];
    while (SpnRefNum < DemJ1939FreezeFrameClass[tempRef].RefNum)
    {
        SpnRefNum++;
        pSpn = &DemSPNClass[*SpnIndex];
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
        if (DemDataElementClass[pSpn->StartIndex].DemDataElementReadFnc != NULL_PTR)
        {
            if (DemDataElementClass[pSpn->StartIndex].DemDataElementReadFnc(pBuffer) != E_OK)
            {
                Dem_MemSet(pBuffer, 0xFFu, DemDataElementClass[pSpn->StartIndex].DemDataElementDataSize);
                DEM_DET_REPORT(DEM_SID_MAINFUNCTION, DEM_E_NODATAAVAILABLE);
            }
            for (cnt = 0; cnt < DemDataElementClass[pSpn->StartIndex].DemDataElementDataSize;cnt++)
            {
                pBuffer++;
            }
        }
#endif
        SpnIndex++;
    }
}


static FUNC(void,DEM_CODE)
Dem_GetJ1939ExtendedFreezeFrame(
	uint16 tempRef,
	uint8* pBuffer
)
{
	uint8 cnt = 0u;
    uint16 SpnRefNum = 0u;
    P2CONST(uint16, AUTOMATIC, DEM_CONST) SpnIndex;
    P2CONST(Dem_SPNClassType, AUTOMATIC, DEM_CONST) pSpn;

    SpnIndex = &DemSPNClassRef[DemJ1939FreezeFrameClass[tempRef].StartIndex];
    while (SpnRefNum < DemJ1939FreezeFrameClass[tempRef].RefNum)
    {
        SpnRefNum++;
        pSpn = &DemSPNClass[*SpnIndex];
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
        if (DemDataElementClass[pSpn->StartIndex].DemDataElementReadFnc != NULL_PTR)
        {
            if (DemDataElementClass[pSpn->StartIndex].DemDataElementReadFnc(pBuffer) != E_OK)
            {
                Dem_MemSet(pBuffer, 0xFFu, DemDataElementClass[pSpn->StartIndex].DemDataElementDataSize);
                DEM_DET_REPORT(DEM_SID_MAINFUNCTION, DEM_E_NODATAAVAILABLE);
            }
            for (cnt = 0; cnt < DemDataElementClass[pSpn->StartIndex].DemDataElementDataSize;cnt++)
            {
                pBuffer++;
            }
        }
#endif
        SpnIndex++;
    }
}
#endif

#if(DEM_PID_CLASS_NUM > 0u)
static FUNC(void,DEM_CODE)
Dem_GetObdFreezeFrame(
	uint8* pBuffer
)
{
	uint8 cnt = 0u;
    uint16 pidRefNum = 0u;
    uint16 pPidIndex = 0u;
    uint16 index;
    P2CONST(Dem_PidClassType, AUTOMATIC, DEM_CONST) pPid;

    while (pPidIndex < DEM_PID_CLASS_NUM)
    {
        pPid = &DemPbCfg.DemPidClass[pPidIndex];
        pPidIndex++;
        pidRefNum = pPid->StartIndex;
        index = 0u;
        while (index < pPid->RefNum)
        {
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
            if (DemDataElementClass[pidRefNum].DemDataElementReadFnc != NULL_PTR)
            {
                if (DemDataElementClass[pidRefNum].DemDataElementReadFnc(pBuffer) != E_OK)
                {
                    Dem_MemSet(pBuffer, 0xFFu, DemDataElementClass[pidRefNum].DemDataElementDataSize);
                    DEM_DET_REPORT(DEM_SID_MAINFUNCTION,  DEM_E_NODATAAVAILABLE );
                }
            }
            for (cnt = 0; cnt < DemDataElementClass[pidRefNum].DemDataElementDataSize;cnt++)
            {
                pBuffer++;
            }
#endif
            pidRefNum++;
            index++;
        }
    }
}
#endif


/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
#endif
