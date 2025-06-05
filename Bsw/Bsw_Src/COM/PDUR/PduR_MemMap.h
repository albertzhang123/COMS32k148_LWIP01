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
**  FILENAME    : PduR_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of PduR              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_MEMMAP_VENDOR_ID  62u
#define PDUR_MEMMAP_MODULE_ID  51u
#define PDUR_MEMMAP_AR_MAJOR_VERSION  4u
#define PDUR_MEMMAP_AR_MINOR_VERSION  2u
#define PDUR_MEMMAP_AR_PATCH_VERSION  2u
#define PDUR_MEMMAP_SW_MAJOR_VERSION  2u
#define PDUR_MEMMAP_SW_MINOR_VERSION  0u
#define PDUR_MEMMAP_SW_PATCH_VERSION  0u
#define PDUR_MEMMAP_VENDOR_API_INFIX  0u


#if defined PDUR_START_SEC_CODE
    #undef PDUR_START_SEC_CODE
    #define START_SEC_CODE
#elif defined PDUR_STOP_SEC_CODE
    #undef PDUR_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE
    #undef PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_IFRXINDICATION_CALLBACK_CODE
    #undef PDUR_STOP_SEC_IFRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE
    #undef PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_IFTXCONFIRMATION_CALLBACK_CODE
    #undef PDUR_STOP_SEC_IFTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
    #undef PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
    #undef PDUR_STOP_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
    #undef PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
    #undef PDUR_STOP_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE
    #undef PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_TPCOPYRXDATA_CALLBACK_CODE
    #undef PDUR_STOP_SEC_TPCOPYRXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE
    #undef PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_TPRXINDICATION_CALLBACK_CODE
    #undef PDUR_STOP_SEC_TPRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE
    #undef PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_TPCOPYTXDATA_CALLBACK_CODE
    #undef PDUR_STOP_SEC_TPCOPYTXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE
    #undef PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined PDUR_STOP_SEC_TPTXCONFIRMATION_CALLBACK_CODE
    #undef PDUR_STOP_SEC_TPTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined PDUR_START_SEC_VAR_INIT_8
    #undef PDUR_START_SEC_VAR_INIT_8
    #define START_SEC_VAR_INIT_8
#elif defined PDUR_STOP_SEC_VAR_INIT_8
    #undef PDUR_STOP_SEC_VAR_INIT_8
    #define STOP_SEC_VAR_INIT_8

#elif defined PDUR_START_SEC_VAR_INIT_PTR
    #undef PDUR_START_SEC_VAR_INIT_PTR
    #define START_SEC_VAR_INIT_PTR
#elif defined PDUR_STOP_SEC_VAR_INIT_PTR
    #undef PDUR_STOP_SEC_VAR_INIT_PTR
    #define STOP_SEC_VAR_INIT_PTR

#elif defined PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #define START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined PDUR_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef PDUR_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined PDUR_START_SEC_VAR_INIT_UNSPECIFIED
    #undef PDUR_START_SEC_VAR_INIT_UNSPECIFIED
    #define START_SEC_VAR_INIT_UNSPECIFIED
#elif defined PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_INIT_UNSPECIFIED

#elif defined PDUR_START_SEC_VAR_NO_INIT_8
    #undef PDUR_START_SEC_VAR_NO_INIT_8
    #define START_SEC_VAR_NO_INIT_8
#elif defined PDUR_STOP_SEC_VAR_NO_INIT_8
    #undef PDUR_STOP_SEC_VAR_NO_INIT_8
    #define STOP_SEC_VAR_NO_INIT_8

#elif defined PDUR_START_SEC_VAR_NO_INIT_BOOLEAN
    #undef PDUR_START_SEC_VAR_NO_INIT_BOOLEAN
    #define START_SEC_VAR_NO_INIT_BOOLEAN
#elif defined PDUR_STOP_SEC_VAR_NO_INIT_BOOLEAN
    #undef PDUR_STOP_SEC_VAR_NO_INIT_BOOLEAN
    #define STOP_SEC_VAR_NO_INIT_BOOLEAN

#elif defined PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONST_PBCFG_UNSPECIFIED
#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONST_PBCFG_UNSPECIFIED

#elif defined PDUR_START_SEC_PBCONFIG_DATA_8
    #undef PDUR_START_SEC_PBCONFIG_DATA_8
    #define START_SEC_CONST_PBCFG_8
#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_8
    #undef PDUR_STOP_SEC_PBCONFIG_DATA_8
    #define STOP_SEC_CONST_PBCFG_8

#elif defined PDUR_START_SEC_PBCONFIG_DATA_16
    #undef PDUR_START_SEC_PBCONFIG_DATA_16
    #define START_SEC_CONST_PBCFG_16
#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_16
    #undef PDUR_STOP_SEC_PBCONFIG_DATA_16
    #define STOP_SEC_CONST_PBCFG_16

#elif defined PDUR_START_SEC_PBCONFIG_DATA_PTR
    #undef PDUR_START_SEC_PBCONFIG_DATA_PTR
    #define START_SEC_CONST_PBCFG_PTR
#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_PTR
    #undef PDUR_STOP_SEC_PBCONFIG_DATA_PTR
    #define STOP_SEC_CONST_PBCFG_PTR

#else
#error "PduR not include this section"
#endif
#include "MemMap.h"

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/

