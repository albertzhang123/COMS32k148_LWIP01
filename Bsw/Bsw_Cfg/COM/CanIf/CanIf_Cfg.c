
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
**  FILENAME    : CanIf_Cfg.c                                                 **
**                                                                            **
**  Created on  : 2022/06/02 13:43:03                                         **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :parameter declaration of CanIf                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/


/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
#define CANIF_CFG_C_AR_MAJOR_VERSION  4u
#define CANIF_CFG_C_AR_MINOR_VERSION  2u
#define CANIF_CFG_C_AR_PATCH_VERSION  2u
#define CANIF_CFG_C_SW_MAJOR_VERSION  2u
#define CANIF_CFG_C_SW_MINOR_VERSION  0u
#define CANIF_CFG_C_SW_PATCH_VERSION  0u

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CanIf.h"
#include "CanSM_Cbk.h"
#include "PduR_CanIf.h"
#include "CanTp_Cbk.h"
//#include "XcpOnCan_Cbk.h"
//#include "CanNm_Cbk.h"
#include "Can.h"
/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
#if(CANIF_CFG_C_AR_MAJOR_VERSION != CANIF_CFG_H_AR_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if(CANIF_CFG_C_AR_MINOR_VERSION != CANIF_CFG_H_AR_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
#endif

#if(CANIF_CFG_C_AR_PATCH_VERSION != CANIF_CFG_H_AR_PATCH_VERSION)
    #error "CanIf.c:Mismatch in Specification Patch Version"
#endif

#if(CANIF_CFG_C_SW_MAJOR_VERSION != CANIF_CFG_H_SW_MAJOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Major Version"
#endif

#if(CANIF_CFG_C_SW_MINOR_VERSION != CANIF_CFG_H_SW_MINOR_VERSION)
    #error "CanIf.c:Mismatch in Specification Minor Version"
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
#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_DispatchConfigType,CANIF_CONST) CanIf_DispatchConfigData =
{
    /* void XXX_ControllerBusOff(uint8 ControllerId) */
    &CanSM_ControllerBusOff,
    /* void XXX_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode)  */
    &CanSM_ControllerModeIndication,
    /* void XXX_TransceiverModeIndication(uint8 TransceiverId,CanTrcv_TrcvModeType TransceiverMode) */
    NULL_PTR,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(Can_DriverApiType,CANIF_CONST_PBCFG) Can_DriverApi[CANIF_CANDRIVER_NUMBER] =
{
    {
        &Can_SetControllerMode,
        &Can_Write,
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfCtrlCanCtrlRefType,CANIF_CONST_PBCFG) CanIf_CtrlRef[CANIF_CANCONTROLLER_NUMBER] =
{
    {
        0u,
    },
    {
        1u,
    },
    {
        2u,
    },
    {
        3u,
    },
    {
        4u,
    },
    {
        5u,
    },
    {
        6u,
    },
    {
        7u,
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_ControllerCfgType,CANIF_CONST) CanIf_CtrlCfgData[CANIF_CANCONTROLLER_NUMBER] =
{
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[0],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[1],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[2],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[3],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_4,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[4],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_5,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[5],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_6,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[6],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_7,
        CANIF_CAN,
        FALSE,
        &CanIf_CtrlRef[7],
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfHrhIdSymRefType,CANIF_CONST_PBCFG) CanIf_HrhRef[CANIF_HRH_NUMBER] =
{
    {
        59u,
        CANIF_FULL_CAN,
    },
    {
        60u,
        CANIF_FULL_CAN,
    },
    {
        2u,
        CANIF_FULL_CAN,
    },
    {
        24u,
        CANIF_FULL_CAN,
    },
    {
        3u,
        CANIF_FULL_CAN,
    },
    {
        61u,
        CANIF_FULL_CAN,
    },
    {
        62u,
        CANIF_FULL_CAN,
    },
    {
        4u,
        CANIF_FULL_CAN,
    },
    {
        5u,
        CANIF_FULL_CAN,
    },
    {
        44u,
        CANIF_FULL_CAN,
    },
    {
        45u,
        CANIF_FULL_CAN,
    },
    {
        25u,
        CANIF_FULL_CAN,
    },
    {
        6u,
        CANIF_FULL_CAN,
    },
    {
        26u,
        CANIF_FULL_CAN,
    },
    {
        27u,
        CANIF_FULL_CAN,
    },
    {
        7u,
        CANIF_FULL_CAN,
    },
    {
        8u,
        CANIF_FULL_CAN,
    },
    {
        28u,
        CANIF_FULL_CAN,
    },
    {
        46u,
        CANIF_FULL_CAN,
    },
    {
        63u,
        CANIF_FULL_CAN,
    },
    {
        0u,
        CANIF_FULL_CAN,
    },
    {
        1u,
        CANIF_FULL_CAN,
    },
    {
        64u,
        CANIF_FULL_CAN,
    },
    {
        9u,
        CANIF_BASIC_CAN,
    },
    {
        29u,
        CANIF_BASIC_CAN,
    },
    {
        47u,
        CANIF_BASIC_CAN,
    },
    {
        65u,
        CANIF_BASIC_CAN,
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef0[1] =
{
    CANIF_RXPDU_Tester_178,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef1[1] =
{
    CANIF_RXPDU_Tester_177,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef2[1] =
{
    CANIF_RXPDU_Tester_101,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef3[1] =
{
    CANIF_RXPDU_Tester_117,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef4[1] =
{
    CANIF_RXPDU_Tester_10A,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef5[1] =
{
    CANIF_RXPDU_Tester_18FF0141,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef6[1] =
{
    CANIF_RXPDU_Tester_130,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef7[1] =
{
    CANIF_RXPDU_Tester_10E,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef8[1] =
{
    CANIF_RXPDU_Tester_10C,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef9[1] =
{
    CANIF_RXPDU_Tester_129_DLC64,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef10[1] =
{
    CANIF_RXPDU_Tester_124_DLC16,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef11[1] =
{
    CANIF_RXPDU_Tester_119,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef12[1] =
{
    CANIF_RXPDU_Tester_109,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef13[1] =
{
    CANIF_RXPDU_Tester_118,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef14[1] =
{
    CANIF_RXPDU_Tester_116,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef15[1] =
{
    CANIF_RXPDU_Tester_108,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef16[1] =
{
    CANIF_RXPDU_Tester_106,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef17[1] =
{
    CANIF_RXPDU_Tester_11F,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef18[1] =
{
    CANIF_RXPDU_Tester_18FF1131,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef19[1] =
{
    CANIF_RXPDU_Tester_137,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef20[1] =
{
    CANIF_RXPDU_CANIF_UDS_RX_PHY,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef21[1] =
{
    CANIF_RXPDU_CANIF_UDS_RX_FUN,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef22[1] =
{
    CANIF_RXPDU_Xcp_Rx_0x600,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef23[1] =
{
    CANIF_RXPDU_CanNm_Rx_0x400_ch0,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef24[1] =
{
    CANIF_RXPDU_CanNm_Rx_0x400_ch1,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef25[1] =
{
    CANIF_RXPDU_CanNm_Rx_0x400_ch2,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(PduIdType,CANIF_CONST) CanIf_HrhRxPduRef26[1] =
{
    CANIF_RXPDU_CanNm_Rx_0x400_ch3,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"



#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfHrhCfgType,CANIF_CONST) CanIf_HrhCfgData[CANIF_HRH_NUMBER] =
{
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef0[0],
        NULL_PTR,
        &CanIf_HrhRef[0],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef1[0],
        NULL_PTR,
        &CanIf_HrhRef[1],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef2[0],
        NULL_PTR,
        &CanIf_HrhRef[2],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        0u,
        1u,
        &CanIf_HrhRxPduRef3[0],
        NULL_PTR,
        &CanIf_HrhRef[3],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef4[0],
        NULL_PTR,
        &CanIf_HrhRef[4],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef5[0],
        NULL_PTR,
        &CanIf_HrhRef[5],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef6[0],
        NULL_PTR,
        &CanIf_HrhRef[6],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef7[0],
        NULL_PTR,
        &CanIf_HrhRef[7],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef8[0],
        NULL_PTR,
        &CanIf_HrhRef[8],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        0u,
        1u,
        &CanIf_HrhRxPduRef9[0],
        NULL_PTR,
        &CanIf_HrhRef[9],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        0u,
        1u,
        &CanIf_HrhRxPduRef10[0],
        NULL_PTR,
        &CanIf_HrhRef[10],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        0u,
        1u,
        &CanIf_HrhRxPduRef11[0],
        NULL_PTR,
        &CanIf_HrhRef[11],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef12[0],
        NULL_PTR,
        &CanIf_HrhRef[12],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        0u,
        1u,
        &CanIf_HrhRxPduRef13[0],
        NULL_PTR,
        &CanIf_HrhRef[13],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        0u,
        1u,
        &CanIf_HrhRxPduRef14[0],
        NULL_PTR,
        &CanIf_HrhRef[14],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef15[0],
        NULL_PTR,
        &CanIf_HrhRef[15],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef16[0],
        NULL_PTR,
        &CanIf_HrhRef[16],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        0u,
        1u,
        &CanIf_HrhRxPduRef17[0],
        NULL_PTR,
        &CanIf_HrhRef[17],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        0u,
        1u,
        &CanIf_HrhRxPduRef18[0],
        NULL_PTR,
        &CanIf_HrhRef[18],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef19[0],
        NULL_PTR,
        &CanIf_HrhRef[19],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef20[0],
        NULL_PTR,
        &CanIf_HrhRef[20],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef21[0],
        NULL_PTR,
        &CanIf_HrhRef[21],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef22[0],
        NULL_PTR,
        &CanIf_HrhRef[22],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        0u,
        1u,
        &CanIf_HrhRxPduRef23[0],
        NULL_PTR,
        &CanIf_HrhRef[23],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        0u,
        1u,
        &CanIf_HrhRxPduRef24[0],
        NULL_PTR,
        &CanIf_HrhRef[24],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        0u,
        1u,
        &CanIf_HrhRxPduRef25[0],
        NULL_PTR,
        &CanIf_HrhRef[25],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        0u,
        1u,
        &CanIf_HrhRxPduRef26[0],
        NULL_PTR,
        &CanIf_HrhRef[26],
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfHthIdSymRefType,CANIF_CONST_PBCFG) CanIf_HthRef[CANIF_HTH_NUMBER] =
{
    {
        67u,
        CANIF_FULL_CAN,
    },
    {
        31u,
        CANIF_FULL_CAN,
    },
    {
        32u,
        CANIF_FULL_CAN,
    },
    {
        33u,
        CANIF_FULL_CAN,
    },
    {
        13u,
        CANIF_FULL_CAN,
    },
    {
        14u,
        CANIF_FULL_CAN,
    },
    {
        15u,
        CANIF_FULL_CAN,
    },
    {
        68u,
        CANIF_FULL_CAN,
    },
    {
        16u,
        CANIF_FULL_CAN,
    },
    {
        69u,
        CANIF_FULL_CAN,
    },
    {
        70u,
        CANIF_FULL_CAN,
    },
    {
        71u,
        CANIF_FULL_CAN,
    },
    {
        17u,
        CANIF_FULL_CAN,
    },
    {
        18u,
        CANIF_FULL_CAN,
    },
    {
        49u,
        CANIF_FULL_CAN,
    },
    {
        50u,
        CANIF_FULL_CAN,
    },
    {
        51u,
        CANIF_FULL_CAN,
    },
    {
        52u,
        CANIF_FULL_CAN,
    },
    {
        53u,
        CANIF_FULL_CAN,
    },
    {
        34u,
        CANIF_FULL_CAN,
    },
    {
        35u,
        CANIF_FULL_CAN,
    },
    {
        19u,
        CANIF_FULL_CAN,
    },
    {
        36u,
        CANIF_FULL_CAN,
    },
    {
        20u,
        CANIF_FULL_CAN,
    },
    {
        37u,
        CANIF_FULL_CAN,
    },
    {
        21u,
        CANIF_FULL_CAN,
    },
    {
        38u,
        CANIF_FULL_CAN,
    },
    {
        39u,
        CANIF_FULL_CAN,
    },
    {
        40u,
        CANIF_FULL_CAN,
    },
    {
        41u,
        CANIF_FULL_CAN,
    },
    {
        54u,
        CANIF_FULL_CAN,
    },
    {
        55u,
        CANIF_FULL_CAN,
    },
    {
        56u,
        CANIF_FULL_CAN,
    },
    {
        72u,
        CANIF_FULL_CAN,
    },
    {
        73u,
        CANIF_FULL_CAN,
    },
    {
        11u,
        CANIF_FULL_CAN,
    },
    {
        12u,
        CANIF_FULL_CAN,
    },
    {
        74u,
        CANIF_FULL_CAN,
    },
    {
        22u,
        CANIF_FULL_CAN,
    },
    {
        42u,
        CANIF_FULL_CAN,
    },
    {
        57u,
        CANIF_FULL_CAN,
    },
    {
        75u,
        CANIF_FULL_CAN,
    },
    {
        78u,
        CANIF_FULL_CAN,
    },
    {
        80u,
        CANIF_FULL_CAN,
    },
    {
        82u,
        CANIF_FULL_CAN,
    },
    {
        84u,
        CANIF_FULL_CAN,
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfHthCfgType,CANIF_CONST) CanIf_HthCfgData[CANIF_HTH_NUMBER] =
{
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[0],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[1],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[2],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[3],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[4],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[5],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[6],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[7],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[8],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[9],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[10],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[11],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[12],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[13],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[14],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[15],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[16],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[17],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[18],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[19],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[20],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[21],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[22],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[23],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[24],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[25],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[26],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[27],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[28],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[29],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[30],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[31],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[32],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[33],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[34],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[35],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[36],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[37],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,
        &CanIf_HthRef[38],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1,
        &CanIf_HthRef[39],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2,
        &CanIf_HthRef[40],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3,
        &CanIf_HthRef[41],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_4,
        &CanIf_HthRef[42],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_5,
        &CanIf_HthRef[43],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_6,
        &CanIf_HthRef[44],
    },
    {
        CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_7,
        &CanIf_HthRef[45],
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_InitHohCfgType,CANIF_CONST) CanIf_InitHohCfgData =
{
    CanIf_HrhCfgData,
    CanIf_HthCfgData,
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfRxPduRefType,CANIF_CONST_PBCFG) CanIf_RxPduRef[CANIF_RXPDU_NUMBER] =
{
    {
        PDUR_SRCPDU_Tester_178,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_177,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_101,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_117,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_10A,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_18FF0141,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_130,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_10E,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_10C,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_129_DLC64,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_124_DLC16,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_119,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_109,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_118,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_116,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_108,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_106,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_11F,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_18FF1131,
        FALSE
    },
    {
        PDUR_SRCPDU_Tester_137,
        FALSE
    },
    {
        CANTP_CANIF_UDS_RX_PHY,
        FALSE
    },
    {
        CANTP_CANIF_UDS_RX_FUN,
        FALSE
    },
    {
    	NULL_PTR,
        FALSE
    },
    {
    	NULL_PTR,
        FALSE
    },
    {
    	NULL_PTR,
        FALSE
    },
    {
    	NULL_PTR,
        FALSE
    },
    {
    	NULL_PTR,
        FALSE
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"




#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_RxPduConfigType,CANIF_CONST) CanIf_RxPduConfigData[CANIF_RXPDU_NUMBER] =
{
    {
        CANIF_RXPDU_Tester_178,
        0x178u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_0,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[0],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_177,
        0x177u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_1,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[1],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_101,
        0x101u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        16u,
        CANIF_HOH0_HRH_2,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[2],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_117,
        0x117u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_3,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[3],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_10A,
        0x10au,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_4,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[4],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_18FF0141,
        0x18ff0141u,
        0x1fffffffu,
        CANIF_RX_EXTENDED_CAN,
        8u,
        CANIF_HOH0_HRH_5,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[5],
        0x0u,
        0x1fffffffu
    },
    {
        CANIF_RXPDU_Tester_130,
        0x130u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_6,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[6],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_10E,
        0x10eu,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_7,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[7],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_10C,
        0x10cu,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_8,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[8],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_129_DLC64,
        0x129u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        64u,
        CANIF_HOH0_HRH_9,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[9],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_124_DLC16,
        0x124u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        16u,
        CANIF_HOH0_HRH_10,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[10],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_119,
        0x119u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_11,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[11],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_109,
        0x109u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_12,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[12],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_118,
        0x118u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_13,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[13],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_116,
        0x116u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_14,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[14],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_108,
        0x108u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_15,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[15],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_106,
        0x106u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_16,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[16],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_11F,
        0x11fu,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_17,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[17],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Tester_18FF1131,
        0x18ff1131u,
        0x1fffffffu,
        CANIF_RX_EXTENDED_CAN,
        8u,
        CANIF_HOH0_HRH_18,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[18],
        0x0u,
        0x1fffffffu
    },
    {
        CANIF_RXPDU_Tester_137,
        0x137u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_19,
        FALSE,
        &PduR_CanIfRxIndication,
        &CanIf_RxPduRef[19],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_CANIF_UDS_RX_PHY,
        0x703u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_20,
        FALSE,
        &CanTp_RxIndication,
        &CanIf_RxPduRef[20],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_CANIF_UDS_RX_FUN,
        0x7dfu,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_21,
        FALSE,
        &CanTp_RxIndication,
        &CanIf_RxPduRef[21],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_Xcp_Rx_0x600,
        0x600u,
        0x7ffu,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_22,
        FALSE,
        NULL_PTR,
        &CanIf_RxPduRef[22],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_CanNm_Rx_0x400_ch0,
        0x400u,
        0x700u,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_23,
        TRUE,
        NULL_PTR,
        &CanIf_RxPduRef[23],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_CanNm_Rx_0x400_ch1,
        0x400u,
        0x700u,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_24,
        TRUE,
        NULL_PTR,
        &CanIf_RxPduRef[24],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_CanNm_Rx_0x400_ch2,
        0x400u,
        0x700u,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_25,
        TRUE,
		NULL_PTR,
        &CanIf_RxPduRef[25],
        0x0u,
        0x7ffu
    },
    {
        CANIF_RXPDU_CanNm_Rx_0x400_ch3,
        0x400u,
        0x700u,
        CANIF_RX_STANDARD_CAN,
        8u,
        CANIF_HOH0_HRH_26,
        TRUE,
        NULL_PTR,
        &CanIf_RxPduRef[26],
        0x0u,
        0x7ffu
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIfTxPduRefType,CANIF_CONST_PBCFG) CanIf_TxPduRef[CANIF_TXPDU_NUMBER] =
{
    {
        PDUR_DESTPDU_ECU_179,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_18FEF121,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_112,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_111,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_18FEF221,
        FALSE,
        16u
    },
    {
        PDUR_DESTPDU_ECU_107,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_103,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_135,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_102,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_134,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_133,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_131,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_10F,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_10D,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_12F,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_12D,
        FALSE,
        64u
    },
    {
        PDUR_DESTPDU_ECU_12C,
        FALSE,
        12u
    },
    {
        PDUR_DESTPDU_ECU_12B,
        FALSE,
        4u
    },
    {
        PDUR_DESTPDU_ECU_121_DLC64,
        FALSE,
        64u
    },
    {
        PDUR_DESTPDU_ECU_11C_DLC16,
        FALSE,
        16u
    },
    {
        PDUR_DESTPDU_ECU_115,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_105,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_114,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_104,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_110,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_100,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_11A,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_11B,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_11D,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_11E,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_122,
        FALSE,
        64u
    },
    {
        PDUR_DESTPDU_ECU_123,
        FALSE,
        64u
    },
    {
        PDUR_DESTPDU_ECU_125,
        FALSE,
        16u
    },
    {
        PDUR_DESTPDU_ECU_136,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_138,
        FALSE,
        8u
    },
    {
        CANTP_CANIF_UDS_TX,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_CANIF_2A,
        FALSE,
        8u
    },
    {
        NULL_PTR,
        FALSE,
        8u
    },
    {
    	NULL_PTR,
        FALSE,
        8u
    },
    {
        NULL_PTR,
        FALSE,
        8u
    },
    {
        NULL_PTR,
        FALSE,
        8u
    },
    {
        NULL_PTR,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_201,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_211,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_221,
        FALSE,
        8u
    },
    {
        PDUR_DESTPDU_ECU_231,
        FALSE,
        8u
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_TxPduConfigType,CANIF_CONST) CanIf_TxPduConfigData[CANIF_TXPDU_NUMBER] =
{
    {
        CANIF_TXPDU_ECU_179,
        0x179u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_0,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[0],
    },
    {
        CANIF_TXPDU_ECU_18FEF121,
        0x18fef121u,
        0x1fffffffu,
        CANIF_TX_EXTENDED_CAN,
        CANIF_HOH0_HTH_1,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[1],
    },
    {
        CANIF_TXPDU_ECU_112,
        0x112u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_2,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[2],
    },
    {
        CANIF_TXPDU_ECU_111,
        0x111u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_3,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[3],
    },
    {
        CANIF_TXPDU_ECU_18FEF221,
        0x18fef221u,
        0x1fffffffu,
        CANIF_TX_EXTENDED_FD_CAN,
        CANIF_HOH0_HTH_4,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[4],
    },
    {
        CANIF_TXPDU_ECU_107,
        0x107u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_5,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[5],
    },
    {
        CANIF_TXPDU_ECU_103,
        0x103u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_6,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[6],
    },
    {
        CANIF_TXPDU_ECU_135,
        0x135u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_7,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[7],
    },
    {
        CANIF_TXPDU_ECU_102,
        0x102u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_8,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[8],
    },
    {
        CANIF_TXPDU_ECU_134,
        0x134u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_9,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[9],
    },
    {
        CANIF_TXPDU_ECU_133,
        0x133u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_10,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[10],
    },
    {
        CANIF_TXPDU_ECU_131,
        0x131u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_11,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[11],
    },
    {
        CANIF_TXPDU_ECU_10F,
        0x10fu,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_12,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[12],
    },
    {
        CANIF_TXPDU_ECU_10D,
        0x10du,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_13,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[13],
    },
    {
        CANIF_TXPDU_ECU_12F,
        0x12fu,
        0x1fffffffu,
        CANIF_TX_EXTENDED_FD_CAN,
        CANIF_HOH0_HTH_14,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[14],
    },
    {
        CANIF_TXPDU_ECU_12D,
        0x12du,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_15,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[15],
    },
    {
        CANIF_TXPDU_ECU_12C,
        0x12cu,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_16,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[16],
    },
    {
        CANIF_TXPDU_ECU_12B,
        0x12bu,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_17,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[17],
    },
    {
        CANIF_TXPDU_ECU_121_DLC64,
        0x121u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_18,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[18],
    },
    {
        CANIF_TXPDU_ECU_11C_DLC16,
        0x11cu,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_19,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[19],
    },
    {
        CANIF_TXPDU_ECU_115,
        0x115u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_20,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[20],
    },
    {
        CANIF_TXPDU_ECU_105,
        0x105u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_21,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[21],
    },
    {
        CANIF_TXPDU_ECU_114,
        0x114u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_22,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[22],
    },
    {
        CANIF_TXPDU_ECU_104,
        0x104u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_23,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[23],
    },
    {
        CANIF_TXPDU_ECU_110,
        0x110u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_24,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[24],
    },
    {
        CANIF_TXPDU_ECU_100,
        0x100u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_25,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[25],
    },
    {
        CANIF_TXPDU_ECU_11A,
        0x11au,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_26,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[26],
    },
    {
        CANIF_TXPDU_ECU_11B,
        0x11bu,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_27,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[27],
    },
    {
        CANIF_TXPDU_ECU_11D,
        0x11du,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_28,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[28],
    },
    {
        CANIF_TXPDU_ECU_11E,
        0x11eu,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_29,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[29],
    },
    {
        CANIF_TXPDU_ECU_122,
        0x122u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_30,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[30],
    },
    {
        CANIF_TXPDU_ECU_123,
        0x123u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_31,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[31],
    },
    {
        CANIF_TXPDU_ECU_125,
        0x125u,
        0x7ffu,
        CANIF_TX_STANDARD_FD_CAN,
        CANIF_HOH0_HTH_32,
        0u,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[32],
    },
    {
        CANIF_TXPDU_ECU_136,
        0x136u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_33,
        1u,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[33],
    },
    {
        CANIF_TXPDU_ECU_138,
        0x138u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_34,
        2u,
        CANID_DYNAMIC,
        0,
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[34],
    },
    {
        CANIF_TXPDU_CANIF_UDS_TX,
        0x70au,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_35,
        3u,
        CANID_STATIC,
        0xffffu,  
        &CanTp_TxConfirmation,
        &CanIf_TxPduRef[35],
    },
    {
        CANIF_TXPDU_CANIF_2A,
        0x70cu,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_36,
        4u,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[36],
    },
    {
        CANIF_TXPDU_Xcp_Tx_0x601,
        0x601u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_37,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        NULL_PTR,
        &CanIf_TxPduRef[37],
    },
    {
        CANIF_TXPDU_CanNm_Tx_0x410,
        0x410u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_38,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        //&CanNm_TxConfirmation,
        &CanIf_TxPduRef[38],
    },
    {
        CANIF_TXPDU_CanNm_Tx_0x411,
        0x411u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_39,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        NULL_PTR,
        &CanIf_TxPduRef[39],
    },
    {
        CANIF_TXPDU_CanNm_Tx_0x412,
        0x412u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_40,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        NULL_PTR,
        &CanIf_TxPduRef[40],
    },
    {
        CANIF_TXPDU_CanNm_Tx_0x413,
        0x413u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_41,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
		NULL_PTR,
        &CanIf_TxPduRef[41],
    },
    {
        CANIF_TXPDU_ECU_201,
        0x201u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_42,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[42],
    },
    {
        CANIF_TXPDU_ECU_211,
        0x211u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_43,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[43],
    },
    {
        CANIF_TXPDU_ECU_221,
        0x221u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_44,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[44],
    },
    {
        CANIF_TXPDU_ECU_231,
        0x231u,
        0x7ffu,
        CANIF_TX_STANDARD_CAN,
        CANIF_HOH0_HTH_45,
        CANIF_UNUSED_UINT16,
        CANID_STATIC,
        0xffffu,  
        &PduR_CanIfTxConfirmation,
        &CanIf_TxPduRef[45],
    },
};
#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
uint8 TxPduBuffer0[16];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
uint8 TxPduBuffer1[8];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
uint8 TxPduBuffer2[8];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
uint8 TxPduBuffer3[8];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
uint8 TxPduBuffer4[8];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_TxPduBufferType, CANIF_VAR) CanIf_TxPduBuffer0[1] =
{
    {
        FALSE,
        0u,
        0xffffu,
        0xffffffffu,
        TxPduBuffer0
    },
};
#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_TxPduBufferType, CANIF_VAR) CanIf_TxPduBuffer1[1] =
{
    {
        FALSE,
        0u,
        0xffffu,
        0xffffffffu,
        TxPduBuffer1
    },
};
#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_TxPduBufferType, CANIF_VAR) CanIf_TxPduBuffer2[1] =
{
    {
        FALSE,
        0u,
        0xffffu,
        0xffffffffu,
        TxPduBuffer2
    },
};
#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_TxPduBufferType, CANIF_VAR) CanIf_TxPduBuffer3[1] =
{
    {
        FALSE,
        0u,
        0xffffu,
        0xffffffffu,
        TxPduBuffer3
    },
};
#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_TxPduBufferType, CANIF_VAR) CanIf_TxPduBuffer4[1] =
{
    {
        FALSE,
        0u,
        0xffffu,
        0xffffffffu,
        TxPduBuffer4
    },
};
#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_TxBufferType, CANIF_VAR) CanIf_TxBuffer[CANIF_TXBUFFER_NUMBER] =
{
    {CanIf_TxPduBuffer0},
    {CanIf_TxPduBuffer1},
    {CanIf_TxPduBuffer2},
    {CanIf_TxPduBuffer3},
    {CanIf_TxPduBuffer4},
};
#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_8
#include "CanIf_MemMap.h"
CONST(uint8,CANIF_CONST) CanIf_TxBufferSizeCfg[CANIF_TXBUFFER_NUMBER] ={1,1,1,1,1};
#define CANIF_STOP_SEC_CONFIG_DATA_8
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_ConfigType,CANIF_CONST) CanIf_InitCfgSet =
{
    NULL_PTR,
    CanIf_CtrlCfgData,
    &CanIf_InitHohCfgData,
    CanIf_RxPduConfigData,
    CanIf_TxPduConfigData,
    CanIf_TxBufferSizeCfg,
};
#define CANIF_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "CanIf_MemMap.h"

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
