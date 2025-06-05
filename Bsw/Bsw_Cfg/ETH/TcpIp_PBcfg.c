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
**  FILENAME    : TcpIp_PBcfg.h                                                                                       **
**                                                                                                                    **
**  Created on  : 2022/06/02 13:42:34                                                                                 **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : TcpIp post-build configuration                                                                      **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                                  Includes                                                          **
***********************************************************************************************************************/
#include "TcpIp_PBcfg.h"

#define TCPIP_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "TcpIp_MemMap.h"



 
CONST(TcpIp_ArpConfigType,TCPIP_CONST) ArpPbCfg[1] =
{
    {
        0x1u,
        0xea60u
    }
};





CONST(TcpIp_CtrlType,TCPIP_CONST) CtrlPbCfg[1] =
{
    {
        0x0u,
        NULL_PTR,
        &ArpPbCfg[0],
        0x0u,
        0x0u
    }
};



CONST(TcpIp_AddrAssignmentType,TCPIP_CONST) AddrAssignmentPbCfg[1] =
{
    {
        NULL_PTR,
        TCPIP_IPADDR_ASSIGNMENT_STATIC,
        0x1u,
        TCPIP_AUTOMATIC
    }
};



CONST(uint32,TCPIP_CONST) StaticIpPbCfg_0[1] = {0xc800a8c0u};

CONST(TcpIp_StaticIpAddressConfigType,TCPIP_CONST) StaticIpAddressPbCfg[1] =
{
    {
        NULL_PTR,
        NULL_PTR,
        &StaticIpPbCfg_0[0]
    }
};

CONST(TcpIp_LocalAddrType,TCPIP_CONST) LocalAdrPbCfg[TCPIP_LOCAL_ADR_NUM] =
{
    {
        TCPIP_UNICAST,
        TCPIP_AF_INET,
        0x0u,
        0x0u,
        &AddrAssignmentPbCfg[0],
        0x1u,
        &StaticIpAddressPbCfg[0]
    }
};


CONST(TcpIp_TcpConfigType,TCPIP_CONST) TcpPbCfg =
{
    0x4e20u,
    0x124f8u,
    0x9u,
    0x6ddd00u,
    NULL_PTR,
    0xea60u,
    0x860u,
    NULL_PTR,
    0x6u,
    0x4e20u,
    0xFFu
};

CONST(TcpIp_UdpConfigType,TCPIP_CONST) UdpPbCfg =
{
    0xFFu
};

CONST(TcpIp_ConfigType,TCPIP_CONST) TcpIp_Config =
{
    &CtrlPbCfg[0],    
    &LocalAdrPbCfg[0],
    &TcpPbCfg,
    &UdpPbCfg,
    1u,
    1u
};
#define TCPIP_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "TcpIp_MemMap.h"

