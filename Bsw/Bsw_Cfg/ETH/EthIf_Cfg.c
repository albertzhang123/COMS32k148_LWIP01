/*******************************************************************************                                        
**                                                                            **                                        
** Copyright (C) iSOFT   (2016)                                               **                                        
**                                                                            **                                        
** All rights reserved.                                                       **                                        
**                                                                            **                                        
** This document contains proprietary information belonging to iSOFT.         **                                        
** Passing on and copying of this document, and communication                 **                                        
** of its contents is not permitted without prior written authorization.      **                                        
**                                                                            **                                        
********************************************************************************                                        
**                                                                            **                                        
**  FILENAME    : EthIf_Cfg.c                                                 **                                        
**                                                                            **                                        
**  Created on  : 2023/05/26 17:57:39                                         **                                        
**  Author      :                                                             **                                        
**  Vendor      :                                                             **                                        
**  DESCRIPTION : EthIf pre compile globle macro define                       **                                        
**                                                                            **                                        
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **                                        
**                                                                            **                                        
*******************************************************************************/            

/*================================================================================================*/
/*      I N C L U D E S                                                                             */
/*================================================================================================*/
#include "EthIf_Types.h"
#include "EthIf_Cfg.h"

#include "TcpIp.h"
#include "Eth.h"

static const EthIf_ControllerCfgType EthIf_ControllerCfgData[ETHIF_CTRL_NUM]=
{
    {
        0,                  /* EthIfCtrlIdx */
        1500,            /* EthIfCtrlMtu; 64..9000*/
        202,                /*EthIfMaxTxBufsTotal;*/
        0xff,                /*EthIfVlanId;*/
        0,                /*EthIfPhysControllerId; reference*/
        0,                /*EthIfEthTrcvId; reference*/
        0                /*EthIfSwitchRefOrPortGroup; reference*/
    },
};


static const EthIf_FrameOwnerCfgType EthIf_FrameOwnerCfgData[2]=
{
    {
       0x800,                /*EthIfFrameType;*/
       0                    /*EthIfOwner;*/
    },
    {
       0x806,                /*EthIfFrameType;*/
       0                    /*EthIfOwner;*/
    },
};

static const EthIfEthCtrlRefType EthIfEthCtrlRef[1] =
{
    {
        0, /*to identify the different Eth channel*/
        0  /*to identify the different driver API*/
    }
};

static const EthIf_PhysControllerType EthIf_PhysControllerCfgData[1]=
{
    {
       0,                /*EthIfPhysControllerId;*/
       &EthIfEthCtrlRef[0]                /*EthIfEthCtrlId;*/
    }
};



static const EthIfULTxConfirmationType EthIfULTxConfirmation[1] =
{
    NULL_PTR
};

static const EthIfULRxIndicationType EthIfULRxIndication[1] = 
{
    TcpIp_RxIndication
};



const EthIf_EthDriverApiType Eth_DriverApi[1] =
{
    {
        Eth_SetControllerMode,
        Eth_GetControllerMode,
        Eth_ProvideTxBuffer,
        Eth_Transmit,
        Eth_SetPhysAddr,
        Eth_GetPhysAddr,
        Eth_UpdatePhysAddrFilter,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Eth_Receive,
        Eth_TxConfirmation
    }
};

EthIf_ConfigType  EthIf_ConfigData =
{
    &EthIf_ControllerCfgData[0],          /*pointer to hold controller config data*/
    &EthIf_FrameOwnerCfgData[0],        /* pointer to hold Owner config data */
    NULL_PTR,              /* pointer to hold trcv config data */
    &EthIf_PhysControllerCfgData[0],    /* pointer to hold Eth controller config data*/
    NULL_PTR,                /* pointer to hold switches config data */
    &EthIfULTxConfirmation[0],      /* pointer to hold Tx confirmation functions list */
    &EthIfULRxIndication[0],        /* Ptr to Rx indication function list */
    NULL_PTR,    /*pointer to hold Trcv Link state functions list */
    1,                                    /* No of Controllers configure */
    0,                                        /* No of Trcv configured */
    0,                                    /* No of Switches configured */
    2,                                    /* No fo Owners configured */
    1                                   /* No fo PhysController configured */
};

