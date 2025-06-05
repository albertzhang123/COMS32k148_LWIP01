/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2016)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    : SchM_SoAd.h                                                **
**                                                                           **
**  Created on  : 12/03/19                                                   **
**  Author      : darren.zhang and zhengfei.li                               **
**  Vendor      :                                                            **
**  DESCRIPTION :      none                                                  **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef SCHM_SOAD_H_
#define SCHM_SOAD_H_
/*=======[I N C L U D E S]====================================================*/
//#include "Os.h"
/*=======[M A C R O S]========================================================*/
#define SOAD_AREA_MSGCONTEXT     (0)

#define SchM_Enter_SoAd(Instance, Exclusive_Area)    SuspendAllInterrupts()
#define SchM_Exit_SoAd(Instance, Exclusive_Area)     ResumeAllInterrupts()

#endif/* SCHM_SOAD_H_ */
