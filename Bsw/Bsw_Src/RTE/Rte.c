/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte.c>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte.c */
/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

#define RTE_CORE

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
//#include "Os.h"
#include "Rte_Type.h"
#include "Rte_Data.h"
#include "Rte_Main.h"
#include "Rte_AR_HeadLampSWC.h"
#include "Rte_IOAbstractionSWC.h"
#include "Com.h"

/*******************************************************************************
 **                        Global Variable Definitions                        **
******************************************************************************/

/*******************************************************************************
 **                        Global Function Definitions                        **
******************************************************************************/

Rte_DE_UInt8 Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_HMIHighBeam_HMIHighBeam_ibuffer = {.value = 0};
Rte_DE_UInt8 Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_HMILowBeam_HMILowBeam_ibuffer = {.value = 0};
Rte_DE_UInt8 Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_HMIPositionLamp_HMIPositionLamp_ibuffer = {.value = 0};
Rte_DE_UInt8 Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_CMDHighBeam_CMDHighBeam_ibuffer = {.value = 0};
Rte_DE_UInt8 Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_CMDLowBeam_CMDLowBeam_ibuffer = {.value = 0};
CONST(Rte_CDS_AR_HeadLampSWC, RTE_CONST) Rte_Inst_CPT_AR_HeadLampSWC = 
{
	.Runnable_Step_HMIHighBeam_HMIHighBeam = &Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_HMIHighBeam_HMIHighBeam_ibuffer,
	.Runnable_Step_HMILowBeam_HMILowBeam = &Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_HMILowBeam_HMILowBeam_ibuffer,
	.Runnable_Step_HMIPositionLamp_HMIPositionLamp = &Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_HMIPositionLamp_HMIPositionLamp_ibuffer,
	.Runnable_Step_CMDHighBeam_CMDHighBeam = &Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_CMDHighBeam_CMDHighBeam_ibuffer,
	.Runnable_Step_CMDLowBeam_CMDLowBeam = &Rte_Buf_CPT_AR_HeadLampSWC_Runnable_Step_CMDLowBeam_CMDLowBeam_ibuffer,
};





#define RTE_START_SEC_CODE
#include "Rte_MemMap.h"


/*******************************************************************************
*Function-Name        Rte_Start
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               Std_ReturnType
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

Std_ReturnType Rte_Start(void)
{
	//(void)SetRelAlarm(OsAlarm_10ms, 10, OS_TICKS2MS_SystemTimer_Core0(10));
	
	
	return RTE_E_OK;
}

/*******************************************************************************
*Function-Name        Rte_Stop
*Service ID           <None>
*Sync/Async           <Synchronous>
*Reentrancy           <Non Reentrant>
*param-Name[in]       <None>
*Param-Name[out]      <None>
*Param-Name[in/out]   <None>
*return               Std_ReturnType
*PreCondition         <None>
*CallByAPI            <None>
******************************************************************************/

Std_ReturnType Rte_Stop(void)
{
	return RTE_E_OK;
}

void Os_TaskEntry_Rte_OsTask_10ms(void)
{
	Rte_Runnable_Step(&Rte_Inst_CPT_AR_HeadLampSWC);
}



void Os_TaskEntry_Rte_OsTask_init(void)
{
	Rte_Runnable_Init(&Rte_Inst_CPT_AR_HeadLampSWC);
}



#define RTE_STOP_SEC_CODE
#include "Rte_MemMap.h"



