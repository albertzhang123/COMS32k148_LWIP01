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
**  FILENAME    : EthIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Yangbo                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for EthIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION> <DATE> <AUTHOR> <REVISION LOG>
* V2.0.0 [2020/7/20] [YangBo] Initial Vertion.
* V2.0.1 [2020/11/10] [YangBo] QAC fix.
* V2.0.2 [2020/01/08] [YangBo] Unit test fix.
* V2.0.3 [2020/01/28] [YangBo] add surport for Eth and EthTrcv multi api.
* V2.0.4 [2020/03/31] [YangBo] Det method change.
* V2.0.5 [2022/05/17] [zhiqiang.huang] Add null pointer check for eth trcv API.
*/
/*******************************************************************************
**                       Version Information Definitions                       **
*******************************************************************************/
#define ETHIF_C_AR_RELEASE_MAJOR_VERSION        19U
#define ETHIF_C_AR_RELEASE_MINOR_VERSION        11U
#define ETHIF_C_AR_RELEASE_REVISION_VERSION     0U

#define ETHIF_C_SW_MAJOR_VERSION                2U
#define ETHIF_C_SW_MINOR_VERSION                0U
#define ETHIF_C_SW_PATCH_VERSION                5U

/*******************************************************************************
**                               Includes                                     **
*******************************************************************************/
#include "EthIf.h"
#include "EthIf_MemMap.h"
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "EthIf_Cbk.h"
#if (STD_ON == ETHIF_USE_ETHTRCV)
#include "EthTrcv.h"
#endif
/*******************************************************************************
**                             Version Check                                  **
*******************************************************************************/
#if (ETHIF_C_AR_RELEASE_MAJOR_VERSION != ETHIF_H_AR_RELEASE_MAJOR_VERSION)
  #error "EthIf.c : Mismatch in Specification Major Version"
#endif
#if (ETHIF_C_AR_RELEASE_MINOR_VERSION != ETHIF_H_AR_RELEASE_MINOR_VERSION)
  #error "EthIf.c : Mismatch in Specification Minor Version"
#endif
#if (ETHIF_C_AR_RELEASE_REVISION_VERSION != ETHIF_H_AR_RELEASE_REVISION_VERSION)
  #error "EthIf.c : Mismatch in Specification Revision Version"
#endif
#if (ETHIF_C_SW_MAJOR_VERSION != ETHIF_H_SW_MAJOR_VERSION)
  #error "EthIf.c : Mismatch in Software Major Version"
#endif
#if (ETHIF_C_SW_MINOR_VERSION != ETHIF_H_SW_MINOR_VERSION)
  #error "EthIf.c : Mismatch in Software Minor Version"
#endif
#if (ETHIF_C_SW_PATCH_VERSION != ETHIF_H_SW_PATCH_VERSION)
  #error "EthIf.c : Mismatch in Software Patch Version"
#endif

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#define ETHIF_DET_REPORT(ApiId,ErrorId)     (\
        (void)Det_ReportError(\
                ETHIF_MODULE_ID,\
                ETHIF_INSTANCE_ID,\
                (ApiId),\
                (ErrorId)))
#else
#define ETHIF_DET_REPORT(ApiId,ErrorId)
#endif
#define VLAN_TAG_SIZE                      (uint16)4u
#define PRIORITY_MAX                       (uint8)8u
#define ETH_FRAME_TYPE_VLAN                (uint16)0x8100
#define INVALID_BUFFER_INDEX               0xFFFFFFFFu
#define INVALID_UINT8                      0xFFu
#define ETHIF_VID_MASK                     0x0FFFu
#define ETHIF_INVALID_VID                  0x0FFFu

#define ETHIF_CONTROLLER_TRCVID(CtrlIdx)   EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfEthTrcvId
#define ETHIF_TRCV_ETHTRCV(TrcvIdx) EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvRef->EthIfEthTrcvIdx
#define ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx) EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvRef->EthTrcvDriverId
#define ETHIF_TRCV_WAKEUPSOURCE(TrcvIdx) EthIf_ConfigPtr->EthIfTrcvCfg[TrcvIdx].EthIfEthTrcvRef->WakeUpSource
#if (STD_ON == ETHIF_USE_ETHTRCV)
#define ETHTRCV_DRIVER_API(driverId) EthTrcv_DriverApi[driverId]
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
#define ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx)   EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfPhysControllerIdRef
#define ETHIF_PHYSCONTROLLER_ETHCONTRLLER(PhysIdx) EthIf_ConfigPtr->EthIf_PhysControllerCfg[PhysIdx].EthIfEthCtrlRef->EthIfEthCtrlId
#define ETHIF_PHYSCONTROLLER_ETHDRIVERID(PhysIdx)  EthIf_ConfigPtr->EthIf_PhysControllerCfg[PhysIdx].EthIfEthCtrlRef->EthDriverId
#define ETH_DRIVER_API(driverId) Eth_DriverApi[driverId]
/*******************************************************************************
**                      Runtime Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef struct {
    uint8 ethifCrlIdx;
    uint32 frameType;
    uint32  bufferIdx;
}EthIfRunTimeType;

typedef struct {
    EthIf_StateType initStatus;  /* var to hold EthIf module status */
    /* ETHIF_MAX_FRAME_OWNER_CFG Mapping from buffer index to Frame type*/
    EthIfRunTimeType ethIfRunTime[ETHIF_MAX_FRAME_OWNER_CFG];
#if(ETHIF_VLAN_USED == STD_ON)
    Eth_ModeType ethIfContrllerMode[ETHIF_CTRL_NUM];
#endif
}EthIf_InternalType;

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static EthIf_InternalType EthIf_Internal =
{
    .initStatus = ETHIF_STATE_UNINIT,

};
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static P2CONST(EthIf_ConfigType, AUTOMATIC,ETHIF_VAR) EthIf_ConfigPtr = NULL_PTR;
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static FUNC(Std_ReturnType,ETHIF_CODE) searchEthifCrlIndexFmVlan(
    P2VAR(uint8, AUTOMATIC, ETHIF_VAR) ethifCrlId,
    VAR(uint8, ETHIF_VAR) ethCrlId,
    VAR(uint16, ETHIF_VAR) vlanId);
static FUNC(Std_ReturnType,ETHIF_CODE) searchEthifCrlIndex(
    P2VAR(uint8, AUTOMATIC, ETHIF_VAR) ethifCrlId,
    VAR(uint8, ETHIF_VAR) ethCrlId);
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/**
 * Initializes the Ethernet Interface.
 * Service ID: 0x01
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CfgPtr:Points to the implementation specific structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_Init( P2CONST(EthIf_ConfigType, ETHIF_APPL_DATA, ETHIF_VAR) CfgPtr )
{
    uint8 index = 0u;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(NULL_PTR == CfgPtr)
    {
         ETHIF_DET_REPORT(ETHIF_SERVICE_ID_INIT,ETHIF_E_INIT_FAILED);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
         EthIf_ConfigPtr = CfgPtr;
         for (index=0; index < EthIf_ConfigPtr->EthIfOwnersCount; index++)
         {
            EthIf_Internal.ethIfRunTime[index].frameType = EthIf_ConfigPtr->EthIfOwnerCfg[index].EthIfFrameType;
            EthIf_Internal.ethIfRunTime[index].bufferIdx = INVALID_BUFFER_INDEX;
            EthIf_Internal.ethIfRunTime[index].ethifCrlIdx = INVALID_UINT8;
         }
         EthIf_Internal.initStatus = ETHIF_STATE_INIT;
    }
}
/**
 * Initializes the Ethernet Interface.
 * Service ID: 0x0b
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): VersionInfoPtr:Version information of this module
 * Return value: NA
 *
 */
#if (ETHIF_VERSION_INFO_API == STD_ON)
FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo( Std_VersionInfoType* VersionInfoPtr )
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfoPtr)
    {
        /*report development error*/
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_VERSION_INFO,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    { 
        VersionInfoPtr->moduleID = ETHIF_MODULE_ID;
        VersionInfoPtr->sw_major_version = ETHIF_H_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = ETHIF_H_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = ETHIF_H_SW_PATCH_VERSION;
        VersionInfoPtr->vendorID = ETHIF_VENDOR_ID;
    }
}
#endif
/**
 * Enables / disables the indexed controller.
 * Service ID: 0x03
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 CtrlMode:ETH_MODE_DOWN: disable the controller ETH_MODE_ACTIVE: enable the controller
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType:E_OK: success E_NOT_OK: controller mode could not be changed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetControllerMode(
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    VAR(Eth_ModeType, ETHIF_VAR) CtrlMode )
{
    Std_ReturnType ret;
    ret = E_NOT_OK;
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE,ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        if(ETH_MODE_DOWN == CtrlMode)
        {
            ret = ETH_DRIVER_API(driverId).Eth_SetControllerMode(ethContrllerId,CtrlMode);
            /*@SWS_EthIf_00263*/
#if(ETHIF_VLAN_USED == STD_ON)
            EthIf_Internal.ethIfContrllerMode[CtrlIdx] = CtrlMode;
#endif
        }
        else
        {
            ret = ETH_DRIVER_API(driverId).Eth_SetControllerMode(ethContrllerId,CtrlMode);
        }

    }
    return ret;
}
/**
 * Obtains the state of the indexed controller.
 * Service ID: 0x04
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): CtrlModePtr:ETH_MODE_DOWN: the controller is disabled ETH_MODE_ACTIVE: the controller is enabled
 * Return value: Std_ReturnType:E_OK: success E_NOT_OK: controller could not be initialized
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetControllerMode( 
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) CtrlModePtr )
{
    Std_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_CTRL_MODE,ETHIF_E_INV_CTRL_IDX);
    }
    else if(CtrlModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_MODE,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = ETH_DRIVER_API(driverId).Eth_GetControllerMode(ethContrllerId,CtrlModePtr);
    }
    return ret;
}
/**
 * Enables / disables the wake up mode or clear the wake-up reason of the 
 * indexed transceiver
 * Service ID: 0x2e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): TrcvIdx:Index of the transceiver within the context of the 
 * Ethernet Interface
 *                 TrcvWakeupMode:ETHTRCV_WUM_DISABLE: disable transceiver wake
 * up ETHTRCV_WUM_ENABLE: enable transceiver wake up ETHTRCV_WUM_CLEAR: clears
 * transceiver wake up reason
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: success E_NOT_OK: transceiver wake up could not be
 * changed or wake-up reason could not be cleared
 *
 */
#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
#if (STD_ON == ETHIF_USE_ETHTRCV)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetTransceiverWakeupMode ( 
    VAR(uint8, ETHIF_VAR) TrcvIdx, 
    VAR(EthTrcv_WakeupModeType, ETHIF_VAR) TrcvWakeupMode )
{
    Std_ReturnType ret;
    EthTrcv_WakeupModeType currentTrcvWakeupMode;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_TRCV_WAKEUP_MODE,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfTrcvCount) <= TrcvIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_TRCV_WAKEUP_MODE,ETHIF_E_INV_TRCV_IDX);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethTrcvIdx = ETHIF_TRCV_ETHTRCV(TrcvIdx);
        trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx);
        ret = EthIf_GetTransceiverWakeupMode(ethTrcvIdx,&currentTrcvWakeupMode);
#if (STD_ON == ETHIF_USE_ETHTRCV)
        if(E_OK == ret)
        {
            if(((ETHTRCV_WUM_ENABLE == TrcvWakeupMode)&&(ETHTRCV_WUM_DISABLE == currentTrcvWakeupMode))||
                ((ETHTRCV_WUM_DISABLE == TrcvWakeupMode)&&(ETHTRCV_WUM_ENABLE == currentTrcvWakeupMode))||
                ((ETHTRCV_WUM_CLEAR == TrcvWakeupMode)&&(ETHTRCV_WUM_DISABLE == currentTrcvWakeupMode)))
            {
                if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_SetTransceiverWakeupMode)
                {
                    ret = ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_SetTransceiverWakeupMode(ethTrcvIdx,TrcvWakeupMode);
                }
            }
        }
#endif /* TD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
/**
 * Returns the wake up mode of the indexed transceiver
 * Service ID: 0x2f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): TrcvIdx:Index of the transceiver within the context of the 
 * Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvWakeupModePtr:ETHTRCV_WUM_DISABLE: transceiver wake up
 * is disabled ETHTRCV_WUM_ENABLE: transceiver wake up is enabled
 * Return value: E_OK: transceiver wake up mode could not be obtained,E_NOT_OK: transceiver wake up mode could not be obtained
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverWakeupMode ( 
    VAR(uint8, ETHIF_VAR) TrcvIdx, 
    P2VAR(EthTrcv_WakeupModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvWakeupModePtr )
{
    Std_ReturnType ret;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfTrcvCount) <= TrcvIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE,ETHIF_E_INV_TRCV_IDX);
    }
    else if(TrcvWakeupModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_WAKEUP_MODE,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethTrcvIdx = ETHIF_TRCV_ETHTRCV(TrcvIdx);
        trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx);
#if (STD_ON == ETHIF_USE_ETHTRCV)
        if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_GetTransceiverWakeupMode)
        {
            ret = ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_GetTransceiverWakeupMode(ethTrcvIdx,TrcvWakeupModePtr);
        }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
#endif
/**
 * Service is called by integration code to check a wakeup source.
 * Service ID: 0x30
 * Sync/Async: Asynchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WakeupSource:source (transceiver) which initiated the wake 
 * up event
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: E_OK: when function has been successfully executed,E_NOT_OK:
 * when function could not be successfully executed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_CheckWakeup ( VAR(EcuM_WakeupSourceType, ETHIF_VAR) WakeupSource )
{
    Std_ReturnType ret;
    EcuM_WakeupSourceType trcvWakeupSource;
    uint8 trcvIdx;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CHECK_WAKE_UP,ETHIF_E_UNINIT);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
#if (STD_ON == ETHIF_USE_ETHTRCV)
        for(trcvIdx = 0;trcvIdx < EthIf_ConfigPtr->EthIfTrcvCount;trcvIdx++)
        {
            trcvWakeupSource = ETHIF_TRCV_WAKEUPSOURCE(trcvIdx);
            if (0u != (WakeupSource & trcvWakeupSource))
            {
                ethTrcvIdx = ETHIF_TRCV_ETHTRCV(trcvIdx);
                trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(trcvIdx);
                if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_CheckWakeup)
                (
                    ret = ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_CheckWakeup(ethTrcvIdx);
                )
                break;
            }
        }
#endif  /* STD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
#endif
#if (STD_ON == ETHIF_USE_ETHTRCV)
/**
 * Obtain state of the indexed transceiver
 * Service ID: 0x07
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): TrcvModePtr:ETHTRCV_MODE_DOWN: the transceiver is disabled 
 *                  ETHTRCV_MODE_ACTIVE: the transceiver is enabled
 * Return value: Std_ReturnType:E_OK: success E_NOT_OK: transceiver mode could not be obtained
 *
 */
#if(4u == ETHIF_ETH_MAIJORVERSION)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverMode( VAR(uint8, ETHIF_VAR) TrcvIdx, 
    P2VAR(EthTrcv_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvModePtr )
#elif(19u == ETHIF_ETH_MAIJORVERSION)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverMode( VAR(uint8, ETHIF_VAR) TrcvIdx, 
    P2VAR(Eth_ModeType, ETHIF_APPL_DATA, ETHIF_VAR) TrcvModePtr )
#endif
{
    Std_ReturnType ret;
    uint8 ethTrcvIdx;
    uint8 trcvDriverId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfTrcvCount) <= TrcvIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE,ETHIF_E_INV_TRCV_IDX);
    }
    else if(TrcvModePtr == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_TRCV_MODE,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ethTrcvIdx = ETHIF_TRCV_ETHTRCV(TrcvIdx);
        trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(TrcvIdx);
#if (STD_ON == ETHIF_USE_ETHTRCV)
        if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_GetTransceiverMode)
        {
            ret = ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_GetTransceiverMode(ethTrcvIdx, TrcvModePtr);
        }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
    }
    return ret;
}
#endif
/**
 * Provides access to a transmit buffer of the specified Ethernet controller.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 FrameType:Ethernet Frame Type (EtherType)
 *                 Priority:Priority value which shall be used for the 3-bit PCP field of the VLAN tag
 * Parameters(INOUT): LenBytePtr:(in: desired length in bytes, out: granted length in bytes)
 * Parameters(OUT): BufIdxPtr:Index to the granted buffer resource. To be used for subsequent requests
 *                  BufPtr:Pointer to the granted buffer
 * Return value: BufReq_ReturnType:(BUFREQ_OK: success BUFREQ_E_NOT_OK: development error detected 
 *                                  BUFREQ_E_BUSY: all buffers in use)
 *
 */
FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideTxBuffer( VAR(uint8, ETHIF_VAR)         CtrlIdx,
                                        VAR(Eth_FrameType, ETHIF_VAR)    FrameType,
                                        VAR(uint8, ETHIF_VAR)            Priority,
                                        P2VAR(Eth_BufIdxType, ETHIF_APPL_DATA, ETHIF_VAR)    BufIdxPtr,
                                        P2VAR(uint8 *, ETHIF_APPL_DATA, ETHIF_VAR)       BufPtr,
                                        P2VAR(uint16, ETHIF_APPL_DATA, ETHIF_VAR)       LenBytePtr
                                        )
{
    BufReq_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;
#if(ETHIF_VLAN_USED == STD_ON)
    uint8 * IntenalBuf;
#endif
    ret = BUFREQ_E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER,ETHIF_E_INV_CTRL_IDX);
    }
    else if((BufIdxPtr == NULL_PTR)||(BufPtr == NULL_PTR)||(LenBytePtr == NULL_PTR))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_PROVIDE_TX_BUFFER,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
#if(ETHIF_VLAN_USED != STD_ON)
        if((EthIf_ConfigPtr->EthIfCtrlCfg->EthIfCtrlMtu) >= *LenBytePtr)
        {
#if(4u == ETHIF_ETH_MAIJORVERSION)
            ret = ETH_DRIVER_API(driverId).Eth_ProvideTxBuffer(ethContrllerId, BufIdxPtr, BufPtr, LenBytePtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
            ret = ETH_DRIVER_API(driverId).Eth_ProvideTxBuffer(ethContrllerId, Priority, BufIdxPtr, BufPtr, LenBytePtr);
#endif
        }
        else
        {
            ret = BUFREQ_E_NOT_OK;
        }
#else
        if(((EthIf_ConfigPtr->EthIfCtrlCfg->EthIfCtrlMtu) >= *LenBytePtr)&&(Priority < PRIORITY_MAX))
        {
            *LenBytePtr= (*LenBytePtr + VLAN_TAG_SIZE);
            /*@req SWS_EthIf_00068 */
#if(4u == ETHIF_ETH_MAIJORVERSION)
            ret = ETH_DRIVER_API(driverId).Eth_ProvideTxBuffer(ethContrllerId, BufIdxPtr, BufPtr, LenBytePtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
            ret = ETH_DRIVER_API(driverId).Eth_ProvideTxBuffer(ethContrllerId, Priority, BufIdxPtr, BufPtr, LenBytePtr);
#endif
            IntenalBuf = *BufPtr;
            if(BUFREQ_OK == ret)
            {
                IntenalBuf[0]  = (uint8)( Priority << 5u);
                IntenalBuf[0] &= (uint8)((~((uint8)1) << 4u));
                IntenalBuf[0] |= (uint8)((EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId & 0x0F00u) >> 8u);
                IntenalBuf[1]  = (uint8)(EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfVlanId & 0x00FFu);
                IntenalBuf[2]  = (uint8)((FrameType >> 8u) & 0xFFu);
                IntenalBuf[3]  = (uint8)( FrameType & 0xFFu);
                *BufPtr = &IntenalBuf[VLAN_TAG_SIZE];
                *LenBytePtr = (*LenBytePtr - VLAN_TAG_SIZE);
            }
            else
            {
                ret = BUFREQ_E_NOT_OK;
            }
        }
#endif
    }
    return  ret ;
}

/**
 * Triggers transmission of a previously filled transmit buffer
 * Service ID: 0x0a
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 *                 FrameType:Ethernet frame type
 *                 TxConfirmation:Activates transmission confirmation
 *                 PhysAddrPtr:Physical target address (MAC address) in network byte order
 * Parameters(INOUT): LenByte:Data length in byte
 * Parameters(OUT): BufIdx,Index of the buffer resource
 * Return value: Std_ReturnType,E_OK: success E_NOT_OK: transmission failed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Transmit(
                  VAR(uint8, ETHIF_VAR) CtrlIdx,
                  VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx,
                  VAR(Eth_FrameType, ETHIF_VAR) FrameType,
                  VAR(boolean, ETHIF_VAR) TxConfirmation,
                  VAR(uint16, ETHIF_VAR) LenByte,
                  P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr
                       )
{
    Std_ReturnType ret;
    uint8 i;
    uint8 driverId;
    uint8 ethContrllerId;

    ret = E_NOT_OK;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT,ETHIF_E_INV_CTRL_IDX);
    }
    else if(BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT,ETHIF_E_INV_PARAM);
    }
    else if(NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRANSMIT,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        if(EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfCtrlMtu >= LenByte)
        {
#if(ETHIF_VLAN_USED == STD_ON)
            {
                ret = ETH_DRIVER_API(driverId).Eth_Transmit(ethContrllerId,BufIdx, 
                                                            ETH_FRAME_TYPE_VLAN, 
                                                            TxConfirmation, 
                                                            (LenByte + VLAN_TAG_SIZE), PhysAddrPtr);
            }
#else
            {
                ret = ETH_DRIVER_API(driverId).Eth_Transmit(ethContrllerId,BufIdx, FrameType, 
                                                            TxConfirmation, LenByte, PhysAddrPtr);
            }
#endif
            if ((E_OK == ret) && (EthIf_ConfigPtr->EthIfULTxConfirmation != NULL_PTR))
            {
                  for (i=0; i < EthIf_ConfigPtr->EthIfOwnersCount; i++)
                  {
                      if ((FrameType == EthIf_Internal.ethIfRunTime[i].frameType))
                      {
                          if (TRUE == TxConfirmation)
                          {
                               EthIf_Internal.ethIfRunTime[i].bufferIdx = BufIdx;
                               EthIf_Internal.ethIfRunTime[i].ethifCrlIdx = CtrlIdx;
                          }
                          else
                          {
                               EthIf_Internal.ethIfRunTime[i].bufferIdx = INVALID_BUFFER_INDEX;
                          }
                          break;
                     }
                  }
             }
        }
        else
        {
            ret = E_NOT_OK;
        }

    }
    return ret ;

}
/**
 * Confirms frame transmission by the indexed controller
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx:Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, ETHIF_CODE) EthIf_TxConfirmation( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx )
{
    void (*upperLayerFun)(uint8 CtrlIdx, Eth_BufIdxType BufIdx);
    uint8 i;
    uint8 index;
    uint8 ethifCrlId;
    uint8 handle;
    Std_ReturnType ret;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfPhysControllerCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION,ETHIF_E_INV_CTRL_IDX);
    }
    else if(BufIdx >= EthIf_ConfigPtr->EthIfCtrlCfg[CtrlIdx].EthIfMaxTxBufsTotal)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TX_CONFIRMATION,ETHIF_E_INV_PARAM);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        if(NULL_PTR != EthIf_ConfigPtr->EthIfULTxConfirmation)
        {
          for (i=0; i < EthIf_ConfigPtr->EthIfOwnersCount; i++)
          {
            if (BufIdx == EthIf_Internal.ethIfRunTime[i].bufferIdx )
            {
                for(index = 0;index < EthIf_ConfigPtr->EthIfCtrlCount;index++)
                {
                    if((ETHIF_CONTROLLER_PHYSCONTROLLER(index) == CtrlIdx)&&
                        (index == EthIf_Internal.ethIfRunTime[i].ethifCrlIdx))
                    {
                        ethifCrlId = index;
                        ret = E_OK;
                        break;
                    }
                }
                if(ret == E_OK)
                {
                    handle = EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfOwner;
                    if(NULL_PTR != EthIf_ConfigPtr->EthIfULTxConfirmation[handle])
                    {
                        upperLayerFun   = EthIf_ConfigPtr->EthIfULTxConfirmation[handle];
                        (* upperLayerFun)( ethifCrlId, BufIdx );
                    }                        
                }
                break;
            }
          }
        }
    }

}
/**
 * Handles a received frame received by the indexed controller
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 FrameType,Frame type of received Ethernet frame
 *                 IsBroadcast,parameter to indicate a broadcast frame
 *                 PhysAddrPtr,Pointer to Physical source address (MAC address in network byte order) of 
 *                 received Ethernet frame DataPtr,Pointer to payload of received Ethernet frame.
 *                 LenByte,Length of the received frame bytes
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_RxIndication( 
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    VAR(Eth_FrameType, ETHIF_VAR) FrameType, 
    VAR(boolean, ETHIF_VAR) IsBroadcast, 
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr, 
    P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr, 
    VAR(uint16, ETHIF_VAR) LenByte )
{
    void (*upperLayerFun)(uint8 CtrlIdx,Eth_FrameType FrameType,boolean IsBroadcast,
                                         const uint8* PhysAddrPtr,const uint8* DataPtr,uint16 LenByte);
    Eth_FrameType UpperLayerFrameType = 0u;
    uint16 UpperLayerLenByte = 0u;
    boolean status = TRUE;
#if(ETHIF_VLAN_USED == STD_ON)
    uint16 tci = 0u;
#endif
    uint8 i = 0u;
    uint8 handle = 0u;
    uint8 ethifCtrlId;
    Std_ReturnType ret;
    uint16 vlanId;
    const uint8* UpperLayerDataPtr = NULL_PTR;
    const uint8* u8DataPtr = (const uint8*)DataPtr;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfPhysControllerCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION,ETHIF_E_INV_CTRL_IDX);
    }
    else if(NULL_PTR == u8DataPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_RX_INDICATION,ETHIF_E_PARAM_POINTER);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
#if(ETHIF_VLAN_USED == STD_ON)
        if(FrameType == ETH_FRAME_TYPE_VLAN)
        {
            tci |= (uint16) u8DataPtr[0] << 8u;
            tci |= (uint16) u8DataPtr[1];
            vlanId = tci & ETHIF_VID_MASK;
        }
#else
        vlanId = ETHIF_INVALID_VID;
#endif
        ret = searchEthifCrlIndexFmVlan(&ethifCtrlId,CtrlIdx,vlanId);   
    }
    if(E_NOT_OK != ret)
    {
#if(ETHIF_VLAN_USED != STD_ON)
        {
            UpperLayerFrameType = FrameType;
            UpperLayerDataPtr   = u8DataPtr;
            UpperLayerLenByte   = LenByte;
        }
#else
        {
             if(FrameType == ETH_FRAME_TYPE_VLAN)
             {
                if(vlanId != EthIf_ConfigPtr->EthIfCtrlCfg[ethifCtrlId].EthIfVlanId)
                {
                    status = FALSE;
                }
                else
                {
                    UpperLayerFrameType = 0u;
                    UpperLayerDataPtr   = &u8DataPtr[VLAN_TAG_SIZE];
                    UpperLayerLenByte   = LenByte-VLAN_TAG_SIZE;
                    UpperLayerFrameType |=  (Eth_FrameType) u8DataPtr[2] << 8u;
                    UpperLayerFrameType |=  (Eth_FrameType) u8DataPtr[3];
                }
             }
             else
             {
                 status = TRUE;
             }
        }
#endif
#if(ETHIF_VLAN_USED == STD_ON)
        if (status == TRUE)
#endif
        {
            for(i =0 ; i <  EthIf_ConfigPtr->EthIfOwnersCount; i++)
            {
                if(UpperLayerFrameType == EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfFrameType)
                {
                    handle = EthIf_ConfigPtr->EthIfOwnerCfg[i].EthIfOwner;
                    if(NULL_PTR != EthIf_ConfigPtr->EthIfULRxIndication)
                    {
                        if(NULL_PTR != EthIf_ConfigPtr->EthIfULRxIndication[handle])
                        {
                            upperLayerFun   = EthIf_ConfigPtr->EthIfULRxIndication[handle];
                            (* upperLayerFun)(ethifCtrlId, UpperLayerFrameType, IsBroadcast, PhysAddrPtr, 
                                            (const uint8 *)UpperLayerDataPtr, UpperLayerLenByte);
                        }
                    }
                }

            }
        }
    }
}
/**
 * Sets the physical source address used by the indexed controller.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 PhysAddrPtr,Pointer to Physical source address (MAC address in network byte order) of received Ethernet frame
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_SetPhysAddr(
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr )
{
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR,ETHIF_E_INV_CTRL_IDX);
    }
    else if(NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_SET_PHY_ADDR,ETHIF_E_PARAM_POINTER);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        (void)ETH_DRIVER_API(driverId).Eth_SetPhysAddr(ethContrllerId,PhysAddrPtr);
    }
}
/**
 * Obtains the physical source address used by the indexed controller
 * Service ID: 0x08
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): PhysAddrPtr,Physical source address (MAC address) in network byte order.
 * Return value: NA
 */
FUNC(void, ETHIF_CODE) EthIf_GetPhysAddr( VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr )
{
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR,ETHIF_E_INV_CTRL_IDX);
    }
    else if(NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_PHY_ADDR,ETHIF_E_PARAM_POINTER);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        (void)ETH_DRIVER_API(driverId).Eth_GetPhysAddr(ethContrllerId, PhysAddrPtr);
    }
}
/**
 * Update the physical source address to/from the indexed controller filter. If the Ethernet Controller is not capable to do
 *  the filtering, the software has to do this.
 * Service ID: 0x0c
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 PhysAddrPtr,Physical source address (MAC address) in network byte order.
 *                 Action,Add or remove the address from the Ethernet controllers filter.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E_OK: filter was successfully changed E_NOT_OK: filter could not be changed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_UpdatePhysAddrFilter( 
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2CONST(uint8, ETHIF_APPL_DATA, ETHIF_VAR) PhysAddrPtr, 
    VAR(Eth_FilterActionType, ETHIF_VAR) Action )
{
    Std_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;
    ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER,ETHIF_E_INV_CTRL_IDX);
    }
    else if(NULL_PTR == PhysAddrPtr)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_UPDATE_PHY_ADDR_FILTER,ETHIF_E_PARAM_POINTER);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = ETH_DRIVER_API(driverId).Eth_UpdatePhysAddrFilter(ethContrllerId, PhysAddrPtr, Action);
    }
    return ret;
}

#if (ETHIF_GLOBAL_TIME_SUPPORT == STD_ON)
/**
 * Returns a time value out of the HW registers according to the capability of the HW. Is the HW resolution is lower than
 *  the Eth_TimeStampType resolution resp. range, than an the remaining bits will be filled with 0.
 * Service ID: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 * Parameters(INOUT): NA
 * Parameters(OUT): timeQualPtr,quality of HW time stamp, e.g. based on current drift
 *                  timeStampPtr,current time stamp
 * Return value: Std_ReturnType,E_OK: successful E_NOT_OK: failed
 *
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCurrentTime( 
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr, 
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr )
{
    Std_ReturnType ret;
    uint8 driverId;
    uint8 ethContrllerId;
    ret = E_NOT_OK;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME,ETHIF_E_INV_CTRL_IDX);
    }
    else if((NULL_PTR == timeQualPtr)||(NULL_PTR == timeStampPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CURRENT_TIME,ETHIF_E_PARAM_POINTER);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = ETH_DRIVER_API(driverId).Eth_GetCurrentTime(ethContrllerId, timeQualPtr, timeStampPtr );
    }
    return ret;
}


/**
 * Activates egress time stamping on a dedicated message object. Some HW does store once the egress time stamp marker and some HW needs 
 * it always before transmission. There will be no functionality, due to the fact, that the message type is always "time
 *  stamped" by network design.
 * Service ID: 0x23
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 BufIdx,Index of the message buffer, where Application expects egress time stamping
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_EnableEgressTimeStamp( 
    VAR(uint8, ETHIF_VAR) CtrlIdx, 
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx )
{
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ENABLE_EGRESS_TIME_STAMP,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_ENABLE_EGRESS_TIME_STAMP,ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        (void)ETH_DRIVER_API(driverId).Eth_EnableEgressTimeStamp(ethContrllerId, BufIdx );
    }
}
/**
 * Reads back the egress time stamp on a dedicated message object. It must be called within the TxConfirmation() function.
 * Service ID: 0x24
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 BufIdx,Index of the message buffer, where Application expects egress time stamping
 * Parameters(INOUT): NA
 * Parameters(OUT): timeQualPtr,quality of HW time stamp, e.g. based on current drift
 *                  timeStampPtr,current time stamp
 * Return value: NA
 *
 */
#if(4u == ETHIF_ETH_MAIJORVERSION)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetEgressTimeStamp(VAR(uint8, ETHIF_VAR) CtrlIdx, 
    VAR(uint8, ETHIF_VAR) BufIdx, 
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr, 
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr )
#elif(19u == ETHIF_ETH_MAIJORVERSION)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetEgressTimeStamp(VAR(uint8, ETHIF_VAR) CtrlIdx, 
    VAR(Eth_BufIdxType, ETHIF_VAR) BufIdx, 
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr, 
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr )
#endif
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP,ETHIF_E_INV_CTRL_IDX);
    }
    else if((NULL_PTR == timeQualPtr)||(NULL_PTR == timeStampPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_EGRESS_TIME_STAMP,ETHIF_E_PARAM_POINTER);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = ETH_DRIVER_API(driverId).Eth_GetEgressTimeStamp(ethContrllerId, BufIdx, timeQualPtr, timeStampPtr );
    } 
    return ret;   
}
/**
 * Reads back the ingress time stamp on a dedicated message object. It must be called within the RxIndication() function.
 * Service ID: 0x25
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 DataPtr,Pointer to the message buffer, where Application expects ingress time stamping
 * Parameters(INOUT): NA
 * Parameters(OUT): timeQualPtr,quality of HW time stamp, e.g. based on current drift
 *                  timeStampPtr,current time stamp
 * Return value: NA
 *
 */
#if(4u == ETHIF_ETH_MAIJORVERSION)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetIngressTimeStamp( VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2VAR(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr, 
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr, 
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr )
#elif(19u == ETHIF_ETH_MAIJORVERSION)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetIngressTimeStamp( VAR(uint8, ETHIF_VAR) CtrlIdx, 
    P2CONST(Eth_DataType, ETHIF_APPL_DATA, ETHIF_VAR) DataPtr, 
    P2VAR(Eth_TimeStampQualType, ETHIF_APPL_DATA, ETHIF_VAR) timeQualPtr, 
    P2VAR(Eth_TimeStampType, ETHIF_APPL_DATA, ETHIF_VAR) timeStampPtr )
#endif
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP,ETHIF_E_INV_CTRL_IDX);
    }
    else if((NULL_PTR == timeQualPtr)||(NULL_PTR == timeStampPtr)||(NULL_PTR == DataPtr))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_INGRESS_TIME_STAMP,ETHIF_E_PARAM_POINTER);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
        ret = ETH_DRIVER_API(driverId).Eth_GetIngressTimeStamp(ethContrllerId,DataPtr,timeQualPtr,timeStampPtr);
    }     
    return ret;   
}
#endif

#if (ETHIF_ENABLE_RX_INTERRUPT == STD_ON)
/**
 * The function checks for new received frames and issues transmission confirmations in
 *  polling mode. It checks also for transceiver state changes.
 * Service ID: 0x20
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx( void )
{
    Eth_RxStatusType RxStatusPtr = ETH_NOT_RECEIVED;
    uint8 i;
    uint8 CtrlIdx;
    uint8 driverId;
    uint8 ethContrllerId;
#if(19u == ETHIF_ETH_MAIJORVERSION)
    uint8 FifoIdx;
#endif
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_RX,ETHIF_E_UNINIT);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        for(CtrlIdx = 0u; CtrlIdx < ETHIF_CTRL_NUM; CtrlIdx++)
        {
            driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
            ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
#if(4u == ETHIF_ETH_MAIJORVERSION)
            (void)ETH_DRIVER_API(driverId).Eth_Receive(ethContrllerId, &RxStatusPtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
            (void)ETH_DRIVER_API(driverId).Eth_Receive(ethContrllerId, FifoIdx, &RxStatusPtr);/*FifoIdx TODO*/
#endif

            for(i= 0; i < ETHIF_RX_INDICATION_ITERATIONS_MAX; i++)
            {
                if(RxStatusPtr == ETH_RECEIVED_MORE_DATA_AVAILABLE)
                {
#if(4u == ETHIF_ETH_MAIJORVERSION)
                    (void)ETH_DRIVER_API(driverId).Eth_Receive(ethContrllerId, &RxStatusPtr);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
                    (void)ETH_DRIVER_API(driverId).Eth_Receive(ethContrllerId, FifoIdx, &RxStatusPtr);/*FifoIdx TODO*/
#endif
                }
                else
                {
                    break;
                }
            }
        }
    }
}

#endif

#if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
/**
 * The function issues transmission confirmations in polling mode.
 *  It checks also for transceiver state changes.
 * Service ID: 0x21
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionTx( void )
{

    uint8 CtrlIdx;
    uint8 driverId;
    uint8 ethContrllerId;
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_TX,ETHIF_E_UNINIT);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        for(CtrlIdx=0; CtrlIdx < ETHIF_CTRL_NUM; CtrlIdx++)
        {
            driverId = ETHIF_PHYSCONTROLLER_ETHDRIVERID(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
            ethContrllerId = ETHIF_PHYSCONTROLLER_ETHCONTRLLER(ETHIF_CONTROLLER_PHYSCONTROLLER(CtrlIdx));
            (void)ETH_DRIVER_API(driverId).Eth_TxConfirmation(ethContrllerId);
        }
    }
}

#endif

/**
 * The function is polling different communication hardware (Ethernet 
 * transceiver, Ethernet switch ports) related information, e.g. link state, 
 * signal quality.
 * Service ID: 0x05
 */
FUNC(void, ETHIF_CODE) EthIf_MainFunctionState ( void )
{
    uint8 trcvlIdx;
    uint8 EthCtrlIdx;
    uint8 EthIfCtrlIdx;
    EthTrcv_LinkStateType linkState;
    uint8 driverId;
#if (STD_ON == ETHIF_USE_ETHTRCV)
    uint8 trcvDriverId;
#endif
    boolean modeCtrl = TRUE;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_MAIN_FUNCTION_STATE,ETHIF_E_UNINIT);
    } 
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
#if (STD_ON == ETHIF_USE_ETHTRCV)
        for(trcvlIdx = 0;trcvlIdx < EthIf_ConfigPtr->EthIfTrcvCount;trcvlIdx++)
        {
            trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(trcvlIdx);
            if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_GetLinkState)
            {
                if(E_OK == ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_GetLinkState(ETHIF_TRCV_ETHTRCV(trcvlIdx), 
                                                                                &linkState ))
                {
                    /*TO notify EthSM the current trcv link state*/
                    EthSM_TrcvLinkStateChg (trcvlIdx, linkState);
                }
            }
        }
#endif  /* STD_ON == ETHIF_USE_ETHTRCV */
#if(ETHIF_VLAN_USED == STD_ON)
        /*@req SWS263*/
        for(EthCtrlIdx = 0;EthCtrlIdx < EthIf_ConfigPtr->EthIfPhysControllerCount;EthCtrlIdx++)
        {
            for(EthIfCtrlIdx = 0;EthIfCtrlIdx < EthIf_ConfigPtr->EthIfCtrlCount;EthIfCtrlIdx++)
            {
                if(EthIf_ConfigPtr->EthIfCtrlCfg[EthIfCtrlIdx].EthIfPhysControllerIdRef == EthCtrlIdx)
                {
                    if(EthIf_Internal.ethIfContrllerMode[EthIfCtrlIdx] != ETH_MODE_DOWN)
                    {
                        modeCtrl = FALSE;
                    }
                }
            }
            if(TRUE == modeCtrl)
            {
                driverId = EthIf_ConfigPtr->EthIf_PhysControllerCfg[EthCtrlIdx].EthIfEthCtrlRef->EthDriverId;
                (void)ETH_DRIVER_API(driverId).Eth_SetControllerMode(EthCtrlIdx, ETH_MODE_DOWN);
            }
        }
#endif
    }

}
/**
 * Called asynchronously when mode has been read out. Triggered by previous Eth_SetControllerMode call. 
 * Can directly be called within the trigger functions.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 CtrlMode,Notified Ethernet controller mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_CtrlModeIndication( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode )
{   
    Std_ReturnType ret = E_NOT_OK;
    uint8 ethIfCrlId;
#if (STD_ON == ETHIF_USE_ETHTRCV)
    uint8  trcvId;
    uint8  trcvDriverId;
#endif
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CTRL_MODE_INDICATION,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfPhysControllerCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_CTRL_MODE_INDICATION,ETHIF_E_INV_CTRL_IDX);
    }  
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        ret = searchEthifCrlIndex(&ethIfCrlId,CtrlIdx);
        if(E_OK == ret)
        {
#if (STD_ON == ETHIF_USE_ETHTRCV)
            trcvId = ETHIF_TRCV_ETHTRCV(ETHIF_CONTROLLER_TRCVID(ethIfCrlId));
            trcvDriverId = ETHIF_TRCV_ETHTRCVDRIVERID(trcvId);
            if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_SetTransceiverMode)
            {
#if(4u == ETHIF_ETH_MAIJORVERSION)
                (void)ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_SetTransceiverMode(trcvId, (EthTrcv_ModeType)CtrlMode);
#elif(19u == ETHIF_ETH_MAIJORVERSION)
                (void)ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_SetTransceiverMode(trcvId, CtrlMode);
#endif
            }
            if (NULL_PTR != ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_TransceiverLinkStateRequest)
            {
                if(ETH_MODE_ACTIVE == CtrlMode)
                {
                    (void)ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_TransceiverLinkStateRequest(trcvId,
                                                                                            ETHTRCV_LINK_STATE_ACTIVE);
                }
                else if(ETH_MODE_DOWN == CtrlMode)
                {
                    (void)ETHTRCV_DRIVER_API(trcvDriverId).EthTrcv_TransceiverLinkStateRequest(trcvId,
                                                                                            ETHTRCV_LINK_STATE_DOWN);
                }
                else
                {
                    /*nothing to do*/
                }
            }
#endif /* STD_ON == ETHIF_USE_ETHTRCV */
            (void)EthSM_CtrlModeIndication(ethIfCrlId, CtrlMode);
        }
    }
      
}
/**
 * Called asynchronously when mode has been read out. Triggered by previous Eth_SetTransceiverMode call.
 *  Can directly be called within the trigger functions.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same CtrlIdx, reentrant for different
 * Parameters(IN): CtrlIdx,Index of the Ethernet controller within the context of the Ethernet Interface
 *                 TrcvMode,Notified Ethernet transceiver mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
FUNC(void, ETHIF_CODE) EthIf_TrcvModeIndication( VAR(uint8, ETHIF_VAR) CtrlIdx, VAR(Eth_ModeType, ETHIF_VAR) CtrlMode )
{
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION,ETHIF_E_UNINIT);
    }
    else if((EthIf_ConfigPtr->EthIfCtrlCount) <= CtrlIdx)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION,ETHIF_E_INV_CTRL_IDX);
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        /*nothing to do*/
    }
}
/**
 * Returns the number and index of all active Ethernet controllers.
 * Service ID: 0x44
 * Sync/Async: Asynchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NumberOfCtrlIdx,in: maximum number of controllers in CtrlIdxListPtr, 0 to return the number of 
 *                      controllers but without filling CtrlIdxListPtr. out: number of active controllers.
 * Parameters(OUT): CtrlIdxListPtr,List of active controller indexes
 * Return value: NA
 *
 */
#if (STD_ON == ETH_GET_CTRLIDX_LIST)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCtrlIdxList( 
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) NumberOfCtrlIdx, 
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) CtrlIdxListPtr )
{
    Eth_ModeType ctrlMode;
    uint8 index;
    uint8 maxCtrlNbr;
    Std_ReturnType ret;

#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    if(EthIf_Internal.initStatus != ETHIF_STATE_INIT)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST,ETHIF_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if(NumberOfCtrlIdx == NULL_PTR)
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_GET_CTRL_IDXLIST,ETHIF_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if(((*NumberOfCtrlIdx) != 0x00u)&&(CtrlIdxListPtr == NULL_PTR))
    {
        ETHIF_DET_REPORT(ETHIF_SERVICE_ID_TRCV_MODE_INDICATION,ETHIF_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else
#endif/*STD_ON == ETHIF_DEV_ERROR_DETECT*/
    {
        maxCtrlNbr = *NumberOfCtrlIdx;
        *NumberOfCtrlIdx = 0u;
        for(index = 0;index < EthIf_ConfigPtr->EthIfCtrlCount;index++)
        {
            if(E_OK == EthIf_GetControllerMode(index,&ctrlMode))
            {
                if(ETH_MODE_ACTIVE == ctrlMode)
                {
                    (*NumberOfCtrlIdx)++;
                    if((maxCtrlNbr != 0u)&&((*NumberOfCtrlIdx) < maxCtrlNbr))
                    {
                        CtrlIdxListPtr[*NumberOfCtrlIdx] = index;
                    }
                }
            }
        }
        ret = E_OK;
    }
    return ret;
}
#endif

static FUNC(Std_ReturnType, ETHIF_CODE) searchEthifCrlIndexFmVlan(
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) ethifCrlId,
    VAR(uint8, ETHIF_VAR) ethCrlId,
    VAR(uint16, ETHIF_VAR) vlanId)
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    
    for(index = 0;index < EthIf_ConfigPtr->EthIfCtrlCount;index++)
    {
        if(ETHIF_CONTROLLER_PHYSCONTROLLER(index) == ethCrlId)
        {
#if(ETHIF_VLAN_USED == STD_ON)
            if(vlanId == EthIf_ConfigPtr->EthIfCtrlCfg[index].EthIfVlanId)
            {
                *ethifCrlId = index;
                ret = E_OK;
                break;
            }
#else
            *ethifCrlId = index;
            ret = E_OK;
            break;
#endif
        }
    }
    return ret;
}

static FUNC(Std_ReturnType, ETHIF_CODE) searchEthifCrlIndex(
    P2VAR(uint8, ETHIF_APPL_DATA, ETHIF_VAR) ethifCrlId,
    VAR(uint8, ETHIF_VAR) ethCrlId)
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    
    for(index = 0;index < EthIf_ConfigPtr->EthIfCtrlCount;index++)
    {
        if(ETHIF_CONTROLLER_PHYSCONTROLLER(index) == ethCrlId)
        {
            *ethifCrlId = index;
            ret = E_OK;
            break;
        }
    }
    return ret;
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"






