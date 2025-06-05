#include "Fls_Test.h"
#include "NvM.h"
#include "MemIf_Types.h"
#include "Can.h"
#include "Com.h"
#include <string.h>
typedef enum
{
	FEE_ERASE_STATE = 0,
	FEE_ERASE_BUSY_STATE = 1,
	FEE_WRITE_STATE = 2,
	FEE_WRITE_BUSY_STATE = 3,
	FEE_READ_STATE= 4,
	FEE_READ_BUSY_STATE= 5,
	FEE_VALIDATE_STATE= 6,
	FEE_ERROR_STATE= 7,
	FEE_FINAL_STATE=8,
	FEE_READ_ERROR_STATE=9
}	Fee_StatusType;
static CONST (uint8, AUTOMATIC) FeeTest_WriteBuff[4]={0x55,0xAA,0x55,0xAA};
static VAR (uint8, AUTOMATIC)  FeeTest_ReadBuff[4]={0};
Fee_StatusType FeeState=FEE_ERASE_STATE;
Can_PduType pstSampleAppData;

uint8 FeeBankIdx = 17;
void Fls_Test(void)
{
    volatile StatusType status;                 /* variable to check system status */
    Std_ReturnType stdRet = E_NOT_OK;
    MemIf_StatusType memif_status;

    memif_status = Fee_GetStatus();

    /* state-machine logic */
    switch(FeeState)
    {
        case FEE_ERASE_STATE:
        {
            if (MEMIF_IDLE == memif_status)
            {
                stdRet = Fee_EraseImmediateBlock(FeeBankIdx);
                if (stdRet != E_OK)
                {
//                    CONSOLE_MESSAGE("FEE Error %d returned by Fee_EraseImmediateBlock()", stdRet);
                	uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x01;
                	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
                    /*CONSOLE_MESSAGE("FEE state-machine state:%d", FeeState);*/
                    FeeState = FEE_ERROR_STATE;
                }
                else
                {
                    /* change the state */
                    FeeState = FEE_ERASE_BUSY_STATE;
                    uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x00;
                    Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
                    stdRet = E_OK;
                }
            }
            else
            {
                stdRet = E_OK;
            }
        }
        break;
        case FEE_ERASE_BUSY_STATE:
            if (MEMIF_IDLE == memif_status)
            {
                FeeState = FEE_WRITE_STATE;
            }
            stdRet = E_OK;
        break;
        case FEE_WRITE_STATE:
        {
            if (MEMIF_IDLE == memif_status)
            {
                stdRet = Fee_Write(FeeBankIdx, (uint8 *)FeeTest_WriteBuff);
                if (stdRet != E_OK)
                {
//                    CONSOLE_MESSAGE("FEE Error returned by Fee_Write()", stdRet);
                	uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x02;
                	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
                    /*CONSOLE_MESSAGE("FEE state-machine state:", FeeState);*/
                    FeeState = FEE_ERROR_STATE;
                }
                else
                {
                    /* change the state */
                    FeeState = FEE_WRITE_BUSY_STATE;
                    stdRet = E_OK;
                }
            }
            else
            {
                stdRet = E_OK;
            }
        }
        break;
        case FEE_WRITE_BUSY_STATE:
            if (MEMIF_IDLE == memif_status)
            {
                FeeState = FEE_READ_STATE;
            }
            stdRet = E_OK;
        break;
        case FEE_READ_STATE:
        {
            if (MEMIF_IDLE == memif_status)
            {
                stdRet = Fee_Read(FeeBankIdx, 0, FeeTest_ReadBuff, 4u);
                if (stdRet != E_OK)
                {
//                    CONSOLE_MESSAGE("FEE Error %d returned by Fee_Read()", stdRet);
                	uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x03;
                	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
                    /*CONSOLE_MESSAGE("FEE state-machine state:", FeeState);*/
                    FeeState = FEE_ERROR_STATE;
                }
                else
                {
                    /* change the state */
                    FeeState = FEE_READ_BUSY_STATE;
                }
            }
            else
            {
                stdRet = E_OK;
            }
        }
        break;
        case FEE_READ_BUSY_STATE:
            if (MEMIF_IDLE == memif_status)
            {
                FeeState = FEE_VALIDATE_STATE;
            }
             stdRet = E_OK;
        break;
        case FEE_VALIDATE_STATE:
        {
            /* compare the read FeeTest_WriteBuffer with the written one */
            stdRet = memcmp((uint8 *)FeeTest_WriteBuff, FeeTest_ReadBuff, 4u);
            if (stdRet != E_OK)
            {
            	uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0x04;
            	Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
                //CONSOLE_MESSAGE("FEE Error %d returned by SampleAppFee_memcmp()", stdRet);
                /*CONSOLE_MESSAGE("FEE state-machine state:%d", FeeState);*/
                FeeState = FEE_READ_ERROR_STATE;
//                CONSOLE_MESSAGE("Fee task READ ERROR!", 0);
            }
            else
            {
                /* change the state */
                FeeState = FEE_FINAL_STATE;
                uint8 CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend = (uint8)0xFF;
                Com_SendSignal(CmdLowBeam_IPDU_COM_CmdSend, &CmdLowBeam_buf_CmdLowBeam_IPDU_COM_CmdSend);
                /*if(FeeBankIdx < 70)
                {
                    FeeBankIdx++;
                    FeeState = FEE_ERASE_STATE;
                }*/
//                CONSOLE_MESSAGE("Fee task ended OK.", 0);
            }
        }
        break;

        case FEE_ERROR_STATE:
        {

        }
        /*break;*/

        default:
        {
            /* if we got here, something went terribly wrong*/
            stdRet = E_NOT_OK;
        }
        break;
    }





}

