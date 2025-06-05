/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2020)                                               **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME    : PduR_Cfg.h                                                  **
**                                                                            **
**  Created on  : 2022/06/02 13:43:03                                         **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Configuration parameter of PDUR                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef  PDUR_CFG_H
#define  PDUR_CFG_H
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "ComStack_Types.h"
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
#define PDUR_CFG_H_AR_MAJOR_VERSION  4u
#define PDUR_CFG_H_AR_MINOR_VERSION  2u
#define PDUR_CFG_H_AR_PATCH_VERSION  2u
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/

#define PDUR_ZERO_COST_OPERATION        STD_OFF

#define PDUR_DEV_ERROR_DETECT           STD_ON

#define PDUR_META_DATA_SUPPORT          STD_OFF

#define PDUR_VERSION_INFO_API           STD_OFF

#define PDUR_BSW_MODULE_SUM             6u
#define PDUR_CANIF      0u
#define PDUR_COM      1u
#define PDUR_CANTP      2u
#define PDUR_DCM      3u
#define PDUR_SOAD      4u
#define PDUR_LDCOM      5u

#define PDUR_CANIF_SUPPORT        STD_ON
#define PDUR_CANTP_SUPPORT        STD_ON
#define PDUR_J1939TP_SUPPORT      STD_OFF
#define PDUR_COM_SUPPORT          STD_ON
#define PDUR_DCM_SUPPORT          STD_ON
#define PDUR_J1939DCM_SUPPORT     STD_OFF
#define PDUR_IPDUM_SUPPORT        STD_OFF
#define PDUR_J1939RM_SUPPORT      STD_OFF
#define PDUR_LDCOM_SUPPORT        STD_ON
#define PDUR_SECOC_SUPPORT        STD_OFF
#define PDUR_DBG_SUPPORT          STD_OFF
#define PDUR_CANNM_SUPPORT        STD_OFF
#define PDUR_OSEKNM_SUPPORT       STD_OFF
#define PDUR_LINIF_SUPPORT        STD_OFF
#define PDUR_LINTP_SUPPORT        STD_OFF
#define PDUR_FRIF_SUPPORT         STD_OFF
#define PDUR_FRNM_SUPPORT         STD_OFF
#define PDUR_DOIP_SUPPORT         STD_OFF
#define PDUR_SOAD_SUPPORT         STD_ON
#define PDUR_UDPNM_SUPPORT        STD_OFF
#define PDUR_DLT_SUPPORT          STD_OFF
#define PDUR_SOMEIPTP_SUPPORT     STD_OFF

#define PDUR_TRANSMIT_SUPPORT                 STD_ON

#define PDUR_CANCEL_TRANSMIT                       STD_OFF 
#define PDUR_CANIF_CANCEL_TRANSMIT                 STD_OFF 
#define PDUR_CANTP_CANCEL_TRANSMIT                 STD_OFF 
#define PDUR_J1939TP_CANCEL_TRANSMIT               STD_OFF 
#define PDUR_SECOC_CANCEL_TRANSMIT                 STD_OFF 
#define PDUR_LINIF_CANCEL_TRANSMIT                 STD_OFF 
#define PDUR_DOIP_CANCEL_TRANSMIT                  STD_OFF 
#define PDUR_SOAD_CANCEL_TRANSMIT                  STD_OFF 
#define PDUR_FRIF_CANCEL_TRANSMIT                  STD_OFF 

#define PDUR_CHANGE_PARAMETER                      STD_OFF 
#define PDUR_CANTP_CHANGE_PARAMETER                STD_OFF
#define PDUR_J1939TP_CHANGE_PARAMETER              STD_OFF
#define PDUR_LINIF_CHANGE_PARAMETER                STD_OFF
#define PDUR_SOAD_CHANGE_PARAMETER                 STD_OFF

#define PDUR_CANCEL_RECEIVE                        STD_OFF 
#define PDUR_CANTP_CANCEL_RECEIVE                  STD_OFF
#define PDUR_J1939TP_CANCEL_RECEIVE                STD_OFF
#define PDUR_LINIF_CANCEL_RECEIVE                  STD_OFF
#define PDUR_DOIP_CANCEL_RECEIVE                   STD_OFF
#define PDUR_SOAD_CANCEL_RECEIVE                   STD_OFF
#define PDUR_SECOC_CANCEL_RECEIVE                  STD_OFF

#define PDUR_RX_INDICATION                         STD_ON 
#define PDUR_TX_CONFIRMATION                       STD_ON 
#define PDUR_TRIGGER_TRANSMIT                      STD_ON 
#define PDUR_COM_TRIGGER_TRANSMIT                  STD_ON 
#define PDUR_LDCOM_TRIGGER_TRANSMIT                STD_OFF 
#define PDUR_DLT_TRIGGER_TRANSMIT                  STD_OFF 
#define PDUR_SECOC_TRIGGER_TRANSMIT                STD_OFF 
#define PDUR_IPDUM_TRIGGER_TRANSMIT                STD_OFF 
#define PDUR_SOMEIPTP_TRIGGER_TRANSMIT             STD_OFF 

#define PDUR_TP_STARTOFRECEPTION_TRANSMIT     STD_ON 
#define PDUR_TP_COPYRXDATA                    STD_ON 
#define PDUR_TP_RXINDICATION                  STD_ON 
#define PDUR_TP_COPYTXDATA                    STD_ON 
#define PDUR_TP_TXCONFIRMATION                STD_ON 

#define PDUR_SRC_PDU_SUM           61u

#define PDUR_SRCPDU_COM_ECU_18FEF121          0u
#define PDUR_SRCPDU_COM_ECU_112          1u
#define PDUR_SRCPDU_COM_ECU_111          2u
#define PDUR_SRCPDU_COM_ECU_18FEF221          3u
#define PDUR_SRCPDU_COM_ECU_107          4u
#define PDUR_SRCPDU_COM_ECU_103          5u
#define PDUR_SRCPDU_COM_ECU_10F          6u
#define PDUR_SRCPDU_COM_ECU_11C_DLC16          7u
#define PDUR_SRCPDU_COM_ECU_115          8u
#define PDUR_SRCPDU_COM_ECU_105          9u
#define PDUR_SRCPDU_COM_ECU_114          10u
#define PDUR_SRCPDU_COM_ECU_104          11u
#define PDUR_SRCPDU_COM_ECU_110          12u
#define PDUR_SRCPDU_COM_ECU_100          13u
#define PDUR_SRCPDU_Tester_101          14u
#define PDUR_SRCPDU_Tester_117          15u
#define PDUR_SRCPDU_Tester_10A          16u
#define PDUR_SRCPDU_Tester_10E          17u
#define PDUR_SRCPDU_Tester_10C          18u
#define PDUR_SRCPDU_Tester_119          19u
#define PDUR_SRCPDU_Tester_109          20u
#define PDUR_SRCPDU_Tester_118          21u
#define PDUR_SRCPDU_Tester_116          22u
#define PDUR_SRCPDU_Tester_108          23u
#define PDUR_SRCPDU_Tester_106          24u
#define PDUR_SRCPDU_COM_ECU_179          25u
#define PDUR_SRCPDU_COM_ECU_135          26u
#define PDUR_SRCPDU_COM_ECU_133          27u
#define PDUR_SRCPDU_COM_ECU_12F          28u
#define PDUR_SRCPDU_COM_ECU_12D          29u
#define PDUR_SRCPDU_COM_ECU_12C          30u
#define PDUR_SRCPDU_COM_ECU_12B          31u
#define PDUR_SRCPDU_COM_ECU_121_DLC64          32u
#define PDUR_SRCPDU_Tester_178          33u
#define PDUR_SRCPDU_Tester_177          34u
#define PDUR_SRCPDU_Tester_18FF0141          35u
#define PDUR_SRCPDU_Tester_130          36u
#define PDUR_SRCPDU_Tester_129_DLC64          37u
#define PDUR_SRCPDU_Tester_124_DLC16          38u
#define PDUR_SRCPDU_COM_ECU_11A          39u
#define PDUR_SRCPDU_COM_ECU_11B          40u
#define PDUR_SRCPDU_COM_ECU_11D          41u
#define PDUR_SRCPDU_COM_ECU_11E          42u
#define PDUR_SRCPDU_Tester_11F          43u
#define PDUR_SRCPDU_Tester_18FF1131          44u
#define PDUR_SRCPDU_COM_ECU_122          45u
#define PDUR_SRCPDU_COM_ECU_123          46u
#define PDUR_SRCPDU_COM_ECU_125          47u
#define PDUR_SRCPDU_COM_ECU_136          48u
#define PDUR_SRCPDU_Tester_137          49u
#define PDUR_SRCPDU_DCM_UDS_TX          50u
#define PDUR_SRCPDU_CANTP_UDS_RX_PHY          51u
#define PDUR_SRCPDU_CANTP_UDS_RX_FUN          52u
#define PDUR_SRCPDU_DCM_2A          53u
#define PDUR_SRCPDU_SOAD_LDCOM_ETH_RX          54u
#define PDUR_SRCPDU_LDCOM_ETH_TX          55u
#define PDUR_SRCPDU_SoAd_Ecu_TcpServer_Rx_20020          56u
#define PDUR_SRCPDU_COM_ECU_201          57u
#define PDUR_SRCPDU_COM_ECU_211          58u
#define PDUR_SRCPDU_COM_ECU_221          59u
#define PDUR_SRCPDU_COM_ECU_231          60u

#define PDUR_DEST_PDU_SUM              65u

#define PDUR_DEST_GATEWAY_TP_PDU_SUM   0u

#define PDUR_DESTPDU_ECU_18FEF121          0u
#define PDUR_DESTPDU_SoAd_ECU_18FEF121          1u
#define PDUR_DESTPDU_ECU_112          2u
#define PDUR_DESTPDU_ECU_111          3u
#define PDUR_DESTPDU_ECU_18FEF221          4u
#define PDUR_DESTPDU_ECU_107          5u
#define PDUR_DESTPDU_ECU_103          6u
#define PDUR_DESTPDU_ECU_10F          7u
#define PDUR_DESTPDU_ECU_11C_DLC16          8u
#define PDUR_DESTPDU_ECU_115          9u
#define PDUR_DESTPDU_ECU_105          10u
#define PDUR_DESTPDU_ECU_114          11u
#define PDUR_DESTPDU_ECU_104          12u
#define PDUR_DESTPDU_ECU_110          13u
#define PDUR_DESTPDU_ECU_100          14u
#define PDUR_DESTPDU_COM_Tester_101          15u
#define PDUR_DESTPDU_COM_Tester_117          16u
#define PDUR_DESTPDU_COM_Tester_10A          17u
#define PDUR_DESTPDU_COM_Tester_10E          18u
#define PDUR_DESTPDU_ECU_10D          19u
#define PDUR_DESTPDU_ECU_134          20u
#define PDUR_DESTPDU_COM_Tester_119          21u
#define PDUR_DESTPDU_COM_Tester_109          22u
#define PDUR_DESTPDU_COM_Tester_118          23u
#define PDUR_DESTPDU_COM_Tester_116          24u
#define PDUR_DESTPDU_COM_Tester_108          25u
#define PDUR_DESTPDU_COM_Tester_106          26u
#define PDUR_DESTPDU_ECU_179          27u
#define PDUR_DESTPDU_ECU_135          28u
#define PDUR_DESTPDU_ECU_133          29u
#define PDUR_DESTPDU_ECU_12F          30u
#define PDUR_DESTPDU_ECU_12D          31u
#define PDUR_DESTPDU_ECU_12C          32u
#define PDUR_DESTPDU_ECU_12B          33u
#define PDUR_DESTPDU_ECU_121_DLC64          34u
#define PDUR_DESTPDU_COM_Tester_178          35u
#define PDUR_DESTPDU_COM_Tester_177          36u
#define PDUR_DESTPDU_COM_Tester_18FF0141          37u
#define PDUR_DESTPDU_ECU_102          38u
#define PDUR_DESTPDU_ECU_131          39u
#define PDUR_DESTPDU_COM_Tester_129_DLC64          40u
#define PDUR_DESTPDU_COM_Tester_124_DLC16          41u
#define PDUR_DESTPDU_ECU_11A          42u
#define PDUR_DESTPDU_ECU_11B          43u
#define PDUR_DESTPDU_ECU_11D          44u
#define PDUR_DESTPDU_ECU_11E          45u
#define PDUR_DESTPDU_COM_Tester_11F          46u
#define PDUR_DESTPDU_COM_Tester_18FF1131          47u
#define PDUR_DESTPDU_ECU_122          48u
#define PDUR_DESTPDU_ECU_123          49u
#define PDUR_DESTPDU_ECU_125          50u
#define PDUR_DESTPDU_ECU_136          51u
#define PDUR_DESTPDU_ECU_138          52u
#define PDUR_DESTPDU_COM_Tester_137          53u
#define PDUR_DESTPDU_CANTP_UDS_TX          54u
#define PDUR_DESTPDU_DCM_UDS_RX_PHY          55u
#define PDUR_DESTPDU_DCM_UDS_RX_FUN          56u
#define PDUR_DESTPDU_CANIF_2A          57u
#define PDUR_DESTPDU_LDCOM_ETH_RX          58u
#define PDUR_DESTPDU_SOAD_LDCOM_ETH_TX          59u
#define PDUR_DESTPDU_SoAd_Ecu_Gw_Tx_20020          60u
#define PDUR_DESTPDU_ECU_201          61u
#define PDUR_DESTPDU_ECU_211          62u
#define PDUR_DESTPDU_ECU_221          63u
#define PDUR_DESTPDU_ECU_231          64u

#define PDUR_ROUTING_PATH_GROUP_SUM        1u

#define PduRRoutingPathGroup_0          0u


#define PDUR_SRC_UP_MULTICAST_TX_IF_SUM              2u
#define PDUR_GATEWAY_DIRECT_BUFFER_PDU_SUM           0u


#define PDUR_TP_BUFFER_SUM           0u
#define PDUR_TX_BUFFER_SUM           0u


#define PDUR_DEFAULT_VALUE_LENGTH           0u
#define PDUR_DEFAULT_VALUE_PDU                  0u

#endif  /* end of PDUR_CFG_H */

/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/


