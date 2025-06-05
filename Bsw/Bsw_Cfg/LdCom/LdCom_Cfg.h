/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2020)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    : LdCom_Cfg.h                                                **
**                                                                           **
**  Created on  : 2022/06/02 13:43:03                                        **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION : Efficient Com for Large Data                               **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef LDCOM_CFG_H_
#define LDCOM_CFG_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "ComStack_Types.h"
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/

#define LDCOM_DEV_ERROR_DETECT                                 STD_ON


#define LDCOM_VERSION_INFO_API                                 STD_OFF


#define LDCOM_SUPPORT_SEND_IF                                  STD_ON


#define LDCOM_SUPPORT_SEND_TP                                  STD_OFF


#define LDCOM_SUPPORT_RECEIVER_IF                              STD_ON


#define LDCOM_SUPPORT_RECEIVER_TP                              STD_OFF


#define LDCOM_SUPPORT_TRIGGER_TRANSMIT                         STD_OFF


#define LDCOM_IPDU_NUMBER_MAX                                   2u

#define LdComIPdu_EthUdpTx                                       0u
#define LdComIPdu_EthUdpRx                                       1u


#endif/* LDCOM_CFG_H_ */



