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
**  FILENAME    : Dem_Dcm.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : API definitions of DEM for DCM                              **
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
#define DEM_DCM_C_AR_MAJOR_VERSION   4U
#define DEM_DCM_C_AR_MINOR_VERSION   2U
#define DEM_DCM_C_AR_PATCH_VERSION   2U
#define DEM_DCM_C_SW_MAJOR_VERSION   2U
#define DEM_DCM_C_SW_MINOR_VERSION   0U
#define DEM_DCM_C_SW_PATCH_VERSION   1U

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dem_Internal.h"
#include "Dem_Dcm.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (DEM_DCM_C_AR_MAJOR_VERSION != DEM_INTERNAL_H_AR_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_MINOR_VERSION != DEM_INTERNAL_H_AR_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_PATCH_VERSION != DEM_INTERNAL_H_AR_PATCH_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MAJOR_VERSION != DEM_INTERNAL_H_SW_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MINOR_VERSION != DEM_INTERNAL_H_SW_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif

#if (DEM_DCM_C_AR_MAJOR_VERSION != DEM_DCM_H_AR_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_MINOR_VERSION != DEM_DCM_H_AR_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_AR_PATCH_VERSION != DEM_DCM_H_AR_PATCH_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MAJOR_VERSION != DEM_DCM_H_SW_MAJOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
#if (DEM_DCM_C_SW_MINOR_VERSION != DEM_DCM_H_SW_MINOR_VERSION)
  #error "Dem_Dcm.c : Mismatch in Specification Major Version"
#endif
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/* FreezeFrameRecordFilter */
typedef struct
{
    /* Filter */
    Dem_DTCFormatType DTCFormat;
    /* Result */
    uint16 NumberOfFilteredRecords;
    uint16 GetNum;
    uint8 EntryIndex;
    uint8 FFIndex;
} Dem_FreezeFrameRecordFilterInfoType;

typedef struct
{
    /* All Group */
    boolean AllGroupIsEnabled;
    Dem_DTCKindType DTCKind;
#if(DEM_GROUP_OF_DTC_NUM > 0)
    uint8 DTCGroupStatus[DEM_GROUP_OF_DTC_NUM_BYTE];
#endif
} Dem_DTCSettingInfoType;

typedef struct
{
    uint16 DTCIndex;
    Dem_DTCOriginType DTCOrigin;
}Dem_DTCRecordUpdateStatusInfoType;

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DTCFilterInfoType,AUTOMATIC) DemDTCFilterInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_DTCSettingInfoType,AUTOMATIC) DemDTCSettingInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_ReturnClearDTCType,AUTOMATIC) DemClearDTCStatus;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
VAR(Dem_DTCByOccurrenceTimeType,AUTOMATIC) DemDTCByOccurrenceTimeInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_DTCRecordUpdateStatusInfoType,AUTOMATIC) DTCRecordUpdateStatusInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"
static VAR(Dem_FreezeFrameRecordFilterInfoType,AUTOMATIC) DemFreezeFrameRecordFilterInfo;
#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"
VAR(boolean,AUTOMATIC) DemDTCStatusChangedInd = FALSE;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"
static VAR(boolean,AUTOMATIC) DemDTCDisable = FALSE;
#define DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_SetDTCSettingProcess(void);

#if(DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
static FUNC(P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_GetExtendedDataRecordCfg(
	uint8 ExtendedDataNumber
);
#endif
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0u)
static FUNC(Dem_ReturnGetExtendedDataRecordByDTCType,DEM_CODE)
Dem_GetExtendedRecordDataLoop(
	uint8 memDest,
	uint32 DTC,
	uint8 ExtendedDataNumber,
	P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
);
#endif
static FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmCheckDTCFilterStatus(
	P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
	P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus
);
static FUNC(void,DEM_CODE)
Dem_DcmGetFreezeFrameDataByRecordNumber(
    uint8 RecordNumber,
    uint16 dtcIndex,
    boolean* Find
);
static FUNC(Dem_ReturnGetFreezeFrameDataByDTCType,DEM_CODE)
Dem_DcmGetFreezeFrameDataCheckFlag(
	boolean Find,
	boolean getData
);
static FUNC(Dem_ReturnGetFreezeFrameDataByDTCType,DEM_CODE)
Dem_DcmGetFreezeFrameDataCheckParameter(
	uint32 DTC,
	Dem_DTCOriginType DTCOrigin,
	uint8 RecordNumber,
	P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
);
static FUNC(Dem_ReturnGetSizeOfDataByDTCType,Dem_CODE)
Dem_DcmGetSizeOfExtendedDataRecordCheckParameter(
	uint32 DTC,
	Dem_DTCOriginType DTCOrigin,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord
);
static FUNC(void,DEM_CODE)
Dem_DcmGetSizeOfExtendedDataRecordLoop(
	uint16 dtcRef,
	uint8 memDest,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord,
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg
);
static FUNC(void,DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTCProcess(
	P2VAR(uint8, AUTOMATIC, DEM_VAR) pFFData,
	P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
	P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttr
);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

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
FUNC(void, DEM_CODE) Dem_DcmInit(void)
{
    DemDTCStatusChangedInd = TRUE;
    DemDTCSettingInfo.AllGroupIsEnabled = TRUE;
#if(DEM_GROUP_OF_DTC_NUM > 0)
    Dem_MemSet((uint8*)&DemDTCSettingInfo.DTCGroupStatus, 0xFFu,
        sizeof(Dem_DTCSettingInfoType));
#endif
    Dem_MemSet((uint8*)&DemDTCFilterInfo, 0x00u, sizeof(Dem_DTCFilterInfoType));
    Dem_MemSet((uint8*)&DemFreezeFrameRecordFilterInfo, 0x00u,
        sizeof(Dem_FreezeFrameRecordFilterInfoType));

    DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed = DEM_EVENT_PARAMETER_INVALID;
    DemDTCByOccurrenceTimeInfo.FirstFailed = DEM_EVENT_PARAMETER_INVALID;
    DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed =
    DEM_EVENT_PARAMETER_INVALID;
    DemDTCByOccurrenceTimeInfo.MostRecentFailed = DEM_EVENT_PARAMETER_INVALID;

    DemClearDTCInfo.ClearAllGroup = FALSE;
    DemClearDTCInfo.DTCGroupIndex = DEM_GROUP_OF_DTC_INVALID;
    DemClearDTCInfo.DTCIndex = DEM_DTC_REF_INVALID;
    DemClearDTCInfo.memDest = DEM_MEM_DEST_INVALID;
    DemClearDTCStatus = DEM_CLEAR_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the supported DTC formats of the ECU.
 * ServiceId           0x3c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the configured DTC translation format. Acombination of
 *                              different DTC formats is not possible.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_DTCTranslationFormatType, DEM_CODE)
Dem_DcmGetTranslationType(void)
{
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETTRANSLATIONTYPE, DEM_E_UNINIT);
    }
    return DEM_TYPE_OF_DTCSUPPORTED;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the DTC Status availability mask.
 * ServiceId           0x16
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTCStatusMask:The value DTCStatusMask indicates the supported DTC status
 *                              bits from the Dem.
 * Param-Name[in/out]  none
 * Return              E_OK: get of DTC status mask was successful
 *                       E_NOT_OK: get of DTC status mask failed
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE)
Dem_DcmGetDTCStatusAvailabilityMask(
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask
)
{
	boolean detErr = FALSE;
	Std_ReturnType retVal = E_NOT_OK;

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETDTCSTATUSAVAILABILITYMASK, DEM_E_UNINIT);
    	detErr = TRUE;
    	retVal = E_NOT_OK;
    }
    if ((detErr == FALSE)&&(DTCStatusMask == NULL_PTR))
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETDTCSTATUSAVAILABILITYMASK, DEM_E_PARAM_POINTER);
    	detErr = TRUE;
    	retVal = E_NOT_OK;
    }

    if(detErr == FALSE)
    {
        *DTCStatusMask = DEM_DTC_STATUS_AVAILABILITY_MASK;
        retVal = E_OK;
    }

    return retVal;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the status of a DTC.
 * ServiceId           0x15
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin: If the Dem supports more than one event memory this
 *                              parameter is used to select the source memory
 *                              the DTCs shall be read from.
 * Param-Name[out]     DTCStatus: This parameter receives the status information of the
 *                          requested DTC. If the return value of the function call is
 *                          other than DEM_STATUS_OK this parameter does not contain valid
 *                          data. 0x00...0xFF match DTCStatusMask as defined in ISO14229-1
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetStatusOfDTCType.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetStatusOfDTCType, DEM_CODE) Dem_DcmGetStatusOfDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCStatus
)
{
    uint8 memDest;
    uint16 dtcRef;
    uint16 dtcIndex;
    Dem_EventIdType EventId = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETSTATUSOFDTC, DEM_E_UNINIT);
        return DEM_STATUS_FAILED;
    }
    if (DTCStatus == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETSTATUSOFDTC, DEM_E_PARAM_POINTER);
        return DEM_STATUS_FAILED;
    }
    /*SWS_Dem_01100*/
    dtcIndex = Dem_GetDTCIndex(DTC,DEM_DTC_FORMAT_UDS);
    if (dtcIndex == DEM_DTC_REF_INVALID)
    {
        return DEM_STATUS_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == TRUE)
    {
        return  DEM_STATUS_WRONG_DTC;
    }
    memDest = Dem_GetInternalMemDest(DTCOrigin);
    if (memDest == DEM_MEM_DEST_INVALID)
    {
        return DEM_STATUS_WRONG_DTCORIGIN;
    }
    while (EventId < DEM_EVENT_PARAMETER_NUM)
    {
        dtcRef = DemPbCfgPtr->DemEventParameter[EventId].DemDTCRef;
        if ((dtcRef != DEM_DTC_REF_INVALID)
            && (DemPbCfgPtr->DemDTC[dtcRef].DemDtcValue == DTC)
            )
        {
            pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(EventId), memDest);
            if (pEntry != NULL_PTR)
            {
            	*DTCStatus = (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
            }
            else
            {	/* return status of DTC which not been stored */
            	*DTCStatus = (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
            }
        	return DEM_STATUS_OK;
        }
        EventId++;
    }
    return DEM_STATUS_FAILED;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the severity of the requested DTC.
                        For large configurations and DTC-calibration, the interface behavior can
                        be asynchronous (splitting the DTC-search into segments).
 * ServiceId           0x0e
 * Sync/Async          Synchronous or Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format.
 * Param-Name[out]     DTCSeverity:This parameter contains the DTCSeverity according to ISO 14229-1.
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetSeverityOfDTCType.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetSeverityOfDTCType, DEM_CODE) Dem_DcmGetSeverityOfDTC(
    uint32 DTC,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity
)
{
    uint16 dtcIndex;
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETSEVERITYOFDTC, DEM_E_UNINIT);
        return DEM_GET_SEVERITYOFDTC_WRONG_DTC;
    }
    if (DTCSeverity == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETSEVERITYOFDTC, DEM_E_PARAM_POINTER);
        return DEM_GET_SEVERITYOFDTC_WRONG_DTC;
    }

    /*SWS_Dem_01100*/
    dtcIndex = Dem_GetDTCIndex(DTC,DEM_DTC_FORMAT_UDS);
    if (dtcIndex == DEM_DTC_REF_INVALID)
    {
        return DEM_GET_SEVERITYOFDTC_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == TRUE)
    {
        return  DEM_GET_SEVERITYOFDTC_WRONG_DTC;
    }
    *DTCSeverity = DemPbCfgPtr->DemDTC[dtcIndex].DemDTCSeverity;

    return DEM_GET_SEVERITYOFDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the functional unit of the requested DTC.
 * ServiceId           0x34
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 * Param-Name[out]     DTCFunctionalUnit:Functional unit value of this DTC
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetFunctionalUnitOfDTCType.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetFunctionalUnitOfDTCType, DEM_CODE) Dem_DcmGetFunctionalUnitOfDTC(
    uint32 DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
)
{
    uint16 dtcIndex;
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETFUNCTIONALUNITOFDTC, DEM_E_UNINIT);
        return DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC;
    }
    if (DTCFunctionalUnit == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETFUNCTIONALUNITOFDTC, DEM_E_PARAM_POINTER);
        return DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC;
    }
    /*SWS_Dem_01100*/
    dtcIndex = Dem_GetDTCIndex(DTC,DEM_DTC_FORMAT_UDS);
    if (dtcIndex == DEM_DTC_REF_INVALID)
    {
        return DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == TRUE)
    {
        return  DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC;
    }
    /*SWS_Dem_00593*/
    *DTCFunctionalUnit = DemPbCfgPtr->DemDTC[dtcIndex].DemDTCFunctionalUnit;
    return DEM_GET_FUNCTIONALUNITOFDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Sets the DTC Filter.
 * ServiceId           0x13
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCStatusMask:Status-byte mask for DTC status-byte filtering Values
 *                       DTCKind: Defines the functional group of DTCs to be reported
 *                       DTCFormat: Defines the output-format of the requested DTC values for the
 *                              sub-sequent API calls.
 *                       DTCOrigin: If the Dem supports more than one event memory this parameter
 *                              is used to select the source memory the DTCs shall be read from.
 *                       FilterWithSeverity: This flag defines whether severity information (ref.
 *                              to parameter below) shall be used for filtering
 *                       DTCSeverityMask: This parameter contains the DTCSeverityMask according
 *                              to ISO14229-1
 *                       FilterForFaultDetectionCounter: This flag defines whether the fault
 *                              detection counter information shall be used for filtering.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a DTC filter.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_DcmSetDTCFilter(
    Dem_UdsStatusByteType DTCStatusMask,
    Dem_DTCKindType DTCKind,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin,
    boolean FilterWithSeverity,
    Dem_DTCSeverityType DTCSeverityMask,
    boolean FilterForFaultDetectionCounter
    )
{
	DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMSETDTCFILTER, DEM_E_UNINIT);
        return DEM_WRONG_FILTER;
    }
    if (((DEM_DTC_FORMAT_OBD == DTCFormat) && (DEM_OBD_SUPPORT == DEM_OBD_NO_OBD_SUPPORT))
            || ((DEM_DTC_FORMAT_J1939 == DTCFormat) && (DEM_J1939_SUPPORT == STD_OFF))
            || ((DEM_DTC_REF_EMISSION_NUM == 0u) && (DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS))
            || (DTCOrigin == DEM_MEM_DEST_INVALID))
    {
    	DEM_DET_REPORT(DEM_SID_DCMSETDTCFILTER, DEM_E_WRONG_CONFIGURATION);
        return DEM_WRONG_FILTER;
    }

#if(DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    /*SWS_Dem_00410*/
    if (((DTCStatusMask & 0xCu) != 0u) && ((DTCStatusMask & 0xF3u) == 0x00u))
    {
        DemDTCFilterInfo.TimeOrder = TRUE;
        DemDTCFilterInfo.LastTime = DemMemDestCfg[DTCOrigin].EntryNum + 1u;
    }
    else
    {
    	DemDTCFilterInfo.TimeOrder = FALSE;
    }
#endif
    DemDTCFilterInfo.DTCStatusMask = (DTCStatusMask & DEM_DTC_STATUS_AVAILABILITY_MASK);/* SWS_Dem_01058 */
    DemDTCFilterInfo.PassedDTCStatusMask = DTCStatusMask;
    DemDTCFilterInfo.DTCKind = DTCKind;
    DemDTCFilterInfo.DTCFormat = DTCFormat;
    DemDTCFilterInfo.DTCOrigin = DTCOrigin;
    DemDTCFilterInfo.FilterWithSeverity = FilterWithSeverity;
    DemDTCFilterInfo.DTCSeverityMask = DTCSeverityMask;
    DemDTCFilterInfo.FilterForFaultDetectionCounter = FilterForFaultDetectionCounter;
    DemDTCFilterInfo.CurrentIndex = 0;
    DemDTCFilterInfo.NumberOfFilteredDTC = 0;
    DemDTCFilterInfo.IsSet = TRUE;

    return DEM_FILTER_ACCEPTED;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the number of a filtered DTC.
 *                      (((statusOfDTC & DTCStatusMask) != 0) && ((severity & DTCSeverityMask) != 0)) == TRUE
 * ServiceId           0x17
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     NumberOfFilteredDTC:The number of DTCs matching the defined status mask.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a number of DTC from the Dem
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE)
Dem_DcmGetNumberOfFilteredDTC(
P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC
)
{
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 iloop = 0;
    uint8 Index;
    uint32 DTC;
    uint8 entryNum;
    uint16 dtcRef;
    sint8 FDC = 0;
    boolean findSameDTC = FALSE;
    uint32 tempDtcValue[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNUMBEROFFILTEREDDTC, DEM_E_UNINIT);
        return DEM_NUMBER_FAILED;
    }
    if (NumberOfFilteredDTC == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNUMBEROFFILTEREDDTC, DEM_E_PARAM_POINTER);
        return DEM_NUMBER_FAILED;
    }
    if(DemDTCFilterInfo.IsSet == FALSE)
    {
        return DEM_NUMBER_FAILED;
    }

    entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;

    /* used for UDS service 0x19 sub-functions 0x0A/0x15 */
    if ((DemDTCFilterInfo.DTCStatusMask == 0x00u)
    		&& (DemDTCFilterInfo.PassedDTCStatusMask == 0x00u)
    		&& (pFilter->DTCFormat == DEM_DTC_FORMAT_UDS))
    {
    	while (iloop < DEM_DTC_NUM)
    	{
    		DTC = DemPbCfgPtr->DemDTC[iloop].DemDtcValue;

            if(DEM_DTC_CFG_INVALID != DTC)
            {
        		pFilter->NumberOfFilteredDTC++;
            }
            iloop++;
    	}
    	*NumberOfFilteredDTC = pFilter->NumberOfFilteredDTC;
    	pFilter->CurrentIndex = 0u;
    	return DEM_NUMBER_OK;
    }

    iloop = 0u;

    while (iloop < entryNum)
    {
        if (pEntry->EventId == 0x00u)
        {
            pEvent = Dem_GetEventInfo(Dem_GetEventInternalId(pEntry->EventId));
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;

            if(DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef == DEM_OBD_DTC_INVALID)
            {
                (void)Dem_GetInteralFDC(pEvent->IntId,&FDC);

                if ((0x00u != (pEvent->UdsStatus & pFilter->DTCStatusMask))
                	 && ((pFilter->FilterWithSeverity == FALSE)
                	 || ((pFilter->FilterWithSeverity == TRUE)
                	 && (0x00u != (pFilter->DTCSeverityMask & DemPbCfgPtr->DemDTC[dtcRef].DemDTCSeverity))))
                     && ((pFilter->FilterForFaultDetectionCounter == FALSE)
                     || ((pFilter->FilterForFaultDetectionCounter == TRUE)
                        && (0 < FDC)))
                        && (DemPbCfgPtr->DemDTC[dtcRef].DemDtcValue != DEM_DTC_CFG_INVALID)
                )
                {
                    tempDtcValue[iloop] = DemPbCfgPtr->DemDTC[dtcRef].DemDtcValue;
                    Index = 0;
                    while (Index < iloop)
                    {
                        if(tempDtcValue[Index] == DemPbCfgPtr->DemDTC[dtcRef].DemDtcValue)
                        {
                        	findSameDTC = TRUE;
                        }
                        Index++;
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
            }
        }
        iloop++;
        pEntry++;
    }
    DemDTCFilterInfo.IsSet = FALSE;
    *NumberOfFilteredDTC = pFilter->NumberOfFilteredDTC;
    return DEM_NUMBER_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC matching the filter criteria.
 * ServiceId           0x18
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC : Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCStatus: This parameter receives the status information of
 *                          the requested DTC.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTC(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus
)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 entryNum;
    uint16 dtcRef;
#if(DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    uint8 iloop;
    uint16 obdDtcRef;
    uint8 dtcFormat = 0xff;
    uint8 Dem_time = 0;
    boolean Find = FALSE;
    uint8 index;
#endif
    Dem_EventIdType eventId;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;

    res = Dem_DcmCheckDTCFilterStatus(DTC,DTCStatus);

    if(res != DEM_FILTERED_OK)
    {
    	return res;
    }

    res = DEM_FILTERED_NO_MATCHING_ELEMENT;

    entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;

    if ((DemDTCFilterInfo.DTCStatusMask == 0x00u) && (DemDTCFilterInfo.PassedDTCStatusMask == 0x00u)
    		&& (pFilter->DTCFormat == DEM_DTC_FORMAT_UDS))
    {
    	while (pFilter->CurrentIndex < DEM_DTC_NUM)
    	{
    		*DTC = DemPbCfgPtr->DemDTC[pFilter->CurrentIndex].DemDtcValue;
            *DTCStatus = (DemDTCGeneralStatus[pFilter->CurrentIndex].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);

            if(DEM_DTC_CFG_INVALID != *DTC)
            {
        		pFilter->CurrentIndex++;
            }

        	return DEM_FILTERED_OK;
    	}
    }

#if(DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW == STD_ON)
    /*SWS_Dem_00410,SWS_Dem_00411
     *the most recent event memory entry shall be reported at first.*/
    if (DemDTCFilterInfo.TimeOrder == TRUE)
    {
    	for (index = 0; (index < entryNum) && (res != DEM_FILTERED_OK); index++)
    	{
			iloop = 0;
			while (iloop < entryNum)
			{
				dtcRef = DemPbCfgPtr->DemEventParameter[pEntry[iloop].EventId-1].DemDTCRef;
				obdDtcRef = DemPbCfgPtr->DemDTC[dtcRef].DemObdDTCRef;

				if((dtcRef != DEM_DTC_REF_INVALID)
						&&(DemPbCfgPtr->DemDTC[dtcRef].DemDtcValue != DEM_DTC_CFG_INVALID))
				{
					dtcFormat = DEM_DTC_FORMAT_UDS;
				}
				else if((dtcRef != DEM_DTC_REF_INVALID)&&(obdDtcRef != DEM_OBD_DTC_INVALID)
						&&(DemPbCfgPtr->DemObdDTC[obdDtcRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
				{
					dtcFormat = DEM_DTC_FORMAT_OBD;
				}
				else if((dtcRef != DEM_DTC_REF_INVALID)&&(obdDtcRef != DEM_OBD_DTC_INVALID)
						&&(DemPbCfgPtr->DemObdDTC[obdDtcRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
				{
					dtcFormat = DEM_DTC_FORMAT_J1939;
				}
				else
				{
					dtcFormat = 0xff;
				}

				if(dtcFormat == pFilter->DTCFormat)
				{
					if ((Dem_time < pEntry[iloop].AbsTime) && (pEntry[iloop].AbsTime < pFilter->LastTime))
					{
						pFilter->CurrentIndex = iloop;
						Dem_time = pEntry[iloop].AbsTime;
						Find = TRUE;
					}
				}
				iloop++;
			}
			if (Find == TRUE)
			{
				pFilter->LastTime = Dem_time;
				eventId = Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId);
				pEvent = Dem_GetEventInfo(eventId);
				dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
				/*SWS_Dem_01101*/
				if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
				{
					res = DEM_FILTERED_NO_MATCHING_ELEMENT;
				}
				else
				{
					Dem_GetEventDTC(pEvent->IntId, pFilter->DTCFormat, DTC);
					*DTCStatus = (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
					if((0u != (*DTCStatus & pFilter->DTCStatusMask)) && (0u != *DTC))
					{
						res = DEM_FILTERED_OK;
					}
				}
			}
    	}
    }
    else
#endif
    {
        while ((res != DEM_FILTERED_OK) && (pFilter->CurrentIndex < entryNum))
        {
            if (pEntry[pFilter->CurrentIndex].EventId != 0x00u)
            {
                eventId = Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId);
                pEvent = Dem_GetEventInfo(eventId);
                dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
                /*SWS_Dem_01101*/
                if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
                {
                    res = DEM_FILTERED_NO_MATCHING_ELEMENT;
                }
                else
                {
                    /*guarantee the event index the new dtc */
                    Dem_GetEventDTC(pEvent->IntId, pFilter->DTCFormat, DTC);
                    *DTCStatus = (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
                    if((0u != (*DTCStatus & pFilter->DTCStatusMask)) && (0u != *DTC))
                    {
                    	res = DEM_FILTERED_OK;
                    }
                }
            }
            pFilter->CurrentIndex++;
        }
    }
    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC and its associated Fault Detection Counter
 *                      (FDC) matching the filter criteria.
 * ServiceId           0x3b
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC: Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       DTCFaultDetectionCounter:This parameter receives the Fault Detection
 *                          Counter information of the requested DTC.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndFDC(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA) DTCFaultDetectionCounter
)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 entryNum;
    uint16 dtcRef;
    Dem_EventIdType eventId;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((DTCFaultDetectionCounter == NULL_PTR) || (DTC == NULL_PTR))
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDFDC, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }

    entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while ((res != DEM_FILTERED_OK)
    && (pFilter->CurrentIndex < entryNum))
    {
        if (pEntry[pFilter->CurrentIndex].EventId != 0x00u)
        {
            eventId = Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId);
            pEvent = Dem_GetEventInfo(eventId);
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            /*SWS_Dem_01101*/
            if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
            {
                res = DEM_FILTERED_NO_MATCHING_ELEMENT;
            }
            else
            {
                /*guarantee the event index the new dtc */
                Dem_GetEventDTC(pEvent->IntId, pFilter->DTCFormat, DTC);
                *DTCFaultDetectionCounter = FDCInfo[dtcRef].FDC;/*SWS_Dem_00788 SWS_Dem_00789 SWS_Dem_00792 SWS_Dem_00793*/
                if(0u != ((DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK) & pFilter->DTCStatusMask))
                {
                	res = DEM_FILTERED_OK;
                }
            }
        }
        pFilter->CurrentIndex++;
    }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the next filtered DTC and its associated Severity matching the filter
                        criteria. The interface has an asynchronous behavior, because a large
                        number of DTCs has to be processed.
 * ServiceId           0x3d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC:Receives the DTC value in respective format of the
                            filter returned by this function. If the return value of
                            the function is other than DEM_FILTERED_OK this
                            parameter does not contain valid data.
 *                       DTCStatus:This parameter receives the status information of
                            the requested DTC.
                            It follows the format as defined in ISO14229-1
                            If the return value of the function call is other than
                            DEM_FILTERED_OK this parameter does not contain valid data.
 *                       DTCSeverity: Receives the severity value returned by the function.
 *                          If the return value of the function is other than
                            DEM_FILTERED_OK this parameter does not contain valid data
 *                       DTCFunctionalUnit: Receives the functional unit value returned by the
                            function. If the return value of the function is other
                            than DEM_FILTERED_OK this parameter does not
                            contain valid data.
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredDTCAndSeverity(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus,
P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit
)
{
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;
    uint8 entryNum;
    uint16 dtcRef;
    Dem_EventIdType eventId;

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if ((DTC == NULL_PTR) || (DTCStatus == NULL_PTR) || (DTCSeverity == NULL_PTR) || (DTCFunctionalUnit == NULL_PTR))
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTCANDSEVERITY, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }

    entryNum = DemMemDestCfg[pFilter->DTCOrigin].EntryNum;
    pEntry = DemMemDestCfg[pFilter->DTCOrigin].EntryList;
    while ((res != DEM_FILTERED_OK)
    && (pFilter->CurrentIndex < entryNum))
    {
        if (pEntry[pFilter->CurrentIndex].EventId != 0x00u)
        {
            eventId = Dem_GetEventInternalId(pEntry[pFilter->CurrentIndex].EventId);
            pEvent = Dem_GetEventInfo(eventId);
            dtcRef = DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef;
            if(dtcRef != DEM_DTC_REF_INVALID)
            {
            	/*SWS_Dem_01101*/
            	if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
            	{
					res = DEM_FILTERED_NO_MATCHING_ELEMENT;
				}
				else
				{
					/*guarantee the event index the new dtc */
					Dem_GetEventDTC(pEvent->IntId, pFilter->DTCFormat, DTC);
					*DTCStatus = (DemDTCGeneralStatus[dtcRef].CbUdsStatus & DEM_DTC_STATUS_AVAILABILITY_MASK);
					*DTCSeverity = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef].DemDTCSeverity;
					*DTCFunctionalUnit = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[pEvent->IntId].DemDTCRef].DemDTCFunctionalUnit;
                    if((0u != (*DTCStatus & pFilter->DTCStatusMask)) && (0u != *DTC))
					{
						res = DEM_FILTERED_OK;
					}
				}
            }
        }
        pFilter->CurrentIndex++;
    }

    return res;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Sets a freeze frame record filter.
 * ServiceId           0x3f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTCFormat: Defines the output-format of the requested DTC values for the
 *                           sub-sequent API calls.
 * Param-Name[out]     NumberOfFilteredRecords:Number of freeze frame records currently stored
 *                           in the event memory.
 * Param-Name[in/out]  none
 * Return              Status of the operation to (re-)set a freeze frame record filter.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnSetFilterType, DEM_CODE)
Dem_DcmSetFreezeFrameRecordFilter(
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    uint8 entryNum = 0;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameRecordFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemFreezeFrameRecordFilterInfo;
    Dem_DTCOriginType DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMSETFREEZEFRAMERECORDFILTER, DEM_E_UNINIT);
        return DEM_WRONG_FILTER;
    }
    if (DEM_DTC_FORMAT_UDS != DTCFormat)
    {
    	DEM_DET_REPORT(DEM_SID_DCMSETFREEZEFRAMERECORDFILTER, DEM_E_WRONG_CONFIGURATION);
        return DEM_WRONG_FILTER;
    }
    if (NumberOfFilteredRecords == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMSETFREEZEFRAMERECORDFILTER, DEM_E_PARAM_POINTER);
        return DEM_WRONG_FILTER;
    }
    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    pFilter->DTCFormat = DTCFormat;
    *NumberOfFilteredRecords = 0;
    pEntry = DemMemDestCfg[DTCOrigin].EntryList;/*SWS_Dem_00210*/
    while (entryNum < DemMemDestCfg[DTCOrigin].EntryNum)
    {
        if (pEntry->EventId != 0x00u)
        {
            *NumberOfFilteredRecords += pEntry->FFNum;
        }
        pEntry++;
        entryNum++;
    }
    pFilter->GetNum = 0;
    pFilter->EntryIndex = 0;
    pFilter->FFIndex = 0;
    pFilter->NumberOfFilteredRecords = *NumberOfFilteredRecords;
    return DEM_FILTER_ACCEPTED;
#else
    return DEM_WRONG_FILTER;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the next freeze frame record number and its associated DTC
 *                          stored in the event memory.
 * ServiceId           0x3a
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     DTC:Receives the DTC value in respective format of the filter
 *                          returned by this function.
 *                       RecordNumber:Freeze frame record number of the reported DTC
 *                          (relative addressing).
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve a DTC and its associated
 *                          snapshot record number from the Dem.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmGetNextFilteredRecord(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) RecordNumber
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)
    Dem_ReturnGetNextFilteredElementType res = DEM_FILTERED_NO_MATCHING_ELEMENT;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameRecordFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemFreezeFrameRecordFilterInfo;
    Dem_DTCOriginType DTCOrigin;
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDRECORD, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if (DTC == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDRECORD, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    if (RecordNumber == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDRECORD, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }
    DTCOrigin = Dem_GetInternalMemDest(DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    while ((res != DEM_FILTERED_OK)
    && (pFilter->GetNum < pFilter->NumberOfFilteredRecords))
    {
        if (pFilter->EntryIndex
        >= DemMemDestCfg[DTCOrigin].EntryNum)/*SWS_Dem_00210*/
        {
            pFilter->EntryIndex = 0;
            pFilter->FFIndex = 0;
        }
        else
        {
            pEntry = &DemMemDestCfg[DTCOrigin].EntryList[pFilter
            ->EntryIndex];
            if (pFilter->FFIndex >= DEM_MAX_NUMBER_FF_RECORDS)
            {
                pFilter->FFIndex = 0;
                pFilter->EntryIndex++;
            }
            else
            {
                if (pEntry->FFList[pFilter->FFIndex].RecordNum != 0xFFu)
                {
                    /*SWS_Dem_01101*/
				if ((DemPbCfgPtr->DemEventParameter[pEntry->EventId].DemDTCRef != DEM_DTC_REF_INVALID)
                    &&(DemDTCGeneralStatus[DemPbCfgPtr->DemEventParameter[pEntry->EventId].DemDTCRef].SuppressionStatus == TRUE))
                    {
                        res = DEM_FILTERED_NO_MATCHING_ELEMENT;
                    }
                    else
                    {
                        Dem_GetEventDTC(Dem_GetEventInternalId(pEntry->EventId),pFilter->DTCFormat, DTC);
                        *RecordNumber = pEntry->FFList[pFilter->FFIndex].RecordNum;
                        res = DEM_FILTERED_OK;
                    }
                    pFilter->GetNum++;
                }
                pFilter->FFIndex++;
            }
        }
    }
    return res;
#else
    return DEM_FILTERED_NO_MATCHING_ELEMENT;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the DTC by occurrence time.
 * ServiceId           0x19
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCRequest:This parameter defines the request type of the DTC.
 * Param-Name[out]     DTC: Receives the DTC value in UDS format returned by the function.
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnGetDTCByOccurrenceTimeType.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetDTCByOccurrenceTimeType, DEM_CODE)
Dem_DcmGetDTCByOccurrenceTime(
    Dem_DTCRequestType DTCRequest,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC
)
{
    Dem_EventIdType IntId;

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETDTCBYOCCURRENCETIME, DEM_E_UNINIT);
        return DEM_OCCURR_NOT_AVAILABLE;
    }
    if (DTC == NULL_PTR)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETDTCBYOCCURRENCETIME, DEM_E_PARAM_POINTER);
        return DEM_OCCURR_NOT_AVAILABLE;
    }
    switch (DTCRequest)
    {
        case DEM_FIRST_FAILED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.FirstFailed;
        break;
        case DEM_MOST_RECENT_FAILED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.MostRecentFailed;
        break;
        case DEM_FIRST_DET_CONFIRMED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed;
        break;
        case DEM_MOST_REC_DET_CONFIRMED_DTC:
        IntId = DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed;
        break;
        default:
        	IntId = DEM_EVENT_PARAMETER_INVALID;
        break;
    }
    if ((IntId != DEM_EVENT_PARAMETER_INVALID)
    && (DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef != DEM_DTC_REF_INVALID))
    {
		/*SWS_Dem_01101*/
		if (DemDTCGeneralStatus[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].SuppressionStatus == TRUE)
		{
			return DEM_OCCURR_NOT_AVAILABLE;
		}
		Dem_GetEventDTC(IntId, DEM_DTC_FORMAT_UDS, DTC);
		return DEM_OCCURR_OK;
    }
    return DEM_OCCURR_NOT_AVAILABLE;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Controls the triggering of Dcm_DemTriggerOnDTCStatus.
 * ServiceId           0xb0
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      TriggerNotification:This parameter specifies whether the triggering of
 *                          the notification shall be enabled (TRUE) or disabled (FALSE).
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_DcmControlDTCStatusChangedNotification(
    boolean TriggerNotification
    )
{
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMCONTROLDTCSTATUSCHANGEDNOTIFICATION, DEM_E_UNINIT);
    	return;
    }
	DemDTCStatusChangedInd = TriggerNotification;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**      Access extended data records and FreezeFrame data                     **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Disables the event memory update of a specific DTC
 *                          (only one at one time).
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Selects the DTC in UDS format, for which DTC record update
 *                          shall be disabled.
 *                       DTCOrigin: If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          for which DTC record update shall be disabled.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation to disable the event memory
 *                          update of a specific DTC.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnDisableDTCRecordUpdateType, DEM_CODE)
Dem_DcmDisableDTCRecordUpdate(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin
    )
{
	uint16 dtcIndex;
    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCRECORDUPDATE, DEM_E_UNINIT);
        return DEM_DISABLE_DTCRECUP_WRONG_DTC;
    }
    if (DemDTCDisable == TRUE)
    {
    	DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCRECORDUPDATE, DEM_E_WRONG_CONDITION);
        return E_NOT_OK;
    }

    dtcIndex = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);

    if (dtcIndex == DEM_DTC_REF_INVALID)
    {
        return DEM_DISABLE_DTCRECUP_WRONG_DTC;
    }

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_DISABLE_DTCRECUP_WRONG_DTCORIGIN;
    }

    DemDTCDisable = TRUE;

    DTCRecordUpdateStatusInfo.DTCIndex = dtcIndex;
    DTCRecordUpdateStatusInfo.DTCOrigin = DTCOrigin;
    DEM_BITS_SET(DemMemDestInfo[DTCOrigin].DisableDTCRecordUpdate, dtcIndex);
    return DEM_DISABLE_DTCRECUP_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Enables the event memory update of the DTC disabled by
 *                          Dem_DcmDisableDTCRecordUpdate() before.
 * ServiceId           0x1b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Always E_OK is returned.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmEnableDTCRecordUpdate(void)
{
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMENABLEDTCRECORDUPDATE, DEM_E_UNINIT);
        return E_NOT_OK;
    }
    else
    {
    	DemDTCDisable = FALSE;
    	DEM_BITS_CLR(DemMemDestInfo[DTCRecordUpdateStatusInfo.DTCOrigin].DisableDTCRecordUpdate, DTCRecordUpdateStatusInfo.DTCIndex);
    }
    return E_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_FREEZE_FRAME_CLASS_NUM > 0)

/*************************************************************************/
/*
 * Brief               Gets freeze frame data by DTC. The function stores the data
 *                          in the provided DestBuffer.
 * ServiceId           0x1d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber:This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     DestBuffer:This parameter contains a byte pointer that points
 *                          to the buffer, to which the freeze frame data record
 *                          shall be written to.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve freeze frame data by DTC.
 */
/*************************************************************************/




#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetFreezeFrameDataByDTCType, DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
)
{
#if(DEM_DID_CLASS_NUM > 0u)

    uint8 memDest;
    uint16 dtcIndex;
    uint16 iloop = 0u;
    boolean getData = FALSE;
    boolean Find = FALSE;
    boolean detErr = FALSE;
    Dem_ReturnGetFreezeFrameDataByDTCType retVal;

    P2VAR(uint8, AUTOMATIC, DEM_VAR) pFFData;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) pFF;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttr;


    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETFREEZEFRAMEDATABYDTC, DEM_E_UNINIT);
        retVal = DEM_GET_FFDATABYDTC_WRONG_DTC;
        detErr = TRUE;
    }
    else
    {
        retVal = Dem_DcmGetFreezeFrameDataCheckParameter( DTC, DTCOrigin, RecordNumber,
    													  DestBuffer, BufSize );
    }

    if((detErr == FALSE) &&
    		(retVal == DEM_GET_FFDATABYDTC_OK))
    {
    	dtcIndex = Dem_GetDTCIndex(DTC,DEM_DTC_FORMAT_UDS);
    	memDest = Dem_GetInternalMemDest(DTCOrigin);
    	*BufSize = 0u;

    	while (iloop < DEM_EVENT_PARAMETER_NUM)
    	{
    		if (DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef == dtcIndex)
    		{
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)

    			Dem_DcmGetFreezeFrameDataByRecordNumber(RecordNumber,dtcIndex,&Find);

    			if (Find == TRUE)
    			{
#endif
    				pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(iloop), memDest);
    				if (pEntry != NULL_PTR)
    				{
    					pFF = Dem_FreezeFrameGetByRecordNum(pEntry, RecordNumber);

    					if (pFF != NULL_PTR)
    					{
    						getData = TRUE;
    						pFFData = pFF->Data;
    						pDTCAttr = Dem_EventDTCAttributesCfg(iloop);
    						if (pDTCAttr->DemFreezeFrameClassRef != DEM_FREEZE_FRAME_INVALID)
    						{
    							Dem_DcmGetFreezeFrameDataByDTCProcess( pFFData, DestBuffer,
																  		BufSize, pDTCAttr );
    						}
    					}
    				}
#if(DEM_FREEZE_FRAME_RECORD_CLASS_NUM > 0u)
    			}
#endif
    		}
    		else
    		{
    			/* do nothing */
    		}
    		iloop++;
    	}

    	retVal = Dem_DcmGetFreezeFrameDataCheckFlag(Find,getData);
#else
    	retVal = DEM_GET_FFDATABYDTC_WRONG_DTC;
#endif
    }
    return retVal;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
/*************************************************************************/
/*
 * Brief               Gets the size of freeze frame data by DTC.
 * ServiceId           0x1f
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC : Diagnostic Trouble Code in UDS format
 *                       DTCOrigin : If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       RecordNumber: This parameter is a unique identifier for a freeze
 *                          frame record as defined in ISO 15031-5 and ISO 14229-1.
 * Param-Name[out]     SizeOfFreezeFrame:
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_DcmGetSizeOfFreezeFrameByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame
)
{
#if(DEM_FREEZE_FRAME_CLASS_NUM > 0u)
    uint16 iloop = 0u;
    uint8 index;
    uint16 dtcRef;
    uint8 memDest;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttr;
    P2VAR(Dem_FreezeFrameInfoType, AUTOMATIC, DEM_VAR) FFList;
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC, DEM_E_UNINIT);
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    if (SizeOfFreezeFrame == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFFREEZEFRAMEBYDTC, DEM_E_PARAM_POINTER);
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    /*SWS_Dem_01100 SWS_Dem_01101 */
    dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    if (dtcRef == DEM_DTC_REF_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
    {
        return  DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    *SizeOfFreezeFrame = 0u;
    memDest = Dem_GetInternalMemDest(DTCOrigin);
    if (memDest == DEM_MEM_DEST_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTCORIGIN;
    }
    if (RecordNumber == 0xFFu)
    {
        while (iloop < DEM_EVENT_PARAMETER_NUM)
        {
            if (DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef == dtcRef)
            {
                pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(iloop), memDest);
                if (pEntry != NULL_PTR)
                {
                    pDTCAttr = Dem_EventDTCAttributesCfg(pEntry->EventId);
                    index = 0u;
                    FFList = pEntry->FFList;
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)
                    while (index < DemFreezeFrameRecNumClass[pDTCAttr->DemFreezeFrameRecNumClassRef].RefNum)
#else
                    while (index < pDTCAttr->DemMaxNumberFreezeFrameRecords)
#endif

                    {
                        if (FFList->RecordNum != 0xFFu)
                        {
                            *SizeOfFreezeFrame += DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].DataSize;
                        }
                        index++;
                        FFList++;
                    }
                }
            }
            iloop++;
        }
        return DEM_GETSIZEBYDTC_OK;
    }
    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if (DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef == dtcRef)
        {
            pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(iloop), memDest);
            if (pEntry != NULL_PTR)
            {
                pDTCAttr = Dem_EventDTCAttributesCfg(pEntry->EventId);
                *SizeOfFreezeFrame += DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].DataSize;
            }
        }
        iloop++;
    }
    return DEM_GETSIZEBYDTC_OK;
#else
    return DEM_GETSIZEBYDTC_WRONG_DTC;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


/*************************************************************************/
/*
 * Brief               Gets extended data by DTC. The function stores the data in
 *                          the provided DestBuffer.
 * ServiceId           0x20
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                       DTCOrigin:If the Dem supports more than one event memory,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                       ExtendedDataNumber:dentification/Number of requested extended data
 *                       record. The values 0xFE and 0xFF are not allowed.
 * Param-Name[out]     DestBuffer:This parameter contains a byte pointer that points to
 *                          the buffer, to which the extended data record shall
 *                          be written to.
 * Param-Name[in/out]  BufSize:When the function is called this parameter contains
 *                          the maximum number of data bytes that can be written to the buffer.
 * Return              Status of the operation to retrieve extended data by DTC.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetExtendedDataRecordByDTCType, DEM_CODE)
Dem_DcmGetExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
)
{
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0u)

    uint8 memDest;
    Dem_ReturnGetExtendedDataRecordByDTCType retVal;
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg;

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC, DEM_E_UNINIT);
        return DEM_RECORD_WRONG_DTC;
    }
    if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC, DEM_E_PARAM_POINTER);
        return DEM_RECORD_WRONG_DTC;
    }
    if (ExtendedDataNumber >= 0xFEu)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETEXTENDEDDATARECORDBYDTC, DEM_E_PARAM_DATA);
        return DEM_RECORD_WRONG_NUMBER;
    }

    memDest = Dem_GetInternalMemDest(DTCOrigin);

    if (memDest == DEM_MEM_DEST_INVALID)
    {
        return DEM_RECORD_WRONG_DTCORIGIN;/*SWS_Dem_00171*/
    }

    pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);

    if (pExtDataCfg == NULL_PTR)
    {
        return DEM_RECORD_WRONG_NUMBER;
    }
    if (*BufSize < pExtDataCfg->DataSize)
    {
        return DEM_RECORD_WRONG_BUFFERSIZE;
    }

    *BufSize = 0u;

    retVal = Dem_GetExtendedRecordDataLoop(
    										memDest,
											DTC,
											ExtendedDataNumber,
											DestBuffer,
											BufSize
										  );

    return retVal;
#else
    return DEM_RECORD_WRONG_DTC;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Gets the size of extended data by DTC.
 * ServiceId           0x21
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTC:Diagnostic Trouble Code in UDS format.
 *                      DTCOrigin:If the Dem supports more than one event memory ,
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 *                      ExtendedDataNumber:Identification/Number of requested extended
 *                          data record.
 * Param-Name[out]     SizeOfExtendedDataRecord:Size of the requested extended data record(s)
 *                          including record number size
 * Param-Name[in/out]  none
 * Return              Status of the operation to retrieve the size of extended data.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnGetSizeOfDataByDTCType, DEM_CODE)
Dem_DcmGetSizeOfExtendedDataRecordByDTC(
    uint32 DTC,
    Dem_DTCOriginType DTCOrigin,
    uint8 ExtendedDataNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord
)
{
    Dem_ReturnGetSizeOfDataByDTCType retVal;
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)

    uint8 memDest;
    uint16 dtcRef;
    boolean detErr = FALSE;
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg;

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC, DEM_E_UNINIT);
        retVal = DEM_GETSIZEBYDTC_WRONG_DTC;
        detErr = TRUE;
    }
    else
    {
        retVal = Dem_DcmGetSizeOfExtendedDataRecordCheckParameter( DTC, DTCOrigin, SizeOfExtendedDataRecord );
    }

    if((detErr == FALSE) && (retVal == DEM_GETSIZEBYDTC_OK))
    {
    	dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    	memDest = Dem_GetInternalMemDest(DTCOrigin);
    	*SizeOfExtendedDataRecord = 0;

    	if (ExtendedDataNumber == 0xFEu)
    	{
    		ExtendedDataNumber = 0x90u;
    		while (ExtendedDataNumber < 0xEFu)
    		{
    			pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
    			if (pExtDataCfg == NULL_PTR)
    			{
    				return DEM_GETSIZEBYDTC_WRONG_RECNUM;
    			}

    			Dem_DcmGetSizeOfExtendedDataRecordLoop( dtcRef, memDest, SizeOfExtendedDataRecord, pExtDataCfg );
    			ExtendedDataNumber++;
    		}
    	}
    	else if (ExtendedDataNumber == 0xFFu)
    	{
    		ExtendedDataNumber = 0x01u;
    		while (ExtendedDataNumber < 0xEFu)
    		{
    			pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
    			if (pExtDataCfg == NULL_PTR)
    			{
    				return DEM_GETSIZEBYDTC_WRONG_RECNUM;
    			}

    			Dem_DcmGetSizeOfExtendedDataRecordLoop( dtcRef, memDest, SizeOfExtendedDataRecord, pExtDataCfg );
    			ExtendedDataNumber++;
    		}
    	}
    	else
    	{
    		pExtDataCfg = Dem_GetExtendedDataRecordCfg(ExtendedDataNumber);
    		if (pExtDataCfg == NULL_PTR)
    		{
    			return DEM_GETSIZEBYDTC_WRONG_RECNUM;
    		}

    		Dem_DcmGetSizeOfExtendedDataRecordLoop( dtcRef, memDest, SizeOfExtendedDataRecord, pExtDataCfg );
    	}
    	retVal = DEM_GETSIZEBYDTC_OK;
    }
#else
    	retVal = DEM_GETSIZEBYDTC_WRONG_DTC;
#endif
    return retVal;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/********[DTC storage]*********/

/*************************************************************************/
/*
 * Brief               Performs a parameter check and gives the result which would also be
                        returned by calling clear with same parameters.
 * ServiceId           0x7b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the result of the parameter check when performing a
 *                      clear command with same parameters (refer to Dem_DcmClearDTC).
                        Only the following return values will be used:
                        DEM_CLEAR_OK: DTC successfully cleared
                        DEM_CLEAR_WRONG_DTC: DTC value not existing (in this format)
                        DEM_CLEAR_WRONG_DTCORIGIN: Wrong DTC origin
                        DEM_CLEAR_FAILED: In case of general errors
with clear parameters
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmCheckClearParameter(uint32 DTC,
        Dem_DTCFormatType DTCFormat,
        Dem_DTCOriginType DTCOrigin)
{
#if (DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr = &DemClearDTCInfo;
#endif
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMCHECKCLEARPARAMETER, DEM_E_UNINIT);
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    if (DemClearDTCLock == DEM_CLEAR_LOCK)
	{
		DemClearDTCStatus = DEM_CLEAR_PENDING;
	}
    else
    {
        if ( DEM_DTC_ORIGIN_PRIMARY_MEMORY != DTCOrigin)
        {
            return DEM_CLEAR_WRONG_DTCORIGIN;
        }
    	if ((DTC !=  DEM_DTC_GROUP_ALL_DTCS)
    	   && (Dem_GetDTCIndex(DTC, DTCFormat) == DEM_DTC_REF_INVALID)
#if(DEM_GROUP_OF_DTC_NUM > 0)
		   && (DEM_GROUP_OF_DTC_INVALID == Dem_GetDTCGroupIndex(DTC))
#endif
           )
    	{
    		return DEM_CLEAR_WRONG_DTC;
    	}
#if(DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_VOLATILE)
        /*SWS_Dem_00570*/
        DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
        DemClearDTCStatus = DEM_CLEAR_OK;
#elif(DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_TRIGGER)
        /*SWS_Dem_00571*/
        DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
        DemClearNonvolatile = TRUE;
        DemClearDTCStatus = DEM_CLEAR_OK;
#elif(DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
        DemClearNonvolatile = TRUE;
        if (DemClearNonvolatileOK == 0x00)
        {
            /*SWS_Dem_00572*/
            DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
            DemClearNonvolatileOK = 0x02;
            DemClearDTCStatus = DEM_CLEAR_OK;
        }
        else if (DemClearNonvolatileOK == 0x01)
        {
            /*SWS_Dem_01057*/
            DemClearDTCLock = DEM_CLEAR_NOT_LOCK;
            DemClearNonvolatileOK = 0x02;
            DemClearDTCStatus = DEM_CLEAR_MEMORY_ERROR;
        }
        else
        {
            if ((pClr->DTCIndex == Dem_GetDTCIndex(DTC, DTCFormat)) &&
            (pClr->DTCFormat == DTCFormat) &&
            (pClr->DTCOrigin == DTCOrigin) &&
            (pClr->SID == DEM_SID_DCMCLEARDTC))
            {
                /*SWS_Dem_00663*/
                DemClearDTCStatus = DEM_CLEAR_PENDING;
            }
            else
            {
                /*SWS_Dem_00662 SWS_Dem_00664*/
                DemClearDTCStatus = DEM_CLEAR_OK;
            }
        }
#endif
    }

    return DemClearDTCStatus;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Clears single DTCs, as well as groups of DTCs. This API is intended
                       for the Dcm. It can only be used through the RTE (due to work-around
                       described below SWS_Dem_00009), and therefore no declaration is exported via Dem_Dcm.h.
 * ServiceId           0x22
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DTC:Defines the DTC in respective format, that shall be
 *                          cleared from the event memory. If the DTC fits to a
 *                          DTC group number, all DTCs of the group shall be cleared.
 *                       DTCFormat:Defines the input-format of the provided DTC value.
 *                       DTCOrigin:If the Dem supports more than one event memory
 *                          this parameter is used to select the source memory
 *                          the DTCs shall be read from.
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Status of the operation of type Dem_ReturnClearDTCType.
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnClearDTCType, DEM_CODE)
Dem_DcmClearDTC(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin
)
{
    P2VAR(Dem_ClearDTCInfoType, AUTOMATIC, DEM_VAR) pClr;
    uint16 dtcRef;
#if (DEM_DTR_NUM > 0)
    uint16 iloop = 0;
#endif

    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMCLEARDTC, DEM_E_UNINIT);
        DemClearDTCStatus = DEM_CLEAR_FAILED;
        return DEM_CLEAR_FAILED;
    }
    if ( DEM_DTC_ORIGIN_PRIMARY_MEMORY != DTCOrigin)
    {
        return DEM_CLEAR_WRONG_DTCORIGIN;
    }
    if ((DTC ==  DEM_DTC_GROUP_ALL_DTCS) && ( DEM_DTC_FORMAT_OBD == DTCFormat))
    {
#if (DEM_DTR_NUM > 0)
        while (iloop < DEM_DTR_NUM)
        {
            if (DemPbCfgPtr->DTR[iloop].DemDtrEventRef == DEM_EVENT_PARAMETER_INVALID)
            {
                Dem_ClearDTRInfoByDTRID(iloop);/*SWS_Dem_00764*/
            }
            iloop++;
        }
#endif
    }


    pClr = &DemClearDTCInfo;
    if (DemClearDTCLock == DEM_CLEAR_LOCK)
    {
        if ((pClr->SaveDTCIndex == Dem_GetDTCIndex(DTC, DTCFormat)) &&
        (pClr->SaveDTCFormat == DTCFormat) &&
        (pClr->SaveDTCOrigin == DTCOrigin) &&
        (pClr->SID == DEM_SID_DCMCLEARDTC))
        {
            /*SWS_Dem_00663*/
            DemClearDTCStatus = DEM_CLEAR_PENDING;
            return DEM_CLEAR_PENDING;
        }
        else
        {
            /*SWS_Dem_00662 SWS_Dem_00664*/
            DemClearDTCStatus = DEM_CLEAR_BUSY;
            return DEM_CLEAR_BUSY;
        }
    }
    else if (DemClearDTCLock == DEM_CLEAR_NOT_LOCK)
    {
        DemClearDTCLock = DEM_CLEAR_LOCK;/*SWS_Dem_00661*/
        pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
        pClr->DTCFormat = DTCFormat;
        pClr->DTCOrigin = DTCOrigin;
        pClr->SaveDTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
        pClr->SaveDTCFormat = DTCFormat;
        pClr->SaveDTCOrigin = DTCOrigin;
        pClr->SID = DEM_SID_CLEARDTC;
    }
    else
    {
        ;
    }
    /*SWS_Dem_00670] */
#if (DEM_CLEAR_DTCLIMITATION == DEM_ONLY_CLEAR_ALL_DTCS)
    if (DTCFormat != DEM_DTC_FORMAT_UDS || (DTC & 0xFFFFFF) != 0xFFFFFF)
    {
        DemClearDTCStatus = DEM_CLEAR_WRONG_DTC;
        return  DEM_CLEAR_WRONG_DTC;
    }
#endif

    DTCOrigin = Dem_GetInternalMemDest(DTCOrigin);
    if (DTCOrigin == DEM_MEM_DEST_INVALID)
    {
        return DEM_CLEAR_WRONG_DTCORIGIN;
    }

    if ((DTC & 0xFFFFFFUL) == 0xFFFFFFUL)
    {
        pClr->ClearAllGroup = TRUE;
    }
#if(DEM_CLEAR_DTCLIMITATION == DEM_ALL_SUPPORTED_DTCS)
    else
    {
        /*SWS_Dem_01100*/
        dtcRef = Dem_GetDTCIndex(DTC, DTCFormat);
        if ((dtcRef == DEM_DTC_REF_INVALID) 
#if(DEM_GROUP_OF_DTC_NUM > 0)
        && (DEM_GROUP_OF_DTC_INVALID == Dem_GetDTCGroupIndex(DTC))
#endif
        )
        {
            DemClearDTCStatus = DEM_CLEAR_WRONG_DTC;
            Dem_ResetDemClearDTCInfo();
            return  DEM_CLEAR_WRONG_DTC;
        }
        else
        {
            if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
            {
                DemClearDTCStatus = DEM_CLEAR_WRONG_DTC;
                Dem_ResetDemClearDTCInfo();
                return   DEM_CLEAR_WRONG_DTC;
            }
        }
        pClr->ClearAllGroup = FALSE;
        #if(DEM_GROUP_OF_DTC_NUM > 0)
        pClr->DTCGroupIndex = Dem_GetDTCGroupIndex(DTC);
        if (pClr->DTCGroupIndex == DEM_GROUP_OF_DTC_INVALID)
        #endif
        {
            pClr->DTCIndex = Dem_GetDTCIndex(DTC, DTCFormat);
            if (pClr->DTCIndex == DEM_DTC_REF_INVALID)
            {
                DemClearDTCStatus = DEM_CLEAR_WRONG_DTC;
                Dem_ResetDemClearDTCInfo();
                return DEM_CLEAR_WRONG_DTC;
            }
        }
    }
#endif
    pClr->memDest = DTCOrigin;
    Dem_Pending = TRUE;
    Dem_ClearDTCProcess();

#if(DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_VOLATILE)
    /*SWS_Dem_00570*/
    DemClearDTCStatus = DEM_CLEAR_OK;
    Dem_ResetDemClearDTCInfo();
    return DEM_CLEAR_OK;
#elif(DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_TRIGGER)
    /*SWS_Dem_00571*/
    DemClearNonvolatile = TRUE;
    DemClearDTCStatus = DEM_CLEAR_OK;
    Dem_ResetDemClearDTCInfo();
    return DEM_CLEAR_OK;
#elif(DEM_CLEAR_DTCBEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    DemClearNonvolatile = TRUE;
    if (DemClearNonvolatileOK == 0x00)
    {
        /*SWS_Dem_00572*/
        DemClearNonvolatileOK = 0x02;
        DemClearDTCStatus = DEM_CLEAR_OK;
        Dem_ResetDemClearDTCInfo();
        return DEM_CLEAR_OK;
    }
    else if (DemClearNonvolatileOK == 0x01)
    {
        /*SWS_Dem_01057*/
        DemClearNonvolatileOK = 0x02;
        DemClearDTCStatus = DEM_CLEAR_MEMORY_ERROR;
        return DEM_CLEAR_MEMORY_ERROR;
    }
    else
    {
        /*SWS_Dem_00663*/
        DemClearDTCStatus = DEM_CLEAR_PENDING;
        return DEM_CLEAR_PENDING;
    }
    DemClearDTCStatus = DEM_CLEAR_PENDING;
    return DEM_CLEAR_PENDING;
#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/*************************************************************************/
/*
 * Brief               Disables the DTC setting for a DTC group.
 * ServiceId           0x24
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCGroup:Defines the group of DTC that shall be disabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind, either
 *                          only OBD-relevant DTCs or all DTCs>
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns status of the operation>
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE) 
Dem_DcmDisableDTCSetting(
    uint32 DTCGroup,
    Dem_DTCKindType DTCKind
)
{
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCSETTING, DEM_E_UNINIT);
        return DEM_CONTROL_DTC_SETTING_N_OK;
    }
#if(DEM_GROUP_OF_DTC_NUM > 0)

    if ((DTCGroup & 0xFFFFFFUL) == 0xFFFFFFUL)
    {
        DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
        Dem_MemSet(DemDTCSettingInfo.DTCGroupStatus, 0x00u,
        DEM_GROUP_OF_DTC_NUM_BYTE);
    }
    else
    {
        DTCGroup = Dem_GetDTCGroupIndex(DTCGroup);
        if (DTCGroup == DEM_GROUP_OF_DTC_INVALID)
        {
            return DEM_CONTROL_DTC_WRONG_DTCGROUP;
        }
        DEM_BITS_CLR(DemDTCSettingInfo.DTCGroupStatus, DTCGroup);
    }
#else
    if ((DTCGroup & 0xFFFFFFUL)!= 0xFFFFFFUL)
    {
        DEM_DET_REPORT(DEM_SID_DCMDISABLEDTCSETTING, DEM_E_WRONG_CONFIGURATION);
        return DEM_CONTROL_DTC_SETTING_N_OK;
    }
    DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
#endif
    DemDTCSettingInfo.DTCKind = DTCKind;
    Dem_SetDTCSettingProcess();
    return DEM_CONTROL_DTC_SETTING_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               Enables the DTC setting for a DTC group.
                        This API is intended for the Dcm. It can only be used through the RTE
                        (due to work-around described below SWS_Dem_00035), and therefore
                        no declaration is exported via Dem_Dcm.h.
 * ServiceId           0x25
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DTCGroup:Defines the group of DTC that shall be enabled to
 *                          store in event memory.
 *                       DTCKind:This parameter defines the requested DTC kind,
 *                          either only OBD-relevant DTCs or all DTCs
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              Returns the status of the operation
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(Dem_ReturnControlDTCSettingType, DEM_CODE)
Dem_DcmEnableDTCSetting(
    uint32 DTCGroup,
    Dem_DTCKindType DTCKind
)
{
    if (DemInitState != DEM_STATE_INIT)
    {
        DEM_DET_REPORT(DEM_SID_DCMENABLEDTCSETTING, DEM_E_UNINIT);
        return DEM_CONTROL_DTC_SETTING_N_OK;
    }
#if(DEM_GROUP_OF_DTC_NUM > 0)
    if ((DTCGroup & 0xFFFFFFUL) == 0xFFFFFFUL)
    {
        DemDTCSettingInfo.AllGroupIsEnabled = FALSE;
        Dem_MemSet(DemDTCSettingInfo.DTCGroupStatus, 0xFFu,
        DEM_GROUP_OF_DTC_NUM_BYTE);
    }
    else
    {
        DTCGroup = (uint32)(Dem_GetDTCGroupIndex(DTCGroup));
        if ((uint8)DTCGroup == DEM_GROUP_OF_DTC_INVALID)
        {
            return DEM_CONTROL_DTC_WRONG_DTCGROUP;
        }
        DEM_BITS_SET(DemDTCSettingInfo.DTCGroupStatus,DTCGroup);
    }
#else
    if ((DTCGroup&0xFFFFFFUL)!= 0xFFFFFFUL)
    {
        DEM_DET_REPORT(DEM_SID_DCMENABLEDTCSETTING, DEM_E_WRONG_CONFIGURATION);
        return DEM_CONTROL_DTC_SETTING_N_OK;
    }
    DemDTCSettingInfo.AllGroupIsEnabled = TRUE;
#endif
    DemDTCSettingInfo.DTCKind = DTCKind;
    Dem_SetDTCSettingProcess();
    return DEM_CONTROL_DTC_SETTING_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
/*************************************************************************/
/*
 * Brief               Dem_SetDTCSettingProcess
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      none
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/


static FUNC(void,DEM_CODE)
Dem_DcmGetSizeOfExtendedDataRecordLoop(
	uint16 dtcRef,
	uint8 memDest,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord,
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtDataCfg
)
{
	uint16 iloop = 0u;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if (DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef == dtcRef)
        {
            pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(iloop), memDest);
            if (pEntry != NULL_PTR)
            {
                *SizeOfExtendedDataRecord += pExtDataCfg->DataSize;
            }
        }
        iloop++;
    }
}

static FUNC(void,DEM_CODE)
Dem_DcmGetFreezeFrameDataByDTCProcess(
	P2VAR(uint8, AUTOMATIC, DEM_VAR) pFFData,
	P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
	P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttr
)
{
    uint8 index;
    uint16 didRefNum;
    uint8  csIndex;
    uint16 csStart;
    uint8 dataSize;

    P2CONST(uint16, AUTOMATIC, DEM_CONST) pDidIndex;
    P2CONST(Dem_DidClassType, AUTOMATIC, DEM_CONST) pDid;

	pDidIndex = &DemDidClassRef[DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].StartIndex];
	*(DestBuffer) = DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].RefNum;

	DestBuffer++;
	*BufSize += 1u;

	for (didRefNum = 0u; didRefNum < DemFreezeFrameClass[pDTCAttr->DemFreezeFrameClassRef].RefNum; didRefNum++)
	{
		pDid = &DemDidClass[*pDidIndex];
		csStart = pDid->StartIndex;
		csIndex = 0u;
		dataSize = 0u;

		*(DestBuffer) = (uint8)((pDid->DemDidIdentifier) >> 8u);	/* DemIdentifier */
		DestBuffer++;

		*(DestBuffer) = (uint8)(pDid->DemDidIdentifier);
		DestBuffer++;

		while (csIndex < pDid->RefNum)
		{
			csIndex++;
#if(DEM_DATA_ELEMENT_CLASS_NUM > 0u)
			if (DemDataElementClass[csStart].DemDataElementReadFnc != NULL_PTR)
			{
				Dem_MemCopy(DestBuffer, pFFData, DemDataElementClass[csStart].DemDataElementDataSize);
	            index = 0u;
	            while (index < DemDataElementClass[csStart].DemDataElementDataSize)
	            {
	            	DestBuffer++;
	            	index++;
	            }

	            index = 0u;
	            while (index < DemDataElementClass[csStart].DemDataElementDataSize)
	            {
	            	pFFData++;
	            	index++;
	            }
				dataSize += DemDataElementClass[csStart].DemDataElementDataSize;
			}
#endif
			csStart++;
		}
		(*BufSize) += (2u + (uint16)dataSize);
		pDidIndex++;
	}
}



#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void, DEM_CODE) Dem_SetDTCSettingProcess(void)
{
    uint16 iloop = 0;
    P2VAR(Dem_EventRelateInformationType, AUTOMATIC, DEM_VAR) pEvent;
    P2CONST(Dem_EventParameterType, AUTOMATIC, DEM_CONST) pEventCfg;

    pEventCfg = DemPbCfgPtr->DemEventParameter;

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        pEvent = Dem_GetEventInfo(iloop);
        if (pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
        {
			if (DemDTCSettingInfo.DTCKind == DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].DTCKind)
			{
#if(DEM_GROUP_OF_DTC_NUM > 0)
				if ((pEventCfg->DemDTCRef != DEM_DTC_REF_INVALID)
					&& (DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].GroupRef
						!= DEM_GROUP_OF_DTC_INVALID))
				{
					if (0x00u == DEM_BITS_ISSET(DemDTCSettingInfo.DTCGroupStatus,
								DemPbCfgPtr->DemDTC[pEventCfg->DemDTCRef].GroupRef))
					{
						DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
						Dem_DebounceFreeze(iloop);/*SWS_Dem_00678*/
					}
					else
					{
						if(0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
						{
							DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
							/* req SWS_Dem_00682 Monitor re-initialization  */
							if (DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE != NULL_PTR)
							{
								DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE(DEM_INIT_MONITOR_REENABLED);
							}
						}
					}
				}
				else
#endif
				{
					if (DemDTCSettingInfo.AllGroupIsEnabled == FALSE)
					{
						DEM_FLAGS_CLR(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);/* ControlDTCSetting is disabled*/
						Dem_DebounceFreeze(iloop);/*SWS_Dem_00678*/
					}
					else
					{
						if(0x00u == DEM_FLAGS_ISSET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING))
						{
							DEM_FLAGS_SET(pEvent->Status, DEM_EVENT_STATUS_DTC_SETTING);
							/* req SWS_Dem_00682 Monitor re-initialization  */
							if (DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE != NULL_PTR)
							{
								DemPbCfgPtr->DemEventParameter[iloop].DemCallbackInitMForE(DEM_INIT_MONITOR_REENABLED);
							}
						}
					}
				}
			}
        }
        pEventCfg++;
        iloop++;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && DTCFormat
 * Param-Name[out]     DTC
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_GetEventDTC(Dem_EventIdType IntId,
    Dem_DTCFormatType DTCFormat,
    P2VAR(uint32, AUTOMATIC, DEM_VAR) DTC)
{

    switch (DTCFormat)
    {
        case DEM_DTC_FORMAT_UDS:
            if ((DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef != DEM_DTC_REF_INVALID)
				&& (DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemDtcValue != DEM_DTC_CFG_INVALID))
            {
                *DTC = DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemDtcValue;
            }
            else
            {
                *DTC = 0x00UL;
            }
        break;

        case DEM_DTC_FORMAT_J1939:
        if ((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
                && (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef].DemJ1939DTCValue != DEM_J1939_DTC_CFG_INVALID))
        {
            *DTC = DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef].DemJ1939DTCValue;
        }
        else
        {
            *DTC = 0x00UL;
        }
        break;

        case DEM_DTC_FORMAT_OBD:
        if ((DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef != DEM_OBD_DTC_INVALID)
                && (DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef].DemDtcValue != DEM_OBD_DTC_CFG_INVALID))
        {
            *DTC = (uint32)DemPbCfgPtr->DemObdDTC[DemPbCfgPtr->DemDTC[DemPbCfgPtr->DemEventParameter[IntId].DemDTCRef].DemObdDTCRef].DemDtcValue;
        }
        else
        {
            *DTC = 0x00UL;
        }
        break;
        default:

        *DTC = 0x00UL;
        break;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && Status
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
FUNC(void, DEM_CODE) Dem_SetOccurrenceEvent(Dem_EventIdType IntId,
    uint8 Status)
{
    switch (Status)
    {
        case DEM_UDS_STATUS_TF:
            if (DemDTCByOccurrenceTimeInfo.FirstFailed
                == DEM_EVENT_PARAMETER_INVALID)
            {
                DemDTCByOccurrenceTimeInfo.FirstFailed = IntId;
            }
            DemDTCByOccurrenceTimeInfo.MostRecentFailed = IntId;
            break;
        case DEM_UDS_STATUS_CDTC:
            if (DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed
                == DEM_EVENT_PARAMETER_INVALID)
            {
                DemDTCByOccurrenceTimeInfo.FirstDtcConfirmed = IntId;
            }
            DemDTCByOccurrenceTimeInfo.MostRecDtcConfirmed = IntId;
            break;
        default:
            break;
    }
    return;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_EXTENDED_DATA_RECORD_CLASS_NUM > 0)
/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ExtendedDataNumber
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              const Dem_ExtendedDataRecordClassType*
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST), DEM_CODE)
Dem_GetExtendedDataRecordCfg(uint8 ExtendedDataNumber)
{
    uint8 iloop = 0;

    while (iloop < DEM_EXTENDED_DATA_RECORD_CLASS_NUM)
    {
        if (DemExtendedDataRecordClass[iloop].DemExtendedDataRecordNumber == ExtendedDataNumber)
        {
            return &(DemExtendedDataRecordClass[iloop]);
        }
        iloop++;
    }
    return NULL_PTR;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif


/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && Status
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#if(DEM_EXTENDED_DATA_CLASS_NUM > 0u)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_ReturnGetExtendedDataRecordByDTCType,DEM_CODE)
Dem_GetExtendedRecordDataLoop(
		uint8 memDest,
		uint32 DTC,
		uint8 ExtendedDataNumber,
		P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
		P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
)
{
	uint16  dtcRef;
	uint16  iloop = 0u;
	uint16  index = 0u;
    uint16  BufferSize = 0u;
    boolean Find = FALSE;

    P2CONST(uint8, AUTOMATIC, DEM_CONST) pExtIndex;
    P2VAR(Dem_EventMemEntryType, AUTOMATIC, DEM_VAR) pEntry;
    dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);    /*SWS_Dem_01100 SWS_Dem_01101*/
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg = &DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef];
    P2CONST(Dem_ExtendedDataRecordClassType, AUTOMATIC, DEM_CONST) pExtRecord;

    if(DEM_EXTENDED_DATA_INVALID == pDTCAttrCfg->DemExtendedDataClassRef)
    {
 	    return DEM_RECORD_WRONG_NUMBER;
    }
    if (dtcRef == DEM_DTC_REF_INVALID)
    {
        return DEM_RECORD_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
    {
        return  DEM_RECORD_WRONG_DTC;
    }

    while (iloop < DEM_EVENT_PARAMETER_NUM)
    {
        if ((dtcRef != DEM_DTC_REF_INVALID)
        	&& (DemPbCfgPtr->DemEventParameter[iloop].DemDTCRef == dtcRef))
        {
        	pDTCAttrCfg = &DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcRef].DemDTCAttributesRef];
            pExtIndex = &DemExtendedDataRecordClassRef[DemExtendedDataClass[pDTCAttrCfg
                ->DemExtendedDataClassRef].StartIndex];
            index = 0u;
            while (index < DemExtendedDataClass[pDTCAttrCfg->DemExtendedDataClassRef].RefNum)
            {
                if (*pExtIndex != DEM_EXTENDED_DATA_RECORD_INVALID)
                {
                    pExtRecord = &DemExtendedDataRecordClass[*pExtIndex];
                    if (pExtRecord->DemExtendedDataRecordNumber == ExtendedDataNumber)
                    {
                    	Find = TRUE;
                    }
                }
                index++;
            }
        	if (Find == TRUE)
        	{
				pEntry = Dem_MemEntryGet(Dem_GetEventExternalId(iloop), memDest);
				if (pEntry != NULL_PTR)
				{
					(void)Dem_ExtendedDataGetFromEntry(pEntry, ExtendedDataNumber, DestBuffer,&BufferSize);
					(*BufSize) += BufferSize;
		            index = 0u;
		            while (index < (*BufSize))
		            {
		            	DestBuffer++;
		            	index++;
		            }
				}
        	}
        }
        else
        {
            /* do nothing */
        }
        iloop++;
    }
    if(Find == FALSE)
    {
        return DEM_RECORD_WRONG_NUMBER;
    }
    return DEM_RECORD_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*************************************************************************/
/*
 * Brief               BRIEF DESCRIPTION
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant/Non Reentrant
 * Param-Name[in]      IntId && Status
 * Param-Name[out]     none
 * Param-Name[in/out]  none
 * Return              none
 */
/*************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE)
Dem_DcmCheckDTCFilterStatus(
P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatus
)
{
    P2VAR(Dem_DTCFilterInfoType, AUTOMATIC, DEM_VAR) pFilter=&DemDTCFilterInfo;

    if (DemInitState != DEM_STATE_INIT)
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTC, DEM_E_UNINIT);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }

    if ((DTCStatus == NULL_PTR) || (DTC == NULL_PTR))
    {
    	DEM_DET_REPORT(DEM_SID_DCMGETNEXTFILTEREDDTC, DEM_E_PARAM_POINTER);
        return DEM_FILTERED_NO_MATCHING_ELEMENT;
    }

    if (((pFilter->DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS)/*SWS_Dem_00653*/
            && (pFilter->DTCStatusMask != DEM_UDS_STATUS_CDTC))
            || ((pFilter->DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS)
            && (pFilter->DTCStatusMask !=  DEM_UDS_STATUS_PDTC))
            || ((pFilter->DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS)
            && (pFilter->DTCOrigin !=  DEM_DTC_ORIGIN_PERMANENT_MEMORY)))
    {
        /*idle*/
    }
    else
    {
        if (Dem_Pending == TRUE)
        {
            return DEM_FILTERED_PENDING;
        }
    }

    return DEM_FILTERED_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(void,DEM_CODE)
Dem_DcmGetFreezeFrameDataByRecordNumber(
	uint8 RecordNumber,
	uint16 dtcIndex,
	boolean* Find
)
{
#if(DEM_TYPE_OF_FREEZE_FRAME_RECORD_NUMERATION == DEM_FF_RECNUM_CONFIGURED)

    uint8 index = 0u;
    P2CONST(uint8, AUTOMATIC, DEM_CONST) pFFRecordIndex;
    P2CONST(Dem_DTCAttributesType, AUTOMATIC, DEM_CONST) pDTCAttrCfg;
    P2CONST(Dem_FreezeFrameRecordClassType, AUTOMATIC, DEM_CONST) pFFRecordCfg;

	pDTCAttrCfg = &DemPbCfgPtr->DemDTCAttributes[DemPbCfgPtr->DemDTC[dtcIndex].DemDTCAttributesRef];
	pFFRecordIndex =
		&DemFreezeFrameRecordClassRef[DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].StartIndex];

	while ((index < DemFreezeFrameRecNumClass[pDTCAttrCfg->DemFreezeFrameRecNumClassRef].RefNum) && (*Find == FALSE))
	{
		if (*pFFRecordIndex != DEM_FREEZE_FRAME_RECORD_INVALID)
		{
			pFFRecordCfg = &DemFreezeFrameRecordClass[*pFFRecordIndex];
			if (pFFRecordCfg->DemFreezeFrameRecordNumber == RecordNumber)
			{
				*Find = TRUE;
			}
		}
		pFFRecordIndex++;
		index++;
	}

#endif
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_ReturnGetFreezeFrameDataByDTCType,DEM_CODE)
Dem_DcmGetFreezeFrameDataCheckFlag(
	boolean Find,
	boolean getData
)
{
    if(Find == FALSE)
    {
        return DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER;
    }
    if(getData == FALSE)
    {
    	return DEM_GET_FFDATABYDTC_FAILED;
    }
    return DEM_GET_FFDATABYDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_ReturnGetFreezeFrameDataByDTCType,DEM_CODE)
Dem_DcmGetFreezeFrameDataCheckParameter(
	uint32 DTC,
	Dem_DTCOriginType DTCOrigin,
	uint8 RecordNumber,
	P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DestBuffer,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize
)
{
    uint16 dtcIndex;
    uint8 memDest;

    dtcIndex = Dem_GetDTCIndex(DTC,DEM_DTC_FORMAT_UDS);
    memDest = Dem_GetInternalMemDest(DTCOrigin);

    if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
    {
        DEM_DET_REPORT(DEM_SID_DCMGETFREEZEFRAMEDATABYDTC, DEM_E_PARAM_POINTER);
        return DEM_GET_FFDATABYDTC_WRONG_DTC;
    }
    if (RecordNumber == 0xFFu)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETFREEZEFRAMEDATABYDTC, DEM_E_PARAM_DATA);
        return DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER;
    }
    /*SWS_Dem_01100 SWS_Dem_01101*/
    if (dtcIndex == DEM_DTC_REF_INVALID)
    {
        return DEM_GET_FFDATABYDTC_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcIndex].SuppressionStatus == TRUE)
    {
        return DEM_GET_FFDATABYDTC_WRONG_DTC;
    }
    if (memDest == DEM_MEM_DEST_INVALID)
    {
        return DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN;
    }
    return DEM_GET_FFDATABYDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_EXTENDED_DATA_CLASS_NUM > 0)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
static FUNC(Dem_ReturnGetSizeOfDataByDTCType,Dem_CODE)
Dem_DcmGetSizeOfExtendedDataRecordCheckParameter(
	uint32 DTC,
	Dem_DTCOriginType DTCOrigin,
	P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord
)
{
    uint16 dtcRef;
    uint8 memDest;

    dtcRef = Dem_GetDTCIndex(DTC, DEM_DTC_FORMAT_UDS);
    memDest = Dem_GetInternalMemDest(DTCOrigin);

    if (SizeOfExtendedDataRecord == NULL_PTR)
    {
        DEM_DET_REPORT(DEM_SID_DCMGETSIZEOFEXTENDEDDATARECORDBYDTC, DEM_E_PARAM_POINTER);
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    /*SWS_Dem_01100 SWS_Dem_01101*/
    if (dtcRef == DEM_DTC_REF_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    if (DemDTCGeneralStatus[dtcRef].SuppressionStatus == TRUE)
    {
        return  DEM_GETSIZEBYDTC_WRONG_DTC;
    }
    if (memDest == DEM_MEM_DEST_INVALID)
    {
        return DEM_GETSIZEBYDTC_WRONG_DTCORIGIN;
    }
    return DEM_GETSIZEBYDTC_OK;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

/*******************************************************************************
**                      end of file                                           **
*******************************************************************************/
