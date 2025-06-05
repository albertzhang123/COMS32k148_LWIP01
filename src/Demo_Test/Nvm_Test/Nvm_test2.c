#include "Nvm_Test2.h"
#include "NvM.h"
#include "Can.h"
#include "Com.h"
#include <string.h>

uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend=(uint8)0x00;
//uint8* PCmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend=&CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend;
uint8 NvMBlock_Did0xF010_RomBuffer[1] = {0x0u};
uint8 NvMBlock_Did0xF193_RomBuffer[4] = {0x30u, 0x30u, 0x30u, 0x30u};
uint8 NvMBlock_Did0xF195_RomBuffer[4] = {0x30u, 0x30u, 0x30u, 0x30u};
uint8 NvMBlock_Did0xF199_RomBuffer[4] = {0x0u, 0x0u, 0x0u, 0x0u};
uint8 NvMBlock_Did0xF19E_RomBuffer[4] = {0x0u, 0x0u, 0x0u, 0x0u};
uint8 NvMBlock_Did0x0100_RomBuffer[1] = {0x0u};
uint8 NvMBlock_Did0x0110_RomBuffer[1] = {0x0u};

uint8 NvM_WriteData[20];
uint8 NvM_ReadData[20];
uint8 NvM_Test_Cnt = 0;
uint16 NvM_Test_C = 0;
uint8 NvM_TestStep = 0;
void NvM_Test2(void)
{
	if (NvM_Test_C < 0xFFFFu)
	    {
	        NvM_RequestResultType NvM_RequestResult;
	        Std_ReturnType ret;
	        uint16 count = 20;
	        uint16 i;
	        ret = NvM_GetErrorStatus(NvMBlock1, &NvM_RequestResult);
	        if (ret != E_OK)
	        {
	            /* error: */
	        	CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0xAA;
	        	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	        }
	        if (NVM_REQ_PENDING != NvM_RequestResult)
	        {
	            /* NvM is idel ,do next test */
	            switch (NvM_TestStep)
	            {
	            case 0u:
	                /* do write */
	                for ( i = 0; i < count; i++)
	                {
	                	NvM_WriteData[i] = NvM_Test_Cnt;
	                }
	                ret = NvM_WriteBlock(NvMBlock1, NvM_WriteData);//NvM_WriteData
	                if (ret != E_OK)
	                {
	                    /* error: */
	                	CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x01;
	                	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	                }
	                else
	                {
	                	CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x02;
	                	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	                    NvM_TestStep++;
	                }
	                break;
	            case 1u:
	                /* do read */
	                for (i = 0; i < count; i++)
	                {
	                    NvM_ReadData[i] = 0xFF;
	                }
	                ret = NvM_ReadBlock(NvMBlock1, NvM_ReadData);
	                if (ret != E_OK)
	                {
	                    /* error: */
	                	CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x03;
	                	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	                }
	                else
	                {
	                    NvM_TestStep++;
	                    CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x04;
	                    Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	                }
	                break;
	            case 2u:
	                /* do compare */
	                for (i = 0; i < count; i++)
	                {
	                    if (NvM_ReadData[i] != NvM_WriteData[i])
	                    {
	                        /* error: */
	                    	CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x05;
	                    	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	                    }else{
	                    	CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x06;
	                    	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
	                    }
	                }
	                NvM_TestStep = 0;
	                NvM_Test_C++;
	                NvM_Test_Cnt++;
	                NvM_Test_Cnt = (NvM_Test_Cnt == 0xFFu) ? 0 : NvM_Test_Cnt;
	                break;
	            default:
	                NvM_TestStep = 0;
	                break;
	            }
	        }
	        else
	        {
	            /* NvM is work , wait */
	        }
	    }
	    else
	    {
	        /*test ok and end*/
	    }



}

