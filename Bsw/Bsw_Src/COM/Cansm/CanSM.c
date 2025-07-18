/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2016)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : CanSM.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation of CANSM module                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CanIf.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define CANSM_C_AR_MAJOR_VERSION    4U
#define CANSM_C_AR_MINOR_VERSION    2U
#define CANSM_C_AR_PATCH_VERSION    2U
#define CANSM_C_SW_MAJOR_VERSION    1U
#define CANSM_C_SW_MINOR_VERSION    0U
#define CANSM_C_SW_PATCH_VERSION    0U

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
/**
 * This callback function notifies the CanSM about a bus-off event on a certain
 * CAN controller, which needs to be considered with the specified bus-off
 * recovery handling for the impacted CAN network.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant (only for different CanControllers)
 * Parameters(IN): ControllerId, CAN controller, which detected a bus-off event
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, CANSM_CONTROLLERBUSOFF_CODE)
CanSM_ControllerBusOff(
    uint8 ControllerId
)
{
	CanIf_SetControllerMode(ControllerId,CANIF_CS_STARTED);
	CanIf_SetPduMode(ControllerId,CANIF_ONLINE);
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
/**
 * This callback shall notify the CanSM module about a CAN controller mode change.
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant (only for different CanControllers)
 * Parameters(IN): ControllerId, CAN controller, whose mode has changed
 *                 ControllerMode, Notified CAN controller mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, CANSM_CONTROLLERNODEINDICATION_CODE)
CanSM_ControllerModeIndication(
    uint8 ControllerId,
    CanIf_ControllerModeType ControllerMode
)
{
    return;
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

/*******************************************************************************
**                            General Notes                                   **
*******************************************************************************/

