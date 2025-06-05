/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_AR_HeadLampSWC.c>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_AR_HeadLampSWC.c */
/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
//#include "Os.h"
#include "Rte_AR_HeadLampSWC.h"
#include "Rte_Data.h"
#include "Com.h"

/*******************************************************************************
 **                        Private Function Declarations                      **
******************************************************************************/

static void Rte_PRE_Runnable_Step(Rte_Instance s);


static void Rte_POST_Runnable_Step(Rte_Instance s);


/*******************************************************************************
*Function-Name        Runnable_Step
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

void Rte_Runnable_Step(Rte_Instance_AR_HeadLampSWC s)
{
	Rte_PRE_Runnable_Step(s);
	Runnable_Step(s);
	Rte_POST_Runnable_Step(s);
}

/*******************************************************************************
*Function-Name        Runnable_Init
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

void Rte_Runnable_Init(Rte_Instance_AR_HeadLampSWC s)
{
	Runnable_Init(s);
}

/*******************************************************************************
 **                        Global Function                                    **
******************************************************************************/

#define RTE_START_SEC_CODE
#include "RTE_MemMap.h"


static void Rte_PRE_Runnable_Step(Rte_Instance s)
	{
		//GetResource(RES_SCHEDULER);
		void Rte_COMCbk_HighBeam_IPDU_COM_LedStatus(void);
		(s)->Runnable_Step_HMIHighBeam_HMIHighBeam->value = Runnable_Step_HMIHighBeam_IN_HMIHighBeam_HMIHighBeam_read_buffer;
		void Rte_COMCbk_lowBeam_IPDU_COM_LedStatus(void);
		(s)->Runnable_Step_HMILowBeam_HMILowBeam->value = Runnable_Step_HMILowBeam_IN_HMILowBeam_HMILowBeam_read_buffer;
		void Rte_COMCbk_PositionLamp_IPDU_COM_LedStatus(void);
		(s)->Runnable_Step_HMIPositionLamp_HMIPositionLamp->value = Runnable_Step_HMIPositionLamp_IN_HMIPositionLamp_HMIPositionLamp_read_buffer;
		
		
		
		//ReleaseResource(RES_SCHEDULER);
		
		
	}

static void Rte_POST_Runnable_Step(Rte_Instance s)
	{
		//GetResource(RES_SCHEDULER);
		uint8 CmdHighBeam_buf_CmdHighBeam_IPDU_COM_CmdSend = (uint8)(s)->Runnable_Step_HMIHighBeam_HMIHighBeam->value;

		/** enter critical section */
		//SuspendAllInterrupts();
		//Com_SendSignal(CmdHighBeam_IPDU_COM_CmdSend, &CmdHighBeam_buf_CmdHighBeam_IPDU_COM_CmdSend);
		/** leave critical section */
		//ResumeAllInterrupts();
		
		uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)(s)->Runnable_Step_HMILowBeam_HMILowBeam->value;

		/** enter critical section */
		//SuspendAllInterrupts();
		//Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
		/** leave critical section */
		//ResumeAllInterrupts();
		
		
		
		
		//ReleaseResource(RES_SCHEDULER);
		
		
	}

#define RTE_STOP_SEC_CODE
#include "RTE_MemMap.h"



