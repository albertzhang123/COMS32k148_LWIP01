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
**  FILENAME    : EthIf_Cfg.h                                                 **                                        
**                                                                            **                                        
**  Created on  : 2023/05/26 17:57:39                                         **                                        
**  Author      :                                                             **                                        
**  Vendor      :                                                             **                                        
**  DESCRIPTION : EthIf pre compile globle macro define                       **                                        
**                                                                            **                                        
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **                                        
**                                                                            **                                        
*******************************************************************************/                                        

#ifndef ETHIF_CFG_H
#define ETHIF_CFG_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "EthIf_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ETHIF_VLAN_USED        STD_OFF

#define ETHIF_CTRL_NUM          1u

#define ETHIF_MAX_FRAME_OWNER_CFG            2u


/* Switches the Development Error Detection ON or OFF */
#define ETHIF_DEV_ERROR_DETECT                         STD_ON

/* Enables / Disables receive interrupt.   */
#define ETHIF_ENABLE_RX_INTERRUPT                      STD_ON

/* Enables / Disables the transmit interrupt.  */
#define  ETHIF_ENABLE_TX_INTERRUPT                     STD_OFF

/* Enables / Disables GetBaudRate API */
#define  ETHIF_GET_BAUDRATE                            STD_OFF

/* Enables / Disables GetCounterState API. */
#define  ETHIF_GET_COUNTERSTATE                        STD_OFF

/* Enables / Disables ETHIf_GetTransceiverWakeupMode API */

/* Enables/Disables the Global Time APIs used amongst others by Global Time Synchronization over ETHernet. */
#define  ETHIF_GLOBAL_TIME_SUPPORT                       STD_ON 

/* Specifies the period of main function ETHIf_MainFunctionRx and ETHIf_MainFunctionTx in seconds.
 * ETHernet Interface does not require this information but the BSW scheduler. Multiplicity 1 */
#define  ETHIF_MAIN_FUNCTION_PERIOD                       10u  

/* Specifies the period of main function EthIf_MainFunctionState in seconds.
 * ETHernet Interface does not require this information but the BSW scheduler. Multiplicity 1 */
#define  ETHIF_MAIN_FUNCTION_STATE_PERIOD               10u   

/*This parameter is deprecated and will be removed in future. Old description:
 * Timeout in seconds after which the ETHIf stops to receive frames in an ETHIfMainFunctionRx period. */

/* Limits the total number of transceivers. */
#define  ETHIF_MAX_TRCVS_TOTAL                         1u

/* Maximum number of ETHernet frames per ETHernet controller polled from  the
 * ETHernet driver within ETHIf_MainFunctionRx. */
#define  ETHIF_RX_INDICATION_ITERATIONS_MAX              0u      
/* Enables / Disables StartAutoNegotiation API. */
#define  ETHIFSTARTAUTONEGOTIATION                    STD_OFF 
 

/* Specifies the frequency of transceiver link state change checks in each period of main function ETHIf_MainFunctionTx.*/
#define  ETHIFTRCVLINKSTATECHGMAINRELOAD              1u

/* Enables / Disables version info API  */
#define  ETHIF_VERSION_INFO_API                          STD_OFF 

/* Enables / Disables version info API macro implementation. */
#define  ETHIF_VERSION_INFO_API_MACRO                  STD_OFF 

/* Configures if wakeup is supported or not. */
#define  ETHIF_WAKEUP_SUPPORT                          STD_OFF 

/* Configures if get contrller list is supported or not. */
#define ETH_GET_CTRLIDX_LIST                          STD_OFF

/*Enables / Disables ETHTRCV*/
#define ETHIF_USE_ETHTRCV                              STD_OFF

/* The eth driver api number used*/
#define ETHIF_ETH_DRIVER_API_NUM    1u

/* The ethTrcv driver api number used*/
#define ETHIF_ETHTRCV_DRIVER_API_NUM    0u

#endif /* ETHIF_CFG_H */

