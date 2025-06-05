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
**  FILENAME    : NvM_Cfg.h                                                   **
**                                                                            **
**  Created on  : 2022/11/09 09:19:13                                 **
**  Author      : <>                                                          **
**  Vendor      :                                                             **
**  DESCRIPTION : Pre-compile configuration parameter of NvM                  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef NVM_CFG_H
#define NVM_CFG_H
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#define NVM_CFG_H_AR_MAJOR_VERSION 4U
#define NVM_CFG_H_AR_MINOR_VERSION 2U
#define NVM_CFG_H_AR_PATCH_VERSION 2U

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define NVM_API_CONFIG_CLASS                            NVM_API_CONFIG_CLASS_3
#define NVM_BSWM_MULTIBLOCK_JOBSTATUS_INFORMATION        STD_OFF
#define NVM_BSWM_SINGLEBLOCK_JOBSTATUS_INFORMATION      STD_OFF
#define NVM_COMPILED_CONFIG_ID                          (0x0u)
#define NVM_CRC_NUM_OF_BYTES                              (0xffffu)
#define NVM_DATASET_SELECTION_BITS                      (0x4u)
#define NVM_DEV_ERROR_DETECT                            STD_ON
#define NVM_DRV_MODE_SWITCH                                 STD_ON
#define NVM_DYNAMIC_CONFIGURATION                         STD_ON
#define NVM_JOB_PRIORITIZATION                            STD_ON
#define NVM_POLLING_MODE                                STD_OFF
#define NVM_REPEAT_MIRROR_OPERATIONS                      (0x3u)
#define NVM_SET_RAM_BLOCK_STATUS_API                    STD_ON
#define NVM_SIZE_IMMEDIATE_JOB_QUEUE                    (0x3u)
#define NVM_SIZE_STANDARD_JOB_QUEUE                        (0x5u)
#define NVM_VERSION_INFO_API                            STD_ON
#define NVM_INCLUDE_CRC                                 STD_ON
#define NVM_CRC_TYPE_8                                  STD_OFF
#define NVM_CRC_TYPE_16                                 STD_ON
#define NVM_CRC_TYPE_32                                 STD_OFF
#define NVM_DEM_PRODUCTION_ERROR_DETECT                    STD_OFF


/*The following definitions used to index NVM block for SW */
#define NvMBlock1               2U
#define NvMBlock2               3U
#define NvMBlock3               4U
#define NvMBlock_All_EventEntry               5U
#define NvMBlock_UDS_InternalData               6U
#define NvMBlock_Dcm_Roe               7U

/*The total number of user configured*/
#define NVM_BLOCK_NUM_ALL                                 7
#define NVM_REDUNDANT_ALL                                  2

#define NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR                0U

#define NVM_MAX_LENGTH_NV_BLOCK                            2048U
#define NVM_MAX_LENGTH_REDUNDANT_BLOCK                    20U

#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON == NVM_JOB_PRIORITIZATION))
/*The total number of different priority*/
#define NVM_TOTAL_NUM_DIFF_PRI                             NVM_TABLE_SIZE_PRIORITY
#endif

#if(STD_ON==NVM_JOB_PRIORITIZATION)
/* The priority table size*/
#define NVM_TABLE_SIZE_PRIORITY                          5U
#endif

#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer1[2];
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer2[20];
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer3[20];
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer4[20];
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer5[2048];
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer6[200];
extern VAR(uint8, NVM_VAR_NOINIT) NvMBlockRamBuffer7[128];
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

#endif /* End of NVM_CFG_H*/
/*******************************************************************************
**                           End Of File                                      **
*******************************************************************************/

