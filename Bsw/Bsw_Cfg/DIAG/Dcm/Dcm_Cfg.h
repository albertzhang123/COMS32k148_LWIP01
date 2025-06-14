/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dcm_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:44>
 */
/*============================================================================*/


#ifndef DCMCFG_H
#define DCMCFG_H
/****************************** references *********************************/
#include "ComStack_Types.h"
#include "CanTp_Cfg.h"
/****************************************************************************************
*********************************DcmGeneral container***********************************
*****************************************************************************************/
#define  DCM_DEV_ERROR_DETECT               		(STD_ON)
#define  DCM_RESPOND_ALL_REQUEST            		(STD_ON)
#define  DCM_VERSION_INFO_API               		(STD_OFF)
#define  DCM_GENERIC_CONNECTION             		(STD_OFF)    
#define  PREEMPTION_PROTOCOL_CANCEL_SUPPORT   		(STD_OFF)

/****************************************************************************************
********************************DcmPageBufferCfg container******************************
*****************************************************************************************/
#define DCM_PAGEDBUFFER_ENABLED                     (STD_ON)
#define DCM_PAGEBUFFER_TIMEOUTVALUE                 0x3e8

/*Enable support for CAN FD frames*/
#define DCM_FD                                      (STD_OFF)

/****************************************************************************************
*********************************DcmDsl container***************************************
*****************************************************************************************/
#define  DCM_CHANNEL_NUM                    		(2u)     /*number of configuration Channel*/
#define  DCM_CHANNEL_LENGTH                 		(2048u)   /*the total length of channel*/
#define  DCM_TEMP_BUFFER_LENGTH                 	(300u)
#define  DCM_DSLPROTOCOLROW_NUM_MAX         		(1u)     /*number of configuration protocol*/
#define  DCM_MAINCONNECTION_NUM             		(1u)
#define  DCM_PERIODICCONNECTION_NUM                 (1u)
#define  DCM_ROECONNECTION_NUM                      (1u)
#define  DCM_CONNECTION_NUM							(1u)

#define  DCM_DSL_RX_ID_NUM                  		(2u)
#define  DCM_DSL_TX_ID_NUM                  		(2u)

#define  DCM_DSLDIAGRESP_FORCERESPENDEN     		(STD_ON) /*Enable/Disable application directly trigger response Pending*/

#define  DCM_FRAME_LENGTH                   		(8u)

#define DCM_DCM_UDS_RX_PHY									0x0 /*RxPdu Id*/
#define DCM_DCM_UDS_RX_FUN									0x1 /*RxPdu Id*/

#define DCM_DCM_UDS_TX									0x0 /*TxPdu Id*/
#define DCM_PDUR_DCM_UDS_TX								PDUR_SRCPDU_DCM_UDS_TX
#define DCM_DCM_2A									0x1 /*TxPdu Id*/
#define DCM_PDUR_DCM_2A								PDUR_SRCPDU_DCM_2A

/****************************************************************************************
*********************************DcmDsd container****************************************
*****************************************************************************************/
#define  DCM_SERVICE_TAB_NUM                		(1u)     /*number of service table*/
/*******************************************/
/*UDS protocol*/
#define  DCM_UDS_FUNC_ENABLED               		(STD_ON) /*Enable/Disable UDS Protocol*/
#if(STD_ON == DCM_UDS_FUNC_ENABLED)		
	#define  DCM_UDS_SERVICE_NUM            		(22u)								
    #define  DCM_UDS_SERVICE0X10_ENABLED    		(STD_ON) /*DiagnosticSessionControl*/
    #define  DCM_UDS_SERVICE0X11_ENABLED    		(STD_ON) /*ECUReset*/					
	#define  DCM_UDS_SERVICE0X14_ENABLED    		(STD_ON) /*ClearDiagnosticInformation*/	
    #define  DCM_UDS_SERVICE0X19_ENABLED    		(STD_ON) /*ReadDTCInformation*/			
    #define  DCM_UDS_SERVICE0X22_ENABLED    		(STD_ON) /*ReadDataByIdentifier*/		
    #define  DCM_UDS_SERVICE0X23_ENABLED    		(STD_ON) /*ReadMemoryByAddress*/		
    #define  DCM_UDS_SERVICE0X24_ENABLED            (STD_ON) /*ReadScalingDataByIdentifier*/
    #define  DCM_UDS_SERVICE0X27_ENABLED    		(STD_ON) /*SecurityAccess*/				
    #define  DCM_UDS_SERVICE0X28_ENABLED    		(STD_ON) /*CommunicationControl*/		
	#define  DCM_UDS_SERVICE0X2A_ENABLED    		(STD_ON) /*ReadDataByPeriodicIdentifier*/
    #define  DCM_UDS_SERVICE0X2C_ENABLED    		(STD_ON) /*DynamicallyDefineDataIdentifier*/
    #define  DCM_UDS_SERVICE0X2E_ENABLED    		(STD_ON) /*WriteDataByIdentifier*/		
	#define  DCM_UDS_SERVICE0X2F_ENABLED    		(STD_ON) /*InputOutputControlByIdentifier*/
    #define  DCM_UDS_SERVICE0X31_ENABLED    		(STD_ON) /*RoutineControl*/				
	#define  DCM_UDS_SERVICE0X34_ENABLED    		(STD_ON) /*RequestDownload*/		
	#define  DCM_UDS_SERVICE0X35_ENABLED    		(STD_ON) /*RequestUpload*/			
	#define  DCM_UDS_SERVICE0X36_ENABLED    		(STD_ON) /*TransferData*/			
	#define  DCM_UDS_SERVICE0X37_ENABLED    		(STD_ON) /*RequestTransferExit*/	
	#define  DCM_UDS_SERVICE0X38_ENABLED    		(STD_ON) /*RequestFileTransfer*/	
	#define  DCM_UDS_SERVICE0X3D_ENABLED    		(STD_ON) /*WriteMemoryByAddress*/	
	#define  DCM_UDS_SERVICE0X3E_ENABLED    		(STD_ON) /*TesterPresent*/			
    #define  DCM_UDS_SERVICE0X83_ENABLED    		(STD_OFF) /*AccessTimingParameter*/	
    #define  DCM_UDS_SERVICE0X84_ENABLED    		(STD_OFF) /*SecuredDataTransmission*/	
    #define  DCM_UDS_SERVICE0X85_ENABLED    		(STD_ON) /*ControlDTCSetting*/		
    #define  DCM_UDS_SERVICE0X86_ENABLED    		(STD_ON) /*ResponseOnEvent*/		
    #define  DCM_UDS_SERVICE0X87_ENABLED    		(STD_OFF) /*LinkControl*/ 			
#endif
/**********************************************/
/*OBD Protocol*/
#define  DCM_OBD_FUNC_ENABLED               		(STD_OFF)   /*Enable/Disable OBD Protocol*/
#if(STD_ON == DCM_OBD_FUNC_ENABLED)		
    #define  DCM_OBD_SERVICE_NUM            		(0u)	
    #define  DCM_OBD_SERVICE0X01_ENABLED    		(STD_OFF)	/*RequestCurrentPowertrainDiagnosticData*/
    #define  DCM_OBD_SERVICE0X02_ENABLED    		(STD_OFF)	/*RequestPowertrainFreezeFrameData*/
    #define  DCM_OBD_SERVICE0X03_ENABLED    		(STD_OFF)	/*RequestEmission_relatedDTC*/	
    #define  DCM_OBD_SERVICE0X04_ENABLED    		(STD_OFF)	/*ClearEmission_relatedDiagnosticInfo*/	
    #define  DCM_OBD_SERVICE0X05_ENABLED    		(STD_OFF)	/*RequestOxygenSensorMonitoringTestResults*/	
    #define  DCM_OBD_SERVICE0X06_ENABLED    		(STD_OFF)	/*RequestOn_boardMonitoringTestResults*/		
    #define  DCM_OBD_SERVICE0X07_ENABLED    		(STD_OFF)	/*RequestEmission_relatedDTCDuringLastDC*/
    #define  DCM_OBD_SERVICE0X08_ENABLED    		(STD_OFF)	/*RequestControl*/
    #define  DCM_OBD_SERVICE0X09_ENABLED    		(STD_OFF)	/*RequestVehicleInformation*/
    #define  DCM_OBD_SERVICE0X0A_ENABLED    		(STD_OFF)	/*RequestEmission_relatedDTCWithPermanentStatus*/
#endif
/**********************************************/
#define  DCM_PERIODIC_FUNC_ENABLED          		(STD_ON) 	/*Enable/Disable Periodic Protocol*/
#define  DCM_ROE_FUNC_ENABLED               		(STD_ON) 	/*Enable/Disable ROE Protocol*/
#define  DCM_DSD_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED    (STD_OFF)
#define  DCM_DSD_REQUEST_SUPPLIER_NOTIFICATION_ENABLED        (STD_OFF)

/******************************************************************************
 ********************************DcmDsp container*****************************
 ******************************************************************************/
#define  DCM_DSP_DID_FUNC_ENABLED					(STD_ON)		/*Enable/disable DID function*/
#define  DCM_DSP_DID_FOR_2F_NUM                     (1u)            /*DID for 2F Number*/
#define  DCM_DSP_DATA_FUNC_ENABLED          		(STD_ON)

#define  DCM_DDDID_STORAGE                          (STD_OFF)       /*Enable/disable DDDID function*/
#define  DCM_DSP_DDDID_MAX_NUMBER                   (2u)            /*max number of the DDDID for 0x2C*/
#define  DCM_DSP_DDDID_ELEMENTS_MAX                 (2u)            /*One DDDID can contain max number of elements */
/**=======================================================**/
#define  DCM_DSP_ECU_RESET_FUNC_ENABLED				(STD_ON)       	/*Enable/disable EcuReset function,corresponding UDS protocol 0x11 Service*/
/**=======================================================**/
#define  DCM_DSP_READ_DTC_FUNC_ENABLED				(STD_ON)       	/*Enable/disable ReadDTC function,corresponding UDS protocol 0x19 Service*/
#define  DCM_UDS_SERVICE0X19_01_ENABLED				(STD_ON)
#define  DCM_UDS_SERVICE0X19_02_ENABLED				(STD_ON)
#define  DCM_UDS_SERVICE0X19_03_ENABLED				(STD_OFF)
#define  DCM_UDS_SERVICE0X19_04_ENABLED				(STD_ON)
#define  DCM_UDS_SERVICE0X19_06_ENABLED				(STD_ON)
#define  DCM_UDS_SERVICE0X19_0A_ENABLED				(STD_ON)
#define  DCM_UDS_SERVICE0X19_0E_ENABLED             (STD_ON)
/**=======================================================**/
#define  DCM_DSP_ROUTINE_FUNC_ENABLED				(STD_ON)       	/*Enable/disable RoutinControl function,corresponding UDS protocol 0x31 Service.*/
#define  DCM_DSP_ROUTINE_MAX_NUM            		(2u)       		/*Number of RoutineID*/
/**=======================================================**/
#define  DCM_SESSION_FUNC_ENABLED					(STD_ON)   		/*Enable/disable SessionControl Function*/
/**=======================================================**/
#define  DCM_SECURITY_FUNC_ENABLED					(STD_ON)   		/*Enable/disable SecurityAccess Function*/
#define  DCM_SECURITY_NUM                           (2u)            /*security total number*/
#define  DCM_SEED_MAX_SIZE                          (4u)            /*security total number*/
#define  DCM_SECURITY_MAX_READOUT_TIME              (0u)
#if(STD_ON == DCM_SECURITY_FUNC_ENABLED)
	#define  DCM_SECURITY_ADR_MAXSIZE       	  	(0u)      		/*In all security levels,the maximum length of ADR*/
#endif
/**=======================================================**/
#define  DCM_DSP_PID_FUNC_ENABLED           		(STD_OFF)		/*corresponding OBD protocol 0x01 Service*/
/**=======================================================**/
#define  DCM_DSP_REQUESTCONTROL_FUNC_ENABLED      	(STD_OFF)   	/*corresponding OBD protocol 0x08 Service*/
/**=======================================================**/
#define  DCM_DSP_VEHINFO_FUNC_ENABLED				(STD_OFF)   	/*corresponding OBD protocol 0x09 Service*/

#define  DCM_DSP_MAX_PERIODIC_DID_TO_READ            0x10u
#define  DCM_DSP_MAX_PERIODIC_DID_SCHEDULER          0x2u

#define  DCM_DSP_ROE_NUM                             (2u)
#define  DCM_ROE_STORAGE_BLOCKID                     (NvMBlock_Dcm_Roe)               /*ROE Storage BlockID*/
#define  DCM_UDS_SERVICE0X86_00_ENABLED              (STD_ON)
#define  DCM_UDS_SERVICE0X86_01_ENABLED              (STD_ON)
#define  DCM_UDS_SERVICE0X86_03_ENABLED              (STD_ON)
#define  DCM_UDS_SERVICE0X86_04_ENABLED              (STD_ON)
#define  DCM_UDS_SERVICE0X86_05_ENABLED              (STD_ON)
#define  DCM_UDS_SERVICE0X86_06_ENABLED              (STD_ON)
 
#define  DCM_BSWM_ENABLE                             (STD_OFF)

#define  NVM_ENABLE                                  (STD_ON)

#define  DCM_TM                                      STD_ON

#endif /* DCMCFG_H_ */

