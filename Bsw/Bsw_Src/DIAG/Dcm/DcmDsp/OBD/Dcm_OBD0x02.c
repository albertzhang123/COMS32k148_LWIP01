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
**  FILENAME    : Dcm_OBD0x02.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x02 API definitions                                 **
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
 * Brief               <OBD service (0x02):  Request Power Train FreezeFrame Data>
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
#if(STD_ON == DCM_OBD_SERVICE0X02_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)Dcm_OBD0x02(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        )
{
    Std_ReturnType  ret = E_OK;
    uint8  MsgCtrlId;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;
    uint8 iloop;
    uint8 PidNum;
    uint8 Pid;
    uint8 frame;
    uint8 Sumframe = 0;
    uint8 MixPid = 0;
    uint32 SupportBuffer;
    uint16 TxOffset;
    uint8 noFindPidNum = 0;
    uint32 DTC;
    OBD_ReadPidValuesTypes OBD_ReadPid;

	DCM_UNUSED(OpStatus);
	DCM_UNUSED(ErrorCode);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
    /* check the massage length and format */
    if ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X02_REQ_DATA_LENGTH_MIN)
       || (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen > DCM_UDS0X02_REQ_DATA_LENGTH_MAX)
       || (0u == (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen % 2u)))
    {
        /*the format of massage is not correct,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        PidNum = (uint8)Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 1u;
        SchM_Enter_Dcm(Dcm_Channel);
        Dcm_Channel[Offset]   = 0x42;
        SchM_Exit_Dcm(Dcm_Channel);
        TxOffset = Offset + 1u;
        for(iloop = 0; (E_OK == ret) && (iloop < PidNum) && (Sumframe == 0u); (iloop += 2u))
        {
            Pid = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[iloop + 1u];
            frame = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[iloop + 2u];
            Sumframe = frame;
            switch(Pid)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
                if (MixPid == NORMAL_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_CheckSupportedPIDs(Pid,
                            &SupportBuffer,SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA);
                    if (SupportBuffer != 0u)
                    {
                        SchM_Enter_Dcm(Dcm_Channel);
                        Dcm_Channel[TxOffset] = Pid;
                        TxOffset += 1u;
                        Dcm_Channel[TxOffset] = frame;
                        TxOffset += 1u;
                        Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]),&SupportBuffer);
                        SchM_Exit_Dcm(Dcm_Channel);
                        TxOffset += 4u;
                    }
                    else
                    {
                        noFindPidNum++;
                    }
                    MixPid = SUPPORT_REQUEST;
                }
                break;
            case 0x02u:
                if (MixPid == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    if (E_NOT_OK == Dem_DcmGetDTCOfOBDFreezeFrame(0u,&DTC,DEM_DTC_FORMAT_OBD))
                    {
                        DTC = 0x0000UL;
                    }
                    SchM_Enter_Dcm(Dcm_Channel);
                    Dcm_Channel[TxOffset] = Pid;
                    TxOffset += 1u;
                    Dcm_Channel[TxOffset] = frame;
                    TxOffset += 1u;
                    /*take every 2 byte of DTC,and fill in Dcm_Channel*/
                    Dcm_Channel[TxOffset] = (uint8)((DTC & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                    TxOffset += 1u;
                    Dcm_Channel[TxOffset] = (uint8)(DTC & OBD_DATA_LSB_MASK);
                    SchM_Exit_Dcm(Dcm_Channel);
                    TxOffset += 1u;
                    MixPid = NORMAL_REQUEST;
                }
                break;
            default:
                if (MixPid == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_ReadPid.SID = SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA;
                    OBD_ReadPid.pid = Pid;
                    OBD_ReadPid.UDSCall = FALSE;
                    ret = OBD_ReadPidValues(&OBD_ReadPid,&TxOffset,&noFindPidNum);
                    /* Request message is supported but data is currently not available,
                     * ignore the request message */
                    MixPid = NORMAL_REQUEST;
                }
                break;
            }
        }

        if (Sumframe != 0u)
        {
            /*the format of massage is not correct,ignore the request message */
            ret = E_NOT_OK;
        }

        /* can not find supported PIDs,ignose the requested message */
        if (noFindPidNum == (PidNum >> 1u))
        {
            ret = E_NOT_OK;
        }
    }

    if (E_OK == ret)
    {
        SchM_Enter_Dcm(Dcm_MsgCtrl);
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = (Dcm_MsgLenType)TxOffset - Offset;
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen  = (Dcm_MsgLenType)TxOffset - Offset;
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData  = &Dcm_Channel[Offset];
        SchM_Exit_Dcm(Dcm_MsgCtrl);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return  ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X02_ENABLED*/

