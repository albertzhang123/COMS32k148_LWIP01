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
**  FILENAME    : SoAd_Cfg.h                                                  **
**                                                                            **
**  Created on  : 2022/06/02 17:33:12                                         **
**  Author      : zhengfei.li                                                 **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR classic Platform 4.2.2                         **
**                                                                            **
*******************************************************************************/
#ifndef SOAD_CFG_H_
#define SOAD_CFG_H_
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
**                       Version  infomation                                  **
*******************************************************************************/
#define SOAD_CFG_H_AR_MAJOR_VERSION                                       4u
#define SOAD_CFG_H_AR_MINOR_VERSION                                       2u
#define SOAD_CFG_H_AR_PATCH_VERSION                                       2u
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define SOAD_DEV_ERROR_DETECT                                           STD_OFF

#define SOAD_IPV6_ADDRESS_ENABLED                                      STD_OFF

#define SOAD_MAIN_FUNCTION_PERIOD                                       10u

#define SOAD_ROUTING_GROUP_MAX                                          0u

#define SOAD_SO_CON_MAX                                                 4u

#define SOAD_PDU_ROUTE_MAX                                              4u

#define SOAD_UdpNm_Tx_Udp                                             0u                    
#define SOAD_SoAd_ECU_18FEF121                                             1u                    
#define SOAD_SOAD_LDCOM_ETH_TX                                             2u                    
#define SOAD_SoAd_Ecu_Gw_Tx_20020                                             3u                    

#define SOAD_SOCKET_ROUTE_MAX                                           3u

#define SOAD_UdpNm_Rx_Udp                                             0u                    
#define SOAD_SOAD_LDCOM_ETH_RX                                             1u                    
#define SOAD_SoAd_Ecu_TcpServer_Rx_20020                                             2u                    

#define SOAD_VERSION_INFO_API                                           STD_OFF

/*******************************************************************************
**                Not classic AutoSar sepcification define                    **
*******************************************************************************/

#define SOAD_SO_CON_GROUP_MAX                                           4u

#define SOAD_PDU_ROUTE_DEST_MAX                                         4u

#define SOAD_SOCKET_ROUTE_DEST_MAX                                      3u

#define SOAD_SUPPORT_UPPERLAYMODULE_MAX                                 2u

#define SOAD_PDUR                                                       STD_ON

#define SOAD_UDPNM                                                      STD_ON

#define SOAD_XCP                                                        STD_OFF

#define SOAD_SD                                                         STD_OFF

#define SOAD_DOIP                                                       STD_OFF

#define SOAD_CDD                                                        STD_OFF

/*******************************************************************************
**                               Function support                             **
*******************************************************************************/

#define SOAD_SUPPORT_UDP                                                STD_ON

#define SOAD_SUPPORT_TCP                                                STD_ON

#define SOAD_SUPPORT_HEADERID                                           STD_ON

#define SOAD_SUPPORT_NPDUUDPTXBUFFER                                    STD_OFF

#define SOAD_MAX_BUFFER_SIZE                                            0u

#define SOAD_MAX_IF_PDU_SIZE                                            1008u

#define SOAD_TXBUFFER_NUM                                               0u

#define SOAD_RXBUFFER_NUM                                               0u

#endif /* SOAD_CFG_H_ */



