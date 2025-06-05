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
**  FILENAME    : cketOwner.h                                                                                         **
**                                                                                                                    **
**  Created on  : 2022/06/02 13:42:34                                                                                 **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : TcpIp socket owner configuration api                                                                **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20210201]  [darren.zhang]      frist create
 **********************************************************************************************************************/

#ifndef TCPIP_SOCKETOWNER_H_
#define TCPIP_SOCKETOWNER_H_

BEGIN_C_DECLS
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_GetSocket(
    VAR(TcpIp_DomainType, AUTOMATIC) Domain,
    VAR(TcpIp_ProtocolType, AUTOMATIC) Protocol,
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr);
#define TcpIp_SoAdGetSocket(a, b, c) TcpIp_GetSocket(a, b, c)

END_C_DECLS

#endif/* TCPIP_SOCKETOWNER_H_ */

