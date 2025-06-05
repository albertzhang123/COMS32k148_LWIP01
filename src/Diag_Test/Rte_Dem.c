/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dem.c>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:48>
 */
/*============================================================================*/


#include "Rte_Dem.h"

Std_ReturnType  Rte_ReadData_0x1F( uint8* Buffer )
{
	*Buffer = 0x77;
    return E_OK;
};
Std_ReturnType Rte_DemCallbackDTCStatusChanged(
	uint32 DTC,
	Dem_UdsStatusByteType DTCStatusOld,
	Dem_UdsStatusByteType DTCStatusNew)
{
	(void)DTC;
	(void)DTCStatusOld;
	(void)DTCStatusNew;
	return E_OK;
}

