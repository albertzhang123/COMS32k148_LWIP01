/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dcm_Callout.c>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:44>
 */
/*============================================================================*/

#include "Dcm_Internal.h"
#include "UDS.h"
#include "DcmDsl_MsgManage.h"
#include "Dcm_CalloutBoot.h"

#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 MemoryBuffer_0x100000[0x200] = {0x20} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

#define  DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
uint8 MemoryBuffer_0x0[0x200] = {0x20} ;
#define  DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

/* Remained Upload Length for 0x35, 0x36, 0x37 */
#define DCM_START_SEC_VAR_NOINIT_32
#include "Dcm_MemMap.h"
uint32 DcmUploadSizeRemain;
uint32 DcmLastUploadSize;
#define  DCM_STOP_SEC_VAR_NOINIT_32
#include "Dcm_MemMap.h"

/* Remained Download Length for 0x34, 0x36, 0x37 */
#define DCM_START_SEC_VAR_NOINIT_32
#include "Dcm_MemMap.h"
uint32 DcmDownloadSizeRemain;
uint32 DcmLastDownloadSize;
#define  DCM_STOP_SEC_VAR_NOINIT_32
#include "Dcm_MemMap.h"

/* BL_AppFlagType* BL_AppFlag = (BL_AppFlagType*) BL_APP_FLAG_ADDRESS; */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_SetProgConditions(
Dcm_OpStatusType OpStatus,
Dcm_ProgConditionsType * ProgConditions
)
{
    /*TODO：Set ReProgramingRequest Flag*/
	/*e.g*/
	/*	BL_AppFlag->FlBootMode = (uint32)FL_EXT_PROG_REQUEST_RECEIVED;
	BL_AppFlag->Busmode = BUS_MODE_ETH;*/
    return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Dcm_EcuStartModeType Dcm_GetProgConditions(
Dcm_ProgConditionsType * ProgConditions
)
{
	/*TODO：check ApplUpdated*/
	/*e.g*/
	/*	if (BL_AppFlag->FlApplUpdate == (uint32)FL_APPL_UPDATED)
	{
		ProgConditions->ApplUpdated = TRUE;
		BL_AppFlag->FlApplUpdate = (uint32)0;
		return DCM_WARM_START;
	}*/
    return DCM_COLD_START;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*for 0x23 service to read data by memory address*/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Dcm_ReturnReadMemoryType Dcm_ReadMemory(Dcm_OpStatusType OpStatus,
		uint8 MemoryIdentifier,
		uint32 MemoryAddress,
		uint32* MemorySize,
		uint8* MemoryData,
		Dcm_NegativeResponseCodeType* ErrorCode)
{
	Dcm_ReturnReadMemoryType ret = DCM_READ_OK;
	uint32 tempRemainSize = DcmUploadSizeRemain;
	uint8 tempBlockId;
	uint8 index;
	uint8 counter = 0;
	uint32 actualSize = 0;

	/* check if in process of 35, 36, 37 */
	if (DCM_UDS0X36_TO_UPLOAD == Dcm_DspProgram.Status)
	{
		tempBlockId = Dcm_DspProgram.blockId;
		if (0xFF == tempBlockId)
		{
			tempBlockId = 0u;
		}
		else
		{
			tempBlockId++;
		}

		if ((Dcm_DspProgram.blockId == Dcm_DspProgram.reqBlockId) &&
			(Dcm_DspProgram.FirstFlag == FALSE))
		{
			tempRemainSize += DcmLastUploadSize;
		}
		/* Uploaed Process is over, send NRC 0x24 */
		if (0u == tempRemainSize)
		{
			*ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
			ret = DCM_READ_FAILED;
		}
		else if (((tempBlockId != Dcm_DspProgram.reqBlockId) &&
				(Dcm_DspProgram.FirstFlag == TRUE)) ||
				((tempBlockId != Dcm_DspProgram.reqBlockId) &&
				(Dcm_DspProgram.blockId != Dcm_DspProgram.reqBlockId) &&
				(Dcm_DspProgram.FirstFlag == FALSE)))
		{
			*ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
			ret = DCM_READ_FAILED;
		}
		else if (tempRemainSize > (Dcm_DspProgram.BlockLength-2))
		{
			actualSize = Dcm_DspProgram.BlockLength-2;
		}
		else
		{
			actualSize = tempRemainSize;
		}
		if (DCM_READ_OK == ret)
		{
			tempRemainSize -= actualSize;
			DcmUploadSizeRemain = tempRemainSize;
			DcmLastUploadSize = actualSize;
	        *MemorySize = actualSize+2;
		}
	}
	else
	{
		actualSize = *MemorySize;
	}

	if ((ret == DCM_READ_OK) &&
		(MemoryAddress <= 0x100100) &&
		(MemoryAddress >= 0x100000) &&
		((MemoryAddress - 0x100000 + actualSize) <= 0x100))
	{
		for (index = (MemoryAddress - 0x100000); counter < actualSize; index++)
		{
			MemoryData[counter] = MemoryBuffer_0x100000[index];
			counter++;
		}
	}
	else if ((ret == DCM_READ_OK) &&
			(MemoryAddress <= 0x000100) &&
			((MemoryAddress + actualSize) <= 0x100))
	{
		for (index = MemoryAddress; counter < actualSize; index++)
		{
			MemoryData[counter] = MemoryBuffer_0x0[index];
			counter++;
		}
	}

    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*for 0x3D service to write data by memory address*/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Dcm_ReturnWriteMemoryType Dcm_WriteMemory(Dcm_OpStatusType OpStatus,
                                            uint8 MemoryIdentifier,
                                            uint32 MemoryAddress,
                                            uint32 MemorySize,
                                            uint8* MemoryData,
                                            Dcm_NegativeResponseCodeType* ErrorCode)
{
	Dcm_ReturnReadMemoryType ret = DCM_READ_FAILED;
	boolean sameBlockId = FALSE;
	uint8 tempBlockId;
	uint8 index;
	uint8 counter = 0;

	/* check if in process of 34, 36, 37 */
	if (DCM_UDS0X36_TO_DOWNLOAD == Dcm_DspProgram.Status)
	{
		tempBlockId = Dcm_DspProgram.blockId;
		if (0xFF == tempBlockId)
		{
			tempBlockId = 0u;
		}
		else
		{
			tempBlockId++;
		}

		if ((Dcm_DspProgram.blockId == Dcm_DspProgram.reqBlockId) &&
			(Dcm_DspProgram.FirstFlag == FALSE))
		{
			sameBlockId = TRUE;
		}
		/* Download Process is over, send NRC 0x24 */
		if ((0u == DcmDownloadSizeRemain) &&
			(FALSE == sameBlockId))
		{
			*ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
		}
		/* Downloaed Data Size goes over Required Length, send NRC 0x71 */
		else if (((MemorySize > DcmDownloadSizeRemain) &&
				(0u != DcmDownloadSizeRemain)) ||
				((0u == DcmDownloadSizeRemain) &&
				(MemorySize > DcmLastDownloadSize)))
		{
			*ErrorCode = DCM_E_TRANSFERDATASUSPENDED;
		}
		else if (((tempBlockId != Dcm_DspProgram.reqBlockId) &&
				(Dcm_DspProgram.FirstFlag == TRUE)) ||
				((tempBlockId != Dcm_DspProgram.reqBlockId) &&
				(Dcm_DspProgram.blockId != Dcm_DspProgram.reqBlockId) &&
				(Dcm_DspProgram.FirstFlag == FALSE)))
		{
			*ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
		}
		else if ((MemorySize+2) > Dcm_DspProgram.BlockLength)
		{
			*ErrorCode = DCM_E_REQUESTOUTOFRANGE;
		}
		else if (FALSE == sameBlockId)
		{
			DcmDownloadSizeRemain -= MemorySize;
			DcmLastDownloadSize = MemorySize;
			ret = DCM_WRITE_OK;
		}
		else
		{
			ret = DCM_WRITE_OK;
		}
	}
	else
	{
		ret = DCM_WRITE_OK;
	}

	if ((DCM_WRITE_OK == ret) &&
		(FALSE == sameBlockId) &&
		(MemoryAddress <= 0x100100) &&
		(MemoryAddress >= 0x100000) &&
		((MemoryAddress - 0x100000 + MemorySize) <= 0x100))
	{
		for (index = (MemoryAddress - 0x100000); counter < MemorySize; index++)
		{
			MemoryBuffer_0x100000[index] = MemoryData[counter];
			counter++;
		}
	}
	else if ((DCM_WRITE_OK == ret) &&
			(FALSE == sameBlockId) &&
			(MemoryAddress <= 0x000100) &&
			((MemoryAddress + MemorySize) <= 0x100))
	{
		for (index = MemoryAddress; counter < MemorySize; index++)
		{
			MemoryBuffer_0x0[index] = MemoryData[counter];
			counter++;
		}
	}

    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*for 0x34 service to request download*/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_ProcessRequestDownload(Dcm_OpStatusType OpStatus,
                                            uint8 DataFormatIdentifier,
                                            uint32 MemoryAddress,
                                            uint32 MemorySize,
                                            uint32* BlockLength,
                                            Dcm_NegativeResponseCodeType* ErrorCode)
{
	DcmDownloadSizeRemain = MemorySize;
	DcmLastDownloadSize = 0;
	*BlockLength = 0x00000400;
    return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*for 0x35 service to request upload*/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_ProcessRequestUpload(Dcm_OpStatusType OpStatus,
                                        uint8 DataFormatIdentifier,
                                        uint32 MemoryAddress,
                                        uint32 MemorySize,
                                        uint32* BlockLength,
                                        Dcm_NegativeResponseCodeType* ErrorCode)
{
	DcmUploadSizeRemain = MemorySize;
	DcmLastUploadSize = 0;
	*BlockLength = 0x00000400;
    return E_OK;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*for 0x37 service to request transfer exit*/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_ProcessRequestTransferExit(Dcm_OpStatusType OpStatus,
                                                uint8* transferRequestParameterRecord,
                                                uint32 transferRequestParameterRecordSize,
                                                uint8* transferResponseParameterRecord,
                                                uint32* transferResponseParameterRecordSize,
                                                Dcm_NegativeResponseCodeType* ErrorCode)
{
	Std_ReturnType ret = E_OK;

	if (DCM_UDS0X36_TO_DOWNLOAD == Dcm_DspProgram.Status)
	{	
		/* process not finished yet, send NRC 0x24 */
		if (DcmDownloadSizeRemain != 0)
		{
			*ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
			ret = E_NOT_OK;
		}
	}
	else if (DCM_UDS0X36_TO_UPLOAD == Dcm_DspProgram.Status)
	{
		/* process not finished yet, send NRC 0x24 */
		if (DcmUploadSizeRemain != 0)
		{
			*ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
			ret = E_NOT_OK;
		}
	}
	else
	{
		/* Not in progress of any, send NRC 0x24 */
		*ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
		ret = E_NOT_OK;
	}

	if ((E_OK == ret) &&
		(0u != transferRequestParameterRecordSize))
	{
		*ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
		ret = E_NOT_OK;
	}

    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/*for 0x38 service to request file transfer*/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_ProcessRequestFileTransfer(Dcm_OpStatusType OpStatus,
                                                uint8 modeofOperation,
                                                uint16 fileSizeParameterLength,
                                                uint8* filePathAndName,
                                                uint8 dataFormatIdentifier,
                                                uint8* fileSizeUncompressedOrDirInfoLength,
                                                uint8* fileSizeCompressed,
                                                uint32* BlockLength,
                                                Dcm_NegativeResponseCodeType* ErrorCode)
{
	Std_ReturnType ret = E_OK;
	uint16 index;
	uint64 SizeUncompressed = 0;
	uint64 SizeCompressed = 0;

	if ((0x01 == modeofOperation) || (0x03 == modeofOperation))
	{
		for (index=0; index<fileSizeParameterLength; index++)
		{
			SizeUncompressed = SizeUncompressed << 8u;
            SizeUncompressed = SizeUncompressed | fileSizeUncompressedOrDirInfoLength[index];
		}
		for (index=0; index<fileSizeParameterLength; index++)
		{
			SizeCompressed = SizeCompressed << 8u;
            SizeCompressed = SizeCompressed | fileSizeCompressed[index];
		}
	}

	if ((SizeUncompressed >= 0x100002) ||
	    (SizeCompressed >= 0x100002))
	{
		*ErrorCode = DCM_E_REQUESTOUTOFRANGE;
		ret = E_NOT_OK;
	}

	if ((0x04 == modeofOperation) || (0x05 == modeofOperation))
	{
		*BlockLength = 0x400;
	}
	else if ((0x01 == modeofOperation) || (0x03 == modeofOperation))
	{
		*BlockLength = 0x400;
	}
	
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
Com_IpduGroupVector ipduGroupVector;
void Rte_DcmControlCommunicationMode(NetworkHandleType DcmDspComMChannelId,Dcm_CommunicationModeType RequestedMode)
{

	switch(RequestedMode)
	{
		case DCM_ENABLE_RX_TX_NORM:

		/*	Com_SetIpduGroup(ipduGroupVector, Com_RxPduGroup_CONTROLLER_0 ,TRUE);
			Com_SetIpduGroup(ipduGroupVector, Com_TxPduGroup_CONTROLLER_0 ,TRUE);
			Com_IpduGroupControl(ipduGroupVector,TRUE);*/
			//Nm_DisableCommunication(DcmDspComMChannelId);
		break;
		case DCM_DISABLE_RX_TX_NM:
			//Nm_DisableCommunication(DcmDspComMChannelId);
		break;
		case DCM_ENABLE_RX_TX_NM:
			//Nm_EnableCommunication(DcmDspComMChannelId);
		break;
		case DCM_ENABLE_RX_TX_NORM_NM:
			Com_SetIpduGroup(ipduGroupVector, Com_PduGroup_CONTROLLER_0 ,TRUE);
			//Com_SetIpduGroup(ipduGroupVector, Com_PduGroup_CONTROLLER_0 ,TRUE);
			Com_IpduGroupControl(ipduGroupVector,TRUE);
			//Nm_EnableCommunication(DcmDspComMChannelId);
		break;
		case DCM_DISABLE_RX_TX_NORMAL:
			/*Com_SetIpduGroup(ipduGroupVector, Com_RxPduGroup_CONTROLLER_0 ,FALSE);
			Com_SetIpduGroup(ipduGroupVector, Com_TxPduGroup_CONTROLLER_0 ,FALSE);
			Com_IpduGroupControl(ipduGroupVector,FALSE);*/
		break;
		case DCM_DISABLE_RX_TX_NORM_NM:
			/*Com_SetIpduGroup(ipduGroupVector, Com_RxPduGroup_CONTROLLER_0 ,FALSE);
			Com_SetIpduGroup(ipduGroupVector, Com_TxPduGroup_CONTROLLER_0 ,FALSE);
			Com_IpduGroupControl(ipduGroupVector,FALSE);*/
			//Nm_DisableCommunication(DcmDspComMChannelId);
		break;
		case DCM_ENABLE_RX_DISABLE_TX_NORM:
		break;
		case DCM_ENABLE_RX_DISABLE_TX_NM:
		break;
		case DCM_ENABLE_RX_DISABLE_TX_NORM_NM:
		break;
		case DCM_DISABLE_RX_ENABLE_TX_NORM:
		break;
		case DCM_DISABLE_RX_ENABLE_TX_NM:
		break;
		case DCM_DISABLE_RX_ENABLE_TX_NORM_NM:
		break;
		default:
		break;
	}
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
void Rte_EnableAllDtcsRecord(void)
{
   /*The update of the DTC status bit information shall continue once a ControlDTCSetting request is performed
     with sub-function set to on or a session layer timeout occurs (server transitions to defaultSession. */
    /* (void)Dem_EnableDTCStorage(DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_KIND_ALL_DTCS);*/
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"


