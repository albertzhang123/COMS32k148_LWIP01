/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Cbk.h>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_Cbk.h */

#ifndef RTE_CBK_H
#define RTE_CBK_H

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
#include "Com.h"
#include "Rte_Data.h"

/*******************************************************************************
 **                        Global Function Definitions                        **
******************************************************************************/

void Rte_COMCbk_lowBeam_IPDU_COM_LedStatus(void);

void Rte_COMCbkTAck_CmdLowBeam_IPDU_COM_CmdSend(void);

void Rte_COMCbkTErr_CmdLowBeam_IPDU_COM_CmdSend(void);

void Rte_COMCbk_PositionLamp_IPDU_COM_LedStatus(void);

void Rte_COMCbk_HighBeam_IPDU_COM_LedStatus(void);

void Rte_COMCbkTAck_CmdHighBeam_IPDU_COM_CmdSend(void);

void Rte_COMCbkTErr_CmdHighBeam_IPDU_COM_CmdSend(void);



#endif

