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
**  FILENAME    : MemIf_Cfg.c                                                 **
**                                                                            **
**  Created on  : 2021/12/28 09:45:58                                         **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : MemIf pre compile globle macro define                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11/4.2.2                **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>   	 <AUTHOR>   	 <REVISION LOG>
 *  V1.0.0    20200819    Haibin.Shao     Initial version.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "MemIf.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#define MEMIF_START_SEC_CONST_8BIT
#include "MemIf_MemMap.h"
CONST(uint8, MEMIF_CONST) MemIf_NumberOfDevices_u8 = MEMIF_NUMBER_OF_DEVICES;
#define MEMIF_STOP_SEC_CONST_8BIT
#include "MemIf_MemMap.h"

#define MEMIF_START_SEC_CONST_32BIT
#include "MemIf_MemMap.h"
CONST(MemIf_MemHwAApi_Type, MEMIF_CONST) MemIf_MemHwaApis_at[MEMIF_NUMBER_OF_DEVICES] =
{
	/*<MEMIF_FEE_API/>*/
	{
		/* Fee */
		Fee_SetMode,
		Fee_Read,
		Fee_Write,
		Fee_EraseImmediateBlock,
		Fee_InvalidateBlock,
		Fee_Cancel,
		Fee_GetStatus,
		Fee_GetJobResult	
	}
};
#define MEMIF_STOP_SEC_CONST_32BIT
#include "MemIf_MemMap.h"
