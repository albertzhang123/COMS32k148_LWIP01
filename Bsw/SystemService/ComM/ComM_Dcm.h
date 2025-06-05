/************************************************************************
*				        ESEC UESTC
* 	 Copyright (C) 2005-2011 ESEC UESTC. All Rights Reserved.
***********************************************************************/

#ifndef COMM_DCM_H_
#define COMM_DCM_H_

#include "ComStack_Types.h"

extern uint8  Dcm_ComM_ActiveDiagnostic;
extern uint8  Dcm_ComM_InactiveDiagnostic;

extern  void ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel);
extern  void ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel);


#endif /* COMM_DCM_H_ */
