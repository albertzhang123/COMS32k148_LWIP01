/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Name: Code-Frame generation feature
 * Description:
 * Version: 9.0
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************

 * Project : HeadLamp_Demo
 * Component: /IOAbstractionSWC/IOAbstractionSWC
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A V 9.0
 * Author: Administrator
 * Date : ����һ ʮ�� 29 10:03:14 2018
 ****************************************************************************/

#include "Rte_IOAbstractionSWC.h"
#include "Dio.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :IOAbstractionSWC) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :IOAbstractionSWC) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :IOAbstractionSWC) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define RE_SetLedState_START_SEC_CODE                   
#include "IOAbstractionSWC_MemMap.h"
FUNC (void , IOAbstractionSWC_CODE) IOAbstractionSWC // return value & FctID
(
		CONSTP2VAR(UInt8, AUTOMATIC, RTE_APPL_DATA) DE_LedState
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :IOAbstractionSWC) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;

	if (*DE_LedState == 1)
	{
		//Dio_WriteChannel(DioConf_DioChannel_RedLED, STD_LOW);
	}
	else
	{
		//Dio_WriteChannel(DioConf_DioChannel_RedLED, STD_HIGH);
	}
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :IOAbstractionSWC) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}

#define RE_SetLedState_STOP_SEC_CODE                       
#include "IOAbstractionSWC_MemMap.h"
