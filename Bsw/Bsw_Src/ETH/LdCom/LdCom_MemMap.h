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
**  FILENAME    : LdCom_MemMap.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of LdCom             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define LDCOM_MEMMAP_VENDOR_ID  62u
#define LDCOM_MEMMAP_MODULE_ID  49u
#define LDCOM_MEMMAP_AR_MAJOR_VERSION  4u
#define LDCOM_MEMMAP_AR_MINOR_VERSION  2u
#define LDCOM_MEMMAP_AR_PATCH_VERSION  2u
#define LDCOM_MEMMAP_SW_MAJOR_VERSION  2u
#define LDCOM_MEMMAP_SW_MINOR_VERSION  0u
#define LDCOM_MEMMAP_SW_PATCH_VERSION  0u
#define LDCOM_MEMMAP_VENDOR_API_INFIX  0u

#if defined LDCOM_START_SEC_CODE
    #undef LDCOM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined LDCOM_STOP_SEC_CODE
    #undef LDCOM_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined LDCOM_START_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMCOPYTXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMTPTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMSTARTOFRECEPTION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMTPRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMRXINDICATION_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMRXINDICATION_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMCOPYRXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
    #undef LDCOM_START_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined LDCOM_STOP_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
    #undef LDCOM_STOP_SEC_LDCOMTRIGGERTRANSMIT_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined LDCOM_START_SEC_VAR_INIT_BOOLEAN
    #undef LDCOM_START_SEC_VAR_INIT_BOOLEAN
    #define START_SEC_VAR_INIT_BOOLEAN
#elif defined LDCOM_STOP_SEC_VAR_INIT_BOOLEAN
    #undef LDCOM_STOP_SEC_VAR_INIT_BOOLEAN
    #define STOP_SEC_VAR_INIT_BOOLEAN

#elif defined LDCOM_START_SEC_VAR_INIT_UNSPECIFIED
    #undef LDCOM_START_SEC_VAR_INIT_UNSPECIFIED
    #define START_SEC_VAR_INIT_UNSPECIFIED
#elif defined LDCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef LDCOM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_INIT_UNSPECIFIED

#elif defined LDCOM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef LDCOM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONST_PBCFG_UNSPECIFIED
#elif defined LDCOM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef LDCOM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONST_PBCFG_UNSPECIFIED

#else
#error "LdCom not include this section"
#endif

#include "MemMap.h"

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/

