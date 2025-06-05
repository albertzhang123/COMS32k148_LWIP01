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
**  FILENAME    : EthSM_MemMap.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : HuRongbo                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of EthSM             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>       <AUTHOR>   <REVISION LOG>
 *  V2.0.0       2020-05-27   HuRongbo   R19_11 EthSM initial version.
 */

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/


/*******************************************************************************
**                      MemMap Symbols Define                                 **
*******************************************************************************/
#ifndef ETHSM_MEMMAP_NOTUSED

#if defined(ETHSM_START_SEC_CODE)
    #undef ETHSM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined(ETHSM_STOP_SEC_CODE)
    #undef ETHSM_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined(ETHSM_START_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE)
    #undef ETHSM_START_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined(ETHSM_STOP_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE)
    #undef ETHSM_STOP_SEC_ETHSMCTRLMODEINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined(ETHSM_START_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE)
    #undef ETHSM_START_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined(ETHSM_STOP_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE)
    #undef ETHSM_STOP_SEC_ETHSMTRCVLINKSTATECHG_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined(ETHSM_START_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE)
    #undef ETHSM_START_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined(ETHSM_STOP_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE)
    #undef ETHSM_STOP_SEC_ETHSMTCPIPMODEINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE


#elif defined(ETHSM_START_SEC_VAR_CLEARED_UNSPECIFIED)
    #undef ETHSM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined(ETHSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
    #undef ETHSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define STOP_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined(ETHSM_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef ETHSM_START_SEC_VAR_INIT_UNSPECIFIED
    #define START_SEC_VAR_INIT_UNSPECIFIED
#elif defined(ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_INIT_UNSPECIFIED

#elif defined(ETHSM_START_SEC_VAR_INIT_PTR)
    #undef ETHSM_START_SEC_VAR_INIT_PTR
    #define START_SEC_VAR_INIT_PTR
#elif defined(ETHSM_STOP_SEC_VAR_INIT_PTR)
    #undef ETHSM_STOP_SEC_VAR_INIT_PTR
    #define STOP_SEC_VAR_INIT_PTR

#elif defined(ETHSM_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef ETHSM_START_SEC_CONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined(ETHSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef ETHSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONFIG_DATA_UNSPECIFIED

#else
    #error "Undefined section in EthSM"
#endif
#endif /* #ifndef ETHSM_MEMMAP_NOTUSED */

#include "MemMap.h"
