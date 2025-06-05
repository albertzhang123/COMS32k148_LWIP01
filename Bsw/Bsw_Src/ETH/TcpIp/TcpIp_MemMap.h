/***********************************************************************************************************************
**                                                                                                                    **
** Copyright (C) iSOFT   (2020)                                                                                       **
**                                                                                                                    **
** All rights reserved.                                                                                               **
**                                                                                                                    **
** This document contains proprietary information belonging to iSOFT                                                  **
** Passing on and copying of this document, and communication                                                         **
** of its contents is not permitted without prior written authorization.                                              **
**                                                                                                                    **
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : TcpIp_Types.h                                                                                       **
**                                                                                                                    **
**  Created on  : 03/12/18                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION :  type definition for TcpIp                                                                          **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          Revision Control History                                                  **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20200828]  [pengfei.zhu]   Update to R19-11.
 *  V2.0.1    [20200928]  [pengfei.zhu]   QAC test.
 *  V2.1.0    [20210123]  [darren.zhang]  Code refactoring
 **********************************************************************************************************************/

#define TCPIP_MEMMAP_VENDOR_ID                      62u
#define TCPIP_MEMMAP_MODULE_ID                      170
#define TCPIP_MEMMAP_AR_MAJOR_VERSION               4u
#define TCPIP_MEMMAP_AR_MINOR_VERSION               2u
#define TCPIP_MEMMAP_AR_PATCH_VERSION               2u
#define TCPIP_MEMMAP_SW_MAJOR_VERSION               2u
#define TCPIP_MEMMAP_SW_MINOR_VERSION               0u
#define TCPIP_MEMMAP_SW_PATCH_VERSION               0u
#define TCPIP_MEMMAP_VENDOR_API_INFIX               0u


#if defined TCPIP_START_SEC_CODE
#undef TCPIP_START_SEC_CODE
#define START_SEC_CODE
#elif defined TCPIP_STOP_SEC_CODE
#undef TCPIP_STOP_SEC_CODE
#define STOP_SEC_CODE

#elif defined TCPIP_START_SEC_CONST_UNSPECIFIED
#undef TCPIP_START_SEC_CONST_UNSPECIFIED
#define START_SEC_CONST_UNSPECIFIED
#elif defined TCPIP_STOP_SEC_CONST_UNSPECIFIED
#undef TCPIP_STOP_SEC_CONST_UNSPECIFIED
#define STOP_SEC_CONST_UNSPECIFIED

#elif defined TCPIP_START_SEC_VAR_INIT_8
#undef TCPIP_START_SEC_VAR_INIT_8
#define START_SEC_VAR_INIT_8
#elif defined TCPIP_STOP_SEC_VAR_INIT_8
#undef TCPIP_STOP_SEC_VAR_INIT_8
#define STOP_SEC_VAR_INIT_8

#elif defined TCPIP_START_SEC_VAR_INIT_PTR
#undef TCPIP_START_SEC_VAR_INIT_PTR
#define START_SEC_VAR_INIT_PTR
#elif defined TCPIP_STOP_SEC_VAR_INIT_PTR
#undef TCPIP_STOP_SEC_VAR_INIT_PTR
#define STOP_SEC_VAR_INIT_PTR

#elif defined TCPIP_START_SEC_VAR_NO_INIT_8
#undef TCPIP_START_SEC_VAR_NO_INIT_8
#define START_SEC_VAR_NO_INIT_8
#elif defined TCPIP_STOP_SEC_VAR_NO_INIT_8
#undef TCPIP_STOP_SEC_VAR_NO_INIT_8
#define STOP_SEC_VAR_NO_INIT_8

#elif defined TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#define START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#define STOP_SEC_VAR_NO_INIT_UNSPECIFIED


#elif defined TCPIP_START_SEC_VAR_CLEARED_PTR
#undef TCPIP_START_SEC_VAR_CLEARED_PTR
#define START_SEC_VAR_CLEARED_PTR
#elif defined TCPIP_STOP_SEC_VAR_CLEARED_PTR
#undef TCPIP_STOP_SEC_VAR_CLEARED_PTR
#define STOP_SEC_VAR_CLEARED_PTR

#elif defined TCPIP_START_SEC_VAR_CLEARED_8
#undef TCPIP_START_SEC_VAR_CLEARED_8
#define START_SEC_VAR_CLEARED_8
#elif defined TCPIP_STOP_SEC_VAR_CLEARED_8
#undef TCPIP_STOP_SEC_VAR_CLEARED_8
#define STOP_SEC_VAR_CLEARED_8

#elif defined TCPIP_START_SEC_VAR_NO_INIT_32
#undef TCPIP_START_SEC_VAR_NO_INIT_32
#define START_SEC_VAR_NO_INIT_32
#elif defined TCPIP_STOP_SEC_VAR_NO_INIT_32
#undef TCPIP_STOP_SEC_VAR_NO_INIT_32
#define STOP_SEC_VAR_NO_INIT_32


#elif defined TCPIP_START_SEC_VAR_NO_INIT_PTR
#undef TCPIP_START_SEC_VAR_NO_INIT_PTR
#define START_SEC_VAR_NO_INIT_PTR
#elif defined TCPIP_STOP_SEC_VAR_NO_INIT_PTR
#undef TCPIP_STOP_SEC_VAR_NO_INIT_PTR
#define STOP_SEC_VAR_NO_INIT_PTR

#elif defined TCPIP_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#undef TCPIP_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#define START_SEC_PBCONFIG_DATA_UNSPECIFIED
#elif defined TCPIP_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#undef TCPIP_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#define STOP_SEC_PBCONFIG_DATA_UNSPECIFIED

#elif defined TCPIP_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef TCPIP_START_SEC_CONFIG_DATA_UNSPECIFIED
#define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined TCPIP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef TCPIP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#define STOP_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined TCPIP_START_SEC_PBCONFIG_DATA_32
#undef TCPIP_START_SEC_PBCONFIG_DATA_32
#define START_SEC_PBCONFIG_DATA_32
#elif defined TCPIP_STOP_SEC_PBCONFIG_DATA_32
#undef TCPIP_STOP_SEC_PBCONFIG_DATA_32
#define STOP_SEC_PBCONFIG_DATA_32

#elif defined TCPIP_START_SEC_PBCONFIG_DATA_16
#undef TCPIP_START_SEC_PBCONFIG_DATA_16
#define START_SEC_PBCONFIG_DATA_16
#elif defined TCPIP_STOP_SEC_PBCONFIG_DATA_16
#undef TCPIP_STOP_SEC_PBCONFIG_DATA_16
#define STOP_SEC_PBCONFIG_DATA_16

#elif defined TCPIP_START_SEC_PBCONFIG_DATA_8
#undef TCPIP_START_SEC_PBCONFIG_DATA_8
#define START_SEC_PBCONFIG_DATA_8
#elif defined TCPIP_STOP_SEC_PBCONFIG_DATA_8
#undef TCPIP_STOP_SEC_PBCONFIG_DATA_8
#define STOP_SEC_PBCONFIG_DATA_8

#elif defined TCPIP_START_SEC_PBCONFIG_DATA_BOOLEAN
#undef TCPIP_START_SEC_PBCONFIG_DATA_BOOLEAN
#define START_SEC_PBCONFIG_DATA_BOOLEAN
#elif defined TCPIP_STOP_SEC_PBCONFIG_DATA_BOOLEAN
#undef TCPIP_STOP_SEC_PBCONFIG_DATA_BOOLEAN
#define STOP_SEC_PBCONFIG_DATA_BOOLEAN

#elif defined TCPIP_START_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#undef TCPIP_START_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#define START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined TCPIP_STOP_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#undef TCPIP_STOP_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#define STOP_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined TCPIP_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#undef TCPIP_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#define START_SEC_PBCONFIG_DATA_UNSPECIFIED
#elif defined TCPIP_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#undef TCPIP_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#define STOP_SEC_PBCONFIG_DATA_UNSPECIFIED

#else
#error "TcpIp not include this section"
#endif
#include "MemMap.h"