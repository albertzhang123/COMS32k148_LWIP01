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
 **********************************************************************************************************************/
#ifndef TCPIP_PBCFG_H_
#define TCPIP_PBCFG_H_
/***********************************************************************************************************************
**                                                  Includes                                                          **
***********************************************************************************************************************/

#include "TcpIp_Types.h"
#include "TcpIp_Cfg.h"


BEGIN_C_DECLS

typedef uint8 TcpIp_IpFramePrioType;

typedef uint32 TcpIp_IpAddrType;




/* Specifies the configuration parameters of the ARP (Address Resolution Protocol) sub-module */
typedef struct
{
    /* Specifies the number of gratuitous ARP replies which shall be sent on assignment of a new IP address */
    uint8 ArpNumGratuitousARPonStartup;
    /* Timeout in ms after which an unused ARP entry is removed */
    uint32 ArpTableEntryTimeout;
} TcpIp_ArpConfigType;







/* Specifies the EthIf controller used for IP communication and TcpIp errors that shall be reported to DEM */
typedef struct
{
    /* Reference to EthIf controller where the IP address shall be assigned */
    TcpIp_RefType EthIfCtrlRef;
    /* Specifies the default value for the priority for all outgoing frames */
    P2CONST(TcpIp_IpFramePrioType, TYPEDEF, TCPIP_CONFIG_DATA)     IpFramePriDefPtr;
 
    /* Specifies the configuration parameters of the IPv4 (Internet Protocol version 4) sub-module */
    /* Specifies the configuration parameters of the ARP (Address Resolution Protocol) sub-module */
    P2CONST(TcpIp_ArpConfigType, TYPEDEF, TCPIP_CONFIG_DATA) ArpCfgPtr;    

    TcpIp_LocalAddrIdType TcpIpLocalAddrStartId;
    TcpIp_LocalAddrIdType TcpIpLocalAddrStopId;
} TcpIp_CtrlType; 

typedef enum
{
    TCPIP_MULTICAST,
    TCPIP_UNICAST,
} TcpIp_AddressType;

/* Defines the lifetime of a dynamically fetched IP address */
typedef enum
{
    TCPIP_FORGET,
    TCPIP_STORE,
} TcpIp_AssignmentLifetimeType;

/* Trigger of address assignment */
typedef enum
{
    TCPIP_AUTOMATIC,
    TCPIP_MANUAL,
} TcpIp_AssignmentTriggerType;

typedef struct
{
    /* Defines the lifetime of a dynamically fetched IP address */
    P2CONST(TcpIp_AssignmentLifetimeType, TYPEDEF, TCPIP_CONFIG_DATA) AssignmentLifetimeTypePtr;
    /* Method of address assignment */
    TcpIp_IpAddrAssignmentType AssignmentMethod;
    /*  Priority of assignment (1 is highest) */
    uint8 TcpIp_AssignmentPriority;
    /* Trigger of address assignment */
    TcpIp_AssignmentTriggerType AssignmentTrigger;

} TcpIp_AddrAssignmentType;


/* This container is a subcontainer of TcpIpLocalAddr and specifies a
static IP address including directly related parameters */
typedef struct
{
    /* IP address of default router (gateway) */
    P2CONST(TcpIp_IpAddrType, TYPEDEF, TCPIP_CONFIG_DATA) DefaultRouterPtr;
    /* Network mask of IPv4 address or address prefix of IPv6 address in CIDR Notation */
    P2CONST(TcpIp_IpAddrType, TYPEDEF, TCPIP_CONFIG_DATA) NetmaskPtr;
    /* Static IP Address */
    P2CONST(TcpIp_IpAddrType, TYPEDEF, TCPIP_CONFIG_DATA) StaticIpAddressPtr;

} TcpIp_StaticIpAddressConfigType;


/* Specifies the local IP (Internet Protocol) addresses used for IP communication */
typedef struct
{
    /* Address type */
    TcpIp_AddressType AddrType;
    /* Address family */
    TcpIp_DomainType DomainType;
    /* Reference to a TcpIpCtrl specifying the EthIf Controller where the IP address shall be assigned and DEM errors
    that shall be reported in case of an error on this controller */
    TcpIp_RefType CtrlRef;
    /* local module inner controller index */
    uint8 ctrlIndex;
    /* This container is a subcontainer of TcpIpLocalAddr and specifies the assignment policy for the IP address */
    P2CONST(TcpIp_AddrAssignmentType, TYPEDEF, TCPIP_CONFIG_DATA) AddrAssignmentCfgPtr;
    /*the number of assignment policy*/
    uint8 AddrAssignmentNum;
    /* This container is a subcontainer of TcpIpLocalAddr and specifies a static IP address including directly related
    parameters */
    P2CONST(TcpIp_StaticIpAddressConfigType, TYPEDEF, TCPIP_CONFIG_DATA) StaticIpAddressCfgPtr;

} TcpIp_LocalAddrType;

/* Specifies the configuration parameters of the TCP (Transmission Control Protocol) sub-module */
typedef struct
{

    /* Timeout in [ms] to receive a FIN from the remote node (after this node has initiated connection termination),
    i.e. maximum time waiting in FINWAIT-2 for a connection termination request from the remote TCP */
    uint32 TcpFinWait2Timeout;
    /* Specifies the interval in [ms] between subsequent keepalive probes */
    uint32 TcpKeepAliveInterval;
    /*  Maximum number of times that a TCP Keep Alive is retransmitted before the connection is closed */
    uint16 TcpKeepAliveProbesMax;
    /* Specifies the time in [ms] between the last data packet sent (simple ACKs are not considered data) and the
    first keepalive probe */
    /* default value : 7200 */
    uint32 TcpKeepAliveTime;
    /* Maximum number of times that a TCP segment is retransmitted before the TCP connection is closed */
    P2CONST(uint8, TYPEDEF, TCPIP_CONFIG_DATA) TcpMaxRtxPtr;
    /* Maximum segment lifetime in [ms] */
    uint32 TcpMsl;
    /* Default value of maximum receive window in bytes */
    uint16 TcpReceiveWindowMax;
    /* Timeout in [ms] before an unacknowledged TCP segment is sent again */
    P2CONST(uint32, TYPEDEF, TCPIP_CONFIG_DATA) TcpRetransmissionTimeout;
    /* Maximum number of times that a TCP SYN is retransmitted */
    uint8 TcpSynMaxRtx;
    /* Timeout in [ms] to complete a remotely initiated TCP connection establishment, i.e. maximum time waiting in
    SYN-RECEIVED for a confirming connection request acknowledgment after having both received and sent a connection
    request */
    uint32 TcpSynReceivedTimeout;

    /* Default Time-to-live value of outgoing TCP packets */
    uint8 TcpTtl;

} TcpIp_TcpConfigType;

/* Specifies the configuration parameters of the UDP (User Datagram Protocol) sub-module */
typedef struct
{
    /* Default Time-to-live value of outgoing UDP packets */
    uint8 UdpTtl;
} TcpIp_UdpConfigType;



/* This container contains the configuration parameters and sub containers of the AUTOSAR TcpIp module */
typedef struct
{
    /* Specifies the EthIf controller used for IP communication and TcpIp errors that shall be reported to DEM */
    P2CONST(TcpIp_CtrlType, TYPEDEF, TCPIP_CONFIG_DATA) CtrlPtr;

    /* Specifies the local IP (Internet Protocol) addresses used for IP communication */
    P2CONST(TcpIp_LocalAddrType, TYPEDEF, TCPIP_CONFIG_DATA) LocalAdrPtr;

    /* Specifies the configuration parameters of the TCP (Transmission Control Protocol) sub-module */
    P2CONST(TcpIp_TcpConfigType, TYPEDEF, TCPIP_CONFIG_DATA) TcpCfgPtr;

    /* Specifies the configuration parameters of the UDP (User Datagram Protocol) sub-module */
    P2CONST(TcpIp_UdpConfigType, TYPEDEF, TCPIP_CONFIG_DATA) UdpCfgPtr;
    

    VAR(uint32, TCPIP_CONFIG_DATA) ctrlNum;
    VAR(uint32, TCPIP_CONFIG_DATA) localAdrNum;
} TcpIp_ConfigType;



END_C_DECLS
#endif /* TCPIP_PBCFG_H_ */


