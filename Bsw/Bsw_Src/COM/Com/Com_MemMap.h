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
**  FILENAME    : Com_MemMap.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of Com               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define COM_MEMMAP_VENDOR_ID        62u
#define COM_MEMMAP_MODULE_ID        50u
#define COM_MEMMAP_AR_MAJOR_VERSION  4u
#define COM_MEMMAP_AR_MINOR_VERSION  2u
#define COM_MEMMAP_AR_PATCH_VERSION  2u
#define COM_MEMMAP_SW_MAJOR_VERSION  2u
#define COM_MEMMAP_SW_MINOR_VERSION  0u
#define COM_MEMMAP_SW_PATCH_VERSION  0u
#define COM_MEMMAP_VENDOR_API_INFIX  0u

#if defined COM_START_SEC_CODE
    #undef COM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined COM_STOP_SEC_CODE
    #undef COM_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined COM_START_SEC_COMTRIGGERTRANSMIT_CALLBACK_CODE
    #undef COM_START_SEC_COMTRIGGERTRANSMIT_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMTRIGGERTRANSMIT_CALLBACK_CODE
    #undef COM_STOP_SEC_COMTRIGGERTRANSMIT_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMRXINDICATION_CALLBACK_CODE
    #undef COM_START_SEC_COMRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMRXINDICATION_CALLBACK_CODE
    #undef COM_STOP_SEC_COMRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMTPRXINDICATION_CALLBACK_CODE
    #undef COM_START_SEC_COMTPRXINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMTPRXINDICATION_CALLBACK_CODE
    #undef COM_STOP_SEC_COMTPRXINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMTXCONFIRMATION_CALLBACK_CODE
    #undef COM_START_SEC_COMTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMTXCONFIRMATION_CALLBACK_CODE
    #undef COM_STOP_SEC_COMTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMTPTXCONFIRMATION_CALLBACK_CODE
    #undef COM_START_SEC_COMTPTXCONFIRMATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMTPTXCONFIRMATION_CALLBACK_CODE
    #undef COM_STOP_SEC_COMTPTXCONFIRMATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMSTARTOFRECEPTION_CALLBACK_CODE
    #undef COM_START_SEC_COMSTARTOFRECEPTION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMSTARTOFRECEPTION_CALLBACK_CODE
    #undef COM_STOP_SEC_COMSTARTOFRECEPTION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMCOPYRXDATA_CALLBACK_CODE
    #undef COM_START_SEC_COMCOPYRXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMCOPYRXDATA_CALLBACK_CODE
    #undef COM_STOP_SEC_COMCOPYRXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_COMCOPYTXDATA_CALLBACK_CODE
    #undef COM_START_SEC_COMCOPYTXDATA_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COM_STOP_SEC_COMCOPYTXDATA_CALLBACK_CODE
    #undef COM_STOP_SEC_COMCOPYTXDATA_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COM_START_SEC_VAR_INIT_8
    #undef COM_START_SEC_VAR_INIT_8
    #define START_SEC_VAR_INIT_8
#elif defined COM_STOP_SEC_VAR_INIT_8
    #undef COM_STOP_SEC_VAR_INIT_8
    #define STOP_SEC_VAR_INIT_8

#elif defined COM_START_SEC_VAR_INIT_PTR
    #undef COM_START_SEC_VAR_INIT_PTR
    #define START_SEC_VAR_INIT_PTR
#elif defined COM_STOP_SEC_VAR_INIT_PTR
    #undef COM_STOP_SEC_VAR_INIT_PTR
    #define STOP_SEC_VAR_INIT_PTR

#elif defined COM_START_SEC_VAR_NO_INIT_BOOLEAN
    #undef COM_START_SEC_VAR_NO_INIT_BOOLEAN
    #define START_SEC_VAR_NO_INIT_BOOLEAN
#elif defined COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
    #undef COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
    #define STOP_SEC_VAR_NO_INIT_BOOLEAN

#elif defined COM_START_SEC_VAR_NO_INIT_8
    #undef COM_START_SEC_VAR_NO_INIT_8
    #define START_SEC_VAR_NO_INIT_8
#elif defined COM_STOP_SEC_VAR_NO_INIT_8
    #undef COM_STOP_SEC_VAR_NO_INIT_8
    #define STOP_SEC_VAR_NO_INIT_8

#elif defined COM_START_SEC_VAR_NO_INIT_16
    #undef COM_START_SEC_VAR_NO_INIT_16
    #define START_SEC_VAR_NO_INIT_16
#elif defined COM_STOP_SEC_VAR_NO_INIT_16
    #undef COM_STOP_SEC_VAR_NO_INIT_16
    #define STOP_SEC_VAR_NO_INIT_16

#elif defined COM_START_SEC_VAR_NO_INIT_32
    #undef COM_START_SEC_VAR_NO_INIT_32
    #define START_SEC_VAR_NO_INIT_32
#elif defined COM_STOP_SEC_VAR_NO_INIT_32
    #undef COM_STOP_SEC_VAR_NO_INIT_32
    #define STOP_SEC_VAR_NO_INIT_32

#elif defined COM_START_SEC_VAR_NO_INIT_64
    #undef COM_START_SEC_VAR_NO_INIT_64
    #define START_SEC_VAR_NO_INIT_64
#elif defined COM_STOP_SEC_VAR_NO_INIT_64
    #undef COM_STOP_SEC_VAR_NO_INIT_64
    #define STOP_SEC_VAR_NO_INIT_64

#elif defined COM_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef COM_START_SEC_VAR_NO_INIT_UNSPECIFIED
    #define START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined COM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #undef COM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define START_SEC_CONST_PBCFG_UNSPECIFIED
#elif defined COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #undef COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
    #define STOP_SEC_CONST_PBCFG_UNSPECIFIED

#elif defined COM_START_SEC_PBCONFIG_DATA_BOOLEAN
    #undef COM_START_SEC_PBCONFIG_DATA_BOOLEAN
    #define START_SEC_CONST_PBCFG_BOOLEAN
#elif defined COM_STOP_SEC_PBCONFIG_DATA_BOOLEAN
    #undef COM_STOP_SEC_PBCONFIG_DATA_BOOLEAN
    #define STOP_SEC_CONST_PBCFG_BOOLEAN

#elif defined COM_START_SEC_PBCONFIG_DATA_8
    #undef COM_START_SEC_PBCONFIG_DATA_8
    #define START_SEC_CONST_PBCFG_8
#elif defined COM_STOP_SEC_PBCONFIG_DATA_8
    #undef COM_STOP_SEC_PBCONFIG_DATA_8
    #define STOP_SEC_CONST_PBCFG_8

#elif defined COM_START_SEC_PBCONFIG_DATA_16
    #undef COM_START_SEC_PBCONFIG_DATA_16
    #define START_SEC_CONST_PBCFG_16
#elif defined COM_STOP_SEC_PBCONFIG_DATA_16
    #undef COM_STOP_SEC_PBCONFIG_DATA_16
    #define STOP_SEC_CONST_PBCFG_16

#elif defined COM_START_SEC_PBCONFIG_DATA_32
    #undef COM_START_SEC_PBCONFIG_DATA_32
    #define START_SEC_CONST_PBCFG_32
#elif defined COM_STOP_SEC_PBCONFIG_DATA_32
    #undef COM_STOP_SEC_PBCONFIG_DATA_32
    #define STOP_SEC_CONST_PBCFG_32

#elif defined COM_START_SEC_PBCONFIG_DATA_64
    #undef COM_START_SEC_PBCONFIG_DATA_64
    #define START_SEC_CONST_PBCFG_64
#elif defined COM_STOP_SEC_PBCONFIG_DATA_64
    #undef COM_STOP_SEC_PBCONFIG_DATA_64
    #define STOP_SEC_CONST_PBCFG_64

#else
#error "Com not include this section"
#endif
#include "MemMap.h"

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/



