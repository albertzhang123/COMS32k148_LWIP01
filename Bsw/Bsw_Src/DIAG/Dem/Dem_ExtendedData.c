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
**  FILENAME    : Dem_ExtendedData.c                                          **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for ExtendedData                     **
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
#define DEM_EXTENDEDDATA_C_AR_MAJOR_VERSION   4U
#define DEM_EXTENDEDDATA_C_AR_MINOR_VERSION   2U
#define DEM_EXTENDEDDATA_C_AR_PATCH_VERSION   2U
#define DEM_EXTENDEDDATA_C_SW_MAJOR_VERSION   2U
#define DEM_EXTENDEDDATA_C_SW_MINOR_VERSION   0U
#define DEM_EXTENDEDDATA_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_EXTENDEDDATA_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif
#if (DEM_EXTENDEDDATA_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_ExtendedData.c : Mismatch in Specification Major Version"
#endif

#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Dem_GetExtendedData>
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
FUNC(void, DEM_CODE) Dem_GetExtendedData(Dem_EventIdType IntId,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer)
{
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    uint16 pExtRecord;
    uint16 index;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop = 0;
    uint8 cnt;

    Dem_UpdateInternalData(IntId);

    if ((pDTCAttrCfg == NULL_PTR ) || (pDTCAttrCfg->DemExtendedDataClassRef == DEM_EXTENDED_DATA_INVALID))
    {
        return;
    }
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while (iloop < DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum)
    {
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0)
            pExtRecord = DemExtendedDataRecordClass[*pExtIndex].DemDataElementClassIndex;
            index = 0;
            while (index < DemExtendedDataRecordClass[*pExtIndex].DemDataElementClassNum)
            {
                if (DemDataElementClass[pExtRecord].DemDataElementReadFnc != NULL_PTR)
                {
                    if (DemDataElementClass[pExtRecord].DemDataElementReadFnc(pBuffer) != E_OK)
                    {
                        Dem_MemSet(pBuffer, 0xFFu, DemDataElementClass[pExtRecord].DemDataElementDataSize);
                        DEM_DET_REPORT(DEM_SID_MAINFUNCTION,  DEM_E_NODATAAVAILABLE );
                    }
                }

                for (cnt = 0;cnt < DemDataElementClass[pExtRecord].DemDataElementDataSize;cnt++)
                {
                	pBuffer++;
                }
                pExtRecord++;
                index++;
            }
#endif
        }
        pExtIndex++;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_ExtendedDataGetFromEntry
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  BufSize
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_ExtendedDataGetFromEntry(
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    uint8 RecordNum,
    P2VAR(uint8, AUTOMATIC, DEM_VAR) pBuffer,
    uint16* BufSize)
{
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(
        Dem_GetEventInternalId(pEntry->EventId));
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop = 0;
    uint8 cnt;
   P2VAR(uint8, AUTOMATIC, DEM_VAR) pExt = pEntry->ExtData;

    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while (iloop < DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum)
    {
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if (pExtRecord->DemExtendedDataRecordNumber == RecordNum)
            {
                if (0x00u == DEM_BITS_ISSET(pEntry->ExtStatus, iloop))
                {
                    return DEM_E_NODATAAVAILABLE;
                }
                else
                {
                    Dem_MemCopy(pBuffer, pExt, pExtRecord->DataSize);
                    *BufSize = pExtRecord->DataSize;
                    return E_OK;
                }
            }
            else
            {
            	for (cnt = 0; cnt < pExtRecord->DataSize; cnt++)
            	{
            		pExt++;
            	}
            }
        }
        pExtIndex++;
        iloop++;
    }
    return DEM_E_WRONG_RECORDNUMBER;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_CheckExtendedDataIsStorage
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
FUNC(boolean, DEM_CODE) Dem_CheckExtendedDataIsStorage(Dem_EventIdType IntId,uint8 Trigger)
{
    boolean res = FALSE;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop = 0;

    if ((pDTCAttrCfg == NULL_PTR )
        || (pDTCAttrCfg->DemExtendedDataClassRef == DEM_EXTENDED_DATA_INVALID))
    {
        return res;
    }
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while ((iloop < DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum)&&(res == FALSE))
    {
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if (pExtRecord->DemExtendedDataRecordTrigger == Trigger)
            {
                res = TRUE;
            }
        }
        pExtIndex++;
        iloop++;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_ExtendedDataStorage
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
FUNC(void, DEM_CODE) Dem_ExtendedDataStorage(Dem_EventIdType IntId,
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry,
    P2VAR(Dem_EventDataBufferType, AUTOMATIC, DEM_VAR) pEventBuffer,
    uint8 Trigger)
{
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = Dem_EventDTCAttributesCfg(IntId);
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    uint8 iloop = 0;
    Dem_DTCOriginType pMemDest;
    uint8 Offset = 0;

    if ((pDTCAttrCfg == NULL_PTR )
        || (pDTCAttrCfg->DemExtendedDataClassRef == DEM_EXTENDED_DATA_INVALID))
    {
        return;
    }
    pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
        ->DemExtendedDataClassRef].StartIndex];
    while (iloop < DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum)
    {
        if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
        {
            pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
            if ((pExtRecord->DemExtendedDataRecordTrigger == Trigger)
                && ((pExtRecord->DemExtendedDataRecordUpdate == DEM_UPDATE_RECORD_YES)
                    || ((0x00u == DEM_BITS_ISSET(pEntry->ExtStatus, iloop))
                            && (pExtRecord->DemExtendedDataRecordUpdate == DEM_UPDATE_RECORD_NO))))
            {/*SWS_Dem_00815] [SWS_Dem_00816] */
                DEM_BITS_SET(pEntry->ExtStatus, iloop); /*set the corresponding bit*/
#if(DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
                if(DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef != DEM_DTC_REF_INVALID)
                {
                	Dem_MemCopy(&(pEntry->ExtData[Offset]),
                		&(DemDTCDataBuffer[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].ExtData[Offset]),
						pExtRecord->DataSize);
                	DEM_UNUSED(pEventBuffer);
                }
#else
                    Dem_MemCopy(&(pEntry->ExtData[Offset]), &(pEventBuffer->ExtData[Offset]), pExtRecord->DataSize);
#endif
            }
            if((pExtRecord->DemExtendedDataRecordTrigger == DEM_TRIGGER_ON_MIRROR)
                    && ((pExtRecord->DemExtendedDataRecordUpdate == DEM_UPDATE_RECORD_YES)
                        || ((0x00u == DEM_BITS_ISSET(pEntry->ExtStatus, iloop))
                                && (pExtRecord->DemExtendedDataRecordUpdate == DEM_UPDATE_RECORD_NO))))
            {/*SWS_Dem_01071*/
                pMemDest = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_MIRROR_MEMORY);
                if (pMemDest != DEM_MEM_DEST_INVALID)
                {
                    pEntry = Dem_MirrorEntryAllocate(IntId, pMemDest);

                    if ((pEntry != NULL_PTR)

                    && (0x00u == DEM_BITS_ISSET(DemMemDestInfo[pMemDest].DisableDTCRecordUpdate, DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef)))
                    {
                        DEM_BITS_SET(pEntry->ExtStatus, iloop); /*set the corresponding bit*/
#if(DEM_EVENT_COMBINATION_SUPPORT == DEM_EVCOMB_ONSTORAGE)
						if(DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef != DEM_DTC_REF_INVALID)
						{
                        	Dem_MemCopy(&(pEntry->ExtData[Offset]),
                        		&(DemDTCDataBuffer[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].ExtData[Offset]),
								pExtRecord->DataSize);
						}
#else
                        Dem_MemCopy(&(pEntry->ExtData[Offset]), &(pEventBuffer->ExtData[Offset]), pExtRecord->DataSize);
#endif
                    }
                }
            }
            Offset += pExtRecord->DataSize;
        }
        pExtIndex++;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
#endif
