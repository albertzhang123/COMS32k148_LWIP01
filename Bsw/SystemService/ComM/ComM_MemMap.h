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
**  FILENAME    : ComM_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      :                                                             **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V2.0.0       2020-07-24  Wanglili       R19_11 initial version.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#if defined COMM_START_SEC_CODE
    #undef COMM_START_SEC_CODE
    #define START_SEC_CODE
#elif defined COMM_STOP_SEC_CODE
    #undef COMM_STOP_SEC_CODE
    #define STOP_SEC_CODE

#elif defined COMM_START_SEC_VAR_INIT_UNSPECIFIED
    #undef COMM_START_SEC_VAR_INIT_UNSPECIFIED
    #define START_SEC_VAR_INIT_UNSPECIFIED
#elif defined COMM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #undef COMM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #define STOP_SEC_VAR_INIT_UNSPECIFIED

#elif defined COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #undef COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #undef COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define STOP_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined COMM_START_SEC_VAR_CLEARED_PTR
    #undef COMM_START_SEC_VAR_CLEARED_PTR
    #define START_SEC_VAR_CLEARED_PTR
#elif defined COMM_STOP_SEC_VAR_CLEARED_PTR
    #undef COMM_STOP_SEC_VAR_CLEARED_PTR
    #define STOP_SEC_VAR_CLEARED_PTR

#elif defined COMM_START_SEC_CONST_BOOLEAN
    #undef COMM_START_SEC_CONST_BOOLEAN
    #define START_SEC_CONST_BOOLEAN
#elif defined COMM_STOP_SEC_CONST_BOOLEAN
    #undef COMM_STOP_SEC_CONST_BOOLEAN
    #define STOP_SEC_CONST_BOOLEAN

#elif defined COMM_START_SEC_CONST_UNSPECIFIED
    #undef COMM_START_SEC_CONST_UNSPECIFIED
    #define START_SEC_CONST_UNSPECIFIED
#elif defined COMM_STOP_SEC_CONST_UNSPECIFIED
    #undef COMM_STOP_SEC_CONST_UNSPECIFIED
    #define STOP_SEC_CONST_UNSPECIFIED

#elif defined COMM_START_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
    #undef COMM_START_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMBUSSMMODEINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
    #undef COMM_START_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMBUSSMBUSSLEEPMODE_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMCOMCBK_CALLBACK_CODE
    #undef COMM_START_SEC_COMMCOMCBK_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMCOMCBK_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMCOMCBK_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMDCMACTIVEDIAGNOSTIC_CALLBACK_CODE
    #undef COMM_START_SEC_COMMDCMACTIVEDIAGNOSTIC_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMDCMACTIVEDIAGNOSTIC_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMDCMACTIVEDIAGNOSTIC_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMDCMINACTIVEDIAGNOSTIC_CALLBACK_CODE
    #undef COMM_START_SEC_COMMDCMINACTIVEDIAGNOSTIC_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMDCMINACTIVEDIAGNOSTIC_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMDCMINACTIVEDIAGNOSTIC_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
    #undef COMM_START_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMECUMWAKEUPINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
    #undef COMM_START_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMECUMPNCWAKEUPINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
    #undef COMM_START_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMCOMMUNICATIONALLOWED_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMNMNETWORKSTARTINDICATION_CALLBACK_CODE
    #undef COMM_START_SEC_COMMNMNETWORKSTARTINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMNMNETWORKSTARTINDICATION_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMNMNETWORKSTARTINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMNMNETWORKMODE_CALLBACK_CODE
    #undef COMM_START_SEC_COMMNMNETWORKMODE_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMNMNETWORKMODE_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMNMNETWORKMODE_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMNMPREPAREBUSSLEEPMODE_CALLBACK_CODE
    #undef COMM_START_SEC_COMMNMPREPAREBUSSLEEPMODE_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMNMPREPAREBUSSLEEPMODE_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMNMPREPAREBUSSLEEPMODE_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMNMBUSSLEEPMODE_CALLBACK_CODE
    #undef COMM_START_SEC_COMMNMBUSSLEEPMODE_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMNMBUSSLEEPMODE_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMNMBUSSLEEPMODE_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#elif defined COMM_START_SEC_COMMNMRESTARTINDICATION_CALLBACK_CODE
    #undef COMM_START_SEC_COMMNMRESTARTINDICATION_CALLBACK_CODE
    #define START_SEC_CALLBACK_CODE
#elif defined COMM_STOP_SEC_COMMNMRESTARTINDICATION_CALLBACK_CODE
    #undef COMM_STOP_SEC_COMMNMRESTARTINDICATION_CALLBACK_CODE
    #define STOP_SEC_CALLBACK_CODE

#else
#error "COMM not include this section"
#endif
#include "MemMap.h"
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
