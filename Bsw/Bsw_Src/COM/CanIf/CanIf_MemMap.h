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
**  FILENAME    : CanIf_MemMap.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of CanIf             **
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
#define CANIF_MEMMAP_VENDOR_ID  62u
#define CANIF_MEMMAP_MODULE_ID  60u
#define CANIF_MEMMAP_AR_MAJOR_VERSION  4u
#define CANIF_MEMMAP_AR_MINOR_VERSION  2u
#define CANIF_MEMMAP_AR_PATCH_VERSION  2u
#define CANIF_MEMMAP_SW_MAJOR_VERSION  2u
#define CANIF_MEMMAP_SW_MINOR_VERSION  0u
#define CANIF_MEMMAP_SW_PATCH_VERSION  0u
#define CANIF_MEMMAP_VENDOR_API_INFIX  0u


#if defined CANIF_START_SEC_CODE
	#undef CANIF_START_SEC_CODE
	#define START_SEC_CODE
#elif defined CANIF_STOP_SEC_CODE
	#undef CANIF_STOP_SEC_CODE
	#define STOP_SEC_CODE

#elif defined CANIF_START_SEC_CANIFTRIGGERTRANSMIT_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFTRIGGERTRANSMIT_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFTRIGGERTRANSMIT_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFTRIGGERTRANSMIT_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFTXCONFIRMATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFRXINDICATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFRXINDICATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFRXINDICATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFRXINDICATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFCONTROLLERBUSOFF_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFCONTROLLERBUSOFF_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFCONTROLLERBUSOFF_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFCONTROLLERBUSOFF_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFCONFIRMPNAVAILABILITY_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFCONFIRMPNAVAILABILITY_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFCONFIRMPNAVAILABILITY_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFCONFIRMPNAVAILABILITY_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFCLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFCLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFCLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFCLEARTRCVWUFFLAGINDICATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFCHECKTRCVWAKEFLAGINDICATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFCHECKTRCVWAKEFLAGINDICATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFCHECKTRCVWAKEFLAGINDICATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFCHECKTRCVWAKEFLAGINDICATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFCONTROLLERMODEINDICATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFCONTROLLERMODEINDICATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFCONTROLLERMODEINDICATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFCONTROLLERMODEINDICATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFTRCVMODEINDICATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFTRCVMODEINDICATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFTRCVMODEINDICATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFTRCVMODEINDICATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_CANIFCURRENTICOMCONFIGURATION_CALLBACK_CODE
	#undef CANIF_START_SEC_CANIFCURRENTICOMCONFIGURATION_CALLBACK_CODE
	#define START_SEC_CALLBACK_CODE
#elif defined CANIF_STOP_SEC_CANIFCURRENTICOMCONFIGURATION_CALLBACK_CODE
	#undef CANIF_STOP_SEC_CANIFCURRENTICOMCONFIGURATION_CALLBACK_CODE
	#define STOP_SEC_CALLBACK_CODE

#elif defined CANIF_START_SEC_VAR_INIT_8
	#undef CANIF_START_SEC_VAR_INIT_8
	#define START_SEC_DATA_INIT_8
#elif defined CANIF_STOP_SEC_VAR_INIT_8
	#undef CANIF_STOP_SEC_VAR_INIT_8
	#define STOP_SEC_DATA_INIT_8

#elif defined CANIF_START_SEC_VAR_INIT_PTR
	#undef CANIF_START_SEC_VAR_INIT_PTR
	#define START_SEC_DATA_INIT_PTR
#elif defined CANIF_STOP_SEC_VAR_INIT_PTR
	#undef CANIF_STOP_SEC_VAR_INIT_PTR
	#define STOP_SEC_DATA_INIT_PTR

#elif defined CANIF_START_SEC_VAR_NO_INIT_8
	#undef CANIF_START_SEC_VAR_NO_INIT_8
	#define START_SEC_DATA_NO_INIT_8
#elif defined CANIF_STOP_SEC_VAR_NO_INIT_8
	#undef CANIF_STOP_SEC_VAR_NO_INIT_8
	#define STOP_SEC_DATA_NO_INIT_8

#elif defined CANIF_START_SEC_VAR_NO_INIT_32
	#undef CANIF_START_SEC_VAR_NO_INIT_32
	#define START_SEC_DATA_NO_INIT_32
#elif defined CANIF_STOP_SEC_VAR_NO_INIT_32
	#undef CANIF_STOP_SEC_VAR_NO_INIT_32
	#define STOP_SEC_DATA_NO_INIT_32

#elif defined CANIF_START_SEC_VAR_NO_INIT_BOOLEAN
	#undef CANIF_START_SEC_VAR_NO_INIT_BOOLEAN
	#define START_SEC_DATA_NO_INIT_BOOLEAN
#elif defined CANIF_STOP_SEC_VAR_NO_INIT_BOOLEAN
	#undef CANIF_STOP_SEC_VAR_NO_INIT_BOOLEAN
	#define STOP_SEC_DATA_NO_INIT_BOOLEAN

#elif defined CANIF_START_SEC_VAR_INIT_UNSPECIFIED
	#undef CANIF_START_SEC_VAR_INIT_UNSPECIFIED
	#define START_SEC_DATA_INIT_UNSPECIFIED
#elif defined CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
	#undef CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
	#define STOP_SEC_DATA_INIT_UNSPECIFIED

#elif defined CANIF_START_SEC_PBCONFIG_DATA_UNSPECIFIED
	#undef CANIF_START_SEC_PBCONFIG_DATA_UNSPECIFIED
	#define START_SEC_CONST_PBCFG_UNSPECIFIED
#elif defined CANIF_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
	#undef CANIF_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
	#define STOP_SEC_CONST_PBCFG_UNSPECIFIED

#elif defined CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
	#undef CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
	#define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
	#undef CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
	#define STOP_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined CANIF_START_SEC_CONFIG_DATA_8
	#undef CANIF_START_SEC_CONFIG_DATA_8
	#define START_SEC_CONFIG_DATA_8
#elif defined CANIF_STOP_SEC_CONFIG_DATA_8
	#undef CANIF_STOP_SEC_CONFIG_DATA_8
	#define STOP_SEC_CONFIG_DATA_8

#else
#error "CanIf not include this section"
#endif
#include "MemMap.h"

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
