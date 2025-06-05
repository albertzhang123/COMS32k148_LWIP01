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
**  FILENAME    : MemIf.c                                                     **
**                                                                            **
**  Created on  : 2020/08/26                                                  **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : MemIf source description                                    **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR classic Platform R19-11                        **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>      <AUTHOR>       <REVISION LOG>
 *  V1.0.0    20200826    Haibin.Shao     Initial version
 *  V1.0.1    20211206    Haibin.Shao     Modify version(update version information)
*/
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define MEMIF_C_AR_MAJOR_VERSION  (19U)
#define MEMIF_C_AR_MINOR_VERSION  (1U)
#define MEMIF_C_AR_PATCH_VERSION  (1U)
#define MEMIF_C_SW_MAJOR_VERSION  (1U)
#define MEMIF_C_SW_MINOR_VERSION  (0U)
#define MEMIF_C_SW_PATCH_VERSION  (1U)

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "MemIf.h"

#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (MEMIF_C_AR_MAJOR_VERSION != MEMIF_H_AR_MAJOR_VERSION)
#error "MemIf.c : Mismatch in Specification Major Version"
#endif
#if (MEMIF_C_AR_MINOR_VERSION != MEMIF_H_AR_MINOR_VERSION)
#error "MemIf.c : Mismatch in Specification _MINOR Version"
#endif
#if (MEMIF_C_AR_PATCH_VERSION != MEMIF_H_AR_PATCH_VERSION)
#error "MemIf.c : Mismatch in Specification PATCH Version"
#endif
#if (MEMIF_C_SW_MAJOR_VERSION != MEMIF_H_SW_MAJOR_VERSION)
#error "MemIf.c : Mismatch in Software Major Version"
#endif
#if (MEMIF_C_SW_MINOR_VERSION != MEMIF_H_SW_MINOR_VERSION)
#error "MemIf.c : Mismatch in Software MINOR Version"
#endif
#if (MEMIF_C_SW_PATCH_VERSION != MEMIF_H_SW_PATCH_VERSION)
#error "MemIf.c : Mismatch in Software PATCH Version"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/


/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
/********************************************************************************************************
 *  MemIf_DetChkDeviceIndex()
 ********************************************************************************************************/
/*!
 * \brief      Checks if device index passed to MemIf is in range
 * \details    Checks if device index passed to MemIf is in range
 * \param[in]  DeviceIndex
 * \return     E_OK: DeviceIndex is valid
 * \return     E_NOT_OK: DeviceIndex is out of range
 * \pre        Pre-compile switch MEMIF_DEV_ERROR_DETECT is enabled
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
static FUNC(Std_ReturnType, MEMIF_CODE) MemIf_DetChkDeviceIndex(uint8 DeviceIndex);
#endif
/*********************************************************************************************************
 *  MemIf_IsBitSet()
 ********************************************************************************************************/
/*!
 * \brief      Checks if Status is set in Mask
 * \details    Checks if Status is set in Mask
 * \param[in]  Mask
 * \param[in]  Status
 * \return     E_OK: Status bit is set
 * \return     E_NOT_OK: Status bit is not set
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
static FUNC(Std_ReturnType, MEMIF_CODE) MemIf_IsBitSet(uint8 Mask, MemIf_StatusType Status);

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#if (STD_ON == MEMIF_CFG_USE_FCT_API)
#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

/******************************************************************************/
/*
 * Brief               Invokes the "Read" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 *                     BlockOffset:
 *                     Length:
 * Param-Name[out]     DataBufferPtr:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Read
(
	VAR(uint8, AUTOMATIC) DeviceIndex,
	VAR(uint16, AUTOMATIC) BlockNumber,
	VAR(uint16, AUTOMATIC) BlockOffset,
	P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr,
	VAR(uint16, AUTOMATIC) Length
)
{
	/* ----- Local Variables --------*/
	Std_ReturnType retVal;
	uint8 errorId;
	/* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON) /* ECO_IGNORE_LINE */
	if(MemIf_DetChkDeviceIndex(DeviceIndex) == (Std_ReturnType)E_NOT_OK)
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
		retVal = (Std_ReturnType)E_NOT_OK;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation --------*/
		if(MemIf_MemHwaApis_at[DeviceIndex].Read == NULL_PTR)
		{
			retVal = (Std_ReturnType)E_NOT_OK;
		}
		else
		{
			retVal = MemIf_MemHwaApis_at[DeviceIndex].Read(BlockNumber,BlockOffset,DataBufferPtr,Length);
		}
	}
	/* ----- Development Error Report --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_READ, errorId);
	}
	else
	{
		/* do nothing */
	}
#else
	(void)errorId;
#endif
	return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "Write" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 *                     DataBufferPtr:
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Write
(
	VAR(uint8, AUTOMATIC) DeviceIndex,
	VAR(VAR(uint16, AUTOMATIC), AUTOMATIC) BlockNumber,
	P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr
)
{
	/* ----- Local Variables --------*/
	Std_ReturnType retVal;
	uint8 errorId;
	/* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(MemIf_DetChkDeviceIndex(DeviceIndex) == (Std_ReturnType)E_NOT_OK)
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
		retVal = (Std_ReturnType)E_NOT_OK;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation --------*/
		if(MemIf_MemHwaApis_at[DeviceIndex].Write == NULL_PTR)
		{
			retVal = (Std_ReturnType)E_NOT_OK;
		}
		else
		{
			retVal = MemIf_MemHwaApis_at[DeviceIndex].Write(BlockNumber, DataBufferPtr);
		}
	}
	/* ----- Development Error Report --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_WRITE, errorId);
	}
	else
	{
		/* do nothing */
	}
#else
	(void)errorId;
#endif
	return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "InvalidateBlock" function of the underlying memory abstraction module selected by the parameter DeviceIndex..
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE) MemIf_InvalidateBlock
(
	VAR(uint8, AUTOMATIC) DeviceIndex,
	VAR(uint16, AUTOMATIC) BlockNumber
)
{
	/* ----- Local Variables --------*/
	Std_ReturnType retVal;
	uint8 errorId;
	/* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(MemIf_DetChkDeviceIndex(DeviceIndex) == (Std_ReturnType)E_NOT_OK)
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
		retVal = (Std_ReturnType)E_NOT_OK;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation --------*/
		if(MemIf_MemHwaApis_at[DeviceIndex].InvalidateBlock == NULL_PTR)
		{
			retVal = (Std_ReturnType)E_NOT_OK;
		}
		else
		{
			retVal = MemIf_MemHwaApis_at[DeviceIndex].InvalidateBlock(BlockNumber);
		}
	}
	/* ----- Development Error Report --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_INVALIDATE, errorId);
	}
	else
	{
		/* do nothing */
	}
#else
	(void)errorId;
#endif
	return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "EraseImmediateBlock" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 *                     BlockNumber: index number of logic block
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              Std_ReturnType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return E_NOT_OK else it shall
 *                     return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(Std_ReturnType, MEMIF_CODE) MemIf_EraseImmediateBlock
(
	VAR(uint8, AUTOMATIC) DeviceIndex,
	VAR(uint16, AUTOMATIC) BlockNumber
)
{
	/* ----- Local Variables --------*/
	Std_ReturnType retVal;
	uint8 errorId;
	/* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(MemIf_DetChkDeviceIndex(DeviceIndex) == (Std_ReturnType)E_NOT_OK)
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
		retVal = (Std_ReturnType)E_NOT_OK;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation --------*/
		if(MemIf_MemHwaApis_at[DeviceIndex].EraseImmediateBlock == NULL_PTR)
		{
			retVal = (Std_ReturnType)E_NOT_OK;
		}
		else
		{
			retVal = MemIf_MemHwaApis_at[DeviceIndex].EraseImmediateBlock(BlockNumber);
		}
	}
	/* ----- Development Error Report --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_ERASE, errorId);
	}
	else
	{
		/* do nothing */
	}
#else
	(void)errorId;
#endif
	return retVal;
}

/******************************************************************************/
/*
 * Brief               Invokes the "Cancel" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, MEMIF_CODE) MemIf_Cancel(VAR(uint8, AUTOMATIC) DeviceIndex)
{
	/* ----- Local Variables --------*/
	uint8 errorId;
	/* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if (MemIf_DetChkDeviceIndex(DeviceIndex) == (Std_ReturnType)E_NOT_OK)
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation --------*/
		if(MemIf_MemHwaApis_at[DeviceIndex].Cancel == NULL_PTR)
		{
			/* do nothing */
		}
		else
		{
			MemIf_MemHwaApis_at[DeviceIndex].Cancel();
		}
	}
	/* ----- Development Error Report --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_CANCEL, errorId);
	}
	else
	{
		/* do nothing */
	}
#else
	(void)errorId;
#endif
	return;
}

/******************************************************************************/
/*
 * Brief               Invokes the "GetJobResult" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_JobResultType:
 *                     In case development error detection is enabled for the Memory
 *                     Abstraction Interface and a development error is detected according to
 *                     SWS_MemIf_00022 the function shall return MEMIF_JOB_FAILED else
 *                     it shall return the value of the called function of the underlying module.
 */
/******************************************************************************/
FUNC(MemIf_JobResultType, MEMIF_CODE) MemIf_GetJobResult(VAR(uint8, AUTOMATIC) DeviceIndex)
{
	/* ----- Local Variables --------*/
	MemIf_JobResultType retVal;
	uint8 errorId;
	/* ----- Development Error Detection ---------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(MemIf_DetChkDeviceIndex(DeviceIndex) == (Std_ReturnType)E_NOT_OK)
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
		retVal = MEMIF_JOB_FAILED;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation ----------*/
		if(MemIf_MemHwaApis_at[DeviceIndex].GetJobResult == NULL_PTR)
		{
			retVal = MEMIF_JOB_FAILED;
		}
		else
		{
			retVal = MemIf_MemHwaApis_at[DeviceIndex].GetJobResult();
		}
	}
	/* ----- Development Error Report ------------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_GET_JOB_RESULT, errorId);
	}
#else
	(void)errorId;
#endif
	return retVal;
}

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"
#endif

#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

/******************************************************************************/
/*
 * Brief               Invokes the "SetMode" functions of all underlying memory abstraction modules.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None: work mode of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, MEMIF_CODE) MemIf_SetMode(VAR(MemIf_ModeType, AUTOMATIC) Mode)
{
	uint8 MemIf_LoopCounter;
	for(MemIf_LoopCounter = 0U; MemIf_LoopCounter < MemIf_NumberOfDevices_u8; MemIf_LoopCounter++)
	{
		MemIf_MemHwaApis_at[MemIf_LoopCounter].SetMode(Mode);
	}
}

/******************************************************************************/
/*
 * Brief               Invokes the "GetStatus" function of the underlying memory abstraction module selected by the parameter DeviceIndex.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      DeviceIndex: index number of EEPROM device
 * Param-Name[out]     None:
 * Param-Name[in/out]  None:
 * Return              MemIf_StatusType:
 */
/******************************************************************************/
FUNC(MemIf_StatusType, MEMIF_CODE) MemIf_GetStatus(VAR(uint8, AUTOMATIC) DeviceIndex)
{
	/* ----- Local Variables -------*/
	uint8 errorId;
	MemIf_StatusType retVal;
	uint8 MemIf_LoopCounter;
	uint8 MemIf_StatusMask = 0U;
	/* ----- Development Error Detection --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON) /* ECO_IGNORE_LINE */
	/* #10 Check if DeviceIndex is out of bounds. BroadcastId (0xFF) is a valid input. */
	if((DeviceIndex != (uint8)MEMIF_BROADCAST_ID) && (DeviceIndex >= MemIf_NumberOfDevices_u8))
	{
		errorId = (uint8)MEMIF_E_PARAM_DEVICE;
		retVal = MEMIF_UNINIT;
	}
	else
#endif
	{
		errorId = (uint8)MEMIF_E_NO_ERROR;
		if((uint8)MEMIF_BROADCAST_ID == DeviceIndex)
		{
			/* #20 Get status of all configured MemHwA modules */
			for(MemIf_LoopCounter = 0U; MemIf_LoopCounter < MemIf_NumberOfDevices_u8; MemIf_LoopCounter++)
			{
				/* #25 Store each status to status mask */
				MemIf_StatusMask |= (uint8)((uint8)1U << (uint8)MemIf_MemHwaApis_at[MemIf_LoopCounter].GetStatus());
			}
			/* #30 Check if UNINIT-bit is set */
			if(MemIf_IsBitSet(MemIf_StatusMask, MEMIF_UNINIT) == (Std_ReturnType)E_OK)
			{
				retVal = MEMIF_UNINIT;
			}
			/* #31 Check if BUSY-bit is set */
			else if(MemIf_IsBitSet(MemIf_StatusMask, MEMIF_BUSY) == (Std_ReturnType)E_OK)
			{
				retVal = MEMIF_BUSY;
			}
			/* #32 Check if BUSY_INTERNAL-bit is set */
			else if(MemIf_IsBitSet(MemIf_StatusMask, MEMIF_BUSY_INTERNAL) == (Std_ReturnType)E_OK)
			{
				retVal = MEMIF_BUSY_INTERNAL;
			}
			/* #33 All MemHwA modules are MEMIF_IDLE */
			else
			{
				retVal = MEMIF_IDLE;
			}
		}
		/* #40 Get status of MemHwA module indexed by DeviceIndex */
		else
		{
			retVal = MemIf_MemHwaApis_at[DeviceIndex].GetStatus();
		}
	}
	/* #50 Report development error if configured */
	/* ----- Development Error Report ---------------------------------------------------------------------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_GET_STATUS, errorId);
	}
#else
	(void)errorId;
#endif
	return retVal;
}

#if (STD_ON == MEMIF_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Service to get the version information of this module.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None:
 * Param-Name[out]     VersionInfo: Pointer to standard version information structure
 * Param-Name[in/out]  None:
 * Return              None:
 */
/******************************************************************************/
FUNC(void, MEMIF_CODE) MemIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, MEMIF_APPL_DATA) VersionInfoPtr)
{
	/* ----- Local Variables ---------*/
	uint8 errorId;
	/* ----- Development Error Detection --------*/
#if (STD_ON == MEMIF_DEV_ERROR_DETECT)
    if(NULL_PTR == VersionInfoPtr)
    {
		errorId = (uint8)MEMIF_E_PARAM_POINTER;
    }
    else
#endif
    {
		errorId = (uint8)MEMIF_E_NO_ERROR;
		/* ----- Implementation ------------------------------------------------------------*/
    	VersionInfoPtr->moduleID = (VAR(uint16, AUTOMATIC))MEMIF_MODULE_ID;
    	VersionInfoPtr->vendorID = (VAR(uint16, AUTOMATIC))MEMIF_VENDOR_ID;
    	VersionInfoPtr->sw_major_version = (VAR(uint8, AUTOMATIC))MEMIF_SW_MAJOR_VERSION;
    	VersionInfoPtr->sw_minor_version = (VAR(uint8, AUTOMATIC))MEMIF_SW_MINOR_VERSION;
    	VersionInfoPtr->sw_patch_version = (VAR(uint8, AUTOMATIC))MEMIF_SW_PATCH_VERSION;
    }
	/* ----- Development Error Report --------*/
#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
	if(errorId != (uint8)MEMIF_E_NO_ERROR)
	{
		(void)Det_ReportError(MEMIF_MODULE_ID, 0U, MEMIF_SID_VERSION_INFO, errorId);
	}
#else
	(void)errorId;
#endif
    return;
}
#endif

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

#if (MEMIF_DEV_ERROR_DETECT == STD_ON)
static FUNC(Std_ReturnType, MEMIF_CODE) MemIf_DetChkDeviceIndex(uint8 DeviceIndex)
{
	Std_ReturnType retVal;
	retVal = (Std_ReturnType)((DeviceIndex >= MemIf_NumberOfDevices_u8) ? E_NOT_OK : E_OK);
	return (retVal);
}
#endif

static FUNC(Std_ReturnType, MEMIF_CODE) MemIf_IsBitSet(uint8 Mask, MemIf_StatusType Status)
{
	Std_ReturnType retVal;
	retVal = (Std_ReturnType)(((Mask & (1U << (uint8)Status)) > 0U) ? E_OK : E_NOT_OK);
	return (retVal);
}

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"
