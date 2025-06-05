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
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifdef DCM_START_SEC_CODE
    #undef DCM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined DCM_STOP_SEC_CODE
    #undef DCM_STOP_SEC_CODE
    #define STOP_SEC_CODE
#elif defined DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #undef DCM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #define START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined DCM_START_SEC_VAR_POWER_ON_INIT_16
    #undef DCM_START_SEC_VAR_POWER_ON_INIT_16
    #define START_SEC_VAR_POWER_ON_INIT_16
#elif defined DCM_STOP_SEC_VAR_POWER_ON_INIT_16
    #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_16
    #define STOP_SEC_VAR_POWER_ON_INIT_16
#elif defined DCM_START_SEC_VAR_POWER_ON_INIT_8
    #undef DCM_START_SEC_VAR_POWER_ON_INIT_8
    #define START_SEC_VAR_POWER_ON_INIT_8
#elif defined DCM_STOP_SEC_VAR_POWER_ON_INIT_8
    #undef DCM_STOP_SEC_VAR_POWER_ON_INIT_8
    #define STOP_SEC_VAR_POWER_ON_INIT_8
#elif defined DCM_START_SEC_VAR_NOINIT_32
    #undef DCM_START_SEC_VAR_NOINIT_32
    #define START_SEC_VAR_NOINIT_32
#elif defined DCM_STOP_SEC_VAR_NOINIT_32
    #undef DCM_STOP_SEC_VAR_NOINIT_32
    #define STOP_SEC_VAR_NOINIT_32
#elif defined DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #undef DCM_START_SEC_VAR_NOINIT_UNSPECIFIED
    #define START_SEC_VAR_NOINIT_UNSPECIFIED
#elif defined DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #undef DCM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
    #define STOP_SEC_VAR_NOINIT_UNSPECIFIED
#elif defined DCM_START_SEC_CONST_32
    #undef DCM_START_SEC_CONST_32
    #define START_SEC_CONST_32
#elif defined DCM_STOP_SEC_CONST_32
    #undef DCM_STOP_SEC_CONST_32
    #define STOP_SEC_CONST_32
#elif defined DCM_START_SEC_CONST_8
    #undef DCM_START_SEC_CONST_8
    #define START_SEC_CONST_8
#elif defined DCM_STOP_SEC_CONST_8
    #undef DCM_STOP_SEC_CONST_8
    #define STOP_SEC_CONST_8
#elif defined DCM_START_SEC_CONST_UNSPECIFIED
    #undef DCM_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#elif defined DCM_STOP_SEC_CONST_UNSPECIFIED
    #undef DCM_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST_UNSPECIFIED
#elif defined DCM_START_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
    #undef DCM_START_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMSTARTOFRECEPTION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMCOPYRXDATA_CALLBACK_CODE
    #undef DCM_START_SEC_DCMCOPYRXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMCOPYRXDATA_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMCOPYRXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMTPRXINDICATION_CALLBACK_CODE
    #undef DCM_START_SEC_DCMTPRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMTPRXINDICATION_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMTPRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMCOPYTXDATA_CALLBACK_CODE
    #undef DCM_START_SEC_DCMCOPYTXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMCOPYTXDATA_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMCOPYTXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
    #undef DCM_START_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMTPTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
    #undef DCM_START_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
    #undef DCM_START_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMCOMMNOCOMMODEENTERED_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
    #undef DCM_START_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMCOMMSILENTCOMMODEENTERED_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#elif defined DCM_START_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
    #undef DCM_START_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined DCM_STOP_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
    #undef DCM_STOP_SEC_DCMCOMMFULLCOMMODEENTERED_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE
#else
    #error "DCM not include this section"
#endif

#include "MemMap.h"

