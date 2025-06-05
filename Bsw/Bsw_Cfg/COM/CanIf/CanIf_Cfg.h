
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
**  FILENAME    : CanIf_Cfg.h                                                 **
**                                                                            **
**  Created on  : 2022/06/02 13:43:03                                         **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : cfg parameter declaration of CanIf                          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
#ifndef CANIF_CFG_H
#define CANIF_CFG_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CANIF_CFG_H_AR_MAJOR_VERSION    4u
#define CANIF_CFG_H_AR_MINOR_VERSION    2u
#define CANIF_CFG_H_AR_PATCH_VERSION    2u
#define CANIF_CFG_H_SW_MAJOR_VERSION    2u
#define CANIF_CFG_H_SW_MINOR_VERSION    0u
#define CANIF_CFG_H_SW_PATCH_VERSION    0u

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/


/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/
#define CANIF_SOFTWARE_FILTER_BINARY                0u
#define CANIF_SOFTWARE_FILTER_INDEX                 1u
#define CANIF_SOFTWARE_FILTER_LINEAR                2u
#define CANIF_SOFTWARE_FILTER_TABLE                 3u


#define CANIF_FIXED_BUFFER            STD_ON

#define CANIF_PRIVATE_DLC_CHECK       STD_ON

#define CANIF_SOFTWARE_FILTER_TYPE    CANIF_SOFTWARE_FILTER_LINEAR

#define CANIF_SUPPORT_TTCAN           STD_OFF

#define CANIF_META_DATA_SUPPORT       STD_OFF

#define CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT               STD_OFF
  
#define CANIF_PUBLIC_DEV_ERROR_DETECT                      STD_ON

#define CANIF_PUBLIC_ICOM_SUPPORT                          STD_OFF

#define CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT                  STD_ON

#define CANIF_PUBLIC_PN_SUPPORT                            STD_OFF

#define CANIF_PUBLIC_READ_RX_PDU_DATA_API                  STD_OFF

#define CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API         STD_OFF

#define CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API         STD_OFF

#define CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API                 STD_ON

#define CANIF_PUBLIC_TX_BUFFERING                          STD_ON

#define CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT            STD_ON

#define CANIF_PUBLIC_VERSION_INFO_API                      STD_ON

#define CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM              STD_OFF

#define CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT            STD_OFF

#define CANIF_SET_BAUDRATE_API                             STD_OFF

#define CANIF_TRIGGER_TRANSMIT_SUPPORT            STD_OFF

#define CANIF_TX_OFFLINE_ACTIVE_SUPPORT           STD_OFF

#define CANIF_WAKE_UP_SUPPORT                     STD_OFF

#define CANIF_CANDRV_WAKE_UP_SUPPORT              STD_OFF

#define CANIF_CANTRCV_WAKE_UP_SUPPORT             STD_OFF

#define CANIF_WAKEUPSOURCE_MAX                    0x3Fu
/*******************************************************************************************/

#define CANIF_RXPDU_NUMBER              27u
#define CANIF_RXPDU_Tester_178         0u
#define CANIF_RXPDU_Tester_177         1u
#define CANIF_RXPDU_Tester_101         2u
#define CANIF_RXPDU_Tester_117         3u
#define CANIF_RXPDU_Tester_10A         4u
#define CANIF_RXPDU_Tester_18FF0141         5u
#define CANIF_RXPDU_Tester_130         6u
#define CANIF_RXPDU_Tester_10E         7u
#define CANIF_RXPDU_Tester_10C         8u
#define CANIF_RXPDU_Tester_129_DLC64         9u
#define CANIF_RXPDU_Tester_124_DLC16         10u
#define CANIF_RXPDU_Tester_119         11u
#define CANIF_RXPDU_Tester_109         12u
#define CANIF_RXPDU_Tester_118         13u
#define CANIF_RXPDU_Tester_116         14u
#define CANIF_RXPDU_Tester_108         15u
#define CANIF_RXPDU_Tester_106         16u
#define CANIF_RXPDU_Tester_11F         17u
#define CANIF_RXPDU_Tester_18FF1131         18u
#define CANIF_RXPDU_Tester_137         19u
#define CANIF_RXPDU_CANIF_UDS_RX_PHY         20u
#define CANIF_RXPDU_CANIF_UDS_RX_FUN         21u
#define CANIF_RXPDU_Xcp_Rx_0x600         22u
#define CANIF_RXPDU_CanNm_Rx_0x400_ch0         23u
#define CANIF_RXPDU_CanNm_Rx_0x400_ch1         24u
#define CANIF_RXPDU_CanNm_Rx_0x400_ch2         25u
#define CANIF_RXPDU_CanNm_Rx_0x400_ch3         26u

#define CANIF_TXPDU_NUMBER              46u
#define CANIF_DYNAMIC_TXPDU_NUMBER      1u
#define CANIF_TXPDU_ECU_179         0u
#define CANIF_TXPDU_ECU_18FEF121         1u
#define CANIF_TXPDU_ECU_112         2u
#define CANIF_TXPDU_ECU_111         3u
#define CANIF_TXPDU_ECU_18FEF221         4u
#define CANIF_TXPDU_ECU_107         5u
#define CANIF_TXPDU_ECU_103         6u
#define CANIF_TXPDU_ECU_135         7u
#define CANIF_TXPDU_ECU_102         8u
#define CANIF_TXPDU_ECU_134         9u
#define CANIF_TXPDU_ECU_133         10u
#define CANIF_TXPDU_ECU_131         11u
#define CANIF_TXPDU_ECU_10F         12u
#define CANIF_TXPDU_ECU_10D         13u
#define CANIF_TXPDU_ECU_12F         14u
#define CANIF_TXPDU_ECU_12D         15u
#define CANIF_TXPDU_ECU_12C         16u
#define CANIF_TXPDU_ECU_12B         17u
#define CANIF_TXPDU_ECU_121_DLC64         18u
#define CANIF_TXPDU_ECU_11C_DLC16         19u
#define CANIF_TXPDU_ECU_115         20u
#define CANIF_TXPDU_ECU_105         21u
#define CANIF_TXPDU_ECU_114         22u
#define CANIF_TXPDU_ECU_104         23u
#define CANIF_TXPDU_ECU_110         24u
#define CANIF_TXPDU_ECU_100         25u
#define CANIF_TXPDU_ECU_11A         26u
#define CANIF_TXPDU_ECU_11B         27u
#define CANIF_TXPDU_ECU_11D         28u
#define CANIF_TXPDU_ECU_11E         29u
#define CANIF_TXPDU_ECU_122         30u
#define CANIF_TXPDU_ECU_123         31u
#define CANIF_TXPDU_ECU_125         32u
#define CANIF_TXPDU_ECU_136         33u
#define CANIF_TXPDU_ECU_138         34u
#define CANIF_TXPDU_CANIF_UDS_TX         35u
#define CANIF_TXPDU_CANIF_2A         36u
#define CANIF_TXPDU_Xcp_Tx_0x601         37u
#define CANIF_TXPDU_CanNm_Tx_0x410         38u
#define CANIF_TXPDU_CanNm_Tx_0x411         39u
#define CANIF_TXPDU_CanNm_Tx_0x412         40u
#define CANIF_TXPDU_CanNm_Tx_0x413         41u
#define CANIF_TXPDU_ECU_201         42u
#define CANIF_TXPDU_ECU_211         43u
#define CANIF_TXPDU_ECU_221         44u
#define CANIF_TXPDU_ECU_231         45u

#define CANIF_HRH_NUMBER           27u
#define CANIF_HOH0_HRH_0           0u
#define CANIF_HOH0_HRH_1           1u
#define CANIF_HOH0_HRH_2           2u
#define CANIF_HOH0_HRH_3           3u
#define CANIF_HOH0_HRH_4           4u
#define CANIF_HOH0_HRH_5           5u
#define CANIF_HOH0_HRH_6           6u
#define CANIF_HOH0_HRH_7           7u
#define CANIF_HOH0_HRH_8           8u
#define CANIF_HOH0_HRH_9           9u
#define CANIF_HOH0_HRH_10           10u
#define CANIF_HOH0_HRH_11           11u
#define CANIF_HOH0_HRH_12           12u
#define CANIF_HOH0_HRH_13           13u
#define CANIF_HOH0_HRH_14           14u
#define CANIF_HOH0_HRH_15           15u
#define CANIF_HOH0_HRH_16           16u
#define CANIF_HOH0_HRH_17           17u
#define CANIF_HOH0_HRH_18           18u
#define CANIF_HOH0_HRH_19           19u
#define CANIF_HOH0_HRH_20           20u
#define CANIF_HOH0_HRH_21           21u
#define CANIF_HOH0_HRH_22           22u
#define CANIF_HOH0_HRH_23           23u
#define CANIF_HOH0_HRH_24           24u
#define CANIF_HOH0_HRH_25           25u
#define CANIF_HOH0_HRH_26           26u

#define CANIF_HTH_NUMBER           46u
#define CANIF_HOH0_HTH_0           0u
#define CANIF_HOH0_HTH_1           1u
#define CANIF_HOH0_HTH_2           2u
#define CANIF_HOH0_HTH_3           3u
#define CANIF_HOH0_HTH_4           4u
#define CANIF_HOH0_HTH_5           5u
#define CANIF_HOH0_HTH_6           6u
#define CANIF_HOH0_HTH_7           7u
#define CANIF_HOH0_HTH_8           8u
#define CANIF_HOH0_HTH_9           9u
#define CANIF_HOH0_HTH_10           10u
#define CANIF_HOH0_HTH_11           11u
#define CANIF_HOH0_HTH_12           12u
#define CANIF_HOH0_HTH_13           13u
#define CANIF_HOH0_HTH_14           14u
#define CANIF_HOH0_HTH_15           15u
#define CANIF_HOH0_HTH_16           16u
#define CANIF_HOH0_HTH_17           17u
#define CANIF_HOH0_HTH_18           18u
#define CANIF_HOH0_HTH_19           19u
#define CANIF_HOH0_HTH_20           20u
#define CANIF_HOH0_HTH_21           21u
#define CANIF_HOH0_HTH_22           22u
#define CANIF_HOH0_HTH_23           23u
#define CANIF_HOH0_HTH_24           24u
#define CANIF_HOH0_HTH_25           25u
#define CANIF_HOH0_HTH_26           26u
#define CANIF_HOH0_HTH_27           27u
#define CANIF_HOH0_HTH_28           28u
#define CANIF_HOH0_HTH_29           29u
#define CANIF_HOH0_HTH_30           30u
#define CANIF_HOH0_HTH_31           31u
#define CANIF_HOH0_HTH_32           32u
#define CANIF_HOH0_HTH_33           33u
#define CANIF_HOH0_HTH_34           34u
#define CANIF_HOH0_HTH_35           35u
#define CANIF_HOH0_HTH_36           36u
#define CANIF_HOH0_HTH_37           37u
#define CANIF_HOH0_HTH_38           38u
#define CANIF_HOH0_HTH_39           39u
#define CANIF_HOH0_HTH_40           40u
#define CANIF_HOH0_HTH_41           41u
#define CANIF_HOH0_HTH_42           42u
#define CANIF_HOH0_HTH_43           43u
#define CANIF_HOH0_HTH_44           44u
#define CANIF_HOH0_HTH_45           45u


#define CANIF_TXBUFFER_NUMBER          5u
#define CANIF_RXBUFFER_NUMBER          0u

#define CANIF_RXNOTIFYSTATUS_BUFFER    0u
#define CANIF_TXNOTIFYSTATUS_BUFFER    0u

#define CANIF_CANDRIVER_NUMBER      1u
#define CANIF_CAN    0u
#define CANIF_CANCONTROLLER_NUMBER      8u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_0    0u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_1    1u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_2    2u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_3    3u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_4    4u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_5    5u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_6    6u
#define CANIF_CANDRV_0_CANIF_CTR_DRV_CanController_7    7u

#define CANIF_TRCVDRIVER_NUMBER      0u
#define CANIF_TRCV_NUMBER      0u

#endif
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
