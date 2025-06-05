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
**  FILENAME    : Rte_SchM.h                                                 **
**                                                                           **
**  Created on  :                                                            **
**  Author      : darren.zhang                                               **
**  Vendor      :                                                            **
**  DESCRIPTION : Nm                                                         **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R1911                      **
**                                                                           **
**************************************************************************** */

#ifndef RTE_SCHM_H_
#define RTE_SCHM_H_
#include "Rte.h"
/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/*Post build configuration type*/
typedef struct
{
    uint8 dymmy;
} SchM_ConfigType;

FUNC(void, RTE_CODE)
SchM_Start(
    void
);

FUNC(void, RTE_CODE)
SchM_Init(
    P2CONST(SchM_ConfigType, AUTOMATIC, RTE_APPL_DATA) ConfigPtr
);


FUNC(void, RTE_CODE)
SchM_Deinit(
    void
);


FUNC(void, RTE_CODE)
SchM_StartTiming(
    void
);

#endif/* RTE_SCHM_H_ */
