
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
**  FILENAME    : ComM_Cfg.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
#ifndef COMM_CFG_H
#define COMM_CFG_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define COMM_CFG_H_AR_RELEASE_MAJOR_VERSION  19u
#define COMM_CFG_H_AR_RELEASE_MINOR_VERSION  11u
#define COMM_CFG_H_AR_RELEASE_PATCH_VERSION  0u
#define COMM_CFG_H_SW_MAJOR_VERSION          1u
#define COMM_CFG_H_SW_MINOR_VERSION          0u
#define COMM_CFG_H_SW_PATCH_VERSION          0u

/*
 * ComMGeneral
 */
/* Whether the CAN bus is supported or not.*/
#define COMM_BUS_CAN_USED                  STD_OFF

/* Whether the LIN bus is supported or not.*/
#define COMM_BUS_LIN_USED                  STD_OFF

/* Whether the FlexRay bus is supported or not.*/
#define COMM_BUS_FLEXRAY_USED              STD_OFF

/* Whether the ETH bus is supported or not.*/
#define COMM_BUS_ETH_USED                  STD_ON

/* Whether the CDD bus is supported or not.*/
#define COMM_BUS_CDD_USED                  STD_OFF

/* Whether the NM is supported or not.*/
#define COMM_NM_USED                       STD_OFF

/* Whether the none volatile memory is supported or not.*/
#define COMM_NVM_USED                       STD_OFF

/* Whether the DCM is supported or not.*/
#define COMM_DCM_USED                       STD_OFF

/* Whether the ECUM is supported or not.*/
#define COMM_ECUM_USED                      STD_ON

/* Whether the RTE notification is supported or not.*/
#define COMM_RTE_NOTIFY_USED                STD_ON

/* Whether the BSWM notification is supported or not.*/
#define COMM_BSWM_USED                      STD_OFF

/* Whether the COM notification is supported or not.*/
#define COMM_COM_USED                       STD_ON


/*This parameter avoids sending of 0-PNC-Vectors*/
#define COMM_0PNC_VECTOR_AVOIDANCE            STD_OFF  
/* Switches the Development Error Detection and Notification ON or OFF.*/
/**@req ComM555*/
#define COMM_DEV_ERROR_DETECT               STD_ON  /*ComMDevErrorDetect*/
#define COMM_DIRECT_USER_MAPPING            STD_OFF  /*ComMDirectUserMapping*/

/* Switches the Porduct Error Detection ON or OFF.*/
#define COMM_DEM_ERROR_DETECT               STD_ON  

/* Defines whether a mode inhibition affects the ECU or not.*/
/**@req ComM563*/
#define COMM_ECU_GROUP_CLASSIFICATION       0x3u /*ComMEcuGroupClassification*/

/* True if mode limitation functionality shall be enabled. true:Enabled false: Disabled*/
/**@req ComM560*/
#define COMM_MODE_LIMITATION_ENABLED        STD_OFF  /*ComMModeLimitationEnabled*/
#define COMM_NM_PASSIVE_MODE_ENABLED        STD_OFF  /*ComMNmPassiveModeEnable*/

/* True if wake up inhibition functionality enabled.*/
/**@req COMM-CFG-010[ComM559]*/
#define COMM_WAKEUP_INHIBITION_ENABLED      STD_OFF   /*ComMWakeupInhibitionEnabled*/

/* The ECU is not allowed to change state of the ECU to "Silent Communication" or 
 *"Full Communication".*/
/**@req COMM-CFG-005[ComM561]*/
#define COMM_NO_COM                         STD_OFF

/* ComM shall perform a reset after entering "No Communication" mode because of an active mode 
 * limitation to "No Communication" mode.*/
/**@req COMM-CFG-006[ComM558]*/
#define COMM_RESET_AFTER_FORCING_NO_COMM     STD_OFF  /*ComMResetAfterForcingNoComm*/

/* Wake up of one channel shall lead to a wake up of all channels if true.*/
/**@req COMM-CFG-007[ComM695]*/
#define COMM_SYNCHRONOUS_WAKE_UP            STD_ON  /*ComMSynchronousWakeUp*/

/* Minimum time duration in seconds, spent in the Full Communication mode.*/
/**@req COMM-CFG-008[ComM557]*/
#define COMM_T_MIN_FULL_COM_MODE_DURATION   5000u /*ComMTMinFullComModeDuration*/

/* Switches the possibility to read the published information with the service.*/
/**@req COMM-CFG-009[ComM622]*/
#define COMM_VERSION_INFO_API               STD_OFF /*ComMVersionInfoApi*/

/* Reference to NvmBlockDescriptor.*/
/**@req COMM-CFG-011[ComM783]*/
#define COMM_GLOBAL_NVM_BLOCK_DESCRIPTOR    0x0u /*ComMGlobalNvMBlockDescriptor*/

#define COMM_PNC_GATEWAY_ENABLED            STD_OFF   /*ComMPncGatewayEnabled*/
#define COMM_PNC_PREPARE_SLEEP_TIMER        0x0u   /*ComMPncPrepareSleepTimer*/

#define COMM_PNC_SUPPORT                    STD_OFF  /*ComMPncSupport*/

/*
 * Additional Configuration
 */
 
 #define COMM_PNC_VECTOR_OFFSET              2u 
 
/* Number of the PNCs*/
#define COMM_NUMBER_OF_PNCS                 0u    
  
#define COMM_NUMBER_OF_PNC_SIGNALS          0u   

/* Number of the channels */
#define COMM_NUMBER_OF_CHANNELS             9u
#define ComMChannel_0      0u
#define ComMChannel_1      1u
#define ComMChannel_2      2u
#define ComMChannel_3      3u
#define ComMChannel_Eth      4u
#define ComMChannel_4      5u
#define ComMChannel_5      6u
#define ComMChannel_6      7u
#define ComMChannel_7      8u 

/* Number of the user */
#define COMM_NUMBER_OF_USERS                3u  
#define ComMUser_baseCan       0u
#define ComMUser_Eth       1u
#define ComMUser_ExtCan       2u

/* Number of the TxSingnal */
#define COMM_NUMBER_OF_TXSIGNAL                0u

#endif /*#ifndef COMM_CFG_H*/

