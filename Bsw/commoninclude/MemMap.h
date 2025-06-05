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
**  FILENAME    : MemMap.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of MemMap   		  **
**  			 (For Tricore G9X step)        						      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R1911                       **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*=======[M E M M A P  S Y M B O L  D E F I N E]==============================*/
#if defined (START_SEC_CODE)
   #undef      START_SEC_CODE
   #pragma default_function_attributes = @ ".bsw_text"
#elif defined (STOP_SEC_CODE)
   #undef      STOP_SEC_CODE
   #pragma default_function_attributes =

#elif defined (START_SEC_CALLOUT_CODE)
   #undef      START_SEC_CALLOUT_CODE
   #pragma default_function_attributes = @ ".bsw_text"
#elif defined (STOP_SEC_CALLOUT_CODE)
   #undef      STOP_SEC_CALLOUT_CODE
   #pragma default_function_attributes =

#elif defined (START_SEC_CALLBACK_CODE)
   #undef      START_SEC_CALLBACK_CODE
   #pragma default_function_attributes = @ ".bsw_text"
#elif defined (STOP_SEC_CALLBACK_CODE)
   #undef      STOP_SEC_CALLBACK_CODE
   #pragma default_function_attributes =


/*BSW Bss data.*/
#elif defined (START_SEC_VAR_CLEARED_UNSPECIFIED)
   #undef      START_SEC_VAR_CLEARED_UNSPECIFIED
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_UNSPECIFIED)
   #undef      STOP_SEC_VAR_CLEARED_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_PTR)
   #undef      START_SEC_VAR_CLEARED_PTR
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_PTR)
   #undef      STOP_SEC_VAR_CLEARED_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_32)
   #undef      START_SEC_VAR_CLEARED_32
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_32)
   #undef      STOP_SEC_VAR_CLEARED_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_16)
   #undef      START_SEC_VAR_CLEARED_16
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_16)
   #undef      STOP_SEC_VAR_CLEARED_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_8)
   #undef      START_SEC_VAR_CLEARED_8
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_8)
   #undef      STOP_SEC_VAR_CLEARED_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_BOOLEAN)
   #undef      START_SEC_VAR_CLEARED_BOOLEAN
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_BOOLEAN)
   #undef      STOP_SEC_VAR_CLEARED_BOOLEAN
   #pragma default_variable_attributes =
/*BSW Init data.*/
#if 0
#elif defined (START_SEC_VAR_INIT_UNSPECIFIED)
   #undef      START_SEC_VAR_INIT_UNSPECIFIED
	#pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_INIT_UNSPECIFIED)
   #undef      STOP_SEC_VAR_INIT_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_INIT_PTR)
   #undef      START_SEC_VAR_INIT_PTR
	#pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_INIT_PTR)
   #undef      STOP_SEC_VAR_INIT_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_INIT_32)
   #undef      START_SEC_VAR_INIT_32
	#pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_INIT_32)
   #undef      STOP_SEC_VAR_INIT_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_INIT_16)
   #undef      START_SEC_VAR_INIT_16
	#pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_INIT_16)
   #undef      STOP_SEC_VAR_INIT_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_INIT_8)
   #undef      START_SEC_VAR_INIT_8
	#pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_INIT_8)
   #undef      STOP_SEC_VAR_INIT_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_INIT_BOOLEAN)
   #undef      START_SEC_VAR_INIT_BOOLEAN
	#pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_INIT_BOOLEAN)
   #undef      STOP_SEC_VAR_INIT_BOOLEAN
   #pragma default_variable_attributes =

// power on
#elif defined (START_SEC_VAR_POWER_ON_INIT_8)
   #undef START_SEC_VAR_POWER_ON_INIT_8
   #pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_POWER_ON_INIT_8)
   #undef      STOP_SEC_VAR_POWER_ON_INIT_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_POWER_ON_INIT_16)
   #undef START_SEC_VAR_POWER_ON_INIT_16
   #pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_POWER_ON_INIT_16)
   #undef      STOP_SEC_VAR_POWER_ON_INIT_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_POWER_ON_INIT_BOOLEAN)
   #undef START_SEC_VAR_POWER_ON_INIT_BOOLEAN
   #pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN)
   #undef      STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #pragma default_variable_attributes = @ ".bsw_data"
#elif defined (STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #pragma default_variable_attributes =
#endif
/*BSW no-init var*/
#elif defined (START_SEC_VAR_NO_INIT_UNSPECIFIED)
   #undef      START_SEC_VAR_NO_INIT_UNSPECIFIED
	#pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
   #undef      STOP_SEC_VAR_NO_INIT_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_NO_INIT_PTR)
   #undef      START_SEC_VAR_NO_INIT_PTR
	#pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_NO_INIT_PTR)
   #undef      STOP_SEC_VAR_NO_INIT_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_NO_INIT_32)
   #undef      START_SEC_VAR_NO_INIT_32
	#pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_NO_INIT_32)
   #undef      STOP_SEC_VAR_NO_INIT_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_NO_INIT_16)
   #undef      START_SEC_VAR_NO_INIT_16
	#pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_NO_INIT_16)
   #undef      STOP_SEC_VAR_NO_INIT_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_NO_INIT_8)
    #undef      START_SEC_VAR_NO_INIT_8
	#pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_NO_INIT_8)
   #undef      STOP_SEC_VAR_NO_INIT_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_NO_INIT_BOOLEAN)
   #undef      START_SEC_VAR_NO_INIT_BOOLEAN
	#pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_NO_INIT_BOOLEAN)
   #undef      STOP_SEC_VAR_NO_INIT_BOOLEAN
   #pragma default_variable_attributes =
   
/*Config data(PB Root.for EcuM pbcfg).*/
#elif defined START_SEC_PBCFG_GLOBALROOT
    #undef START_SEC_PBCFG_GLOBALROOT
    #pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined STOP_SEC_PBCFG_GLOBALROOT
    #undef STOP_SEC_PBCFG_GLOBALROOT
    #pragma default_variable_attributes =
#elif defined (START_SEC_CONST_PBCFG_UNSPECIFIED)
   #undef      START_SEC_CONST_PBCFG_UNSPECIFIED
	#pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONST_PBCFG_UNSPECIFIED)
   #undef      STOP_SEC_CONST_PBCFG_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_PBCFG_PTR)
   #undef      START_SEC_CONST_PBCFG_PTR
	#pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONST_PBCFG_PTR)
   #undef      STOP_SEC_CONST_PBCFG_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_PBCFG_32)
   #undef      START_SEC_CONST_PBCFG_32
	#pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONST_PBCFG_32)
   #undef      STOP_SEC_CONST_PBCFG_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_PBCFG_16)
   #undef      START_SEC_CONST_PBCFG_16
	#pragma default_variable_attributes = @ ".bsw_const_cfg"

#elif defined (STOP_SEC_CONST_PBCFG_16)
   #undef      STOP_SEC_CONST_PBCFG_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_PBCFG_8)
   #undef      START_SEC_CONST_PBCFG_8
	#pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONST_PBCFG_8)
   #undef      STOP_SEC_CONST_PBCFG_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_PBCFG_BOOLEAN)
   #undef      START_SEC_CONST_PBCFG_BOOLEAN
	#pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONST_PBCFG_BOOLEAN)
   #undef      STOP_SEC_CONST_PBCFG_BOOLEAN
   #pragma default_variable_attributes =
/*PC/Lc Config data.*/
#elif defined (START_SEC_CONFIG_DATA_UNSPECIFIED)
   #undef      START_SEC_CONFIG_DATA_UNSPECIFIED
   #pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONFIG_DATA_UNSPECIFIED)
   #undef      STOP_SEC_CONFIG_DATA_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONFIG_DATA_PTR)
   #undef      START_SEC_CONFIG_DATA_PTR
   #pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONFIG_DATA_PTR)
   #undef      STOP_SEC_CONFIG_DATA_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONFIG_DATA_32)
   #undef      START_SEC_CONFIG_DATA_32
   #pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONFIG_DATA_32)
   #undef      STOP_SEC_CONFIG_DATA_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONFIG_DATA_16)
   #undef      START_SEC_CONFIG_DATA_16
   #pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONFIG_DATA_16)
   #undef      STOP_SEC_CONFIG_DATA_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONFIG_DATA_8)
   #undef      START_SEC_CONFIG_DATA_8
   #pragma default_variable_attributes = @ ".bsw_const_cfg"
#elif defined (STOP_SEC_CONFIG_DATA_8)
   #undef      STOP_SEC_CONFIG_DATA_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONFIG_DATA_BOOLEAN)
   #undef      START_SEC_CONFIG_DATA_BOOLEAN
   #pragma default_variable_attributes = @ ".bsw_const_cfg"

#elif defined (STOP_SEC_CONFIG_DATA_BOOLEAN)
   #undef      STOP_SEC_CONFIG_DATA_BOOLEAN
   #pragma default_variable_attributes =
/*Const data.*/
#elif defined (START_SEC_CONST_UNSPECIFIED)
   #undef      START_SEC_CONST_UNSPECIFIED
   #pragma default_variable_attributes = @ ".bsw_const"
#elif defined (STOP_SEC_CONST_UNSPECIFIED)
   #undef      STOP_SEC_CONST_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_PTR)
   #undef      START_SEC_CONST_PTR
   #pragma default_variable_attributes = @ ".bsw_const"
#elif defined (STOP_SEC_CONST_PTR)
   #undef      STOP_SEC_CONST_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_32)
   #undef      START_SEC_CONST_32
   #pragma default_variable_attributes = @ ".bsw_const"
#elif defined (STOP_SEC_CONST_32)
   #undef      STOP_SEC_CONST_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_16)
   #undef      START_SEC_CONST_16
   #pragma default_variable_attributes = @ ".bsw_const"
#elif defined (STOP_SEC_CONST_16)
   #undef      STOP_SEC_CONST_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_8)
   #undef      START_SEC_CONST_8
   #pragma default_variable_attributes = @ ".bsw_const"
#elif defined (STOP_SEC_CONST_8)
   #undef      STOP_SEC_CONST_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_CONST_BOOLEAN)
   #undef      START_SEC_CONST_BOOLEAN
   #pragma default_variable_attributes = @ ".bsw_const"
#elif defined (STOP_SEC_CONST_BOOLEAN)
   #undef      STOP_SEC_CONST_BOOLEAN
   #pragma default_variable_attributes =
   
/*Share data.*/
#elif defined (START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED)
   #undef      START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_SHARE_UNSPECIFIED)
   #undef      STOP_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_SHARE_PTR)
   #undef      START_SEC_VAR_CLEARED_SHARE_PTR
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_SHARE_PTR)
   #undef      STOP_SEC_VAR_CLEARED_SHARE_PTR
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_SHARE_32)
   #undef      START_SEC_VAR_CLEARED_SHARE_32
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_SHARE_32)
   #undef      STOP_SEC_VAR_CLEARED_SHARE_32
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_SHARE_16)
   #undef      START_SEC_VAR_CLEARED_SHARE_16
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_SHARE_16)
   #undef      STOP_SEC_VAR_CLEARED_SHARE_16
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_SHARE_8)
   #undef      START_SEC_VAR_CLEARED_SHARE_8
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_SHARE_8)
   #undef      STOP_SEC_VAR_CLEARED_SHARE_8
   #pragma default_variable_attributes =

#elif defined (START_SEC_VAR_CLEARED_SHARE_BOOLEAN)
   #undef      START_SEC_VAR_CLEARED_SHARE_BOOLEAN
   #pragma default_variable_attributes = @ ".bsw_bss"
#elif defined (STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN)
   #undef      STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN
   #pragma default_variable_attributes =

#endif 


/*=======[E N D   O F   F I L E]==============================================*/
