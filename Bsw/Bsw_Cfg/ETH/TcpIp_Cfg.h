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
**  FILENAME    : TcpIp_Cfg.h                                                                                         **
**                                                                                                                    **
**  Created on  : 2022/06/02 13:42:34                                                                                 **
**  Author      :                                                                                                     **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : TcpIp pre-compile globle macro define                                                               **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

#ifndef TCPIP_CFG_H_
#define TCPIP_CFG_H_

/***********************************************************************************************************************
**                                                  Includes                                                          **
***********************************************************************************************************************/
#include "Std_Types.h"
/***********************************************************************************************************************
**                                             Version  infomation                                                    **
***********************************************************************************************************************/
#define TCPIP_CFG_H_AR_MAJOR_VERSION                            19u
#define TCPIP_CFG_H_AR_MINOR_VERSION                            11u
#define TCPIP_CFG_H_AR_PATCH_VERSION                            0u
#define TCPIP_CFG_H_SW_MAJOR_VERSION                            2u
#define TCPIP_CFG_H_SW_MINOR_VERSION                            0u
#define TCPIP_CFG_H_SW_PATCH_VERSION                            0u

/***********************************************************************************************************************
**                                              Inner Symbols                                                         **
***********************************************************************************************************************/

#define TCPIP_SC1                                               1u
#define TCPIP_SC2                                               2u
#define TCPIP_SC3                                               3u

/***********************************************************************************************************************
**                                               Global Symbols                                                       **
***********************************************************************************************************************/

/* Memory size in bytes reserved for TCP/IP buffers */
#define TCPIP_BUFFER_MEMORY                                     32000u
/* Switches the Default Error Tracer (Det) detection and notification */
#define TCPIP_DEV_ERROR_DETECT                                  STD_ON
/* Enables (TRUE) or disables (FALSE) the DHCP (Dynamic Host
Configuration Protocol) Server  */
#define TCPIP_DHCP_SERVER_ENABLED                               STD_OFF
/* Period of TcpIp_MainFunction in [ms] */
#define TCPIP_MAINFUNCTION_PERIOD                               10u
/* Enables/disables the API TcpIp_ResetIpAssignment of a DHCP-client */
#define TCPIP_RESET_IP_ASSIGNMENT_API                           STD_OFF
/* In order to customize the TcpIp Stack to the specific needs of the
user it can be scaled according to the scalability classes */
#define TCPIP_SCALABILITY_CLASS                                 TCPIP_SC1
/* Enables (TRUE) or disabled (FALSE) support of TCP (Transmission
Control Protocol) */
#define TCPIP_TCP_ENABLED                                       STD_ON
/* Maximum number of TCP sockets */
#define TCPIP_TCP_SOCKET_MAX                                    10u
/* Enables (TRUE) or disabled (FALSE) support of UDP (User Datagram
Protocol) */
#define TCPIP_UDP_ENABLED                                       STD_ON
/* Maximum number of UDP sockets */
#define TCPIP_UDP_SOCKET_MAX                                    10u
/* If true the TcpIp_GetVersionInfo API is available */
#define TCPIP_VERSION_INFO_API                                  STD_OFF

/* This container is a subcontainer of TcpIp and specifies the
general configuration parameters of the TCP/IP stack for IPv4 */
/************************* TcpIpIpV4General ***************************/

/* Enables (TRUE) or disables (FALSE) support of ARP (Address Resolution
Protocol) */
#define TCPIP_ARP_ENABLED                                       STD_ON
/* Enables (TRUE) or disables (FALSE) the Auto-IP (automatic private IP
addressing) sub-module */
#define TCPIP_AUTOIP_ENABLED                                    STD_OFF
/* Enables (TRUE) or disables (FALSE) the DHCP (Dynamic Host
Configuration Protocol) Client */
#define TCPIP_DHCP_CLIENT_ENABLED                               STD_OFF
/* Enables (TRUE) or disabled (FALSE) support of ICMP (Internet Control
Message Protocol) */
#define TCPIP_ICMP_ENABLED                                      STD_ON
/* Enables (TRUE) or disables (FALSE) support of IPv4 (Internet Protocol
version 4) */
#define TCPIP_IPV4_ENABLED                                      STD_ON
/* Maximum number of LocalAddr table entries for IPv4 */
#define TCPIP_LOCAL_ADDR_IPV4_ENTRIES_MAX                       1u
/* Enables (TRUE) or disables (FALSE) the discovery of the maximum
transmission unit on a path according to IETF RfC 1191 */
#define TCPIP_PATH_MTU_DISCOVERY_ENABLED                        STD_OFF     

/* This container is a subcontainer of TcpIp and specifies the
general configuration parameters of the TCP/IP stack for IPv6 */
/************************* TcpIpIpV6General***************************/

/* Enables (TRUE) or disables (FALSE) the DHCPv6 (Dynamic Host
Configuration Protocol for IPv6) Client */
#define TCPIP_DHCPV6_CLIENT_ENABLED                             STD_OFF
/* Enables (TRUE) or disables (FALSE) support of IPv6 (Internet Protocol
version 6) */
#define TCPIP_IPV6_ENABLED                                      STD_OFF
/* Enables (TRUE) or disables (FALSE) Path MTU Discovery support for
IPv6 according to IETF RFC 1981. */
#define TCPIP_IPV6_PATH_MTU_DISCOVERY_ENABLED                   STD_OFF
/* Maximum number of LocalAddr table entries for IPv6 */
#define TCPIP_LOCAL_ADDR_IPV6_ENTRIES_MAX                       0u
/* Enables (TRUE) or disables (FALSE) support of Address Resoultion and
Neighbor Unreachability Detetion via NDP */
#define TCPIP_NDPADDRESSRESOLUTIONUNRECHABILITYDETECTIONENABLED  STD_OFF
/* Enables (TRUE) or disables (FALSE) support of Prefix and Router
Discovery via NDP */
#define TCPIP_NDPPREFIXANDROUTERDISCOVERY_ENABLED               STD_OFF
/* Enables (TRUE) or disables (FALSE) DEM module */
#define TCPIP_DEM_SUPPORT                                       STD_OFF
/* Enables (TRUE) or disables (FALSE) NVM module*/
#define TCPIP_NVM_SUPPORT                                       STD_OFF
#define TCPIP_ETHSM_SUPPORT                                     STD_ON
#if !defined(TCIP_USED_MOUDLE_DET)
#define TCIP_USED_MOUDLE_DET                                    STD_ON
#endif/* !defined(TCIP_USED_MOUDLE_DET) */
#if !defined(TCPIP_INITIALIZATION_AVAILABLE)
#define TCPIP_INITIALIZATION_AVAILABLE                          STD_ON
#endif/* !defined(TCPIP_INITIALIZATION_AVAILABLE) */
/* the number of TcpIpLocalAddr*/
#define TCPIP_LOCAL_ADR_NUM                                     1u
/*the number of controller*/
#define TCPIP_CONTROLLER_NUM                                    1u
#define TCPIP_MUILT_SOCKET_OWNERCFG                             STD_OFF
#define TCPIP_SOCKETOWNER_ID_TCPIPSOCKETOWNER                   0
#define TCPIP_TCPIPLOCALADDR_0                                0
#endif /* TCPIP_CFG_H_ */

