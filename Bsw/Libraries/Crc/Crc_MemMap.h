/*******************************************************************************
**                                                                            **
** Copyright (C) 2020, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : Crc_MemMap.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : ShenXu,Yb                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef CRC_MEMMAP_H_
#define CRC_MEMMAP_H_
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#define CRC_MEMMAP_H_AR_MAJOR_VERSION 4U
#define CRC_MEMMAP_H_AR_MINOR_VERSION 4U
#define CRC_MEMMAP_H_AR_PATCH_VERSION 4U
#define CRC_MEMMAP_H_SW_MAJOR_VERSION 2U
#define CRC_MEMMAP_H_SW_MINOR_VERSION 0U
#define CRC_MEMMAP_H_SW_PATCH_VERSION 0U

/*
 * <SIZE> -- BOOLEAN/8BIT/16BIT/32BIT/UNSPECIFIED
 */

/*
 * <MSN>_START_SEC_CODE
 * <MSN>_STOP_SEC_CODE
 * To be used for mapping code to applicaiton block,boot block,external flash etc.
 */

/*
 * <MSN>_START_SEC_CODE_FAST
 * <MSN>_STOP_SEC_CODE_FAST
 * To be used for mapping code to local block,boot block of ISR API.
 */

/*
 * <MSN>_START_SEC_VAR_NOINIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_NOINIT_<SIZE>
 * To be used for all global or static variables that are never initialized.
 */

/*
 * <MSN>_START_SEC_VAR_POWER_ON_INIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_POWER_ON_INIT_<SIZE>
 * To be used for all global or static variables that are initialized only after power on reset.
 */

/*
 * <MSN>_START_SEC_VAR_<SIZE>
 * <MSN>_STOP_SEC_VAR_<SIZE>
 * To be used for all global or static variables that are initialized after every reset(the normal case).
 */

/*
 * <MSN>_START_SEC_VAR_FAST_NOINIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_FAST_NOINIT_<SIZE>
 * To be used for all global or static variables that have at least one of the following properties:
 *      *accessed bitwise
 *      *frequently used
 *      *high number of accesses in source code
 *  Some platforms allow the use of bit instructions for variables located in this specific RAM area as
 *  well as shorter addressing instructions.This saves code and runtime.
 */

/*
 * <MSN>_START_SEC_VAR_FAST_POWER_ON_INIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_FAST_POWER_ON_INIT_<SIZE>
 * To be used for all global or static variables that have at least one of the following properties:
 *      *accessed bitwise
 *      *frequently used
 *      *high number of accesses in source code
 *  Some platforms allow the use of bit instructions for variables located in this specific RAM area as
 *  well as shorter addressing instructions.This saves code and runtime.
 */

/*
 * <MSN>_START_SEC_VAR_FAST_<SIZE>
 * <MSN>_STOP_SEC_VAR_FAST_<SIZE>
 * To be used for all global or static variables that have at least one of the following properties:
 *      *accessed bitwise
 *      *frequently used
 *      *high number of accesses in source code
 *  Some platforms allow the use of bit instructions for variables located in this specific RAM area as
 *  well as shorter addressing instructions.This saves code and runtime.
 */

/*
 * <MSN>_START_SEC_CONST_<SIZE>
 * <MSN>_STOP_SEC_CONST_<SIZE>
 * To be used for all global or static constants.
 */

/*
 * <MSN>_START_SEC_CONST_FAST_<SIZE>
 * <MSN>_STOP_SEC_CONST_FAST_<SIZE>
 * To be used for all global or static constants in fast segment block.
 */

/*
 * <MSN>_START_CONST_PBCFG
 * <MSN>_STOP_CONST_PBCFG
 * Constants with attributes that show that they reside in one segment for module configuration.
 */

/*
 * <MSN>_START_CONST_PBCFG_ROOT
 * <MSN>_STOP_CONST_PBCFG_ROOT
 * Constants with attributes that show that they reside in one segment for module configuration for root structure.
 */

/* @req MEMMAP010 @req MEMMAP004 @req MEMMAP021 @req MEMMAP003 @req MEMMAP018 */
/*=======[M A C R O S]========================================================*/
#if defined (START_WITH_IF)

/*=======[C R C]==============================================================*/

#elif defined (CRC_START_SEC_CODE)
#undef      CRC_START_SEC_CODE
#define START_SEC_CODE
#elif defined (CRC_STOP_SEC_CODE)
#undef      CRC_STOP_SEC_CODE
#define STOP_SEC_CODE

#elif defined (CRC_START_SEC_CODE_FAST)
#undef      CRC_START_SEC_CODE_FAST
#define START_SEC_CODE_FAST
#elif defined (CRC_STOP_SEC_CODE_FAST)
#undef      CRC_STOP_SEC_CODE_FAST
#define STOP_SEC_CODE_FAST

#elif defined (CRC_START_SEC_VAR_NOINIT_BOOLEAN)
#undef      CRC_START_SEC_VAR_NOINIT_BOOLEAN
#define START_SEC_VAR_NOINIT_BOOLEAN
#elif defined (CRC_STOP_SEC_VAR_NOINIT_BOOLEAN)
#undef      CRC_STOP_SEC_VAR_NOINIT_BOOLEAN
#define STOP_SEC_VAR_NOINIT_BOOLEAN

#elif defined (CRC_START_SEC_VAR_NOINIT_8BIT)
#undef      CRC_START_SEC_VAR_NOINIT_8BIT
#define START_SEC_VAR_NOINIT_8BIT
#elif defined (CRC_STOP_SEC_VAR_NOINIT_8BIT)
#undef      CRC_STOP_SEC_VAR_NOINIT_8BIT
#define STOP_SEC_VAR_NOINIT_8BIT

#elif defined (CRC_START_SEC_VAR_NOINIT_16BIT)
#undef      CRC_START_SEC_VAR_NOINIT_16BIT
#define START_SEC_VAR_NOINIT_16BIT
#elif defined (CRC_STOP_SEC_VAR_NOINIT_16BIT)
#undef      CRC_STOP_SEC_VAR_NOINIT_16BIT
#define STOP_SEC_VAR_NOINIT_16BIT

#elif defined (CRC_START_SEC_VAR_NOINIT_32BIT)
#undef      CRC_START_SEC_VAR_NOINIT_32BIT
#define START_SEC_VAR_NOINIT_32BIT
#elif defined (CRC_STOP_SEC_VAR_NOINIT_32BIT)
#undef      CRC_STOP_SEC_VAR_NOINIT_32BIT
#define STOP_SEC_VAR_NOINIT_32BIT

#elif defined (CRC_START_SEC_VAR_NOINIT_UNSPECIFIED)
#undef      CRC_START_SEC_VAR_NOINIT_UNSPECIFIED
#define START_SEC_VAR_NOINIT_UNSPECIFIED
#elif defined (CRC_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
#undef      CRC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#define STOP_SEC_VAR_NOINIT_UNSPECIFIED

#elif defined (CRC_START_SEC_VAR_POWER_ON_INIT_BOOLEAN)
#undef      CRC_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#define START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#elif defined (CRC_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN)
#undef      CRC_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#define STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN

#elif defined (CRC_START_SEC_VAR_POWER_ON_INIT_8BIT)
#undef      CRC_START_SEC_VAR_POWER_ON_INIT_8BIT
#define START_SEC_VAR_POWER_ON_INIT_8BIT
#elif defined (CRC_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
#undef      CRC_STOP_SEC_VAR_POWER_ON_INIT_8BIT
#define STOP_SEC_VAR_POWER_ON_INIT_8BIT

#elif defined (CRC_START_SEC_VAR_POWER_ON_INIT_16BIT)
#undef      CRC_START_SEC_VAR_POWER_ON_INIT_16BIT
#define START_SEC_VAR_POWER_ON_INIT_16BIT
#elif defined (CRC_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
#undef      CRC_STOP_SEC_VAR_POWER_ON_INIT_16BIT
#define STOP_SEC_VAR_POWER_ON_INIT_16BIT

#elif defined (CRC_START_SEC_VAR_POWER_ON_INIT_32BIT)
#undef      CRC_START_SEC_VAR_POWER_ON_INIT_32BIT
#define START_SEC_VAR_POWER_ON_INIT_32BIT
#elif defined (CRC_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
#undef      CRC_STOP_SEC_VAR_POWER_ON_INIT_32BIT
#define STOP_SEC_VAR_POWER_ON_INIT_32BIT

#elif defined (CRC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
#undef      CRC_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#define START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined (CRC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
#undef      CRC_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#define STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED

#elif defined (CRC_START_SEC_VAR_BOOLEAN)
#undef      CRC_START_SEC_VAR_BOOLEAN
#define START_SEC_VAR_BOOLEAN
#elif defined (CRC_STOP_SEC_VAR_BOOLEAN)
#undef      CRC_STOP_SEC_VAR_BOOLEAN
#define STOP_SEC_VAR_BOOLEAN

#elif defined (CRC_START_SEC_VAR_8BIT)
#undef      CRC_START_SEC_VAR_8BIT
#define START_SEC_VAR_8BIT
#elif defined (CRC_STOP_SEC_VAR_8BIT)
#undef      CRC_STOP_SEC_VAR_8BIT
#define STOP_SEC_VAR_8BIT

#elif defined (CRC_START_SEC_VAR_16BIT)
#undef      CRC_START_SEC_VAR_16BIT
#define START_SEC_VAR_16BIT
#elif defined (CRC_STOP_SEC_VAR_16BIT)
#undef      CRC_STOP_SEC_VAR_16BIT
#define STOP_SEC_VAR_16BIT

#elif defined (CRC_START_SEC_VAR_32BIT)
#undef      CRC_START_SEC_VAR_32BIT
#define START_SEC_VAR_32BIT
#elif defined (CRC_STOP_SEC_VAR_32BIT)
#undef      CRC_STOP_SEC_VAR_32BIT
#define STOP_SEC_VAR_32BIT

#elif defined (CRC_START_SEC_VAR_UNSPECIFIED)
#undef      CRC_START_SEC_VAR_UNSPECIFIED
#define START_SEC_VAR_UNSPECIFIED
#elif defined (CRC_STOP_SEC_VAR_UNSPECIFIED)
#undef      CRC_STOP_SEC_VAR_UNSPECIFIED
#define STOP_SEC_VAR_UNSPECIFIED


#elif defined (CRC_START_SEC_VAR_FAST_NOINIT_BOOLEAN)
#undef      CRC_START_SEC_VAR_FAST_NOINIT_BOOLEAN
#define START_SEC_VAR_FAST_NOINIT_BOOLEAN
#elif defined (CRC_STOP_SEC_VAR_FAST_NOINIT_BOOLEAN)
#undef      CRC_STOP_SEC_VAR_FAST_NOINIT_BOOLEAN
#define STOP_SEC_VAR_FAST_NOINIT_BOOLEAN

#elif defined (CRC_START_SEC_VAR_FAST_NOINIT_8BIT)
#undef      CRC_START_SEC_VAR_FAST_NOINIT_8BIT
#define START_SEC_VAR_FAST_NOINIT_8BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_NOINIT_8BIT)
#undef      CRC_STOP_SEC_VAR_FAST_NOINIT_8BIT
#define STOP_SEC_VAR_FAST_NOINIT_8BIT

#elif defined (CRC_START_SEC_VAR_FAST_NOINIT_16BIT)
#undef      CRC_START_SEC_VAR_FAST_NOINIT_16BIT
#define START_SEC_VAR_FAST_NOINIT_16BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_NOINIT_16BIT)
#undef      CRC_STOP_SEC_VAR_FAST_NOINIT_16BIT
#define STOP_SEC_VAR_FAST_NOINIT_16BIT

#elif defined (CRC_START_SEC_VAR_FAST_NOINIT_32BIT)
#undef      CRC_START_SEC_VAR_FAST_NOINIT_32BIT
#define START_SEC_VAR_FAST_NOINIT_32BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_NOINIT_32BIT)
#undef      CRC_STOP_SEC_VAR_FAST_NOINIT_32BIT
#define STOP_SEC_VAR_FAST_NOINIT_32BIT

#elif defined (CRC_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
#undef      CRC_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#elif defined (CRC_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
#undef      CRC_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#define STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED

#elif defined (CRC_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
#undef      CRC_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
#define START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
#elif defined (CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
#undef      CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
#define STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN

#elif defined (CRC_START_SEC_VAR_FAST_POWER_ON_INIT_8BIT)
#undef      CRC_START_SEC_VAR_FAST_POWER_ON_INIT_8BIT
#define START_SEC_VAR_FAST_POWER_ON_INIT_8BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_8BIT)
#undef      CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_8BIT
#define STOP_SEC_VAR_FAST_POWER_ON_INIT_8BIT

#elif defined (CRC_START_SEC_VAR_FAST_POWER_ON_INIT_16BIT)
#undef      CRC_START_SEC_VAR_FAST_POWER_ON_INIT_16BIT
#define START_SEC_VAR_FAST_POWER_ON_INIT_16BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_16BIT)
#undef      CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_16BIT
#define STOP_SEC_VAR_FAST_POWER_ON_INIT_16BIT

#elif defined (CRC_START_SEC_VAR_FAST_POWER_ON_INIT_32BIT)
#undef      CRC_START_SEC_VAR_FAST_POWER_ON_INIT_32BIT
#define START_SEC_VAR_FAST_POWER_ON_INIT_32BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_32BIT)
#undef      CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_32BIT
#define STOP_SEC_VAR_FAST_POWER_ON_INIT_32BIT

#elif defined (CRC_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
#undef      CRC_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
#define START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
#elif defined (CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
#undef      CRC_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
#define STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED

#elif defined (CRC_START_SEC_VAR_FAST_BOOLEAN)
#undef      CRC_START_SEC_VAR_FAST_BOOLEAN
#define START_SEC_VAR_FAST_BOOLEAN
#elif defined (CRC_STOP_SEC_VAR_FAST_BOOLEAN)
#undef      CRC_STOP_SEC_VAR_FAST_BOOLEAN
#define STOP_SEC_VAR_FAST_BOOLEAN

#elif defined (CRC_START_SEC_VAR_FAST_8BIT)
#undef      CRC_START_SEC_VAR_FAST_8BIT
#define START_SEC_VAR_FAST_8BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_8BIT)
#undef      CRC_STOP_SEC_VAR_FAST_8BIT
#define STOP_SEC_VAR_FAST_8BIT

#elif defined (CRC_START_SEC_VAR_FAST_16BIT)
#undef      CRC_START_SEC_VAR_FAST_16BIT
#define START_SEC_VAR_FAST_16BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_16BIT)
#undef      CRC_STOP_SEC_VAR_FAST_16BIT
#define STOP_SEC_VAR_FAST_16BIT

#elif defined (CRC_START_SEC_VAR_FAST_32BIT)
#undef      CRC_START_SEC_VAR_FAST_32BIT
#define START_SEC_VAR_FAST_32BIT
#elif defined (CRC_STOP_SEC_VAR_FAST_32BIT)
#undef      CRC_STOP_SEC_VAR_FAST_32BIT
#define STOP_SEC_VAR_FAST_32BIT

#elif defined (CRC_START_SEC_VAR_FAST_UNSPECIFIED)
#undef      CRC_START_SEC_VAR_FAST_UNSPECIFIED
#define START_SEC_VAR_FAST_UNSPECIFIED
#elif defined (CRC_STOP_SEC_VAR_FAST_UNSPECIFIED)
#undef      CRC_STOP_SEC_VAR_FAST_UNSPECIFIED
#define STOP_SEC_VAR_FAST_UNSPECIFIED

#elif defined (CRC_START_SEC_CONST_BOOLEAN)
#undef      CRC_START_SEC_CONST_BOOLEAN
#define START_SEC_CONST_BOOLEAN
#elif defined (CRC_STOP_SEC_CONST_BOOLEAN)
#undef      CRC_STOP_SEC_CONST_BOOLEAN
#define STOP_SEC_CONST_BOOLEAN

#elif defined (CRC_START_SEC_CONST_8BIT)
#undef      CRC_START_SEC_CONST_8BIT
#define START_SEC_CONST_8BIT
#elif defined (CRC_STOP_SEC_CONST_8BIT)
#undef      CRC_STOP_SEC_CONST_8BIT
#define STOP_SEC_CONST_8BIT

#elif defined (CRC_START_SEC_CONST_16BIT)
#undef      CRC_START_SEC_CONST_16BIT
#define START_SEC_CONST_16BIT
#elif defined (CRC_STOP_SEC_CONST_16BIT)
#undef      CRC_STOP_SEC_CONST_16BIT
#define STOP_SEC_CONST_16BIT

#elif defined (CRC_START_SEC_CONST_32BIT)
#undef      CRC_START_SEC_CONST_32BIT
#define START_SEC_CONST_32BIT
#elif defined (CRC_STOP_SEC_CONST_32BIT)
#undef      CRC_STOP_SEC_CONST_32BIT
#define STOP_SEC_CONST_32BIT

#elif defined (CRC_START_SEC_CONST_UNSPECIFIED)
#undef      CRC_START_SEC_CONST_UNSPECIFIED
#define START_SEC_CONST_UNSPECIFIED
#elif defined (CRC_STOP_SEC_CONST_UNSPECIFIED)
#undef      CRC_STOP_SEC_CONST_UNSPECIFIED
#define STOP_SEC_CONST_UNSPECIFIED

#elif defined (CRC_START_SEC_CONST_FAST_BOOLEAN)
#undef      CRC_START_SEC_CONST_FAST_BOOLEAN
#define START_SEC_CONST_FAST_BOOLEAN
#elif defined (CRC_STOP_SEC_CONST_FAST_BOOLEAN)
#undef      CRC_STOP_SEC_CONST_FAST_BOOLEAN
#define STOP_SEC_CONST_FAST_BOOLEAN

#elif defined (CRC_START_SEC_CONST_FAST_8BIT)
#undef      CRC_START_SEC_CONST_FAST_8BIT
#define START_SEC_CONST_FAST_8BIT
#elif defined (CRC_STOP_SEC_CONST_FAST_8BIT)
#undef      CRC_STOP_SEC_CONST_FAST_8BIT
#define STOP_SEC_CONST_FAST_8BIT

#elif defined (CRC_START_SEC_CONST_FAST_16BIT)
#undef      CRC_START_SEC_CONST_FAST_16BIT
#define START_SEC_CONST_FAST_16BIT
#elif defined (CRC_STOP_SEC_CONST_FAST_16BIT)
#undef      CRC_STOP_SEC_CONST_FAST_16BIT
#define STOP_SEC_CONST_FAST_16BIT

#elif defined (CRC_START_SEC_CONST_FAST_32BIT)
#undef      CRC_START_SEC_CONST_FAST_32BIT
#define START_SEC_CONST_FAST_32BIT
#elif defined (CRC_STOP_SEC_CONST_FAST_32BIT)
#undef      CRC_STOP_SEC_CONST_FAST_32BIT
#define STOP_SEC_CONST_FAST_32BIT

#elif defined (CRC_START_SEC_CONST_FAST_UNSPECIFIED)
#undef      CRC_START_SEC_CONST_FAST_UNSPECIFIED
#define START_SEC_CONST_FAST_UNSPECIFIED
#elif defined (CRC_STOP_SEC_CONST_FAST_UNSPECIFIED)
#undef      CRC_STOP_SEC_CONST_FAST_UNSPECIFIED
#define STOP_SEC_CONST_FAST_UNSPECIFIED

#elif defined (CRC_START_SEC_CONST_PBCFG)
#undef      CRC_START_SEC_CONST_PBCFG
#define START_SEC_CONST_PBCFG
#elif defined (CRC_STOP_SEC_CONST_PBCFG)
#undef      CRC_STOP_SEC_CONST_PBCFG
#define STOP_SEC_CONST_PBCFG

#elif defined (CRC_START_SEC_CONFIG_DATA)
#undef      CRC_START_SEC_CONFIG_DATA
#define START_SEC_CONFIG_DATA
#elif defined (CRC_STOP_SEC_CONFIG_DATA)
#undef      CRC_STOP_SEC_CONFIG_DATA
#define STOP_SEC_CONFIG_DATA

#endif /* START_WITH_IF */

//#include "MemMap.h"
#endif
/*=======[E N D   O F   F I L E]==============================================*/
