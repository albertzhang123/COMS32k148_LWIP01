/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Data.c>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_Data.c */
/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
//#include "Os.h"
#include "Rte_Data.h"

/*******************************************************************************
 **                        Global Variable Definitions                        **
******************************************************************************/

UInt8 Runnable_Step_HMIHighBeam_IN_HMIHighBeam_HMIHighBeam_read_buffer;

UInt8 Runnable_Step_HMILowBeam_IN_HMILowBeam_HMILowBeam_read_buffer;

UInt8 Runnable_Step_HMIPositionLamp_IN_HMIPositionLamp_HMIPositionLamp_read_buffer;

#define RTE_START_SEC_VAR_NO_INIT_8
#include "RTE_MemMap.h"


Std_ReturnType Rte_Status_lowBeam_IPDU_COM_LedStatus = RTE_E_OK;

Std_ReturnType Rte_Status_CmdLowBeam_IPDU_COM_CmdSend = RTE_E_OK;

Std_ReturnType Rte_Status_PositionLamp_IPDU_COM_LedStatus = RTE_E_OK;

Std_ReturnType Rte_Status_HighBeam_IPDU_COM_LedStatus = RTE_E_OK;

Std_ReturnType Rte_Status_CmdHighBeam_IPDU_COM_CmdSend = RTE_E_OK;

#define RTE_STOP_SEC_VAR_NO_INIT_8
#include "RTE_MemMap.h"


void rte_memcpy(uint8 * pDest, const uint8 * pSrc, uint32 length)
{
	while (length--) 
	{
		*pDest++ = *pSrc++;
	}
}


