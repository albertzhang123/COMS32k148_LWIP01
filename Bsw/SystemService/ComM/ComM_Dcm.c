/************************************************************************
*				        ESEC UESTC
* 	 Copyright (C) 2005-2011 ESEC UESTC. All Rights Reserved.
***********************************************************************/

/******************************* references ************************************/
#include "ComM_Dcm.h"
#include "Dcm_Cbk.h"


void ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel)
{
    Dcm_ComM_FullComModeEntered(Channel);
    return;
}

void ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel)
{
    Dcm_ComM_NoComModeEntered(Channel);
    return;
}
