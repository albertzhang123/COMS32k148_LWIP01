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
**  FILENAME    : Ea_Lcfg.h                                                        **
**                                                                            **
**  Created on  : 2020/08/19                                                  **
**  Author      : Haibin.Shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Dlt header description                             		  **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>   	 <AUTHOR>   	 <REVISION LOG>
 *  V1.0.0     2020-08-19   Haibin.Shao   R19_11 Ea initial version.
 */
#ifndef _MEMIF_TYPES_H_
#define _MEMIF_TYPES_H_

/*==============================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "Std_Types.h"

/*==============================================================================
*                              SOURCE FILE VERSION INFORMATION
==============================================================================*/
/*---- Version identification ---------------*/
#define MEMIF_VENDOR_ID                          43
#define MEMIF_MODULE_ID                          22

#define MEMIF_AR_RELEASE_MAJOR_VERSION           19U
#define MEMIF_AR_RELEASE_MINOR_VERSION           1U
#define MEMIF_AR_RELEASE_REVISION_VERSION        1U
#define MEMIF_SW_MAJOR_VERSION                   1U
#define MEMIF_SW_MINOR_VERSION                   0U
#define MEMIF_SW_PATCH_VERSION                   3U

#define MEMIF_CFG_H_AR_MAJOR_VERSION            (19U)
#define MEMIF_CFG_H_AR_MINOR_VERSION            (1U)
#define MEMIF_CFG_H_AR_PATCH_VERSION            (1U)
#define MEMIF_CFG_H_SW_MAJOR_VERSION            (1U)
#define MEMIF_CFG_H_SW_MINOR_VERSION            (0U)
#define MEMIF_CFG_H_SW_PATCH_VERSION            (1U)

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/**** Error Codes in Development-Mode *******/
#define MEMIF_E_NO_ERROR                       (0x00u)
#define MEMIF_E_PARAM_DEVICE                   (0x01u)
#define MEMIF_E_PARAM_POINTER                  (0x02u)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/* A memory hardware abstraction module's state is denoted in 
 * this type. Mainly used in MemIf_GetStatus and related 
 * APIs of memory hardware abstraction modules.
 */
typedef enum
{
    MEMIF_UNINIT = 0,
    MEMIF_IDLE = 1,
    MEMIF_BUSY = 2,
    MEMIF_BUSY_INTERNAL = 3
}	MemIf_StatusType;

/* A requested job can have one these values after it has been
 * processed. Use in MemIf_GetJobResult and related memory 
 * hardware abstraction module APIs. 
 */
typedef enum
{
    MEMIF_JOB_OK = 0,
    MEMIF_JOB_FAILED = 1,
    MEMIF_JOB_PENDING = 2,
	MEMIF_JOB_CANCELED = 3,
    MEMIF_BLOCK_INCONSISTENT = 4,
    MEMIF_BLOCK_INVALID = 5
}	MemIf_JobResultType;


/* Specifies the processing speed of requested jobs. The exact 
 * processing duration in the different modes depends on 
 * configuration of the underlying memory hardware abstraction 
 * modules.   
 */
typedef enum
{
    MEMIF_MODE_SLOW = 0,
    MEMIF_MODE_FAST = 1
}	MemIf_ModeType;

/* Workaround for AUTOSAR Release 2.0 Flash Drivers:
 * 
 * Fls assumes, that MemIf provides MemIf_AddressType and MemIf_LenghthType.
 * Eep assumes, that it should define its address types on its own.
 * MemIf assumes, that Eep is right and hence does NOT provide these 
 * types. However, there are some implementations, that did not recognise 
 * this inconsistency, so we have to provide these types in order to
 * get Flash drivers and Flash Eeprom Emultation integrated with the 
 * other BSW Modules. In order to minimize the risk of limitting the 
 * address range of underlying flash drivers, both types are based 
 * on 32bit-address range (allowing 4GB of data, which should be 
 * considerably adequate). This might not be the optimum data type 
 * on all platforms, but it's the safest.
 */
typedef uint32 MemIf_AddressType;
typedef uint32 MemIf_LengthType;

/*---- API CFG Macros ------------------------------------------------------*/

/*---- API CFG Type definitions --------------------------------------------*/
/* type definitions for referencing APIs of underlying memory hardware abstraction modules */
typedef P2FUNC (void, MEMIF_CODE, MemIf_ApiSetModeType)(MemIf_ModeType Mode);
typedef P2FUNC (Std_ReturnType, MEMIF_CODE, MemIf_ApiReadType)(uint16 BlockNumber,uint16 BlockOffset,uint8 *DataBufferPtr,uint16 Length);
typedef P2FUNC (Std_ReturnType, MEMIF_CODE, MemIf_ApiWriteType)(uint16 BlockNumber,uint8 const *DataBufferPtr);
typedef P2FUNC (Std_ReturnType, MEMIF_CODE, MemIf_ApiEraseImmediateBlockType)(uint16 BlockNumber);
typedef P2FUNC (Std_ReturnType, MEMIF_CODE, MemIf_ApiInvalidateBlockType)(uint16 BlockNumber);
typedef P2FUNC (void, MEMIF_CODE, MemIf_ApiCancelType)(void);
typedef P2FUNC (MemIf_StatusType, MEMIF_CODE, MemIf_ApiGetStatusType)(void);
typedef P2FUNC (MemIf_JobResultType, MEMIF_CODE, MemIf_ApiGetJobResultType)(void);

/* configuration structure, handles the API mapping */
typedef struct
{
	MemIf_ApiSetModeType SetMode;
    MemIf_ApiReadType Read;
    MemIf_ApiWriteType Write;
    MemIf_ApiEraseImmediateBlockType EraseImmediateBlock;
    MemIf_ApiInvalidateBlockType InvalidateBlock;
    MemIf_ApiCancelType Cancel;
    MemIf_ApiGetStatusType GetStatus;
    MemIf_ApiGetJobResultType GetJobResult;
}	MemIf_MemHwAApi_Type;

#endif
