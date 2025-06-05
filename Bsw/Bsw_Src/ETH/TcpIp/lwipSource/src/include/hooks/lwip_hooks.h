#ifndef LWIP_HOOKS_H_
#define LWIP_HOOKS_H_

#include "TcpIp_Cfg.h"


#if TCPIP_LWIP_HOOKS


#if (STD_ON == TCPIP_TCP_SYNISN_RECALC)
u32_t TcpIp_HookTcpIsn(const ip_addr_t* local_ip, u16_t local_port, const ip_addr_t* remote_ip, u16_t remote_port);
#endif/* (STD_ON == TCPIP_TCP_SYNISN_RECALC) */


#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
struct netif* TcpIp_HookIp4InputAccept(struct netif* netifPtr, const ip_addr_t* destIpPtr);

u8_t TcpIp_HookEthArpInputForus(const struct netif* netifPtr, const ip_addr_t* destAdrPtr, ip_addr_t* srcaAdrPtr);

void TcpIp_HookGratuitousArp(struct netif* netifPtr);
const ip4_addr_t* TcpIp_HookEthArpGetGw(struct netif* netif, const ip4_addr_t* dest);

#if (STD_ON == TCPIP_UDP_ENABLED)
u8_t TcpIp_UdpTransmitLocalIpCompare(const ip_addr_t* pcbLocalAddrPtr, const struct netif* netifPtr);
#endif/* (STD_ON == TCPIP_UDP_ENABLED) */

#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */

#endif/* TCPIP_LWIP_HOOKS */

#endif/* LWIP_HOOKS_H_ */