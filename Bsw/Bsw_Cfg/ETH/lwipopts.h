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
**  FILENAME    : lwipopts.h                                                                                          **
**                                                                                                                    **
**  Created on  : 2022/06/02 13:42:34                                                                                 **
**  Author      :                                                                                                     **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : lwip pre compile macro define                                                                       **
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

#ifndef LWIPOPTS_H_
#define LWIPOPTS_H_


/*
  ------------------------------------
  ----- fixed configuration ----------
  ------------------------------------
*/
#define NO_SYS                          1

#define NO_SYS_NO_TIMERS                1

#define SYS_LIGHTWEIGHT_PROT            0

#define MEM_ALIGNMENT                   4

#define LWIP_SUPPORT_CUSTOM_PBUF        1

#define MEMP_NUM_PBUF                   16

#define MEMP_NUM_RAW_PCB                1

#define PBUF_POOL_SIZE                  32

#define TCP_LISTEN_BACKLOG              1

#define LWIP_NETCONN                    0

#define LWIP_SOCKET                     0

#define LWIP_STATS                      0

#define LWIP_NETIF_EXT_STATUS_CALLBACK  1
/*
   ------------------------------------
   ----- AUTOSAR configuration --------
   ------------------------------------
*/
#define AUTOSAR_MEMMAP_SUPPORT          1
#define MEM_SIZE                        32000

#define MEMP_NUM_UDP_PCB                10
#define MEMP_NUM_TCP_PCB                10
#define MEMP_NUM_TCP_PCB_LISTEN         MEMP_NUM_TCP_PCB


#define LWIP_ARP                        1
#define ARP_TABLE_SIZE                  10
#define ARP_MAXAGE                      60
#define ARP_QUEUEING                    0

#define LWIP_IPV4                       1

#define LWIP_ICMP                       1
#define LWIP_RAW                        LWIP_ICMP
#define RAW_TTL                         ICMP_TTL


#define LWIP_DHCP                       0
#define LWIP_AUTOIP                     0
#define LWIP_DHCP_AUTOIP_COOP           0


#define LWIP_UDP                        1
#define UDP_TTL                         255

#define LWIP_TCP                        1
#define TCP_TTL                         255
#define TCP_WND                         2144
#define TCP_SYNMAXRTX                   6
#define TCP_MSL   60000UL /* The maximum segment lifetime in milliseconds */
#define LWIP_TCP_KEEPALIVE              1
#define TCP_KEEPIDLE_DEFAULT            7200000UL /* Default KEEPALIVE timer in milliseconds */
#define TCP_KEEPINTVL_DEFAULT           75000UL  /* Default Time between KEEPALIVE probes in milliseconds */
#define TCP_KEEPCNT_DEFAULT             9U        /* Default Counter for KEEPALIVE probes */

#define LWIP_NETIF_HOSTNAME             0


#define LWIP_SINGLE_NETIF                 1
#define CHECKSUM_GEN_IP                 1
#define CHECKSUM_CHECK_IP               1

#define CHECKSUM_GEN_UDP                1
#define CHECKSUM_CHECK_UDP              1

#define CHECKSUM_GEN_TCP                1
#define CHECKSUM_CHECK_TCP              1

#define CHECKSUM_GEN_ICMP               1
#define CHECKSUM_CHECK_ICMP             1

#define CHECKSUM_GEN_ICMP6              1
#define CHECKSUM_CHECK_ICMP6            1

#define LWIP_TCP_PCB_NUM_EXT_ARGS       1
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif/* BYTE_ORDER */

#endif /*LWIPOPTS__H__ */

