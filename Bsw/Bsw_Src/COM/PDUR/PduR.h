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
**  FILENAME    : PduR.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : API declaration and type definitions of PDUR                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef  PDUR_H
#define  PDUR_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Types.h"
#if(STD_ON == PDUR_CANIF_SUPPORT)
#include "CanIf.h"
#endif
#if(STD_ON == PDUR_CANTP_SUPPORT)
#include "CanTp.h"
#endif
#if(STD_ON == PDUR_J1939TP_SUPPORT)
#include "J1939Tp.h"
#endif
#if(STD_ON == PDUR_COM_SUPPORT)
#include "Com_Cbk.h"
#endif
#if(STD_ON == PDUR_DCM_SUPPORT)
#include "Dcm.h"
#include "Dcm_Cbk.h"
#endif
#if(STD_ON == PDUR_J1939DCM_SUPPORT)
#include "J1939Dcm_Cbk.h"
#endif
#if(STD_ON == PDUR_IPDUM_SUPPORT)
#include "IpduM.h"
#include "IpduM_Cbk.h"
#endif
#if(STD_ON == PDUR_J1939RM_SUPPORT)
#include "J1939Rm_Cbk.h"
#endif
#if(STD_ON == PDUR_LDCOM_SUPPORT)
#include "LdCom_Cbk.h"
#endif
#if(STD_ON == PDUR_DLT_SUPPORT)
#include "Dlt_Cbk.h"
#endif
#if(STD_ON == PDUR_SECOC_SUPPORT)
#include "SecOC.h"
#include "SecOC_Cbk.h"
#endif
#if(STD_ON == PDUR_DBG_SUPPORT)
#include "Dbg_Cbk.h"
#endif
#if(STD_ON == PDUR_CANNM_SUPPORT)
#include "CanNm.h"
#endif
#if(STD_ON == PDUR_OSEKNM_SUPPORT)
#include "OsekNm.h"
#endif
#if(STD_ON == PDUR_UDPNM_SUPPORT)
#include "UdpNm.h"
#endif
#if(STD_ON == PDUR_LINIF_SUPPORT)
#include "LinIf.h"
#endif
#if(STD_ON == PDUR_FRIF_SUPPORT)
#include "FrIf.h"
#endif
#if(STD_ON == PDUR_FRNM_SUPPORT)
#include "FrNm.h"
#endif
#if(STD_ON == PDUR_DOIP_SUPPORT)
#include "DoIP.h"
#endif
#if(STD_ON == PDUR_SOAD_SUPPORT)
#include "SoAd.h"
#endif
#if(STD_ON == PDUR_SOMEIPTP_SUPPORT)
#include "SomeIpTp.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define PDUR_VENDOR_ID               62u
#define PDUR_MODULE_ID               51u
#define PDUR_INSTANCE_ID             0u
#define PDUR_H_AR_MAJOR_VERSION      4u
#define PDUR_H_AR_MINOR_VERSION      2u
#define PDUR_H_AR_PATCH_VERSION      2u
#define PDUR_H_SW_MAJOR_VERSION      2u
#define PDUR_H_SW_MINOR_VERSION      0u
#define PDUR_H_SW_PATCH_VERSION      1u

#if(STD_OFF == PDUR_ZERO_COST_OPERATION)

#if(STD_ON == PDUR_DEV_ERROR_DETECT)
/* General function id */
#define PDUR_INIT_ID                       (0xF0u)
#define PDUR_GETVERSIONINFO_ID             (0xF1u)
#define PDUR_GETCONFIGURATIONID_ID         (0xF2u)
#define PDUR_ENABLEROUTING_ID              (0xF3u)

#define PDUR_TRANSMIT_ID                   (0x49u)
#define PDUR_CANCELTRANSMIT_ID             (0x4Au)
#define PDUR_CHANGEPARAMETER_ID            (0x4Bu)
#define PDUR_CANCELRECEIVE_ID              (0x4Cu)

#define PDUR_TPCOPYRXDATA_ID               (0x44u)
#define PDUR_TPRXINDICATION_ID             (0x45u)
#define PDUR_TPSTARTOFRECEPTION_ID         (0x46u)
#define PDUR_TPCOPYTXDATA_ID               (0x43u)
#define PDUR_TPTXCONFIRMATION_ID           (0x48u)

/* Error Classification */
#define PDUR_E_INIT_FAILED                   (0x00u)
#define PDUR_E_PARAM_POINTER                 (0x09u)
#define PDUR_E_UNINIT                        (0x01u)
#define PDUR_E_PDU_ID_INVALID                (0x02u)
#define PDUR_E_ROUTING_PATH_GROUP_ID_INVALID (0x08u)
#endif/*STD_ON == PDUR_DEV_ERROR_DETECT*/

/*The API and DET Ref to Det_ReportRuntimeError*/
#define PDUR_DISABLEROUTING_ID               (0xF4u)
#define PDUR_IFRXINDICATION_ID               (0x42u)
#define PDUR_IFTRIGGERTRANSMIT_ID            (0x41u)
#define PDUR_IFTXCONFIRMATION_ID             (0x40u)
#define PDUR_E_TP_TX_REQ_REJECTED            (0x03u)
#define PDUR_E_PDU_INSTANCES_LOST            (0x0Au)


/*the Private Macro Definition*/
#define PDUR_UNUSED_UINT8                    (0xFFu)
#define PDUR_UNUSED_UINT16                   (0xFFFFu)
#define PDUR_NOUSED(paramter)                (void)paramter

#define PDUR_ROUTEPATHGROUP_CFG(id)     (\
PduR_ConfigStd->PduRRoutingPathGroupRef[id])

#define PDUR_DESTPDU_CFG(id)     (\
PduR_ConfigStd->PduRDestPduRef[id])

#define PDUR_SRCEPDU_CFG(id)     (\
PduR_ConfigStd->PduRSrcPduRef[id])

#define PDUR_ROUTINGPATH_CFG(id)     (\
PduR_ConfigStd->PduRRoutingTableRef[PduR_ConfigStd->PduRConfigId].PduRRoutingPathRef[id])

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

#if (STD_ON == PDUR_VERSION_INFO_API)
    #if (STD_ON == PDUR_DEV_ERROR_DETECT)
    #define PduR_GetVersionInfo(VersionInfo) \
        do{\
            if (NULL_PTR == (VersionInfo))\
            {\
                Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, PDUR_GETVERSIONINFO_ID, PDUR_E_PARAM_POINTER);\
            }\
            else \
            {\
                (VersionInfo)->vendorID = PDUR_VENDOR_ID; \
                (VersionInfo)->moduleID = PDUR_MODULE_ID; \
                (VersionInfo)->sw_major_version = PDUR_H_SW_MAJOR_VERSION; \
                (VersionInfo)->sw_minor_version = PDUR_H_SW_MINOR_VERSION; \
                (VersionInfo)->sw_patch_version = PDUR_H_SW_PATCH_VERSION; \
            }\
        }while(0)
    #else
    #define PduR_GetVersionInfo(VersionInfo) \
        do{\
              (VersionInfo)->vendorID = PDUR_VENDOR_ID; \
              (VersionInfo)->moduleID = PDUR_MODULE_ID; \
              (VersionInfo)->sw_major_version = PDUR_H_SW_MAJOR_VERSION; \
              (VersionInfo)->sw_minor_version = PDUR_H_SW_MINOR_VERSION; \
              (VersionInfo)->sw_patch_version = PDUR_H_SW_PATCH_VERSION; \
        }while(0)
    #endif
#endif

extern FUNC(void, PDUR_CODE)
PduR_Init(P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_CONST_PBCFG) ConfigPtr);

extern FUNC(PduR_PBConfigIdType, PDUR_CODE)
PduR_GetConfigurationId(void);

extern FUNC(void, PDUR_CODE)
PduR_EnableRouting(PduR_RoutingPathGroupIdType id);

extern FUNC(void, PDUR_CODE)
PduR_DisableRouting(PduR_RoutingPathGroupIdType id,boolean initialize);

extern FUNC(Std_ReturnType, PDUR_CODE)
PduR_Transmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

extern FUNC(Std_ReturnType, PDUR_CODE)
PduR_CancelTransmit(PduIdType id);

extern FUNC(Std_ReturnType, PDUR_CODE)
PduR_ChangeParameter(PduIdType id,TPParameterType parameter,uint16 value);

extern FUNC(Std_ReturnType, PDUR_CODE)
PduR_CancelReceive(PduIdType id);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(void, PDUR_CODE)
PduR_IfRxIndication(
        PduIdType RxPduId,
        P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr);
#define PDUR_STOP_SEC_IFRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(void, PDUR_CODE)
PduR_IfTxConfirmation(PduIdType TxPduId);
#define PDUR_STOP_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(Std_ReturnType, PDUR_CODE)
PduR_IfTriggerTransmit(
        PduIdType TxPduId,
        P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr);
#define PDUR_STOP_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_TpStartOfReception
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
#define PDUR_STOP_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_TpCopyRxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
#define PDUR_STOP_SEC_TPCOPYRXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(void, PDUR_CODE)
PduR_TpRxIndication
(
    PduIdType id,
    Std_ReturnType result);
#define PDUR_STOP_SEC_TPRXINDICATION_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(BufReq_ReturnType, PDUR_CODE)
PduR_TpCopyTxData
(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);
#define PDUR_STOP_SEC_TPCOPYTXDATA_CALLBACK_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
extern FUNC(void, PDUR_CODE)
PduR_TpTxConfirmation(
    PduIdType id,
    Std_ReturnType result);
#define PDUR_STOP_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#include "PduR_MemMap.h"
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*define in PduR_PBCfg.c*/
#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
extern CONST(PduR_PBConfigType, PDUR_CONST_PBCFG) PduR_PBConfigData;
#define PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#endif/*STD_OFF == PDUR_ZERO_COST_OPERATION*/

#endif  /* end of PDUR_H */
/*******************************************************************************
**                      End Of File                                           **
*******************************************************************************/


