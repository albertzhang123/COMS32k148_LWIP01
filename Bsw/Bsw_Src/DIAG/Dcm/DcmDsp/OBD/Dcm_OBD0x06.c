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
**  FILENAME    : Dcm_OBD0x06.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x06 API definitions                                 **
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
 * Brief               <OBD service (0x06): Request On-Board Monitoring Test-results
 *                                           for Specific Monitored Systems>
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
#if(STD_ON == DCM_OBD_SERVICE0X06_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)Dcm_OBD0x06(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode
        )
{
    uint8  MsgCtrlId;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;
    uint8 MidNum;
    uint8 iloop;
    uint8 Mid;
    uint32 SupportBuffer;
    uint16 TxOffset;
    uint8 noFindMidNum = 0;
#ifdef DEM_OBD_SUPPORT
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    uint8 index;
    uint8 TidNum;
    uint8 TIDvalue;
    uint8 UaSID;
    uint16 Testvalue;
    uint16 Lowlimvalue;
    uint16 Upplimvalue;
#endif
#endif
    Std_ReturnType ret = E_OK;

	DCM_UNUSED(OpStatus);
	DCM_UNUSED(ErrorCode);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
    if ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X06_REQ_DATA_LENGTH_MIN)
        || (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen > DCM_UDS0X06_REQ_DATA_LENGTH_MAX))
    {
        /* requested message format is wrong,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        MidNum = (uint8)Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 1u;
        SchM_Enter_Dcm(Dcm_Channel);
        Dcm_Channel[Offset]   = 0x46;
        SchM_Exit_Dcm(Dcm_Channel);
        TxOffset = Offset + 1u;
        for (iloop = 0; iloop < MidNum; iloop++)
        {
            Mid = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[iloop + 1u];
            switch(Mid)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
#ifdef DEM_OBD_SUPPORT
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
                (void)Dem_DcmGetAvailableOBDMIDs(Mid,&SupportBuffer);
#endif
#endif
                if (SupportBuffer != 0u)
                {
                    SchM_Enter_Dcm(Dcm_Channel);
                    Dcm_Channel[TxOffset] = Mid;
                    TxOffset += 1u;
                    Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]),&SupportBuffer);
                    SchM_Exit_Dcm(Dcm_Channel);
                    TxOffset += 4u;
                }
                else
                {
                    noFindMidNum++;
                }
                break;
            default:
                if (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen !=
                        DCM_UDS0X06_REQ_DATA_LENGTH_MIN)
                {
                    /* requested message format is wrong,ignore the request message */
                    ret = E_NOT_OK;
                }
                else
                {
#ifdef DEM_OBD_SUPPORT
#if(DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
                    (void)Dem_DcmGetNumTIDsOfOBDMID(Mid,&TidNum);
                    if (TidNum != 0u)
                    {
                        for (index = 0; index < TidNum;index++)
                        {
                            (void)Dem_DcmGetDTRData(Mid,index,&TIDvalue,&UaSID,
                                    &Testvalue,&Lowlimvalue,&Upplimvalue);
                            SchM_Enter_Dcm(Dcm_Channel);
                            Dcm_Channel[TxOffset] = Mid;
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = TIDvalue;
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = UaSID;
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)((Testvalue &
                                    (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)(Testvalue & OBD_DATA_LSB_MASK);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)((Lowlimvalue &
                                    (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)(Lowlimvalue & OBD_DATA_LSB_MASK);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)((Upplimvalue &
                                    (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)(Upplimvalue & OBD_DATA_LSB_MASK);
                            SchM_Exit_Dcm(Dcm_Channel);
                            TxOffset += 1u;
                        }
                    }
                    else
#endif
#endif
                    {
                        /* there is no supported PID,ignore the request message */
                        ret = E_NOT_OK;
                    }
                }
                break;
            }
        }
        /* can not find supported PIDs,ignose the requested message */
        if (noFindMidNum == MidNum)
        {
            /* there is no supported PID,ignore the request message */
            ret = E_NOT_OK;
        }
    }
    /* check tx data length */
    if ((E_OK == ret) && ((TxOffset - Offset) >
    Dcm_DslCfg.pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size,ignore the request message */
        ret = E_NOT_OK;
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
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X06_ENABLED*/
