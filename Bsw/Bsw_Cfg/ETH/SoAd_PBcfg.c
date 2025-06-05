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
**  FILENAME    : SoAd_PBcfg.c                                                **
**                                                                            **
**  Created on  : 2022/06/02 17:33:12                                         **
**  Author      : zhengfei.li                                                 **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR classic Platform 4.2.2                         **
**                                                                            **
*******************************************************************************/
#include "SoAd_Types.h"

#include "PduR.h"        
//#include "UdpNm.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) PduRoute_PduRouteDestIndex_0[1] = {0x0u};  
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) PduRoute_PduRouteDestIndex_1[1] = {0x1u};  
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) PduRoute_PduRouteDestIndex_2[1] = {0x2u};  
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) PduRoute_PduRouteDestIndex_3[1] = {0x3u};  
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_PduRouteType,SOAD_CONST) PduRouteCfg[4] =
{
    {
        SOAD_UdpNm_Tx_Udp,
        SOAD_UPPER_LAYER_IF,
        0x1u,
        &PduRoute_PduRouteDestIndex_0[0],
        SOAD_UP_MODULE_UDPNM,
		NULL_PTR,
        FALSE
    },
    {
        SOAD_SoAd_ECU_18FEF121,
        SOAD_UPPER_LAYER_IF,
        0x1u,
        &PduRoute_PduRouteDestIndex_1[0],
        SOAD_UP_MODULE_PDUR,
        PDUR_DESTPDU_SoAd_ECU_18FEF121,
        FALSE
    },
    {
        SOAD_SOAD_LDCOM_ETH_TX,
        SOAD_UPPER_LAYER_IF,
        0x1u,
        &PduRoute_PduRouteDestIndex_2[0],
        SOAD_UP_MODULE_PDUR,
        PDUR_DESTPDU_SOAD_LDCOM_ETH_TX,
        FALSE
    },
    {
        SOAD_SoAd_Ecu_Gw_Tx_20020,
        SOAD_UPPER_LAYER_IF,
        0x1u,
        &PduRoute_PduRouteDestIndex_3[0],
        SOAD_UP_MODULE_PDUR,
        PDUR_DESTPDU_SoAd_Ecu_Gw_Tx_20020,
        FALSE
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
   
     


#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(TcpIp_SockAddrType,SOAD_CONST) SoConRemoteAddrCfg[4] =
{
    {
        TCPIP_AF_INET,
        0x9c40u,
        {
            0xffffffffu
        }
    },
    {
        TCPIP_AF_INET,
        0x4e21u,
        {
            0x6400a8c0u
        }
    },
    {
        TCPIP_AF_INET,
        0x4e2bu,
        {
            0x6400a8c0u
        }
    },
    {
        TCPIP_AF_INET,
        0x0u,
        {
            0x00000000u
        }
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"


#define SOAD_START_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
CONST(SoAd_UpLayerModuleType,SOAD_CONST) SoConRefUpModule_0[1] =
{
    SOAD_UP_MODULE_UDPNM,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
CONST(SoAd_UpLayerModuleType,SOAD_CONST) SoConRefUpModule_1[1] =
{
    SOAD_UP_MODULE_PDUR,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
CONST(SoAd_UpLayerModuleType,SOAD_CONST) SoConRefUpModule_2[1] =
{
    SOAD_UP_MODULE_PDUR,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"
CONST(SoAd_UpLayerModuleType,SOAD_CONST) SoConRefUpModule_3[1] =
{
    SOAD_UP_MODULE_PDUR,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_8
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) SoConRefSocketRoute_0[1] =
{
    0x0u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) SoConRefSocketRoute_1[1] =
{
    0x1u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) SoConRefSocketRoute_2[1] =
{
    0x2u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"


#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) SoConRefPduRouteDest_0[1] =
{
    0x0u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) SoConRefPduRouteDest_1[2] =
{
    0x1u,
    0x3u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_uintx,SOAD_CONST) SoConRefPduRouteDest_2[1] =
{
    0x2u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"



#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_SocketConnectionType,SOAD_CONST) SocketConnectionCfg[4] =
{
    {
        0x0u,
        0x0u,
        &SoConRemoteAddrCfg[0],
        0x1u,
        &SoConRefUpModule_0[0],
        0x1u,
        &SoConRefSocketRoute_0[0],
        NULL_PTR,
        NULL_PTR,
        TCPIP_IPPROTO_UDP,
        0x1u,
        &SoConRefPduRouteDest_0[0],
    },
    {
        0x1u,
        0x1u,
        &SoConRemoteAddrCfg[1],
        0x1u,
        &SoConRefUpModule_1[0],
        0x0u,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        TCPIP_IPPROTO_UDP,
        0x2u,
        &SoConRefPduRouteDest_1[0],
    },
    {
        0x2u,
        0x2u,
        &SoConRemoteAddrCfg[2],
        0x1u,
        &SoConRefUpModule_2[0],
        0x1u,
        &SoConRefSocketRoute_1[0],
        NULL_PTR,
        NULL_PTR,
        TCPIP_IPPROTO_UDP,
        0x1u,
        &SoConRefPduRouteDest_2[0],
    },
    {
        0x3u,
        0x3u,
        &SoConRemoteAddrCfg[3],
        0x1u,
        &SoConRefUpModule_3[0],
        0x1u,
        &SoConRefSocketRoute_2[0],
        NULL_PTR,
        NULL_PTR,
        TCPIP_IPPROTO_TCP,
        0x0u,
        NULL_PTR,
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"













#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_SocketTcpType,SOAD_CONST) SocketTcpProtocolCfg[1] =
{
    {
        FALSE,
        FALSE,
        FALSE,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR
    },    
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"


#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_SocketUdpType,SOAD_CONST) SocketUdpProtocolCfg[3] =
{
    {
        FALSE,
        FALSE,
        NULL_PTR
    },
    {
        FALSE,
        FALSE,
        NULL_PTR
    },
    {
        FALSE,
        FALSE,
        NULL_PTR
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) SoConGroup_SoConIndex_0[1] =
{
    0x0u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) SoConGroup_SoConIndex_1[1] =
{
    0x1u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) SoConGroup_SoConIndex_2[1] =
{
    0x2u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) SoConGroup_SoConIndex_3[1] =
{
    0x3u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_SocketConnectionGroupType,SOAD_CONST) SocketConnectionGroupCfg[4] =
{
    {
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        FALSE,
        0x9c40u,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        {
            TCPIP_AF_INET,
            TCPIP_TCPIPLOCALADDR_0
        },
        {
            NULL_PTR,
            &SocketUdpProtocolCfg[0]
        },
        0x1u,
        &SoConGroup_SoConIndex_0[0]
    },
    {
        TRUE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        FALSE,
        0x4e20u,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        {
            TCPIP_AF_INET,
            TCPIP_TCPIPLOCALADDR_0
        },
        {
            NULL_PTR,
            &SocketUdpProtocolCfg[1]
        },
        0x1u,
        &SoConGroup_SoConIndex_1[0]
    },
    {
        FALSE,
        FALSE,
		TRUE,
        TRUE,
        FALSE,
        FALSE,
        0x4e2au,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        {
            TCPIP_AF_INET,
            TCPIP_TCPIPLOCALADDR_0
        },
        {
            NULL_PTR,
            &SocketUdpProtocolCfg[2]
        },
        0x1u,
        &SoConGroup_SoConIndex_2[0]
    },
    {
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        FALSE,
        0x4e34u,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        {
            TCPIP_AF_INET,
            TCPIP_TCPIPLOCALADDR_0
        },
        {
            &SocketTcpProtocolCfg[0],
            NULL_PTR
        },
        0x1u,
        &SoConGroup_SoConIndex_3[0]
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"


#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) RxSocketConnectionRef_0[1] =
{
    0x0u,    
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) RxSocketConnectionRef_1[1] =
{
    0x2u,    
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"
CONST(SoAd_SoConIdType,SOAD_CONST) RxSocketConnectionRef_2[1] =
{
    0x3u,    
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_16
#include "SoAd_MemMap.h"


#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_SocketRouteType,SOAD_CONST) SocketRouteCfg[3] =
{
    {
        NULL_PTR,
        0x0u,
        &RxSocketConnectionRef_0[0]
    },
    {
        NULL_PTR,
        0x0u,
        &RxSocketConnectionRef_1[0]
    },
    {
        NULL_PTR,
        0x1u,
        &RxSocketConnectionRef_2[0]
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
     

#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_SocketRouteDestType,SOAD_CONST) SocketRouteDestCfg[3] =
{
    {
        SOAD_UdpNm_Rx_Udp,
        SOAD_UPPER_LAYER_IF,
        0x0u,
        NULL_PTR,
        SOAD_UP_MODULE_UDPNM,
		NULL_PTR,
        FALSE
    },
    {
        SOAD_SOAD_LDCOM_ETH_RX,
        SOAD_UPPER_LAYER_IF,
        0x0u,
        NULL_PTR,
        SOAD_UP_MODULE_PDUR,
        PDUR_SRCPDU_SOAD_LDCOM_ETH_RX,
        FALSE
    },
    {
        SOAD_SoAd_Ecu_TcpServer_Rx_20020,
        SOAD_UPPER_LAYER_IF,
        0x0u,
        NULL_PTR,
        SOAD_UP_MODULE_PDUR,
        PDUR_SRCPDU_SoAd_Ecu_TcpServer_Rx_20020,
        FALSE
    },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_PBCONFIG_DATA_32
#include "SoAd_MemMap.h"
CONST(uint32,SOAD_CONST) TxPduHeaderId[2] =
{
    0xf121u,
    0x20020u,
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_32
#include "SoAd_MemMap.h"




#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_PduRouteDestType,SOAD_CONST) PduRouteDestCfg[4] =
{
     {         
         NULL_PTR,
         0x0u,
         NULL_PTR,
         0x0u,
         0x0u
     },
     {         
         &TxPduHeaderId[0],
         0x0u,
         NULL_PTR,
         0x1u,
         0x1u
     },
     {         
         NULL_PTR,
         0x0u,
         NULL_PTR,
         0x2u,
         0x2u
     },
     {         
         &TxPduHeaderId[1],
         0x0u,
         NULL_PTR,
         0x1u,
         0x3u
     },
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"


#define SOAD_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"
CONST(SoAd_ConfigType,SOAD_CONST) SoAd_Config =
{
    0x4u,
    &PduRouteCfg[0],
    0x0u,
    NULL_PTR,
    0x4u,
    &SocketConnectionCfg[0],
    0x4u,
    &SocketConnectionGroupCfg[0],
    0x3u,
    &SocketRouteCfg[0],
    0x3u,
    &SocketRouteDestCfg[0],
    0x4u,
    &PduRouteDestCfg[0],
    0x0u,
    NULL_PTR
};
#define SOAD_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "SoAd_MemMap.h"


