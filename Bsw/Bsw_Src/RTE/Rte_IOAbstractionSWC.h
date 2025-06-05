/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_IOAbstractionSWC.h>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_IOAbstractionSWC.h */

#ifndef RTE_IOABSTRACTIONSWC_H
#define RTE_IOABSTRACTIONSWC_H

/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
#include "Rte_DataHandleType.h"
#include "Rte_IOAbstractionSWC_Type.h"

/*******************************************************************************
 **                        Macro Definitions                                  **
******************************************************************************/

typedef struct 
{
	uint8 Rte_Dummy;
} Rte_CDS_IOAbstractionSWC;



/** API mappings */

/*******************************************************************************
 **                        Global Function Declarations                       **
******************************************************************************/

/** API functions */

/** runnables */

#define IOAbstractionSWC_START_SEC_CODE
#include "IOAbstractionSWC_MemMap.h"


extern FUNC(void, IOAbstractionSWC_CODE)  IOAbstractionSWC(CONSTP2VAR(UInt8, AUTOMATIC, RTE_APPL_DATA) DE_LedState);

#define IOAbstractionSWC_STOP_SEC_CODE
#include "IOAbstractionSWC_MemMap.h"




#endif

