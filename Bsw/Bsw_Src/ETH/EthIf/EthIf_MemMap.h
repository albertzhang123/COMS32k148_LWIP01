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
**  FILENAME    : EthIf_MemMap.h                                              **
**                                                                            **
**  Created on  : 2020-11-1                                                   **
**  Author      : YB                                                          **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-11-12   YB             initial version.
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#if defined (START_WITH_IF)

#elif defined ETHIF_START_SEC_CODE
    #undef ETHIF_START_SEC_CODE
    #define START_SEC_CODE
#elif defined ETHIF_STOP_SEC_CODE
    #undef ETHIF_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
    #undef ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #undef ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define STOP_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined ETHIF_START_SEC_VAR_CLEARED_32
    #undef ETHIF_START_SEC_VAR_CLEARED_32
    #define START_SEC_VAR_CLEARED_32
#elif defined ETHIF_STOP_SEC_VAR_CLEARED_32
    #undef ETHIF_STOP_SEC_VAR_CLEARED_32
    #define STOP_SEC_VAR_CLEARED_32

#elif defined ETHIF_START_SEC_VAR_CLEARED_16
    #undef ETHIF_START_SEC_VAR_CLEARED_16
    #define START_SEC_VAR_CLEARED_16
#elif defined ETHIF_STOP_SEC_VAR_CLEARED_16
    #undef ETHIF_STOP_SEC_VAR_CLEARED_16
    #define STOP_SEC_VAR_CLEARED_16

#elif defined ETHIF_START_SEC_VAR_CLEARED_8
    #undef ETHIF_START_SEC_VAR_CLEARED_8
    #define START_SEC_VAR_CLEARED_8
#elif defined ETHIF_STOP_SEC_VAR_CLEARED_8
    #undef ETHIF_STOP_SEC_VAR_CLEARED_8
    #define STOP_SEC_VAR_CLEARED_8

#elif defined ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
    #undef ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #undef ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined ETHIF_START_SEC_CONST_PTR
    #undef ETHIF_START_SEC_CONST_PTR
    #define START_SEC_CONST_PTR
#elif defined ETHIF_STOP_SEC_CONST_PTR
    #undef ETHIF_STOP_SEC_CONST_PTR
    #define STOP_SEC_CONST_PTR

#elif defined ETHIF_START_SEC_CONST_8
    #undef ETHIF_START_SEC_CONST_8
    #define START_SEC_CONST_8
#elif defined ETHIF_STOP_SEC_CONST_8
    #undef ETHIF_STOP_SEC_CONST_8
    #define STOP_SEC_CONST_8

#elif defined ETHIF_START_SEC_CONST_16
    #undef ETHIF_START_SEC_CONST_16
    #define START_SEC_CONST_16
#elif defined ETHIF_STOP_SEC_CONST_16
    #undef ETHIF_STOP_SEC_CONST_16
    #define STOP_SEC_CONST_16

#elif defined ETHIF_START_SEC_CONST_32
    #undef ETHIF_START_SEC_CONST_32
    #define START_SEC_CONST_32
#elif defined ETHIF_STOP_SEC_CONST_32
    #undef ETHIF_STOP_SEC_CONST_32
    #define STOP_SEC_CONST_32

#elif defined ETHIF_START_SEC_CONST_UNSPECIFIED
    #undef ETHIF_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#elif defined ETHIF_STOP_SEC_CONST_UNSPECIFIED
    #undef ETHIF_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST_UNSPECIFIED

#endif /* START_WITH_IF */
#include "MemMap.h"
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
