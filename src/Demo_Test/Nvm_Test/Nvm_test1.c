#include "Nvm_Test1.h"
#include "NvM.h"
#include "Can.h"
#include "Com.h"
#include <string.h>

NvM_RequestResultType NvmStatus=(uint8)0x00;


void NvM_Test1(void)
{
	Std_ReturnType ret;
	uint8 *RAMBlock2 = (uint8 *)0x1FFFF802;
	uint8 *RAMBlock3 = (uint8 *)0x1FFFF816;
	uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend=(uint8)0x00;
	static uint8 NvM_TaskNum = 1;
	if(NvM_TaskNum == 1)
	{
		NvM_GetErrorStatus(NvMBlock1,&NvmStatus);
		if( NVM_REQ_PENDING != NvmStatus )
		{


			memset(NvMBlock1,0xaa,20);
			//ret=NvM_WriteBlock(NvMBlock1,NvMBlockRamBuffer2);
			if (ret != E_OK)
			{
				CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x01;
				Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);

			}
			else
			{
				/* change the state */
				NvM_TaskNum = 2;
				CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x02;
				Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
			}
		}
	}
	else if(NvM_TaskNum == 2)
	{

		//ret=NvM_ReadBlock(NvMBlock1, RAMBlock3);
		if (ret != E_OK)
		{
			CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0xEF;
			Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);

		}
		else
		{

			CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0xFF;
			Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
		}
	}


}

