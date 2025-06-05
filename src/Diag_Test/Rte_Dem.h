/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dem.h>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:48>
 */
/*============================================================================*/

#ifndef RTEDEM_H
#define RTEDEM_H

#include "Dem_Types.h"

extern Std_ReturnType  Rte_ReadData_0x1F( uint8* Buffer );
extern Std_ReturnType Rte_DemCallbackDTCStatusChanged(
	    uint32 DTC,
	    Dem_UdsStatusByteType DTCStatusOld,
	    Dem_UdsStatusByteType DTCStatusNew);

#endif /*RTEDEM_H*/
