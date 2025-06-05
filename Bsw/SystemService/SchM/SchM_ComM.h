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
**  FILENAME    : SchM_ComM.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : stanleyluo                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION : ComM Schedule Manager                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*  <VERSION>    <DATE>      <AUTHOR>       <REVISION LOG>
 *  V2.0.0       20210927  darren.zhang       R19_11 initial version.
*/


#ifndef SCHM_COMM_H
#define SCHM_COMM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
//#include "Os.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define COMM_AREA_NVM_ACCESS        0x101u
#define COMM_AERA_STATE_ACCESS      0x102u

#define SCHM_ENTER_COMM(_instance,_areaid)  do{SuspendAllInterrupts();}while(0)
#define SCHM_EXIT_COMM(_instance,_areaid)   do{ResumeAllInterrupts();}while(0)

#define SCHM_ENTER_COMM_CH(_ch)        SCHM_ENTER_COMM(COMM_INSTANCE_ID,_ch)
#define SCHM_EXIT_COMM_CH(_ch)         SCHM_EXIT_COMM(COMM_INSTANCE_ID,_ch)



/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/



/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/



/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/


#endif /* SCHM_COMM_H */
