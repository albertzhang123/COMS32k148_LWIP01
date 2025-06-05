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
**  FILENAME    : EthSM_Cfg.h                                                 **
**                                                                            **
**  Created on  : 2022/06/02 14:32:14                                         **
**  Author      :                                                             **
**  Vendor      :                                                             **
**  DESCRIPTION : Pre-compile configuration parameter of EthSM                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef ETHSM_CFG_H
#define ETHSM_CFG_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Switches the development error detection and notification on or off. */
#define ETHSM_DEV_ERROR_DETECT          STD_ON

/* Disables the API to the EthIf. */
#define ETHSM_DUMMY_MODE                STD_OFF

/* Specifies the period in ms that the MainFunction has to be triggered with.*/
#define ETHSM_MAIN_FUNCTION_PERIOD      10u

/* Enables and disables the version info API. */
#define ETHSM_VERSION_INFO_API          STD_OFF  /*EthSMVersionInfoApi */

/* EthSM Channel(EthSM Network) number */
#define ETHSM_CHANNEL_NUM               1u

/* Indicate EthSM wheather use DEM */
#define ETHSM_DEM_SUPPORT               STD_OFF

#endif /* ETHSM_CFG_H */
