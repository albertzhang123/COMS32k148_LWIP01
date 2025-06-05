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
**  FILENAME    : Com_Cfg.h                                                   **
**                                                                            **
**  Created on  : 2022/06/02 13:43:03                                         **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Configuration parameter of Com                              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#ifndef  COM_CFG_H
#define  COM_CFG_H

#include "ComStack_Types.h"
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
#define COM_CFG_H_AR_MAJOR_VERSION    4u
#define COM_CFG_H_AR_MINOR_VERSION    2u
#define COM_CFG_H_AR_PATCH_VERSION    2u
#define COM_CFG_H_SW_MAJOR_VERSION    2u
#define COM_CFG_H_SW_MINOR_VERSION    0u
#define COM_CFG_H_SW_PATCH_VERSION    0u

/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/

#define COM_CANCELLATION_SUPPORT                     STD_OFF
#define COM_DEV_ERROR_DETECT                         STD_OFF
#define COM_ENABLE_SIGNAL_GROUP_ARRAY_API            STD_OFF
#define COM_VERSION_INFO_API                         STD_OFF
#define COM_ENABLE_MDT_FOR_CYCLIC_TRANSMISSION       STD_ON
#define COM_METADATA_SUPPORT                         STD_OFF
#define COM_RETRY_FAILED_TRANSMIT_REQUESTS           STD_OFF

#define COM_IPDUGROUP_NUMBER                         4u
#define Com_PduGroup_CONTROLLER_0        0u
#define Com_PduGroup_CONTROLLER_1        1u
#define Com_PduGroup_CONTROLLER_2        2u
#define Com_PduGroup_CONTROLLER_3        3u	

#define COM_RXIPDU_NUMBER        18u
#define COM_RXPDU_COM_Tester_101        0u
#define COM_RXPDU_COM_Tester_117        1u
#define COM_RXPDU_COM_Tester_10A        2u
#define COM_RXPDU_COM_Tester_10E        3u
#define COM_RXPDU_COM_Tester_119        4u
#define COM_RXPDU_COM_Tester_109        5u
#define COM_RXPDU_COM_Tester_118        6u
#define COM_RXPDU_COM_Tester_116        7u
#define COM_RXPDU_COM_Tester_108        8u
#define COM_RXPDU_COM_Tester_106        9u
#define COM_RXPDU_COM_Tester_178        10u
#define COM_RXPDU_COM_Tester_177        11u
#define COM_RXPDU_COM_Tester_18FF0141        12u
#define COM_RXPDU_COM_Tester_129_DLC64        13u
#define COM_RXPDU_COM_Tester_124_DLC16        14u
#define COM_RXPDU_COM_Tester_11F        15u
#define COM_RXPDU_COM_Tester_18FF1131        16u
#define COM_RXPDU_COM_Tester_137        17u

#define COM_TXIPDU_NUMBER    34u
#define COM_TXPDU_COM_ECU_18FEF121        18u
#define COM_TXPDU_COM_ECU_112        19u
#define COM_TXPDU_COM_ECU_111        20u
#define COM_TXPDU_COM_ECU_18FEF221        21u
#define COM_TXPDU_COM_ECU_107        22u
#define COM_TXPDU_COM_ECU_103        23u
#define COM_TXPDU_COM_ECU_10F        24u
#define COM_TXPDU_COM_ECU_11C_DLC16        25u
#define COM_TXPDU_COM_ECU_115        26u
#define COM_TXPDU_COM_ECU_105        27u
#define COM_TXPDU_COM_ECU_114        28u
#define COM_TXPDU_COM_ECU_104        29u
#define COM_TXPDU_COM_ECU_110        30u
#define COM_TXPDU_COM_ECU_100        31u
#define COM_TXPDU_COM_ECU_179        32u
#define COM_TXPDU_COM_ECU_135        33u
#define COM_TXPDU_COM_ECU_133        34u
#define COM_TXPDU_COM_ECU_12F        35u
#define COM_TXPDU_COM_ECU_12D        36u
#define COM_TXPDU_COM_ECU_12C        37u
#define COM_TXPDU_COM_ECU_12B        38u
#define COM_TXPDU_COM_ECU_121_DLC64        39u
#define COM_TXPDU_COM_ECU_11A        40u
#define COM_TXPDU_COM_ECU_11B        41u
#define COM_TXPDU_COM_ECU_11D        42u
#define COM_TXPDU_COM_ECU_11E        43u
#define COM_TXPDU_COM_ECU_125        44u
#define COM_TXPDU_COM_ECU_122        45u
#define COM_TXPDU_COM_ECU_123        46u
#define COM_TXPDU_COM_ECU_136        47u
#define COM_TXPDU_COM_ECU_201        48u
#define COM_TXPDU_COM_ECU_211        49u
#define COM_TXPDU_COM_ECU_221        50u
#define COM_TXPDU_COM_ECU_231        51u
#define COM_RXIPDUBUFF_SIZE        216u
#define COM_TXIPDUBUFF_SIZE        524u

#define COM_SIGNAL_BOOLBUFF_SIZE            1u
#define COM_RXGROUPSIGNAL_BOOLBUFF_SIZE     0u
#define COM_SIGNAL_BOOL_INVALID_SIZE        0u

#define COM_SIGNAL_8BITBUFF_SIZE            117u
#define COM_RXGROUPSIGNAL_8BITBUFF_SIZE     4u
#define COM_SIGNAL_8BIT_INVALID_SIZE        2u

#define COM_SIGNAL_16BITBUFF_SIZE           5u
#define COM_RXGROUPSIGNAL_16BITBUFF_SIZE    0u
#define COM_SIGNAL_16BIT_INVALID_SIZE       0u

#define COM_SIGNAL_32BITBUFF_SIZE           3u
#define COM_RXGROUPSIGNAL_32BITBUFF_SIZE    0u
#define COM_SIGNAL_32BIT_INVALID_SIZE       0u

#define COM_SIGNAL_64BITBUFF_SIZE           5u
#define COM_RXGROUPSIGNAL_64BITBUFF_SIZE    0u
#define COM_SIGNAL_64BIT_INVALID_SIZE       0u

#define COM_TMCTXSIGNAL_NUMBER              2u
#define COM_TMCTXGROUPSIGNAL_NUMBER         0u

#define COM_TXSIGNALGROUP_NUMBER            2u
#define ECU_136_Signal_Group01               0u
#define ECU_136_Signal_Group02               1u

#define COM_RXSIGNALGROUP_NUMBER            2u
#define ECU_137_Signal_Group01               0u
#define ECU_137_Signal_Group02               1u

#define COM_TXSIGNAL_NUMBER                  54u
#define BCM1_stParkBrkLSgn_GW_CCVS_18FEF121               0u
#define Signal_112_IPDU_COM_ECU_112               1u
#define Signal_111_IPDU_COM_ECU_111               2u
#define BCM1_stParkBrkLSgn_GW_CCVS_FD_18FEF221               3u
#define Signal_107_IPDU_COM_ECU_107               4u
#define Signal_103_IPDU_COM_ECU_103               5u
#define Signal_10F_IPDU_COM_ECU_10F               6u
#define Signal_11C_IPDU_COM_ECU_11C_DLC16               7u
#define Signal_115_IPDU_COM_ECU_115               8u
#define Signal_105_IPDU_COM_ECU_105               9u
#define Signal_114_IPDU_COM_ECU_114               10u
#define Signal_104_IPDU_COM_ECU_104               11u
#define Signal_110_3_IPDU_COM_ECU_110               12u
#define Signal_110_2_IPDU_COM_ECU_110               13u
#define Signal_110_1_IPDU_COM_ECU_110               14u
#define Signal_110_0_IPDU_COM_ECU_110               15u
#define Signal_100_3_IPDU_COM_ECU_100               16u
#define Signal_100_2_IPDU_COM_ECU_100               17u
#define Signal_100_1_IPDU_COM_ECU_100               18u
#define Signal_100_0_IPDU_COM_ECU_100               19u
#define Signal_179_IPDU_COM_ECU_179               20u
#define Signal_135_IPDU_COM_ECU_135               21u
#define Signal_133_1_IPDU_COM_ECU_133               22u
#define Signal_133_0_IPDU_COM_ECU_133               23u
#define Signal_12F_IPDU_COM_ECU_12F               24u
#define Signal_12D_IPDU_COM_ECU_12D               25u
#define Signal_12C_IPDU_COM_ECU_12C               26u
#define Signal_12B_IPDU_COM_ECU_12B               27u
#define Signal_121_IPDU_COM_ECU_121_DLC64               28u
#define Signal_11A_IPDU_COM_ECU_11A               29u
#define Signal_11B_IPDU_COM_ECU_11B               30u
#define Signal_11D_IPDU_COM_ECU_11D               31u
#define Signal_11E_IPDU_COM_ECU_11E               32u
#define Signal_125_IPDU_COM_ECU_125               33u
#define Signal_122_Bit0_Bool_IPDU_COM_ECU_122               34u
#define Signal_122_Bit32_Float32_IPDU_COM_ECU_122               35u
#define Signal_122_Bit64_Float64_IPDU_COM_ECU_122               36u
#define Signal_122_Bit16_Sint16_IPDU_COM_ECU_122               37u
#define Signal_122_Bit32_Sint32_IPDU_COM_ECU_122               38u
#define Signal_122_Bit64_Sint64_IPDU_COM_ECU_122               39u
#define Signal_122_Bit8_Sint8_IPDU_COM_ECU_122               40u
#define Signal_122_Bit16_Uint16_IPDU_COM_ECU_122               41u
#define Signal_122_Bit16_Uint32_IPDU_COM_ECU_122               42u
#define Signal_123_Bit64_Uint64_IPDU_COM_ECU_123               43u
#define Signal_123_Bit8_Uint8_DYN_IPDU_COM_ECU_123               44u
#define Signal_123_Bit8_Uint8_N_IPDU_COM_ECU_123               45u
#define BCM2_vVehSpd_GW_CCVS_18FEF121               46u
#define BCM1_stRunBrkLSgn_GW_CCVS_18FEF121               47u
#define BCM2_vVehSpd_GW_CCVS_FD_18FEF221               48u
#define BCM1_stRunBrkLSgn_GW_CCVS_FD_18FEF221               49u
#define Signal_201_IPDU_COM_ECU_201               50u
#define Signal_211_IPDU_COM_ECU_211               51u
#define Signal_221_IPDU_COM_ECU_221               52u
#define Signal_231_IPDU_COM_ECU_231               53u

#define COM_RXSIGNAL_NUMBER               27u
#define Signal_101_1_IPDU_COM_Tester_101             0u
#define Signal_101_0_IPDU_COM_Tester_101             1u
#define Signal_117_IPDU_COM_Tester_117             2u
#define Signal_10A_IPDU_COM_Tester_10A             3u
#define Signal_10E_1_IPDU_COM_Tester_10E             4u
#define Signal_10E_0_IPDU_COM_Tester_10E             5u
#define Signal_119_byte0_IPDU_COM_Tester_119             6u
#define Signal_109_byte0_IPDU_COM_Tester_109             7u
#define Signal_118_IPDU_COM_Tester_118             8u
#define Signal_116_IPDU_COM_Tester_116             9u
#define Signal_108_IPDU_COM_Tester_108             10u
#define Signal_106_IPDU_COM_Tester_106             11u
#define Signal_178_IPDU_COM_Tester_178             12u
#define Signal_177_byte7_IPDU_COM_Tester_177             13u
#define Signal_177_byte6_IPDU_COM_Tester_177             14u
#define Signal_177_byte5_IPDU_COM_Tester_177             15u
#define Signal_177_byte4_IPDU_COM_Tester_177             16u
#define Signal_177_byte3_IPDU_COM_Tester_177             17u
#define Signal_177_byte2_IPDU_COM_Tester_177             18u
#define Signal_177_byte1_IPDU_COM_Tester_177             19u
#define Signal_132_1_IPDU_COM_Tester_132             20u
#define BCM2_vVehSpd_BCM2_VEL_18FF0141             21u
#define Signal_129_IPDU_COM_Tester_129_DLC64             22u
#define Signal_124_IPDU_COM_Tester_124_DLC16             23u
#define Signal_11F_IPDU_COM_Tester_11F             24u
#define BCM1_stParkBrkLSgn_BCM1_IO1_18FF1131             25u
#define BCM1_stRunBrkLSgn_BCM1_IO1_18FF1131             26u	

#define COM_GWSOURCESIGNAL_UPDATE_NUMBER           0u
#define COM_ONEEVERYNFILTERSIGNAL_NUMBER           0u
#define COM_RXSIGNALMASKNEWDIFFERMASKOLD_NUMBER    0u

#define COM_GWSIGNAL_BOOLBUFF_SIZE          0u
#define COM_GWSIGNAL_8BITBUFF_SIZE          4u
#define COM_GWSIGNAL_16BITBUFF_SIZE         0u
#define COM_GWSIGNAL_32BITBUFF_SIZE         0u
#define COM_GWSIGNAL_64BITBUFF_SIZE         0u

#define COM_TXGROUPSIGNAL_NUMBER        4u	
#define ECU_136_Signal_Group01_Signal_136_00             54u
#define ECU_136_Signal_Group01_Signal_136_01             55u
#define ECU_136_Signal_Group02_Signal_136_10             56u
#define ECU_136_Signal_Group02_Signal_136_11             57u

#define COM_RXGROUPSIGNAL_NUMBER        4u
#define ECU_137_Signal_Group01_Signal_137_00             27u
#define ECU_137_Signal_Group01_Signal_137_01             28u
#define ECU_137_Signal_Group02_Signal_137_10             29u
#define ECU_137_Signal_Group02_Signal_137_11             30u

#define COM_GWMAPPING_NUMBER         4u
#define COM_SOURCE_SIGNAL_NUMBER     4u
#define COM_DST_SIGNAL_NUMBER        7u


#define COM_TXTPPDU_SUPPORT          STD_OFF
#define COM_RXTPPDU_SUPPORT          STD_OFF

#endif

