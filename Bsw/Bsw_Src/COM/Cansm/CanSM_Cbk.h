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
**  FILENAME    : CanSM_Cbk.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : A list of functions provided for other modules              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/




#ifndef CANSM_CBK_H
#define CANSM_CBK_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "CanIf.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/




/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/



/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/



/*******************************************************************************
**                      Global Functions                                      **
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
);
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
);
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"



#endif /* CANSM_CBK_H */
