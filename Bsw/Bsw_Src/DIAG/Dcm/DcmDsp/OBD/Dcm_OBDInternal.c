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
**  FILENAME    : Dcm_OBDInternal.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD Internal API definitions                            **
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

/*************************************************************************/
/*
 * Brief               MemSet
 * ServiceId           --
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Val && Size
 * Param-Name[out]     none
 * Param-Name[in/out]  Dest
 * Return              none
 */
/*************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) OBD_MemSet(
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Dest,
    uint8 Val,
    uint32 Size)
{
    uint32 Cnt = Size;
    while (Cnt > 0u)
    {
        Dest[Cnt] = Val;
        Cnt--;
    }
    return;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Read PID values>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ObdMidCfgIndex && ptxPos && pnoFindPIDsNum>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE) OBD_ReadPidValues_0x01(
        uint8 pid,
	    uint8 ctrlIndex,
        P2VAR(uint16, AUTOMATIC, DCM_VAR) pBufOffset,
        boolean UDSCall
        )
{
    uint16 txOffset = *pBufOffset;
    uint16 Offset = 0;
    uint8 iloop;
    uint8 Buffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 txBuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    Std_ReturnType ret = E_NOT_OK;
    P2CONST(Dcm_DspPidDataType,AUTOMATIC,DCM_VAR) pDcmDspPidData;
    P2CONST(Dcm_DspPidSupportInfoType,AUTOMATIC,DCM_VAR) DcmDspPidSupportInfo;
    Dcm_NegativeResponseCodeType NRC;

    pDcmDspPidData = Dcm_DspCfg.pDcmDspPid[ctrlIndex].pDcmDspPidData;
    if (pDcmDspPidData != NULL_PTR)
    {

        for(iloop = 0; iloop < Dcm_DspCfg.pDcmDspPid[ctrlIndex].pDcmDspPidDataNum;
                iloop++)
        {
            if (pDcmDspPidData->DcmDspPidService01->DcmDspPidDataReadFnc != NULL_PTR)
            {
                ret = (pDcmDspPidData->DcmDspPidService01->DcmDspPidDataReadFnc)
                        (DCM_INVALID_UINT8,&Buffer[Offset],&NRC);
                if (E_OK == ret)
                {
                    if (pDcmDspPidData->pDcmDspPidDataSupportInfo != NULL_PTR)
                    {
                        DcmDspPidSupportInfo = pDcmDspPidData->
                           pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoRef;
                        OBD_BITS_SET(txBuffer,pDcmDspPidData->
                           pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoBit);
                        Dcm_MemoryCopy(&Buffer[Offset],
                           &txBuffer[DcmDspPidSupportInfo->DcmDspPidSupportInfoPos],
                           DcmDspPidSupportInfo->DcmDspPidSupportInfoLen);
                    }
                    else
                    {
                        Dcm_MemoryCopy(&Buffer[Offset],&txBuffer[Offset],
                                pDcmDspPidData->DcmDspPidDataSize);
                    }
                }
                else
                {
                    if (pDcmDspPidData->pDcmDspPidDataSupportInfo != NULL_PTR)
                    {
                        DcmDspPidSupportInfo = pDcmDspPidData->
                          pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoRef;
                        OBD_BITS_SET(txBuffer,pDcmDspPidData->
                          pDcmDspPidDataSupportInfo->DcmDspPidDataSupportInfoBit);
                        OBD_MemSet(
                           &txBuffer[DcmDspPidSupportInfo->DcmDspPidSupportInfoPos],
                           0x00,DcmDspPidSupportInfo->DcmDspPidSupportInfoLen);
                    }
                    else
                    {
                        OBD_MemSet(&txBuffer[Offset],0x00,
                                pDcmDspPidData->DcmDspPidDataSize);
                    }
                }
            }
            Offset += pDcmDspPidData->DcmDspPidDataSize;
            pDcmDspPidData++;
        }
    }
    if (Offset <= Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize)
    {
        SchM_Enter_Dcm(Dcm_Channel);
        if (TRUE == UDSCall)
        {
            Dcm_Channel[txOffset] = 0xFF;
            txOffset++;
        }
        Dcm_Channel[txOffset] = pid;
        txOffset++;
        Dcm_MemoryCopy(txBuffer,&Dcm_Channel[txOffset],
                Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize);
        SchM_Exit_Dcm(Dcm_Channel);
        txOffset += Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize;
        *pBufOffset = txOffset;
        ret = E_OK;
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/******************************************************************************/
/*
 * Brief               <Read PID values>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ObdMidCfgIndex && ptxPos && pnoFindPIDsNum>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE) OBD_ReadPidValues(
        OBD_ReadPidValuesTypes* OBD_ReadPid,
        P2VAR(uint16, AUTOMATIC, DCM_VAR) pBufOffset,
        P2VAR(uint8, AUTOMATIC, DCM_VAR) pNoFindPidNum
        )
{
    uint16 txOffset = *pBufOffset;
    uint16 Offset = 0;
    uint16 SumOffset = 0;
    uint16 BufferOffset = 0;
    uint8 Find = FALSE;
    uint8 iloop;
    uint8 ctrlIndex = 0;
    uint8 Buffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 txBuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint16 buffersize = 0;
    Std_ReturnType ret = E_NOT_OK;
    P2CONST(Dcm_DspPidDataType,AUTOMATIC,DCM_VAR) pDcmDspPidData;

    for (iloop = 0; (FALSE == Find) && (iloop < Dcm_DspCfg.DcmDspPidNum); iloop++)
    {
        if((OBD_ReadPid->pid == Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidIdentifier)
            && ((((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01)
                    || (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                    && (OBD_ReadPid->SID == SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA))
                || (((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_02)
                    || (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                    && (OBD_ReadPid->SID == SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA)))
            && (TRUE == Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidUsed)
            )
        {
            Find = TRUE;
            ctrlIndex = iloop;
        }
    }

    if(TRUE == Find)
    {
        OBD_MemSet(&txBuffer[Offset],0x00,Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize);
        switch(OBD_ReadPid->SID)
        {
        case SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA:
            ret = OBD_ReadPidValues_0x01(OBD_ReadPid->pid,ctrlIndex,pBufOffset,OBD_ReadPid->UDSCall);
            break;
        case SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA:
            pDcmDspPidData = Dcm_DspCfg.pDcmDspPid[ctrlIndex].pDcmDspPidData;
            if (pDcmDspPidData != NULL_PTR)
            {
                for (iloop = 0; iloop < Dcm_DspCfg.pDcmDspPid[iloop].pDcmDspPidDataNum; iloop++)
                {
                    Offset = BufferOffset;
                    if (E_OK == Dem_DcmReadDataOfOBDFreezeFrame(OBD_ReadPid->pid,
                            (uint8)((pDcmDspPidData->DcmDspPidDataPos + 7u) >> 3u),
                            &Buffer[Offset],&buffersize))
                    {
                        SumOffset += buffersize;
                    }
                    Dcm_MemoryCopy(&Buffer[BufferOffset],&txBuffer[BufferOffset],
                            pDcmDspPidData->DcmDspPidDataSize);
                    BufferOffset += pDcmDspPidData->DcmDspPidDataSize;
                    pDcmDspPidData++;
                }
            }
            if ((BufferOffset <= Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize) &&
                    (SumOffset != 0u))
            {
                SchM_Enter_Dcm(Dcm_Channel);
                if (TRUE == OBD_ReadPid->UDSCall)
                {
                    Dcm_Channel[txOffset] = 0xFF;
                    txOffset++;
                }
                Dcm_Channel[txOffset] = OBD_ReadPid->pid;
                txOffset++;
                Dcm_Channel[txOffset] = 0x00;
                txOffset++;
                Dcm_MemoryCopy(txBuffer,&Dcm_Channel[txOffset],
                        Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize);
                SchM_Exit_Dcm(Dcm_Channel);
                txOffset += Dcm_DspCfg.pDcmDspPid[ctrlIndex].DcmDspPidSize;
                *pBufOffset = txOffset;
                ret = E_OK;
            }
            break;
        default:
            /*idle*/
            break;
        }
    }
    else
    {
        (*pNoFindPidNum)++;
        ret = E_OK; /* can not find pid,return E_OK */
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/******************************************************************************/
/*
 * Brief               <Check supported Parameter id>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) OBD_CheckSupportedPIDs(
        uint8 pid,
        P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf,
        uint8 SID)
{
    uint8 iloop;
    boolean Find = FALSE;

    /* check parameter */
    if(NULL_PTR != pDataBuf)
    {
        *pDataBuf = 0;
#if(STD_ON == DCM_DSP_PID_FUNC_ENABLED)
        for(iloop = 0; iloop < Dcm_DspCfg.DcmDspPidNum; iloop++)
        {
            if(((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidIdentifier >= (pid +0x01u))
               && (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidIdentifier <= (pid +0x20u)))
                && ((((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01)
                        || (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                        && (SID == SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA))
                    || (((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_02)
                        || (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                        && (SID == SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA)))
                && (TRUE == Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidUsed))

            {
                *pDataBuf |= (uint32)1u <<
                        (0x20u - (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidIdentifier - pid));
            }

            if(((pid == Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidIdentifier) || (pid == 0x0u))
                && ((((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01)
                        || (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                        && (SID == SID_REQUEST_CURRENT_POWERTRAIN_DIAGNOSTIC_DATA))
                    || (((Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_02)
                        || (Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidService == DCM_SERVICE_01_02))
                        && (SID == SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA)))
                && (TRUE == Dcm_DspCfg.pDcmDspPid[iloop].DcmDspPidUsed)
                )
            {
                Find = TRUE;
            }
        }

        if (Find == FALSE)
        {
            *pDataBuf = 0;
        }
#endif
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

/******************************************************************************/
/*
 * Brief               <Check supported test id>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <none>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#if (DCM_DSP_REQUESTCONTROL_FUNC_ENABLED == STD_ON)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) OBD_CheckSupportedTIDs(uint8 Tid, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf)
{
    uint8 iloop;
    boolean Find = FALSE;

    /* check parameter */
    if(NULL_PTR != pDataBuf)
    {
        *pDataBuf = 0;
        for (iloop = 0; iloop < Dcm_DspCfg.DcmDspRequestControlNum; iloop++)
        {
            if((Dcm_DspCfg.pDcmDspRequestControl[iloop].DcmDspRequestControlTestId >= (Tid +0x01u))
           && (Dcm_DspCfg.pDcmDspRequestControl[iloop].DcmDspRequestControlTestId <= (Tid +0x20u)))
            {
               *pDataBuf |= (uint32)1u << (0x20u -
                   (Dcm_DspCfg.pDcmDspRequestControl[iloop].DcmDspRequestControlTestId - Tid));
            }

            if((Dcm_DspCfg.pDcmDspRequestControl[iloop].DcmDspRequestControlTestId == Tid) || (Tid == 0x0u))
            {
                Find = TRUE;
            }
        }

        if (Find == FALSE)
        {
            *pDataBuf = 0;
        }
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/******************************************************************************/
/*
 * Brief               <find config index number of TID>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pCfgIndex>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE) OBD_FindCfgIndexOfTestID(
        uint8 testID, P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex)
{
    uint8 iloop;
    Std_ReturnType ret = E_NOT_OK;

    /* check parameter */
    if(NULL_PTR != pCfgIndex)
    {
        for (iloop = 0; (iloop < Dcm_DspCfg.DcmDspRequestControlNum) && (ret == E_NOT_OK); iloop++)
        {
            if(testID == Dcm_DspCfg.pDcmDspRequestControl[iloop].DcmDspRequestControlTestId)
            {
                (*pCfgIndex) = iloop;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

#if (DCM_DSP_VEHINFO_FUNC_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Brief               <Set Availability InfoType Value>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <InfoType && pDataBuf>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) OBD_SetAvailabilityInfoTypeValue(
        uint8 InfoType, P2VAR(uint32, AUTOMATIC, DCM_VAR) pDataBuf)
{
    uint8 iloop;
    boolean Find = FALSE;

    /* check parameter */
    if(NULL_PTR != pDataBuf)
    {
        *pDataBuf = 0;
        for (iloop = 0; iloop < Dcm_DspCfg.DcmDspVehInfoNum; iloop++)
        {
            if((Dcm_DspCfg.pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType >= (InfoType +0x01u))
               && (Dcm_DspCfg.pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType <= (InfoType +0x20u)))
            {
               *pDataBuf |= (uint32)1u << (0x20u -
                       (Dcm_DspCfg.pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType - InfoType));
            }

            if((Dcm_DspCfg.pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType == InfoType) || (InfoType == 0x0u))
            {
                Find = TRUE;
            }
         }

        if (Find == FALSE)
        {
            *pDataBuf = 0;
        }
    }
    return ;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/******************************************************************************/
/*
 * Brief               <find config index number of VehInfo>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <testID && pCfgIndex>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE) OBD_FindCfgIndexOfVehInfo(
        uint8 vehInfo, P2VAR(uint8, AUTOMATIC, DCM_VAR) pCfgIndex)
{
    uint8 iloop;
    Std_ReturnType ret = E_NOT_OK;

    /* check parameter */
    if(NULL_PTR != pCfgIndex)
    {
        for(iloop = 0; (iloop < Dcm_DspCfg.DcmDspVehInfoNum) && (E_NOT_OK == ret); iloop++)
        {
            if(vehInfo == Dcm_DspCfg.pDcmDspVehInfo[iloop].DcmDspVehInfoInfoType)
            {
                (*pCfgIndex) = iloop;
                ret = E_OK;
            }
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
