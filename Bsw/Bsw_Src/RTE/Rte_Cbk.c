/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Cbk.c>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_Cbk.c */
/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
#include "Rte_Cbk.h"

/*******************************************************************************
 **                        Global Function                                    **
******************************************************************************/

/*******************************************************************************
*Function-Name        Rte_COMCbk_lowBeam_IPDU_COM_LedStatus
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbk_lowBeam_IPDU_COM_LedStatus(void)
{
	boolean lowBeam_IPDU_COM_LedStatus_buf = 0;
	UInt8 data;
	boolean isSuccess = TRUE;
	Std_ReturnType comStatus;
	//comStatus = Com_ReceiveSignal(lowBeam_IPDU_COM_LedStatus, &lowBeam_IPDU_COM_LedStatus_buf);
	if (comStatus != E_OK) 
	{
		isSuccess = FALSE;
	}
	
	if (isSuccess == TRUE) 
	{
		data = (UInt8)lowBeam_IPDU_COM_LedStatus_buf;
		Runnable_Step_HMILowBeam_IN_HMILowBeam_HMILowBeam_read_buffer = data;
	}
	
}

/*******************************************************************************
*Function-Name        Rte_COMCbkTAck_CmdLowBeam_IPDU_COM_CmdSend
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbkTAck_CmdLowBeam_IPDU_COM_CmdSend(void)
{
	if (!(Rte_Status_CmdLowBeam_IPDU_COM_CmdSend & RTE_FLAG_TRANSMIT_ACK)) 
	{
		Rte_Status_CmdLowBeam_IPDU_COM_CmdSend |= RTE_FLAG_TRANSMIT_ACK;
	}
}

/*******************************************************************************
*Function-Name        Rte_COMCbkTErr_CmdLowBeam_IPDU_COM_CmdSend
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbkTErr_CmdLowBeam_IPDU_COM_CmdSend(void)
{
	if (!(Rte_Status_CmdLowBeam_IPDU_COM_CmdSend & RTE_FLAG_TRANSMIT_ACK)) 
	{
		Rte_Status_CmdLowBeam_IPDU_COM_CmdSend |= RTE_FLAG_TRANSMIT_ACK;
		Rte_Status_CmdLowBeam_IPDU_COM_CmdSend |= RTE_FLAG_COM_STOPPED;
	}
}

/*******************************************************************************
*Function-Name        Rte_COMCbk_PositionLamp_IPDU_COM_LedStatus
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbk_PositionLamp_IPDU_COM_LedStatus(void)
{
	boolean PositionLamp_IPDU_COM_LedStatus_buf = 0;
	UInt8 data;
	boolean isSuccess = TRUE;
	Std_ReturnType comStatus;

	//comStatus = Com_ReceiveSignal(PositionLamp_IPDU_COM_LedStatus, &PositionLamp_IPDU_COM_LedStatus_buf);
	if (comStatus != E_OK) 
	{
		isSuccess = FALSE;
	}
	
	if (isSuccess == TRUE) 
	{
		data = (UInt8)PositionLamp_IPDU_COM_LedStatus_buf;
		Runnable_Step_HMIPositionLamp_IN_HMIPositionLamp_HMIPositionLamp_read_buffer = data;
	}
	

}

/*******************************************************************************
*Function-Name        Rte_COMCbk_HighBeam_IPDU_COM_LedStatus
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbk_HighBeam_IPDU_COM_LedStatus(void)
{
	boolean HighBeam_IPDU_COM_LedStatus_buf = 0;
	UInt8 data;
	boolean isSuccess = TRUE;
	Std_ReturnType comStatus;
	//comStatus = Com_ReceiveSignal(HighBeam_IPDU_COM_LedStatus, &HighBeam_IPDU_COM_LedStatus_buf);
	if (comStatus != E_OK) 
	{
		isSuccess = FALSE;
	}
	
	if (isSuccess == TRUE) 
	{
		data = (UInt8)HighBeam_IPDU_COM_LedStatus_buf;
		Runnable_Step_HMIHighBeam_IN_HMIHighBeam_HMIHighBeam_read_buffer = data;
	}
	//Dem_SetEventStatus(DemEventParameter_0xC00001, DEM_EVENT_STATUS_FAILED);
}

/*******************************************************************************
*Function-Name        Rte_COMCbkTAck_CmdHighBeam_IPDU_COM_CmdSend
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbkTAck_CmdHighBeam_IPDU_COM_CmdSend(void)
{
	if (!(Rte_Status_CmdHighBeam_IPDU_COM_CmdSend & RTE_FLAG_TRANSMIT_ACK)) 
	{
		Rte_Status_CmdHighBeam_IPDU_COM_CmdSend |= RTE_FLAG_TRANSMIT_ACK;
	}
}

/*******************************************************************************
*Function-Name        Rte_COMCbkTErr_CmdHighBeam_IPDU_COM_CmdSend
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               void
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

void Rte_COMCbkTErr_CmdHighBeam_IPDU_COM_CmdSend(void)
{
	if (!(Rte_Status_CmdHighBeam_IPDU_COM_CmdSend & RTE_FLAG_TRANSMIT_ACK)) 
	{
		Rte_Status_CmdHighBeam_IPDU_COM_CmdSend |= RTE_FLAG_TRANSMIT_ACK;
		Rte_Status_CmdHighBeam_IPDU_COM_CmdSend |= RTE_FLAG_COM_STOPPED;
	}
}


