/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Data.h>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:16>
 */
/*============================================================================*/

/* Rte_Data.h */

#ifndef RTE_DATA_H
#define RTE_DATA_H

/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
#include "Rte_Type.h"

static inline void Rte_DataInit(void * data,uint32 Length)
{
	uint32 i;
	uint8* pBuf = (uint8*)data;
	for (i = 0; i < Length; i++) 
	{
		pBuf[i] = 0;
	}
}

/*******************************************************************************
 **                        Global Variable Declarations                       **
******************************************************************************/

extern UInt8 Runnable_Step_HMIHighBeam_IN_HMIHighBeam_HMIHighBeam_read_buffer;

extern UInt8 Runnable_Step_HMILowBeam_IN_HMILowBeam_HMILowBeam_read_buffer;

extern UInt8 Runnable_Step_HMIPositionLamp_IN_HMIPositionLamp_HMIPositionLamp_read_buffer;

#define RTE_START_SEC_VAR_NO_INIT_8
#include "RTE_MemMap.h"


extern Std_ReturnType Rte_Status_lowBeam_IPDU_COM_LedStatus;

extern Std_ReturnType Rte_Status_CmdLowBeam_IPDU_COM_CmdSend;

extern Std_ReturnType Rte_Status_PositionLamp_IPDU_COM_LedStatus;

extern Std_ReturnType Rte_Status_HighBeam_IPDU_COM_LedStatus;

extern Std_ReturnType Rte_Status_CmdHighBeam_IPDU_COM_CmdSend;

#define RTE_STOP_SEC_VAR_NO_INIT_8
#include "RTE_MemMap.h"


void rte_memcpy(uint8 * pDest, const uint8 * pSrc, uint32 length);

/*******************************************************************************
 **                        Global Function Declarations                       **
******************************************************************************/



#endif

