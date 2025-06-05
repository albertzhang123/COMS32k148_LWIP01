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
**  FILENAME    : Dcm_OBD0x04.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x04 API definitions                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "OBD.h"
#include "Dcm_OBDInternal.h"

/******************************************************************************/
/*
 * Brief               <OBD service (0x04):  Clear/reset emission-related diagnostic information>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#if(STD_ON == DCM_OBD_SERVICE0X04_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)Dcm_OBD0x04(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        )
{
    uint8  MsgCtrlId;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;
    Std_ReturnType  ret = E_OK;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if (OpStatus != DCM_PENDING)
    {
        /*check the massage length*/
        if (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen != DCM_UDS0X04_REQ_DATA_LENGTH)
        {
            ret = E_NOT_OK;
        }
    }
    if ((E_OK == ret) && (Dcm_DspCfg.pDcmDspClearDTC != NULL_PTR)
              && (Dcm_DspCfg.pDcmDspClearDTC->DcmDsp_ClearDTCCheckFnc != NULL_PTR))
    {
		  ret = Dcm_DspCfg.pDcmDspClearDTC->DcmDsp_ClearDTCCheckFnc(
				  DEM_DTC_GROUP_ALL_DTCS, ErrorCode);
    }
    if (E_OK == ret)
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
        ret = Dem_DcmClearDTC(DEM_DTC_GROUP_ALL_DTCS,DEM_DTC_FORMAT_OBD,
                DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        if(DEM_CLEAR_OK == ret)
        {
            /*assemble positive response*/
            SchM_Enter_Dcm(Dcm_Channel);
            Dcm_Channel[Offset]   = 0x44;/*response SID*/
            SchM_Exit_Dcm(Dcm_Channel);
            SchM_Enter_Dcm(Dcm_MsgCtrl);
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 1u;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen  = 1u;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData  = &Dcm_Channel[Offset];
            SchM_Exit_Dcm(Dcm_MsgCtrl);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
            ret = E_OK;
        }
        else if (DEM_CLEAR_PENDING == ret)
        {
            SchM_Enter_Dcm(Dcm_MsgCtrl);
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            SchM_Exit_Dcm(Dcm_MsgCtrl);
        }
        else
        {
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X04_ENABLED*/
