/***********************************************************************************************************************
**                                                                                                                    **
** Copyright (C) iSOFT   (2021)                                                                                       **
**                                                                                                                    **
** All rights reserved.                                                                                               **
**                                                                                                                    **
** This document contains proprietary information belonging to iSOFT.                                                 **
** Passing on and copying of this document, and communication of its contents is not permitted                        **
** without prior written authorization.                                                                               **
**                                                                                                                    **
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : TcpIp_Lcfg.h                                                                                        **
**                                                                                                                    **
**  Created on  : 2022/06/02 13:42:34                                                                         **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : link-time data type definition                                                                      **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20200828]  [pengfei.zhu]   Update to R19-11.
 **********************************************************************************************************************/
#ifndef TCPIP_LCFG_H_
#define TCPIP_LCFG_H_

/***********************************************************************************************************************
 *  INCLUDES
***********************************************************************************************************************/

#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_Types.h"

BEGIN_C_DECLS

typedef void (*TcpIp_User_IcmpMsgHandlerType)(
    TcpIp_LocalAddrIdType LocalAddrId,
    const TcpIp_SockAddrType *RemoteAddrPtr,
    uint8 Ttl,
    uint8 Type,
    uint8 Code,
    uint16 DataLength,
    uint8 *DataPtr);

/* socket callback function typedef */

typedef void (*TcpIp_Up_RxIndicationType)(
    TcpIp_SocketIdType SocketId,
    const TcpIp_SockAddrType *RemoteAddrPtr,
    uint8 *BufPtr,
    uint16 Length);

typedef void (*TcpIp_Up_TcpIpEventType)(
    TcpIp_SocketIdType SocketId,
    TcpIp_EventType Event);

typedef void (*TcpIp_Up_TxConfirmationType)(
    TcpIp_SocketIdType SocketId,
    uint16 Length);

typedef Std_ReturnType (*TcpIp_Up_TcpAcceptedType)(
    TcpIp_SocketIdType SocketId,
    TcpIp_SocketIdType SocketIdConnected,
    const TcpIp_SockAddrType *RemoteAddrPtr);

typedef void (*TcpIp_Up_TcpConnectedType)(
    TcpIp_SocketIdType SocketId);

typedef BufReq_ReturnType (*TcpIp_Up_SocketOwnerCopyTxDataType)(
    TcpIp_SocketIdType SocketId,
    uint8 *BufPtr,
    uint16 BufLength);

typedef void (*TcpIp_Up_LocalIpAddrAssignmentChgType)(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) IpAddrId,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) State);
/* Specifies the configuration parameters of the ARP (Address Resolution Protocol) sub-module */
typedef struct
{
    /* Maximum number of entries in the ARP table */
    uint16 ArpTableSizeMax;
} TcpIp_ArpLConfigType; 
/* Specifies the configuration parameters of the ICMP (Internet Control Message Protocol) sub-module */
typedef struct
{
    /* ICMP message handler function */
    TcpIp_User_IcmpMsgHandlerType User_IcmpMsgHandlerFuncPtr;
} TcpIp_IcmpLConfigType;

/* Specifies the configuration parameters of the IPv4 (Internet Protocol version 4) sub-module */
typedef struct
{
    P2CONST(TcpIp_ArpLConfigType, TYPEDEF, TCPIP_CONST)    ArpLCfgPtr; 

    P2CONST(TcpIp_IcmpLConfigType, TYPEDEF, TCPIP_CONST)    IcmpLCfgPtr; 

} TcpIp_IpV4LCfgType;
 
 

/* Specifies the upper layer modules of TcpIp using the socket API */
/* shall show TcpIpSocketOwnerUpperLayerType */
typedef struct
{
    /* This value specifies the ID of the socket user */
    uint32          TcpIpSocketOwnerId;
    /* This parameter defines the name of the <Up_CopyTxData> function of the TcpIpSocketOwner module */
    TcpIp_Up_SocketOwnerCopyTxDataType Up_CopyTxDataFuncPtr;
    /* This parameter defines the name of the <Up_LocalIpAddrAssignmentChg> function of the TcpIpSocketOwner module */
    TcpIp_Up_LocalIpAddrAssignmentChgType Up_LocalIpAddrAssignmentChgFuncPtr;
    /* This parameter defines the name of the <Up_RxIndication> function of the TcpIpSocketOwner module */
    TcpIp_Up_RxIndicationType Up_RxIndicationFuncPtr;
    /* This parameter defines the name of the <Up_TcpAccepted> function of the TcpIpSocketOwner module */
    TcpIp_Up_TcpAcceptedType Up_TcpAcceptedFuncPtr;
    /* This parameter defines the name of the <Up_TcpConnected> function of the TcpIpSocketOwner module */
    TcpIp_Up_TcpConnectedType Up_TcpConnectedFuncPtr;
    /* This parameter defines the name of the <Up_TcpIpEvent> function of the TcpIpSocketOwner module */
    TcpIp_Up_TcpIpEventType Up_TcpIpEventFuncPtr;
    /* This parameter defines the name of the <Up_TxConfirmation> function of the TcpIpSocketOwner module */
    TcpIp_Up_TxConfirmationType Up_TxConfirmationFuncPtr;
} TcpIp_SocketOwnerLCfgType;

typedef struct
{ 
    /* Specifies the upper layer modules of TcpIp using the socket API */
    P2CONST(TcpIp_SocketOwnerLCfgType, TYPEDEF, TCPIP_CONST) SocketOwnerLCfgPtr;
    VAR(uint8,TCPIP_CONST) SocketOwnerNum;
} TcpIp_LConfigType;

#define TCPIP_START_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#include "TcpIp_MemMap.h"
extern CONST(TcpIp_LConfigType, TCPIP_CONST) TcpIp_LCfg;
#define TCPIP_STOP_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#include "TcpIp_MemMap.h"

END_C_DECLS
#endif /* TCPIP_LCFG_H_ */

