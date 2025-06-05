/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2020)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    : Cantp_MemMap.h                                             **
**                                                                           **
**  Created on  : 2020/5/9 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : memmap of NvM module.                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifdef NVM_START_SEC_CODE
    #undef NVM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined NVM_STOP_SEC_CODE
    #undef NVM_STOP_SEC_CODE
    #define STOP_SEC_CODE
#elif defined NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #undef NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #define START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined NVM_START_SEC_CONST_UNSPECIFIED
    #undef NVM_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#elif defined NVM_STOP_SEC_CONST_UNSPECIFIED
    #undef NVM_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST_UNSPECIFIED
#elif defined NVM_START_SEC_VAR_POWER_ON_INIT_16
    #undef NVM_START_SEC_VAR_POWER_ON_INIT_16
    #define START_SEC_VAR_POWER_ON_INIT_16
#elif defined NVM_STOP_SEC_VAR_POWER_ON_INIT_16
    #undef NVM_STOP_SEC_VAR_POWER_ON_INIT_16
    #define STOP_SEC_VAR_POWER_ON_INIT_16
#elif defined NVM_START_SEC_VAR_CLEARED_8
    #undef NVM_START_SEC_VAR_CLEARED_8
    #define START_SEC_VAR_CLEARED_8
#elif defined NVM_STOP_SEC_VAR_CLEARED_8
    #undef NVM_STOP_SEC_VAR_CLEARED_8
    #define STOP_SEC_VAR_CLEARED_8
#elif defined NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #undef NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #undef NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define STOP_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined NVM_START_SEC_VAR_CLEARED_16
    #undef NVM_START_SEC_VAR_CLEARED_16
    #define START_SEC_VAR_CLEARED_16
#elif defined NVM_STOP_SEC_VAR_CLEARED_16
    #undef NVM_STOP_SEC_VAR_CLEARED_16
    #define STOP_SEC_VAR_CLEARED_16
#elif defined NVM_START_SEC_JOBERRORNOTIFICATIONCALLBACK_CODE
    #undef NVM_START_SEC_JOBERRORNOTIFICATIONCALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined NVM_STOP_SEC_JOBERRORNOTIFICATIONCALLBACK_CODE
    #undef NVM_STOP_SEC_JOBERRORNOTIFICATIONCALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined NVM_START_SEC_JOBENDNOTIFICATIONCALLBACK_CODE
    #undef NVM_START_SEC_JOBENDNOTIFICATIONCALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined NVM_STOP_SEC_JOBENDNOTIFICATIONCALLBACK_CODE
    #undef NVM_STOP_SEC_JOBENDNOTIFICATIONCALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#else
    #error "NvM not include this section"
#endif

#include "MemMap.h"

