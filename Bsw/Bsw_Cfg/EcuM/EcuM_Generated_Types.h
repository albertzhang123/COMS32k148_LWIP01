/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2020)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : EcuM_Generated_Types.h                                      **
**                                                                            **
**  Created on  : 2022/01/06 09:47:31                                         **
**  Author      :                                                             **
**  Vendor      :                                                             **
**  DESCRIPTION :  contains generated type declarations that fulfill the      **
**                 forward declarations in EcuM.h                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19_11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/


#ifndef ECUM_GENERATED_TYPES_H
#define ECUM_GENERATED_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define ECUM_WKSOURCE(sourceId) (uint32)((uint32)0x01u << (sourceId))
#define EcuMWakeupSource		ECUM_WKSOURCE(5)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*********************Generated Configuration Types***************************/
typedef struct
{
	P2CONST(void, TYPEDEF, ECUM_APPL_DATA) bswMPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) mcuPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) portPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) adcPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) pwmPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) flsPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) canTrcvPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) canPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) gptPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) icuPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) canIfPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) pduRPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) comPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) canTpPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) dcmPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) nvmPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) xcpPbCfg;
	P2CONST(void, TYPEDEF, ECUM_CONFIG_DATA) demPbCfg;	
} EcuM_GenBSWPbCfgType;

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/



/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/


#endif /* ECUM_GENERATED_TYPES_H */
