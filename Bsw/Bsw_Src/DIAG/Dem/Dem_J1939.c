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
**  FILENAME    : Dem_J1939.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for J1939                            **
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
#define DEM_J1939_C_AR_MAJOR_VERSION   4U
#define DEM_J1939_C_AR_MINOR_VERSION   2U
#define DEM_J1939_C_AR_PATCH_VERSION   2U
#define DEM_J1939_C_SW_MAJOR_VERSION   2U
#define DEM_J1939_C_SW_MINOR_VERSION   0U
#define DEM_J1939_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_J1939_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_J1939.c : Mismatch in Specification Major Version"
#endif
#if (DEM_J1939_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_J1939.c : Mismatch in Specification Major Version"
#endif
#if (DEM_J1939_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_J1939.c : Mismatch in Specification Major Version"
#endif
#if (DEM_J1939_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_J1939.c : Mismatch in Specification Major Version"
#endif
#if (DEM_J1939_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_J1939.c : Mismatch in Specification Major Version"
#endif

#if (DEM_J1939_SUPPORT == STD_ON)
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define DEM_NUMBER_OF_READINESS_GROUPS_FORJ1939 20u
/* FreezeFrameRecordFilter */
typedef struct
{
    /* Filter */
    Dem_DTCFormatType DTCFormat;

    /* Result */
    uint16 NumberOfFilteredRecords;
    uint16 GetNum;
    uint8 MemDestIndex;
    uint8 EntryIndex;
    uint8 FFIndex;
} Dem_J1939FreezeFrameRecordFilterInfoType;

/* DTC Filter */
typedef struct
{
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter;
    Dem_DTCKindType DemJ1939DTCFilterInfo;
    uint8 node;
    Dem_DTCOriginType DTCOrigin;
    /* Result */
    boolean IsSet;
    uint16 NumberOfFilteredDTC;
    uint8 GetNum;
    uint8 CurrentIndex;
} Dem_J1939DTCFilterInfoType;

/* Freeze Frame Filter */
typedef struct
{
    Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind;
    uint8 node;
    uint8 CurrentIndex;
    boolean IsSet;
    uint16 SPNIndex;
    uint8 SPNCounter;
    boolean IsFind;
    boolean IsOK;
} Dem_J1939DcmFreezeFrameFilterType;

typedef struct
{
    uint8 node;
    uint8 CurrentIndex;
    boolean IsSet;
} DemJ1939DTCGetNodeType;

typedef struct
{
    uint8 node;
    uint8 CurrentIndex;
    boolean IsSet;
} Dem_J1939DcmRatioFilterType;

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_J1939DcmFreezeFrameFilterType,AUTOMATIC) DemJ1939DcmFreezeFrameFilter;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_J1939DcmRatioFilterType,AUTOMATIC) DemJ1939DcmRatioFilter;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_J1939DTCFilterInfoType,AUTOMATIC) DemJ1939DTCFilter;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(DemJ1939DTCGetNodeType,AUTOMATIC) DemJ1939DTCGetNode;
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
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_J1939Init(void)
{
    Dem_MemSet((uint8*)&DemJ1939DTCFilter, 0x00u, sizeof(Dem_J1939DTCFilterInfoType));
    Dem_MemSet((uint8*)&DemJ1939DcmFreezeFrameFilter, 0x00u,
        sizeof(Dem_J1939DcmFreezeFrameFilterType));
    Dem_MemSet((uint8*)&DemJ1939DcmRatioFilter, 0x00u,
        sizeof(Dem_J1939DcmRatioFilterType));

}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the DTC filter for a specific node and returns the composite lamp status of the filtered DTCs
 * ServiceId           0x90
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCStatusFilter: The following types are available:
                        DEM_J1939DTC_ACTIVE
                        DEM_J1939DTC_PREVIOUSLY_ACTIVE
                        DEM_J1939DTC_PENDING
                        DEM_J1939DTC_PERMANENT
                        DEM_J1939DTC_CURRENTLY_ACTIVE
 *                      DTCKind: Defines the functional group of DTCs to be reported
 *                      node:Nm node Id
 * Param-Name[out]     LampStatus:Receives the commulated lamp status
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType , DEM_CODE)
Dem_J1939DcmSetDTCFilter(Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
        Dem_DTCKindType DemJ1939DTCFilterInfo,
        uint8 node,
        Dem_J1939DcmLampStatusType* LampStatus)
{

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETDTCFILTER, DEM_E_UNINIT);
        return DEM_WRONG_FILTER;
    }
    if (LampStatus == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETDTCFILTER, DEM_E_PARAM_POINTER);
        return DEM_WRONG_FILTER;
    }
    DemJ1939DTCFilter.DTCStatusFilter = DTCStatusFilter;
    DemJ1939DTCFilter.DemJ1939DTCFilterInfo = DemJ1939DTCFilterInfo;
    DemJ1939DTCFilter.node = node;
    if (DTCStatusFilter == DEM_J1939DTC_PERMANENT)/*find the DTCOrigin*/
    {
        DemJ1939DTCFilter.DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PERMANENT_MEMORY);
    }
    else
    {
        DemJ1939DTCFilter.DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    }
    DemJ1939DTCFilter.CurrentIndex = 0;
    DemJ1939DTCFilter.NumberOfFilteredDTC = 0;
    DemJ1939DTCFilter.GetNum = 0;
    DemJ1939DTCFilter.IsSet = TRUE;

    *LampStatus = 0x00u;
#if(DEM_INDICATOR_NUM > 0)
    /*Composite Malfunction Indicator Lamp Status*/
    if (DEM_MALFUNCTION_LAMP_INDICATOR != DemIndicator_INVALID)
    {

        if (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
        {
            *LampStatus |= 0x4000;/*SWS_Dem_00858]*/
        }
        if ((DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
                || (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x00C0;/*SWS_Dem_00859]*/
        }
        if (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
        {
            *LampStatus &= ~0x00C0;/*SWS_Dem_00860]*/
        }
        if (DemWIRStatus[DEM_MALFUNCTION_LAMP_INDICATOR] ==  DEM_INDICATOR_FAST_FLASH)
        {
            *LampStatus &= ~0x0080;/*SWS_Dem_00861]*/
        }
    }

    /*Composite Red Stop Lamp Status*/
    if (DEM_RED_STOP_LAMP_INDICATOR != DemIndicator_INVALID)
    {
        if (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
        {
            *LampStatus |= 0x1000;/*SWS_Dem_00862]*/
        }
        if ((DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
                || (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x0030;/*SWS_Dem_00863]*/
        }
        if (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
        {
            *LampStatus &= ~0x0030;/*SWS_Dem_00864]*/
        }
        if (DemWIRStatus[DEM_RED_STOP_LAMP_INDICATOR] ==  DEM_INDICATOR_FAST_FLASH)
        {
            *LampStatus &= ~0x0020;/*SWS_Dem_00865]*/
        }
    }

    /*Composite Amber Warning Lamp Status*/
    if (DEM_AMBER_WARING_LAMP_INDICATOR != DemIndicator_INVALID)
    {
        if (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
        {
            *LampStatus |= 0x0400;/*SWS_Dem_00866]*/
        }
        if ((DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
                || (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x000c;/*SWS_Dem_00867]*/
        }
        if (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
        {
            *LampStatus &= ~0x000c;/*SWS_Dem_00868]*/
        }
        if (DemWIRStatus[DEM_AMBER_WARING_LAMP_INDICATOR] ==  DEM_INDICATOR_FAST_FLASH)
        {
            *LampStatus &= ~0x0008;/*SWS_Dem_00869]*/
        }
    }

    /*Composite Protect Lamp Status*/
    if (DEM_PROTECT_LAMP_INDICATOR != DemIndicator_INVALID)
    {
        if (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] != DEM_INDICATOR_OFF)
        {
            *LampStatus |= 0x0100;/*SWS_Dem_00870]*/
        }
        if ((DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_OFF)
                || (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x0003;/*SWS_Dem_00871]*/
        }
        if (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] == DEM_INDICATOR_SLOW_FLASH)
        {
            *LampStatus &=~ 0x0003;/*SWS_Dem_00872]*/
        }
        if (DemWIRStatus[DEM_PROTECT_LAMP_INDICATOR] ==  DEM_INDICATOR_FAST_FLASH)
        {
            *LampStatus&=~0x0002;/*SWS_Dem_00873]*/
        }
    }
#endif
    return DEM_FILTER_ACCEPTED;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the number of currently filtered DTCs set by the function Dem_J1939DcmSetDTCFilter
 * ServiceId           0x91
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     NumberOfFilteredDTC: The number of DTCs matching the defined status mask.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNumberOfFilteredDTCType , DEM_CODE)
Dem_J1939DcmGetNumberOfFilteredDTC(uint16* NumberOfFilteredDTC)
{

    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    P2VAR(Dem_J1939DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemJ1939DTCFilter;
    uint8 iloop = 0;
    uint8 J1939Index = 0;
    uint8 entryNum = 0;
    uint16 dtcRef = 0;
    boolean findSameDTC = FALSE;
    boolean IsFind = FALSE;
    uint32 tempDtcValue[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC, DEM_E_UNINIT);
        return DEM_NUMBER_FAILED;
    }
    if (NumberOfFilteredDTC == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNUMBEROFFILTEREDDTC, DEM_E_PARAM_POINTER);
        return DEM_NUMBER_FAILED;
    }

    if(pFilter->IsSet == FALSE)
    {
        return DEM_NUMBER_FAILED;
    }
    entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while (iloop < entryNum)
    {
        if (pEntry->EventId != 0x00)
        {
            pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;

            if ((DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)
                 && (pFilter->DemJ1939DTCFilterInfo == DemPbCfgPtr->DemDTC[dtcRef].DTCKind))/*match kind and J1939DTC*/
            {
#if(DEM_J1939_NODE_NUM > 0)
                index = 0;
                nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                 DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                           DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                while (index < nodeNum)
                {
                    if (pFilter->node == *nodeIndex)
                    {
                        IsFind = TRUE;/*find the node*/
                    }
                    index++;
                    nodeIndex++;
                }
                if (IsFind == TRUE)
                {
#endif
                    IsFind = FALSE;
                    switch(pFilter->DTCStatusFilter)
                    {/*match the DTCStatusFilter*/
                    case DEM_J1939DTC_ACTIVE:/*(ConfirmedDTC == 1 AND TestFailed == 1) OR MIL_ON*/
#if(DEM_INDICATOR_NUM > 0)
                        if (((0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                                && (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
                                || (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].
                                              AttrStartIndex].DemIndicatorRef] != DEM_INDICATOR_OFF))
                        {
                            IsFind = TRUE;
                        }
#endif
                        break;
                    case DEM_J1939DTC_PREVIOUSLY_ACTIVE:/*ConfirmedDTC == 1 AND TestFailed == 0 AND MIL_OFF*/
#if(DEM_INDICATOR_NUM > 0)
                        if ((0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                                && (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                                && (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].
                                                 AttrStartIndex].DemIndicatorRef] == DEM_INDICATOR_OFF))
                        {
                            IsFind = TRUE;
                        }
#endif
                        break;
                    case DEM_J1939DTC_PENDING:/*PendingDTC == 1*/
                        if (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
                        {
                            IsFind = TRUE;
                        }
                        break;
                    case DEM_J1939DTC_PERMANENT:
                        IsFind = TRUE;
                        break;
                    case DEM_J1939DTC_CURRENTLY_ACTIVE:/*TestFailed == 1*/
                        if (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                        {
                            IsFind = TRUE;
                        }
                        break;
                    default:
                        break;
                    }
                    if (IsFind == TRUE)
                    {
                        IsFind = FALSE;
                        tempDtcValue[iloop] = DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue;
                        J1939Index = 0;
                        while (J1939Index < iloop)
                        {
                            if(tempDtcValue[J1939Index] == DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue)
                            {
                            	findSameDTC = TRUE;   /* find same dtc of diffrence event memory */
                            }
                            J1939Index++;
                        }
                        if(findSameDTC == FALSE)
                        {
                            pFilter->NumberOfFilteredDTC++;
                        }
                        else
                        {
                        	findSameDTC = FALSE;
                        }
                    }
#if(DEM_J1939_NODE_NUM > 0)
                }
#endif
            }
        }
        iloop++;
        pEntry++;
    }
    *NumberOfFilteredDTC = pFilter->NumberOfFilteredDTC;
    return DEM_NUMBER_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC.
 * ServiceId           0x92
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC: Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence counter. If the return value of the function call
                        is other than DEM_FILTERED_OK this parameter
                        does not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

FUNC(Dem_ReturnGetNextFilteredElementType , DEM_CODE)
Dem_J1939DcmGetNextFilteredDTC(uint32* J1939DTC,uint8* OccurenceCounter)
{

    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_J1939DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemJ1939DTCFilter;
    uint8 entryNum = 0;
    uint16 dtcRef = 0;
    Dem_EventIdType eventId = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    boolean IsFind;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDDTC, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((OccurenceCounter == NULL_PTR) || (J1939DTC == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDDTC, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if (Dem_Pending == TRUE)
    {/*SWS_Dem_00877]*/
        return DEM_FILTERED_PENDING;
    }
    entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while ((res != DEM_FILTERED_OK)
    && (pFilter->CurrentIndex < entryNum))
    {
        if (pEntry[pFilter->CurrentIndex].EventId != 0x00)
        {
            eventId = Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId);
            pEvent = Dem_GetEventInfo(eventId);
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            IsFind = FALSE;
            switch(pFilter->DTCStatusFilter)
            {/*match the DTCStatusFilter*/
              case DEM_J1939DTC_ACTIVE:/*(ConfirmedDTC == 1 AND TestFailed == 1) OR MIL_ON*/
                  if (((0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                          && (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
#if(DEM_INDICATOR_NUM > 0)
                          || (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].
                                        AttrStartIndex].DemIndicatorRef] != DEM_INDICATOR_OFF)
#endif
                  )
                  {
                      IsFind = TRUE;
                  }
                  break;
              case DEM_J1939DTC_PREVIOUSLY_ACTIVE:/*ConfirmedDTC == 1 AND TestFailed == 0 AND MIL_OFF*/
                  if ((0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                          && (0x00 == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
#if(DEM_INDICATOR_NUM > 0)
                          && (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].
                                           AttrStartIndex].DemIndicatorRef] == DEM_INDICATOR_OFF)
#endif
                  )
                  {
                      IsFind = TRUE;
                  }
                  break;
              case DEM_J1939DTC_PENDING:/*PendingDTC == 1*/
                  if (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_PDTC))
                  {
                      IsFind = TRUE;
                  }
                  break;
              case DEM_J1939DTC_PERMANENT:
                  IsFind = TRUE;
                  break;
              case DEM_J1939DTC_CURRENTLY_ACTIVE:/*TestFailed == 1*/
                  if (0x00 != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                  {
                      IsFind = TRUE;
                  }
                  break;
              default:
                  break;
            }
            /*SWS_Dem_01101]*/
            if ((DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE) || (IsFind == FALSE))
            {
                res = DEM_FILTERED_NO_MATCHING_ELEMENT;
            }
            else
            {
                /*guarantee the event index the new dtc */
                Dem_GetEventDTC(pEvent->IntId, DEM_DTC_FORMAT_J1939, J1939DTC);
                *OccurenceCounter = pEvent->OccurrenceCounter;
                if (*OccurenceCounter > 0x7F)
                {
                    *OccurenceCounter = 0x7F;/*SWS_Dem_00877]*/
                }
                res = DEM_FILTERED_OK;
            }
        }
        pFilter->CurrentIndex++;
    }
    return res;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_J1939_DM31_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the filter to the first applicable DTC for the DM31 response for a specific node
 * ServiceId           0x93
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Nm node Id of requesting client
 * Param-Name[out]     None
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void , DEM_CODE) Dem_J1939DcmFirstDTCwithLampStatus(uint8 node)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint16 dtcRef = 0;
    uint8 iloop = 0;
    boolean IsFind = FALSE;
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMFIRSTDTCWITHLAMPSTATUS, DEM_E_UNINIT);
    }
    else
    {
		DemJ1939DTCGetNode.node = node;
		DemJ1939DTCGetNode.IsSet = TRUE;
		while ((iloop < entryNum) && (IsFind == FALSE))
	   {
		   if (pEntry->EventId != 0x00u)
		   {
			   pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
			   dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
		        if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
		        		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match kind and J1939DTC*/
			   {
	#if(DEM_J1939_NODE_NUM > 0)
				   index = 0;
				   nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
											DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
				   nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
											   DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
				   while (index < nodeNum)
				   {
					   if (DemJ1939DTCGetNode.node == *nodeIndex)
					   {
	#endif
						   /*SWS_Dem_01101]*/
						   if (DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE)
						   {
							   DemJ1939DTCGetNode.CurrentIndex = iloop;
							   IsFind = TRUE;
							   break;
						   }
	#if(DEM_J1939_NODE_NUM > 0)
					   }
					   index++;
					   nodeIndex++;
				   }
	#endif
			   }
		   }
		   iloop++;
		   pEntry++;
	   }
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the next filtered J1939 DTC for DM31 including current LampStatus.
 * ServiceId           0x94
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     LampStatus:Receives the lamp status returned by this function.
 *                      If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
                        J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data.
                        OccurenceCounter:This parameter receives the corresponding occurrence counter.
                        If the return value of the function call
                        is other than DEM_FILTERED_OK this parameter
                        does not contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType , DEM_CODE)
Dem_J1939DcmGetNextDTCwithLampStatus(Dem_J1939DcmLampStatusType* LampStatus,
        uint32* J1939DTC,
        uint8* OccurenceCounter)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    uint16 dtcRef = 0;
    Dem_EventIdType eventId = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent = NULL_PTR;
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    P2CONST(Dem_IndicatorAttributeType, AUTOMATIC, DEM_CONST) pWIRAttr;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((OccurenceCounter == NULL_PTR) || (J1939DTC == NULL_PTR) || (LampStatus == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTDTCWITHLAMPSTATUS, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if (Dem_Pending == TRUE)
    {/*SWS_Dem_00882] */
        return DEM_FILTERED_PENDING;
    }
    while ((res != DEM_FILTERED_OK) && (DemJ1939DTCGetNode.IsSet == TRUE)
    && (DemJ1939DTCGetNode.CurrentIndex < entryNum))
    {
        if (pEntry[DemJ1939DTCGetNode.CurrentIndex].EventId != 0x00u)
        {
            eventId = Dem_GetEventInternalId(pEntry[DemJ1939DTCGetNode.CurrentIndex].EventId);
            pEvent = Dem_GetEventInfo(eventId);
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
            		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match kind and J1939DTC*/
            {
#if(DEM_J1939_NODE_NUM > 0)
                index = 0;
                nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                  DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                     DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                while (index < nodeNum)
                {
                    if (DemJ1939DTCGetNode.node == *nodeIndex)
                    {
#endif
                        /*SWS_Dem_01101]*/
                        if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
                        {
                            res = DEM_FILTERED_NO_MATCHING_ELEMENT;
                        }
                        else
                        {
                            /*guarantee the event index the new dtc */
                            Dem_GetEventDTC(pEvent->IntId, DEM_DTC_FORMAT_J1939, J1939DTC);
                            *OccurenceCounter = pEvent->OccurrenceCounter;
                            if (*OccurenceCounter > 0x7Fu)
                            {
                                *OccurenceCounter = 0x7Fu;/*SWS_Dem_00882]*/
                            }
                            res = DEM_FILTERED_OK;
                            break;
                        }
#if(DEM_J1939_NODE_NUM > 0)
                    }
                    index++;
                    nodeIndex++;
                }
#endif
            }
        }
        DemJ1939DTCGetNode.CurrentIndex++;
    }
    *LampStatus = 0u;
    if (pEvent != NULL_PTR)
    {
#if(DEM_INDICATOR_NUM > 0)
        pWIRAttr =&DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].AttrStartIndex];
        /*Malfunction Indicator Lamp*/
        if ((pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
           && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)))
        {
            *LampStatus |= 0x0040u;/*SWS_Dem_00883]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
             && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0xC000u;/*SWS_Dem_00884] */
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
            && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_SLOW_FLASH))
        {
            *LampStatus |= 0x0000u;/*SWS_Dem_00860]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_MALFUNCTION_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_FAST_FLASH))
        {
            *LampStatus |= 0x4000u;/*SWS_Dem_00861]*/
        }

        /*Red Stop Lamp*/
        if ((pWIRAttr->DemIndicatorRef == DEM_RED_STOP_LAMP_INDICATOR)
                && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)))
        {
            *LampStatus |= 0x0010u;/*SWS_Dem_00887]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_RED_STOP_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
             && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x3000u;/*SWS_Dem_00888]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_RED_STOP_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                    && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_SLOW_FLASH))
        {
            *LampStatus |= 0x0000u;/*SWS_Dem_00889]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_RED_STOP_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_FAST_FLASH))
        {
            *LampStatus |= 0x1000u;/*SWS_Dem_00890]*/
        }

        /*Amber Warning Lamp*/
        if ((pWIRAttr->DemIndicatorRef == DEM_AMBER_WARING_LAMP_INDICATOR)
                && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)))
        {
            *LampStatus |= 0x0004u;/*SWS_Dem_00891]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_AMBER_WARING_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
             && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x0C00u;/*SWS_Dem_00892]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_AMBER_WARING_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                    && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_SLOW_FLASH))
        {
            *LampStatus |= 0x0000u;/*SWS_Dem_00893]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_AMBER_WARING_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_FAST_FLASH))
        {
            *LampStatus |= 0x0400u;/*SWS_Dem_00894]*/
        }

        /*Protect Lamp*/
        if ((pWIRAttr->DemIndicatorRef == DEM_PROTECT_LAMP_INDICATOR)
                && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_WIR)))
        {
            *LampStatus |= 0x0001u;/*SWS_Dem_00895]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_PROTECT_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
             && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_CONTINUOUS))
        {
            *LampStatus |= 0x0300u;/*SWS_Dem_00896]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_PROTECT_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                    && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_SLOW_FLASH))
        {
            *LampStatus |= 0x0000u;/*SWS_Dem_00897]*/
        }
        if ((pWIRAttr->DemIndicatorRef == DEM_PROTECT_LAMP_INDICATOR)
                && (DemWIRStatus[pWIRAttr->DemIndicatorRef] != DEM_INDICATOR_OFF)
                && (pWIRAttr->DemIndicatorBehaviour ==  DEM_INDICATOR_FAST_FLASH))
        {
            *LampStatus |= 0x0100u;/*SWS_Dem_00898]*/
        }
#endif
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif


#if(DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Clears active DTCs as well as previously active DTCs
 * ServiceId           0x95
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCTypeFilter:The following types are available:
                        DEM_J1939DTC_CLEAR_ALL
                        DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE
 *                      node: Nm node Id of requesting client
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_J1939DcmClearDTC(
        Dem_J1939DcmSetClearFilterType DTCTypeFilter,
        uint8 node
    )
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr;

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMCLEARDTC, DEM_E_UNINIT);
        return DEM_CLEAR_FAILED;
    }

    pClr = &DemClearDTCInfo;
    if (DemClearDTCLock == DEM_CLEAR_LOCK)
    {
        if ( (pClr->DTCTypeFilter == DTCTypeFilter) &&
        (pClr->SID == DEM_SID_J1939DCMCLEARDTC)
        && (pClr->node == node))
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
        pClr->DTCTypeFilter = DTCTypeFilter;
        pClr->SID = DEM_SID_J1939DCMCLEARDTC;
        pClr->node = node;
    }
    else
    {
        ;
    }
    pClr->ClearAllGroup = TRUE;
    pClr->memDest = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    Dem_Pending = TRUE;
    Dem_ClearDTCProcess();

    Dem_ResetDemClearDTCInfo();
    return DEM_CLEAR_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_J1939_EXPANDED_FREEZE_FRAME_SUPPORT == STD_ON)
#if(DEM_J1939_FREEZE_FRAME_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the FreezeFrame filter for a specific node
 * ServiceId           0x96
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      FreezeFrameKind: The following types are available:
                        DEM_J1939DCM_FREEZEFRAME
                        DEM_J1939DCM_EXPANDED_FREEZEFRAME
                        DEM_J1939DCM_SPNS_IN_EXPANDED_
                        FREEZEFRAME
                        node:Nm node Id of requesting client
 * Param-Name[out]     None
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a FreezeFrame filter.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType , DEM_CODE)
Dem_J1939DcmSetFreezeFrameFilter(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind,uint8 node)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint16 dtcRef = 0;
    uint8 iloop = 0;
    boolean IsFind = FALSE;
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    Dem_ReturnSetFilterType res = DEM_FILTER_ACCEPTED;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETFREEZEFRAMEFILTER, DEM_E_UNINIT);
        return DEM_CLEAR_FAILED;
    }
    DemJ1939DcmFreezeFrameFilter.IsSet = TRUE;
    DemJ1939DcmFreezeFrameFilter.FreezeFrameKind = FreezeFrameKind;
    DemJ1939DcmFreezeFrameFilter.node = node;
    DemJ1939DcmFreezeFrameFilter.SPNIndex = 0;

    while ((iloop < entryNum) && (IsFind == FALSE))
    {
       if (pEntry->EventId != 0x00u)
       {
           pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
           dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;

           if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
           		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match kind and J1939DTC*/
           {
               /*SWS_Dem_01101]*/
               if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
               {
                   DemJ1939DcmFreezeFrameFilter.CurrentIndex++;
               }
               else
               {
#if(DEM_J1939_NODE_NUM > 0)
                   index = 0;
                   nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                           DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                   nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                          DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                   while (index < nodeNum)
                   {
                       if (DemJ1939DcmFreezeFrameFilter.node == *nodeIndex)
                       {
#endif
                           DemJ1939DcmFreezeFrameFilter.CurrentIndex = iloop;
                           IsFind = TRUE;
                           res = DEM_FILTER_ACCEPTED;
#if(DEM_J1939_NODE_NUM > 0)
                           break;
                       }
                       index++;
                       nodeIndex++;
                   }
#endif
               }
           }
       }
       iloop++;
       pEntry++;
   }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets next freeze frame data. The function stores the data in the provided DestBuffer
 * ServiceId           0x97
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     J1939DTC:Receives the J1939DTC value. If the return value of
                        the function is other than DEM_FILTERED_OK this
                        parameter does not contain valid data
                        OccurenceCounter:This parameter receives the corresponding occurrence counter.
                        If the return value of the function call
                        is other than DEM_FILTERED_OK this parameter
                        does not contain valid data.
 * Param-Name[in/out]  DestBuffer:This parameter contains a byte pointer that points
                        to the buffer, to which the freeze frame data record
                        shall be written to.
                        BufSize:When the function is called this parameter contains
                        the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType , DEM_CODE)
Dem_J1939DcmGetNextFreezeFrame(uint32* J1939DTC,
        uint8* OccurenceCounter,
        uint8* DestBuffer,
        uint16* BufSize)
{

    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    uint16 dtcRef = 0;
    Dem_EventIdType eventId = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFF = NULL_PTR;
#endif
    uint8 iloop = 0;

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((OccurenceCounter == NULL_PTR) || (J1939DTC == NULL_PTR) || (DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    Dem_MemSet(DestBuffer, 0u, *BufSize);
    if (Dem_Pending == TRUE)
    {/*SWS_Dem_00902]*/
        return DEM_FILTERED_PENDING;
    }
    while ((res != DEM_FILTERED_OK) && (DemJ1939DcmFreezeFrameFilter.IsSet == TRUE)
    && (DemJ1939DcmFreezeFrameFilter.CurrentIndex < entryNum))
    {
        if (pEntry[DemJ1939DcmFreezeFrameFilter.CurrentIndex].EventId != 0x00u)
        {
            if (pEntry->FFNum == 0u)
            {
                return DEM_E_NODATAAVAILABLE;
            }
            eventId = Dem_GetEventInternalId(pEntry[DemJ1939DcmFreezeFrameFilter.CurrentIndex].EventId);
            pEvent = Dem_GetEventInfo(eventId);
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
            		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match J1939DTC*/
            {
#if(DEM_J1939_NODE_NUM > 0)
                index = 0;
                nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                       DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                           DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                while (index < nodeNum)
                {
                    if (DemJ1939DcmFreezeFrameFilter.node == *nodeIndex)
                    {
#endif
                        /*SWS_Dem_01101]*/
                        if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
                        {
                            DemJ1939DcmFreezeFrameFilter.CurrentIndex++;
                            res = DEM_FILTERED_NO_MATCHING_ELEMENT;
                        }
                        else
                        {
                            /*guarantee the event index the new dtc */
                            Dem_GetEventDTC(pEvent->IntId, DEM_DTC_FORMAT_J1939, J1939DTC);
                            *OccurenceCounter = pEvent->OccurrenceCounter;
                            if (*OccurenceCounter > 0x7Fu)
                            {
                                *OccurenceCounter = 0x7Fu;/*SWS_Dem_00882]*/
                            }
                            iloop = 0;
                            while (iloop < DEM_MAX_NUMBER_FF_RECORDS)
                            {
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                                pFF = pEntry->FFList[iloop].Data;
#endif
                                switch (DemJ1939DcmFreezeFrameFilter.FreezeFrameKind)
                                {
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                                case DEM_J1939DCM_FREEZEFRAME:
                                    if (DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].
                                            DemJ1939FreezeFrameClassRef != DEM_J1939_FREEZE_FRAME_INVALID)
                                    {
                                        if (*BufSize < DemJ1939FreezeFrameClass[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                      DemDTCAttributesRef].DemJ1939FreezeFrameClassRef].DataSize)
                                        {
                                            return DEM_FILTERED_BUFFER_TOO_SMALL;
                                        }
                                        Dem_MemCopy(DestBuffer, pFF, DemJ1939FreezeFrameClass[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                                DemDTCAttributesRef].DemJ1939FreezeFrameClassRef].DataSize);
                                        *BufSize = DemJ1939FreezeFrameClass[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                             DemDTCAttributesRef].DemJ1939FreezeFrameClassRef].DataSize;
                                        res = DEM_FILTERED_OK;
                                    }
                                    break;
                                case DEM_J1939DCM_EXPANDED_FREEZEFRAME:
                                    if (DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].
                                            DemJ1939ExpandedFreezeFrameClassRef != DEM_J1939_FREEZE_FRAME_INVALID)
                                    {
                                        if (*BufSize < DemJ1939FreezeFrameClass[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                                 DemDTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef].DataSize)
                                        {
                                            return DEM_FILTERED_BUFFER_TOO_SMALL;
                                        }
                                        Dem_MemCopy(DestBuffer, pFF, DemJ1939FreezeFrameClass[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                                DemDTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef].DataSize);
                                        *BufSize = DemJ1939FreezeFrameClass[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                          DemDTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef].DataSize;
                                        res = DEM_FILTERED_OK;
                                    }
                                    break;
#endif
                                case DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME:
                                default:
                                    DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFREEZEFRAME, DEM_E_WRONG_CONDITION);
                                    break;
                                }
                                iloop++;
                            }
                        }
#if(DEM_J1939_NODE_NUM > 0)
                    }
                    index++;
                    nodeIndex++;
                }
#endif
            }
        }
        DemJ1939DcmFreezeFrameFilter.CurrentIndex++;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               Gets next SPN.
 * ServiceId           0x98
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPNSupported: This parameter contains the next SPN in the ExpandedFreezeFrame
                        SPNDataLength: This parameter contains the corresponding dataLength of the SPN
 * Param-Name[in/out]  None
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType , DEM_CODE)
Dem_J1939DcmGetNextSPNInFreezeFrame(uint32* SPNSupported, uint8* SPNDataLength)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    uint16 dtcRef = 0;
    Dem_EventIdType eventId = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;

#if(DEM_J1939_NODE_NUM > 0)
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 DemJ1939ExFFClassRef = 0;
#endif
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    uint8 iloop = 0;

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((SPNSupported == NULL_PTR) || (SPNDataLength == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    *SPNSupported = 0;
    *SPNDataLength = 0;
    if (Dem_Pending == TRUE)
    {/*SWS_Dem_00905]*/
        return DEM_FILTERED_PENDING;
    }
    while ((res != DEM_FILTERED_OK) && (DemJ1939DcmFreezeFrameFilter.IsSet == TRUE)
    && (DemJ1939DcmFreezeFrameFilter.CurrentIndex < entryNum))
    {
        if (pEntry[DemJ1939DcmFreezeFrameFilter.CurrentIndex].EventId != 0x00u)
        {
            if (pEntry->FFNum == 0u)
            {
                return DEM_E_NODATAAVAILABLE;
            }
            eventId = Dem_GetEventInternalId(pEntry[DemJ1939DcmFreezeFrameFilter.CurrentIndex].EventId);
            pEvent = Dem_GetEventInfo(eventId);
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            if (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID)/*match J1939DTC*/
            {
#if(DEM_J1939_NODE_NUM > 0)
                index = 0;
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                DemJ1939ExFFClassRef = DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939ExpandedFreezeFrameClassRef;
#endif
                nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                while ((index < nodeNum) && (DemJ1939DcmFreezeFrameFilter.IsFind == FALSE))
                {
                    if (DemJ1939DcmFreezeFrameFilter.node == *nodeIndex)
                    {
#endif
                        /*SWS_Dem_01101]*/
                        if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
                        {
                            res = DEM_FILTERED_NO_MATCHING_ELEMENT;
                        }
                        else
                        {
                            iloop = 0;
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                            while (iloop < DEM_MAX_NUMBER_FF_RECORDS)
                            {
                                if (DemJ1939DcmFreezeFrameFilter.IsOK == FALSE)
                                {
                                    DemJ1939DcmFreezeFrameFilter.SPNIndex = DemJ1939FreezeFrameClass[DemJ1939ExFFClassRef].StartIndex;
                                    DemJ1939DcmFreezeFrameFilter.SPNCounter = 0;
                                    DemJ1939DcmFreezeFrameFilter.IsOK = TRUE;
                                }
#endif
                                switch (DemJ1939DcmFreezeFrameFilter.FreezeFrameKind)
                                {
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                                case DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME:
                                    if (DemJ1939ExFFClassRef != DEM_J1939_FREEZE_FRAME_INVALID)
                                    {
                                        while (DemJ1939DcmFreezeFrameFilter.SPNCounter < DemJ1939FreezeFrameClass[DemJ1939ExFFClassRef].RefNum)
                                        {
                                            *SPNSupported = DemSPNClass[DemJ1939DcmFreezeFrameFilter.SPNIndex].DemSPNIdentifier;
                                            *SPNDataLength = DemSPNClass[DemJ1939DcmFreezeFrameFilter.SPNIndex].DataSize;
                                            DemJ1939DcmFreezeFrameFilter.SPNCounter++;
                                            DemJ1939DcmFreezeFrameFilter.SPNIndex++;
                                            res = DEM_FILTERED_OK;
                                        }
                                    }
                                    break;
#endif
                                case DEM_J1939DCM_FREEZEFRAME:
                                case DEM_J1939DCM_EXPANDED_FREEZEFRAME:
                                default:
                                    DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTSPNINFREEZEFRAME, DEM_E_WRONG_CONDITION);
                                    break;
                                }
                                iloop++;
#if(DEM_J1939_FREEZE_FRAME_CLASS_NUM > 0)
                            }
#endif
                            DemJ1939DcmFreezeFrameFilter.IsFind = TRUE;
                        }
#if(DEM_J1939_NODE_NUM > 0)
                    }
                    index++;
                    nodeIndex++;
                }
#endif
            }
        }
        DemJ1939DcmFreezeFrameFilter.CurrentIndex++;
        DemJ1939DcmFreezeFrameFilter.IsFind = FALSE;
        DemJ1939DcmFreezeFrameFilter.IsOK = FALSE;
        DemJ1939DcmFreezeFrameFilter.SPNCounter = 0;
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_J1939_RATIO_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 * corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           0x99
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     IgnitionCycleCounter: Ignition Cycle Counter
                        OBDMonitoringConditionsEncountered: OBD Monitoring Conditions Encountered
 * Param-Name[in/out]  None
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType , DEM_CODE)
Dem_J1939DcmSetRatioFilter(uint16* IgnitionCycleCounter,
        uint16* OBDMonitoringConditionsEncountered,
        uint8 node)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint16 dtcRef = 0;
    uint8 iloop = 0;
    boolean IsFind = FALSE;
#if(DEM_RATIO_NUM > 0)
    uint16 Ratioindex = 0;
#endif
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    Dem_ReturnSetFilterType res = DEM_FILTER_ACCEPTED;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETRATIOFILTER, DEM_E_UNINIT);
        return DEM_WRONG_FILTER;
    }
    if ((IgnitionCycleCounter == NULL_PTR) || (OBDMonitoringConditionsEncountered == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMSETRATIOFILTER, DEM_E_PARAM_POINTER);
        return DEM_WRONG_FILTER;
    }
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    *IgnitionCycleCounter = IgnUpCycleCounter;
#endif
#if(DEM_RATIO_NUM > 0)
    *OBDMonitoringConditionsEncountered = IUMPRValue[0].General_Denominator;
#endif
    DemJ1939DcmRatioFilter.node = node;
    DemJ1939DcmRatioFilter.IsSet = TRUE;
    DemJ1939DcmRatioFilter.CurrentIndex = 0;

    while ((iloop < entryNum) && (IsFind == FALSE))
    {
       if (pEntry->EventId != 0x00u)
       {
           pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
           dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;

           if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
           		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match kind and J1939DTC*/
           {
               /*SWS_Dem_01101]*/
               if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
               {
                   res = DEM_WRONG_FILTER;
               }
               else
               {
#if(DEM_J1939_NODE_NUM > 0)
                   index = 0;
                   nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                   nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                   DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                   while ((index < nodeNum) && (IsFind == FALSE))
                   {
                       if (DemJ1939DcmRatioFilter.node == *nodeIndex)
                       {
#endif
#if(DEM_RATIO_NUM > 0)
                           Ratioindex = 0;
                           while ((Ratioindex < DEM_RATIO_NUM) && (IsFind == FALSE))
                           {
                               if (DemRatio[Ratioindex].DemDiagnosticEventRef == pEvent->IntId)
                               {
                                   DemJ1939DcmRatioFilter.CurrentIndex = iloop;
                                   IsFind = TRUE;
                                   res = DEM_FILTER_ACCEPTED;
                               }
                               Ratioindex++;
                           }
#endif
#if(DEM_J1939_NODE_NUM > 0)
                       }
                       index++;
                       nodeIndex++;
                   }
#endif
               }
           }
       }
       iloop++;
       pEntry++;
   }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               The function sets the Ratio filter for a specific node and returns the
 * corresponding Ignition Cycle Counter and General Denominator.
 * ServiceId           0x9a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SPN:Receives the SPN of the applicaple system monitor. If the return value of the function is other than
                        DEM_FILTERED_OK this parameter does not contain valid data.
                        Numerator:Receives the Numerator of the applicable system
                        monitor. If the return value of the function is other
                        than DEM_FILTERED_OK this parameter does not
                        contain valid data.
                        Denominator:Receives the Denominator of the applicable system
                        monitor. If the return value of the function is other
                        than DEM_FILTERED_OK this parameter does not
                        contain valid data.
 * Param-Name[in/out]  None
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType , DEM_CODE)
Dem_J1939DcmGetNextFilteredRatio(uint16* SPN,
        uint16* Numerator,
        uint16* Denominator)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
    uint8 entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint16 dtcRef = 0;
#if(DEM_RATIO_NUM > 0)
    uint16 Ratioindex = 0;
#endif
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDRATIO, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((SPN == NULL_PTR) || (Numerator == NULL_PTR) || (Denominator == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMGETNEXTFILTEREDRATIO, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if (Dem_Pending == TRUE)
    {/*SWS_Dem_00913]*/
        return DEM_FILTERED_PENDING;
    }
    while ((DemJ1939DcmRatioFilter.CurrentIndex < entryNum)
            && (res != DEM_FILTERED_OK) && (DemJ1939DcmRatioFilter.IsSet == TRUE))
   {
       if (pEntry[DemJ1939DcmRatioFilter.CurrentIndex].EventId != 0x00u)
       {
           pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry[DemJ1939DcmRatioFilter.CurrentIndex].EventId));
           dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
           if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
           		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match kind and J1939DTC*/
           {
               /*SWS_Dem_01101]*/
               if (DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE)
               {
#if(DEM_J1939_NODE_NUM > 0)
                   index = 0;
                   nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                             DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                   nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                              DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                   while ((index < nodeNum) && (res != DEM_FILTERED_OK))
                   {
                       if (DemJ1939DcmRatioFilter.node == *nodeIndex)
                       {
#endif
#if(DEM_RATIO_NUM > 0)
                           Ratioindex = 0;
                           while ((Ratioindex < DEM_RATIO_NUM) && (res != DEM_FILTERED_OK))
                           {
                               if ((DemRatio[Ratioindex].DemDiagnosticEventRef == pEvent->IntId)
                                       && (0x00u != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_AVAILABLE)))/*SWS_Dem_01104] */
                               {
                                   *SPN = (uint16)(((DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue & 0xff0000u) >> 16u)
                                           | (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue & 0x00ff00u));
                                   /*version 4 shall be used TODO:SPN in 1939-73 is 19 bits, and there only 16bits to reported*/
                                   *Numerator = IUMPRValue[Ratioindex].Numerator;
                                   *Denominator = IUMPRValue[Ratioindex].Denominator;
                                   res = DEM_FILTERED_OK;
                               }
                               Ratioindex++;
                           }
#endif
#if(DEM_J1939_NODE_NUM > 0)
                       }
                       index++;
                       nodeIndex++;
                   }
#endif
               }
           }
       }
       DemJ1939DcmRatioFilter.CurrentIndex++;
       pEntry++;
   }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 1 (DM05) computed by the Dem.
 * ServiceId           0x9b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 1 (DM05) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType , DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness1(Dem_J1939DcmDiagnosticReadiness1Type* DataValue,uint8 node)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    uint8 iloop = 0;
    uint8 entryNum = 0;
    uint16 dtcRef = 0;
#if(DEM_J1939_NODE_NUM > 0)
    uint8 index = 0;
    boolean IsFind = FALSE;
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
#endif
    uint8  ReadinessGroupIndex = 0;
    uint8  supportedGroups[3];
    uint8  notCompletedGroups[3];
    uint32 J1939Dtc;
    uint16  obdDtcRef;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (DataValue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS1, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    Dem_MemSet(supportedGroups, 0u, (uint8)3);
    Dem_MemSet(notCompletedGroups, 0u, (uint8)3);
    DataValue->ActiveTroubleCodes = 0;
    DataValue->ContinuouslyMonitoredSystemsSupport_Status = 0;
    DataValue->NonContinuouslyMonitoredSystemsStatus = 0;
    DataValue->NonContinuouslyMonitoredSystemsSupport = 0;
    DataValue->OBDCompliance = 0xff;
    DataValue->PreviouslyActiveDiagnosticTroubleCodes = 0;
    entryNum = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryNum;
    pEntry = DemMemDestCfg[Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY)].EntryList;
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    DataValue->OBDCompliance = DemGeneralOBD.DemOBDCompliancy;
#endif
    while (iloop < entryNum)
    {
        if (pEntry->EventId != 0x00u)
        {
            pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
            		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))/*match J1939DTC*/
            {
                if (DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE)
                {/*SWS_Dem_01101]*/
#if(DEM_J1939_NODE_NUM > 0)
                    index = 0;
                    nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                                    DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                    nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
                                              DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                    while (index < nodeNum)
                    {
                        if (node == *nodeIndex)
                        {
                            IsFind = TRUE;/*find the node*/
                        }
                        index++;
                        nodeIndex++;
                    }
                    if (IsFind == TRUE)
                    {
                        IsFind = FALSE;
#endif
#if(DEM_INDICATOR_NUM > 0)
                        if (((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                                && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
                                || (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].
                                              AttrStartIndex].DemIndicatorRef] != DEM_INDICATOR_OFF))
                        {
                            DataValue->ActiveTroubleCodes +=1u;
                        }
                        if ((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                                && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
                                && (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].
                                          AttrStartIndex].DemIndicatorRef] == DEM_INDICATOR_OFF))
                        {
                            DataValue->PreviouslyActiveDiagnosticTroubleCodes +=1u;
                        }
#endif
#if(DEM_J1939_NODE_NUM > 0)
                    }
#endif
                }
            }
        }
        iloop++;
        pEntry++;
    }
    while (ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FORJ1939)
    {
        iloop = 0;
        while (iloop < DEM_EVENT_PARAMETER_NUM)
        {
            dtcRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
            obdDtcRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;
            /*SWS_Dem_01101]*/
            if (DemDTCGeneralStatus[dtcRef].SuppressionStatus != TRUE)
            {
                if (DemPbCfgPtr->DemObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex)
                {
                    J1939Dtc = DemPbCfgPtr->DemObdDTC[obdDtcRef].DemJ1939DTCValue;
                    if ((J1939Dtc != 0UL) && (obdDtcRef != DEM_OBD_DTC_INVALID))
                    {/*SWS_Dem_01101]*/
#if(DEM_J1939_NODE_NUM > 0)
						index = 0;
						nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
												  DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
						nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].
											   DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
						while (index < nodeNum)
						{
							if (node == *nodeIndex)
							{
#endif
								DEM_BITS_SET(supportedGroups, ReadinessGroupIndex);
								if (((0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TF))
										&& (0x00u == DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_TNCSLC)))
										|| (0x00u != DEM_FLAGS_ISSET(DemEventRelateInformation[iloop].UdsStatus, DEM_UDS_STATUS_WIR)))
								{
									;
								}
								else
								{
									DEM_BITS_SET(notCompletedGroups, ReadinessGroupIndex);
								}
#if(DEM_J1939_NODE_NUM > 0)
							}
							index++;
							nodeIndex++;
						}
#endif
                    }
                }
            }
            iloop++;
        }
        ReadinessGroupIndex++;
    }
    DataValue->ContinuouslyMonitoredSystemsSupport_Status = ((supportedGroups[0u] & 0xFu) | ((uint8)((notCompletedGroups[0u] & 0xFu) << 4u)));
    DataValue->NonContinuouslyMonitoredSystemsSupport = (((uint16)(((uint16)supportedGroups[0] & 0xF0u) >> 4u))
            | (uint16)(((uint16)supportedGroups[1u] & 0xFFu) << 4u) | (uint16)((((uint16)supportedGroups[2u] & 0xFFu) << 12u)));
    DataValue->NonContinuouslyMonitoredSystemsStatus = ((((uint16)notCompletedGroups[0u] & 0xF0u) >> 4u)
            | (uint16)(((uint16)notCompletedGroups[1u] & 0xFFu) << 4u) | ((uint16)(((uint16)notCompletedGroups[2u] & 0xFFu) << 12u)));
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 2 (DM21) computed by the Dem.
 * ServiceId           0x9c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 2 (DM21) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType , DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness2(
	Dem_J1939DcmDiagnosticReadiness2Type* DataValue,
	uint8 node
)
{
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    uint16 DistWithMilOn = 0U;
    uint16 currentDistanceInformation = 0U;
    uint16 TimeWithMilOn  = 0U;
    uint16 CurrentTimeInformation = 0U;
#endif
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS2, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (DataValue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS2, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    DataValue->DistanceSinceDTCsCleared = 0;
    DataValue->DistanceTraveledWhileMILisActivated = 0;
    DataValue->MinutesRunbyEngineWhileMILisActivated = 0;
    DataValue->TimeSinceDiagnosticTroubleCodesCleared = 0;
    DEM_UNUSED(node);
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    /* MinutesRunbyEngineWhileMILisActivated */
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
    if (OBDTimeMILOn < 0xFAFFu) /*J1939-73*/
    {
        DataValue->MinutesRunbyEngineWhileMILisActivated = OBDTimeMILOn;
    }
    else
    {
        DataValue->MinutesRunbyEngineWhileMILisActivated = 0xFAFFu;
    }
    /*TimeSinceDiagnosticTroubleCodesCleared*/
    CurrentTimeInformation = Dem_ReadDistanceInformation();
    if (CurrentTimeInformation > OBDTimeDTCLastClear)
    {
        OBDTimeDTCClear = CurrentTimeInformation - OBDTimeDTCLastClear;
    }
    if (OBDTimeDTCClear < 0xFAFFu)
    {
        DataValue->TimeSinceDiagnosticTroubleCodesCleared = OBDTimeDTCClear;
    }
    else
    {
        DataValue->TimeSinceDiagnosticTroubleCodesCleared = 0xFAFFu;
    }
    /*DistanceSinceDTCsCleared*/
    currentDistanceInformation = Dem_ReadDistanceInformation();
    if (currentDistanceInformation > OBDDistanceDTCClear)
    {
        DistSinceDtcCleared = currentDistanceInformation - OBDDistanceDTCClear;
    }
    if (DistSinceDtcCleared < 0xFAFFu)
    {
        DataValue->DistanceSinceDTCsCleared = DistSinceDtcCleared;
    }
    else
    {
        DataValue->DistanceSinceDTCsCleared = 0xFAFFu;
    }
    /*DistanceTraveledWhileMILisActivated*/
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
    if (OBDDistanceMILOn < 0xFAFFu)
    {
        DataValue->DistanceTraveledWhileMILisActivated = OBDDistanceMILOn;
    }
    else
    {
        DataValue->DistanceTraveledWhileMILisActivated = 0xFAFFu;
    }
    return E_OK;
#else
    return E_NOT_OK;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#if(DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*************************************************************************/
/*
 * Brief               Service to report the value of Diagnostic Readiness 3 (DM26) computed by the Dem.
 * ServiceId           0x9d
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      node:Nm node Id of requesting client
 * Param-Name[out]     DataValue:Buffer of 8 bytes containing the contents of Diagnostic Readiness 3 (DM26) computed by the Dem.
 * Param-Name[in/out]  None
 * Return              E_OK: Operation was successful
                        E_NOT_OK: Operation failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType , DEM_CODE)
Dem_J1939DcmReadDiagnosticReadiness3(
	Dem_J1939DcmDiagnosticReadiness3Type* DataValue,
	uint8 node
)
{
    uint16 iloop = 0;
    uint16 ReadinessGroupIndex = 0;
    uint8  enabledGroups[3];
    uint8  notCompletedGroups[3];
    uint32 J1939Dtc;
    uint16  obdDtcRef;
    uint16  tempRef;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS3, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    if (DataValue == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_J1939DCMREADDIAGNOSTICREADINESS3, DEM_E_PARAM_POINTER);
        return E_NOT_OK;
    }
    DEM_UNUSED(node);
    Dem_MemSet(enabledGroups, 0xffu, (uint8)3);
    Dem_MemSet(notCompletedGroups, 0u, (uint8)3);
    DataValue->ContinuouslyMonitoredSystemsEnableCompletedStatus = 0;
    DataValue->NonContinuouslyMonitoredSystems = 0;
    DataValue->NonContinuouslyMonitoredSystemsEnableStatus = 0;
    DataValue->NumberofWarmupsSinceDTCsCleared = 0;
    DataValue->TimeSinceEngineStart = 0;
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    DataValue->TimeSinceEngineStart = Dem_ReadTimeInformation();
    DataValue->NumberofWarmupsSinceDTCsCleared = WarmUpCycleCounter;
#endif
    while (ReadinessGroupIndex < DEM_NUMBER_OF_READINESS_GROUPS_FORJ1939)
    {
        iloop = 0;
        while (iloop < DEM_EVENT_PARAMETER_NUM)
        {
            tempRef = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
            obdDtcRef = DemPbCfgPtr->DemDTC[tempRef].DemObdDTCRef;
            if (DemDTCGeneralStatus[tempRef].SuppressionStatus != TRUE)
            {/*SWS_Dem_01101]*/
                if (DemPbCfgPtr->DemObdDTC[obdDtcRef].DemEventOBDReadinessGroup == ReadinessGroupIndex)
                {
                    /* Only include emission related DTCs */
                    J1939Dtc = DemPbCfgPtr->DemObdDTC[obdDtcRef].DemJ1939DTCValue;
                    if ((J1939Dtc != 0UL) &&(obdDtcRef != DEM_OBD_DTC_INVALID))
                    {
                        pEvent = Dem_GetEventInfo(iloop);
                        if ((0u != DEM_BITS_ISSET(DemOperationCycleStatus,DemPbCfgPtr->DemEventParameter[iloop].DemOperationCycleRef))
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
            iloop++;
        }
        ReadinessGroupIndex++;
    }
    DataValue->ContinuouslyMonitoredSystemsEnableCompletedStatus = ((enabledGroups[0u] & 0xFu) | (uint8)((notCompletedGroups[0u] & 0xFu) << 4u));
    DataValue->NonContinuouslyMonitoredSystemsEnableStatus = ((uint16)(((uint16)enabledGroups[0u] & 0xF0u) >> 4u)
            | (uint16)((((uint16)enabledGroups[1u] & 0xFFu) << 4u)) | (uint16)((((uint16)enabledGroups[2u] & 0xFFu) << 12u)));
    DataValue->NonContinuouslyMonitoredSystems = ((uint16)(((uint16)notCompletedGroups[0u] & 0xF0u) >> 4u)
            | (uint16)(((uint16)notCompletedGroups[1u] & 0xFFu) << 4u) | (uint16)(((uint16)notCompletedGroups[2u] & 0xFFu) << 12u));
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#endif
/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
