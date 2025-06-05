/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2020)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#include "UDS.h"


/****************************************************************
     UDS: TransferData (36 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X36_ENABLED)
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x36SubCheck(
        uint8 ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode)
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

#if(STD_ON == DCM_SESSION_FUNC_ENABLED)
    /*session check,check whether the current session supports the request service*/
    ret = DsdInternal_SesCheck(ProtocolCtrlId,SID_TRANSFER_DATA);
    if(E_NOT_OK == ret)
    {
        /*the current session does not support the request service,send NRC = 0x7F*/
        *ErrorCode = DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION;
    }
#endif
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
    /*security check,check whether the current security supports the request service*/
    if(E_OK == ret)
    {
        ret = DsdInternal_SecurityCheck(ProtocolCtrlId,SID_TRANSFER_DATA);
        if(E_NOT_OK == ret)
        {
            /*the current security does not support the request service,send NRC = 0x33*/
            *ErrorCode = DCM_E_SECURITYACCESSDENIED;
        }
    }
#endif
    if(E_OK == ret)
    {
        /*min-length check*/
        if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status ==
                DCM_UDS0X38_0x04) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x05))
        {
            if (DCM_UDS0X36_REQ_DATA_UPLOAD_MINLENGTH >
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the min length of message is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
        else if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) || (Dcm_DspProgram.Status
                == DCM_UDS0X38_0x01) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
        {
            if (DCM_UDS0X36_REQ_DATA_DOWNLOAD_MINLENGTH >
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the min length of message is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
        else
        {
            if (DCM_UDS0X36_REQ_DATA_UPLOAD_MINLENGTH >
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
            {
                /*the min length of message is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
            else
            {
                /*the request message is not sequence,send NRC 0x24*/
                *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/***************************/
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE)Dcm_Uds0x36ServiceConditionCheck(
        uint8 ProtocolCtrlId,
        P2VAR(uint8,AUTOMATIC,DCM_VAR) MemoryIdentifier,
        P2VAR(uint8,AUTOMATIC,DCM_VAR) tempblockId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode)
{
    Std_ReturnType ret;

    ret = Dcm_Uds0x36SubCheck(ProtocolCtrlId,ErrorCode);
    if ((E_OK == ret) && ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD)
              || (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD)))
    {
        if (Dcm_DspProgram.MemoryIdInfoIndex != 0xFFu)
        {
            *MemoryIdentifier = Dcm_DspCfg.pDcmDspMemory->
              DcmDspMemoryIdInfo[Dcm_DspProgram.MemoryIdInfoIndex].DcmDspMemoryIdValue;
        }
        else
        {
            /*the MemoryIdInfoIndex is not correct,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        /* then calculate block index and check block index*/
        if ((uint8)0xFFU == *tempblockId)
        {
            *tempblockId = (uint8)0;
        }
        else
        {
            (*tempblockId)++;
        }
        if ((Dcm_DspProgram.Status == DCM_UDS0X36_INIT) || (Dcm_DspProgram.Status > DCM_UDS0X38_0x05))
        {
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            ret = E_NOT_OK;
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*******************************/
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE)Dcm_UDS0x36_TxCheck(
        uint8  ProtocolCtrlId,
        uint8 tempblockId,
        uint32 MemorySize,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode)
{
    uint8 MsgCtrlId;
    Std_ReturnType ret = E_OK;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;

    /* check tx data length */
    if((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD)
        && (MemorySize > Dcm_DslCfg.pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    else
    {
        if (2u > Dcm_DslCfg.pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize)
        {
            /*Pdu length is bigger than buffer size */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) ||
                (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD))
        {
            /* calculate address */
            SchM_Enter_Dcm(Dcm_DspProgram);
            Dcm_DspProgram.address += Dcm_DspProgram.BlockLength;
            SchM_Exit_Dcm(Dcm_DspProgram);
        }
        /*the processing is successful,assemble positive response*/
        SchM_Enter_Dcm(Dcm_Channel);
        Dcm_Channel[Offset] = 0x76;             /*response SID*/
        Dcm_Channel[Offset + 1u] = tempblockId;
        /*blockSequenceCounter*/
        SchM_Exit_Dcm(Dcm_Channel);

        SchM_Enter_Dcm(Dcm_DspProgram);
        Dcm_DspProgram.blockId = tempblockId;
        SchM_Exit_Dcm(Dcm_DspProgram);

        if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status
              == DCM_UDS0X38_0x04) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x05))
        {
            SchM_Enter_Dcm(Dcm_MsgCtrl);
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MemorySize;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen = MemorySize;
            SchM_Exit_Dcm(Dcm_MsgCtrl);
        }
        else if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) || (Dcm_DspProgram.Status
                == DCM_UDS0X38_0x01) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
        {
            SchM_Enter_Dcm(Dcm_MsgCtrl);
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 2u;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = 2u;
            SchM_Exit_Dcm(Dcm_MsgCtrl);
        }
        else
        {
            /*idle*/
        }
        SchM_Enter_Dcm(Dcm_MsgCtrl);
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData = &Dcm_Channel[Offset];
        SchM_Exit_Dcm(Dcm_MsgCtrl);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/*******************************/
FUNC(Std_ReturnType, DCM_CODE)Dcm_UDS0x36(
        Dcm_OpStatusType OpStatus,
        uint8  ProtocolCtrlId,
        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR) ErrorCode)
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8  TxChannelCtrlIndex;
    uint8  TxChannelCfgIndex;
    uint16 Offset;
    uint8 MemoryIdentifier = 0;
    uint32 MemorySize;
    uint8 tempblockId = Dcm_DspProgram.blockId;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId =  Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    ret = Dcm_Uds0x36ServiceConditionCheck(ProtocolCtrlId,&MemoryIdentifier,&tempblockId,ErrorCode);

    if (E_OK == ret)
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
        Dcm_DspProgram.reqBlockId = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];

        /*process is running and the request format is correct,to call the callout Following NRCs
         * shall be the responsibility of the callout function 0x23,0x31,0x71,0x72,0x92,0x93*/
        if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status ==
              DCM_UDS0X38_0x04) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x05))
        {
            MemorySize = Dcm_DspProgram.BlockLength;
            ret = Dcm_ReadMemory(OpStatus,MemoryIdentifier,Dcm_DspProgram.address,
                      &MemorySize,(&Dcm_Channel[Offset + 2u]),ErrorCode);
            if (DCM_READ_FAILED == ret)
            {
                ret = E_NOT_OK;
            }
#if(STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
            else if(DCM_READ_FORCE_RCRRP == ret)
            {
                /*the processing is pending,send NRC 0x78*/
                DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
                DsdInternal_ProcessingDone(ProtocolCtrlId);
                SchM_Enter_Dcm(Dcm_MsgCtrl);
                Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
                SchM_Exit_Dcm(Dcm_MsgCtrl);
            }
#endif
            else if (DCM_READ_PENDING == ret)
            {
                ret = DCM_E_PENDING;
                SchM_Enter_Dcm(Dcm_MsgCtrl);
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                SchM_Exit_Dcm(Dcm_MsgCtrl);
            }
            else
            {
                /*idle*/
            }
        }
        else if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) || (Dcm_DspProgram.Status
                == DCM_UDS0X38_0x01) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
        {
            MemorySize = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen;
            ret = Dcm_WriteMemory(OpStatus,MemoryIdentifier,Dcm_DspProgram.address,MemorySize-2,
                    &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2],ErrorCode);
            if (DCM_WRITE_FAILED == ret)
            {
                ret =  E_NOT_OK;
            }
            #if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
            else if(DCM_WRITE_FORCE_RCRRP == ret)
            {
                /*the processing is pending,send NRC 0x78*/
                DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
                DsdInternal_ProcessingDone(ProtocolCtrlId);
                SchM_Enter_Dcm(Dcm_MsgCtrl);
                Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
                SchM_Exit_Dcm(Dcm_MsgCtrl);
            }
            #endif /* (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN) */
            else if (DCM_WRITE_PENDING == ret)
            {
                ret = DCM_E_PENDING;
                SchM_Enter_Dcm(Dcm_MsgCtrl);
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                SchM_Exit_Dcm(Dcm_MsgCtrl);
            }
            else
            {
                /*idle*/
            }
        }
        else
        {
            /*idle*/
        }
    }

    if (E_OK == ret)
    {
    	if ((0 == tempblockId) &&
    		(0xFF == Dcm_DspProgram.reqBlockId))
    	{
    		tempblockId = 0xFF;
    	}
    	else if (((tempblockId - 1u) == Dcm_DspProgram.reqBlockId) && (
                Dcm_DspProgram.FirstFlag == FALSE))
        {
            /* receive the same dataBlockId again */
            tempblockId--;
        }
        else
        {
            /*idle*/
        }
        if (TRUE == Dcm_DspProgram.FirstFlag)
        {   
            Dcm_DspProgram.FirstFlag = FALSE;
        }

        ret = Dcm_UDS0x36_TxCheck(ProtocolCtrlId,tempblockId,MemorySize,ErrorCode);
    }

    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
