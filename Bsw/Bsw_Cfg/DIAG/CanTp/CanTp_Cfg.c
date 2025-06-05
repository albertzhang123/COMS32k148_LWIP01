/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanTp_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:40>
 */
/*============================================================================*/


/*=======[I N C L U D E S]================================*/
#include "CanTp_Cfg.h"
#include "CanTp_Types.h"

/*=======[I N T E R N A L   D A T A]=======================*/

/*CanTpChannel_UDS RxSdu address information*/

#define CANTP_START_SEC_CONST_32
#include "Cantp_MemMap.h"
static CONST(uint32, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_Nar = 70u;
#define CANTP_STOP_SEC_CONST_32
#include "Cantp_MemMap.h"

#define CANTP_START_SEC_CONST_32
#include "Cantp_MemMap.h"
static CONST(uint32, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_Ncr = 150u;
#define CANTP_STOP_SEC_CONST_32
#include "Cantp_MemMap.h"

#define CANTP_START_SEC_CONST_8
#include "CanTp_MemMap.h"
static CONST(uint8, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_Bs = 0u;
#define CANTP_STOP_SEC_CONST_8
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CONST_8
#include "CanTp_MemMap.h"
static CONST(uint8, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_STmin = 10u;
#define CANTP_STOP_SEC_CONST_8
#include "CanTp_MemMap.h"


#define CANTP_START_SEC_CONST_32
#include "Cantp_MemMap.h"
static CONST(uint32, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_Nar = 70u;
#define CANTP_STOP_SEC_CONST_32
#include "Cantp_MemMap.h"

#define CANTP_START_SEC_CONST_32
#include "Cantp_MemMap.h"
static CONST(uint32, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_Ncr = 150u;
#define CANTP_STOP_SEC_CONST_32
#include "Cantp_MemMap.h"

#define CANTP_START_SEC_CONST_8
#include "CanTp_MemMap.h"
static CONST(uint8, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_Bs = 0u;
#define CANTP_STOP_SEC_CONST_8
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CONST_8
#include "CanTp_MemMap.h"
static CONST(uint8, CANTP_CONST) CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_STmin = 10u;
#define CANTP_STOP_SEC_CONST_8
#include "CanTp_MemMap.h"


#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"
static CONST(CanTp_RxNSduType, CANTP_CONST) CanTp_Ch0RxNSdus[CANTP_CH0_RXNSDU_NUMBER] =
{
	{
		&CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_Bs,
	    &CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_Nar,
	    70u,
	    &CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_Ncr,
	    CANTP_STANDARD,
	    CANTP_PDUR_CANTP_UDS_RX_PHY,
	    CANTP_CANTP_UDS_RX_PHY,
		CANTP_CANIF_UDS_RX_PHY,
		CANTP_CANIF_UDS_TX,				
		CANTP_CANIF_CANIF_UDS_TX,	
		0u,
		0u,
	    CANTP_PADDING_ON,
	    CANTP_PHYSICAL_RX,
	    NULL_PTR,
	    &CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_PHY_STmin,
		NULL_PTR,
		NULL_PTR,
		NULL_PTR,
		CAN20,
	},
	{
		&CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_Bs,
	    &CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_Nar,
	    70u,
	    &CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_Ncr,
	    CANTP_STANDARD,
	    CANTP_PDUR_CANTP_UDS_RX_FUN,
	    CANTP_CANTP_UDS_RX_FUN,
		CANTP_CANIF_UDS_RX_FUN,
		0xFFu,
		0xFFu,	
		0u,
		0u,
	    CANTP_PADDING_ON,
	    CANTP_FUNCTIONAL_RX,
	    NULL_PTR,
	    &CanTp_Ch0RxSdu_CANTP_CANTP_UDS_RX_FUN_STmin,
		NULL_PTR,
		NULL_PTR,
		NULL_PTR,
		CAN20,
	},
};
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"

/*CanTpChannel_UDS TxSdu address information*/
#define CANTP_START_SEC_CONST_32
#include "CanTp_MemMap.h"
static CONST(uint32, CANTP_CONST) CanTp_Ch0TxSdu_CANTP_CANTP_UDS_TX_Nbs = 150u;
#define CANTP_STOP_SEC_CONST_32
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CONST_32
#include "CanTp_MemMap.h"
static CONST(uint32, CANTP_CONST) CanTp_Ch0TxSdu_CANTP_CANTP_UDS_TX_Ncs = 100u;
#define CANTP_STOP_SEC_CONST_32
#include "CanTp_MemMap.h"



#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"
static CONST(CanTp_TxNSduType, CANTP_CONST) CanTp_Ch0TxNSdus[CANTP_CH0_TXNSDU_NUMBER] =
{	
	{
		70u,
		&CanTp_Ch0TxSdu_CANTP_CANTP_UDS_TX_Nbs,
		&CanTp_Ch0TxSdu_CANTP_CANTP_UDS_TX_Ncs,
		CANTP_STANDARD,
		CANTP_PDUR_CANTP_UDS_TX,	
		CANTP_CANTP_UDS_TX,
		CANTP_CANIF_UDS_TX,	
		CANTP_CANIF_UDS_RX_PHY,
		CANTP_CANIF_CANIF_UDS_TX,
		0u,
		8u,	
		CANTP_PADDING_ON,
		CANTP_PHYSICAL_TX,
		NULL_PTR,
		NULL_PTR,
		NULL_PTR,
		CAN20,
    },
};
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"
static CONST(CanTp_ChannelType, CANTP_CONST) CanTp_CfgChannel[CANTP_CHANNEL_NUMBER] =
{	
	{
		CANTP_MODE_FULL_DUPLEX,
		CANTP_CH0_RXNSDU_NUMBER,
		&CanTp_Ch0RxNSdus[0],
		CANTP_CH0_TXNSDU_NUMBER,
		&CanTp_Ch0TxNSdus[0]
	},
};
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"
CONST(CanTp_ConfigType, CANTP_CONST) CanTp_CfgData =
{
	CANTP_MAIN_FUNCTION_PERIOD,
	0u,
	&CanTp_CfgChannel[0]
};
#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include "CanTp_MemMap.h"

/*=======[E X T E R N A L   D A T A]===========================*/
/*=======[E N D   O F   F I L E]==============================*/

