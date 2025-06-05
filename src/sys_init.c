/*================================================================================================*/
/**
*    @file        sys_init.c
*    @version     1.0.1
*
*    @brief       Integration Framework - System initialization code
*     
*    (c) Copyright 2006-2016 Freescale Semiconductor, Inc. 
*       Copyright 2017-2018 NXP
*    All Rights Reserved.
*
*    This file contains sample code only. It is not part of the production code deliverables.
*/       
/*=================================================================================================
*    Platform      :      ARM
*    Build Version :      S32K14x_MCAL_1_0_1_RTM_ASR_REL_4_2_REV_0002_20170831
*
==================================================================================================*/
/*==================================================================================================
====================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

    
/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Mcal.h"
#include "Platform_Types.h"
#include "StdRegMacros.h"
#include "sys_init.h"
#include "device_registers.h"
/** 
* @brief sys_init
* @details function used to enable all interrupts
*/
void sys_enableAllInterrupts(void)
{
    ResumeAllInterrupts();
}
/** 
* @brief sys_init
* @details function used to disable all interrupts
*/
void sys_disableAllInterrupts(void)
{
    SuspendAllInterrupts();
}
/** 
* @brief sys_init
* @details function used to disable the interrupt number id
*/
void sys_disableIsrSource(uint8 id)
{
    NVIC_DisableIRQ(id);
}
/** 
* @brief sys_init
* @details function used to enable the interrupt number id and set up the priority
*/
void sys_enableIsrSource(uint8 id, uint8 prio)
{
    NVIC_SetPriority(id, prio);
    NVIC_EnableIRQ(id);
}
/** 
* @brief sys_init
* @details function used to register the interrupt handler in the interrupt vector
*/
void  sys_registerIsrHandler(uint8 irq_id, uint32 isr_handler)
{


#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
	Mcal_goToSupervisor();
#endif
    REG_WRITE32(S32_SCB->VTOR + ((16 + irq_id) << 2), isr_handler);
#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
	Mcal_goToUser();
#endif  
 
 
}
    

