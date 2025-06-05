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
**  FILENAME    : PduR_Cfg.c                                                  **
**                                                                            **
**  Created on  : 2022/06/02 13:43:03                                         **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Configuration parameter of PDUR                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "PduR.h"
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
#define PDUR_CFG_C_AR_MAJOR_VERSION  4u
#define PDUR_CFG_C_AR_MINOR_VERSION  2u
#define PDUR_CFG_C_AR_PATCH_VERSION  2u



/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
#if (PDUR_CFG_C_AR_MAJOR_VERSION != PDUR_CFG_H_AR_MAJOR_VERSION)
   #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_CFG_C_AR_MINOR_VERSION != PDUR_CFG_H_AR_MINOR_VERSION)
   #error "PduR.c : Mismatch in Specification Major Version"
#endif
#if (PDUR_CFG_C_AR_PATCH_VERSION != PDUR_CFG_H_AR_PATCH_VERSION)
   #error "PduR.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRBswModuleType,PDUR_CONST) 
PduR_BswModuleConfigData[PDUR_BSW_MODULE_SUM] =
{
    {
        PDUR_CANIF,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE
    },
    {
        PDUR_COM,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        TRUE,
        TRUE,
        TRUE,
        TRUE
    },
    {
        PDUR_CANTP,
        FALSE,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        FALSE,
        TRUE
    },
    {
        PDUR_DCM,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        TRUE
    },
    {
        PDUR_SOAD,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        FALSE,
        TRUE,
        TRUE
    },
    {
        PDUR_LDCOM,
        FALSE,
        FALSE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        TRUE,
        FALSE,
        TRUE,
        TRUE
    },
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRDestPduType,PDUR_CONST)
PduR_DestPduConfigData[PDUR_DEST_PDU_SUM] =
{
    { 
        PDUR_DESTPDU_ECU_18FEF121,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_18FEF121,
        CANIF_TXPDU_ECU_18FEF121,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_SoAd_ECU_18FEF121,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_18FEF121,
        SOAD_SoAd_ECU_18FEF121,
        PDUR_SOAD,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_112,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_112,
        CANIF_TXPDU_ECU_112,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_111,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_111,
        CANIF_TXPDU_ECU_111,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_18FEF221,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_18FEF221,
        CANIF_TXPDU_ECU_18FEF221,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_107,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_107,
        CANIF_TXPDU_ECU_107,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_103,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_103,
        CANIF_TXPDU_ECU_103,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_10F,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_10F,
        CANIF_TXPDU_ECU_10F,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_11C_DLC16,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_11C_DLC16,
        CANIF_TXPDU_ECU_11C_DLC16,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_115,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_115,
        CANIF_TXPDU_ECU_115,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_105,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_105,
        CANIF_TXPDU_ECU_105,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_114,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_114,
        CANIF_TXPDU_ECU_114,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_104,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_104,
        CANIF_TXPDU_ECU_104,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_110,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_110,
        CANIF_TXPDU_ECU_110,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_100,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_100,
        CANIF_TXPDU_ECU_100,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_101,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_101,
        COM_RXPDU_COM_Tester_101,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_117,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_117,
        COM_RXPDU_COM_Tester_117,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_10A,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_10A,
        COM_RXPDU_COM_Tester_10A,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_10E,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_10E,
        COM_RXPDU_COM_Tester_10E,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_10D,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_10C,
        CANIF_TXPDU_ECU_10D,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_134,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_10C,
        CANIF_TXPDU_ECU_134,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_119,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_119,
        COM_RXPDU_COM_Tester_119,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_109,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_109,
        COM_RXPDU_COM_Tester_109,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_118,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_118,
        COM_RXPDU_COM_Tester_118,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_116,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_116,
        COM_RXPDU_COM_Tester_116,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_108,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_108,
        COM_RXPDU_COM_Tester_108,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_106,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_106,
        COM_RXPDU_COM_Tester_106,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_179,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_179,
        CANIF_TXPDU_ECU_179,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_135,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_135,
        CANIF_TXPDU_ECU_135,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_133,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_133,
        CANIF_TXPDU_ECU_133,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_12F,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_12F,
        CANIF_TXPDU_ECU_12F,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_12D,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_12D,
        CANIF_TXPDU_ECU_12D,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_12C,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_12C,
        CANIF_TXPDU_ECU_12C,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_12B,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_12B,
        CANIF_TXPDU_ECU_12B,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_121_DLC64,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_121_DLC64,
        CANIF_TXPDU_ECU_121_DLC64,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_178,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_178,
        COM_RXPDU_COM_Tester_178,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_177,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_177,
        COM_RXPDU_COM_Tester_177,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_18FF0141,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_18FF0141,
        COM_RXPDU_COM_Tester_18FF0141,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_102,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_130,
        CANIF_TXPDU_ECU_102,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_131,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_130,
        CANIF_TXPDU_ECU_131,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_129_DLC64,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_129_DLC64,
        COM_RXPDU_COM_Tester_129_DLC64,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_124_DLC16,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_124_DLC16,
        COM_RXPDU_COM_Tester_124_DLC16,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_11A,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_11A,
        CANIF_TXPDU_ECU_11A,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_11B,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_11B,
        CANIF_TXPDU_ECU_11B,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_11D,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_11D,
        CANIF_TXPDU_ECU_11D,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_11E,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_11E,
        CANIF_TXPDU_ECU_11E,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_11F,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_11F,
        COM_RXPDU_COM_Tester_11F,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_18FF1131,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_18FF1131,
        COM_RXPDU_COM_Tester_18FF1131,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_122,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_122,
        CANIF_TXPDU_ECU_122,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_123,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_123,
        CANIF_TXPDU_ECU_123,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_125,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_125,
        CANIF_TXPDU_ECU_125,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_136,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_136,
        CANIF_TXPDU_ECU_136,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_138,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_136,
        CANIF_TXPDU_ECU_138,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_COM_Tester_137,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_Tester_137,
        COM_RXPDU_COM_Tester_137,
        PDUR_COM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_CANTP_UDS_TX,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_DCM_UDS_TX,
        CANTP_CANTP_UDS_TX,
        PDUR_CANTP,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_DCM_UDS_RX_PHY,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_CANTP_UDS_RX_PHY,
        DCM_DCM_UDS_RX_PHY,
        PDUR_DCM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_DCM_UDS_RX_FUN,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_CANTP_UDS_RX_FUN,
        DCM_DCM_UDS_RX_FUN,
        PDUR_DCM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_CANIF_2A,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_DCM_2A,
        CANIF_TXPDU_CANIF_2A,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_LDCOM_ETH_RX,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_SOAD_LDCOM_ETH_RX,
        LdComIPdu_EthUdpRx,
        PDUR_LDCOM,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_SOAD_LDCOM_ETH_TX,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_LDCOM_ETH_TX,
        SOAD_SOAD_LDCOM_ETH_TX,
        PDUR_SOAD,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_SoAd_Ecu_Gw_Tx_20020,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_SoAd_Ecu_TcpServer_Rx_20020,
        SOAD_SoAd_Ecu_Gw_Tx_20020,
        PDUR_SOAD,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_201,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_201,
        CANIF_TXPDU_ECU_201,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_211,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_211,
        CANIF_TXPDU_ECU_211,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_221,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_221,
        CANIF_TXPDU_ECU_221,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
    { 
        PDUR_DESTPDU_ECU_231,
        PDUR_DIRECT,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        PDUR_UNUSED_UINT16,
        TRUE,
        PDUR_SRCPDU_COM_ECU_231,
        CANIF_TXPDU_ECU_231,
        PDUR_CANIF,
        PDUR_UNUSED_UINT16,
        NULL_PTR,
        
    },
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRSrcPduType,PDUR_CONST)
PduR_SrcPduConfigData[PDUR_SRC_PDU_SUM] =
{
    {
        PDUR_SRCPDU_COM_ECU_18FEF121,
        TRUE,
        0u,
        COM_TXPDU_COM_ECU_18FEF121,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_112,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_112,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_111,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_111,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_18FEF221,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_18FEF221,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_107,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_107,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_103,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_103,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_10F,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_10F,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_11C_DLC16,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_11C_DLC16,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_115,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_115,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_105,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_105,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_114,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_114,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_104,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_104,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_110,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_110,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_100,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_100,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_Tester_101,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_101,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_117,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_117,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_10A,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_10A,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_10E,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_10E,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_10C,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_10C,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_119,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_119,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_109,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_109,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_118,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_118,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_116,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_116,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_108,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_108,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_106,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_106,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_179,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_179,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_135,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_135,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_133,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_133,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_12F,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_12F,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_12D,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_12D,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_12C,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_12C,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_12B,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_12B,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_121_DLC64,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_121_DLC64,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_Tester_178,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_178,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_177,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_177,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_18FF0141,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_18FF0141,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_130,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_130,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_129_DLC64,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_129_DLC64,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_124_DLC16,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_124_DLC16,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_11A,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_11A,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_11B,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_11B,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_11D,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_11D,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_11E,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_11E,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_Tester_11F,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_11F,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_Tester_18FF1131,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_18FF1131,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_122,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_122,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_123,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_123,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_125,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_125,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_136,
        TRUE,
        1u,
        COM_TXPDU_COM_ECU_136,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_Tester_137,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANIF_RXPDU_Tester_137,
        PDUR_CANIF,
        
    },
    {
        PDUR_SRCPDU_DCM_UDS_TX,
        TRUE,
        PDUR_UNUSED_UINT16,
        DCM_DCM_UDS_TX,
        PDUR_DCM,
        
    },
    {
        PDUR_SRCPDU_CANTP_UDS_RX_PHY,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANTP_CANTP_UDS_RX_PHY,
        PDUR_CANTP,
        
    },
    {
        PDUR_SRCPDU_CANTP_UDS_RX_FUN,
        TRUE,
        PDUR_UNUSED_UINT16,
        CANTP_CANTP_UDS_RX_FUN,
        PDUR_CANTP,
        
    },
    {
        PDUR_SRCPDU_DCM_2A,
        TRUE,
        PDUR_UNUSED_UINT16,
        DCM_DCM_2A,
        PDUR_DCM,
        
    },
    {
        PDUR_SRCPDU_SOAD_LDCOM_ETH_RX,
        TRUE,
        PDUR_UNUSED_UINT16,
        SOAD_SOAD_LDCOM_ETH_RX,
        PDUR_SOAD,
        
    },
    {
        PDUR_SRCPDU_LDCOM_ETH_TX,
        TRUE,
        PDUR_UNUSED_UINT16,
        LdComIPdu_EthUdpTx,
        PDUR_LDCOM,
        
    },
    {
        PDUR_SRCPDU_SoAd_Ecu_TcpServer_Rx_20020,
        TRUE,
        PDUR_UNUSED_UINT16,
        SOAD_SoAd_Ecu_TcpServer_Rx_20020,
        PDUR_SOAD,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_201,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_201,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_211,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_211,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_221,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_221,
        PDUR_COM,
        
    },
    {
        PDUR_SRCPDU_COM_ECU_231,
        TRUE,
        PDUR_UNUSED_UINT16,
        COM_TXPDU_COM_ECU_231,
        PDUR_COM,
        
    },
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_18FEF121[2] = 
{
    PDUR_DESTPDU_ECU_18FEF121,
    PDUR_DESTPDU_SoAd_ECU_18FEF121,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_112[1] = 
{
    PDUR_DESTPDU_ECU_112,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_111[1] = 
{
    PDUR_DESTPDU_ECU_111,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_18FEF221[1] = 
{
    PDUR_DESTPDU_ECU_18FEF221,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_107[1] = 
{
    PDUR_DESTPDU_ECU_107,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_103[1] = 
{
    PDUR_DESTPDU_ECU_103,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_10F[1] = 
{
    PDUR_DESTPDU_ECU_10F,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_11C_DLC16[1] = 
{
    PDUR_DESTPDU_ECU_11C_DLC16,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_115[1] = 
{
    PDUR_DESTPDU_ECU_115,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_105[1] = 
{
    PDUR_DESTPDU_ECU_105,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_114[1] = 
{
    PDUR_DESTPDU_ECU_114,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_104[1] = 
{
    PDUR_DESTPDU_ECU_104,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_110[1] = 
{
    PDUR_DESTPDU_ECU_110,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_100[1] = 
{
    PDUR_DESTPDU_ECU_100,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_101[1] = 
{
    PDUR_DESTPDU_COM_Tester_101,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_117[1] = 
{
    PDUR_DESTPDU_COM_Tester_117,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_10A[1] = 
{
    PDUR_DESTPDU_COM_Tester_10A,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_10E[1] = 
{
    PDUR_DESTPDU_COM_Tester_10E,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_10C[2] = 
{
    PDUR_DESTPDU_ECU_10D,
    PDUR_DESTPDU_ECU_134,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_119[1] = 
{
    PDUR_DESTPDU_COM_Tester_119,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_109[1] = 
{
    PDUR_DESTPDU_COM_Tester_109,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_118[1] = 
{
    PDUR_DESTPDU_COM_Tester_118,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_116[1] = 
{
    PDUR_DESTPDU_COM_Tester_116,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_108[1] = 
{
    PDUR_DESTPDU_COM_Tester_108,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_106[1] = 
{
    PDUR_DESTPDU_COM_Tester_106,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_179[1] = 
{
    PDUR_DESTPDU_ECU_179,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_135[1] = 
{
    PDUR_DESTPDU_ECU_135,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_133[1] = 
{
    PDUR_DESTPDU_ECU_133,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_12F[1] = 
{
    PDUR_DESTPDU_ECU_12F,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_12D[1] = 
{
    PDUR_DESTPDU_ECU_12D,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_12C[1] = 
{
    PDUR_DESTPDU_ECU_12C,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_12B[1] = 
{
    PDUR_DESTPDU_ECU_12B,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_121_DLC64[1] = 
{
    PDUR_DESTPDU_ECU_121_DLC64,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_178[1] = 
{
    PDUR_DESTPDU_COM_Tester_178,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_177[1] = 
{
    PDUR_DESTPDU_COM_Tester_177,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_18FF0141[1] = 
{
    PDUR_DESTPDU_COM_Tester_18FF0141,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_130[2] = 
{
    PDUR_DESTPDU_ECU_102,
    PDUR_DESTPDU_ECU_131,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_129_DLC64[1] = 
{
    PDUR_DESTPDU_COM_Tester_129_DLC64,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_124_DLC16[1] = 
{
    PDUR_DESTPDU_COM_Tester_124_DLC16,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_11A[1] = 
{
    PDUR_DESTPDU_ECU_11A,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_11B[1] = 
{
    PDUR_DESTPDU_ECU_11B,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_11D[1] = 
{
    PDUR_DESTPDU_ECU_11D,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_11E[1] = 
{
    PDUR_DESTPDU_ECU_11E,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_11F[1] = 
{
    PDUR_DESTPDU_COM_Tester_11F,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_Tester_18FF1131[1] = 
{
    PDUR_DESTPDU_COM_Tester_18FF1131,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_122[1] = 
{
    PDUR_DESTPDU_ECU_122,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_123[1] = 
{
    PDUR_DESTPDU_ECU_123,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_125[1] = 
{
    PDUR_DESTPDU_ECU_125,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_136[2] = 
{
    PDUR_DESTPDU_ECU_136,
    PDUR_DESTPDU_ECU_138,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_137[1] = 
{
    PDUR_DESTPDU_COM_Tester_137,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_UDS_TX[1] = 
{
    PDUR_DESTPDU_CANTP_UDS_TX,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_UDS_RX_PHY[1] = 
{
    PDUR_DESTPDU_DCM_UDS_RX_PHY,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_UDS_RX_FUN[1] = 
{
    PDUR_DESTPDU_DCM_UDS_RX_FUN,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_UDS_2A[1] = 
{
    PDUR_DESTPDU_CANIF_2A,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_Soad_LdCom_Eth[1] = 
{
    PDUR_DESTPDU_LDCOM_ETH_RX,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_LdCom_Soad_Eth[1] = 
{
    PDUR_DESTPDU_SOAD_LDCOM_ETH_TX,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_SoAd_SoAd_Eth[1] = 
{
    PDUR_DESTPDU_SoAd_Ecu_Gw_Tx_20020,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_201[1] = 
{
    PDUR_DESTPDU_ECU_201,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_211[1] = 
{
    PDUR_DESTPDU_ECU_211,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_221[1] = 
{
    PDUR_DESTPDU_ECU_221,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST)
PduR_PDUR_ROUTING_ECU_231[1] = 
{
    PDUR_DESTPDU_ECU_231,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRRoutingPathType,PDUR_CONST)
PduR_RoutingPathConfigData0[61] =
{
    {
        PDUR_SRCPDU_COM_ECU_18FEF121,
        2u,
        PduR_PDUR_ROUTING_ECU_18FEF121,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_112,
        1u,
        PduR_PDUR_ROUTING_ECU_112,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_111,
        1u,
        PduR_PDUR_ROUTING_ECU_111,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_18FEF221,
        1u,
        PduR_PDUR_ROUTING_ECU_18FEF221,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_107,
        1u,
        PduR_PDUR_ROUTING_ECU_107,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_103,
        1u,
        PduR_PDUR_ROUTING_ECU_103,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_10F,
        1u,
        PduR_PDUR_ROUTING_ECU_10F,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_11C_DLC16,
        1u,
        PduR_PDUR_ROUTING_ECU_11C_DLC16,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_115,
        1u,
        PduR_PDUR_ROUTING_ECU_115,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_105,
        1u,
        PduR_PDUR_ROUTING_ECU_105,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_114,
        1u,
        PduR_PDUR_ROUTING_ECU_114,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_104,
        1u,
        PduR_PDUR_ROUTING_ECU_104,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_110,
        1u,
        PduR_PDUR_ROUTING_ECU_110,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_100,
        1u,
        PduR_PDUR_ROUTING_ECU_100,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_101,
        1u,
        PduR_PDUR_ROUTING_Tester_101,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_117,
        1u,
        PduR_PDUR_ROUTING_Tester_117,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_10A,
        1u,
        PduR_PDUR_ROUTING_Tester_10A,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_10E,
        1u,
        PduR_PDUR_ROUTING_Tester_10E,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_10C,
        2u,
        PduR_PDUR_ROUTING_Tester_10C,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_119,
        1u,
        PduR_PDUR_ROUTING_Tester_119,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_109,
        1u,
        PduR_PDUR_ROUTING_Tester_109,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_118,
        1u,
        PduR_PDUR_ROUTING_Tester_118,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_116,
        1u,
        PduR_PDUR_ROUTING_Tester_116,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_108,
        1u,
        PduR_PDUR_ROUTING_Tester_108,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_106,
        1u,
        PduR_PDUR_ROUTING_Tester_106,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_179,
        1u,
        PduR_PDUR_ROUTING_ECU_179,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_135,
        1u,
        PduR_PDUR_ROUTING_ECU_135,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_133,
        1u,
        PduR_PDUR_ROUTING_ECU_133,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_12F,
        1u,
        PduR_PDUR_ROUTING_ECU_12F,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_12D,
        1u,
        PduR_PDUR_ROUTING_ECU_12D,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_12C,
        1u,
        PduR_PDUR_ROUTING_ECU_12C,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_12B,
        1u,
        PduR_PDUR_ROUTING_ECU_12B,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_121_DLC64,
        1u,
        PduR_PDUR_ROUTING_ECU_121_DLC64,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_178,
        1u,
        PduR_PDUR_ROUTING_Tester_178,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_177,
        1u,
        PduR_PDUR_ROUTING_Tester_177,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_18FF0141,
        1u,
        PduR_PDUR_ROUTING_Tester_18FF0141,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_130,
        2u,
        PduR_PDUR_ROUTING_Tester_130,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_129_DLC64,
        1u,
        PduR_PDUR_ROUTING_Tester_129_DLC64,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_124_DLC16,
        1u,
        PduR_PDUR_ROUTING_Tester_124_DLC16,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_11A,
        1u,
        PduR_PDUR_ROUTING_ECU_11A,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_11B,
        1u,
        PduR_PDUR_ROUTING_ECU_11B,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_11D,
        1u,
        PduR_PDUR_ROUTING_ECU_11D,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_11E,
        1u,
        PduR_PDUR_ROUTING_ECU_11E,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_11F,
        1u,
        PduR_PDUR_ROUTING_Tester_11F,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_18FF1131,
        1u,
        PduR_PDUR_ROUTING_Tester_18FF1131,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_122,
        1u,
        PduR_PDUR_ROUTING_ECU_122,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_123,
        1u,
        PduR_PDUR_ROUTING_ECU_123,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_125,
        1u,
        PduR_PDUR_ROUTING_ECU_125,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_136,
        2u,
        PduR_PDUR_ROUTING_ECU_136,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_Tester_137,
        1u,
        PduR_PDUR_ROUTING_ECU_137,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_DCM_UDS_TX,
        1u,
        PduR_UDS_TX,
        TRUE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_CANTP_UDS_RX_PHY,
        1u,
        PduR_UDS_RX_PHY,
        TRUE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_CANTP_UDS_RX_FUN,
        1u,
        PduR_UDS_RX_FUN,
        TRUE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_DCM_2A,
        1u,
        PduR_UDS_2A,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_SOAD_LDCOM_ETH_RX,
        1u,
        PduR_PDUR_Soad_LdCom_Eth,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_LDCOM_ETH_TX,
        1u,
        PduR_PDUR_LdCom_Soad_Eth,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_SoAd_Ecu_TcpServer_Rx_20020,
        1u,
        PduR_PDUR_SoAd_SoAd_Eth,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_201,
        1u,
        PduR_PDUR_ROUTING_ECU_201,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_211,
        1u,
        PduR_PDUR_ROUTING_ECU_211,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_221,
        1u,
        PduR_PDUR_ROUTING_ECU_221,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
    {
        PDUR_SRCPDU_COM_ECU_231,
        1u,
        PduR_PDUR_ROUTING_ECU_231,
        FALSE,
        FALSE,
        PDUR_UNUSED_UINT16
    },
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRRoutingTableType,PDUR_CONST)
PduR_RoutingTableConfigData[1] =
{
   {
      PduR_RoutingPathConfigData0
   }
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"
CONST(uint16,PDUR_CONST) 
PduR_DestPduPathGroup0[4] =
{
      PDUR_DESTPDU_ECU_10D,
      PDUR_DESTPDU_ECU_134,
      PDUR_DESTPDU_ECU_102,
      PDUR_DESTPDU_ECU_131,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_16
#include "PduR_MemMap.h"

#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduRRoutingPathGroupType,PDUR_CONST)
PduR_RoutingPathGroupConfigData[PDUR_ROUTING_PATH_GROUP_SUM] =
{
   {
        TRUE,
        PduRRoutingPathGroup_0,
        4u,
        PduR_DestPduPathGroup0
    },
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"


#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
const PduR_PBConfigType PduR_PBConfigData =
{
    0u,
    PduR_RoutingPathGroupConfigData,
    PduR_RoutingTableConfigData,
    PduR_SrcPduConfigData,
    PduR_DestPduConfigData,
};
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/

