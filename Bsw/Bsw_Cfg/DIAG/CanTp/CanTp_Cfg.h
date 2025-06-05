/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <CanTp_Cfg.h>
 *  @brief      <>
 *  
 *  <MCU:G9X>
 *  
 *  @author     <>
 *  @date       <2021-12-28 09:45:40>
 */
/*============================================================================*/


/*============================================================================*/
#ifndef CANTP_CFG_H
#define CANTP_CFG_H

#include "CanIf_Cfg.h"
#include "PduR_Cfg.h"


/*======== [I N C L U D E S]========================*/
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/*========[M A C R O S] ==========================*/

/* Switches the Development Error Detection and Notification ON or OFF */
#define CANTP_DEV_ERROR_DETECT 					STD_ON

/* Switches the time service ON or OFF */
#define CANTP_TM                                STO_ON

/* The time for MainFunction,expressed as the value with ms */
#define CANTP_MAIN_FUNCTION_PERIOD 				0x0u

/*configuration MACRO addressing mode for cutting*/
#define CANTP_EXTENDED_ADDRESSING_SUPPORT    	STD_OFF
#define CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT	STD_OFF
#define CANTP_MIXED_ADDRESSING_SUPPORT    		STD_OFF
#define CANTP_MIXED29_ADDRESSING_SUPPORT    	STD_OFF

/* Used for the initialization of unused bytes with a certain value */
#define CANTP_PADDING_BYTE 						0x55u

/* Preprocessor switch for enabling Transmit Cancellation and Receive Cancellation. */
#define CANTP_TC 					    		STD_OFF

/* Preprocessor switch for enabling CanTp_ChangeParameterRequest Api*/
#define CANTP_CHANGE_PARAMETER					STD_ON

/* Preprocessor switch for enabling CanTpReadParameterApi Api*/
#define CANTP_READ_PARAMETER					STD_OFF

#define CANTP_VERSION_INFO_API 					STD_OFF

/*Enable support for CAN FD frames*/
#define CANTP_FD								STD_OFF

#define CANTP_DYN_ID_SUPPORT                	STD_OFF
#define CANTP_GENERIC_CONNECTION_SUPPORT		STD_OFF

/* Total number of channel used in CanTp module. */
#define CANTP_CHANNEL_NUMBER 		   			0x1u

 
/* -------------------- CanTpChannel_UDS -------------------- */

#define CANTP_CH0_RXNSDU_NUMBER     			0x2u
#define CANTP_CH0_TXNSDU_NUMBER     			0x1u


/* CanTpRxNSdu_0 */	
#define CANTP_PDUR_CANTP_UDS_RX_PHY     					PDUR_SRCPDU_CANTP_UDS_RX_PHY/*RxIPdu Id*/
#define CANTP_CANTP_UDS_RX_PHY								0x71u/*RxNSdu Id*/
#define CANTP_CANIF_UDS_RX_PHY		     					0x6eu/*RxNPdu Id*/
#define CANTP_CANIF_UDS_TX   							0x6du/*TxFcNPdu Id*/
#define CANTP_CANIF_CANIF_UDS_TX  						CANIF_TXPDU_CANIF_UDS_TX/*TxFcLPdu Id*/

/* CanTpRxNSdu_1 */	
#define CANTP_PDUR_CANTP_UDS_RX_FUN     					PDUR_SRCPDU_CANTP_UDS_RX_FUN/*RxIPdu Id*/
#define CANTP_CANTP_UDS_RX_FUN								0x72u/*RxNSdu Id*/
#define CANTP_CANIF_UDS_RX_FUN		     					0x6fu/*RxNPdu Id*/


/* CanTpTxNSdu_0 */	
#define CANTP_PDUR_CANTP_UDS_TX     					PDUR_DESTPDU_CANTP_UDS_TX/*TxIPdu Id*/
#define CANTP_CANTP_UDS_TX								0x70u/*TxNSdu Id*/
#define CANTP_CANIF_UDS_TX		     				    0x6du/*TxNPdu Id*/
#define CANTP_CANIF_UDS_RX_PHY   							0x6eu/*RxFcNPdu Id*/
#define CANTP_CANIF_CANIF_UDS_TX						CANIF_TXPDU_CANIF_UDS_TX/*TxLPdu Id*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif /* CANTP_CFG_H */
