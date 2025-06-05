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
**  FILENAME    : Rte_ComM_Type.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V2.0.0       2021-02-24  Wanglili       R19_11 initial version.
 */
#ifndef RTE_COMM_TYPE_H
#define RTE_COMM_TYPE_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#if !defined(COMM_VERSION_R19_11)
#define COMM_VERSION_R19_11      STD_ON
#endif/* !defined(COMM_VERSION_R19_11) */
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef uint8 ComM_UserHandleType;

/* Inhibit wakeup and limit to NoCom mask */
typedef uint8 ComM_InhibitionStatusType;
#define COMM_MASK_INHIBIT_WAKEUP    (ComM_InhibitionStatusType)0x01u
#define COMM_MASK_LIMIT_TO_NOCOM    (ComM_InhibitionStatusType)0x02u

typedef uint8 ComM_ModeType;
#define COMM_NO_COMMUNICATION           (ComM_ModeType)0u
#define COMM_SILENT_COMMUNICATION       (ComM_ModeType)1u
#define COMM_FULL_COMMUNICATION         (ComM_ModeType)2u

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* RTE_COMM_TYPE_H */
