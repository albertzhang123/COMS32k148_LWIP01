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
**  Created on  : 2022/06/02 13:42:34                                                                                 **
**  Author      :                                                                                                     **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : tcpip module link-time configuration data                                                           **
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

/***********************************************************************************************************************
**                                                  Includes                                                          **
***********************************************************************************************************************/
#include "TcpIp_Lcfg.h"
#include "SoAd_Cbk.h"

#define TCPIP_START_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#include "TcpIp_MemMap.h"
CONST(TcpIp_SocketOwnerLCfgType,TCPIP_CONST) SocketOwnerLCfg[1] =
{
    {
    		TCPIP_SOCKETOWNER_ID_TCPIPSOCKETOWNER,
			&SoAd_CopyTxData,
			&SoAd_LocalIpAddrAssignmentChg,
			&SoAd_RxIndication,
			&SoAd_TcpAccepted,
			&SoAd_TcpConnected,
			&SoAd_TcpIpEvent,
			&SoAd_TxConfirmation
    }
};


CONST(TcpIp_LConfigType,TCPIP_CONST) TcpIp_LCfg =
{
    &SocketOwnerLCfg[0],
    1u
};
#define TCPIP_STOP_SEC_CONFIG_DATA_PREBUILD_UNSPECIFIED
#include "TcpIp_MemMap.h"

