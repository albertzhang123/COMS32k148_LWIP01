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
**  FILENAME    : Dcm_OBD0x08.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x08 API definitions                                 **
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
 * Brief               <OBD service (0x08): Request Control of On-Board System, Test or Component>
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
#if(STD_ON == DCM_OBD_SERVICE0X08_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)Dcm_OBD0x08(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode)
{
    uint8  MsgCtrlId;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;
    uint8 iloop;
    uint8 TidNum;
    uint8 Tid;
    uint8 MixTid = 0;
    uint16 TxOffset;
    uint32 SupportBuffer;
    uint8 noFindTidNum = 0;
    uint8 CtrlIndex;
    uint8 inputbuffer[7] = {0};
    uint8 outputbuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 counter = 0;
    Std_ReturnType ret = E_OK;
    boolean ProcessOn = TRUE;

	DCM_UNUSED(OpStatus);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
    /*check the massage length*/
    if ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X08_REQ_DATA_LENGTH_MIN)
       || (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen > DCM_UDS0X08_REQ_DATA_LENGTH_MAX))
    {
        /* requested message format is wrong,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        TidNum = (uint8)Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 1u;
        SchM_Enter_Dcm(Dcm_Channel);
        Dcm_Channel[Offset] = 0x48;
        SchM_Exit_Dcm(Dcm_Channel);
        TxOffset = Offset + 1u;
        for (iloop = 0; (iloop < TidNum) && (E_OK == ret) && (ProcessOn == TRUE); iloop++)
        {
            Tid = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[counter + 1u];
            switch(Tid)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
                if (MixTid == NORMAL_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_CheckSupportedTIDs(Tid, &SupportBuffer);
                    if (SupportBuffer != 0u)
                    {
                        SchM_Enter_Dcm(Dcm_Channel);
                        Dcm_Channel[TxOffset] = Tid;
                        TxOffset += 1u;
                        Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]),&SupportBuffer);
                        SchM_Exit_Dcm(Dcm_Channel);
                        TxOffset += 4u;
                    }
                    else
                    {
                        noFindTidNum++;
                    }
                    MixTid = SUPPORT_REQUEST;
                    counter++;
                }
                break;
            default:
                if (MixTid == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    if(E_OK == OBD_FindCfgIndexOfTestID(Tid,&CtrlIndex))
                    {
                        Dcm_MemoryCopy(&(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1u]),
                                inputbuffer,Dcm_DspCfg.pDcmDspRequestControl[CtrlIndex].
                                DcmDspRequestControlInBufferSize);
                        if (Dcm_DspCfg.pDcmDspRequestControl[CtrlIndex].
                                DcmDspRequestControlInBufferSize == (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 2u))
                        {
							if (E_OK == Rte_RequestControl(Tid,inputbuffer,outputbuffer))
							{
								SchM_Enter_Dcm(Dcm_Channel);
								Dcm_Channel[TxOffset] = Tid;
								TxOffset += 1u;
								Dcm_MemoryCopy(outputbuffer,&Dcm_Channel[TxOffset], Dcm_DspCfg.
								   pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlOutBufferSize);
								SchM_Exit_Dcm(Dcm_Channel);
								TxOffset += Dcm_DspCfg.pDcmDspRequestControl[CtrlIndex].
										DcmDspRequestControlOutBufferSize;
								ProcessOn = FALSE;
							}
							else
							{
								/* the conditions of the system are not proper to run,send NRC 0x22*/
								*ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
								ret = E_NOT_OK;
							}
                        }
                        else
                        {
                            /* there is only one TID can be done,ignore the request message with more TIDs*/
                            ret = E_NOT_OK;
                        }
                    }
                    else
                    {
                        /* there is no supported VehInfotype,ignore the request message */
                        ret = E_NOT_OK;
                    }
                    MixTid = NORMAL_REQUEST;
                }
                break;
            }
        }
        if( noFindTidNum == TidNum )
        {
            /* ignore the request message */
            ret = E_NOT_OK;
        }
    }

    /* check tx data length */
    if((E_OK == ret) && ((TxOffset - Offset) >
    (Dcm_DslCfg.pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize)))
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
#endif /*STD_ON == DCM_OBD_SERVICE0X08_ENABLED*/

