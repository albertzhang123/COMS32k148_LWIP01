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
**  FILENAME    : TcpIp_Externals.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : pengfei.zhu                                                 **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : External functions definition                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20200828]  [pengfei.zhu]   Add Tcp_Tls function.
 *  V2.0.1    [20200928]  [pengfei.zhu]   QAC test.
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "TcpIp_Externals.h"

#if (TCPIP_TCP_TLS_ENABLED == STD_ON)


FUNC(void, TCPIP_CODE)
Rte_IcmpMsgHandler(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) RemoteAddrPtr,
    VAR(uint8, AUTOMATIC) Ttl,
    VAR(uint8, AUTOMATIC) Type,
    VAR(uint8, AUTOMATIC) Code,
    VAR(uint16, AUTOMATIC) DataLength,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr
)
{
}


FUNC(void, TCPIP_CODE)
Rte_DADAddressConflict(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) IpAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) LocalPhyAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) RemotePhyAddrPtr
)
{
}


FUNC(Std_ReturnType, TCPIP_CODE)
Rte_TlsGetCurrentTimeStamp(
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_CONST) CurrentTimeUtc
)
{
    return 0;
}

FUNC(Std_ReturnType, TCPIP_CODE)
Rte_TlsServerGetPskIdentityHint(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_TlsConnectionIdType, AUTOMATIC) TlsConnectionId,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_CONST) IdentityHintLengthPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_CONST) IdentityHintPtr
)
{
    return 0;
}


FUNC(Std_ReturnType, TCPIP_CODE)
Rte_TlsClientGetPskIdentity(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_TlsConnectionIdType, AUTOMATIC) TlsConnectionId,
    VAR(uint16, AUTOMATIC) PskIdentityHintLength,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) PskIdentityHintPtr,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_CONST) PskKeyIdentityLengthPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_CONST) PskKeyIdentityPtr,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_CONST) CsmKeyId
)
{
    return 0;
}



FUNC(Std_ReturnType, TCPIP_CODE)
Rte_TlsServerGetPskIdentity(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_TlsConnectionIdType, AUTOMATIC) TlsConnectionId,
    VAR(uint16, AUTOMATIC) PskKeyIdentityPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) PskKeyIdentity,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_CONST) CsmKeyId
)
{
    return 0;
}

#endif/* (TCPIP_TCP_TLS_ENABLED == STD_ON) */




