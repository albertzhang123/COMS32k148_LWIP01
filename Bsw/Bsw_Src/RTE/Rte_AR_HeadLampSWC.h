/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_AR_HeadLampSWC.h>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_AR_HeadLampSWC.h */

#ifndef RTE_AR_HEADLAMPSWC_H
#define RTE_AR_HEADLAMPSWC_H

/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
#include "Rte_DataHandleType.h"
#include "Rte_AR_HeadLampSWC_Type.h"

/*******************************************************************************
 **                        Macro Definitions                                  **
******************************************************************************/

typedef struct 
{
	P2VAR(Rte_DE_UInt8, AUTOMATIC, RTE_DATA)  Runnable_Step_HMIHighBeam_HMIHighBeam;
	P2VAR(Rte_DE_UInt8, AUTOMATIC, RTE_DATA)  Runnable_Step_HMILowBeam_HMILowBeam;
	P2VAR(Rte_DE_UInt8, AUTOMATIC, RTE_DATA)  Runnable_Step_HMIPositionLamp_HMIPositionLamp;
	P2VAR(Rte_DE_UInt8, AUTOMATIC, RTE_DATA)  Runnable_Step_CMDHighBeam_CMDHighBeam;
	P2VAR(Rte_DE_UInt8, AUTOMATIC, RTE_DATA)  Runnable_Step_CMDLowBeam_CMDLowBeam;
} Rte_CDS_AR_HeadLampSWC;

#ifndef RTE_CORE
typedef CONSTP2CONST(Rte_CDS_AR_HeadLampSWC, AUTOMATIC, RTE_CONST) Rte_Instance;
#endif

typedef CONSTP2CONST(Rte_CDS_AR_HeadLampSWC, AUTOMATIC, RTE_CONST) Rte_Instance_AR_HeadLampSWC;




/** API mappings */

#define Rte_IRead_Runnable_Step_HMIHighBeam_HMIHighBeam(s) ((s)->Runnable_Step_HMIHighBeam_HMIHighBeam->value)

#define Write_Runnable_Step_HMIHighBeam_HMIHighBeam(s,v) ((s)->Runnable_Step_HMIHighBeam_HMIHighBeam->value = (v))

#define Rte_IRead_Runnable_Step_HMILowBeam_HMILowBeam(s) ((s)->Runnable_Step_HMILowBeam_HMILowBeam->value)

#define Write_Runnable_Step_HMILowBeam_HMILowBeam(s,v) ((s)->Runnable_Step_HMILowBeam_HMILowBeam->value = (v))

#define Rte_IRead_Runnable_Step_HMIPositionLamp_HMIPositionLamp(s) ((s)->Runnable_Step_HMIPositionLamp_HMIPositionLamp->value)

#define Write_Runnable_Step_HMIPositionLamp_HMIPositionLamp(s,v) ((s)->Runnable_Step_HMIPositionLamp_HMIPositionLamp->value = (v))

#define Rte_IWrite_Runnable_Step_CMDHighBeam_CMDHighBeam(s,v) ((s)->Runnable_Step_CMDHighBeam_CMDHighBeam->value = (v))

#define Rte_IWriteRef_Runnable_Step_CMDHighBeam_CMDHighBeam() ((P2VAR(UInt8, AUTOMATIC, RTE_DATA))  &Rte_Inst_AR_HeadLampSWC.Runnable_Step_CMDHighBeam_CMDHighBeam->value)

#define Rte_IWrite_Runnable_Step_CMDLowBeam_CMDLowBeam(s,v) ((s)->Runnable_Step_CMDLowBeam_CMDLowBeam->value = (v))

#define Rte_IWriteRef_Runnable_Step_CMDLowBeam_CMDLowBeam() ((P2VAR(UInt8, AUTOMATIC, RTE_DATA))  &Rte_Inst_AR_HeadLampSWC.Runnable_Step_CMDLowBeam_CMDLowBeam->value)

#define Rte_Call_CS_IF_SetLedState_OP_SetLedState(s,v) (IOAbstractionSWC(v))

/*******************************************************************************
 **                        Global Function Declarations                       **
******************************************************************************/

/** API functions */

void Rte_Runnable_Step(Rte_Instance_AR_HeadLampSWC s);

void Rte_Runnable_Init(Rte_Instance_AR_HeadLampSWC s);

/** runnables */

#define AR_HeadLampSWC_START_SEC_CODE
#include "AR_HeadLampSWC_MemMap.h"


extern void Runnable_Step(Rte_Instance_AR_HeadLampSWC s);

extern void Runnable_Init(Rte_Instance_AR_HeadLampSWC s);

#define AR_HeadLampSWC_STOP_SEC_CODE
#include "AR_HeadLampSWC_MemMap.h"




#endif

