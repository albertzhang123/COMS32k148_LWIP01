/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "Mcu.h"
#include "Port.h"
#include "Dio.h"
#include "Can.h"
#include "Gpt.h"
#include "sys_init.h"
#include "CanIf.h"
#include "EcuM_Generated_Types.h"



#include "ComM.h"
//#include "CanSm.h"
#include "Eth.h"
#include "PhyTja110x.h"
#include "EthComm.h"
#include "EthIf.h"
#include "TcpIp.h"
#include "SoAd.h"
#include "LdCom.h"
#include "EthSM.h"
#include "Com.h"
#include "Fls.h"
#include "Fee.h"
#include "NVM.h"
#include "Rte_Dcm.h"
#include "CanTp.h"
#include "Dcm.h"
#include "Dem.h"
#define GPT_TIMER_LED_TICKS 2400000
#define GPT_TIMER_FLS1_TICKS 120000
#define GPT_TIMER_FLS2_TICKS 240000

uint32 Diag_Timer = 0;
uint32 Diag_ResetDelayTimer = 0;
uint32 FaultStatus = 1;

P2CONST(EcuM_GenBSWPbCfgType, AUTOMATIC, CANIF_APPL_DATA) pbCfg;
static volatile VAR(Dio_LevelType,AUTOMATIC)SampleApp_REDLedState=STD_HIGH;
static volatile VAR(Dio_LevelType,AUTOMATIC)SampleApp_BLUELedState=STD_HIGH;
static volatile VAR(Dio_LevelType,AUTOMATIC)SampleApp_GREENLedState=STD_HIGH;
static volatile VAR(Dio_LevelType,AUTOMATIC)Gpt_100msFlag=FALSE;
static volatile VAR(Dio_LevelType,AUTOMATIC)Gpt_5msFlag=FALSE;
static volatile VAR(Dio_LevelType,AUTOMATIC)Gpt_10msFlag=FALSE;
extern CONST(Mcu_ConfigType, MCU_CONST) McuModuleConfiguration;
extern ISR (LPIT_0_CH_0_ISR);
extern ISR (LPIT_0_CH_1_ISR);
extern ISR (LPIT_0_CH_2_ISR);
extern ISR (Can_IsrFCA_MB_00_15);
extern ISR (Can_IsrFCA_BO);
extern ISR (Eth_TxIrqHdlr_0);
extern ISR (Eth_RxIrqHdlr_0);
Can_ReturnType canRet;
uint8 CanHardwareObject_2;
Can_PduType pstSampleAppData;
uint8 MAC_Addr[6];
void Init_Intterupt(void);
void SampleAppGptLed(void);
void SampleAppGptFls1(void);
void SampleAppGptFls2(void);
void LwIP_Init(void);
void Ethernet_Init(void);
void Init_Intterupt()
{
	sys_disableAllInterrupts();
	/*GPT_interrupt*/
	sys_enableIsrSource(48,((7) << 4));
	sys_registerIsrHandler(48,(uint32)&LPIT_0_CH_0_ISR);//LPIT0_Ch0_IRQn(48)

	sys_enableIsrSource(49,((8) << 4));
	sys_registerIsrHandler(49,(uint32)&LPIT_0_CH_1_ISR);//LPIT0_Ch0_IRQn(48)

	sys_enableIsrSource(50,((9) << 4));
	sys_registerIsrHandler(50,(uint32)&LPIT_0_CH_2_ISR);//LPIT0_Ch0_IRQn(48)
	/*CAN_interrupt*/
	sys_enableIsrSource(81,((10) << 4));
	sys_registerIsrHandler(81,(uint32)&Can_IsrFCA_MB_00_15);//CAN0_ORed_0_15_MB_IRQn(81)
	/*ETH_interrupt*/

	sys_enableIsrSource(73,((11) << 4));//ENET_TX_IRQn
    sys_registerIsrHandler(73,(uint32)&Eth_TxIrqHdlr_0);//Eth_TxIrqHdlr_0(48)
    sys_enableIsrSource(74,((12) << 4));//ENET_RX_IRQn
    sys_registerIsrHandler(74,(uint32)&Eth_RxIrqHdlr_0);//Eth_RxIrqHdlr_0(48)

	sys_enableAllInterrupts();
}
/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses the Icu
*               and Dio drivers to toggle a LED on a push button
*/
int main(void)
{

    /* Initialize the Mcu driver */
    Mcu_Init(&McuModuleConfiguration);

    Mcu_InitClock(McuClockSettingConfig_0);

#if (MCU_NO_PLL == STD_OFF)
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
         //Busy wait until the System PLL is locked
    }

    Mcu_DistributePllClock();
#endif

    Mcu_SetMode(McuModeSettingConf_0);
    Init_Intterupt();
    /* Initialize all pins using the Port driver */
    Port_Init(&PortConfigSet);
    Can_Init(&CanConfigSet);
    Fls_Init(&FlsConfigSet);
    Fee_Init(NULL_PTR);
    EthIf_Init(&EthIf_ConfigData);
    EthSM_Init();
    Ethernet_Init();

    PHY_TJA1101_Init();

    Ethernet_FrameData_Init();

    TcpIp_Init(&TcpIp_Config);
    SoAd_Init(&SoAd_Config);

    ComM_Init(&ComM_Config);
    SoAd_RequestIpAddrAssignment(2, TCPIP_IPADDR_ASSIGNMENT_ALL, NULL_PTR, 0u, NULL_PTR);
    SoAd_OpenSoCon(2);
    LdCom_Init(&LdCom_InitCfgSet);

    //NvM_Init(NULL_PTR);
    //Dem_PreInit();
	CanIf_Init(&CanIf_InitCfgSet);
	PduR_Init(&PduR_PBConfigData);
	Com_Init(&Com_PBConfigData);

	//CanSM_Init(&CanSM_Config);
	//CanTp_Init(&CanTp_CfgData);
	//Dcm_Init(&Dcm_Cfg);

	Com_IpduGroupVector ipduGroupVector;
	Com_SetIpduGroup(ipduGroupVector, Com_PduGroup_CONTROLLER_0 ,TRUE);
	Com_ReceptionDMControl(ipduGroupVector);
	Com_IpduGroupControl(ipduGroupVector,TRUE);

	ComM_RequestComMode(ComMUser_Eth, COMM_FULL_COMMUNICATION);
	ComM_CommunicationAllowed(ComMChannel_Eth, TRUE);
	/* Initialize Gpt driver */
	Gpt_Init(&GptChannelConfigSet);
	Gpt_EnableNotification(0);
	Gpt_EnableNotification(1);
	Gpt_EnableNotification(2);
	Gpt_StartTimer(0,GPT_TIMER_LED_TICKS);
	Gpt_StartTimer(1,GPT_TIMER_FLS1_TICKS);
	Gpt_StartTimer(2,GPT_TIMER_FLS2_TICKS);
	//canRet=Can_SetControllerMode(CanConf_CanController_CanController_0,CAN_T_START);
	canRet=CanIf_SetControllerMode(CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0,CANIF_CS_STARTED);

	canRet=CanIf_SetPduMode(CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0, CANIF_ONLINE);

	/*VAR(uint8,CAN_VAR) data[8U]={0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31};
	pstSampleAppData.length=8U;
	pstSampleAppData.swPduHandle=1U;//与此参数无关
	pstSampleAppData.sdu=data;
	pstSampleAppData.id=0x302U;
	canRet=Can_Write(1,&pstSampleAppData);*/
	/*定义 LdCom 发送的 Pdu*/
	uint8 LdComSrcPduData[10] = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9};
	PduInfoType LdComTransmitPdu;
	LdComTransmitPdu.SduDataPtr = LdComSrcPduData;
	LdComTransmitPdu.SduLength = 10;
	LdComTransmitPdu.MetaDataPtr = NULL_PTR;
    while (1)
    {

    	if(Gpt_100msFlag == TRUE)
    	{
    		Gpt_100msFlag=FALSE;
    		PHY_TJA1101_GetCurrentStatus();

    		//Ethernet_TransmitFramesTest();

    		Com_MainFunctionRx();
    		Com_MainFunctionRouteSignals();

    		//Os_TaskEntry_Rte_OsTask_10ms();
    		//Fls_Test();
    		//NvM_Test1();
    		//NvM_Test2();
    		Com_MainFunctionTx();
    		/*if( 1== FaultStatus)
			{
			Dem_SetEventStatus(DemEventParameter_0xC00001, DEM_EVENT_STATUS_FAILED);

			}
			else if(0 == FaultStatus)
			{
			Dem_SetEventStatus(DemEventParameter_0xC00001, DEM_EVENT_STATUS_PASSED);
			}*/
    		//PHY_TJA1101_SendEthernetFrame();
    		EthSM_TrcvLinkStateChg(0,ETHTRCV_LINK_STATE_ACTIVE);
    		TcpIp_MainFunction();
    		LdCom_Transmit(LdComIPdu_EthUdpTx,&LdComTransmitPdu);

    	}else if(Gpt_5msFlag == TRUE){
    		Gpt_5msFlag=FALSE;
    		//NvM_ReadAll();
    		/*NvM_MainFunction();
			Fee_MainFunction();
			Fls_MainFunction();
			Dem_Init(&DemPbCfg);*/
    		ComM_MainFunction(ComMChannel_Eth);


    	}else if(Gpt_10msFlag == TRUE){

    		Gpt_10msFlag=FALSE;
    		/*CanTp_MainFunction();
    	    Dcm_MainFunction();
    		Dem_MainFunction();*/
    		Eth_MainFunction();
    		EthIf_MainFunctionRx();
    		EthIf_MainFunctionTx();
    		EthSM_MainFunction();
    		SoAd_MainFunction();
    	}



    }

    return (1U);
}

void SampleAppGptLed(void)
{
	Gpt_100msFlag=TRUE;
	if(STD_HIGH==SampleApp_REDLedState)
	{
		SampleApp_REDLedState=STD_LOW;
	}
	else
	{
		SampleApp_REDLedState=STD_HIGH;
	}


	Dio_WriteChannel((Dio_ChannelType)DioConf_DioChannel_RedLED, (Dio_LevelType)SampleApp_REDLedState);
}
void SampleAppGptFls1(void)
{
	Gpt_5msFlag=TRUE;
	if(STD_HIGH==SampleApp_BLUELedState)
	{
		SampleApp_BLUELedState=STD_LOW;
	}
	else
	{
		SampleApp_BLUELedState=STD_HIGH;
	}


	Dio_WriteChannel((Dio_ChannelType)DioConf_DioChannel_BlueLED, (Dio_LevelType)SampleApp_BLUELedState);
}
void SampleAppGptFls2(void)
{
	Gpt_10msFlag=TRUE;
	if(STD_HIGH==SampleApp_GREENLedState)
	{
		SampleApp_GREENLedState=STD_LOW;
	}
	else
	{
		SampleApp_GREENLedState=STD_HIGH;
	}


	Dio_WriteChannel((Dio_ChannelType)DioConf_DioChannel_GreenLED, (Dio_LevelType)SampleApp_GREENLedState);
}
void Ethernet_Init(void)
{
	Std_ReturnType retVal;
	uint8_t dummyCnt;
	uint8 MAC_Addr[6];

	/*Set SIU_FECCR register manually to select MII-Lite. API is avalable in MCAL4.3*/
	//REG_WRITE32(SIU_FECCR_REG_ADDR, (uint32)0x00000001UL);

	/* Initializes the Ethernet Driver. */
	Eth_Init(&EthConfigSet);

	dummyCnt = 0;
	/* Initializes the Ethernet Driver. */
	retVal = Eth_ControllerInit(CTRL_INDEX, CFG_INDEX);

	if (retVal == E_OK)
	{
		retVal = E_NOT_OK;

		/* Enables the given controller. */
		retVal = Eth_SetControllerMode(CTRL_INDEX, ETH_MODE_ACTIVE);

		if (retVal == E_OK)
		{
			/* Execute several empty operation to enable system can set breakpoint here when in debug mode. */
			while (dummyCnt < 5U) dummyCnt++;

			/* Obtains the physical source address used by the indexed controller. */
			Eth_GetPhysAddr(CTRL_INDEX, &MAC_Addr[0]);
		}
		else
		{
			/* Execute several empty operation to enable system can set breakpoint here when in debug mode. */
			while (dummyCnt < 5U) dummyCnt++;
		}
	}
	else
	{
		/* Execute several empty operation to enable system can set breakpoint here when in debug mode. */
		while (dummyCnt < 5U) dummyCnt++;
	}
}
