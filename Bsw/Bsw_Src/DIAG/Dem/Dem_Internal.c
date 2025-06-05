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
**  FILENAME    : Dem_Internal.c                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for internal                         **
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
#define DEM_INTERNAL_C_AR_MAJOR_VERSION   4U
#define DEM_INTERNAL_C_AR_MINOR_VERSION   2U
#define DEM_INTERNAL_C_AR_PATCH_VERSION   2U
#define DEM_INTERNAL_C_SW_MAJOR_VERSION   2U
#define DEM_INTERNAL_C_SW_MINOR_VERSION   0U
#define DEM_INTERNAL_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"
#include "Dem.h"
#if(DEM_TRIGGER_DCM_REPORTS == STD_ON)
#include "Dcm.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_INTERNAL_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_Internal.c : Mismatch in Specification Major Version"
#endif
#if (DEM_INTERNAL_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_Internal.c : Mismatch in Specification Major Version"
#endif
#if (DEM_INTERNAL_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_Internal.c : Mismatch in Specification Major Version"
#endif
#if (DEM_INTERNAL_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_Internal.c : Mismatch in Specification Major Version"
#endif
#if (DEM_INTERNAL_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_Internal.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
#define DEM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
VAR(Dem_ClearLockStatus,AUTOMATIC) DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) DemClearNonvolatile = FALSE;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"
VAR(uint8,AUTOMATIC) DemClearNonvolatileOK = 0x02u;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_ClearDTCInfoType,AUTOMATIC) DemClearDTCInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_InternalDataType,AUTOMATIC) DemInternalData;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_INIT_BOOLEAN
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) Dem_Pending = FALSE;
#define DEM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_ClearAllDTC(uint8 memDest);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
#if(DEM_GROUP_OF_DTC_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_ClearGroupDTC(uint8 memDest,
    uint8 GroupIndex);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_ClearOneDTC(uint8 memDest,
    uint16 DTCIndex);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               MemSet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Val && Size
 * Param-Name[out]     none
 * Param-Name[in/out]  Dest
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_MemSet(
    P2VAR(uint8, AUTOMATIC, DEM_VAR) Dest,
    uint8 Val,
    uint32 Size)
{
    while (Size > 0UL)
    {
        *(Dest) = Val;
        Dest++;
        Size--;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               MemCopy
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Src && Size
 * Param-Name[out]     Dest
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_MemCopy(
    P2VAR(void, AUTOMATIC, DEM_VAR) Dest,
    P2CONST(void, AUTOMATIC, DEM_VAR) Src,
    uint32 Size)
{
    uint8 * pDest = (uint8 *)Dest;
    const uint8 * pSrc = (const uint8 *)Src;
    while (Size > 0u)
    {
    	Size--;
        pDest[Size] = pSrc[Size];
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               CheckCondictionFulfilled
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cond && group && len
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              boolean
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(boolean, DEM_CODE) Dem_CheckCondictionFulfilled(
    P2CONST(uint8, AUTOMATIC, DEM_VAR) cond,
    P2CONST(uint8, AUTOMATIC, DEM_VAR) group,
    uint8 len)
{
    boolean res = TRUE;

    while ((len > 0u) && (res != FALSE))
    {
        len--;
        if (((*cond) & (*group)) != *group)
        {
            res = FALSE;
        }
        else
        {
            cond++;
            group++;
        }
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_DTC_ATTRIBUTES_NUM > 0)
/*************************************************************************/
/*
 * Brief               Get EventDTCAttributesCfg
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      IntId
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              const Dem_DTCAttributesType*
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_EventDTCAttributesCfg(Dem_EventIdType IntId)
{
    uint16 Ref;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pRes = NULL_PTR;

    /* DtcRef */
    Ref = DemPbCfg.DemEventParameter[IntId].DemDTCRef;
    if (Ref != DEM_DTC_REF_INVALID)
    {
        /* DTCAttributesRef */
        Ref = DemPbCfg.DemDTC[Ref].DemDTCAttributesRef;
        pRes = &DemPbCfg.DemDTCAttributes[Ref];
    }

    return pRes;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*************************************************************************/
/*
 * Brief               GetInternalMemDest
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCOrigin
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              uint8
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(uint8, DEM_CODE) Dem_GetInternalMemDest(Dem_DTCOriginType DTCOrigin)
{
    uint8 iloop = 0;

    while (iloop < DEM_MEM_DEST_TOTAL_NUM)
    {
        if (DemMemDestCfg[iloop].ExtId == DTCOrigin)
        {
            return iloop;
        }
        iloop++;
    }
    return DEM_MEM_DEST_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

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
FUNC(Dem_EventIdType, DEM_CODE) Dem_GetEventIdByDTC(uint32 DTC)
{
    Dem_EventIdType id = 0;

    while (id < DEM_EVENT_PARAMETER_NUM)
    {
        if ((DemPbCfgPtr->DemEventParameter[id].DemDTCRef != DEM_DTC_REF_INVALID)
            && ((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[id].DemDTCRef].DemDtcValue == DTC)
            || (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[id].DemDTCRef].DemObdDTCRef].DemJ1939DTCValue == DTC)
            || (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[id].DemDTCRef].DemObdDTCRef].DemDtcValue == DTC))
            )
        {
            return id;
        }
        id++;
    }
    return DEM_EVENT_PARAMETER_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

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
FUNC(void, DEM_CODE) Dem_TriggerOnEventStatus(
	Dem_EventIdType IntId,
    Dem_UdsStatusByteType OldStatus,
    Dem_UdsStatusByteType NewStatus
)
{
#if(DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemPbCfgPtr->DemEventParameter[IntId];
    uint8 iloop = 0;
    P2CONST(Dem_TriggerOnEventStatusType, AUTOMATIC, DEM_CONST) pCbk =
        &DemCallbackEventStatusChanged[pEventCfg->StatusChangedCbkStartIndex];
#else
    DEM_UNUSED(OldStatus);
    DEM_UNUSED(NewStatus);
#endif

#if (DEM_TRIGGER_DCM_REPORTS == STD_ON)
    uint32 DTC = 0x00;
#else
    DEM_UNUSED(IntId);
#endif
    /*SWS_Dem_00828]*/
    if (DemDTCStatusChangedInd == TRUE)
    {
#if (DEM_TRIGGER_DCM_REPORTS == STD_ON)
        /*SWS_Dem_00284]*/
        Dem_GetEventDTC(IntId, DEM_DTC_FORMAT_UDS, &DTC);
        if (DTC != 0x00UL)
        {
            (void)Dcm_DemTriggerOnDTCStatus(DTC,OldStatus,NewStatus);
#if(DEM_CALLBACK_DTC_STATUS_CHANGED_NUM > 0)
            while(iloop < DEM_CALLBACK_DTC_STATUS_CHANGED_NUM)
            {
                if(DemCallbackDTCStatusChanged[iloop] != NULL_PTR)
                {
                    (void)(DemCallbackDTCStatusChanged[iloop])(DTC,OldStatus,NewStatus);
                }
                iloop++;
            }
#endif
        }
        /*SWS_Dem_00986]*/
        Dem_GetEventDTC(IntId, DEM_DTC_FORMAT_OBD, &DTC);
        iloop = 0;
        if (DTC != 0x00UL)
        {
#if(DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM > 0)
            while(iloop < DEM_CALLBACK_OBD_DTC_STATUS_CHANGED_NUM)
            {
                if(DemCallbackObdDTCStatusChanged[iloop] != NULL_PTR)
                {
                    (void)(DemCallbackObdDTCStatusChanged[iloop])(DTC,OldStatus,NewStatus);
                }
                iloop++;
            }
#endif
        }
        /*SWS_Dem_00987]*/
        Dem_GetEventDTC(IntId, DEM_DTC_FORMAT_J1939, &DTC);
        iloop = 0;
        if (DTC != 0x00UL)
        {
#if(DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM > 0)
            while(iloop < DEM_CALLBACK_J1939_DTC_STATUS_CHANGED_NUM)
            {
                if(DemCallbackJ1939DTCStatusChanged[iloop] != NULL_PTR)
                {
                    (void)(DemCallbackJ1939DTCStatusChanged[iloop])(DTC,OldStatus,NewStatus);
                }
                iloop++;
            }
#endif
        }
#endif
    }
    /*SWS_Dem_00016] */
#if(DEM_CALLBACK_EVENT_STATUS_CHANGED_TOTAL_NUM > 0)
    iloop = 0;
    while (iloop < pEventCfg->StatusChangedCbkNum)
    {
        if (*pCbk != NULL_PTR)
        {
            (void)(*pCbk)(IntId + 1u, OldStatus, NewStatus);
        }
        pCbk++;
        iloop++;
    }
#endif
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               ClearAllDTC
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      memDest
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_ClearAllDTC(uint8 memDest)
{
    uint8 iloop = 0;
    uint16 dtcRef;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr = &DemClearDTCInfo;
#if(DEM_J1939_NODE_NUM > 0)
    P2CONST(uint8, AUTOMATIC, DEM_VAR) nodeIndex;
    uint8 nodeNum;
    uint8 index = 0;
#endif

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        pEvent = &DemEventRelateInformation[iloop];
        dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;

        if ((DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
        		&&(DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
        {
            if ((pClr->SID == DEM_SID_J1939DCMCLEARDTC)
                && (((pClr->DTCTypeFilter == DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE)
                        && ((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                        && (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
    #if(DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0)
                        && (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].AttrStartIndex].DemIndicatorRef] == DEM_INDICATOR_OFF)
    #endif
    					))
                || ((pClr->DTCTypeFilter == DEM_J1939DTC_CLEAR_ALL)
                        && (((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_CDTC))
                        && (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF)))
    #if(DEM_INDICATOR_ATTRIBUTE_TOTAL_NUM > 0)
                        || (DemWIRStatus[DemIndicatorAttribute[DemPbCfgPtr->DemEventParameter[pEvent->IntId].AttrStartIndex].DemIndicatorRef] != DEM_INDICATOR_OFF)
    #endif
    					))))
            {
    #if(DEM_J1939_NODE_NUM > 0)
                index = 0;
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                nodeIndex = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeIDRef;
                nodeNum = DemPbCfgPtr->J1939Node[DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef].DemJ1939DTC_J1939NodeRef].NodeNum;
                while (index < nodeNum)
                {
                    if (pClr->node == *nodeIndex)
                    {
                        Dem_Clear(pEvent,memDest);/*find the node*/
                        break;
                    }
                    index++;
                    nodeIndex++;
                }
    #endif
            }
        }
        else
        {
            Dem_Clear(pEvent,memDest);
        }
        iloop++;
    }
    /*SWS_Dem_00399] */
    if (DemMemDestInfo[memDest].RecordNum == 0u)
    {
        DemMemDestInfo[memDest].OverFlow = FALSE;
    }
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    Dem_ClearOBDInfo();
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
#if(DEM_GROUP_OF_DTC_NUM > 0)
/*************************************************************************/
/*
 * Brief               ClearGroupDTC
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
static FUNC(void, DEM_CODE) Dem_ClearGroupDTC(uint8 memDest,
    uint8 GroupIndex)
{
    uint8 iloop = 0;
    uint16 Ref;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
		Ref = DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef;
		pEvent = &DemEventRelateInformation[iloop];
		if ((Ref != DEM_DTC_REF_INVALID)
				&&(DemPbCfgPtr->DemDTC[Ref].GroupRef == GroupIndex)
				&&(0x00 != DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_ENABLED_CONDICTION)))
		{
			Dem_Clear(pEvent,memDest);
		}
		iloop++;
    }
    /*SWS_Dem_00399] */
    if (DemMemDestInfo[memDest].RecordNum == 0u)
    {
        DemMemDestInfo[memDest].OverFlow = FALSE;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
/*************************************************************************/
/*
 * Brief               ClearOneDTC
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
static FUNC(void, DEM_CODE) Dem_ClearOneDTC(uint8 memDest,
    uint16 DTCIndex)
{
    uint16 iloop = 0;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if (DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef == DTCIndex)
        {
			pEvent = &DemEventRelateInformation[iloop];
			Dem_Clear(pEvent, memDest);
        }
        iloop++;
    }
    /*SWS_Dem_00399] */
    if (DemMemDestInfo[memDest].RecordNum == 0u)
    {
        DemMemDestInfo[memDest].OverFlow = FALSE;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               ClearDTCProcess
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
FUNC(void, DEM_CODE) Dem_ClearDTCProcess(void)
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr = &DemClearDTCInfo;

    if (pClr->memDest == DEM_MEM_DEST_INVALID)
    {
        return;/*SWS_Dem_00171]*/
    }

    if (pClr->ClearAllGroup == TRUE)
    {
        Dem_ClearAllDTC(pClr->memDest);
    }
#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
    else
    {
        if (pClr->DTCGroupIndex != DEM_GROUP_OF_DTC_INVALID)
        {
#if(DEM_GROUP_OF_DTC_NUM > 0)
            Dem_ClearGroupDTC(pClr->memDest, pClr->DTCGroupIndex);
#endif
        }
        else
        {
            if (pClr->DTCIndex != DEM_DTC_REF_INVALID)
            {
                Dem_ClearOneDTC(pClr->memDest, pClr->DTCIndex);
            }
        }
    }
#endif
    Dem_Pending = FALSE;
    DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC && DTCFormat
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              DTC Index
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(uint16, DEM_CODE) Dem_GetDTCIndex(uint32 DTC,
    Dem_DTCFormatType DTCFormat)
{
    uint16 index = 0;

    switch(DTCFormat)
    {
        case DEM_DTC_FORMAT_UDS:
        while (index < DEM_DTC_NUM)
        {
            if (DemPbCfgPtr->DemDTC[index].DemDtcValue == DTC)
            {
                return index;
            }
            index++;
        }
        break;

        case DEM_DTC_FORMAT_OBD:
        while (index < DEM_DTC_NUM)
        {
            if ((DemPbCfgPtr->DemDTC[index].DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[index].DemObdDTCRef].DemDtcValue == DTC))
            {
                return index;
            }
            index++;
        }
        break;

        case DEM_DTC_FORMAT_J1939:
        while (index < DEM_DTC_NUM)
        {
            if ((DemPbCfgPtr->DemDTC[index].DemObdDTCRef != DEM_OBD_DTC_INVALID)
                    && (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[index].DemObdDTCRef].DemJ1939DTCValue == DTC))
            {
                return index;
            }
            index++;
        }
        break;
        default:
        break;
    }
    return DEM_DTC_REF_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_GROUP_OF_DTC_NUM > 0)
/*************************************************************************/
/*
 * Brief               GetDTCGroupIndex
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      Id
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              DTCGroupIndex
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(uint8, DEM_CODE) Dem_GetDTCGroupIndex(uint32 Id)
{
    uint8 iloop = 0;

    while (iloop < DEM_GROUP_OF_DTC_NUM)
    {
        if (DemGroupOfDTC[iloop] == Id)
        {
            return iloop;
        }
        iloop++;
    }
    return DEM_GROUP_OF_DTC_INVALID;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               Dem_UpdateInternalData
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
FUNC(void, DEM_CODE) Dem_UpdateInternalData(uint16 IntId)
{
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg = &DemPbCfg.DemEventParameter[IntId];
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent = &DemEventRelateInformation[IntId];
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pMemDest;
    uint8 iloop = 0;

    DemInternalData.Occctr = pEvent->OccurrenceCounter;/*SWS_Dem_00471]*/
	if (pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
	{
		pDTCAttrCfg =
					&DemPbCfg.DemDTCAttributes[DemPbCfg.DemDTC[pEventCfg->DemDTCRef].DemDTCAttributesRef];

		if (pDTCAttrCfg->DemAgingAllowed == FALSE)
		{
			DemInternalData.AgingUpCnt = 0;/*SWS_Dem_01044]*/
		}
		else
		{
			DemInternalData.AgingUpCnt = pEvent->AgingCounter;/*SWS_Dem_00643]*/
		}
		if (pDTCAttrCfg->DemAgingAllowed == FALSE)
		{
			DemInternalData.AgingDownCnt = pDTCAttrCfg->DemAgingCycleCounterThreshold;/*SWS_Dem_01043]*/
		}
		else
		{
			DemInternalData.AgingDownCnt = pDTCAttrCfg->DemAgingCycleCounterThreshold - pEvent->AgingCounter;/*SWS_Dem_00673]*/
		}
		pMemDest = pDTCAttrCfg->DemMemoryDestinationRef;
		DemInternalData.Ovflind = FALSE;
		while (iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC)
		{
			if (pMemDest[iloop] != DEM_MEM_DEST_INVALID)
			{
				if (DemMemDestInfo[pMemDest[iloop]].OverFlow == TRUE)
				{/*if the memDest have one OverFlow is true, then the internal OverFlow is true*/
					DemInternalData.Ovflind = TRUE;/*SWS_Dem_00473] */
				}
			}
			iloop++;
		}
		DemInternalData.Significance = pDTCAttrCfg->DemDTCSignificance;/*SWS_Dem_01084]*/
		DemInternalData.MaxFDCDuringCurrentCycle = FDCInfo[pEventCfg->DemDTCRef].MaxFDCDuringCurrentCycle;/*SWS_Dem_00819]*/
		DemInternalData.MaxFDCSinceLastClear = FDCInfo[pEventCfg->DemDTCRef].MaxFDCSinceLastClear;/*SWS_Dem_00818]*/
		if (FDCInfo[pEventCfg->DemDTCRef].FDC < 0)/*SWS_Dem_01084]*/
		{
			DemInternalData.CurrentFDC = (uint8)(0xFF + FDCInfo[pEventCfg->DemDTCRef].FDC +1);
		}
		else
		{
			DemInternalData.CurrentFDC = (uint8)FDCInfo[pEventCfg->DemDTCRef].FDC;
		}
	}
    DemInternalData.CyclesSinceLastFailed = pEvent->CyclesSinceLastFailed;/*SWS_Dem_00820]*/
    DemInternalData.CyclesSinceFirstFailed = pEvent->CyclesSinceFirstFailed;/*SWS_Dem_00821]*/
    DemInternalData.FailedCycles = pEvent->FailureCounter;/*SWS_Dem_00822]*/
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_EventQueueAddDebouncePassed
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventQueueAddDebouncePassed(
		P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent)
{
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
    if (0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
    {
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
    }
#endif
    DEM_FLAGS_CLR(pEvent->UdsStatus,
    DEM_UDS_STATUS_TF|DEM_UDS_STATUS_TNCTOC);/* bit 0 6*/
    DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_ACTIVE);
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_EventQueueAddDebounceFailed
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pEvent
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_EventQueueAddDebounceFailed(
		P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent)
{
    uint8 iloop = 0;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pMemDest;
    if (0x00u == DEM_FLAGS_ISSET(pEvent->UdsStatus, DEM_UDS_STATUS_TF))
    {
        pMemDest = DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef].
                                                 DemDTCAttributesRef].DemMemoryDestinationRef;
        while (iloop < DEM_MEM_DEST_MAX_NUM_OF_DTC)
        {
            if (DemMemDestCfg[*pMemDest].ExtId == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
            {
                Dem_SetOccurrenceEvent(pEvent->IntId, DEM_UDS_STATUS_TF);/*SWS_Dem_00219 */
            }
            iloop++;
            pMemDest++;
        }
        DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_TFBITCHANGE);
    }
    /* [SWS_Dem_00036]*/
    DEM_FLAGS_SET(pEvent->UdsStatus,
    DEM_UDS_STATUS_TF|DEM_UDS_STATUS_TFTOC|DEM_UDS_STATUS_TFSLC);/* bit 0 1 5*/
    DEM_FLAGS_CLR(pEvent->UdsStatus,
    DEM_UDS_STATUS_TNCTOC|DEM_UDS_STATUS_TNCSLC);				 /*bit4 6 */
    DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_ACTIVE);

#if(DEM_OCCURRENCE_COUNTER_PROCESSING == DEM_PROCESS_OCCCTR_TF)
    /*SWS_Dem_00524 SWS_Dem_00625 */
    if ((pEvent->OccurrenceCounter < 0xFFu)
        &&((DEM_FLAGS_ISSET(pEvent->UdsStatus,DEM_UDS_STATUS_TF) != 0u)
            &&(DEM_FLAGS_ISSET(pEvent->Status,DEM_EVENT_STATUS_TFBITCHANGE) != 0u)))
    {
		pEvent->OccurrenceCounter++;
    }
#endif

    if((0x00u != DEM_FLAGS_ISSET(pEvent->UdsStatus,DEM_UDS_STATUS_TFTOC))
    		&&(pEvent->FailureCounterAlreadyIncremented == 0))	/* according to 14229,not AutoSAR  */
    {
        /* Test Failed This Operation Cycle */
        pEvent->CyclesSinceLastFailed = 0u;
        pEvent->FailureCounterAlreadyIncremented = 1;		/* Set Failure Cycle Counter increment flag */

        if (pEvent->FailureCounter < 0xFFu)
        {
            pEvent->FailureCounter++;/*SWS_Dem_00782*/
        }
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/*************************************************************************/
/*
 * Brief               Dem_Clear
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      memDest && pEntry && pEvent
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_Clear(
        P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent,
		uint8 memDest
)
{
    boolean allowed = FALSE;
    boolean clear = FALSE;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;

	/*SWS_Dem_00514] [SWS_Dem_00515]*/
	/* req SWS_Dem_00680 Monitor re-initialization  */
	if(DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemCallbackInitMForE != NULL_PTR)
	{
		DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemCallbackInitMForE(DEM_INIT_MONITOR_CLEAR);
	}

	if ((DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemCallbackClearEventAllowed != NULL_PTR)
		 && (DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemCallbackClearEventAllowed->ClearEventAllowed != NULL_PTR))
	{
		if (E_OK == DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemCallbackClearEventAllowed->ClearEventAllowed(&allowed))
		{
			if (allowed == TRUE)
			{
				/*SWS_Dem_00385] */
				pEvent->UdsStatus = 0x00;
				DEM_FLAGS_SET(pEvent->UdsStatus,DEM_UDS_STATUS_TNCSLC|DEM_UDS_STATUS_TNCTOC);/* bit 4 6 the initialized value 0x50 */

			}
			else
			{
				clear = TRUE;/*SWS_Dem_00667]*/
				if (DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemCallbackClearEventAllowed->DemClearEventAllowedBehavior
						== DEM_ONLY_THIS_CYCLE_AND_READINESS)
				{
					/*SWS_Dem_00669]*/
					DEM_FLAGS_SET(pEvent->UdsStatus,DEM_UDS_STATUS_TNCSLC|DEM_UDS_STATUS_TNCTOC);/* bit 4 6 reset */
					DEM_FLAGS_CLR(pEvent->UdsStatus,DEM_UDS_STATUS_TFTOC|DEM_UDS_STATUS_TFSLC);/* bit 1 5 reset */
				}/*SWS_Dem_00668]*/
			}
		}
		/*SWS_Dem_00516] */
		else
		{
			/*SWS_Dem_00385] */
			pEvent->UdsStatus = 0x00;
			DEM_FLAGS_SET(pEvent->UdsStatus,DEM_UDS_STATUS_TNCSLC|DEM_UDS_STATUS_TNCTOC);/* bit 4 6 the initialized value 0x50 */
		}
	}
	else
	{
		/*SWS_Dem_00385] */
		pEvent->UdsStatus = 0x00;/* All other bits are set to 0 */
		DEM_FLAGS_SET(pEvent->UdsStatus,DEM_UDS_STATUS_TNCSLC|DEM_UDS_STATUS_TNCTOC);/* bit 4 6 set to 1 */
       
	}

    Dem_UpdateCombinedDtcStatus(pEvent->IntId);/* update the combination event dtc status*/
    Dem_ResponseOnDtcStatusChange(pEvent->IntId);
    if (clear == FALSE)
    {
        Dem_DebounceReset(pEvent->IntId);/*SWS_Dem_00343]*/
        if (DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef != DEM_DTC_REF_INVALID)
        {
        	FDCInfo[DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef].MaxFDCSinceLastClear = 0;/*SWS_Dem_00794]*/
        }
#if (DEM_DTR_NUM > 0)
        Dem_ClearDTRInfoByEventID(pEvent->IntId);/*SWS_Dem_00763]*/
#endif
        /*SWS_Dem_00667]*/
		pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(pEvent->IntId), memDest);
		if (pEntry != NULL_PTR)
		{
			Dem_MemEntryDelete(pEntry,memDest);/*SWS_Dem_00660] */
		}

    }
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Dem_ResetDemClearDTCInfo
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      Id
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_ResetDemClearDTCInfo(void)
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr = &DemClearDTCInfo;
    pClr->DTCIndex = DEM_DTC_REF_INVALID;
    pClr->DTCGroupIndex = DEM_GROUP_OF_DTC_INVALID;
    pClr->ClearAllGroup = FALSE;
    DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
