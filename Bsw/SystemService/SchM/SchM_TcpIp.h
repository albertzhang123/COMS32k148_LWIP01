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
**  FILENAME    : TcpIp_Internal.h                                                                                    **
**                                                                                                                    **
**  Created on  : 03/12/18                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : internal type definition for TcpIp                                                                  **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/
/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20200828]  [pengfei.zhu]   Update to R19-11.
 *  V2.0.1    [20200928]  [pengfei.zhu]   QAC test.
 **********************************************************************************************************************/
#ifndef SCHM_TCPIP_H_
#define SCHM_TCPIP_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
//#include "Os.h"
 #include "Mcal.h"
/***********************************************************************************************************************
**                                          Macro Definitions                                                         **
***********************************************************************************************************************/
#define TCPIP_AREA_TX   (0)
#define TCPIP_AREA_RX   (1)
#define TCPIP_AREA_ALL  (2)

#if 1
#    define SchM_Enter_TcpIp(Instance, Exclusive_Area) SuspendAllInterrupts()
#    define SchM_Exit_TcpIp(Instance, Exclusive_Area) ResumeAllInterrupts()
#else
#    define SchM_Enter_TcpIp(Instance, Exclusive_Area)
#    define SchM_Exit_TcpIp(Instance, Exclusive_Area)
#endif

#endif /* SCHM_TCPIP_H_ */
