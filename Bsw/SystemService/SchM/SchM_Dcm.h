/************************************************************************
*				        ESEC UESTC
* 	 Copyright (C) 2005-2011 ESEC UESTC. All Rights Reserved.
***********************************************************************/

/**
 * Log:  
 * Revision 1.0 2011-6-22ÏÂÎç02:15:18 chenms
 * description: create
 *
 */

/**
 * @file 	SchM.h
 * @brief
 *	<li>Function£º </li>
 *  <li>Design Points£º</li>
 *  <p>
 *  
 *  </p>
 * @author 	chenms
 * @date 	2011-6-22
 * 
 */
#include "Mcu.h"

/******************************* definitions ***********************************/
#define SchM_Switch_DcmDiagnosticSessionControl(Session)          	SchM_Switch_##bsnp##_DcmDiagnosticSessionControl(Session)
#define SchM_Switch_DcmControlDTCSetting(Setting)                	SchM_Switch_##bsnp##_DcmControlDTCSetting(Setting)
#define SchM_Switch_DcmEcuReset(Reset)                          	SchM_Switch_##bsnp##_DcmEcuReset(Reset)

#define SchM_Switch_bsnp_DcmDiagnosticSessionControl(Session)
#define SchM_Switch_bsnp_DcmControlDTCSetting(Setting)
#define SchM_Switch_bsnp_DcmEcuReset(Reset)							SchM_PerformReset(Reset)



#define SchM_Enter_Dcm(exclusiveArea)  SchM_Enter_Dcm_##exclusiveArea
#define SchM_Exit_Dcm(exclusiveArea)   SchM_Exit_Dcm_##exclusiveArea

#define SchM_Enter_Dcm_Dcm_ChannelCtrl
#define SchM_Exit_Dcm_Dcm_ChannelCtrl

#define SchM_Enter_Dcm_Dcm_MsgCtrl
#define SchM_Exit_Dcm_Dcm_MsgCtrl

#define SchM_Enter_Dcm_Dcm_ConnectionCtrl
#define SchM_Exit_Dcm_Dcm_ConnectionCtrl

#define SchM_Enter_Dcm_Dcm_ProtocolCtrl
#define SchM_Exit_Dcm_Dcm_ProtocolCtrl

#define SchM_Enter_Dcm_Dcm_ProtocolPreemptCtrl
#define SchM_Exit_Dcm_Dcm_ProtocolPreemptCtrl

#define SchM_Enter_Dcm_Dcm_MkCtrl
#define SchM_Exit_Dcm_Dcm_MkCtrl

#define SchM_Enter_Dcm_Dcm_SesCtrl
#define SchM_Exit_Dcm_Dcm_SesCtrl

#define SchM_Enter_Dcm_Dcm_SecCtrl
#define SchM_Exit_Dcm_Dcm_SecCtrl

#define SchM_Enter_Dcm_Dcm_Channel
#define SchM_Exit_Dcm_Dcm_Channel

#define SchM_Enter_Dcm_Dcm_CommCtrl
#define SchM_Exit_Dcm_Dcm_CommCtrl

#define SchM_Enter_Dcm_Dcm_LinkControlCtrl
#define SchM_Exit_Dcm_Dcm_LinkControlCtrl

#define SchM_Enter_Dcm_Dcm_DspProgram
#define SchM_Exit_Dcm_Dcm_DspProgram

#define SchM_Enter_Dcm_Dcm_PageBufferData
#define SchM_Exit_Dcm_Dcm_PageBufferData

#define SchM_Enter_Dcm_Dcm_RoutineControlState
#define SchM_Exit_Dcm_Dcm_RoutineControlState


/****************************** implementations ********************************/
typedef uint8  SchM_ReturnType;
#define  SCHM_E_NOFUNC   ((uint8)0x05)
#define  SCHM_E_STATE    ((uint8)0x07)



