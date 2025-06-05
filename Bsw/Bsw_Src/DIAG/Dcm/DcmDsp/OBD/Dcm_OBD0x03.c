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
**  FILENAME    : Dcm_OBD0x03.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x03 API definitions                                 **
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
 * Brief               <OBD service (0x03):  Request emission-related diagnostic trouble codes>
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
#if(STD_ON == DCM_OBD_SERVICE0X03_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)Dcm_OBD0x03(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        )
{
    uint32 dtc = 0;
    Dem_UdsStatusByteType dtcStatus = 0;
    uint16 TxOffset;
    uint8 DTCNumber = 0;
    uint8  MsgCtrlId;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;
    Std_ReturnType  ret = E_OK;
    Dem_ReturnSetFilterType DemRet;

	DCM_UNUSED(OpStatus);
	DCM_UNUSED(ErrorCode);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
    TxOffset = Offset +2u;
    /*check the massage length*/
    if (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen != DCM_UDS0X03_REQ_DATA_LENGTH)
    {
        ret = E_NOT_OK;
    }
    else
    {
        DemRet = Dem_DcmSetDTCFilter(DEM_UDS_STATUS_CDTC,
            DEM_DTC_KIND_EMISSION_REL_DTCS,DEM_DTC_FORMAT_OBD,DEM_DTC_ORIGIN_PRIMARY_MEMORY,
            DEM_FILTER_WITH_SEVERITY_NO,DEM_SEVERITY_NO_SEVERITY,DEM_FILTER_FOR_FDC_NO);
    }
    if((E_OK == ret) && (DEM_FILTER_ACCEPTED == DemRet))
    {
        while (DEM_FILTERED_OK == Dem_DcmGetNextFilteredDTC(&dtc, &dtcStatus))
        {
            if((TxOffset - Offset) >
            (Dcm_DslCfg.pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
            {
                /*Pdu length is bigger than buffer size,ignore the request message */
                ret = E_NOT_OK;
                break;
            }
            else
            {
                SchM_Enter_Dcm(Dcm_Channel);
                Dcm_Channel[TxOffset] = (uint8)((dtc & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                TxOffset++;
                Dcm_Channel[TxOffset] = (uint8)(dtc & OBD_DATA_LSB_MASK);
                TxOffset++;
                SchM_Exit_Dcm(Dcm_Channel);
                DTCNumber++;
            }
        }
        if (E_OK == ret)
        {
            SchM_Enter_Dcm(Dcm_Channel);
            Dcm_Channel[Offset]   = 0x43;
            Dcm_Channel[Offset + 1u] = DTCNumber;
            SchM_Exit_Dcm(Dcm_Channel);
            SchM_Enter_Dcm(Dcm_MsgCtrl);
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = (Dcm_MsgLenType)TxOffset - Offset;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen  = (Dcm_MsgLenType)TxOffset - Offset;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData  = &Dcm_Channel[Offset];
            SchM_Exit_Dcm(Dcm_MsgCtrl);
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    else
    {
        /*the condition is not correct,ignore the request message */
        ret = E_NOT_OK;
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X03_ENABLED*/

