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
**  FILENAME    : Lwip_MemMap.h                                                                                       **
**                                                                                                                    **
**  Created on  : 10/03/21                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : lwip large memory mapping                                                                           **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V1.0.0    [20190813]  [darren.zhang]   Initial version
 **********************************************************************************************************************/

#if defined(LWIP_HEAP_START_SEC_VAR_NO_INIT_UNSPECIFIED)
#undef LWIP_HEAP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#define LWIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#elif defined(LWIP_HEAP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED)
#undef LWIP_HEAP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#define LWIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined(LWIP_MEMP_START_SEC_VAR_INIT_UNSPECIFIED)
#undef LWIP_MEMP_START_SEC_VAR_INIT_UNSPECIFIED
#define LWIP_START_SEC_VAR_INIT_UNSPECIFIED
#elif defined(LWIP_MEMP_STOP_SEC_VAR_INIT_UNSPECIFIED)
#undef LWIP_MEMP_STOP_SEC_VAR_INIT_UNSPECIFIED
#define LWIP_STOP_SEC_VAR_INIT_UNSPECIFIED

#else
#error "lwip not include this section"
#endif

#include "MemMap.h"