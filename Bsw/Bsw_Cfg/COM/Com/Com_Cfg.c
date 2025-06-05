
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
**  FILENAME    : Com_Cfg.c                                                   **
**                                                                            **
**  Created on  : 2022/06/02 13:43:03                                         **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Configuration parameter of Com                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Com.h"
#include "Rte_Cbk.h"
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
#define COM_CFG_C_AR_MAJOR_VERSION  4u
#define COM_CFG_C_AR_MINOR_VERSION  2u
#define COM_CFG_C_AR_PATCH_VERSION  2u
#define COM_CFG_C_SW_MAJOR_VERSION  2u
#define COM_CFG_C_SW_MINOR_VERSION  0u
#define COM_CFG_C_SW_PATCH_VERSION  0u

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
#if(COM_CFG_C_AR_MAJOR_VERSION != COM_CFG_H_AR_MAJOR_VERSION)
    #error "Com.c:Mismatch in Specification Major Version"
#endif

#if(COM_CFG_C_AR_MINOR_VERSION != COM_CFG_H_AR_MINOR_VERSION)
    #error "Com.c:Mismatch in Specification Minor Version"
#endif

#if(COM_CFG_C_AR_PATCH_VERSION != COM_CFG_H_AR_PATCH_VERSION)
    #error "Com.c:Mismatch in Specification Patch Version"
#endif

#if(COM_CFG_C_SW_MAJOR_VERSION != COM_CFG_H_SW_MAJOR_VERSION)
    #error "Com.c:Mismatch in Specification Major Version"
#endif

#if(COM_CFG_C_SW_MINOR_VERSION != COM_CFG_H_SW_MINOR_VERSION)
    #error "Com.c:Mismatch in Specification Minor Version"
#endif

/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define COM_START_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"
VAR(uint8, COM_VAR) Com_TxIPduRuntimeBuff[COM_TXIPDUBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"
CONST(uint8,COM_CONST_PBCFG) Com_TxIPduInitValue[COM_TXIPDUBUFF_SIZE] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,
0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,
0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,
0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,
0x04,0x05,0x06,0x07
};    
#define COM_STOP_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"
VAR(uint8, COM_VAR) Com_RxIPduRuntimeBuff[COM_RXIPDUBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"
CONST(uint8,COM_CONST_PBCFG) Com_RxIPduInitValue[COM_RXIPDUBUFF_SIZE]= {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};    
#define COM_STOP_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
VAR(boolean, COM_VAR) Com_SignalBoolRuntimeBuff[COM_SIGNAL_BOOLBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_BOOLEAN
#include "Com_MemMap.h"
CONST(boolean,COM_CONST_PBCFG) Com_SignalBoolInitValue[COM_SIGNAL_BOOLBUFF_SIZE] = {
FALSE
};
#define COM_STOP_SEC_PBCONFIG_DATA_BOOLEAN
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"
VAR(uint8, COM_VAR) Com_Signal8BitRuntimeBuff[COM_SIGNAL_8BITBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"
CONST(uint8,COM_CONST_PBCFG) Com_Signal8BitInitValue[COM_SIGNAL_8BITBUFF_SIZE] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
0x07,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
0x07,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
0x07,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
0x07,0x00,0x00,0x00,0x00
};
#define COM_STOP_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_16
#include "Com_MemMap.h"
VAR(uint16, COM_VAR) Com_Signal16BitRuntimeBuff[COM_SIGNAL_16BITBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(uint16,COM_CONST_PBCFG) Com_Signal16BitInitValue[COM_SIGNAL_16BITBUFF_SIZE]={
0x0,0x00,0x00,0x00,0x00
};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_32
#include "Com_MemMap.h"
VAR(uint32, COM_VAR) Com_Signal32BitRuntimeBuff[COM_SIGNAL_32BITBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_32
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_32
#include "Com_MemMap.h"
CONST(uint32,COM_CONST_PBCFG) Com_Signal32BitInitValue[COM_SIGNAL_32BITBUFF_SIZE] = {
0x00,0x00,0x00
};
#define COM_STOP_SEC_PBCONFIG_DATA_32
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_64
#include "Com_MemMap.h"
VAR(uint64, COM_VAR) Com_Signal64BitRuntimeBuff[COM_SIGNAL_64BITBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_64
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_64
#include "Com_MemMap.h"
CONST(uint64,COM_CONST_PBCFG) Com_Signal64BitInitValue[COM_SIGNAL_64BITBUFF_SIZE]={
0x0,0x0,0x00,0x00,0x00
};
#define COM_STOP_SEC_PBCONFIG_DATA_64
#include "Com_MemMap.h"


#define COM_START_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"
VAR(uint8, COM_VAR) Com_RxGroupSignal8BitShadowBuff[COM_RXGROUPSIGNAL_8BITBUFF_SIZE];
#define COM_STOP_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"





#define COM_START_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"
VAR(uint8, COM_VAR) Com_GWSignal8BitBuff[COM_GWSIGNAL_8BITBUFF_SIZE]; 
#define COM_STOP_SEC_VAR_NO_INIT_8
#include "Com_MemMap.h"




#define COM_START_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"
VAR(boolean, COM_VAR) TxSignalTMCRunTime[COM_TMCTXSIGNAL_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_BOOLEAN
#include "Com_MemMap.h"
CONST(boolean,COM_CONST_PBCFG) Com_TxSignalInitTMC[COM_TMCTXSIGNAL_NUMBER]= {
    TRUE,TRUE};
#define COM_STOP_SEC_PBCONFIG_DATA_BOOLEAN
#include "Com_MemMap.h"



#define COM_START_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"
CONST(uint8,COM_CONST_PBCFG) Com_Signal8BitInvalidValue[COM_SIGNAL_8BIT_INVALID_SIZE]={
0xAA,0xAA
};
#define COM_STOP_SEC_PBCONFIG_DATA_8
#include "Com_MemMap.h"




#define COM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_RxIPduRunTimeStateType, COM_VAR) Com_RxIPduRunTimeState[COM_RXIPDU_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_RxIPduRunTimeStateType, COM_CONST) Com_RxIPduInitState[COM_RXIPDU_NUMBER]=
{
    {
        FALSE,
        FALSE,
        FALSE,
        16u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        64u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        16u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        FALSE,
        8u,
        0u,
        FALSE,
        FALSE,
        0u,
        FALSE,
        0u,
        0u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_TxIPduRunTimeStateType, COM_VAR) Com_TxIPduRunTimeState[COM_TXIPDU_NUMBER];
#define COM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxIPduRunTimeStateType, COM_CONST) Com_TxIPduInitState[COM_TXIPDU_NUMBER]=
{
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_MIXED,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_MIXED,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        16u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        16u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_MIXED,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_MIXED,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_DIRECT,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_DIRECT,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_DIRECT,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        64u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        12u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        4u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        64u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        20u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        64u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        64u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
    {
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        8u,
        0u,
        COM_TX_MODE_PERIODIC,
        0u,
        0u,
        0u,
        0u,
        FALSE,
        0u,
        0u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_IPduCounterType, COM_CONST) ComIPduCounter[6]=
{
    {
        NULL_PTR,
        8u,
        8u,
        0u,
        255u
    },
    {
        NULL_PTR,
        8u,
        8u,
        0u,
        255u
    },
    {
        NULL_PTR,
        8u,
        56u,
        0u,
        255u
    },
    {
        NULL_PTR,
        8u,
        56u,
        0u,
        255u
    },
    {
        NULL_PTR,
        8u,
        56u,
        0u,
        255u
    },
    {
        NULL_PTR,
        8u,
        56u,
        0u,
        255u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_IPduReplicationType, COM_CONST) ComIPduReplication[1] =
{
    {
        3u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_0[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_1[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_2[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_3[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_4[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_5[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_6[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_7[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_8[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_9[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_10[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_11[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_12[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_13[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_14[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_15[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_16[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComRxIPduGroupsRef_17[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalGroupIdType, COM_CONST) ComRxIPduSignalGroupsRef_0[2] = {0u,1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_0[2] = {0u,1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_1[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_2[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_3[2] = {4u,5u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_4[1] = {6u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_5[1] = {7u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_6[1] = {8u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_7[1] = {9u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_8[1] = {10u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_9[1] = {11u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_10[1] = {12u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_11[7] = {13u,14u,15u,16u,17u,18u,19u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_12[2] = {20u,21u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_13[1] = {22u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_14[1] = {23u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_15[1] = {24u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxIPduSignalsRef_16[2] = {25u,26u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_RxIPduType, COM_CONST) Com_RxIPdu[COM_RXIPDU_NUMBER]=
{
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_101,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_0[0],
        0u,
        NULL_PTR,
        2u,
        &ComRxIPduSignalsRef_0[0],
        0u,
        NULL_PTR,
        0u,
        16u,
        0u,
        0u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_117,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_1[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_1[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        16u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_10A,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_2[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_2[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        24u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_10E,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_3[0],
        0u,
        NULL_PTR,
        2u,
        &ComRxIPduSignalsRef_3[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        32u,
        NULL_PTR,
        NULL_PTR,
        TRUE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_119,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_4[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_4[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        40u,
        &ComIPduCounter[0],
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_109,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_5[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_5[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        48u,
        &ComIPduCounter[1],
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_118,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_6[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_6[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        56u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_116,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_7[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_7[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        64u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_108,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_8[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_8[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        72u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_106,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_9[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_9[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        80u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_178,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_10[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_10[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        88u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_177,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_11[0],
        0u,
        NULL_PTR,
        7u,
        &ComRxIPduSignalsRef_11[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        96u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_18FF0141,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_12[0],
        0u,
        NULL_PTR,
        2u,
        &ComRxIPduSignalsRef_12[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        104u,
        NULL_PTR,
        NULL_PTR,
        TRUE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_129_DLC64,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_13[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_13[0],
        0u,
        NULL_PTR,
        0u,
        64u,
        0u,
        112u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_124_DLC16,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_14[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_14[0],
        0u,
        NULL_PTR,
        0u,
        16u,
        0u,
        176u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_11F,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_15[0],
        0u,
        NULL_PTR,
        1u,
        &ComRxIPduSignalsRef_15[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        192u,
        NULL_PTR,
        NULL_PTR,
        FALSE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_18FF1131,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_16[0],
        0u,
        NULL_PTR,
        2u,
        &ComRxIPduSignalsRef_16[0],
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        200u,
        NULL_PTR,
        NULL_PTR,
        TRUE
    },
    {
        NULL_PTR,
        
        COM_RXPDU_COM_Tester_137,
        COM_DEFERRED,
        COM_PDU_NORMAL,
        1u,
        &ComRxIPduGroupsRef_17[0],
        2u,
        &ComRxIPduSignalGroupsRef_0[0],
        0u,
        NULL_PTR,
        0u,
        NULL_PTR,
        0u,
        8u,
        0u,
        208u,
        &ComIPduCounter[5],
        &ComIPduReplication[0],
        FALSE
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_0[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_1[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_2[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_3[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_4[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_5[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_6[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_7[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_8[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_9[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_10[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_11[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_12[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_13[1] = {0u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_14[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_15[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_16[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_17[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_18[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_19[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_20[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_21[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_22[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_23[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_24[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_25[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_26[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_27[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_28[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_IpduGroupIdType, COM_CONST) ComTxIPduGroupsRef_29[1] = {3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalGroupIdType, COM_CONST) ComTxIPduSignalGroupsRef_0[2] = {0u,1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_0[3] = {0u,46u,47u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_1[1] = {1u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_2[1] = {2u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_3[3] = {3u,48u,49u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_4[1] = {4u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_5[1] = {5u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_6[1] = {6u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_7[1] = {7u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_8[1] = {8u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_9[1] = {9u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_10[1] = {10u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_11[1] = {11u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_12[4] = {12u,13u,14u,15u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_13[4] = {16u,17u,18u,19u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_14[1] = {20u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_15[1] = {21u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_16[2] = {22u,23u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_17[1] = {24u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_18[1] = {25u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_19[1] = {26u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_20[1] = {27u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_21[1] = {28u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_22[1] = {29u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_23[1] = {30u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_24[1] = {31u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_25[1] = {32u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_26[1] = {33u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_27[9] = {34u,35u,36u,37u,38u,39u,40u,41u,42u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_28[3] = {43u,44u,45u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_29[1] = {50u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_30[1] = {51u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_31[1] = {52u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxIPduSignalsRef_32[1] = {53u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxModeType, COM_CONST) ComTxModeTrue[34]=
{ 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        4u,
        20u,
    }, 
    {
        COM_TX_MODE_MIXED,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_MIXED,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_MIXED,
        4u,
        10u,
        3u,
        200u,
    }, 
    {
        COM_TX_MODE_MIXED,
        4u,
        10u,
        2u,
        200u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        1u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_DIRECT,
        5u,
        10u,
        0u,
        0u,
    }, 
    {
        COM_TX_MODE_DIRECT,
        5u,
        10u,
        0u,
        0u,
    }, 
    {
        COM_TX_MODE_DIRECT,
        1u,
        0u,
        0u,
        0u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        3u,
        200u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        3u,
        200u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        2u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        3u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        154u,
        200u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        2u,
        200u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        34u,
        200u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        30u,
        10u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        4u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        6u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        8u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        10u,
        20u,
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxModeType, COM_CONST) ComTxModeFalse[2]=
{ 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    }, 
    {
        COM_TX_MODE_PERIODIC,
        1u,
        0u,
        0u,
        20u,
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxIPduType, COM_CONST) Com_TxIPdu[COM_TXIPDU_NUMBER]=
{
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_18FEF121,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_0[0],
        0u,
        NULL_PTR,
        3u,
        &ComTxIPduSignalsRef_0[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_18FEF121,
        0u,
        8u,
        0u,
        0u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[0],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_112,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_1[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_1[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_112,
        0u,
        8u,
        0u,
        8u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[1],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_111,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_2[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_2[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_111,
        0u,
        8u,
        0u,
        16u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[2],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_18FEF221,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_3[0],
        0u,
        NULL_PTR,
        3u,
        &ComTxIPduSignalsRef_3[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_18FEF221,
        0u,
        16u,
        0u,
        24u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[3],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_107,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_4[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_4[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_107,
        0u,
        8u,
        0u,
        40u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[4],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_103,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_5[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_5[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_103,
        0u,
        8u,
        0u,
        48u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[5],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_10F,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_6[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_6[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_10F,
        0u,
        8u,
        0u,
        56u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[6],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_11C_DLC16,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_7[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_7[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_11C_DLC16,
        0u,
        16u,
        0u,
        64u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[7],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_115,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_8[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_8[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_115,
        0u,
        8u,
        0u,
        80u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[8],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_105,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_9[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_9[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_105,
        0u,
        8u,
        0u,
        88u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[9],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_114,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_10[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_10[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_114,
        0u,
        8u,
        0u,
        96u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[10],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_104,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_11[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_11[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_104,
        0u,
        8u,
        0u,
        104u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[11],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_110,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_12[0],
        0u,
        NULL_PTR,
        4u,
        &ComTxIPduSignalsRef_12[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_110,
        0u,
        8u,
        0u,
        112u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[12],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_100,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_13[0],
        0u,
        NULL_PTR,
        4u,
        &ComTxIPduSignalsRef_13[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_100,
        0u,
        8u,
        0u,
        120u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[13],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_179,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_14[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_14[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_179,
        0u,
        8u,
        0u,
        128u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[14],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_135,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_15[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_15[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_135,
        0u,
        8u,
        0u,
        136u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[15],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_133,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_16[0],
        0u,
        NULL_PTR,
        2u,
        &ComTxIPduSignalsRef_16[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_133,
        0u,
        8u,
        0u,
        144u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[16],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_12F,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_17[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_17[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_12F,
        0u,
        8u,
        0u,
        152u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[17],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_12D,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_18[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_18[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_12D,
        0u,
        64u,
        0u,
        160u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[18],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_12C,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_19[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_19[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_12C,
        0u,
        12u,
        0u,
        224u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[19],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_12B,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_20[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_20[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_12B,
        0u,
        4u,
        0u,
        236u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[20],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_121_DLC64,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_21[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_21[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_121_DLC64,
        0u,
        64u,
        0u,
        240u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[21],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_11A,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_22[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_22[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_11A,
        0u,
        8u,
        0u,
        304u,
        &ComIPduCounter[2],
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[22],
        &ComTxModeFalse[0],
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_11B,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_23[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_23[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_11B,
        0u,
        8u,
        0u,
        312u,
        &ComIPduCounter[3],
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[23],
        &ComTxModeFalse[1],
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_11D,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_24[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_24[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_11D,
        0u,
        8u,
        0u,
        320u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[24],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_11E,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_25[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_25[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_11E,
        0u,
        8u,
        0u,
        328u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[25],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_125,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_26[0],
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_26[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_125,
        0u,
        20u,
        0u,
        336u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[26],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_122,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_27[0],
        0u,
        NULL_PTR,
        9u,
        &ComTxIPduSignalsRef_27[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_122,
        0u,
        64u,
        0u,
        356u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[27],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_123,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_28[0],
        0u,
        NULL_PTR,
        3u,
        &ComTxIPduSignalsRef_28[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_123,
        0u,
        56u,
        8u,
        420u,
        NULL_PTR,
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[28],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_136,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        1u,
        &ComTxIPduGroupsRef_29[0],
        2u,
        &ComTxIPduSignalGroupsRef_0[0],
        0u,
        NULL_PTR,
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_136,
        0u,
        8u,
        0u,
        484u,
        &ComIPduCounter[4],
        NULL_PTR,
        4u,
        COM_CONFIRMATION,
        &ComTxModeTrue[29],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_201,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        0u,
        NULL_PTR,
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_29[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_201,
        0u,
        8u,
        0u,
        492u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[30],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_211,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        0u,
        NULL_PTR,
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_30[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_211,
        0u,
        8u,
        0u,
        500u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[31],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_221,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        0u,
        NULL_PTR,
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_31[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_221,
        0u,
        8u,
        0u,
        508u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[32],
        NULL_PTR,
        0u
    },
    {
        NULL_PTR,
        
        COM_TXPDU_COM_ECU_231,
        COM_DEFERRED,
        NULL_PTR,
        COM_PDU_NORMAL,
        0u,
        NULL_PTR,
        0u,
        NULL_PTR,
        1u,
        &ComTxIPduSignalsRef_32[0],
        0u,
        NULL_PTR,
        PDUR_SRCPDU_COM_ECU_231,
        0u,
        8u,
        0u,
        516u,
        NULL_PTR,
        NULL_PTR,
        0u,
        COM_CONFIRMATION,
        &ComTxModeTrue[33],
        NULL_PTR,
        0u
    },        
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_FilterType, COM_CONST) ComFilter[4]=
{
    {
        COM_NEW_IS_WITHIN,
        0x0u,
        0x0u,
        0xfu,
        0x0u,
        0x0u,
        0x2u,
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16,
        0u
    },
    {
        COM_NEW_IS_WITHIN,
        0x0u,
        0x0u,
        0xfu,
        0x0u,
        0x0u,
        0x2u,
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16,
        1u
    },
    {
        COM_NEW_IS_WITHIN,
        0x0u,
        0x0u,
        0x5fu,
        0x30u,
        0x0u,
        0x2u,
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        COM_MASKED_NEW_EQUALS_X,
        0xf0u,
        0xf0u,
        0x0u,
        0x0u,
        0x0u,
        0x2u,
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_RxSignalType, COM_CONST) Com_RxSignal[COM_RXSIGNAL_NUMBER]=
{
    {
        64u,
        8u,
        0u,
        0u,
        0u,
        0u,
        Signal_101_1_IPDU_COM_Tester_101,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        24u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        1u,
        0u,
        Signal_101_0_IPDU_COM_Tester_101,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        25u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        2u,
        1u,
        Signal_117_IPDU_COM_Tester_117,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        26u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        &ComFilter[2],
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        3u,
        2u,
        Signal_10A_IPDU_COM_Tester_10A,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        27u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        &ComFilter[3],
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        32u,
        8u,
        0u,
        0u,
        4u,
        3u,
        Signal_10E_1_IPDU_COM_Tester_10E,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        28u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        0u
    },
    {
        0u,
        8u,
        0u,
        0u,
        5u,
        3u,
        Signal_10E_0_IPDU_COM_Tester_10E,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        29u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        6u,
        4u,
        Signal_119_byte0_IPDU_COM_Tester_119,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        30u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        7u,
        5u,
        Signal_109_byte0_IPDU_COM_Tester_109,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        31u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        8u,
        6u,
        Signal_118_IPDU_COM_Tester_118,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        32u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        9u,
        7u,
        Signal_116_IPDU_COM_Tester_116,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        33u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        10u,
        8u,
        Signal_108_IPDU_COM_Tester_108,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        34u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        11u,
        9u,
        Signal_106_IPDU_COM_Tester_106,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        35u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        56u,
        64u,
        0u,
        0u,
        12u,
        10u,
        Signal_178_IPDU_COM_Tester_178,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        1u,
        0u,
        COM_UINT64,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        56u,
        8u,
        0u,
        0u,
        13u,
        11u,
        Signal_177_byte7_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        44u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        48u,
        8u,
        0u,
        0u,
        14u,
        11u,
        Signal_177_byte6_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        45u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        40u,
        8u,
        0u,
        0u,
        15u,
        11u,
        Signal_177_byte5_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        46u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        32u,
        8u,
        0u,
        0u,
        16u,
        11u,
        Signal_177_byte4_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        47u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        24u,
        8u,
        0u,
        0u,
        17u,
        11u,
        Signal_177_byte3_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        48u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        16u,
        8u,
        0u,
        0u,
        18u,
        11u,
        Signal_177_byte2_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        49u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        8u,
        8u,
        0u,
        0u,
        19u,
        11u,
        Signal_177_byte1_IPDU_COM_Tester_177,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        50u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        32u,
        8u,
        0u,
        0u,
        20u,
        12u,
        Signal_132_1_IPDU_COM_Tester_132,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        51u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        1u
    },
    {
        0u,
        16u,
        0u,
        0u,
        21u,
        12u,
        BCM2_vVehSpd_BCM2_VEL_18FF0141,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        0u,
        0u,
        COM_UINT16,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        22u,
        13u,
        Signal_129_IPDU_COM_Tester_129_DLC64,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        52u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        23u,
        14u,
        Signal_124_IPDU_COM_Tester_124_DLC16,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        53u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        0u,
        8u,
        0u,
        0u,
        24u,
        15u,
        Signal_11F_IPDU_COM_Tester_11F,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        58u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        COM_UNUSED_UINT16
    },
    {
        46u,
        2u,
        0u,
        0u,
        25u,
        16u,
        BCM1_stParkBrkLSgn_BCM1_IO1_18FF1131,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        59u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        2u
    },
    {
        48u,
        2u,
        0u,
        0u,
        26u,
        16u,
        BCM1_stRunBrkLSgn_BCM1_IO1_18FF1131,
        COM_INVALID_ACTION_NOTIFY,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        80u,
        0u,
        COM_UINT8,
        NULL_PTR,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u,
        
        COM_UNUSED_UINT16,
        3u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxSignalType, COM_CONST) Com_TxSignal[COM_TXSIGNAL_NUMBER]=
{
    {
        2u,
        2u,
        NULL_PTR,
        BCM1_stParkBrkLSgn_GW_CCVS_18FEF121,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        4u,
        0u,
        COM_UINT8,
        0u,
        0u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_112_IPDU_COM_ECU_112,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        5u,
        0u,
        COM_UINT8,
        0u,
        1u,
        NULL_PTR,
        COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_111_IPDU_COM_ECU_111,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        6u,
        0u,
        COM_UINT8,
        0u,
        2u,
        NULL_PTR,
        COM_TRIGGERED_ON_CHANGE,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        2u,
        2u,
        NULL_PTR,
        BCM1_stParkBrkLSgn_GW_CCVS_FD_18FEF221,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        7u,
        0u,
        COM_UINT8,
        0u,
        3u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_107_IPDU_COM_ECU_107,
        FALSE,
        NULL_PTR,
        0u,
        COM_BIG_ENDIAN,
        8u,
        0u,
        COM_UINT8,
        0u,
        4u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_103_IPDU_COM_ECU_103,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        9u,
        0u,
        COM_UINT8,
        0u,
        5u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_10F_IPDU_COM_ECU_10F,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        10u,
        0u,
        COM_UINT8,
        0u,
        6u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_11C_IPDU_COM_ECU_11C_DLC16,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        11u,
        0u,
        COM_UINT8,
        0u,
        7u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_115_IPDU_COM_ECU_115,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        12u,
        0u,
        COM_UINT8,
        0u,
        8u,
        NULL_PTR,
        COM_TRIGGERED,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_105_IPDU_COM_ECU_105,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        13u,
        0u,
        COM_UINT8,
        0u,
        9u,
        NULL_PTR,
        COM_TRIGGERED,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_114_IPDU_COM_ECU_114,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        14u,
        0u,
        COM_UINT8,
        0u,
        10u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_104_IPDU_COM_ECU_104,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        15u,
        0u,
        COM_UINT8,
        0u,
        11u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        24u,
        8u,
        NULL_PTR,
        Signal_110_3_IPDU_COM_ECU_110,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        16u,
        0u,
        COM_UINT8,
        0u,
        12u,
        NULL_PTR,
        COM_TRIGGERED_WITHOUT_REPETITION,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        16u,
        8u,
        NULL_PTR,
        Signal_110_2_IPDU_COM_ECU_110,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        17u,
        0u,
        COM_UINT8,
        0u,
        12u,
        NULL_PTR,
        COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        8u,
        8u,
        NULL_PTR,
        Signal_110_1_IPDU_COM_ECU_110,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        18u,
        0u,
        COM_UINT8,
        0u,
        12u,
        NULL_PTR,
        COM_TRIGGERED_ON_CHANGE,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_110_0_IPDU_COM_ECU_110,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        19u,
        0u,
        COM_UINT8,
        0u,
        12u,
        NULL_PTR,
        COM_TRIGGERED,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        24u,
        8u,
        NULL_PTR,
        Signal_100_3_IPDU_COM_ECU_100,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        20u,
        0u,
        COM_UINT8,
        0u,
        13u,
        NULL_PTR,
        COM_TRIGGERED_WITHOUT_REPETITION,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        16u,
        8u,
        NULL_PTR,
        Signal_100_2_IPDU_COM_ECU_100,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        21u,
        0u,
        COM_UINT8,
        0u,
        13u,
        NULL_PTR,
        COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        8u,
        8u,
        NULL_PTR,
        Signal_100_1_IPDU_COM_ECU_100,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        22u,
        0u,
        COM_UINT8,
        0u,
        13u,
        NULL_PTR,
        COM_TRIGGERED_ON_CHANGE,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_100_0_IPDU_COM_ECU_100,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        23u,
        0u,
        COM_UINT8,
        0u,
        13u,
        NULL_PTR,
        COM_TRIGGERED,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        56u,
        64u,
        NULL_PTR,
        Signal_179_IPDU_COM_ECU_179,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        0u,
        0u,
        COM_UINT64,
        0u,
        14u,
        NULL_PTR,
        COM_TRIGGERED,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_135_IPDU_COM_ECU_135,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        36u,
        0u,
        COM_UINT8,
        0u,
        15u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        32u,
        8u,
        NULL_PTR,
        Signal_133_1_IPDU_COM_ECU_133,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        37u,
        0u,
        COM_UINT8,
        0u,
        16u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_133_0_IPDU_COM_ECU_133,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        38u,
        0u,
        COM_UINT8,
        0u,
        16u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_12F_IPDU_COM_ECU_12F,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        39u,
        0u,
        COM_UINT8,
        0u,
        17u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_12D_IPDU_COM_ECU_12D,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        40u,
        0u,
        COM_UINT8,
        0u,
        18u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_12C_IPDU_COM_ECU_12C,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        41u,
        0u,
        COM_UINT8,
        0u,
        19u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        8u,
        8u,
        NULL_PTR,
        Signal_12B_IPDU_COM_ECU_12B,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        42u,
        0u,
        COM_UINT8,
        0u,
        20u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_121_IPDU_COM_ECU_121_DLC64,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        43u,
        0u,
        COM_UINT8,
        0u,
        21u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_11A_IPDU_COM_ECU_11A,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        54u,
        0u,
        COM_UINT8,
        0u,
        22u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        &ComFilter[0],
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_11B_IPDU_COM_ECU_11B,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        55u,
        0u,
        COM_UINT8,
        0u,
        23u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        &ComFilter[1],
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_11D_IPDU_COM_ECU_11D,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        56u,
        0u,
        COM_UINT8,
        0u,
        24u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_11E_IPDU_COM_ECU_11E,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        57u,
        0u,
        COM_UINT8,
        0u,
        25u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        8u,
        NULL_PTR,
        Signal_125_IPDU_COM_ECU_125,
        FALSE,
        NULL_PTR,
        1u,
        COM_LITTLE_ENDIAN,
        60u,
        0u,
        COM_UINT8,
        0u,
        26u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        1u,
        NULL_PTR,
        Signal_122_Bit0_Bool_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        0u,
        0u,
        COM_BOOLEAN,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        32u,
        32u,
        NULL_PTR,
        Signal_122_Bit32_Float32_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        0u,
        0u,
        COM_FLOAT32,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        96u,
        64u,
        NULL_PTR,
        Signal_122_Bit64_Float64_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        2u,
        0u,
        COM_FLOAT64,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        112u,
        16u,
        NULL_PTR,
        Signal_122_Bit16_Sint16_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        1u,
        0u,
        COM_SINT16,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        144u,
        32u,
        NULL_PTR,
        Signal_122_Bit32_Sint32_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        1u,
        0u,
        COM_SINT32,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        208u,
        64u,
        NULL_PTR,
        Signal_122_Bit64_Sint64_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        3u,
        0u,
        COM_SINT64,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        216u,
        8u,
        NULL_PTR,
        Signal_122_Bit8_Sint8_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        61u,
        0u,
        COM_SINT8,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        232u,
        16u,
        NULL_PTR,
        Signal_122_Bit16_Uint16_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        2u,
        0u,
        COM_UINT16,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        264u,
        32u,
        NULL_PTR,
        Signal_122_Bit16_Uint32_IPDU_COM_ECU_122,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        2u,
        0u,
        COM_UINT32,
        0u,
        27u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        64u,
        NULL_PTR,
        Signal_123_Bit64_Uint64_IPDU_COM_ECU_123,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        4u,
        0u,
        COM_UINT64,
        0u,
        28u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        448u,
        0u,
        NULL_PTR,
        Signal_123_Bit8_Uint8_DYN_IPDU_COM_ECU_123,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_OPAQUE,
        62u,
        8u,
        COM_UINT8_DYN,
        0u,
        28u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        8u
    },
    {
        128u,
        0u,
        NULL_PTR,
        Signal_123_Bit8_Uint8_N_IPDU_COM_ECU_123,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_OPAQUE,
        70u,
        8u,
        COM_UINT8_N,
        0u,
        28u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        8u
    },
    {
        8u,
        16u,
        NULL_PTR,
        BCM2_vVehSpd_GW_CCVS_18FEF121,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        3u,
        0u,
        COM_UINT16,
        0u,
        0u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        34u,
        2u,
        NULL_PTR,
        BCM1_stRunBrkLSgn_GW_CCVS_18FEF121,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        78u,
        0u,
        COM_UINT8,
        0u,
        0u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        80u,
        16u,
        NULL_PTR,
        BCM2_vVehSpd_GW_CCVS_FD_18FEF221,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        4u,
        0u,
        COM_UINT16,
        0u,
        3u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        100u,
        2u,
        NULL_PTR,
        BCM1_stRunBrkLSgn_GW_CCVS_FD_18FEF221,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_LITTLE_ENDIAN,
        79u,
        0u,
        COM_UINT8,
        0u,
        3u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        0u
    },
    {
        0u,
        0u,
        NULL_PTR,
        Signal_201_IPDU_COM_ECU_201,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_OPAQUE,
        81u,
        8u,
        COM_UINT8_N,
        0u,
        30u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        8u
    },
    {
        0u,
        0u,
        NULL_PTR,
        Signal_211_IPDU_COM_ECU_211,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_OPAQUE,
        89u,
        8u,
        COM_UINT8_N,
        0u,
        31u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        8u
    },
    {
        0u,
        0u,
        NULL_PTR,
        Signal_221_IPDU_COM_ECU_221,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_OPAQUE,
        97u,
        8u,
        COM_UINT8_N,
        0u,
        32u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        8u
    },
    {
        0u,
        0u,
        NULL_PTR,
        Signal_231_IPDU_COM_ECU_231,
        FALSE,
        NULL_PTR,
        COM_UNUSED_UINT16,
        COM_OPAQUE,
        105u,
        8u,
        COM_UINT8_N,
        0u,
        33u,
        NULL_PTR,
        COM_PENDING,
        COM_UNUSED_UINT32,
        NULL_PTR,
        0u,
        8u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComRxGroupSignalId_0[2]={0u,1u};
CONST(Com_SignalIdType, COM_CONST) ComRxGroupSignalId_1[2]={2u,3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_RxSignalGroupType, COM_CONST) Com_RxSignalGroup[COM_RXSIGNALGROUP_NUMBER]=
{
    {
        COM_INVALID_ACTION_NOTIFY,
        0u,
        0u,
        ECU_137_Signal_Group01,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        NULL_PTR,
        NULL_PTR,
        COM_UNUSED_UINT32,
        2u,
        &ComRxGroupSignalId_0[0],
        17u,
        27u,
        COM_UNUSED_UINT16,
    },
    {
        COM_INVALID_ACTION_NOTIFY,
        0u,
        0u,
        ECU_137_Signal_Group02,
        NULL_PTR,
        NULL_PTR,
        COM_TIMEOUT_ACTION_NONE,
        NULL_PTR,
        NULL_PTR,
        COM_UNUSED_UINT32,
        2u,
        &ComRxGroupSignalId_1[0],
        17u,
        28u,
        COM_UNUSED_UINT16,
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"
CONST(Com_SignalIdType, COM_CONST) ComTxGroupSignalId_0[2]={0u,1u};
CONST(Com_SignalIdType, COM_CONST) ComTxGroupSignalId_1[2]={2u,3u};
#define COM_STOP_SEC_PBCONFIG_DATA_16
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxSignalGroupType, COM_CONST) Com_TxSignalGroup[COM_TXSIGNALGROUP_NUMBER]=
{
    {
        NULL_PTR,
        ECU_136_Signal_Group01,
        FALSE,
        NULL_PTR,
        NULL_PTR,
        0u,
        NULL_PTR,
        COM_PENDING,
        0u,
        2u,
        &ComTxGroupSignalId_0[0],
        29u
     },
    {
        NULL_PTR,
        ECU_136_Signal_Group02,
        FALSE,
        NULL_PTR,
        NULL_PTR,
        0u,
        NULL_PTR,
        COM_PENDING,
        24u,
        2u,
        &ComTxGroupSignalId_1[0],
        29u
     },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_RxGroupSignalType, COM_CONST) Com_RxGroupSignal[COM_RXGROUPSIGNAL_NUMBER]=
{
    {
        8u,
        8u,
        ECU_137_Signal_Group01_Signal_137_00,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        0u,
        0u,
        COM_SINT8,
        NULL_PTR,
        ECU_137_Signal_Group01,
        0u,
        0u,
        COM_UNUSED_UINT16,
    },
    {
        16u,
        8u,
        ECU_137_Signal_Group01_Signal_137_01,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        1u,
        0u,
        COM_SINT8,
        NULL_PTR,
        ECU_137_Signal_Group01,
        0u,
        0u,
        COM_UNUSED_UINT16,
    },
    {
        32u,
        8u,
        ECU_137_Signal_Group02_Signal_137_10,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        2u,
        0u,
        COM_UINT8,
        NULL_PTR,
        ECU_137_Signal_Group02,
        0u,
        0u,
        COM_UNUSED_UINT16,
    },
    {
        40u,
        8u,
        ECU_137_Signal_Group02_Signal_137_11,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        3u,
        0u,
        COM_UINT8,
        NULL_PTR,
        ECU_137_Signal_Group02,
        0u,
        0u,
        COM_UNUSED_UINT16,
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_TxGroupSignalType, COM_CONST) Com_TxGroupSignal[COM_TXGROUPSIGNAL_NUMBER]=
{
    {
        8u,
        8u,
        ECU_136_Signal_Group01_Signal_136_00,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        113u,
        0u,
        COM_SINT8,
        NULL_PTR,
        FALSE,
        COM_PENDING,
        ECU_136_Signal_Group01,
        0u,
        0u
    },
    {
        16u,
        8u,
        ECU_136_Signal_Group01_Signal_136_01,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        114u,
        0u,
        COM_SINT8,
        NULL_PTR,
        FALSE,
        COM_PENDING,
        ECU_136_Signal_Group01,
        0u,
        0u
    },
    {
        32u,
        8u,
        ECU_136_Signal_Group02_Signal_136_10,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        115u,
        0u,
        COM_UINT8,
        NULL_PTR,
        FALSE,
        COM_PENDING,
        ECU_136_Signal_Group02,
        0u,
        0u
    },
    {
        40u,
        8u,
        ECU_136_Signal_Group02_Signal_136_11,
        COM_UNUSED_UINT16,
        COM_BIG_ENDIAN,
        116u,
        0u,
        COM_UINT8,
        NULL_PTR,
        FALSE,
        COM_PENDING,
        ECU_136_Signal_Group02,
        0u,
        0u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_GwDestType, COM_CONST) Com_GwDest_0[1]=
{
    {
        COM_SIGNAL,
        21u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_GwDestType, COM_CONST) Com_GwDest_1[2]=
{
    {
        COM_SIGNAL,
        5u
    },
    {
        COM_SIGNAL,
        22u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_GwDestType, COM_CONST) Com_GwDest_2[2]=
{
    {
        COM_SIGNAL,
        0u
    },
    {
        COM_SIGNAL,
        3u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_GwDestType, COM_CONST) Com_GwDest_3[2]=
{
    {
        COM_SIGNAL,
        47u
    },
    {
        COM_SIGNAL,
        49u
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_GwMappingType, COM_CONST) Com_GwMapping[COM_GWMAPPING_NUMBER]=
{
    {
        COM_SIGNAL,
        4u,
        1u,
        &Com_GwDest_0[0]
    },
    {
        COM_SIGNAL,
        20u,
        2u,
        &Com_GwDest_1[0]
    },
    {
        COM_SIGNAL,
        25u,
        2u,
        &Com_GwDest_2[0]
    },
    {
        COM_SIGNAL,
        26u,
        2u,
        &Com_GwDest_3[0]
    },
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_ConfigType, COM_CONST_PBCFG) Com_PBConfigData =
{
    &Com_RxIPdu[0],
    &Com_TxIPdu[0],
    &Com_RxSignal[0],
    &Com_TxSignal[0],
    &Com_RxSignalGroup[0],
    &Com_TxSignalGroup[0],
    &Com_RxGroupSignal[0],
    &Com_TxGroupSignal[0],
    &Com_GwMapping[0],
    NULL_PTR,
    NULL_PTR
};
#define COM_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
