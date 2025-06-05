/******************************************************************************
**                                                                           **
** Copyright (C) iSOFT   (2020)                                              **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to iSOFT.        **
** Passing on and copying of this document, and communication                **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
**************************************************************************** **
**                                                                           **
**  FILENAME    : SoAd.c                                                     **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION : none                                                       **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11           **
**                                                                           **
**************************************************************************** */
/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>  <DATE>    <AUTHOR>      <REVISION LOG>
 *  V2.0.0    20200507  zhengfei.li   Initial version
 *                                    (Upgrade according to the R19_11 standards)
 *  V2.0.1    20211108  zhengfei.li   Set up administrative data before sending
 *                                    TCP message(Avoid TxConfirmation coming early)
 *  V2.0.2    20220104  zhengfei.li   Udp status management parameters are set first
 *                                    and then notified to the upper layer.
 *                                    SoAdCopyTxData management variables plus
 *                                    interrupt protection.
 *  V2.0.3    20220106  zhengfei.li   SoAd_TxConfirmation invalid SoCon protection.
 ******************************************************************************/
/*===========================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
============================================================================*/
#include "SoAd.h"
#include "SoAd_Cbk.h"
#include "Det.h"
#include "SchM_SoAd.h"
/*===========================================================================
*                              SOURCE FILE VERSION INFORMATION
============================================================================*/
#define SOAD_VENDOR_ID_C                        62u
#define SOAD_C_AR_MAJOR_VERSION                 4u
#define SOAD_C_AR_MINOR_VERSION                 2u
#define SOAD_C_AR_PATCH_VERSION                 2u
#define SOAD_C_SW_MAJOR_VERSION                 2u
#define SOAD_C_SW_MINOR_VERSION                 0u
#define SOAD_C_SW_PATCH_VERSION                 3u
/*==========================================================================
*                                     FILE VERSION CHECKS
===========================================================================*/
/* Check if current file and SOAD header file are of the same vendor */
#if (SOAD_VENDOR_ID_C != SOAD_VENDOR_ID)
#error "SoAd.c and SoAd.h have different vendor ids"
#endif /* SOAD_VENDOR_ID_C != SOAD_VENDOR_ID */
/* Check if current file and SOAD header file are of
 the same Autosar version */
#if ((SOAD_C_AR_MAJOR_VERSION != SOAD_H_AR_MAJOR_VERSION) || \
     (SOAD_C_AR_MINOR_VERSION != SOAD_H_AR_MINOR_VERSION) || \
     (SOAD_C_AR_PATCH_VERSION != SOAD_H_AR_PATCH_VERSION))
#error "AutoSar Version Numbers of SoAd.c and SoAd.h are different"
#endif
/* Check if current file and SOAD header file are of
the same Software version */
#if ((SOAD_C_SW_MAJOR_VERSION != SOAD_H_SW_MAJOR_VERSION) || \
     (SOAD_C_SW_MINOR_VERSION != SOAD_H_SW_MINOR_VERSION) || \
     (SOAD_C_SW_PATCH_VERSION != SOAD_H_SW_PATCH_VERSION))
#error "Software Version Numbers of SoAd.c and SoAd.h are different"
#endif
/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/
typedef enum
{
    SOAD_STATE_UNINIT = 0u,
    SOAD_STATE_INIT
} SoAd_Module_StateType;

typedef enum
{
    SOAD_SOCON_REQUEST_NONE = 0u,
    SOAD_SOCON_REQUEST_OPEN,
    SOAD_SOCON_REQUEST_CLOSE
} SoAd_SoConRequestType;

typedef struct
{
    PduLengthType txPduTotalLength;
    PduLengthType txResidueLength;
    PduLengthType upCopyLength;
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    uint32 tirgTimer;
#endif/* STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER */
} SoAd_SoTxBufManType;


typedef struct
{
    PduLengthType EnterPosition;
    PduLengthType ExitPosition;
    PduLengthType ExitPduResidueLength;
    SoAd_uintx SoAd_SocketRouteId;
} SoAd_SoRxBufferManType;

typedef struct
{
    boolean SoConCloseAbort;
    boolean RemoteAddrIsSet;
    TcpIp_SockAddrType RemoteAddr;
    boolean TcpIpSocketValid;
    boolean SocketNeedClose;
    SoAd_SoConModeType SoAd_SoConMode;
    SoAd_SoConRequestType SoCon_Request[2u];
    PduIdType ActiveSoAdPduRouteId;
#if (STD_ON == SOAD_SUPPORT_UDP)
    uint32 UdpAliveSupervisionTime;
#endif/* STD_ON == SOAD_SUPPORT_UDP */
    TcpIp_SocketIdType TcpIpSocketId;
    PduLengthType TxNoAckLength;
    boolean TpPduTransmitting;
    boolean RemoteAddrNeedRelease;
    boolean ChangeModeToOffline;
} SoAd_InnerSocketManageType;

typedef struct
{
    boolean TcpIpSocketValid;
    TcpIp_IpAddrStateType LocalIpAddrState;
    TcpIp_SocketIdType TcpIpSocketId;
    SoAd_SoConIdType SoConSocketValidNum;
} SoAd_InnerSocketGroupManageType;

typedef struct
{
    uint8 TxPendingNum;
    uint8* PduDataPtr;
    PduLengthType PduLength;
    SoAd_SoConIdType TxSoConId;
} SoAd_PduRoutInnerManageType;

typedef struct
{
    boolean RoutGroupEnable;
} SoAd_RoutingGroupInnerManageType;

typedef struct
{
    boolean PduRouteDestEnable;
    /*the PduRouteDest TCP not ACK position of all TCP not ACK data*/
    PduLengthType TxPendingLength;
} SoAd_PduRouteDestInnerManagerType;

typedef struct
{
    boolean SocketRouteDestEnable;
} SoAd_SocketRouteDestInnerManagerType;

/*used by SoAd_RxFullTpPduHandle*/
typedef struct
{
    SoAd_SoConIdType SoConId;
    SoAd_uintx SocketRouteId;
    PduLengthType RxDataLength;
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr;
    PduLengthType UpBufferSize;
    PduLengthType PduLength;
} SoAd_RxFullTpPduHandleType;

/*used by SoAd_RxFullTpPduHandle*/
#if (STD_ON == SOAD_SUPPORT_UDP)
typedef struct
{
    P2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrChanged;
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddr;
} SoAd_FindTheValidSoConType;
#endif
/*==============================================================================
*                                       LOCAL MACROS
==============================================================================*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
#define SOAD_DET_REPORT(ApiId, ErrorId) (       \
        (void)Det_ReportError(                  \
            SOAD_MODULE_ID,                     \
            SOAD_INSTANCE,                      \
            (ApiId),                            \
            (ErrorId)))
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
#define SOAD_UNUSED_UINT8                (0xFFu)
#define SOAD_UNUSED_UINT32               (0xFFFFFFFFu)
#define SOAD_UNUSED_UINT16               (0xFFFFu)
#define SOAD_HEADER_SIZE            (8u)
/*configuration parameter addressing capability*/
#define SOAD_SOCKET_TXBUF_INDEX_CFG(soId)     (\
SoAd_PbCfgPtr->SoAd_SocketConnection[soId].SoAd_TxBufCfgIndexPtr)
#define SOAD_SOCKET_LINKGORUP(soId)         (\
SoAd_PbCfgPtr->SoAd_SocketConnection[soId].SoAd_SocketConnectionGroupRef)
#define SOAD_PDUROUT_CFG(pduId)    (\
    SoAd_PbCfgPtr->SoAd_PduRoute[pduId])
#define SOAD_PDUROT_DESTINDEX_CFG(pduId,destIndex) (\
    SoAd_PbCfgPtr->SoAd_PduRoute[pduId].SoAd_PduRouteDestIndex[destIndex])
#define SOAD_PDUROTDEST_CFG(pduRouDestIndex)    (\
    SoAd_PbCfgPtr->SoAd_PduRouteDestTotalPtr[pduRouDestIndex])
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
#define SOAD_UPPERLAY_PCCFG(upModule)   (\
    SoAd_PcCfg.SoAd_UpLayerFuncTalbe[upModule])
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
#define SOAD_ROUTGROUP_CFG(routGroupId)      (\
SoAd_PbCfgPtr->SoAd_RoutingGroup[routGroupId])
#define SOAD_ROUTGROUP_TXTRIG(routGroupId)      (\
SoAd_PbCfgPtr->SoAd_RoutingGroup[routGroupId].SoAd_RoutingGroupTxTriggerable)
/* BigEndian */
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#define SOAD_FILL_HEADER(buffer, headerId, lenth)           do{\
        (buffer)[0u] = (uint8)(((uint32)(headerId)) >> 24u);     \
        (buffer)[1u] = (uint8)(((uint32)(headerId)) >> 16u);     \
        (buffer)[2u] = (uint8)(((uint32)(headerId)) >> 8u);      \
        (buffer)[3u] = (uint8)((uint32)(headerId));              \
        (buffer)[4u] = (uint8)(((uint32)(lenth)) >> 24u);        \
        (buffer)[5u] = (uint8)(((uint32)(lenth)) >> 16u);        \
        (buffer)[6u] = (uint8)(((uint32)(lenth)) >> 8u);         \
        (buffer)[7u] = (uint8)((uint32)(lenth));                 \
}while(0)
#endif/* STD_ON == SOAD_SUPPORT_HEADERID */

#define SOAD_PDU_ROUTE_DEST_NUM (\
    SoAd_PbCfgPtr->SoAd_PduRouteDestTotalNum)
#define SOAD_SOCKET_ROUTE_DEST_NUM (\
    SoAd_PbCfgPtr->SoAd_SocketRouteDestTotalNum)
#define SOAD_SO_CON_GROUP_NUM (\
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroupNum)
#define SOAD_PDU_ROUTE_NUM (\
    SoAd_PbCfgPtr->SoAd_PduRouteNum)
#define SOAD_ROUTING_GROUP_NUM (\
    SoAd_PbCfgPtr->SoAd_RoutingGroupNum)
#define SOAD_SOCON_NUM (\
    SoAd_PbCfgPtr->SoAd_SocketConnectionNum)

#define SOAD_SOCON_CFG(soConId) (\
    SoAd_PbCfgPtr->SoAd_SocketConnection[soConId])
#define SOAD_SOCONGROUP_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId]
#define SOAD_SOCKETLOCALADDR_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId].SoAd_SocketLocalAddressRef
#define SOAD_SOCONREMOTEADDR_CFG(soConId) \
    SoAd_PbCfgPtr->SoAd_SocketConnection[soConId].SoAd_SocketRemoteAddress
#if (STD_ON == SOAD_SUPPORT_TCP)
#define SOAD_TCPTYPE_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId].SoAd_SocketProtocol.SoAd_SocketTcpPtr
#define SOAD_SOCONGROUP_TCPIMME_TXCONF(soConGop)    (\
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGop].\
    SoAd_SocketProtocol.SoAd_SocketTcpPtr->SoAd_SocketTcpImmediateTpTxConfirmation)
#endif/* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
#define SOAD_UDPTYPE_CFG(soConGroupId) \
    SoAd_PbCfgPtr->SoAd_SocketConnectionGroup[soConGroupId].SoAd_SocketProtocol.SoAd_SocketUdpPtr
#endif/* STD_ON == SOAD_SUPPORT_UDP */
#define SOAD_SRDEST_CFG(socketRouteId) \
    SoAd_PbCfgPtr->SoAd_SocketRouteDestTotalPtr[socketRouteId]
#define SOAD_SOCKETBUFFER_CFG(bufferId) (\
    SoAd_PbCfgPtr->SoAd_SocketBufferCfgPtr[bufferId])
#define SOAD_SOCKETROUTE_CFG(socketRouteId) \
    SoAd_PbCfgPtr->SoAd_SocketRoute[socketRouteId]
#define SOAD_PDUROUTE_CFG(pduRouteId) \
    SoAd_PbCfgPtr->SoAd_PduRoute[pduRouteId]
#define SOAD_GET_HEADER(dataPtr,bufStartId)   ( \
        ((uint32)(dataPtr)[(bufStartId)] << 24u) | \
        ((uint32)(dataPtr)[(bufStartId)+1u] << 16u) | \
        ((uint32)(dataPtr)[(bufStartId)+2u] << 8u) | \
        ((uint32)(dataPtr)[(bufStartId)+3u]) \
        )
#define SOAD_GET_LENGTH(dataPtr,bufStartId)   ( \
        ((uint32)(dataPtr)[(bufStartId)+4u] << 24u) | \
        ((uint32)(dataPtr)[(bufStartId)+5u] << 16u) | \
        ((uint32)(dataPtr)[(bufStartId)+6u] << 8u) | \
        ((uint32)(dataPtr)[(bufStartId)+7u]) \
        )

#if (0x100u > SOAD_SO_CON_MAX)
#define SOAD_METADATATOSOCONID(SoConId,MetaDataPtr)  ( \
        SoConId = MetaDataPtr[0u]                      \
        )
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
#define SOAD_METADATATOSOCONID(SoConId,MetaDataPtr)  (  \
        SoConId = (SoAd_SoConIdType)(MetaDataPtr[0u]) | \
        (SoAd_SoConIdType)(MetaDataPtr[1u]) << 8u)      \
        )
#else
#define SOAD_METADATATOSOCONID(SoConId,MetaDataPtr)  (  \
        SoConId = (SoAd_SoConIdType)(MetaDataPtr[1u]) | \
        (SoAd_SoConIdType)(MetaDataPtr[0u]) << 8u)      \
        )
#endif
#endif
/*==============================================================================
*                                      LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                                      LOCAL VARIABLES
==============================================================================*/
#define SOAD_START_SEC_VAR_INIT_PTR
#include "SoAd_MemMap.h"
static P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST)
SoAd_PbCfgPtr = NULL_PTR;
#define SOAD_STOP_SEC_VAR_INIT_PTR
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"
static VAR(SoAd_Module_StateType, SOAD_VAR_POWER_ON_INIT)
SoAd_Module_Status = SOAD_STATE_UNINIT;
#define SOAD_STOP_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_InnerSocketManageType, SOAD_VAR)
SoAd_SoConManager[SOAD_SO_CON_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_InnerSocketGroupManageType, SOAD_VAR)
SoAd_SoConGroupManager[SOAD_SO_CON_GROUP_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"

#if(0u < SOAD_ROUTING_GROUP_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_RoutingGroupInnerManageType, SOAD_VAR)
SoAd_RoutGroupManager[SOAD_ROUTING_GROUP_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/

#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_PduRouteDestInnerManagerType, SOAD_VAR)
SoAd_PduRouteDestManager[SOAD_PDU_ROUTE_DEST_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif /* 0u< SOAD_PDU_ROUTE_DEST_MAX */

#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_SocketRouteDestInnerManagerType, SOAD_VAR)
SoAd_SocketRouteDestManager[SOAD_SOCKET_ROUTE_DEST_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif/* 0u < SOAD_SOCKET_ROUTE_DEST_MAX */

#if (0u < SOAD_PDU_ROUTE_MAX)
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_PduRoutInnerManageType, SOAD_VAR)
SoAd_PduRoutManager[SOAD_PDU_ROUTE_MAX];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif/* 0u < SOAD_PDU_ROUTE_MAX */

#if (0u < SOAD_MAX_BUFFER_SIZE)
#define SOAD_START_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
static VAR(uint8, SOAD_VAR) SoAd_PduBuffer[SOAD_MAX_BUFFER_SIZE];
#define SOAD_STOP_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_MAX_BUFFER_SIZE */

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#define SOAD_START_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
static VAR(uint8, SOAD_VAR) SoAd_IfPduBuffer[SOAD_MAX_IF_PDU_SIZE];
#define SOAD_STOP_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"
#endif /* 0u < SOAD_MAX_IF_PDU_SIZE */

#if (0u < SOAD_TXBUFFER_NUM)
/* socket link buffer management data */
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_SoTxBufManType, SOAD_VAR)
SoAd_SoTxBufMan[SOAD_TXBUFFER_NUM];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif/* 0U< SOAD_TXBUFFER_NUM */

#if (0u < SOAD_RXBUFFER_NUM)
/* socket link buffer management data */
#define SOAD_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
static VAR(SoAd_SoRxBufferManType, SOAD_VAR)
SoAd_SoRxBufMan[SOAD_RXBUFFER_NUM];
#define SOAD_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
#endif/* 0u < SOAD_RXBUFFER_NUM */
/*==============================================================================
*                                      GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                                      GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==============================================================================*/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
static FUNC(void, SOAD_CODE)
SoAd_MetaDataFillHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);

static FUNC(void, SOAD_CODE)
SoAd_SocnTransmitAutomaticSoConSetupHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(TcpIp_ProtocolType, AUTOMATIC) Protocol);

static FUNC(void, SOAD_CODE)
SoAd_OpenTcpServerSoConHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE)
SoAd_OpenTcpClientSoConHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE)
SoAd_MainFunctionTxConfirmationHandle(void);

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(void, SOAD_CODE)
SoAd_MainFunctionNPduUdpTxHandle(SoAd_SoConIdType SoConId);
#endif

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetIfPduData(PduIdType TxPduId);
#endif

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_NPduUdpTransmitHandle(
        PduIdType TxPduId,
        SoAd_uintx PduRouteDestId,
        SoAd_SoConIdType SoConId);
#endif

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfTransmitSoCon(
        PduIdType TxPduId,
        SoAd_uintx PduRouteDestId,
        SoAd_SoConIdType SoConId);
#endif

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_TpPduTxErrorHandle(
        SoAd_SoConIdType SoConId,
        SoAd_uintx TxBufferId,
        SoAd_UpLayerModuleType UpModule,
        PduIdType UpPduId);
#endif

static FUNC(boolean, SOAD_CODE)
SoAd_SoConRemoteAddrLocked(SoAd_SoConIdType SoConId);

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_MainFunctionTpTransmitHandle(SoAd_SoConIdType SoConId);
#endif

static FUNC(void, SOAD_CODE)
SoAd_ReleaseRemoteAddrHandle(
        SoAd_SoConIdType SoConId,
        SoAd_uintx SoConGroupId);
static FUNC(void, SOAD_CODE)
SoAd_PduRouteManagerInit(PduIdType TxPduId);

static FUNC(void, SOAD_CODE)
SoAd_ReleaseRemoteAddrMainFunctionHandle(SoAd_SoConIdType SoConId);

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_TxBufferInit(SoAd_uintx TxBufferId);
#endif

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_RxBufferInit(SoAd_uintx RxBufferId);
#endif

/* soad local memcpy function,imitation lib memcpy */
static FUNC(void, SOAD_CODE)
SoAd_LocalMemCpy(
        P2VAR(uint8, AUTOMATIC, SOAD_VAR) destPtr,
        P2CONST(uint8, AUTOMATIC, SOAD_VAR) srcPtr,
        VAR(PduLengthType, AUTOMATIC) len);

#if(0u < SOAD_RXBUFFER_NUM)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableNewPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) BufferManageId);

static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableOldPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) BufferManageId);

static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableTpPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(PduLengthType, AUTOMATIC) UpBufferSize);
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableIfPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) BufferManageId);
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/
#endif/*0u < SOAD_RXBUFFER_NUM*/

#if(0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_RxDataMainFunctionHeaderDisableHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) BufferManageId);

static FUNC(void, SOAD_CODE)
SoAd_SoConRxDataMainFunctionHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
#endif/*0u < SOAD_RXBUFFER_NUM*/

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#if(0u < SOAD_RXBUFFER_NUM)
static FUNC(boolean, SOAD_CODE)
SoAd_HeaderEnableUdpSoConRxTpPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
        P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) DataPosition,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif/*0u < SOAD_RXBUFFER_NUM*/

static FUNC(boolean, SOAD_CODE)
SoAd_HeaderEnableUdpSoConRxHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(uint16, AUTOMATIC) RxMessageLength,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif/* STD_ON == SOAD_SUPPORT_UDP */

#if(0u < SOAD_RXBUFFER_NUM)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableTcpSoConRxTpPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
        VAR(PduLengthType, AUTOMATIC) RxMessageLength,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);

static FUNC(void, SOAD_CODE)
SoAd_RxFullTpPduHandle(
        VAR(SoAd_RxFullTpPduHandleType, AUTOMATIC) Parameter);
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE)
SoAd_HeaderDisableSoConRxTpPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
        VAR(PduLengthType, AUTOMATIC) RxMessageLength,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableTcpSoConRxHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(PduLengthType, AUTOMATIC) RxMessageLength,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

/*copy data to SoAd Rx Buffer*/
static FUNC(boolean, SOAD_CODE) SoAd_CopyDataToRxBuffer(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(PduLengthType, AUTOMATIC) RxMessageLength,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);
#endif/*0u < SOAD_RXBUFFER_NUM*/

static FUNC(void, SOAD_CODE)
SoAd_HeaderDisableSoConRxIfPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
        VAR(PduLengthType, AUTOMATIC) RxMessageLength,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr);

#if(0u < SOAD_SOCKET_ROUTE_DEST_MAX)
static FUNC(boolean, SOAD_CODE)
SoAd_FindTheValidSoCon(
        VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
        P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
        VAR(SoAd_FindTheValidSoConType, AUTOMATIC) Parameter,
#endif
        P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConId);
#endif/*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/


#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(boolean, SOAD_CODE)
SoAd_UdpStrictHeaderLenCheck(
        VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
        VAR(uint16, AUTOMATIC) Length);
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE)
SoAd_UdpAliveSupervisionTimeHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
#endif/* STD_ON == SOAD_SUPPORT_UDP */


static FUNC(boolean, SOAD_CODE)
SoAd_GetBestMatchAlgorithmSoCon(
        VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
        P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
        P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_CONST) SoConId,
        VAR(SoAd_BestMatchStateType, AUTOMATIC) CheckSoConState);

static FUNC(void, SOAD_CODE)
SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
        VAR(boolean, AUTOMATIC) SocketProtocolIsTcp,
#endif
        VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
SoAd_OpenUdpSoConHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);
#endif/* STD_ON == SOAD_SUPPORT_UDP */

static FUNC(void, SOAD_CODE)
SoAd_SoConModeChgNotificationAndTpStartOfReception(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE)
SoAd_SoConLocalIpAddrAssignmentChg(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

static FUNC(void, SOAD_CODE)
SoAd_SoConModeChangeHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);

#if(0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_CloseRxTpPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId);
#endif

static FUNC(void, SOAD_CODE)
SoAd_SoConCloseHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup,
        VAR(boolean, AUTOMATIC) CloseSocket);

static FUNC(void, SOAD_CODE)
SoAd_DisconnectionAndRecoveryHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup);

/* routing group init  and calculte SoAdSocketRouteDest ,SoAdPduRouteDest
routing control variable*/
static FUNC(void, SOAD_CODE)
SoAd_RoutingGroupInit(void);

/* id: routing group identifier specifying the routing group
enFlag: group control enable shall be set TRUE otherwise is FALSE
soConIdPtr:socket connection index specifying the socket connection on
which the routing group  */
#if(0u < SOAD_ROUTING_GROUP_MAX)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_RoutingGroupControl(
        VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
        VAR(boolean, AUTOMATIC) enFlag,
        P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) soConIdPtr);
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/

/* TpTransmit to specification socket connection id */
#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpTransmitSoCon(
       SoAd_SoConIdType SoConId,
       SoAd_uintx PduRouteDestId,
       P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr);
#endif/*0u < SOAD_TXBUFFER_NUM*/

/*Called by SoAd_TcpIpEvent.
 *Close the Udp SoCon Group*/
#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
SoAd_CloseUdpSoConGroup(
        VAR(TcpIp_EventType, AUTOMATIC) Event,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroupId);
#endif

/*Called by SoAd_TxConfirmation.
 *Tcp TxConfirmation handle*/
#if (STD_ON == SOAD_SUPPORT_TCP)
static FUNC(void, SOAD_CODE)
SoAd_TxConfirmationHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) soConId,
        VAR(uint16, AUTOMATIC) Length,
        VAR(SoAd_uintx, AUTOMATIC) pduRouteDestId);
#endif

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
/*===============================================================================
*                                       GLOBAL FUNCTIONS
==============================================================================*/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x01
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SoAdConfigPtr Pointer to the configuration data of the SoAd module.
Parameters (inout):None
Parameters (out): None
Return value: None
Description: Initializes the Socket Adaptor.
*/
FUNC(void, SOAD_CODE)
SoAd_Init(
    P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST) SoAdConfigPtr)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00216*/
    if (NULL_PTR == SoAdConfigPtr)
    {
        SOAD_DET_REPORT(SOAD_SID_INIT, SOAD_E_INIT_FAILED);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_uintx cnt;
        SoAd_uintx soConGroupId;
        /*SWS_SoAd_00211*/
        SoAd_PbCfgPtr = SoAdConfigPtr;
        /*SWS_SoAd_00723*/
        for (cnt = 0u; cnt < SOAD_SOCON_NUM; cnt++)
        {
            SoAd_SoConManager[cnt].SoAd_SoConMode = SOAD_SOCON_OFFLINE;
            SoAd_SoConManager[cnt].ChangeModeToOffline = FALSE;
#if (STD_ON == SOAD_SUPPORT_UDP)
            SoAd_SoConManager[cnt].UdpAliveSupervisionTime = 0u;
#endif/* STD_ON == SOAD_SUPPORT_UDP */
            /*init the socket connection's remote address to configuration parameter*/
            soConGroupId = SOAD_SOCON_CFG(cnt).SoAd_SocketConnectionGroupRef;
            SoAd_SoConManager[cnt].RemoteAddr.domain =
                SOAD_SOCKETLOCALADDR_CFG(soConGroupId).DomainType;
            if (NULL_PTR == SOAD_SOCON_CFG(cnt).SoAd_SocketRemoteAddress)
            {
                SoAd_SoConManager[cnt].RemoteAddrIsSet = FALSE;
                TCPIP_SET_IPADDR_ANY(SoAd_SoConManager[cnt].RemoteAddr);
                SoAd_SoConManager[cnt].RemoteAddr.port = TCPIP_PORT_ANY;
            }
            else
            {
                SoAd_SoConManager[cnt].RemoteAddrIsSet = TRUE;
                SoAd_SoConManager[cnt].RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(cnt);
            }
            /*init socket connection's Open/Close request*/
            if (TRUE == SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SocketAutomaticSoConSetup)
            {
                SoAd_SoConManager[cnt].SoCon_Request[0] = SOAD_SOCON_REQUEST_OPEN;
            }
            else
            {
                SoAd_SoConManager[cnt].SoCon_Request[0] = SOAD_SOCON_REQUEST_NONE;
            }
            SoAd_SoConManager[cnt].SoCon_Request[1] = SOAD_SOCON_REQUEST_NONE;
            /*other parameter init of SoAd_SoConManager*/
            SoAd_SoConManager[cnt].ActiveSoAdPduRouteId = SOAD_UNUSED_UINT16;
            SoAd_SoConManager[cnt].SoConCloseAbort = FALSE;
            SoAd_SoConManager[cnt].SocketNeedClose = FALSE;
            SoAd_SoConManager[cnt].TcpIpSocketValid = FALSE;
            SoAd_SoConManager[cnt].TcpIpSocketId = (TcpIp_SocketIdType)SOAD_UNUSED_UINT16;
            SoAd_SoConManager[cnt].TpPduTransmitting = FALSE;
            SoAd_SoConManager[cnt].TxNoAckLength = 0u;
            SoAd_SoConManager[cnt].RemoteAddrNeedRelease = FALSE;
        }
        /*init SoAd_SoConGroupManage parameter*/
        for (cnt = 0u; cnt < SOAD_SO_CON_GROUP_NUM; cnt++)
        {
            SoAd_SoConGroupManager[cnt].LocalIpAddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
            SoAd_SoConGroupManager[cnt].SoConSocketValidNum = 0u;
            SoAd_SoConGroupManager[cnt].TcpIpSocketId = (TcpIp_SocketIdType)SOAD_UNUSED_UINT16;
            SoAd_SoConGroupManager[cnt].TcpIpSocketValid = FALSE;
        }
        /* routing group init */
        SoAd_RoutingGroupInit();
        /* socket connection link buffer management data init */
#if (0u < SOAD_TXBUFFER_NUM)
        for(cnt = 0u; cnt < SOAD_TXBUFFER_NUM; cnt++)
        {
            SoAd_TxBufferInit(cnt);
        }
#endif/* 0u < SOAD_TXBUFFER_NUM */
#if (0u < SOAD_RXBUFFER_NUM)
        for(cnt = 0u; cnt < SOAD_RXBUFFER_NUM; cnt++)
        {
            SoAd_RxBufferInit(cnt);
        }
#endif/* 0u < SOAD_RXBUFFER_NUM */
#if (0u < SOAD_PDU_ROUTE_MAX)
        for(cnt = 0u; cnt < SOAD_PDU_ROUTE_NUM; cnt++)
        {
            SoAd_PduRouteManagerInit(cnt);
        }
#endif/* 0u < SOAD_PDU_ROUTE_MAX */
        SoAd_Module_Status = SOAD_STATE_INIT;
    }
    return;
}
/*============================================================================*/
/*
Service ID[hex]: 0x07
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): TxPduId Transmit PduId specifying the SoAd socket connection for which
the socket connection index shall be returned.
Parameters (inout): None
Parameters (out): SoConIdPtr Pointer to memory receiving the socket connection index asked
for.
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful
Description: Returns socket connection index related to the specified TxPduId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetSoConId(
    VAR(PduIdType, AUTOMATIC) TxPduId,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConIdPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00609*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONID, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00610*/
    if ((TRUE == detNoErr) && (TxPduId >= SOAD_PDU_ROUTE_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONID, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*SWS_SoAd_00724*/
        if(1u == SOAD_PDUROUTE_CFG(TxPduId).SoAd_PduRouteDestNum)
        {
            SoAd_uintx pduRouteDestId;
            pduRouteDestId = SOAD_PDUROUTE_CFG(TxPduId).SoAd_PduRouteDestIndex[0u];
            *SoConIdPtr = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_SoConId;
            result = E_OK;
        }
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x02
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): None
Parameters (inout): None
Parameters (out): versioninfo Pointer to where to store the version information of this module.
Return value: None
Description: Returns the version information.
 */
#if (STD_ON == SOAD_VERSION_INFO_API)
FUNC(void, SOAD_CODE)
SoAd_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, SOAD_APPL_DATA) versioninfo)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)

    if (NULL_PTR == versioninfo)
    {
        SOAD_DET_REPORT(SOAD_SID_GETVERSIONINFO, SOAD_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        versioninfo->moduleID = (VAR(uint16, AUTOMATIC))SOAD_MODULE_ID;
        versioninfo->vendorID = (VAR(uint16, AUTOMATIC))SOAD_VENDOR_ID;
        versioninfo->sw_major_version =
            (VAR(uint8, AUTOMATIC))SOAD_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version =
            (VAR(uint8, AUTOMATIC))SOAD_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version =
            (VAR(uint8, AUTOMATIC))SOAD_H_SW_PATCH_VERSION;
    }

    return;
}
#endif /*STD_ON == SOAD_VERSION_INFO_API*/
/*============================================================================*/
/*
Service ID[hex]: 0x03
Sync/Async: Synchronous
Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
Parameters (in): SoAdSrcPduId This parameter contains a unique identifier referencing to the
PDU Routing Table and thereby specifiying the socket to be
used for tranmission of the data.
SoAdSrcPduInfoPtr A pointer to a structure with socket related data: data length
and pointer to a data buffer.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted, e.g. due to a
still ongoing transmission in the corresponding socket or the to
be transmitted message is too long.
Description: Requests transmission of an I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfTransmit(
    VAR(PduIdType, AUTOMATIC) SoAdSrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_DEV_ERROR_DETECT)

    /*SWS_SoAd_00213*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_IFTRANSMIT, SOAD_E_NOTINIT);
    }
    /*SWS_SoAd_00214*/
    else  if (((SoAdSrcPduId >= SOAD_PDU_ROUTE_NUM)
               || (SOAD_UPPER_LAYER_IF !=
                   SOAD_PDUROUTE_CFG(SoAdSrcPduId).SoAd_TxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_IFTRANSMIT, SOAD_E_INV_PDUID);
    }
    else
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*the TxPdu is transmitting,refuse the new transmit request*/
        if(SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[SoAdSrcPduId].TxPendingNum)
        {
            //SchM_Enter_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
            /*1.get pdu data from up module*/
            if((NULL_PTR == SoAdSrcPduInfoPtr) || (NULL_PTR == SoAdSrcPduInfoPtr->SduDataPtr))
            {
                result = SoAd_GetIfPduData(SoAdSrcPduId);
            }
            else
            {
                if(SoAdSrcPduInfoPtr->SduLength <= (SOAD_MAX_IF_PDU_SIZE - SOAD_HEADER_SIZE))
                {
                    SoAd_LocalMemCpy(&SoAd_IfPduBuffer[SOAD_HEADER_SIZE],SoAdSrcPduInfoPtr->SduDataPtr,SoAdSrcPduInfoPtr->SduLength);
                    SoAd_PduRoutManager[SoAdSrcPduId].PduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
                    SoAd_PduRoutManager[SoAdSrcPduId].PduLength = SoAdSrcPduInfoPtr->SduLength;
                    if((TRUE == SOAD_PDUROUT_CFG(SoAdSrcPduId).SoAd_MetaDataEnable)
                            && (NULL_PTR != SoAdSrcPduInfoPtr->MetaDataPtr))
                    {
                        SOAD_METADATATOSOCONID(SoAd_PduRoutManager[SoAdSrcPduId].TxSoConId,
                            SoAdSrcPduInfoPtr->MetaDataPtr);
                    }
                    else
                    {
                        SoAd_PduRoutManager[SoAdSrcPduId].TxSoConId = NO_TXMETADATA;
                    }
                    result = E_OK;
                }
            }
            if(E_OK == result)
            {
                /*2.check the PduRouteDest/SoCon transmit condition and transmit the pdu on related SoCon(s)*/
                SoAd_uintx cnt,pduRouteDestNum,pduRouteDestId;
                SoAd_uintx txSuccessCnt = 0u;
                SoAd_uintx txFailCnt = 0u;
                pduRouteDestNum = SOAD_PDUROUT_CFG(SoAdSrcPduId).SoAd_PduRouteDestNum;
                for(cnt = 0u;(cnt < pduRouteDestNum) && (0u == txFailCnt);cnt++)
                {
                    pduRouteDestId = SOAD_PDUROT_DESTINDEX_CFG(SoAdSrcPduId, cnt);
                    if(TRUE == SoAd_PduRouteDestManager[pduRouteDestId].PduRouteDestEnable)
                    {
                        SoAd_SoConIdType soConId;
                        boolean tpPduTcpTransmitting = FALSE;
                        soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_SoConId;
                        if((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                                && (TRUE == SoAd_SoConManager[soConId].TpPduTransmitting))
                        {
                            tpPduTcpTransmitting = TRUE;
                        }
                        if(((NO_TXMETADATA == SoAd_PduRoutManager[SoAdSrcPduId].TxSoConId)
                                || (soConId == SoAd_PduRoutManager[SoAdSrcPduId].TxSoConId))
                                    &&((SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
                                        && (TRUE != SoAd_SoConManager[soConId].SocketNeedClose)
                                        && (FALSE == tpPduTcpTransmitting)))
                        {
                            if(E_OK == SoAd_IfTransmitSoCon(SoAdSrcPduId,pduRouteDestId,soConId))
                            {
                                txSuccessCnt++;
                            }
                            else
                            {
                                txFailCnt++;
                            }
                        }
                    }
                }
                if((txSuccessCnt > 0u) && (0u == txFailCnt))
                {
                    result = E_OK;
                }
                else
                {
                    result = E_NOT_OK;
                }
            }
            //SchM_Exit_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
        }
    }
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/
    SOAD_NOUSED(SoAdSrcPduId);
    SOAD_NOUSED(SoAdSrcPduInfoPtr);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1D
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier indirectly specifying PDUs to be
transmitted (after requesting the newest data from the related upper layer).
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Triggers the transmission of all If-TxPDUs identified by the parameter id after
requesting the data from the related upper layer.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfRoutingGroupTransmit(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id)
{
    Std_ReturnType result = E_NOT_OK;
#if ((0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE))
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00661*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_IFROUTINGGROUPTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00658*/
    if((TRUE == detNoErr)
            && ((id >= SOAD_ROUTING_GROUP_NUM)
                || (TRUE != SOAD_ROUTGROUP_TXTRIG(id))))
    {
        SOAD_DET_REPORT(SOAD_SID_IFROUTINGGROUPTRANSMIT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_uintx cnt,pduRouteDestId,pduRouteDestNum;
        PduIdType txPduId = 0xFFFFu;
        pduRouteDestNum = SOAD_ROUTGROUP_CFG(id).SoAd_PduRouteDestNum;
        SchM_Enter_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
        for(cnt = 0u;cnt < pduRouteDestNum;cnt++)
        {
            PduIdType pduRouteId;
            pduRouteDestId = SOAD_ROUTGROUP_CFG(id).SoAd_PduRouteDestIndexPtr[cnt];
            pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
            if((SOAD_UPPER_LAYER_IF == SOAD_PDUROUT_CFG(pduRouteId).SoAd_TxUpperLayerType)
                    && (SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[pduRouteId].TxPendingNum))
            {
                if(TRUE == SoAd_PduRouteDestManager[pduRouteDestId].PduRouteDestEnable)
                {
                    SoAd_SoConIdType soConId;
                    boolean tpPduTcpTransmitting = FALSE;
                    soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_SoConId;
                    if((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                            && (TRUE == SoAd_SoConManager[soConId].TpPduTransmitting))
                    {
                        tpPduTcpTransmitting = TRUE;
                    }
                    if((SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
                                && (TRUE != SoAd_SoConManager[soConId].SocketNeedClose)
                                && (FALSE == tpPduTcpTransmitting))
                    {

                        if(txPduId != pduRouteId)
                        {
                            (void)SoAd_GetIfPduData(pduRouteId);
                            txPduId = pduRouteId;
                        }
                        if((NULL_PTR != SoAd_PduRoutManager[pduRouteId].PduDataPtr)
                               && ((NO_TXMETADATA == SoAd_PduRoutManager[pduRouteId].TxSoConId)
                                       || (soConId == SoAd_PduRoutManager[pduRouteId].TxSoConId)))
                        {
                            (void)SoAd_IfTransmitSoCon(pduRouteId,pduRouteDestId,soConId);
                        }
                    }
                }
            }
        }
        SchM_Exit_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
        if(0xFFFFu != txPduId)
        {
            result = E_OK;
        }
    }
#endif/*(0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE)*/
    SOAD_NOUSED(id);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1f
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in):id routing group identifier indirectly specifying PDUs to be
transmitted (after requesting the newest data from the related
upper layer).
SoConId socket connection index specifying the socket connection on
which the PDUs shall be transmitted
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK The request was successful.
E_NOT_OK The request was not successful.
Description: Triggers the transmission of all If-TxPDUs identified by the parameter id on the
socket connection specified by SoConId after requesting the data from the related
upper layer.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfSpecificRoutingGroupTransmit(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if ((0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE))
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00712*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00713*/
    if((TRUE == detNoErr)
            && ((id >= SOAD_ROUTING_GROUP_NUM)
                || (TRUE != SOAD_ROUTGROUP_TXTRIG(id))
                ||(SoConId >= SOAD_SOCON_NUM)))
    {
        SOAD_DET_REPORT(SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_uintx cnt,pduRouteDestId,pduRouteDestNum;
        pduRouteDestNum = SOAD_ROUTGROUP_CFG(id).SoAd_PduRouteDestNum;
        SchM_Enter_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
        for(cnt = 0u;cnt < pduRouteDestNum;cnt++)
        {
            PduIdType pduRouteId;
            pduRouteDestId = SOAD_ROUTGROUP_CFG(id).SoAd_PduRouteDestIndexPtr[cnt];
            pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
            if((SOAD_UPPER_LAYER_IF == SOAD_PDUROUT_CFG(pduRouteId).SoAd_TxUpperLayerType)
                    && (SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[pduRouteId].TxPendingNum))
            {
                if(TRUE == SoAd_PduRouteDestManager[pduRouteDestId].PduRouteDestEnable)
                {
                    SoAd_SoConIdType soConId;
                    boolean tpPduTcpTransmitting = FALSE;
                    soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_SoConId;
                    if((TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                            && (TRUE == SoAd_SoConManager[soConId].TpPduTransmitting))
                    {
                        tpPduTcpTransmitting = TRUE;
                    }
                    if((SoConId == soConId)
                            &&(SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
                            && (TRUE != SoAd_SoConManager[soConId].SocketNeedClose)
                            && (FALSE == tpPduTcpTransmitting))
                    {
                        (void)SoAd_GetIfPduData(pduRouteId);
                        if((NULL_PTR != SoAd_PduRoutManager[pduRouteId].PduDataPtr)
                               && ((NO_TXMETADATA == SoAd_PduRoutManager[pduRouteId].TxSoConId)
                                       || (soConId == SoAd_PduRoutManager[pduRouteId].TxSoConId)))
                        {
                            (void)SoAd_IfTransmitSoCon(pduRouteId,pduRouteDestId,soConId);
                        }
                        if(E_NOT_OK == result)
                        {
                            result = E_OK;
                        }
                    }
                }
            }
        }
        SchM_Exit_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
    }
#endif/*(0u < SOAD_ROUTING_GROUP_MAX) && (0u < SOAD_MAX_IF_PDU_SIZE)*/
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}

/*============================================================================*/
/*
Service ID[hex]: 0x05
Sync/Async: Synchronous
Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
Parameters (in): PduId Identifiaction of the I-PDU to be cancelled.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation:
E_OK: request accepted (but not yet performed).
E_NOT_OK: request not accepted (e.g. cancellation not
possible).
Description: Requests cancellation of the transmission via TP for a specific I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpCancelTransmit(
    VAR(PduIdType, AUTOMATIC) PduId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00605*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00606*/
    if((TRUE == detNoErr) && ((PduId >= SOAD_PDU_ROUTE_NUM)
                || (SOAD_UPPER_LAYER_TP != SOAD_PDUROUTE_CFG(PduId).SoAd_TxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELTRANSMIT, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if(0u < SOAD_TXBUFFER_NUM)
        SoAd_uintx pduRotDestIndex;
        SoAd_SoConIdType soConId;
        /*TP PDU just reference one PduRouteDest(one SoCon),TP transmit just support 1:1*/
        pduRotDestIndex = SOAD_PDUROT_DESTINDEX_CFG(PduId, 0u);
        soConId = SOAD_PDUROTDEST_CFG(pduRotDestIndex).SoAd_SoConId;
        if((TRUE == SoAd_SoConManager[soConId].TpPduTransmitting)
            && (PduId == SoAd_SoConManager[soConId].ActiveSoAdPduRouteId))
        {
            result = E_OK;
            SoAd_SoConManager[soConId].SocketNeedClose = TRUE;
        }
#endif/*0u < SOAD_TXBUFFER_NUM*/
    }
    SOAD_NOUSED(PduId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x06
Sync/Async: Synchronous
Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
Parameters (in): PduId Identifiaction of the I-PDU to be cancelled.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation:
E_OK: request accepted (but not yet performed).
E_NOT_OK: request not accepted (e.g. cancellation not
possible).
Description: Requests cancellation of the reception via TP for a specific I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpCancelReceive(VAR(PduIdType, AUTOMATIC) PduId)
{
    Std_ReturnType result = E_NOT_OK;
#if(0u < SOAD_RXBUFFER_NUM)
    SoAd_SoConIdType soConId;
    SoAd_uintx bufferId;
    SoAd_uintx soConGroupId;
    boolean headerEnabled;
#endif/*0u < SOAD_RXBUFFER_NUM*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00607*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELRECEIVE, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00608*/
    if((TRUE == detNoErr) && ((PduId >= SOAD_SOCKET_ROUTE_DEST_NUM)
            || (SOAD_UPPER_LAYER_TP != SOAD_SRDEST_CFG(PduId).SoAd_RxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_TPCANCELRECEIVE, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if(0u < SOAD_RXBUFFER_NUM)
        /*TP socket route can't reference SoConGroup*/
        soConId = *SOAD_SOCKETROUTE_CFG(PduId).SoAd_RxSocketConnRefIndexPtr;
        bufferId = SOAD_SOCON_CFG(soConId).SoAd_RxBufCfgIndexPtr[0u];
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        if(SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
        {
            soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
            headerEnabled = SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_PduHeaderEnable;
            if(TRUE == headerEnabled)
            {
                if((PduId == SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId)
                        && (SoAd_SoRxBufMan[bufferId].ExitPduResidueLength > 0u))
                {
                    SoAd_SoConManager[soConId].SocketNeedClose = TRUE;
                    result = E_OK;
                }
            }
            else
            {
                SoAd_SoConManager[soConId].SocketNeedClose = TRUE;
                result = E_OK;
            }
        }
#endif/*0u < SOAD_RXBUFFER_NUM*/
    }
    SOAD_NOUSED(PduId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x04
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in):
SoAdSrcPduId This parameter contains a unique identifier referencing to the
PDU Routing Table and thereby specifiying the socket to be
used for tranmission of the data.
SoAdSrcPduInfoPtr A pointer to a structure with socket related data. Only the
length data is valid.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted, e.g. due to a
still ongoing transmission in the corresponding socket or the to
be transmitted message is too long.
Description: Requests transmission of an I-PDU.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpTransmit(
    VAR(PduIdType, AUTOMATIC) SoAdSrcPduId,
    P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00224*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TPTRANSMIT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00237*/
    if((TRUE == detNoErr)
            && ((SoAdSrcPduId >= SOAD_PDU_ROUTE_NUM)
                || (SOAD_UPPER_LAYER_TP !=
                    SOAD_PDUROUTE_CFG(SoAdSrcPduId).SoAd_TxUpperLayerType)))
    {
        SOAD_DET_REPORT(SOAD_SID_TPTRANSMIT, SOAD_E_INV_PDUID);
        detNoErr = FALSE;
    }
    /* add det check input parameter point */
    if((TRUE == detNoErr)
            && (NULL_PTR == SoAdSrcPduInfoPtr))
    {
        SOAD_DET_REPORT(SOAD_SID_TPTRANSMIT, SOAD_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        #if (0u < SOAD_TXBUFFER_NUM)
        if((SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[SoAdSrcPduId].TxPendingNum)
             && (0u < SoAdSrcPduInfoPtr->SduLength))
        {
            SoAd_uintx pduRouteDestId;
            pduRouteDestId = SOAD_PDUROT_DESTINDEX_CFG(SoAdSrcPduId,0u);
            if(TRUE == SoAd_PduRouteDestManager[pduRouteDestId].PduRouteDestEnable)
            {
                SoAd_SoConIdType soConId;
                soConId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_SoConId;
                if((SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
                        && (TRUE != SoAd_SoConManager[soConId].SocketNeedClose)
                        && (FALSE == SoAd_SoConManager[soConId].TpPduTransmitting))
                {
                    result = SoAd_TpTransmitSoCon(
                                 soConId,
                                 pduRouteDestId,
                                 SoAdSrcPduInfoPtr);
                    if(E_OK == result)
                    {
                        SoAd_SoConManager[soConId].ActiveSoAdPduRouteId = SoAdSrcPduId;
                    }
                }
            }
        }
        #endif/*0u < SOAD_TXBUFFER_NUM*/
    }
    SOAD_NOUSED(SoAdSrcPduId);
    SOAD_NOUSED(SoAdSrcPduInfoPtr);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x08
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index specifying the socket connection which
shall be opened
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: This service opens the socket connection specified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_OpenSoCon(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    SoAd_uintx soConGroup;
    /*SWS_SoAd_00615*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_OPENSOCON, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00611*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_OPENSOCON, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00528*/
    if (TRUE == detNoErr)
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_OPENSOCON, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        if (SOAD_SOCON_REQUEST_NONE == SoAd_SoConManager[SoConId].SoCon_Request[0u])
        {
            SoAd_SoConManager[SoConId].SoCon_Request[0u] = SOAD_SOCON_REQUEST_OPEN;
        }
        else
        {
            if (SOAD_SOCON_REQUEST_NONE == SoAd_SoConManager[SoConId].SoCon_Request[1u])
            {
                if (SOAD_SOCON_REQUEST_CLOSE == SoAd_SoConManager[SoConId].SoCon_Request[0u])
                {
                    SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_OPEN;
                }
            }
            else if (SOAD_SOCON_REQUEST_CLOSE == SoAd_SoConManager[SoConId].SoCon_Request[1u])
            {
                SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
            }
            else
            {
                ;/* do nothing */
            }
        }
        result = E_OK;
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x09
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index specifying the socket connection which
shall be closed
abort TRUE: socket connection will immediately be terminated.
FALSE: socket connection will be terminated if no other upper
layer is using this socket connection.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: This service closes the socket connection specified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_CloseSoCon(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(boolean, AUTOMATIC) abort)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    SoAd_uintx soConGroup;
    /*SWS_SoAd_00616*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_CLOSESOCON, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00612*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_CLOSESOCON, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00529*/
    if (TRUE == detNoErr)
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_CLOSESOCON, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        SoAd_SoConManager[SoConId].SoConCloseAbort = abort;
        if(FALSE == abort)
        {
            if (SOAD_SOCON_REQUEST_NONE == SoAd_SoConManager[SoConId].SoCon_Request[0u])
            {
                SoAd_SoConManager[SoConId].SoCon_Request[0u] = SOAD_SOCON_REQUEST_CLOSE;
            }
            else
            {
                if (SOAD_SOCON_REQUEST_NONE == SoAd_SoConManager[SoConId].SoCon_Request[1u])
                {
                    if (SOAD_SOCON_REQUEST_OPEN == SoAd_SoConManager[SoConId].SoCon_Request[0u])
                    {
                        SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_CLOSE;
                    }
                }
                else if (SOAD_SOCON_REQUEST_OPEN == SoAd_SoConManager[SoConId].SoCon_Request[1u])
                {
                    SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
                }
                else
                {
                    ;/* do nothing */
                }
            }
        }
        else
        {
            SoAd_SoConManager[SoConId].SoCon_Request[0u] = SOAD_SOCON_REQUEST_CLOSE;
            SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
        }
        result = E_OK;
    }
    return result;
}
/*============================================================================*/
/*
Service ID [hex]: 0x22
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in):
SoConId: socket connection index specifying the socket connection
         for which the state shall be returned.
Parameters (inout): None
Parameters (out):
ModePtr: Pointer to memory where the socket connection state shall be stored.
Return value: None
Description: Returns current state of the socket connection specified by SoConId.
 */
FUNC(void, SOAD_CODE)
SoAd_GetSoConMode(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(SoAd_SoConModeType, AUTOMATIC, SOAD_APPL_DATA) ModePtr)
{
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONMODE, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((SoConId >= SOAD_SOCON_NUM) ||(NULL_PTR == ModePtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_GETSOCONMODE, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        *ModePtr = SoAd_SoConManager[SoConId].SoAd_SoConMode;
    }
    return;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0A
Sync/Async: Asynchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId Socket connection index specifying the socket connection for
which the IP address shall be set
Type Type of IP address assignment which shall be initiated.
LocalIpAddrPtr Pointer to structure containing the IP address which shall be
assigned to the EthIf controller indirectly specified via SoConId.
Note: This parameter is only used in case the parameter Type is
set to TCPIP_IPADDR_ASSIGNMENT_STATIC, can be set to
NULL_PTR otherwise.
Netmask Network mask of IPv4 address or address prefix of IPv6 address
in CIDR Notation.
Note: This parameter is only used in case the parameter Type is
set to TCPIP_IPADDR_ASSIGNMENT_STATIC.
DefaultRouterPtr Pointer to structure containing the IP address of the default router
(gateway) which shall be assigned.
Note: This parameter is only used in case the parameter Type is
set to TCPIP_IPADDR_ASSIGNMENT_STATIC, can be set to
NULL_PTR otherwise.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the local IP address assignment which shall be used for the
socket connection specified by SoConId is initiated.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_RequestIpAddrAssignment(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) Type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) LocalIpAddrPtr,
    VAR(uint8, AUTOMATIC) Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) DefaultRouterPtr)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00613*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_REQUESTIPADDRASSIGNMENT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00617*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_REQUESTIPADDRASSIGNMENT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_RequestIpAddrAssignment(localAddrId, Type, LocalIpAddrPtr,
                                               Netmask, DefaultRouterPtr);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0B
Sync/Async: Asynchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index specifying the socket connection for
which the IP address shall be released
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the local IP address assignment used for the socket
connection specified by SoConId is released.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_ReleaseIpAddrAssignment(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00618*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEIPADDRASSIGNMENT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00619*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEIPADDRASSIGNMENT, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_ReleaseIpAddrAssignment(localAddrId);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0C
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index representing the SoAd socket
connection for which the actual local IP address shall be
obtained.
Parameters (inout): LocalAddrPtr Pointer to a struct where the local address (IP address and port)
is stored.
The struct member domain shall be set by the caller of the API to
the desired TcpIp_DomainType and it shall be ensured by the
caller that the struct is large enough to store an address of the
selected type (INET or INET6).
DefaultRouterPtr Pointer to struct where the IP address of the default router
(gateway) is stored (struct member "port" is not used and of
arbitrary value).
The struct must be of the same type and size as LocalAddrPtr.
Parameters (out): NetmaskPtr Pointer to memory where Network mask of IPv4 address or
address prefix of IPv6 address in CIDR Notation is stored
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Retrieves the local address (IP address and port) actually used for the SoAd
socket connection specified by SoConId, the netmask and default router
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetLocalAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) NetmaskPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DefaultRouterPtr)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00621*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETLOCALADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00620*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETLOCALADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        LocalAddrPtr->port = SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketLocalPort;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_GetIpAddr(localAddrId, LocalAddrPtr, NetmaskPtr,
                                 DefaultRouterPtr);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0D
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index representing the SoAd socket
connection for which the physical source address of the related
EthIf controller shall be obtained.
Parameters (inout): None
Parameters (out): PhysAddrPtr Pointer to the memory where the physical source address (MAC
address) in network byte order is stored
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Retrieves the physical source address of the EthIf controller used by the SoAd
socket connection specified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetPhysAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PhysAddrPtr)
{
    TcpIp_LocalAddrIdType localAddrId;
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00623*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETPHYSADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00622*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETPHYSADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        result = TcpIp_GetPhysAddr(localAddrId, PhysAddrPtr);
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1C
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): SoConId socket connection index representing the SoAd socket
connection for which the actually specified remote address shall
be obtained.
Parameters (inout): None
Parameters (out): IpAddrPtr Pointer to a struct where the retrieved remote address (IP
address and port) is stored.
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Retrieves the remote address (IP address and port) actually used for the SoAd
socket connection specified by SoConId
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00659*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00660*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        if (IpAddrPtr->domain == SoAd_SoConManager[SoConId].RemoteAddr.domain)
        {
            if (TRUE == SoAd_SoConManager[SoConId].RemoteAddrIsSet)
            {
                /*SWS_SoAd_00664*/
                
                *IpAddrPtr = SoAd_SoConManager[SoConId].RemoteAddr;
                result = E_OK;
            }
            /*SWS_SoAd_00666*/
            else
            {
                ;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00698*/
        else
        {
            SOAD_DET_REPORT(SOAD_SID_GETREMOTEADDR, SOAD_E_INV_ARG);
        }
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0E
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
enabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Enables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id.
Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP
or UDP socket of the TCP/IP stack specified by a PduRoute or the other way
around specified by a SocketRoute.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_EnableRouting(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00624*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLEROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00625*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLEROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if(0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, TRUE, NULL_PTR);
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x20
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
enabled
SoConId socket connection index specifying the socket connection on
which the routing group shall be enabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK The request was successful.
E_NOT_OK The request was not successful.
Description: Enables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id only on the socket connection identified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_EnableSpecificRouting(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00715*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLESPECIFICROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00716*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_ENABLESPECIFICROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if(0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, TRUE, &SoConId);
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x0F
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
disabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK The request was successful
E_NOT_OK The request was not successful.
Description: Disables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id.
Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP
or UDP socket of the TCP/IP stack specified by a PduRoute or the other way
around specified by a SocketRoute.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_DisableRouting(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00627*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLEROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00626*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLEROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if(0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, FALSE, NULL_PTR);
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x21
Sync/Async: Synchronous
Reentrancy: Reentrant
Parameters (in): id routing group identifier specifying the routing group to be
disabled
SoConId socket connection index specifying the socket connection on
which the routing group shall be disabled
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK The request was successful.
E_NOT_OK The request was not successful.
Description: Disables routing of a group of PDUs in the SoAd related to the RoutingGroup
specified by parameter id only on the socket connection identified by SoConId.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_DisableSpecificRouting(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00718*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLESPECIFICROUTING, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00719*/
    if ((TRUE == detNoErr) && (id >= SOAD_ROUTING_GROUP_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_DISABLESPECIFICROUTING, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if(0u < SOAD_ROUTING_GROUP_MAX)
        result = SoAd_RoutingGroupControl(id, FALSE, &SoConId);
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
    }
    SOAD_NOUSED(id);
    SOAD_NOUSED(SoConId);
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x10
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId socket connection index specifying the socket connection for
which the remote address shall be set
RemoteAddrPtr Struct containint the IP address and port to be set.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the remote address (IP address and port) of the specified
socket connection shall be set.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_SetRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    boolean remoteAddrLocked;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00628*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((SoConId >= SOAD_SOCON_NUM) || (NULL_PTR == RemoteAddrPtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00531*/
    if (TRUE == detNoErr)
    {
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    /*SWS_SoAd_00699*/
    if (TRUE == detNoErr)
    {
        if (RemoteAddrPtr->domain != SoAd_SoConManager[SoConId].RemoteAddr.domain)
        {
            SOAD_DET_REPORT(SOAD_SID_SETREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        remoteAddrLocked = SoAd_SoConRemoteAddrLocked(SoConId);
        if(FALSE == remoteAddrLocked)
        {
            /*SWS_SoAd_00533*/
            SoAd_SoConManager[SoConId].RemoteAddrIsSet = TRUE;
            SoAd_SoConManager[SoConId].RemoteAddr = *RemoteAddrPtr;
            /*SWS_SoAd_00687*/
            if((SOAD_SOCON_ONLINE == SoAd_SoConManager[SoConId].SoAd_SoConMode)
                    && (TCPIP_IS_IPADDR_ANY(*RemoteAddrPtr)
                        || (TCPIP_PORT_ANY == RemoteAddrPtr->port)))
            {
                SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            }
            else
            {
                SoAd_DisconnectionAndRecoveryHandle(SoConId, soConGroup);
            }
            result = E_OK;
        }
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1e
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId Index of any socket connection that is part of the
SoAdSocketConnectionGroup.
RemoteAddrPtr Pointer to the structure containing the requested remote IP
address and port.
Parameters (inout): None
Parameters (out): AssignedSoConIdPtr Pointer to the SoAd_SoConIdType where the index of the
socket connection configured with the remote address
(RemoteAddrPtr) shall be stored.
Return value: Std_ReturnType
E_OK: The request was accepted.
E_NOT_OK: The request was rejected,AssignedSoConIdPtr remains unchanged.
Description: This API service shall either return the socket connection index of the
SoAdSocketConnectionGroup where the specified remote address (IP address
and port) is set or assign the remote address to an unused socket connection from
the same SoAdSocketConnectionGroup.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_SetUniqueRemoteAddr(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) AssignedSoConIdPtr)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    SoAd_SoConIdType soConId = 0u;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00672*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00673*/
    if ((TRUE == detNoErr) && ((SoConId >= SOAD_SOCON_NUM) || (NULL_PTR == RemoteAddrPtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*similar to SWS_SoAd_00531*/
    if (TRUE == detNoErr)
    {
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    /*SWS_SoAd_00700*/
    if (TRUE == detNoErr)
    {
        if (RemoteAddrPtr->domain != SoAd_SoConManager[SoConId].RemoteAddr.domain)
        {
            SOAD_DET_REPORT(SOAD_SID_SETUNIQUEREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*SWS_SoAd_00675,SWS_SoAd_00676*/
        if(TRUE == SoAd_GetBestMatchAlgorithmSoCon(soConGroup, RemoteAddrPtr, &soConId, SOAD_SOCON_MATCH_REQUEST_NONE))
        {
            SoAd_SoConManager[soConId].RemoteAddrIsSet = TRUE;
            SoAd_SoConManager[soConId].RemoteAddr = *RemoteAddrPtr;
            result = E_OK;
            *AssignedSoConIdPtr = soConId;
            SoAd_DisconnectionAndRecoveryHandle(soConId, soConGroup);
        }
        /*SWS_SoAd_00678*/
        else
        {
            result = E_NOT_OK;
        }
    }
    return result;
}
/*============================================================================*/
/*
Service ID [hex]: 0x23
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters(in):
SoConId: Index of the socket connection for which the remote address shall be released.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: By this API service the remote address (IP address and port) of the specified
socket connection shall be released, i.e. set back to the configured remote address setting.
 */
FUNC(void, SOAD_CODE)
SoAd_ReleaseRemoteAddr(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    boolean remoteAddrLocked;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEREMOTEADDR, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_RELEASEREMOTEADDR, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    /*similar to SWS_SoAd_00744*/
    if (TRUE == detNoErr)
    {
        if (TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
        {
            SOAD_DET_REPORT(SOAD_SID_RELEASEREMOTEADDR, SOAD_E_INV_ARG);
            detNoErr = FALSE;
        }
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        remoteAddrLocked = SoAd_SoConRemoteAddrLocked(SoConId);
        if(FALSE == remoteAddrLocked)
        {
            SoAd_ReleaseRemoteAddrHandle(SoConId, soConGroup);
        }
        else
        {
            SoAd_SoConManager[SoConId].RemoteAddrNeedRelease = TRUE;
        }
    }
    return;
}
/*============================================================================*/
/*
Service ID[hex]: 0x11
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): id Identification of the I-PDU which the parameter change
shall affect parameter Identifier of the parameter to be changed
value New parameter value
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service the SoAd or TCP/IP stack is requested to change a connection
parameter. E.g. the Nagle algorithm may be controlled by this API.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpChangeParameter(
    VAR(PduIdType, AUTOMATIC) id,
    VAR(TPParameterType, AUTOMATIC) parameter,
    VAR(uint16, AUTOMATIC) value)
{
    /*follow AUTOSAR_R19_11:SWS_SoAd_00730
      not follow AUTOSAR4.2.2:SWS_SoAd_00629,SWS_SoAd_00630,SWS_SoAd_00631
    */
    SOAD_NOUSED(id);
    SOAD_NOUSED(parameter);
    SOAD_NOUSED(value);
    return E_NOT_OK;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1A
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId socket connection index specifying the socket connection for
which the hostname shall be read
Parameters (inout): length As input parameter, contains the length of the provided data
buffer.Will be overwritten with the length of the actual data.
Parameters (out): data Pointer to provided memory buffer the hostname, i.e. the Fully
Qualified Domain Name (FQDN) according to IETF RFC
4702/IETF RFC 4704 will be copied to.
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service an upper layer of the SoAd can read the currently configured
hostname, i.e. FQDN option in the DHCP submodule of the TCP/IP stack.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_ReadDhcpHostNameOption(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) length,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) data)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
    TcpIp_LocalAddrIdType localAddrId;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00701*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_READDHCPHOSTNAMEOPTION, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00702*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_READDHCPHOSTNAMEOPTION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        if (TCPIP_AF_INET == SOAD_SOCKETLOCALADDR_CFG(soConGroup).DomainType)
        {
            /*SWS_SoAd_00703*/
            result = TcpIp_DhcpReadOption(localAddrId, 81u, length, data);
        }
#if (STD_ON == SOAD_IPV6_ADDRESS_ENABLED)
        else
        {
            uint16 dataLength;
            dataLength = (uint16)(*length);
            /*SWS_SoAd_00704*/
            result = TcpIp_DhcpV6ReadOption(localAddrId, 39u,&dataLength,data);
        }
#endif/*STD_ON == SOAD_IPV6_ADDRESS_ENABLED*/
    }
    return result;
}
/*============================================================================*/
/*
Service ID[hex]: 0x1B
Sync/Async: Synchronous
Reentrancy: Reentrant for different SoConIds. Non reentrant for the same SoConId.
Parameters (in): SoConId socket connection index specifying the socket connection for
which the hostname shall be changed length Length of hostname to be set.
data Pointer to memory containing the hostname, i.e. the Fully
Qualified Domain Name (FQDN) according to IETF RFC
4702/IETF RFC 4704.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType E_OK: The request has been accepted
E_NOT_OK: The request has not been accepted
Description: By this API service an upper layer of the SoAd can set the hostname, i.e. FQDN
option in the DHCP submodule of the TCP/IP stack.
 */
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_WriteDhcpHostNameOption(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(uint8, AUTOMATIC) length,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_CONST) data)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_uintx soConGroup;
    TcpIp_LocalAddrIdType localAddrId;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00705*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00706*/
    if ((TRUE == detNoErr) && (SoConId >= SOAD_SOCON_NUM))
    {
        SOAD_DET_REPORT(SOAD_SID_WRITEDHCPHOSTNAMEOPTION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
        localAddrId = SOAD_SOCKETLOCALADDR_CFG(soConGroup).LocalAddrId;
        if (TCPIP_AF_INET == SOAD_SOCKETLOCALADDR_CFG(soConGroup).DomainType)
        {
            /*SWS_SoAd_00707*/
            result = TcpIp_DhcpWriteOption(localAddrId, 81u, length, data);
        }
#if (STD_ON == SOAD_IPV6_ADDRESS_ENABLED)
        else
        {
            /*SWS_SoAd_00708*/
            result = TcpIp_DhcpV6WriteOption(localAddrId, 39u, (uint16)length, data);
        }
#endif/*STD_ON == SOAD_IPV6_ADDRESS_ENABLED*/
    }
    return result;
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x12
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
RemoteAddrPtr Pointer to memory containing IP address and port of the
remote host which sent the data.
BufPtr Pointer to the received data.
Length Data length of the received TCP segment or UDP datagram.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: The TCP/IP stack calls this primitive after the reception of data on a socket. The
socket identifier along with configuration information determines which module is
to be called.
 */
#define SOAD_START_SEC_SOADRXINDICATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_RxIndication(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) Length)
{
#if(0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    boolean socketIdValid;
    SoAd_uintx soConGroupId;
    SoAd_SoConIdType soConId = NO_TXMETADATA;
#if (STD_ON == SOAD_SUPPORT_UDP)
    /*used to revert the remote address change*/
    SoAd_FindTheValidSoConType parameter;
    TcpIp_SockAddrType oldRemoteAddr;
    boolean remoteAddrChanged = FALSE;
#endif
    boolean validPduData = TRUE;
    SoAd_uintx socketRouteId;
#endif/*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00264*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00268*/
    if ((TRUE == detNoErr) && (NULL_PTR == RemoteAddrPtr))
    {
        SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if ((TRUE == detNoErr) && ((0u == Length) || (NULL_PTR == BufPtr)))
    {
        SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
#if (STD_ON == SOAD_SUPPORT_UDP)
        parameter.RemoteAddr = &oldRemoteAddr;
        parameter.RemoteAddrChanged = &remoteAddrChanged;
#endif
#if(0u < SOAD_SOCKET_ROUTE_DEST_MAX)
        /*find the valid Socket Connection*/
        socketIdValid = SoAd_FindTheValidSoCon(
                            SocketId,
                            RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
                            parameter,
#endif
                            &soConId);
        /*handle the received TCP segment and UDP message */
        if(TRUE == socketIdValid)
        {
            if((NO_TXMETADATA != soConId)
                    && (FALSE == SoAd_SoConManager[soConId].SocketNeedClose))
            {
                soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
#if (STD_ON == SOAD_SUPPORT_HEADERID)
                if(TRUE == SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_PduHeaderEnable)
                {
                    if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                    {
#if(0u < SOAD_RXBUFFER_NUM)
                        SoAd_HeaderEnableTcpSoConRxHandle(soConId, Length, BufPtr);
#endif/*0u < SOAD_RXBUFFER_NUM*/
                    }
#if (STD_ON == SOAD_SUPPORT_UDP)
                    /*header enable and UDP socket connection*/
                    else
                    {
                        validPduData = SoAd_HeaderEnableUdpSoConRxHandle(soConId, Length, BufPtr);
                    }
#endif/* STD_ON == SOAD_SUPPORT_UDP */
                }
                /*Header is disabled,just one socket route ref the SoCon*/
                else
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
                {
                    if(NULL_PTR != SOAD_SOCON_CFG(soConId).SoAd_RefSocketRoutePtr)
                    {
                        socketRouteId = SOAD_SOCON_CFG(soConId).SoAd_RefSocketRoutePtr[0u];
                        if(SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
                        {
                            #if(0u < SOAD_RXBUFFER_NUM)
                            if(TRUE == SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
                            {
                                SoAd_HeaderDisableSoConRxTpPduHandle(soConId, socketRouteId, Length, BufPtr);
                            }
                            #if (STD_ON == SOAD_SUPPORT_UDP)
                            else
                            {
                                validPduData = FALSE;
                            }
                            #endif/*STD_ON == SOAD_SUPPORT_UDP*/
                            #endif/*0u < SOAD_RXBUFFER_NUM*/
                        }
                        else
                        {
                            /*Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or SoAdRxUpperLayerType is TP)*/
                            if(TRUE == SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable)
                            {
                                SoAd_HeaderDisableSoConRxIfPduHandle(soConId, socketRouteId, Length, BufPtr);
                            }
                            #if (STD_ON == SOAD_SUPPORT_UDP)
                            else
                            {
                                validPduData = FALSE;
                            }
                            #endif/*STD_ON == SOAD_SUPPORT_UDP*/
                        }
                    }
                }
#if (STD_ON == SOAD_SUPPORT_UDP)
                /*SWS_SoAd_00710*/
                if((FALSE == validPduData) && (TRUE == remoteAddrChanged))
                {
                    SoAd_SoConManager[soConId].RemoteAddr = oldRemoteAddr;
                    SoAd_SoConManager[soConId].UdpAliveSupervisionTime = 0u;
                    SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                }
#endif/*STD_ON == SOAD_SUPPORT_UDP*/
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        else
        {
            /*SWS_SoAd_00267*/
            SOAD_DET_REPORT(SOAD_SID_RXINDICATION, SOAD_E_INV_SOCKETID);
        }
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
#endif/*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/
    }
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(RemoteAddrPtr);
    SOAD_NOUSED(BufPtr);
    SOAD_NOUSED(Length);
    return;
}
#define SOAD_STOP_SEC_SOADRXINDICATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x13
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
BufPtr Pointer to buffer for transmission data.
BufLength Length of provided data buffer.
Parameters (inout): None
Parameters (out): None
Return value: BufReq_ReturnType BUFREQ_OK: Data has been copied to the transmit buffer
completely as requested.
BUFREQ_E_NOT_OK: Data has not been copied. Request
failed. (No further action for TcpIp required. Later the upper
layer might either close the socket or retry the transmit request)
Description: This service requests to copy data for transmission to the buffer indicated.
This call is triggered by TcpIp_Transmit(). Note: The call to <Up>_CopyTxData()
may happen in the context of TcpIp_Transmit().
 */
#define SOAD_START_SEC_SOADCOPYTXDATA_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(BufReq_ReturnType, SOAD_CODE)
SoAd_CopyTxData(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) BufLength)
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    #if (0u < SOAD_TXBUFFER_NUM)
    SoAd_SoConIdType soConId;
    #endif
    #if ((0u < SOAD_TXBUFFER_NUM) || (STD_ON == SOAD_DEV_ERROR_DETECT))
    SoAd_SoConIdType cnt;
    boolean socketIdValid = FALSE;
    for(cnt = 0u;(cnt < SOAD_SOCON_NUM) && (FALSE == socketIdValid);cnt++)
    {
        if((SocketId == SoAd_SoConManager[cnt].TcpIpSocketId)
                && (SOAD_SOCON_ONLINE == SoAd_SoConManager[cnt].SoAd_SoConMode)
                && (TRUE == SoAd_SoConManager[cnt].TpPduTransmitting)
                && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(cnt).SoAd_TcpUdpProtocol))
        {
            socketIdValid = TRUE;
            #if (0u < SOAD_TXBUFFER_NUM)
            soConId = cnt;
            #endif
        }
    }
    #endif/*(0u < SOAD_TXBUFFER_NUM) || (STD_ON == SOAD_DEV_ERROR_DETECT)*/
    #if (STD_ON == SOAD_DEV_ERROR_DETECT)
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_COPYTXDATA, SOAD_E_NOTINIT);
    }
    else if(TRUE != socketIdValid)
    {
        SOAD_DET_REPORT(SOAD_SID_COPYTXDATA, SOAD_E_INV_SOCKETID);
    }
    else
    #endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        #if (0u < SOAD_TXBUFFER_NUM)
        if((TRUE == socketIdValid) && (NULL_PTR != BufPtr) && (0u < BufLength))
        {
            PduInfoType PduInfo;
            PduIdType pduRouteId;
            SoAd_UpLayerModuleType upModule;
            PduLengthType validDataLength;
            SoAd_uintx txBufferId,pduRouteDestId;
            txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(soConId);
            PduInfo.MetaDataPtr = NULL_PTR;
            PduInfo.SduLength = BufLength;
            PduInfo.SduDataPtr = BufPtr;
            pduRouteId = SoAd_SoConManager[soConId].ActiveSoAdPduRouteId;
            upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
            #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr)
            {
                ret = SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr(
                              SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId,
                              &PduInfo,
                              NULL_PTR,
                              &validDataLength);
                if(BUFREQ_OK == ret)
                {
                    SchM_Enter_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
                    SoAd_SoTxBufMan[txBufferId].upCopyLength += BufLength;
                    if((TRUE == SOAD_SOCONGROUP_TCPIMME_TXCONF(SOAD_SOCKET_LINKGORUP(soConId)))
                            && (SoAd_SoTxBufMan[txBufferId].txPduTotalLength ==
                                    SoAd_SoTxBufMan[txBufferId].upCopyLength))
                    {
                        SoAd_SoTxBufMan[txBufferId].txPduTotalLength = 0u;
                        SoAd_SoTxBufMan[txBufferId].upCopyLength = 0u;
                        SoAd_SoConManager[soConId].TpPduTransmitting = FALSE;
                        if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
                        {
                            SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                                    SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId,
                                    E_OK);
                        }
                    }
                    SoAd_SoConManager[soConId].TxNoAckLength += BufLength;
                    pduRouteDestId = SOAD_PDUROUT_CFG(pduRouteId).SoAd_PduRouteDestIndex[0];
                    SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength = SoAd_SoConManager[soConId].TxNoAckLength;
                    SchM_Exit_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
                }
                else
                {
                    SoAd_TpPduTxErrorHandle(soConId,
                            txBufferId,
                            upModule,
                            SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId);
                    ret = BUFREQ_E_NOT_OK;
                }
            }
            #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
        }
        #endif/*0u < SOAD_TXBUFFER_NUM*/
    }
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(BufPtr);
    SOAD_NOUSED(BufLength);
    return ret;
}
#define SOAD_STOP_SEC_SOADCOPYTXDATA_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x14
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
Length Number of transmitted data bytes.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: The TCP/IP stack calls this function after the data has been acknowledged by the
peer for TCP.
Caveats: The upper layer might not be able to determine exactly which data bytes
have been confirmed.
 */
#define SOAD_START_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_TxConfirmation(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(uint16, AUTOMATIC) Length)
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConcnt;
    SoAd_SoConIdType soConId = NO_TXMETADATA;
    for(soConcnt = 0u;(soConcnt < SOAD_SOCON_NUM) && (NO_TXMETADATA == soConId);soConcnt++)
    {
        if((SocketId == SoAd_SoConManager[soConcnt].TcpIpSocketId)
                && (SOAD_SOCON_ONLINE == SoAd_SoConManager[soConcnt].SoAd_SoConMode)
                && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConcnt).SoAd_TcpUdpProtocol))
        {
            soConId = soConcnt;
        }
    }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TXCONFIRMATION, SOAD_E_NOTINIT);
    }
    else if(NO_TXMETADATA == soConId)
    {
        SOAD_DET_REPORT(SOAD_SID_TXCONFIRMATION, SOAD_E_INV_SOCKETID);
    }
    /* SWS_SoAd_00271 */
    else if((0u == Length) || (SoAd_SoConManager[soConId].TxNoAckLength < Length))
    {
        SOAD_DET_REPORT(SOAD_SID_TXCONFIRMATION, SOAD_E_INV_ARG);
    }
    else
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        if(NO_TXMETADATA != soConId)
        {
            SoAd_uintx cnt,pduRouteDestId;
            SoAd_SoConManager[soConId].TxNoAckLength -= Length;
            for(cnt = 0u;cnt < SOAD_SOCON_CFG(soConId).SoAd_RefPduRouteDestNum;cnt++)
            {
                pduRouteDestId = SOAD_SOCON_CFG(soConId).SoAd_RefPduRouteDestPtr[cnt];
                if(0u < SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength)
                {
                    SoAd_TxConfirmationHandle(soConId,Length,pduRouteDestId);
                }
            }
        }
    }
#endif/*STD_ON == SOAD_SUPPORT_TCP*/
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(Length);
    return;
}
#define SOAD_STOP_SEC_SOADTXCONFIRMATION_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x15
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource which
has been used at TcpIp_Bind()
SocketIdConnected Socket identifier of the local socket resource used for the
established connection.
RemoteAddrPtr IP address and port of the remote host.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK upper layer accepts the established connection
E_NOT_OK upper layer refuses the established
connection, TcpIp stack shall close the connection.
Description: This service gets called if the stack put a socket into the listen mode before (as
server) and a peer connected to it (as client).
In detail: The TCP/IP stack calls this function after a socket was set into the listen
state with TcpIp_TcpListen() and a TCP connection is requested by the peer.
 */
#define SOAD_START_SEC_SOADTCPACCEPTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TcpAccepted(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketIdConnected,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConId = 0u;
    SoAd_uintx soConGroupId;
    boolean soConGroupValid = FALSE;
    boolean soConValid = FALSE;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00272*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPACCEPTED, SOAD_E_NOTINIT);
    }
    else if(NULL_PTR == RemoteAddrPtr)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPACCEPTED, SOAD_E_PARAM_POINTER);
    }
    else
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        for(soConGroupId = 0u;
                (soConGroupId < SOAD_SO_CON_GROUP_NUM)
                && (FALSE == soConGroupValid); soConGroupId++)
        {
            if((TRUE == SoAd_SoConGroupManager[soConGroupId].TcpIpSocketValid)
                    && (SocketId == SoAd_SoConGroupManager[soConGroupId].TcpIpSocketId)
                    && (NULL_PTR != SOAD_TCPTYPE_CFG(soConGroupId))
                    && (FALSE == SOAD_TCPTYPE_CFG(soConGroupId)->SoAd_SocketTcpInitiate))
            {
                if(TRUE == SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SocketMsgAcceptanceFilterEnabled)
                {
                    if(TRUE == SoAd_GetBestMatchAlgorithmSoCon(soConGroupId, RemoteAddrPtr, &soConId, SOAD_SOCON_MATCH_REQUEST_RECONNECT))
                    {
                        soConValid = TRUE;
                    }
                }
                else
                {
                    /*if SoAd_SocketMsgAcceptanceFilterEnabled is FALSE:the SoConGroup just have only one SoCon*/
                    soConId = SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SoConId[0u];
                    if((SoAd_SoConManager[soConId].TcpIpSocketValid == FALSE)
                        && (SoAd_SoConManager[soConId].SoAd_SoConMode == SOAD_SOCON_RECONNECT))
                    {
                        soConValid = TRUE;
                    }   
                }
                if(TRUE == soConValid)
                {
                    /*Set the new tcp communication socket NoDelay flag*/
                    if (NULL_PTR != SOAD_TCPTYPE_CFG(soConGroupId)->SoAd_SocketTcpNoDelay)
                    {
                        uint8 delayVlu = 0x01u;
                        if (TRUE == SOAD_TCPTYPE_CFG(soConGroupId)->SoAd_SocketTcpNoDelay[0])
                        {
                            delayVlu = 0x0u;
                        }
                        (void)TcpIp_ChangeParameter(SocketIdConnected, TCPIP_PARAMID_TCP_NAGLE, &delayVlu);
                    }
                    /*SWS_SoAd_00636:At SoAd_TcpAccepted(), SoAd shall perform the following
                    actions if the TCP SoAdSocketConnectionGroup related to SocketId has both
                    SoAdSocketTcpInitiate and SoAdSocketMsgAcceptanceFilterEnabled set to FALSE
                    and is not online (i.e. current connection state not SOAD_SOCON_ONLINE)*/
                    SoAd_SoConManager[soConId].RemoteAddrIsSet = TRUE;
                    SoAd_SoConManager[soConId].RemoteAddr = *RemoteAddrPtr;
                    SoAd_SoConManager[soConId].TcpIpSocketValid = TRUE;
                    SoAd_SoConManager[soConId].TcpIpSocketId = SocketIdConnected;
                    SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                    SoAd_SoConGroupManager[soConGroupId].SoConSocketValidNum += 1u;
                    result = E_OK;
                }
                soConGroupValid = TRUE;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        /*SWS_SoAd_00273*/
        if (TRUE != soConGroupValid)
        {
            SOAD_DET_REPORT(SOAD_SID_TCPACCEPTED, SOAD_E_INV_SOCKETID);
        }
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    }
#endif/* STD_ON == SOAD_SUPPORT_TCP */
    SOAD_NOUSED(SocketId);
    SOAD_NOUSED(SocketIdConnected);
    SOAD_NOUSED(RemoteAddrPtr);
    return result;
}
#define SOAD_STOP_SEC_SOADTCPACCEPTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x16
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called if the stack initiated a TCP connection
before (as client) and the peer (the server) acknowledged the connection set up.
In detail:
The TCP/IP stack calls this function after a socket was requested to connect with
TcpIp_TcpConnect() and a TCP connection is confirmed by the peer.
The parameter value of SocketId equals the SocketId value of the preceeding
TcpIp_TcpConnect() call.
 */
#define SOAD_START_SEC_SOADTCPCONNECTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_TcpConnected(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId)
{
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_SoConIdType soConId;
    SoAd_uintx soConGroupId;
    boolean socketIdValid = FALSE;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00274*/
    if(SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPCONNECTED, SOAD_E_NOTINIT);
    }
    else
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        for(soConId = 0u; (soConId < SOAD_SOCON_NUM)
                && (FALSE == socketIdValid); soConId++)
        {
            soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
            /*SWS_SoAd_00593:Within SoAd_TcpConnected() SoAd shall change
             * the state of the socket connection to SOAD_SOCON_ONLINE*/
            if((TRUE == SoAd_SoConManager[soConId].TcpIpSocketValid)
                    && (SocketId == SoAd_SoConManager[soConId].TcpIpSocketId)
                    && (TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(soConId).SoAd_TcpUdpProtocol)
                    && (TRUE == SOAD_TCPTYPE_CFG(soConGroupId)->SoAd_SocketTcpInitiate))
            {
                if(SOAD_SOCON_ONLINE != SoAd_SoConManager[soConId].SoAd_SoConMode)
                {
                    SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                }
                socketIdValid = TRUE;
            }
        }
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
        if(FALSE == socketIdValid)
        {
            SOAD_DET_REPORT(SOAD_SID_TCPCONNECTED, SOAD_E_INV_SOCKETID);
        }
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    }
#endif /* STD_ON == SOAD_SUPPORT_TCP */
    SOAD_NOUSED(SocketId);
    return;
}
#define SOAD_STOP_SEC_SOADTCPCONNECTED_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x17
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource.
Event This parameter contains a description of the event just encountered.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called if the stack encounters a condition described by the
values in Event.
 */
#define SOAD_START_SEC_SOADTCPIPEVENT_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_TcpIpEvent(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_EventType, AUTOMATIC) Event)
{
    SoAd_uintx cnt;
#if (STD_ON == SOAD_SUPPORT_TCP)
    SoAd_uintx soConGroupId;
    SoAd_SoConIdType soConId;
#endif/*STD_ON == SOAD_SUPPORT_TCP*/
    boolean socketIdValid = FALSE;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00276*/
    if (SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    /*SWS_SoAd_00278*/
    if ((TRUE == detNoErr) && (Event > TCPIP_TLS_HANDSHAKE_SUCCEEDED))
    {
        SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT,  SOAD_E_INV_ARG);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        if(TCPIP_TLS_HANDSHAKE_SUCCEEDED != Event)
        {
            /*find a udp SoCon Group or a tcp SoCon Group(listen socket)*/
            for(cnt = 0u; (cnt < SOAD_SO_CON_GROUP_NUM)
                    && (FALSE == socketIdValid); cnt++)
            {
                if((TRUE == SoAd_SoConGroupManager[cnt].TcpIpSocketValid)
                        && (SocketId == SoAd_SoConGroupManager[cnt].TcpIpSocketId))
                {
                    #if (STD_ON == SOAD_SUPPORT_TCP)
                    if(NULL_PTR != SOAD_TCPTYPE_CFG(cnt))
                    {
                        if(TCPIP_TCP_CLOSED == Event)
                        {
                            /*SWS_SoAd_00645:TCPIP_TCP_CLOSED for a Listen-Socket*/
                            SoAd_SoConGroupManager[cnt].TcpIpSocketValid = FALSE;
                        }
                        #if (STD_ON == SOAD_DEV_ERROR_DETECT)
                        /*SWS_SoAd_00278*/
                        else
                        {
                            SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT,  SOAD_E_INV_ARG);
                        }
                        #endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
                    }
                    #endif/* STD_ON == SOAD_SUPPORT_TCP */
                    #if (STD_ON == SOAD_SUPPORT_UDP)
                    if(NULL_PTR != SOAD_UDPTYPE_CFG(cnt))
                    {
                        SoAd_CloseUdpSoConGroup(Event,cnt);
                    }
                    #endif/* STD_ON == SOAD_SUPPORT_UDP */
                    socketIdValid = TRUE;
                }
            }

            /*find a tcp socket connection*/
            #if (STD_ON == SOAD_SUPPORT_TCP)
            for(soConId = 0u; (soConId < SOAD_SOCON_NUM) && (FALSE == socketIdValid); soConId++)
            {
                /*SWS_SoAd_00646,SWS_SoAd_00688*/
                if((TRUE == SoAd_SoConManager[soConId].TcpIpSocketValid)
                        && (SocketId == SoAd_SoConManager[soConId].TcpIpSocketId)
                        && (TCPIP_UDP_CLOSED != Event))
                {
                    /*Adjust the standard SWS_SoAd_00646*/
                    if(SOAD_SOCON_OFFLINE != SoAd_SoConManager[soConId].SoAd_SoConMode)
                    {
                        soConGroupId = SOAD_SOCON_CFG(soConId).SoAd_SocketConnectionGroupRef;
                        if(TCPIP_TCP_FIN_RECEIVED == Event)
                        {
                            SoAd_SoConCloseHandle(soConId,soConGroupId,TRUE);
                        }
                        else
                        {
                            SoAd_SoConManager[soConId].TcpIpSocketValid = FALSE;
                            SoAd_SoConCloseHandle(soConId,soConGroupId,FALSE);
                            if(0u < SoAd_SoConGroupManager[soConGroupId].SoConSocketValidNum)
                            {
                                (SoAd_SoConGroupManager[soConGroupId].SoConSocketValidNum) -= 1u;
                            }
                        }
                        /*Notification UP Module SoCon changed*/
                        if(SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
                        {
                            if(SoAd_SoConManager[soConId].ChangeModeToOffline == TRUE)
                            {
                                SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_OFFLINE;
                                SoAd_SoConManager[soConId].ChangeModeToOffline = FALSE;
                            }
                            else
                            {
                                SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                            }
                            SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, soConGroupId);
                        }
                        /*server-side,think about close the"listen socket"*/
                        if((FALSE == SOAD_TCPTYPE_CFG(soConGroupId)->SoAd_SocketTcpInitiate)
                                && (0u == SoAd_SoConGroupManager[soConGroupId].SoConSocketValidNum))
                        {
                            (void)TcpIp_Close(SoAd_SoConGroupManager[soConGroupId].TcpIpSocketId,
                                              SoAd_SoConManager[soConId].SoConCloseAbort);
                        }
                    }
                    else
                    {
                        SoAd_SoConManager[soConId].TcpIpSocketValid = FALSE;
                    }
                    socketIdValid = TRUE;
                }
            }
            #endif/*SOAD_SUPPORT_TCP*/
            #if (STD_ON == SOAD_DEV_ERROR_DETECT)
            /*SWS_SoAd_00277*/
            if(FALSE == socketIdValid )
            {
                SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT, SOAD_E_INV_SOCKETID);
            }
            #endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
        }
    }
    return;
}
#define SOAD_STOP_SEC_SOADTCPIPEVENT_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x18
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): IpAddrId IP address Identifier, representing an IP address specified in the TcpIp
module configuraiton (e.g. static IPv4 address on EthIf controller 0).
State state of IP address assignment
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called by the TCP/IP stack if an IP address assignment changes
(i.e. new address assigned or assigned address becomes invalid).
 */
#define SOAD_START_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_LocalIpAddrAssignmentChg(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) IpAddrId,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) State)
{
    SoAd_uintx cnt0;
    SoAd_SoConIdType soConId,cnt1;
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    /*SWS_SoAd_00279*/
    if(SOAD_STATE_UNINIT == SoAd_Module_Status)
    {
        SOAD_DET_REPORT(SOAD_SID_LOCALIPADDRASSIGNMENTCHG, SOAD_E_NOTINIT);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif/* STD_ON == SOAD_DEV_ERROR_DETECT */
    {
        /*SWS_SoAd_00729*/
        for(cnt0 = 0u; (cnt0 < SOAD_SO_CON_GROUP_NUM); cnt0++)
        {
            if(IpAddrId == SOAD_SOCKETLOCALADDR_CFG(cnt0).LocalAddrId)
            {
                SoAd_SoConGroupManager[cnt0].LocalIpAddrState = State;
                for(cnt1 = 0u;cnt1 < SOAD_SOCONGROUP_CFG(cnt0).SoAd_SoConNum;cnt1++)
                {
                    soConId = SOAD_SOCONGROUP_CFG(cnt0).SoAd_SoConId[cnt1];
                    if((SOAD_SOCON_OFFLINE != SoAd_SoConManager[soConId].SoAd_SoConMode)
                            && (TCPIP_IPADDR_STATE_UNASSIGNED == State))
                    {
                        SoAd_SoConManager[soConId].SocketNeedClose = TRUE;
                    }
                    SoAd_SoConLocalIpAddrAssignmentChg(soConId, cnt0);
                }
            }
        }
    }
    return;
}
#define SOAD_STOP_SEC_SOADLOCALIPADDRASSIGNMENTCHG_CALLBACK_CODE
#include "SoAd_MemMap.h"
/*============================================================================*/
/*
Service ID[hex]: 0x19
Description: Schedules the Socket Adaptor. (Entry point for scheduling)
 */
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
FUNC(void, SOAD_CODE)
SoAd_MainFunction(VAR(void, AUTOMATIC))
{
    SoAd_SoConIdType soConCnt;
    if (SOAD_STATE_INIT == SoAd_Module_Status)
    {
        for (soConCnt = 0u; soConCnt < SOAD_SOCON_NUM; soConCnt++)
        {
#if (0u < SOAD_TXBUFFER_NUM)
            if(NULL_PTR != SOAD_SOCKET_TXBUF_INDEX_CFG(soConCnt))
            {
                SoAd_MainFunctionTpTransmitHandle(soConCnt);
            }
#endif/*0u < SOAD_TXBUFFER_NUM*/
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
            SoAd_MainFunctionNPduUdpTxHandle(soConCnt);
#endif/*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/
            SoAd_ReleaseRemoteAddrMainFunctionHandle(soConCnt);
#if (STD_ON == SOAD_SUPPORT_UDP)
            SoAd_UdpAliveSupervisionTimeHandle(soConCnt);
#endif/* STD_ON == SOAD_SUPPORT_UDP */
            /*handle the Open/Close of requested(SoAd_OpenSoCon,SoAd_CloseSoCon,
             * SoAdSocketAutomaticSoConSetup) socket connections;
             * handle the Disconnection and recovery of SoCon*/
            SoAd_SoConModeChangeHandle(soConCnt);
#if(0u < SOAD_RXBUFFER_NUM)
            /*copy the PDU data(in the Rx Buffer) to the Up module*/
            SoAd_SoConRxDataMainFunctionHandle(soConCnt);
#endif/*0u < SOAD_RXBUFFER_NUM*/
        }
        SoAd_MainFunctionTxConfirmationHandle();
    }

    return;
}
#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"
/*==============================================================================
*                                       LOCAL FUNCTIONS
==============================================================================*/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"
static FUNC(void, SOAD_CODE)
SoAd_SocnTransmitAutomaticSoConSetupHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_ProtocolType, AUTOMATIC) Protocol)
{
    SoAd_uintx soConGroup;
    /* socket connection of type automatic (i.e.
    configuration parameter SoAdSocketAutomaticSoConSetup set to TRUE)
    which uses a wildcard in the configured remote address */
    soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    if(TRUE == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SocketAutomaticSoConSetup)
    {
        P2CONST(TcpIp_SockAddrType, TYPEDEF, SOAD_APPL_CONST)
        soConRemoteAdrCfgPtr = SOAD_SOCON_CFG(SoConId).SoAd_SocketRemoteAddress;
        if((NULL_PTR != soConRemoteAdrCfgPtr)
                && ((TCPIP_IS_IPADDR_ANY(*soConRemoteAdrCfgPtr))
                    || (TCPIP_PORT_ANY == soConRemoteAdrCfgPtr->port)))
        {
#if (STD_ON == SOAD_SUPPORT_TCP)
            if (TCPIP_IPPROTO_TCP == Protocol)
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
            }
#endif/* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
            if (TCPIP_IPPROTO_UDP == Protocol)
            {
                SoAd_SoConManager[SoConId].RemoteAddr = *soConRemoteAdrCfgPtr;
                SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            }
#endif/* STD_ON == SOAD_SUPPORT_UDP */
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_PduRouteManagerInit(PduIdType TxPduId)
{
    SoAd_PduRoutManager[TxPduId].PduDataPtr = NULL_PTR;
    SoAd_PduRoutManager[TxPduId].PduLength = 0u;
    SoAd_PduRoutManager[TxPduId].TxPendingNum = SOAD_UNUSED_UINT8;
    SoAd_PduRoutManager[TxPduId].TxSoConId = NO_TXMETADATA;
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_RoutingGroupInit(void)
{
/*init all RouteGroup enable state*/
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    SoAd_uintx cnt1;
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_uintx cnt2;
#endif
#if (0u < SOAD_ROUTING_GROUP_MAX)
    SoAd_RoutingGroupIdType cnt0;
    SoAd_RoutingGroupIdType routeGroupId;
    for(cnt0 = 0u;cnt0 < SOAD_ROUTING_GROUP_NUM;cnt0++)
    {
        SoAd_RoutGroupManager[cnt0].RoutGroupEnable =
                SOAD_ROUTGROUP_CFG(cnt0).SoAd_RoutingGroupIsEnabledAtInit;
    }
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
/*init all SocketRouteDest enable state*/
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    for(cnt1 = 0u;cnt1 < SOAD_SOCKET_ROUTE_DEST_NUM;cnt1++)
    {
        if(0u == SOAD_SRDEST_CFG(cnt1).SoAd_RxRoutingGroupRefNum)
        {
            SoAd_SocketRouteDestManager[cnt1].SocketRouteDestEnable = TRUE;
        }
#if (0u < SOAD_ROUTING_GROUP_MAX)
        else
        {
            for(cnt0 = 0u;(cnt0 < SOAD_SRDEST_CFG(cnt1).SoAd_RxRoutingGroupRefNum)
                    && (FALSE == SoAd_SocketRouteDestManager[cnt1].SocketRouteDestEnable);cnt0++)
            {
                routeGroupId = SOAD_SRDEST_CFG(cnt1).SoAd_RxRoutingGroupRefPtr[cnt0];
                if(TRUE == SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    SoAd_SocketRouteDestManager[cnt1].SocketRouteDestEnable = TRUE;
                }
            }
        }
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
    }
#endif/*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/
/*init all PduRouteDest enable state*/
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    for(cnt2 = 0u;cnt2 < SOAD_PDU_ROUTE_DEST_NUM;cnt2++)
    {
        SoAd_PduRouteDestManager[cnt2].TxPendingLength = 0u;
        if(0u == SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxRoutingGroupRefNum)
        {
            SoAd_PduRouteDestManager[cnt2].PduRouteDestEnable = TRUE;
        }
#if (0u < SOAD_ROUTING_GROUP_MAX)
        else
        {
            for(cnt0 = 0u;(cnt0 < SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxRoutingGroupRefNum)
                    && (FALSE == SoAd_PduRouteDestManager[cnt2].PduRouteDestEnable);cnt0++)
            {
                routeGroupId = SOAD_PDUROTDEST_CFG(cnt2).SoAd_TxRoutingGroupRefPtr[cnt0];
                if(TRUE == SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    SoAd_PduRouteDestManager[cnt2].PduRouteDestEnable = TRUE;
                }
            }
        }
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/
    }
#endif/*0u < SOAD_PDU_ROUTE_DEST_MAX*/
    return;
}

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_GetIfPduData(PduIdType TxPduId)
{
    Std_ReturnType result = E_NOT_OK;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metadata[2] = {0xFF,0xFF};
    upModule = SOAD_PDUROUT_CFG(TxPduId).SoAd_RefModule;
    #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TriggerTransmit_FuncPtr)
    {
        pduInfo.SduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
        pduInfo.SduLength = (SOAD_MAX_IF_PDU_SIZE - SOAD_HEADER_SIZE);
        if(TRUE == SOAD_PDUROUT_CFG(TxPduId).SoAd_MetaDataEnable)
        {
            pduInfo.MetaDataPtr = &metadata[0];
        }
        else
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
        result = SOAD_UPPERLAY_PCCFG(upModule).If_TriggerTransmit_FuncPtr(
                     SOAD_PDUROUT_CFG(TxPduId).SoAd_UpPduId,
                     &pduInfo);
    }
    #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
    if(E_OK == result)
    {
        SoAd_PduRoutManager[TxPduId].PduDataPtr = &SoAd_IfPduBuffer[SOAD_HEADER_SIZE];
        SoAd_PduRoutManager[TxPduId].PduLength = pduInfo.SduLength;
        if(TRUE == SOAD_PDUROUT_CFG(TxPduId).SoAd_MetaDataEnable)
        {
            SOAD_METADATATOSOCONID(SoAd_PduRoutManager[TxPduId].TxSoConId,
                pduInfo.MetaDataPtr);
        }
        else
        {
            SoAd_PduRoutManager[TxPduId].TxSoConId = NO_TXMETADATA;
        }
    }
    return result;
}
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/

static FUNC(boolean, SOAD_CODE)
SoAd_SoConRemoteAddrLocked(SoAd_SoConIdType SoConId)
{
    boolean remoteAddrLocked = FALSE;
#if((0u < SOAD_RXBUFFER_NUM) || (0u < SOAD_TXBUFFER_NUM))
    SoAd_uintx bufferId;
#endif
    /*SWS_SoAd_00532*/
    if(SOAD_SOCON_OFFLINE == SoAd_SoConManager[SoConId].SoAd_SoConMode)
    {
        remoteAddrLocked = FALSE;
    }
    else if(TCPIP_IPPROTO_UDP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
#if(0u < SOAD_RXBUFFER_NUM)
        /*if the Rx PDU is Receiving(UDP),not set the remote addr*/
        if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr)
        {
            bufferId = (SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u]
                             - SOAD_TXBUFFER_NUM);
            /*the Rx Buffer is empty and the Residue Rx Pdu length is 0*/
            if((0u < SoAd_SoRxBufMan[bufferId].ExitPduResidueLength)
                    || (SoAd_SoRxBufMan[bufferId].EnterPosition
                            != SoAd_SoRxBufMan[bufferId].ExitPosition))
            {
                remoteAddrLocked = TRUE;
            }
        }
#endif/*0u < SOAD_RXBUFFER_NUM*/
#if(0u < SOAD_TXBUFFER_NUM)
        /*if the Tx PDU is transmitting(UDP),not set the remote addr*/
        if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_TxBufCfgIndexPtr)
        {
            bufferId = SOAD_SOCON_CFG(SoConId).SoAd_TxBufCfgIndexPtr[0u];
            if(0u < SoAd_SoTxBufMan[bufferId].txPduTotalLength)
            {
                remoteAddrLocked = TRUE;
            }
        }
#endif/*0u < SOAD_TXBUFFER_NUM*/
    }
    else
    {
        remoteAddrLocked = TRUE;
    }
    return remoteAddrLocked;
}

static FUNC(void, SOAD_CODE)
SoAd_ReleaseRemoteAddrHandle(
        SoAd_SoConIdType SoConId,
        SoAd_uintx SoConGroupId)
{
    if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_SocketRemoteAddress)
    {
        SoAd_SoConManager[SoConId].RemoteAddrIsSet = TRUE;
        SoAd_SoConManager[SoConId].RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
    }
    /*Standard undefined,release to wildcards*/
    else
    {
        TCPIP_SET_IPADDR_ANY(SoAd_SoConManager[SoConId].RemoteAddr);
        SoAd_SoConManager[SoConId].RemoteAddr.port = TCPIP_PORT_ANY;
    }
    /*SWS_SoAd_00687*/
    if((SOAD_SOCON_ONLINE == SoAd_SoConManager[SoConId].SoAd_SoConMode)
            && (TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[SoConId].RemoteAddr)
                || (TCPIP_PORT_ANY == SoAd_SoConManager[SoConId].RemoteAddr.port)))
    {
        SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroupId);
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_ReleaseRemoteAddrMainFunctionHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    if(TRUE == SoAd_SoConManager[SoConId].RemoteAddrNeedRelease)
    {
        if(FALSE == SoAd_SoConRemoteAddrLocked(SoConId))
        {
            SoAd_ReleaseRemoteAddrHandle(SoConId, soConGroup);
            SoAd_SoConManager[SoConId].RemoteAddrNeedRelease = FALSE;
        }
    }
    return;
}
/* soad local memcpy function,imitation lib memcpy */
static FUNC(void, SOAD_CODE)
SoAd_LocalMemCpy(
    P2VAR(uint8, AUTOMATIC, SOAD_VAR) destPtr,
    P2CONST(uint8, AUTOMATIC, SOAD_VAR) srcPtr,
    VAR(PduLengthType, AUTOMATIC) len)
{
    PduLengthType dataLength = len;
    while(dataLength > 0UL)
    {
        *destPtr = *srcPtr;
        destPtr++;
        srcPtr++;
        dataLength--;
    }
    return;
}


#if(0u < SOAD_RXBUFFER_NUM)

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableNewPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    PduLengthType bufferedDataLength,bufStartId;
    PduLengthType bufferLength,bufferStartPos;
    boolean findValidSocketRoute;
    uint32 header;
    PduLengthType pduLength;
    SoAd_uintx cnt,socketRouteId;
    PduLengthType upBufferSize;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metaData[2u];
    bufferLength = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufPos;
    if(SoAd_SoRxBufMan[BufferManageId].ExitPosition <
            SoAd_SoRxBufMan[BufferManageId].EnterPosition)
    {
        bufferedDataLength = (SoAd_SoRxBufMan[BufferManageId].EnterPosition -
                              SoAd_SoRxBufMan[BufferManageId].ExitPosition);
    }
    else
    {
        bufferedDataLength = (bufferLength + SoAd_SoRxBufMan[BufferManageId].EnterPosition -
                              SoAd_SoRxBufMan[BufferManageId].ExitPosition);
    }
    if(bufferedDataLength >= SOAD_HEADER_SIZE)
    {
        PduLengthType destHeaderLocation;
        findValidSocketRoute = FALSE;
        bufStartId = bufferStartPos + SoAd_SoRxBufMan[BufferManageId].ExitPosition;
        destHeaderLocation = SoAd_SoRxBufMan[BufferManageId].ExitPosition + SOAD_HEADER_SIZE;
        /*the header data is overturn of the buffer*/
        if(destHeaderLocation > bufferLength)
        {
            uint8 headerData[8];
            destHeaderLocation -= bufferLength;
            SoAd_LocalMemCpy(&headerData[0u],
                &SoAd_PduBuffer[bufStartId],
                (SOAD_HEADER_SIZE - destHeaderLocation));
            SoAd_LocalMemCpy(&headerData[SOAD_HEADER_SIZE - destHeaderLocation],
                &SoAd_PduBuffer[bufferStartPos],
                destHeaderLocation);
            header = SOAD_GET_HEADER(headerData, 0u);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(headerData, 0u));
        }
        else
        {
            header = SOAD_GET_HEADER(SoAd_PduBuffer, bufStartId);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(SoAd_PduBuffer, bufStartId));
        }
        for(cnt = 0u; (cnt < SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRouteNum)
                && (FALSE == findValidSocketRoute); cnt++)
        {
            socketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[cnt];
            if(header == *SOAD_SOCKETROUTE_CFG(socketRouteId).SoAd_RxPduHeaderId)
            {
                findValidSocketRoute = TRUE;
            }
        }
        /*find the valid socket route(dest),and the socket route dest is enabled(according to the RouteGroup Manager)*/
        if((TRUE == findValidSocketRoute)
                && (TRUE == SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable))
        {
            SoAd_SoRxBufMan[BufferManageId].SoAd_SocketRouteId = socketRouteId;
            SoAd_SoRxBufMan[BufferManageId].ExitPduResidueLength = pduLength;
            SoAd_SoRxBufMan[BufferManageId].ExitPosition += SOAD_HEADER_SIZE;
            if(SoAd_SoRxBufMan[BufferManageId].ExitPosition >= bufferLength)
            {
                SoAd_SoRxBufMan[BufferManageId].ExitPosition -= bufferLength;
            }
            if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId,SOAD_HEADER_SIZE);
            }
#if (0u < SOAD_MAX_IF_PDU_SIZE)
            /*IF PDU data are in the SoAd Rx Buffer,just is TCP(Header Enable)*/
            if(SOAD_UPPER_LAYER_IF == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
            {
                SoAd_HeaderEnableIfPduHandle(SoConId,BufferManageId);
            }
            else
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/
            {
                if(pduLength > 0u)
                {
                    pduInfo.SduDataPtr = NULL_PTR;
                    pduInfo.SduLength = 0u;
                    if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
                    {
                        SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
                        pduInfo.MetaDataPtr = &metaData[0u];
                    }
                    else
                    {
                        pduInfo.MetaDataPtr = NULL_PTR;
                    }
                    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
                    #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(
                                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                                &pduInfo,
                                pduLength,
                                &upBufferSize))
                    {
                        SoAd_HeaderEnableTpPduHandle(SoConId, upBufferSize);
                    }
                    else
                    {
                        SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
                    }
                    #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
                }
            }
        }
        else
        {
            if(bufferedDataLength >= (pduLength + SOAD_HEADER_SIZE))
            {
                SoAd_SoRxBufMan[BufferManageId].ExitPosition += (pduLength + SOAD_HEADER_SIZE);
                if(SoAd_SoRxBufMan[BufferManageId].ExitPosition >= bufferLength)
                {
                    SoAd_SoRxBufMan[BufferManageId].ExitPosition -= bufferLength;
                }
                if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, (uint32)pduLength + (uint32)SOAD_HEADER_SIZE);
                }
                (void)Det_ReportRuntimeError(SOAD_MODULE_ID,SOAD_INSTANCE,
                        SOAD_SID_MAINFUNCTION,SOAD_E_INV_PDUHEADER_ID);
            }
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableOldPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    SoAd_uintx socketRouteId;
    PduInfoType pduInfo;
    uint8 metaData[2u];
    PduLengthType upBufferSize;
    SoAd_UpLayerModuleType upModule;
    socketRouteId = SoAd_SoRxBufMan[BufferManageId].SoAd_SocketRouteId;
    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
#if (0u < SOAD_MAX_IF_PDU_SIZE)
    if(SOAD_UPPER_LAYER_IF == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
    {
        SoAd_HeaderEnableIfPduHandle(SoConId,BufferManageId);
    }
    else
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/
    {
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 0u;
        if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
        #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                    SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                    &pduInfo,
                    &upBufferSize))
        {
            SoAd_HeaderEnableTpPduHandle(SoConId, upBufferSize);
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                E_NOT_OK);
            SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
        }
        #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) UpBufferSize)
{
    PduLengthType copyToUpLength;
    SoAd_uintx bufferId;
    PduLengthType bufferLength;
    PduLengthType bufferStartPos;
    SoAd_uintx socketRouteId;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metaData[2];
    PduLengthType upBufferSize;
    bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
    bufferLength = SOAD_SOCKETBUFFER_CFG(bufferId).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(bufferId).bufPos;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    socketRouteId = SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId;
    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
    if(SoAd_SoRxBufMan[bufferId].ExitPosition < SoAd_SoRxBufMan[bufferId].EnterPosition)
    {
        copyToUpLength = (SoAd_SoRxBufMan[bufferId].EnterPosition -
                              SoAd_SoRxBufMan[bufferId].ExitPosition);
    }
    else
    {
        copyToUpLength = (bufferLength - SoAd_SoRxBufMan[bufferId].ExitPosition);
    }
    if(copyToUpLength > UpBufferSize)
    {
        copyToUpLength = UpBufferSize;
    }
    if(copyToUpLength >= SoAd_SoRxBufMan[bufferId].ExitPduResidueLength)
    {
        copyToUpLength = SoAd_SoRxBufMan[bufferId].ExitPduResidueLength;
    }
    if(copyToUpLength > 0u)
    {
        pduInfo.SduDataPtr = &SoAd_PduBuffer[bufferStartPos +
                              SoAd_SoRxBufMan[bufferId].ExitPosition];
        pduInfo.SduLength = copyToUpLength;
        if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
        #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                    SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                    &pduInfo,
                    &upBufferSize))
        {
            if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId,
                                        copyToUpLength);
            }
            SoAd_SoRxBufMan[bufferId].ExitPosition += copyToUpLength;
            if(SoAd_SoRxBufMan[bufferId].ExitPosition >= bufferLength)
            {
                SoAd_SoRxBufMan[bufferId].ExitPosition -= bufferLength;
            }
            SoAd_SoRxBufMan[bufferId].ExitPduResidueLength -= copyToUpLength;
            if(0u == SoAd_SoRxBufMan[bufferId].ExitPduResidueLength)
            {
                SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                    SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                    E_OK);
            }
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                E_NOT_OK);
            SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
        }
        #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
    }
    return;
}
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/


#if (0u < SOAD_MAX_IF_PDU_SIZE)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableIfPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    PduLengthType srcPos;
    PduLengthType destPos;
    PduLengthType bufferedDataLength;
    PduLengthType bufferLength;
    PduLengthType bufferStartPos;
    SoAd_uintx socketRouteId;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metaData[2];
    bufferLength = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufPos;
    socketRouteId = SoAd_SoRxBufMan[BufferManageId].SoAd_SocketRouteId;
    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;

    if(SoAd_SoRxBufMan[BufferManageId].ExitPosition <
            SoAd_SoRxBufMan[BufferManageId].EnterPosition)
    {
        bufferedDataLength = (SoAd_SoRxBufMan[BufferManageId].EnterPosition -
                              SoAd_SoRxBufMan[BufferManageId].ExitPosition);
    }
    else
    {
        bufferedDataLength = (bufferLength + SoAd_SoRxBufMan[BufferManageId].EnterPosition -
                              SoAd_SoRxBufMan[BufferManageId].ExitPosition);
    }

    if(bufferedDataLength >= SoAd_SoRxBufMan[BufferManageId].ExitPduResidueLength)
    {
        srcPos = SoAd_SoRxBufMan[BufferManageId].ExitPosition;
        destPos = srcPos + SoAd_SoRxBufMan[BufferManageId].ExitPduResidueLength;
        pduInfo.SduLength = SoAd_SoRxBufMan[BufferManageId].ExitPduResidueLength;
        if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
        #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        if(destPos > bufferLength)
        {
            SchM_Enter_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
            /*SoAd_IfPduBuffer is used for IF TCP receive buffer*/
            SoAd_LocalMemCpy(&SoAd_IfPduBuffer[0u], &SoAd_PduBuffer[bufferStartPos + srcPos],
                        (bufferLength - srcPos));
            SoAd_LocalMemCpy(&SoAd_IfPduBuffer[bufferLength - srcPos], &SoAd_PduBuffer[0u],
                        (destPos - bufferLength));
            pduInfo.SduDataPtr = &SoAd_IfPduBuffer[0u];
            SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                &pduInfo);
            SchM_Exit_SoAd(SOAD_INSTANCE,SOAD_AREA_MSGCONTEXT);
        }
        else
        {
            pduInfo.SduDataPtr = &SoAd_PduBuffer[bufferStartPos + srcPos];
            SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                &pduInfo);
        }
        #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
        /*IF PDU used Rx Buffer must be TCP.
        SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId,
                                SoAd_SoRxBufMan[BufferManageId].ExitPduResidueLength);
        SoAd_SoRxBufMan[BufferManageId].ExitPosition = destPos;
        if(SoAd_SoRxBufMan[BufferManageId].ExitPosition >= bufferLength)
        {
            SoAd_SoRxBufMan[BufferManageId].ExitPosition -= bufferLength;
        }
        SoAd_SoRxBufMan[BufferManageId].ExitPduResidueLength = 0u;
    }
    return;
}
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/
#endif/*0u < SOAD_RXBUFFER_NUM*/

#if(0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_RxDataMainFunctionHeaderDisableHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) BufferManageId)
{
    SoAd_uintx socketRouteId;
    PduInfoType pduInfo;
    uint8 metaData[2u];
    SoAd_UpLayerModuleType upModule;
    PduLengthType upBufferSize;
    PduLengthType copyToUpLength;
    PduLengthType bufferLength;
    PduLengthType bufferStartPos;
    bufferLength = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufLen;
    bufferStartPos = SOAD_SOCKETBUFFER_CFG(BufferManageId + SOAD_TXBUFFER_NUM).bufPos;
    socketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[0u];
    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0u;
    if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
    #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                &pduInfo,
                &upBufferSize))
    {
        if(SoAd_SoRxBufMan[BufferManageId].ExitPosition <
                SoAd_SoRxBufMan[BufferManageId].EnterPosition)
        {
            copyToUpLength = (SoAd_SoRxBufMan[BufferManageId].EnterPosition -
                              SoAd_SoRxBufMan[BufferManageId].ExitPosition);
        }
        else
        {
            copyToUpLength = (bufferLength - SoAd_SoRxBufMan[BufferManageId].ExitPosition);
        }
        if(copyToUpLength > upBufferSize)
        {
            copyToUpLength = upBufferSize;
        }
        if(copyToUpLength > 0u)
        {
            pduInfo.SduDataPtr = &SoAd_PduBuffer[bufferStartPos +
                SoAd_SoRxBufMan[BufferManageId].ExitPosition];
            pduInfo.SduLength = copyToUpLength;
            if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                        SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                        &pduInfo,
                        &upBufferSize))
            {
                if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                    (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId,
                                            copyToUpLength);
                }
                SoAd_SoRxBufMan[BufferManageId].ExitPosition += copyToUpLength;
                if(SoAd_SoRxBufMan[BufferManageId].ExitPosition >= bufferLength)
                {
                    SoAd_SoRxBufMan[BufferManageId].ExitPosition -= bufferLength;
                }
            }
            else
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
            }
        }
    }
    else
    {
        SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
    }
    #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_SoConRxDataMainFunctionHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx bufferId;
#if (STD_ON == SOAD_SUPPORT_HEADERID)
    boolean headerEnabled;
    uint16 soConGroupId;
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
    if((NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr)
            && (NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr))
    {
        bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        /*the EnterPosition is not equal to ExitPosition:the Rx Buffer have store receive data*/
        if(SoAd_SoRxBufMan[bufferId].EnterPosition != SoAd_SoRxBufMan[bufferId].ExitPosition)
        {
#if (STD_ON == SOAD_SUPPORT_HEADERID)
            soConGroupId = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
            headerEnabled = SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_PduHeaderEnable;
            /*header enabled handle*/
            if(TRUE == headerEnabled)
            {
                /*need to analysis the newly PDU header*/
                if(0u == SoAd_SoRxBufMan[bufferId].ExitPduResidueLength)
                {
                    SoAd_HeaderEnableNewPduHandle(SoConId,bufferId);
                }
                /*don't need to analysis the newly PDU header*/
                else
                {
                    SoAd_HeaderEnableOldPduHandle(SoConId,bufferId);
                }
            }
            /*header disabled handle:just TP PDU may use the Rx Buffer*/
            else
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
            {
                SoAd_RxDataMainFunctionHeaderDisableHandle(SoConId,bufferId);
            }
        }
    }
    return;
}
#endif/*0u < SOAD_RXBUFFER_NUM*/

static FUNC(void, SOAD_CODE)
SoAd_MetaDataFillHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    #if(0x100u > SOAD_SO_CON_MAX)
    DataPtr[0u] = SoConId;
    #else/*0x100u > SOAD_SO_CON_MAX*/
    if(LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    {
        DataPtr[0u] = (uint8)SoConId;
        DataPtr[1u] = (uint8)(SoConId >> 8u);
    }
    else
    {
        DataPtr[0u] = (uint8)(SoConId >> 8u);
        DataPtr[1u] = (uint8)SoConId;
    }
    #endif/*0x100u > SOAD_SO_CON_MAX*/
    return;
}

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
#if(0u < SOAD_RXBUFFER_NUM)
static FUNC(boolean, SOAD_CODE)
SoAd_HeaderEnableUdpSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) DataPosition,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    boolean validPduData = TRUE;
    SoAd_uintx bufferId;
    PduInfoType pduInfo;
    uint8 metaData[2];
    SoAd_UpLayerModuleType upModule;
    PduLengthType upBufferSize;
    PduLengthType pduLength;
    pduLength = (PduLengthType)(SOAD_GET_LENGTH(DataPtr, (*DataPosition)));
    if(0u < pduLength)
    {
        bufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        upModule = SOAD_SRDEST_CFG(SocketRouteId).SoAd_RefModule;
        /*check the Rx Buffer is empty or not*/
        if((SoAd_SoRxBufMan[bufferId].EnterPosition == SoAd_SoRxBufMan[bufferId].ExitPosition)
                && (0u == SoAd_SoRxBufMan[bufferId].ExitPduResidueLength))
        {
            pduInfo.SduDataPtr = NULL_PTR;
            pduInfo.SduLength = 0;
            if(TRUE == SOAD_SRDEST_CFG(SocketRouteId).SoAd_MetaDataEnable)
            {
                SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
                pduInfo.MetaDataPtr = &metaData[0u];
            }
            else
            {
                pduInfo.MetaDataPtr = NULL_PTR;
            }
            #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
            if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(
                        SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                        &pduInfo,pduLength,&upBufferSize))
            {
                if(upBufferSize >= pduLength)
                {
                    pduInfo.SduDataPtr = &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE];
                    pduInfo.SduLength = pduLength;
                    if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                                SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                                &pduInfo,&upBufferSize))
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                            SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,E_OK);
                    }
                    else
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                            SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,E_NOT_OK);
                    }
                }
                else if(upBufferSize > 0u)
                {
                    pduInfo.SduDataPtr = &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE];
                    pduInfo.SduLength = upBufferSize;

                    if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                                SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                                &pduInfo,&upBufferSize))
                    {
                        /*copy the remainder data(Length-pduInfo.SduLength) to SoAd Rx Buffer */
                        if(TRUE == SoAd_CopyDataToRxBuffer(SoConId, (pduLength - pduInfo.SduLength),
                                &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE + pduInfo.SduLength]))
                        {
                            SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = pduLength - pduInfo.SduLength;
                            SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = SocketRouteId;
                        }
                    }
                    else
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                            SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,E_NOT_OK);
                    }
                }
                else
                {
                    /*copy the remainder data(Length) to SoAd Rx Buffer */
                    if(TRUE == SoAd_CopyDataToRxBuffer(SoConId, pduLength,
                            &DataPtr[(*DataPosition) + SOAD_HEADER_SIZE]))
                    {
                        SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = pduLength;
                        SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = SocketRouteId;
                    }
                }
            }
            #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
        }
        else
        {
            /*copy the data(Length+8) to Rx Buffer*/
            (void)SoAd_CopyDataToRxBuffer(SoConId, (SOAD_HEADER_SIZE + pduLength), &DataPtr[*DataPosition]);
        }
    }
    else
    {
        validPduData = FALSE;
    }

    *DataPosition += (SOAD_HEADER_SIZE + pduLength);
    return validPduData;
}
#endif/*0u < SOAD_RXBUFFER_NUM*/

static FUNC(boolean, SOAD_CODE)
SoAd_HeaderEnableUdpSoConRxHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(uint16, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    boolean validPduData = TRUE;
    SoAd_uintx soConGroupId;
    boolean findValidSocketRoute;
    uint32 header;
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metaData[2];
    SoAd_uintx cnt;
    SoAd_uintx socketRouteId;
    PduLengthType dataPosition = 0u;
    PduLengthType pduLength;
    soConGroupId = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    if(TRUE == SoAd_UdpStrictHeaderLenCheck(soConGroupId, DataPtr, RxMessageLength))
    {
        while(dataPosition < RxMessageLength)
        {
            findValidSocketRoute = FALSE;
            header = SOAD_GET_HEADER(DataPtr, dataPosition);
            pduLength = (PduLengthType)(SOAD_GET_LENGTH(DataPtr, dataPosition));
            if((dataPosition + SOAD_HEADER_SIZE + pduLength) <= RxMessageLength)
            {
                for(cnt = 0u; (cnt < SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRouteNum)
                        && (FALSE == findValidSocketRoute); cnt++)
                {
                    socketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[cnt];
                    if(header == *SOAD_SOCKETROUTE_CFG(socketRouteId).SoAd_RxPduHeaderId)
                    {
                        findValidSocketRoute = TRUE;
                    }
                }
                /*findValidSocketRoute and socket dest routing group is enabled*/
                if((TRUE == findValidSocketRoute)
                        && (TRUE == SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable))
                {
                    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
                    if(SOAD_UPPER_LAYER_IF == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
                    {
                        pduInfo.SduDataPtr = &DataPtr[dataPosition + SOAD_HEADER_SIZE];
                        pduInfo.SduLength = pduLength;
                        if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
                        {
                            SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
                            pduInfo.MetaDataPtr = &metaData[0u];
                        }
                        else
                        {
                            pduInfo.MetaDataPtr = NULL_PTR;
                        }
                        #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                        SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
                            SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId, &pduInfo);
                        #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
                        dataPosition = dataPosition + SOAD_HEADER_SIZE + pduLength;
                    }
                    else
                    {
#if(0u < SOAD_RXBUFFER_NUM)
                        validPduData = SoAd_HeaderEnableUdpSoConRxTpPduHandle(
                                SoConId, socketRouteId, &dataPosition, DataPtr);
#endif/*0u < SOAD_RXBUFFER_NUM*/
                    }
                }
                else
                {
                    dataPosition = dataPosition + SOAD_HEADER_SIZE + pduLength;
                    (void)Det_ReportRuntimeError(SOAD_MODULE_ID,SOAD_INSTANCE,
                                                 SOAD_SID_RXINDICATION,SOAD_E_INV_PDUHEADER_ID);
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        validPduData = FALSE;
    }

    return validPduData;
}
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
#endif/* STD_ON == SOAD_SUPPORT_UDP */

#if(0u < SOAD_RXBUFFER_NUM)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableTcpSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    PduInfoType pduInfo;
    uint8 metaData[2];
    PduLengthType copyToUpLength,bufferLength;
    PduLengthType upBufferSize = 0u;
    SoAd_uintx bufferId;
    SoAd_UpLayerModuleType upModule;
    PduLengthType pduLength;
    SoAd_RxFullTpPduHandleType parameter;
    pduLength = (PduLengthType)SOAD_GET_LENGTH(DataPtr, 0u);
    upModule = SOAD_SRDEST_CFG(SocketRouteId).SoAd_RefModule;
    bufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
    bufferLength = SOAD_SOCKETBUFFER_CFG(bufferId).bufLen;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = 0u;
    if(TRUE == SOAD_SRDEST_CFG(SocketRouteId).SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
    #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
    if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(
                SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                &pduInfo,pduLength,&upBufferSize))
    {
        /*if not all data can be processed*/
        if((bufferLength + upBufferSize) < (RxMessageLength - SOAD_HEADER_SIZE))
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                E_NOT_OK);
            SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
        }
        else
        {
            /*the message's data belong to more than one PDU*/
            if(pduLength <= (RxMessageLength - SOAD_HEADER_SIZE))
            {
                parameter.DataPtr = DataPtr;
                parameter.PduLength = pduLength;
                parameter.RxDataLength = RxMessageLength;
                parameter.SoConId = SoConId;
                parameter.SocketRouteId = SocketRouteId;
                parameter.UpBufferSize = upBufferSize;
                SoAd_RxFullTpPduHandle(parameter);
            }
            /*the message's all data is part of the PDU*/
            else
            {
                copyToUpLength = upBufferSize;
                if(upBufferSize > (RxMessageLength - SOAD_HEADER_SIZE))
                {
                    copyToUpLength = (RxMessageLength - SOAD_HEADER_SIZE);
                }
                if(0u < copyToUpLength)
                {
                    pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
                    pduInfo.SduLength = copyToUpLength;
                    if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                                SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                                &pduInfo,
                                &upBufferSize))
                    {
                        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId,copyToUpLength);
                        /*copy the remainder data(RxMessageLength-8-copyToUpLength) to SoAd Rx Buffer*/
                        if(TRUE == SoAd_CopyDataToRxBuffer(SoConId,
                                                           (RxMessageLength - SOAD_HEADER_SIZE - copyToUpLength),
                                                           &DataPtr[SOAD_HEADER_SIZE + copyToUpLength]))
                        {
                            SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = (pduLength - copyToUpLength);
                            SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = SocketRouteId;
                        }
                    }
                    else
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                            SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,E_NOT_OK);
                        SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
                    }
                }
                else
                {
                    /*copy the remainder data(RxMessageLength-8) to SoAd Rx Buffer*/
                    if(TRUE == SoAd_CopyDataToRxBuffer(SoConId, (RxMessageLength - SOAD_HEADER_SIZE), &DataPtr[SOAD_HEADER_SIZE]))
                    {
                        SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = pduLength;
                        SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = SocketRouteId;
                    }
                }
            }
        }
    }
    else
    {
        SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
    }
    #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_RxFullTpPduHandle(
        VAR(SoAd_RxFullTpPduHandleType, AUTOMATIC) Parameter)
{
    PduInfoType pduInfo;
    uint8 metaData[2];
    SoAd_UpLayerModuleType upModule;
    SoAd_uintx bufferId;
    PduLengthType upBufferSize;
    uint8* DataPtr;
    bufferId = *SOAD_SOCON_CFG(Parameter.SoConId).SoAd_RxBufCfgIndexPtr;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    upModule = SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_RefModule;
    DataPtr = Parameter.DataPtr;
    upBufferSize = Parameter.UpBufferSize;
    if(TRUE == SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(Parameter.SoConId,&metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
    /*the UpBuffer is large enough to copy the total pdu data*/
    if(upBufferSize >= Parameter.PduLength)
    {
        pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
        pduInfo.SduLength = Parameter.PduLength;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                    SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_UpPduId,
                    &pduInfo,
                    &upBufferSize))
        {
            /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
            (void)TcpIp_TcpReceived(SoAd_SoConManager[Parameter.SoConId].TcpIpSocketId, Parameter.PduLength);
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_UpPduId,
                E_OK);
            /*copy the remainder data(include the next pdu's header and data) to SoAd Rx Buffer*/
            (void)SoAd_CopyDataToRxBuffer(Parameter.SoConId, (Parameter.RxDataLength - SOAD_HEADER_SIZE - Parameter.PduLength),
                                          &DataPtr[SOAD_HEADER_SIZE + Parameter.PduLength]);
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                    SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_UpPduId, E_NOT_OK);
            SoAd_SoConManager[Parameter.SoConId].SocketNeedClose = TRUE;
        }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    }
    else if(upBufferSize > 0u)
    {
        pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
        pduInfo.SduLength = upBufferSize;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                    SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_UpPduId,
                    &pduInfo,
                    &upBufferSize))
        {
            /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
            (void)TcpIp_TcpReceived(SoAd_SoConManager[Parameter.SoConId].TcpIpSocketId, upBufferSize);
            /*copy the remainder data to SoAd Rx Buffer*/
            if(TRUE == SoAd_CopyDataToRxBuffer(Parameter.SoConId,
                                               (Parameter.RxDataLength - SOAD_HEADER_SIZE - pduInfo.SduLength),
                                               &DataPtr[SOAD_HEADER_SIZE + pduInfo.SduLength]))
            {
                SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = (Parameter.PduLength - pduInfo.SduLength);
                SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = Parameter.SocketRouteId;
            }
        }
        else
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(
                    SOAD_SRDEST_CFG(Parameter.SocketRouteId).SoAd_UpPduId, E_NOT_OK);
            SoAd_SoConManager[Parameter.SoConId].SocketNeedClose = TRUE;
        }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    }
    else
    {
        /*copy the remainder data to SoAd Rx Buffer*/
        if(TRUE == SoAd_CopyDataToRxBuffer(Parameter.SoConId, (Parameter.RxDataLength - SOAD_HEADER_SIZE), &DataPtr[SOAD_HEADER_SIZE]))
        {
            SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = Parameter.PduLength;
            SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = Parameter.SocketRouteId;
        }
    }
    return;
}
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE)
SoAd_HeaderDisableSoConRxTpPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    SoAd_uintx bufferId;
    PduInfoType pduInfo;
    SoAd_UpLayerModuleType upModule;
    PduLengthType upBufferSize;
    uint8 metaData[2u];
    bufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    upModule = SOAD_SRDEST_CFG(SocketRouteId).SoAd_RefModule;
    /*SWS_SoAd_00568:
      if the SoAd receive buffer does not contain any TP data for this socket connection*/
    if(SoAd_SoRxBufMan[bufferId].ExitPosition == SoAd_SoRxBufMan[bufferId].EnterPosition)
    {
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 0u;
        if(TRUE == SOAD_SRDEST_CFG(SocketRouteId).SoAd_MetaDataEnable)
        {
            SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
            pduInfo.MetaDataPtr = &metaData[0u];
        }
        else
        {
            pduInfo.MetaDataPtr = NULL_PTR;
        }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                    SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                    &pduInfo,
                    &upBufferSize))
        {
            if(upBufferSize >= RxMessageLength)
            {
                pduInfo.SduLength = RxMessageLength;
            }
            else
            {
                pduInfo.SduLength = upBufferSize;
            }
            pduInfo.SduDataPtr = DataPtr;
            if(BUFREQ_OK == SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyRxData_FuncPtr(
                        SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
                        &pduInfo,
                        &upBufferSize))
            {
                if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                    (void)TcpIp_TcpReceived(
                        SoAd_SoConManager[SoConId].TcpIpSocketId,
                        pduInfo.SduLength);
                }
                /*copy data to SoAd Rx Buffer*/
                if(pduInfo.SduLength < RxMessageLength)
                {
                    (void)SoAd_CopyDataToRxBuffer(
                            SoConId,
                            (RxMessageLength - pduInfo.SduLength),
                            &DataPtr[pduInfo.SduLength]);
                }
            }
            else
            {
                SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
            }
        }
        else
        {
            SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
        }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    }
    /*SWS_SoAd_00568:
      if the SoAd receive buffer already contains TP data for this socket connection
      and is able to store all (newley) received data*/
    else
    {
        (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, &DataPtr[0u]);
    }
    return;
}

#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(void, SOAD_CODE)
SoAd_HeaderEnableTcpSoConRxHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    uint32 header;
    PduLengthType pduLength;
#if (0u < SOAD_MAX_IF_PDU_SIZE)
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metaData[2];
#endif
    SoAd_uintx cnt,socketRouteId;
    boolean findValidSocketRoute = FALSE;
    SoAd_uintx bufferId;
    bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
    /*used for SoAd_RxBuffer Manage*/
    bufferId = bufferId - SOAD_TXBUFFER_NUM;
    /*the newly pdu:copy data to up layer,copy residue data to SoAd Rx Buffer*/
    if((SoAd_SoRxBufMan[bufferId].EnterPosition == SoAd_SoRxBufMan[bufferId].ExitPosition)
            && (0u == SoAd_SoRxBufMan[bufferId].ExitPduResidueLength)
            && (RxMessageLength >= SOAD_HEADER_SIZE))
    {
        header = SOAD_GET_HEADER(DataPtr, 0u);
        pduLength = (PduLengthType)SOAD_GET_LENGTH(DataPtr, 0u);
        for(cnt = 0u; (cnt < SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRouteNum)
                && (FALSE == findValidSocketRoute); cnt++)
        {
            socketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[cnt];
            if(header == *SOAD_SOCKETROUTE_CFG(socketRouteId).SoAd_RxPduHeaderId)
            {
                findValidSocketRoute = TRUE;
            }
        }
        /*findValidSocketRoute and socket dest routing group is enabled.*/
        if((TRUE == findValidSocketRoute)
                && (TRUE == SoAd_SocketRouteDestManager[socketRouteId].SocketRouteDestEnable))
        {
            (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, SOAD_HEADER_SIZE);
            if(SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType)
            {
                /*Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or SoAdRxUpperLayerType is TP)*/
                if(0u < pduLength)
                {
                    SoAd_HeaderEnableTcpSoConRxTpPduHandle(SoConId, socketRouteId,
                                                           RxMessageLength, DataPtr);
                }
            }
#if (0u < SOAD_MAX_IF_PDU_SIZE)
            /*IF PDU Rx handle*/
            else
            {
                /*the message's data belong to more than one PDU*/
                if(pduLength <= (RxMessageLength - SOAD_HEADER_SIZE))
                {
                    pduInfo.SduDataPtr = &DataPtr[SOAD_HEADER_SIZE];
                    pduInfo.SduLength = pduLength;
                    if(TRUE == SOAD_SRDEST_CFG(socketRouteId).SoAd_MetaDataEnable)
                    {
                        SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
                        pduInfo.MetaDataPtr = &metaData[0u];
                    }
                    else
                    {
                        pduInfo.MetaDataPtr = NULL_PTR;
                    }
                    upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

                    SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
                        SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                        &pduInfo);
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

                    /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
                    (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, pduLength);
                    /*copy the remainder data(Length-8-pduLength) to SoAd Rx Buffer*/
                    (void)SoAd_CopyDataToRxBuffer(SoConId, (RxMessageLength - SOAD_HEADER_SIZE - pduLength),
                                                  &DataPtr[SOAD_HEADER_SIZE + pduLength]);
                }
                /*the message is part data of one PDU*/
                else
                {
                    /*copy the remainder data(Length-8) to SoAd Rx Buffer*/
                    if(TRUE == SoAd_CopyDataToRxBuffer(SoConId, (RxMessageLength - SOAD_HEADER_SIZE),
                                                       &DataPtr[SOAD_HEADER_SIZE]))
                    {
                        SoAd_SoRxBufMan[bufferId].ExitPduResidueLength = pduLength;
                        SoAd_SoRxBufMan[bufferId].SoAd_SocketRouteId = socketRouteId;
                    }
                }
            }
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/
        }
        else
        {
            if((RxMessageLength - SOAD_HEADER_SIZE) >= pduLength)
            {
                (void)Det_ReportRuntimeError(SOAD_MODULE_ID,SOAD_INSTANCE,
                                             SOAD_SID_RXINDICATION,SOAD_E_INV_PDUHEADER_ID);
                /*discard the all pdu data*/
                (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId,
                                        (uint32)pduLength + (uint32)SOAD_HEADER_SIZE);
                (void)SoAd_CopyDataToRxBuffer(SoConId,
                                              (RxMessageLength - SOAD_HEADER_SIZE - pduLength),
                                              &DataPtr[pduLength + SOAD_HEADER_SIZE]);
            }
            /*need copy the part pdu data to buffer,then discard the all data of the pdu.*/
            else
            {
                (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, DataPtr);
            }
        }
    }
    /*copy the total data to SoAd Rx Buffer*/
    else
    {
        (void)SoAd_CopyDataToRxBuffer(SoConId, RxMessageLength, DataPtr);
    }
    return;
}
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

/*copy data to SoAd Rx Buffer*/
static FUNC(boolean, SOAD_CODE) SoAd_CopyDataToRxBuffer(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    PduLengthType srcPos;
    PduLengthType destPos;
    boolean copyDataOk = TRUE;
    SoAd_uintx bufferId = SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr[0u];
    PduLengthType bufferPosition = SOAD_SOCKETBUFFER_CFG(bufferId).bufPos;
    PduLengthType bufferLength = SOAD_SOCKETBUFFER_CFG(bufferId).bufLen;
    if(0u < RxMessageLength)
    {
        /*used for SoAd_RxBuffer Manage*/
        bufferId = bufferId - SOAD_TXBUFFER_NUM;
        srcPos = SoAd_SoRxBufMan[bufferId].EnterPosition;
        destPos = srcPos + RxMessageLength;
        if(SoAd_SoRxBufMan[bufferId].ExitPosition <= SoAd_SoRxBufMan[bufferId].EnterPosition)
        {
            if(destPos >= bufferLength)
            {
                if((destPos - bufferLength) < SoAd_SoRxBufMan[bufferId].ExitPosition)
                {
                    SoAd_LocalMemCpy(&SoAd_PduBuffer[bufferPosition + srcPos], DataPtr,
                                    (bufferLength - srcPos));
                    SoAd_LocalMemCpy(&SoAd_PduBuffer[bufferPosition],
                                (DataPtr + (bufferLength - srcPos)),
                                (destPos - bufferLength));
                    SoAd_SoRxBufMan[bufferId].EnterPosition = destPos - bufferLength;
                }
                /*SWS_SoAd_00693:larger than the remaining available buffer size */
                else
                {
                    copyDataOk = FALSE;
                }
            }
            else
            {
                SoAd_LocalMemCpy(&SoAd_PduBuffer[bufferPosition + srcPos],
                            DataPtr, RxMessageLength);
                SoAd_SoRxBufMan[bufferId].EnterPosition = destPos;
            }
        }
        else
        {
            if(destPos < SoAd_SoRxBufMan[bufferId].ExitPosition)
            {
                SoAd_LocalMemCpy(&SoAd_PduBuffer[bufferPosition + srcPos],
                            DataPtr, RxMessageLength);
                SoAd_SoRxBufMan[bufferId].EnterPosition = destPos;
            }
            else
            {
                copyDataOk = FALSE;
            }
        }
        if(FALSE == copyDataOk)
        {
            SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
            (void)Det_ReportRuntimeError(SOAD_MODULE_ID,SOAD_INSTANCE,SOAD_SID_RXINDICATION,SOAD_E_NOBUFS);
        }
    }
    return copyDataOk;
}
#endif/*0u < SOAD_RXBUFFER_NUM*/

static FUNC(void, SOAD_CODE)
SoAd_HeaderDisableSoConRxIfPduHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SocketRouteId,
    VAR(PduLengthType, AUTOMATIC) RxMessageLength,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) DataPtr)
{
    SoAd_UpLayerModuleType upModule;
    PduInfoType pduInfo;
    uint8 metaData[2u];
    upModule = SOAD_SRDEST_CFG(SocketRouteId).SoAd_RefModule;
    pduInfo.SduDataPtr = DataPtr;
    pduInfo.SduLength = RxMessageLength;
    if(TRUE == SOAD_SRDEST_CFG(SocketRouteId).SoAd_MetaDataEnable)
    {
        SoAd_MetaDataFillHandle(SoConId,&metaData[0u]);
        pduInfo.MetaDataPtr = &metaData[0u];
    }
    else
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

    if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr)
    {
        SOAD_UPPERLAY_PCCFG(upModule).If_RxIndication_FuncPtr(
            SOAD_SRDEST_CFG(SocketRouteId).SoAd_UpPduId,
            &pduInfo);
    }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        /*SWS_SoAd_00564:SoAd shall confirm the reception of all data*/
        (void)TcpIp_TcpReceived(SoAd_SoConManager[SoConId].TcpIpSocketId, RxMessageLength);
    }
    return;
}

#if(0u < SOAD_SOCKET_ROUTE_DEST_MAX)
static FUNC(boolean, SOAD_CODE)
SoAd_FindTheValidSoCon(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
#if (STD_ON == SOAD_SUPPORT_UDP)
    VAR(SoAd_FindTheValidSoConType, AUTOMATIC) Parameter,
#endif
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConId)
{
    SoAd_uintx cnt;
    boolean socketIdValid = FALSE;
    boolean soConValid = FALSE;
    SoAd_SoConIdType soConId;
#if (STD_ON == SOAD_SUPPORT_TCP)
    uint16 soConGroupId;
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
    *(Parameter.RemoteAddrChanged) = FALSE;
#endif
#if (STD_ON == SOAD_SUPPORT_UDP)
    /*find a udp socket connection*/
    for(cnt = 0u; (cnt < SOAD_SO_CON_GROUP_NUM)
            && (FALSE == socketIdValid); cnt++)
    {
        if((TRUE == SoAd_SoConGroupManager[cnt].TcpIpSocketValid)
                && (SocketId == SoAd_SoConGroupManager[cnt].TcpIpSocketId)
                && (NULL_PTR != SOAD_UDPTYPE_CFG(cnt)))
        {
            if(TRUE == SOAD_SOCONGROUP_CFG(cnt).SoAd_SocketMsgAcceptanceFilterEnabled)
            {
                /*SWS_SoAd_00657:Best Match Algorithm*/
                if(TRUE == SoAd_GetBestMatchAlgorithmSoCon(cnt, RemoteAddrPtr, &soConId, SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT))
                {
                    *(Parameter.RemoteAddr) = SoAd_SoConManager[soConId].RemoteAddr;
                    /*SWS_SoAd_00592:(a) overwrite the remote address parts
                    specified with wildcards with the related source address
                    parts of the received message and (b) change the state of
                      the socket connection to SOAD_SOCON_ONLINE */
                    if((SOAD_SOCON_ONLINE != SoAd_SoConManager[soConId].SoAd_SoConMode)
                            && (FALSE == SOAD_UDPTYPE_CFG(cnt)->SoAd_SocketUdpListenOnly))
                    {
                        *(Parameter.RemoteAddrChanged) = TRUE;
                        SoAd_SoConManager[soConId].RemoteAddr = *RemoteAddrPtr;
                        SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_ONLINE;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, cnt);
                    }
                    /*Start the UdpAliveSupervisionTime*/
                    if((*(Parameter.RemoteAddrChanged)) == TRUE)
                    {
                        if((NULL_PTR != SOAD_UDPTYPE_CFG(cnt)->SoAd_SocketUdpAliveSupervisionTimeout)
                                && (TCPIP_IS_IPADDR_ANY(*SOAD_SOCON_CFG(soConId).SoAd_SocketRemoteAddress)
                                        || (TCPIP_PORT_ANY == SOAD_SOCON_CFG(soConId).SoAd_SocketRemoteAddress->port)))
                        {
                            SoAd_SoConManager[soConId].UdpAliveSupervisionTime =
                                SOAD_UDPTYPE_CFG(cnt)->SoAd_SocketUdpAliveSupervisionTimeout[0u];
                        }
                    }
                    else
                    {
                        if(0u < SoAd_SoConManager[soConId].UdpAliveSupervisionTime)
                        {
                            SoAd_SoConManager[soConId].UdpAliveSupervisionTime =
                                SOAD_UDPTYPE_CFG(cnt)->SoAd_SocketUdpAliveSupervisionTimeout[0u];
                        }
                    }
                    soConValid = TRUE;
                }
            }
            else
            {
                soConId = SOAD_SOCONGROUP_CFG(cnt).SoAd_SoConId[0u];
                if(SoAd_SoConManager[soConId].SoAd_SoConMode == SOAD_SOCON_ONLINE)
                {
                    soConValid = TRUE;
                }
            }
            socketIdValid = TRUE;
        }
    }

#endif/* STD_ON == SOAD_SUPPORT_UDP */
#if (STD_ON == SOAD_SUPPORT_TCP)
    /*find a tcp socket connection*/
    for(cnt = 0u; (cnt < SOAD_SOCON_NUM) && (FALSE == socketIdValid); cnt++)
    {
        soConGroupId = SOAD_SOCON_CFG(cnt).SoAd_SocketConnectionGroupRef;
        if(((FALSE == SoAd_SoConGroupManager[soConGroupId].TcpIpSocketValid)
                || (SocketId != SoAd_SoConGroupManager[soConGroupId].TcpIpSocketId))
                && (TRUE == SoAd_SoConManager[cnt].TcpIpSocketValid)
                && (SocketId == SoAd_SoConManager[cnt].TcpIpSocketId))
        {
            if(TRUE == SOAD_SOCONGROUP_CFG(soConGroupId).SoAd_SocketMsgAcceptanceFilterEnabled)
            {
                if((SoAd_SoConManager[cnt].RemoteAddr.domain == RemoteAddrPtr->domain)
                        && (TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[cnt].RemoteAddr)
                            || (TCPIP_EQ_IPADDR(SoAd_SoConManager[cnt].RemoteAddr,
                                   *RemoteAddrPtr)))
                        && ((SoAd_SoConManager[cnt].RemoteAddr.port == TCPIP_PORT_ANY)
                            || (SoAd_SoConManager[cnt].RemoteAddr.port ==
                                   RemoteAddrPtr->port)))
                {
                    soConValid = TRUE;
                    soConId = (SoAd_SoConIdType)cnt;
                }
            }
            else
            {
                soConValid = TRUE;
                soConId = (SoAd_SoConIdType)cnt;
            }
            socketIdValid = TRUE;
        }
    }

#endif/* STD_ON == SOAD_SUPPORT_TCP */

    if(TRUE == soConValid)
    {
        *SoConId = soConId;
    }
    return socketIdValid;
}
#endif/*0u < SOAD_SOCKET_ROUTE_DEST_MAX*/

#if (STD_ON == SOAD_SUPPORT_UDP)
#if (STD_ON == SOAD_SUPPORT_HEADERID)
static FUNC(boolean, SOAD_CODE)
SoAd_UdpStrictHeaderLenCheck(
    VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) Length)
{
    uint32 dataPosition = 0u;
    uint32 pduLength;
    boolean udpStrictHeaderLenCheck = FALSE;
    if(Length >= SOAD_HEADER_SIZE)
    {
        if(TRUE == SOAD_UDPTYPE_CFG(SoConGroupId)->SoAd_SocketUdpStrictHeaderLenCheckEnabled)
        {
            while(dataPosition < Length)
            {
                pduLength = SOAD_GET_LENGTH(BufPtr, dataPosition);
                dataPosition = dataPosition + SOAD_HEADER_SIZE + pduLength;
                if(dataPosition == Length)
                {
                    udpStrictHeaderLenCheck = TRUE;
                }
            }
        }
        else
        {
            udpStrictHeaderLenCheck = TRUE;
        }
    }
    return udpStrictHeaderLenCheck;
}
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/

static FUNC(void, SOAD_CODE)
SoAd_UdpAliveSupervisionTimeHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx soConGroup;
    if(0u < SoAd_SoConManager[SoConId].UdpAliveSupervisionTime)
    {
        (SoAd_SoConManager[SoConId].UdpAliveSupervisionTime) -= 1u;
        if(0u == SoAd_SoConManager[SoConId].UdpAliveSupervisionTime)
        {
            /*SWS_SoAd_00695:alive supervision timer runs out*/
            SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
            soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_SocketRemoteAddress)
            {
                SoAd_SoConManager[SoConId].RemoteAddr=*SOAD_SOCONREMOTEADDR_CFG(SoConId);
            }

        }
    }
    return;
}
#endif/* STD_ON == SOAD_SUPPORT_UDP */

static FUNC(boolean, SOAD_CODE)
SoAd_GetBestMatchAlgorithmSoCon(
    VAR(SoAd_uintx, AUTOMATIC) SoConGroupId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_CONST) SoConId,
    VAR(SoAd_BestMatchStateType, AUTOMATIC) CheckSoConState)
{
    boolean result;
    SoAd_SoConIdType cnt,soConIndex;
    uint8 priority = 0u;
    /*Best Match Algorithm*/
    for(cnt = 0u;
            (cnt < SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConNum)
            && (4u != priority); cnt++)
    {
        soConIndex = SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConId[cnt];
        if((SOAD_SOCON_MATCH_REQUEST_NONE == CheckSoConState)
                || ((SOAD_SOCON_MATCH_REQUEST_RECONNECT == CheckSoConState) 
                && (SoAd_SoConManager[soConIndex].SoAd_SoConMode == SOAD_SOCON_RECONNECT)
                && (SoAd_SoConManager[soConIndex].TcpIpSocketValid == FALSE))
                || ((SOAD_SOCON_MATCH_REQUEST_ONLINE_RECONNECT == CheckSoConState) 
                && (SoAd_SoConManager[soConIndex].SoAd_SoConMode != SOAD_SOCON_OFFLINE)))
        {
            if(TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[soConIndex].RemoteAddr)
                    && (TCPIP_PORT_ANY == SoAd_SoConManager[soConIndex].RemoteAddr.port))
            {
                if(priority < 1u)
                {
                    *SoConId = soConIndex;
                    priority = 1u;
                }
            }
            else if(TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[soConIndex].RemoteAddr)
                    && (RemoteAddrPtr->port == SoAd_SoConManager[soConIndex].RemoteAddr.port))
            {
                if(priority < 2u)
                {
                    *SoConId = soConIndex;
                    priority = 2u;
                }
            }
            else if(TCPIP_EQ_IPADDR(*RemoteAddrPtr,SoAd_SoConManager[soConIndex].RemoteAddr)
                    && (TCPIP_PORT_ANY == SoAd_SoConManager[soConIndex].RemoteAddr.port))
            {
                if(priority < 3u)
                {
                    *SoConId = soConIndex;
                    priority = 3u;
                }
            }
            else if(TCPIP_EQ_IPADDR(*RemoteAddrPtr,SoAd_SoConManager[soConIndex].RemoteAddr)
                    && (RemoteAddrPtr->port == SoAd_SoConManager[soConIndex].RemoteAddr.port))
            {
                if(priority < 4u)
                {
                    *SoConId = soConIndex;
                    priority = 4u;
                }
            }
            else
            {
                /*do nothing*/
            }
        }
    }

    if((0u == priority) || (4u < priority))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    return result;
}



static FUNC(void, SOAD_CODE)
SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
    VAR(boolean, AUTOMATIC) SocketProtocolIsTcp,
#endif
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    /*In case of a TCP/UDP socket*/
    if (NULL_PTR != SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketFramePriority)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FRAMEPRIO,
                  SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketFramePriority);
    }
    if (NULL_PTR != SOAD_SOCONGROUP_CFG(SoConGroup).SoAdSocketDifferentiatedServicesField)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_DSCP,
                  SOAD_SOCONGROUP_CFG(SoConGroup).SoAdSocketDifferentiatedServicesField);
    }
    if (NULL_PTR != SOAD_SOCONGROUP_CFG(SoConGroup).SoAdSocketFlowLabel)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FLOWLABEL,
                  SOAD_SOCONGROUP_CFG(SoConGroup).SoAdSocketFlowLabel);
    }
    if (NULL_PTR != SOAD_SOCONGROUP_CFG(SoConGroup).SoAdSocketPathMTUEnable)
    {
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_PATHMTU_ENABLE,
                  SOAD_SOCONGROUP_CFG(SoConGroup).SoAdSocketPathMTUEnable);
    }
#if (STD_ON == SOAD_SUPPORT_TCP)
    /*In case of a TCP socket*/
    if (TRUE == SocketProtocolIsTcp)
    {
        if (NULL_PTR != SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketTpRxBufferMin)
        {
            (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_RXWND_MAX,
                      SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketTpRxBufferMin);
        }
        if (NULL_PTR != SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpNoDelay)
        {
            uint8 delayVlu = 0x01u;
            if (TRUE == SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpNoDelay[0])
            {
                delayVlu = 0x0u;
            }
            (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_NAGLE, &delayVlu);
        }
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE,
                  &SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAlive);
        if (NULL_PTR != SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAliveTime)
        {
            (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE_TIME,
                      SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAliveTime);
        }
        if (NULL_PTR != SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAliveProbesMax)
        {
            (void)TcpIp_ChangeParameter(SocketId,TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX,
                      SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAliveProbesMax);
        }
        if (NULL_PTR != SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAliveInterval)
        {
            (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL,
                      SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpKeepAliveInterval);
        }
        if (NULL_PTR != SOAD_TCPTYPE_CFG(SoConGroup)->SoAdSocketTCPOptionFilterId)
        {
            (void)TcpIp_ChangeParameter(SocketId,TCPIP_PARAMID_TCP_OPTIONFILTER,
                      SOAD_TCPTYPE_CFG(SoConGroup)->SoAdSocketTCPOptionFilterId);
        }
        if (NULL_PTR != SOAD_TCPTYPE_CFG(SoConGroup)->SoAdSocketTcpTlsConnectionId)
        {
            (void)TcpIp_ChangeParameter(SocketId,TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT,
                      SOAD_TCPTYPE_CFG(SoConGroup)->SoAdSocketTcpTlsConnectionId);
        }
    }
#endif /*STD_ON == SOAD_SUPPORT_TCP*/
    return;
}
#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
SoAd_OpenUdpSoConHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    uint16 localPort;
    TcpIp_SocketIdType socketId;
    if(E_OK == TcpIp_SoAdGetSocket((SoAd_SoConManager[SoConId].RemoteAddr.domain),
                                TCPIP_IPPROTO_UDP,&socketId))
    {
        /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
        SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
                FALSE,
#endif
                socketId, SoConGroup);
        /*init the SoAd_SoConManager parameter*/
        SoAd_SoConManager[SoConId].TcpIpSocketValid = TRUE;
        SoAd_SoConManager[SoConId].TcpIpSocketId = socketId;
        SoAd_SoConGroupManager[SoConGroup].TcpIpSocketValid = TRUE;
        SoAd_SoConGroupManager[SoConGroup].TcpIpSocketId = socketId;
        SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum = 1u;
        /*Bind the socket to the local address and port*/
        localPort = SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketLocalPort;
        if(E_OK == TcpIp_Bind(socketId,
                              SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId,
                              &localPort))
        {
            /*SWS_SoAd_00591*/
            if((TRUE == SOAD_UDPTYPE_CFG(SoConGroup)->SoAd_SocketUdpListenOnly)
                    || ((!TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[SoConId].RemoteAddr))
                        && (TCPIP_PORT_ANY != SoAd_SoConManager[SoConId].RemoteAddr.port)))
            {
                if(SOAD_SOCON_ONLINE != SoAd_SoConManager[SoConId].SoAd_SoConMode)
                {
                    SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_ONLINE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
            }
            /*SWS_SoAd_00686*/
            else
            {
                if(SOAD_SOCON_RECONNECT != SoAd_SoConManager[SoConId].SoAd_SoConMode)
                {
                    SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
            }
        }
        else
        {
            /*don't care the return value*/
            (void)TcpIp_Close(socketId, TRUE);
        }
    }
    return;
}
#endif/* STD_ON == SOAD_SUPPORT_UDP */



static FUNC(void, SOAD_CODE)
SoAd_SoConModeChgNotificationAndTpStartOfReception(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    uint8 cnt0;
    SoAd_uintx cnt1,socketRouteId;
    SoAd_UpLayerModuleType upModule;
    PduLengthType bufferSize = 0u;
    /*if SoAdSocketSoConModeChgNotification is set to TRUE,call <Up>_SoConModeChg()*/
    if(TRUE == SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketSoConModeChgNotification)
    {
        for(cnt0 = 0u;cnt0 < SOAD_SOCON_CFG(SoConId).SoAd_RefUpNum; cnt0++)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = SOAD_SOCON_CFG(SoConId).SoAd_RefUpModulePtr[cnt0];
            if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).SoConModeChg_FuncPtr)
            {
                SOAD_UPPERLAY_PCCFG(upModule).SoConModeChg_FuncPtr(SoConId,
                    SoAd_SoConManager[SoConId].SoAd_SoConMode);
            }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

        }
    }
    if(SOAD_SOCON_ONLINE == SoAd_SoConManager[SoConId].SoAd_SoConMode)
    {
        for(cnt1 = 0u;cnt1 < SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRouteNum;cnt1++)
        {
            socketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[cnt1];
            /*SWS_SoAd_00595:For socket connection with PDU Header mode disabled
              (SoAdPduHeaderEnable = FALSE) and an upper layer with TP-API*/
            if(
#if(STD_ON == SOAD_SUPPORT_HEADERID)
                (NULL_PTR == SOAD_SOCKETROUTE_CFG(socketRouteId).SoAd_RxPduHeaderId)
                &&
#endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
                (SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(socketRouteId).SoAd_RxUpperLayerType))
            {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

                upModule = SOAD_SRDEST_CFG(socketRouteId).SoAd_RefModule;
                if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr)
                {
                    if(BUFREQ_OK != SOAD_UPPERLAY_PCCFG(upModule).Tp_StartOfReception_FuncPtr(
                                SOAD_SRDEST_CFG(socketRouteId).SoAd_UpPduId,
                                NULL_PTR,
                                0,
                                &bufferSize))
                    {
                        SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
                    }
                }
                else
                {
                    SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
                }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

            }
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_SoConLocalIpAddrAssignmentChg(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    /*if SoAd_SocketIpAddrAssignmentChgNotification is set to TRUE,call <Up>_LocalIpAddrAssignmentChg*/
    if(TRUE == SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketIpAddrAssignmentChgNotification)
    {
        uint8 cnt;
        SoAd_UpLayerModuleType upModule;
        for(cnt = 0u;cnt < SOAD_SOCON_CFG(SoConId).SoAd_RefUpNum; cnt++)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = SOAD_SOCON_CFG(SoConId).SoAd_RefUpModulePtr[cnt];
            if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).LocalIpAddrAssignmentChg_FuncPtr)
            {
                SOAD_UPPERLAY_PCCFG(upModule).LocalIpAddrAssignmentChg_FuncPtr(SoConId,
                    SoAd_SoConGroupManager[SoConGroup].LocalIpAddrState);
            }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_OpenTcpServerSoConHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    TcpIp_SocketIdType socketId;
    uint16 localPort;
    if(FALSE == SoAd_SoConGroupManager[SoConGroup].TcpIpSocketValid)
    {
        if(E_OK == TcpIp_SoAdGetSocket((SoAd_SoConManager[SoConId].RemoteAddr.domain),
                                       TCPIP_IPPROTO_TCP, &socketId))
        {
            /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
            SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
                    TRUE,
#endif
                    socketId, SoConGroup);
            SoAd_SoConGroupManager[SoConGroup].TcpIpSocketValid = TRUE;
            SoAd_SoConGroupManager[SoConGroup].TcpIpSocketId = socketId;
            SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum = 0u;
            /*Bind the socket to the local address and port*/
            localPort = SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketLocalPort;
            if(E_OK == TcpIp_Bind(socketId,
                                  SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId,
                                  &localPort))
            {
                if(E_OK == TcpIp_TcpListen(socketId,
                        SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SoConNum))
                {
                    /*SWS_SoAd_00686*/
                    if(SOAD_SOCON_RECONNECT != SoAd_SoConManager[SoConId].SoAd_SoConMode)
                    {
                        SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                    }
                }
                else
                {
                    /*don't care the return value*/
                    (void)TcpIp_Close(socketId, TRUE);
                }
            }
            else
            {
                /*don't care the return value*/
                (void)TcpIp_Close(socketId, TRUE);
            }
        }
    }
    else
    {
        /*SWS_SoAd_00686:tcp server may have changed to ONLINE before*/
        if(SOAD_SOCON_OFFLINE == SoAd_SoConManager[SoConId].SoAd_SoConMode)
        {
            SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
        }
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_OpenTcpClientSoConHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    TcpIp_SocketIdType socketId;
    uint16 localPort;
    if(E_OK == TcpIp_SoAdGetSocket((SoAd_SoConManager[SoConId].RemoteAddr.domain),
                                   TCPIP_IPPROTO_TCP, &socketId))
    {
        /*Change the socket specific parameters according to [SWS_SoAd_00689]*/
        SoAd_ChangeSocketParameter(
#if (STD_ON == SOAD_SUPPORT_TCP)
                TRUE,
#endif
                socketId, SoConGroup);
        SoAd_SoConManager[SoConId].TcpIpSocketValid = TRUE;
        SoAd_SoConManager[SoConId].TcpIpSocketId = socketId;
        /*Bind the socket to the local address and port*/
        localPort = SOAD_SOCONGROUP_CFG(SoConGroup).SoAd_SocketLocalPort;
        if(E_OK == TcpIp_Bind(socketId,
                              SOAD_SOCKETLOCALADDR_CFG(SoConGroup).LocalAddrId,
                              &localPort))
        {
            if(E_OK == TcpIp_TcpConnect(socketId, &(SoAd_SoConManager[SoConId].RemoteAddr)))
            {
                /*SWS_SoAd_00686*/
                if(SOAD_SOCON_RECONNECT != SoAd_SoConManager[SoConId].SoAd_SoConMode)
                {
                    SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
            }
            else
            {
                /*don't care the return value*/
                (void)TcpIp_Close(socketId, TRUE);
            }
        }
        else
        {
            /*don't care the return value*/
            (void)TcpIp_Close(socketId, TRUE);
        }
    }
    return;
}


static FUNC(void, SOAD_CODE)
SoAd_SoConModeChangeHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx soConGroup;
    soConGroup = SOAD_SOCON_CFG(SoConId).SoAd_SocketConnectionGroupRef;
    if(SOAD_SOCON_REQUEST_OPEN == SoAd_SoConManager[SoConId].SoCon_Request[0u])
    {
        if(SOAD_SOCON_OFFLINE != SoAd_SoConManager[SoConId].SoAd_SoConMode)
        {
            SoAd_SoConManager[SoConId].SoCon_Request[0u] = SoAd_SoConManager[SoConId].SoCon_Request[1u];
            SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
        }
        /*SWS_SoAd_00589*/
        else if((TRUE == SoAd_SoConManager[SoConId].RemoteAddrIsSet)
                && (TCPIP_IPADDR_STATE_ASSIGNED == SoAd_SoConGroupManager[soConGroup].LocalIpAddrState)
                && (FALSE == SoAd_SoConManager[SoConId].TcpIpSocketValid))
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            if(TCPIP_IPPROTO_UDP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if(1u == SOAD_SOCONGROUP_CFG(soConGroup).SoAd_SoConNum)
                {
                    SoAd_OpenUdpSoConHandle(SoConId,soConGroup);
                }
                /*SWS_SoAd_00639*/
                else
                {
                    if(FALSE == SoAd_SoConGroupManager[soConGroup].TcpIpSocketValid)
                    {
                        SoAd_OpenUdpSoConHandle(SoConId,soConGroup);
                    }
                    /*Activate the socket connection for communication */
                    else
                    {
                        SoAd_SoConManager[SoConId].TcpIpSocketValid = TRUE;
                        SoAd_SoConManager[SoConId].TcpIpSocketId = SoAd_SoConGroupManager[soConGroup].TcpIpSocketId;
                        (SoAd_SoConGroupManager[soConGroup].SoConSocketValidNum) += 1u;
                        /*SWS_SoAd_00591*/
                        if((TRUE == SOAD_UDPTYPE_CFG(soConGroup)->SoAd_SocketUdpListenOnly)
                                || ((!TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[SoConId].RemoteAddr))
                                    && (TCPIP_PORT_ANY != SoAd_SoConManager[SoConId].RemoteAddr.port)))
                        {
                            if(SOAD_SOCON_ONLINE != SoAd_SoConManager[SoConId].SoAd_SoConMode)
                            {
                                SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_ONLINE;
                                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
                            }
                        }
                        /*SWS_SoAd_00686*/
                        else
                        {
                            if(SOAD_SOCON_RECONNECT != SoAd_SoConManager[SoConId].SoAd_SoConMode)
                            {
                                SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
                            }
                        }
                    }
                }
            }
#endif/* STD_ON == SOAD_SUPPORT_UDP */
#if (STD_ON == SOAD_SUPPORT_TCP)
            if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if(TRUE == SOAD_TCPTYPE_CFG(soConGroup)->SoAd_SocketTcpInitiate)
                {
                    SoAd_OpenTcpClientSoConHandle(SoConId,soConGroup);
                }
                /*SWS_SoAd_00638*/
                else
                {
                    SoAd_OpenTcpServerSoConHandle(SoConId,soConGroup);
                }
            }
#endif/* STD_ON == SOAD_SUPPORT_TCP */
            /*The Open request is cleared only when the Open request is successfully executed*/
            if(SOAD_SOCON_OFFLINE != SoAd_SoConManager[SoConId].SoAd_SoConMode)
            {
                SoAd_SoConManager[SoConId].SoCon_Request[0u] = SoAd_SoConManager[SoConId].SoCon_Request[1u];
                SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
            }
        }
        else
        {
            /*do nothing*/
        }
    }
    else if(SOAD_SOCON_REQUEST_CLOSE == SoAd_SoConManager[SoConId].SoCon_Request[0u])
    {
        /*SWS_SoAd_00604*/
        if(SOAD_SOCON_OFFLINE != SoAd_SoConManager[SoConId].SoAd_SoConMode)
        {
            #if (STD_ON == SOAD_SUPPORT_TCP)
            if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                SoAd_SoConManager[SoConId].ChangeModeToOffline = TRUE;
            }
            #endif/*STD_ON == SOAD_SUPPORT_TCP*/
            SoAd_SoConCloseHandle(SoConId, soConGroup,TRUE);
            #if (STD_ON == SOAD_SUPPORT_UDP)
            if(TCPIP_IPPROTO_UDP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                SoAd_SoConManager[SoConId].TcpIpSocketValid = FALSE;
                SoAd_SoConManager[SoConId].TcpIpSocketId = (TcpIp_SocketIdType)SOAD_UNUSED_UINT16;
                SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_OFFLINE;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, soConGroup);
            }
            #endif/*STD_ON == SOAD_SUPPORT_UDP*/
        }
        SoAd_SoConManager[SoConId].SoCon_Request[0u] = SoAd_SoConManager[SoConId].SoCon_Request[1u];
        SoAd_SoConManager[SoConId].SoCon_Request[1u] = SOAD_SOCON_REQUEST_NONE;
    }
    else
    {
        /*handle the Disconnection And Recovery of SoCon*/
        SoAd_DisconnectionAndRecoveryHandle(SoConId, soConGroup);
    }
    return;
}

#if(0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_CloseRxTpPduHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    SoAd_uintx rxBufferId;
    SoAd_uintx soAdSocketRouteId = SOAD_UNUSED_UINT16;
    PduIdType pduId;
    SoAd_UpLayerModuleType upModule;
    rxBufferId = *SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr;
    rxBufferId = rxBufferId - SOAD_TXBUFFER_NUM;
    if(SOAD_UNUSED_UINT16 == SoAd_SoRxBufMan[rxBufferId].SoAd_SocketRouteId)
    {
        if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr)
        {
            soAdSocketRouteId = SOAD_SOCON_CFG(SoConId).SoAd_RefSocketRoutePtr[0u];
        }
    }
    else
    {
        soAdSocketRouteId = SoAd_SoRxBufMan[rxBufferId].SoAd_SocketRouteId;
    }
    if((soAdSocketRouteId < SOAD_SOCKET_ROUTE_DEST_NUM)
            && (SOAD_UPPER_LAYER_TP == SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_RxUpperLayerType))
    {
        upModule = SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_RefModule;
        pduId = SOAD_SRDEST_CFG(soAdSocketRouteId).SoAd_UpPduId;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

    #if(STD_ON == SOAD_SUPPORT_HEADERID)
        if(NULL_PTR != SOAD_SOCKETROUTE_CFG(soAdSocketRouteId).SoAd_RxPduHeaderId)
        {
            if(SoAd_SoRxBufMan[rxBufferId].ExitPduResidueLength > 0u)
            {
                SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(pduId, E_NOT_OK);
            }
        }
        else
    #endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
        {
            SOAD_UPPERLAY_PCCFG(upModule).Tp_RxIndication_FuncPtr(pduId, E_NOT_OK);
        }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    }
    /*clear the SoCon Rx Buffer*/
    SoAd_RxBufferInit(rxBufferId);
    return;
}
#endif/*(0u < SOAD_RXBUFFER_NUM)*/

static FUNC(void, SOAD_CODE)
SoAd_SoConCloseHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup,
    VAR(boolean, AUTOMATIC) CloseSocket)
{
#if(0u < SOAD_PDU_ROUTE_DEST_MAX)
    SoAd_uintx cnt,pduRouteDestId;
    PduIdType pduRouteId;
#endif
#if((0u < SOAD_RXBUFFER_NUM) || (0u < SOAD_PDU_ROUTE_DEST_MAX))
    SoAd_UpLayerModuleType upModule;
#endif
#if(0u < SOAD_PDU_ROUTE_DEST_MAX)
    for(cnt = 0u;cnt < SOAD_SOCON_CFG(SoConId).SoAd_RefPduRouteDestNum;cnt++)
    {
        pduRouteDestId = SOAD_SOCON_CFG(SoConId).SoAd_RefPduRouteDestPtr[cnt];
        if(0u < SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength)
        {
            /*Init PduRouteDestManager parameter*/
            SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength = 0u;
        }
        pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
        /*Init PduRouteManager parameter*/
        SoAd_PduRouteManagerInit(pduRouteId);
        if((TRUE == SoAd_SoConManager[SoConId].TpPduTransmitting)
                && (pduRouteId == SoAd_SoConManager[SoConId].ActiveSoAdPduRouteId))
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
            if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
            {
                /*TP transmitting,call the up TpTxConfirmation with E_NOT_OK*/
                SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                        SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId,
                        E_NOT_OK);
            }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

        }
    }
#if(0u < SOAD_TXBUFFER_NUM)
    if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_TxBufCfgIndexPtr)
    {
        /*Init Tx Buffer parameter*/
        SoAd_TxBufferInit(*SOAD_SOCON_CFG(SoConId).SoAd_TxBufCfgIndexPtr);
    }
#endif/*0u < SOAD_TXBUFFER_NUM*/
#endif/*0u < SOAD_PDU_ROUTE_DEST_MAX*/

#if(0u < SOAD_RXBUFFER_NUM)
    /* TP PDU is receiving handle*/
    if(NULL_PTR != SOAD_SOCON_CFG(SoConId).SoAd_RxBufCfgIndexPtr)
    {
        SoAd_CloseRxTpPduHandle(SoConId);
    }
#endif/*0u < SOAD_RXBUFFER_NUM*/

#if (STD_ON == SOAD_SUPPORT_TCP)
    /*close the tcpip socket*/
    if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        if(TRUE == CloseSocket)
        {
            if(SoAd_SoConManager[SoConId].TcpIpSocketValid == TRUE)
            {
                (void)TcpIp_Close(SoAd_SoConManager[SoConId].TcpIpSocketId,
                                              SoAd_SoConManager[SoConId].SoConCloseAbort);
            }
            else
            {
                if(SoAd_SoConManager[SoConId].ChangeModeToOffline == TRUE)
                {
                    SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_OFFLINE;
                    SoAd_SoConManager[SoConId].ChangeModeToOffline = FALSE;
                    SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                }
                if((SoAd_SoConGroupManager[SoConGroup].TcpIpSocketValid == TRUE)
                    && (SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum == 0u))
                {
                    (void)TcpIp_Close(SoAd_SoConGroupManager[SoConGroup].TcpIpSocketId,
                                      SoAd_SoConManager[SoConId].SoConCloseAbort);
                }
            }
        }
    }
#endif/* STD_ON == SOAD_SUPPORT_TCP */
#if (STD_ON == SOAD_SUPPORT_UDP)
    if(TCPIP_IPPROTO_UDP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        if((0u < SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum)
                && (TRUE == SoAd_SoConManager[SoConId].TcpIpSocketValid))
        {
            (SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum) -= 1u;
        }
        if(0u == SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum)
        {
            if(TRUE == CloseSocket)
            {
                (void)TcpIp_Close(SoAd_SoConGroupManager[SoConGroup].TcpIpSocketId,
                                  SoAd_SoConManager[SoConId].SoConCloseAbort);
            }
        }
    }
#endif/* STD_ON == SOAD_SUPPORT_UDP */
    /*clear the SoAd_SoConManager parameters*/
    SoAd_SoConManager[SoConId].SoConCloseAbort = FALSE;
    SoAd_SoConManager[SoConId].SocketNeedClose = FALSE;
    SoAd_SoConManager[SoConId].TpPduTransmitting = FALSE;
    SoAd_SoConManager[SoConId].TxNoAckLength = 0u;
#if (STD_ON == SOAD_SUPPORT_UDP)
    SoAd_SoConManager[SoConId].UdpAliveSupervisionTime = 0u;
#endif/*STD_ON == SOAD_SUPPORT_UDP*/
    /*SWS_SoAd_00527:SoAd shall reset the remote address to the configured remote
      address (or unset the remote address in case no remote address has been
      configured)*/
    if(NULL_PTR != SOAD_SOCONREMOTEADDR_CFG(SoConId))
    {
        SoAd_SoConManager[SoConId].RemoteAddr = *SOAD_SOCONREMOTEADDR_CFG(SoConId);
    }
    return;
}

static FUNC(void, SOAD_CODE)
SoAd_DisconnectionAndRecoveryHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(SoAd_uintx, AUTOMATIC) SoConGroup)
{
    /* handle the disconnection */
    if(TRUE == SoAd_SoConManager[SoConId].SocketNeedClose)
    {
        SoAd_SoConCloseHandle(SoConId, SoConGroup,TRUE);
        /*Notification UP Module SoCon changed*/
        if(SoAd_SoConManager[SoConId].SoAd_SoConMode == SOAD_SOCON_ONLINE)
        {
            SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
            SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
        }
    }
    else if(SOAD_SOCON_RECONNECT == SoAd_SoConManager[SoConId].SoAd_SoConMode)
    {
        if((TCPIP_IPADDR_STATE_ASSIGNED == SoAd_SoConGroupManager[SoConGroup].LocalIpAddrState)
                && (TRUE == SoAd_SoConManager[SoConId].RemoteAddrIsSet))
        {
#if (STD_ON == SOAD_SUPPORT_UDP)
            /*UDP socket connection recovery*/
            if(TCPIP_IPPROTO_UDP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                if(FALSE == SoAd_SoConGroupManager[SoConGroup].TcpIpSocketValid)
                {
                    SoAd_OpenUdpSoConHandle(SoConId,SoConGroup);
                }
                else
                {
                    /*SWS_SoAd_00686*/
                    if((TRUE == SOAD_UDPTYPE_CFG(SoConGroup)->SoAd_SocketUdpListenOnly)
                            || ((!TCPIP_IS_IPADDR_ANY(SoAd_SoConManager[SoConId].RemoteAddr))
                                && (TCPIP_PORT_ANY != SoAd_SoConManager[SoConId].RemoteAddr.port)))
                    {
                        SoAd_SoConManager[SoConId].SoAd_SoConMode = SOAD_SOCON_ONLINE;
                        SoAd_SoConModeChgNotificationAndTpStartOfReception(SoConId, SoConGroup);
                    }
                    if(FALSE == SoAd_SoConManager[SoConId].TcpIpSocketValid)
                    {
                        SoAd_SoConManager[SoConId].TcpIpSocketValid = TRUE;
                        SoAd_SoConManager[SoConId].TcpIpSocketId =
                            SoAd_SoConGroupManager[SoConGroup].TcpIpSocketId;
                        (SoAd_SoConGroupManager[SoConGroup].SoConSocketValidNum) += 1u;
                    }
                }
            }
#endif/* STD_ON == SOAD_SUPPORT_UDP */
#if (STD_ON == SOAD_SUPPORT_TCP)
            if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                /*SWS_SoAd_00590*/
                if(TRUE == SOAD_TCPTYPE_CFG(SoConGroup)->SoAd_SocketTcpInitiate)
                {
                    if(FALSE == SoAd_SoConManager[SoConId].TcpIpSocketValid)
                    {
                        SoAd_OpenTcpClientSoConHandle(SoConId,SoConGroup);
                    }
                }
                /*SWS_SoAd_00638*/
                else
                {
                    SoAd_OpenTcpServerSoConHandle(SoConId,SoConGroup);
                }
            }
#endif/* STD_ON == SOAD_SUPPORT_TCP */
        }
    }
    else
    {
        /*do nothing*/
    }
    return;
}

/* id: routing group identifier specifying the routing group
enFlag: group control enable shall be set TRUE otherwise is FALSE
soConIdPtr:socket connection index specifying the socket connection on
which the routing group  */
#if(0u < SOAD_ROUTING_GROUP_MAX)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_RoutingGroupControl(
    VAR(SoAd_RoutingGroupIdType, AUTOMATIC) id,
    VAR(boolean, AUTOMATIC) enFlag,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) soConIdPtr)
{
    SoAd_uintx cnt,routeDestId;
    SoAd_RoutingGroupIdType routeGroupId,routeGroupCnt;
    boolean flagSet;
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
    const SoAd_SocketRouteDestType* soRuDestCfgPtr;
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
    const SoAd_PduRouteDestType* pduRouDestCfgPtr;
#endif
    if(NULL_PTR != soConIdPtr)
    {
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
        for(cnt = 0u;cnt < SOAD_SOCON_CFG(*soConIdPtr).SoAd_RefSocketRouteNum;cnt++)
        {
            routeDestId = SOAD_SOCON_CFG(*soConIdPtr).SoAd_RefSocketRoutePtr[cnt];
            soRuDestCfgPtr = &SOAD_SRDEST_CFG(routeDestId);
            flagSet = FALSE;
            for(routeGroupCnt = 0u;
                    (routeGroupCnt < soRuDestCfgPtr->SoAd_RxRoutingGroupRefNum)&&(FALSE == flagSet);
                    routeGroupCnt++)
            {
                routeGroupId = soRuDestCfgPtr->SoAd_RxRoutingGroupRefPtr[routeGroupCnt];
                if(id == routeGroupId)
                {
                    SoAd_SocketRouteDestManager[routeDestId].SocketRouteDestEnable = enFlag;
                    flagSet = TRUE;
                }
            }
        }
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
        for(cnt = 0u;cnt < SOAD_SOCON_CFG(*soConIdPtr).SoAd_RefPduRouteDestNum;cnt++)
        {
            routeDestId = SOAD_SOCON_CFG(*soConIdPtr).SoAd_RefPduRouteDestPtr[cnt];
            pduRouDestCfgPtr = &SOAD_PDUROTDEST_CFG(routeDestId);
            flagSet = FALSE;
            for(routeGroupCnt = 0u;
                    (routeGroupCnt < pduRouDestCfgPtr->SoAd_TxRoutingGroupRefNum)&&(FALSE == flagSet);
                    routeGroupCnt++)
            {
                routeGroupId = pduRouDestCfgPtr->SoAd_TxRoutingGroupRefPtr[routeGroupCnt];
                if(id == routeGroupId)
                {
                    SoAd_PduRouteDestManager[routeDestId].PduRouteDestEnable = enFlag;
                    flagSet = TRUE;
                }
            }
        }
#endif
    }
    else
    {
        const SoAd_RoutingGroupType* routGroupCfgPtr = &SoAd_PbCfgPtr->SoAd_RoutingGroup[id];
        SoAd_RoutGroupManager[id].RoutGroupEnable = enFlag;
#if (0u < SOAD_SOCKET_ROUTE_DEST_MAX)
        for(cnt = 0u;cnt < routGroupCfgPtr->SoAd_SocketRouteDestNum;cnt++)
        {
            routeDestId = routGroupCfgPtr->SoAd_SocketRouteDestIndexPtr[cnt];
            soRuDestCfgPtr = &SOAD_SRDEST_CFG(routeDestId);
            flagSet = FALSE;
            for(routeGroupCnt = 0u;
                    (routeGroupCnt < soRuDestCfgPtr->SoAd_RxRoutingGroupRefNum)&&(FALSE == flagSet);
                    routeGroupCnt++)
            {
                routeGroupId = soRuDestCfgPtr->SoAd_RxRoutingGroupRefPtr[routeGroupCnt];
                if(TRUE == SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    flagSet = TRUE;
                }
            }
            SoAd_SocketRouteDestManager[routeDestId].SocketRouteDestEnable = flagSet;
        }
#endif
#if (0u < SOAD_PDU_ROUTE_DEST_MAX)
        for(cnt = 0u;cnt < routGroupCfgPtr->SoAd_PduRouteDestNum;cnt++)
        {
            routeDestId = routGroupCfgPtr->SoAd_PduRouteDestIndexPtr[cnt];
            pduRouDestCfgPtr = &SOAD_PDUROTDEST_CFG(routeDestId);
            flagSet = FALSE;
            for(routeGroupCnt = 0u;
                    (routeGroupCnt < pduRouDestCfgPtr->SoAd_TxRoutingGroupRefNum)&&(FALSE == flagSet);
                    routeGroupCnt++)
            {
                routeGroupId = pduRouDestCfgPtr->SoAd_TxRoutingGroupRefPtr[routeGroupCnt];
                if(TRUE == SoAd_RoutGroupManager[routeGroupId].RoutGroupEnable)
                {
                    flagSet = TRUE;
                }
            }
            SoAd_PduRouteDestManager[routeDestId].PduRouteDestEnable = flagSet;
        }
#endif
    }
    return E_OK;
}
#endif/*0u < SOAD_ROUTING_GROUP_MAX*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_TxBufferInit(SoAd_uintx TxBufferId)
{
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    SoAd_SoTxBufMan[TxBufferId].tirgTimer = 0u;
#endif/*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/
    SoAd_SoTxBufMan[TxBufferId].upCopyLength = 0u;
    SoAd_SoTxBufMan[TxBufferId].txPduTotalLength = 0u;
    SoAd_SoTxBufMan[TxBufferId].txResidueLength = SOAD_SOCKETBUFFER_CFG(TxBufferId).bufLen;
    return;
}
#endif/*0u < SOAD_TXBUFFER_NUM*/

#if (0u < SOAD_RXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_RxBufferInit(SoAd_uintx RxBufferId)
{
    SoAd_SoRxBufMan[RxBufferId].EnterPosition = 0x0u;
    SoAd_SoRxBufMan[RxBufferId].ExitPosition = 0x0u;
    SoAd_SoRxBufMan[RxBufferId].ExitPduResidueLength = 0x0u;
    SoAd_SoRxBufMan[RxBufferId].SoAd_SocketRouteId = SOAD_UNUSED_UINT16;
    return;
}
#endif/*0u < SOAD_RXBUFFER_NUM*/

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(void, SOAD_CODE)
SoAd_MainFunctionNPduUdpTxHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx txBufferId;
    if(NULL_PTR != SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId))
    {
        txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
        if(0u < SoAd_SoTxBufMan[txBufferId].tirgTimer)
        {
            SoAd_SoTxBufMan[txBufferId].tirgTimer -= 1u;
            if(0u == SoAd_SoTxBufMan[txBufferId].tirgTimer)
            {
                (void)TcpIp_UdpTransmit(
                          SoAd_SoConManager[SoConId].TcpIpSocketId,
                          &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                          &SoAd_SoConManager[SoConId].RemoteAddr,
                          (uint16)SoAd_SoTxBufMan[txBufferId].txPduTotalLength);
                SoAd_TxBufferInit(txBufferId);
                SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId,TCPIP_IPPROTO_UDP);
            }
        }
    }
    return;
}
#endif

static FUNC(void, SOAD_CODE)
SoAd_MainFunctionTxConfirmationHandle(void)
{
    PduIdType txPduId;
    SoAd_UpLayerModuleType upModule;
    for(txPduId = 0u;txPduId < SOAD_PDU_ROUTE_NUM;txPduId++)
    {
        if(0u == SoAd_PduRoutManager[txPduId].TxPendingNum)
        {
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

            upModule = SOAD_PDUROUT_CFG(txPduId).SoAd_RefModule;
            if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr)
            {
                SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr(
                         SOAD_PDUROUT_CFG(txPduId).SoAd_UpPduId);
            }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

            SoAd_PduRouteManagerInit(txPduId);
        }
    }
    return;
}

#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_NPduUdpTransmitHandle(
        PduIdType TxPduId,
        SoAd_uintx PduRouteDestId,
        SoAd_SoConIdType SoConId)
{
    Std_ReturnType result;
    SoAd_uintx txBufferId;
    PduLengthType pduLength;
    txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    pduLength = SoAd_PduRoutManager[TxPduId].PduLength + SOAD_HEADER_SIZE;
    SOAD_FILL_HEADER(
        &SoAd_IfPduBuffer[0],
        *SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId,
        SoAd_PduRoutManager[TxPduId].PduLength);
    if(SOAD_TRIGGER_ALWAYS == *(SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxUdpTriggerMode))
    {
        if(SoAd_SoTxBufMan[txBufferId].txResidueLength < pduLength)
        {
            (void)TcpIp_UdpTransmit(
                      SoAd_SoConManager[SoConId].TcpIpSocketId,
                      &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                      &SoAd_SoConManager[SoConId].RemoteAddr,
                      (uint16)SoAd_SoTxBufMan[txBufferId].txPduTotalLength);

            SoAd_TxBufferInit(txBufferId);

            result = TcpIp_UdpTransmit(
                         SoAd_SoConManager[SoConId].TcpIpSocketId,
                         &SoAd_IfPduBuffer[0],
                         &SoAd_SoConManager[SoConId].RemoteAddr,
                         (uint16)pduLength);
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId,TCPIP_IPPROTO_UDP);
        }
        else
        {
            SoAd_LocalMemCpy(
                    &SoAd_PduBuffer[(SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos)+(SoAd_SoTxBufMan[txBufferId].txPduTotalLength)],
                    &SoAd_IfPduBuffer[0],
                    pduLength);
            SoAd_SoTxBufMan[txBufferId].txPduTotalLength += pduLength;
            SoAd_SoTxBufMan[txBufferId].txResidueLength -= pduLength;
            result = TcpIp_UdpTransmit(
                         SoAd_SoConManager[SoConId].TcpIpSocketId,
                         &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                         &SoAd_SoConManager[SoConId].RemoteAddr,
                         (uint16)SoAd_SoTxBufMan[txBufferId].txPduTotalLength);

            SoAd_TxBufferInit(txBufferId);
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId,TCPIP_IPPROTO_UDP);
        }
    }
    else
    {
        if(SoAd_SoTxBufMan[txBufferId].txResidueLength < pduLength)
        {
            (void)TcpIp_UdpTransmit(
                      SoAd_SoConManager[SoConId].TcpIpSocketId,
                      &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                      &SoAd_SoConManager[SoConId].RemoteAddr,
                      (uint16)SoAd_SoTxBufMan[txBufferId].txPduTotalLength);

            SoAd_TxBufferInit(txBufferId);

            SoAd_LocalMemCpy(
                    &SoAd_PduBuffer[(SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos)+(SoAd_SoTxBufMan[txBufferId].txPduTotalLength)],
                    &SoAd_IfPduBuffer[0],
                    pduLength);
            SoAd_SoTxBufMan[txBufferId].txPduTotalLength += pduLength;
            SoAd_SoTxBufMan[txBufferId].txResidueLength -= pduLength;
            SoAd_SoTxBufMan[txBufferId].tirgTimer = SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_CfgUdpTriggerTimeout;
            result = E_OK;
        }
        else
        {
            SoAd_LocalMemCpy(
                    &SoAd_PduBuffer[(SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos)+(SoAd_SoTxBufMan[txBufferId].txPduTotalLength)],
                    &SoAd_IfPduBuffer[0],
                    pduLength);
            SoAd_SoTxBufMan[txBufferId].txPduTotalLength += pduLength;
            SoAd_SoTxBufMan[txBufferId].txResidueLength -= pduLength;
            if((SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_CfgUdpTriggerTimeout < SoAd_SoTxBufMan[txBufferId].tirgTimer)
                || (SoAd_SoTxBufMan[txBufferId].tirgTimer == 0u))
            {
                SoAd_SoTxBufMan[txBufferId].tirgTimer = SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_CfgUdpTriggerTimeout;
            }
            result = E_OK;
        }
    }
    if(E_OK == result)
    {
        if(SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[TxPduId].TxPendingNum)
        {
            SoAd_PduRoutManager[TxPduId].TxPendingNum = 0u;
        }
    }
    return result;
}
#endif/*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/

#if (0u < SOAD_MAX_IF_PDU_SIZE)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_IfTransmitSoCon(PduIdType TxPduId,SoAd_uintx PduRouteDestId,SoAd_SoConIdType SoConId)
{
    Std_ReturnType result;
#if (STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER)
    if(NULL_PTR != SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxUdpTriggerMode)
    {
        result = SoAd_NPduUdpTransmitHandle(TxPduId,PduRouteDestId,SoConId);
    }
    else
#endif/*STD_ON == SOAD_SUPPORT_NPDUUDPTXBUFFER*/
    {
        PduLengthType pduLength;
        uint8 *dataPtr;
        #if (STD_ON == SOAD_SUPPORT_HEADERID)
        if(NULL_PTR != SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId)
        {
            SOAD_FILL_HEADER(
                &SoAd_IfPduBuffer[0],
                *SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId,
                SoAd_PduRoutManager[TxPduId].PduLength);
            pduLength = SoAd_PduRoutManager[TxPduId].PduLength + SOAD_HEADER_SIZE;
            dataPtr = &SoAd_IfPduBuffer[0];
        }
        else
        #endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
        {
            pduLength = SoAd_PduRoutManager[TxPduId].PduLength;
            dataPtr = SoAd_PduRoutManager[TxPduId].PduDataPtr;
        }
        if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
        {
            /*transmit success manage parameter set*/
            SoAd_SoConManager[SoConId].TxNoAckLength += pduLength;
            SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = SoAd_SoConManager[SoConId].TxNoAckLength;
            if(SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[TxPduId].TxPendingNum)
            {
                SoAd_PduRoutManager[TxPduId].TxPendingNum = 1u;
            }
            else
            {
                SoAd_PduRoutManager[TxPduId].TxPendingNum += 1u;
            }
            result = TcpIp_TcpTransmit(
                         SoAd_SoConManager[SoConId].TcpIpSocketId,
                         dataPtr,
                         pduLength,
                         TRUE);
            if(E_OK != result)
            {
                SoAd_SoConManager[SoConId].TxNoAckLength -= pduLength;
                SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = 0u;
                if(1u == SoAd_PduRoutManager[TxPduId].TxPendingNum)
                {
                    SoAd_PduRoutManager[TxPduId].TxPendingNum = SOAD_UNUSED_UINT8;
                }
                else
                {
                    SoAd_PduRoutManager[TxPduId].TxPendingNum -= 1u;
                }
            }
        }
        else
        {
            result = TcpIp_UdpTransmit(
                         SoAd_SoConManager[SoConId].TcpIpSocketId,
                         dataPtr,
                         &SoAd_SoConManager[SoConId].RemoteAddr,
                         (uint16)pduLength);
            if(E_OK == result)
            {
                if(SOAD_UNUSED_UINT8 == SoAd_PduRoutManager[TxPduId].TxPendingNum)
                {
                    SoAd_PduRoutManager[TxPduId].TxPendingNum = 0u;
                }
            }
            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId,TCPIP_IPPROTO_UDP);
        }
    }
    if(E_NOT_OK == result)
    {
        SoAd_PduRoutManager[TxPduId].TxPendingNum = SOAD_UNUSED_UINT8;
    }
    return result;
}
#endif/*0u < SOAD_MAX_IF_PDU_SIZE*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_MainFunctionTpTransmitHandle(SoAd_SoConIdType SoConId)
{
    SoAd_uintx txBufferId;
    txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    if((TRUE == SoAd_SoConManager[SoConId].TpPduTransmitting)
            && (FALSE == SoAd_SoConManager[SoConId].SocketNeedClose)
            && (SoAd_SoTxBufMan[txBufferId].upCopyLength < SoAd_SoTxBufMan[txBufferId].txPduTotalLength))
    {
        PduInfoType PduInfo;
        PduIdType pduRouteId,upPduId;
        SoAd_UpLayerModuleType upModule;
        PduLengthType validDataLength;
        BufReq_ReturnType result = BUFREQ_E_NOT_OK;
        Std_ReturnType resultLo;
        PduInfo.MetaDataPtr = NULL_PTR;
        PduInfo.SduLength = 0u;
        PduInfo.SduDataPtr = NULL_PTR;
        pduRouteId = SoAd_SoConManager[SoConId].ActiveSoAdPduRouteId;
        upPduId = SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
        upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
        if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr)
        {
            result = SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr(
                          upPduId,&PduInfo,NULL_PTR,&validDataLength);
            if((BUFREQ_OK == result)
                    && (SoAd_SoTxBufMan[txBufferId].txPduTotalLength >=
                            (SoAd_SoTxBufMan[txBufferId].upCopyLength + validDataLength)))
            {
                if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
                {
                    if(E_NOT_OK == TcpIp_TcpTransmit(
                                 SoAd_SoConManager[SoConId].TcpIpSocketId,
                                 NULL_PTR,
                                 validDataLength,
                                 FALSE))
                    {
                        result = BUFREQ_E_NOT_OK;
                    }
                }
                else
                {
                    PduInfo.SduDataPtr = &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos
                                                         + SoAd_SoTxBufMan[txBufferId].upCopyLength];
                    PduInfo.SduLength = validDataLength;
                    result = SOAD_UPPERLAY_PCCFG(upModule).Tp_CopyTxData_FuncPtr(
                                  upPduId,&PduInfo,NULL_PTR,&validDataLength);
                    if(BUFREQ_OK == result)
                    {
                        SoAd_SoTxBufMan[txBufferId].upCopyLength += PduInfo.SduLength;
                        if(SoAd_SoTxBufMan[txBufferId].txPduTotalLength ==
                                SoAd_SoTxBufMan[txBufferId].upCopyLength)
                        {
                            resultLo = TcpIp_UdpTransmit(
                                         SoAd_SoConManager[SoConId].TcpIpSocketId,
                                         &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                                         &SoAd_SoConManager[SoConId].RemoteAddr,
                                         (uint16)SoAd_SoTxBufMan[txBufferId].txPduTotalLength);
                            if(E_OK == resultLo)
                            {
                                SoAd_SoTxBufMan[txBufferId].txPduTotalLength = 0u;
                                SoAd_SoTxBufMan[txBufferId].upCopyLength = 0u;
                                SoAd_SoConManager[SoConId].TpPduTransmitting = FALSE;
                                if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
                                {
                                    SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                                            upPduId,E_OK);
                                }
                            }
                            else
                            {
                                result = BUFREQ_E_NOT_OK;
                            }
                            SoAd_SocnTransmitAutomaticSoConSetupHandle(SoConId,TCPIP_IPPROTO_UDP);
                        }
                    }
                }
            }
            else
            {
                result = BUFREQ_E_NOT_OK;
            }
        }
        if(BUFREQ_E_NOT_OK == result)
        {
            SoAd_TpPduTxErrorHandle(SoConId,txBufferId,upModule,upPduId);
        }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    }
    return;
}
#endif/*0u < SOAD_TXBUFFER_NUM*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(void, SOAD_CODE)
SoAd_TpPduTxErrorHandle(
        SoAd_SoConIdType SoConId,
        SoAd_uintx TxBufferId,
        SoAd_UpLayerModuleType UpModule,
        PduIdType UpPduId)
{
    if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
    {
        SoAd_SoConManager[SoConId].SocketNeedClose = TRUE;
    }
    else
    {
        SoAd_TxBufferInit(TxBufferId);
        SoAd_SoConManager[SoConId].TpPduTransmitting = FALSE;
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)

        if(NULL_PTR != SOAD_UPPERLAY_PCCFG(UpModule).Tp_TxConfirmation_FuncPtr)
        {
            SOAD_UPPERLAY_PCCFG(UpModule).Tp_TxConfirmation_FuncPtr(UpPduId, E_NOT_OK);
        }
#endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */

    }
    return;
}
#endif/*0u < SOAD_TXBUFFER_NUM*/

#if (0u < SOAD_TXBUFFER_NUM)
static FUNC(Std_ReturnType, SOAD_CODE)
SoAd_TpTransmitSoCon(
       SoAd_SoConIdType SoConId,
       SoAd_uintx PduRouteDestId,
       P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) SoAdSrcPduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    SoAd_uintx txBufferId;
    txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(SoConId);
    if((SOAD_SOCKETBUFFER_CFG(txBufferId).bufLen >= SoAdSrcPduInfoPtr->SduLength)
            || (0u == SOAD_SOCKETBUFFER_CFG(txBufferId).bufLen))
    {
        #if (STD_ON == SOAD_SUPPORT_HEADERID)
        if(NULL_PTR != SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId)
        {
            if(TCPIP_IPPROTO_TCP == SOAD_SOCON_CFG(SoConId).SoAd_TcpUdpProtocol)
            {
                uint8 tcpHeaderData[SOAD_HEADER_SIZE];
                SOAD_FILL_HEADER(
                    &tcpHeaderData[0],
                    *SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId,
                    SoAdSrcPduInfoPtr->SduLength);
                /*transmit success manage parameter set*/
                SoAd_SoConManager[SoConId].TxNoAckLength += SOAD_HEADER_SIZE;
                SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = SoAd_SoConManager[SoConId].TxNoAckLength;
                SoAd_SoTxBufMan[txBufferId].txPduTotalLength = (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE);
                SoAd_SoTxBufMan[txBufferId].upCopyLength = SOAD_HEADER_SIZE;
                SoAd_SoConManager[SoConId].TpPduTransmitting = TRUE;
                ret = TcpIp_TcpTransmit(
                             SoAd_SoConManager[SoConId].TcpIpSocketId,
                             &tcpHeaderData[0],
                             SOAD_HEADER_SIZE,
                             TRUE);
                if(E_OK != ret)
                {
                    SoAd_SoConManager[SoConId].TxNoAckLength -= SOAD_HEADER_SIZE;
                    SoAd_PduRouteDestManager[PduRouteDestId].TxPendingLength = 0u;
                    SoAd_SoTxBufMan[txBufferId].txPduTotalLength = 0u;
                    SoAd_SoTxBufMan[txBufferId].upCopyLength = 0u;
                    SoAd_SoConManager[SoConId].TpPduTransmitting = FALSE;
                }
            }
            else
            {
                if(SOAD_SOCKETBUFFER_CFG(txBufferId).bufLen >= (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE))
                {
                    SOAD_FILL_HEADER(
                        &SoAd_PduBuffer[SOAD_SOCKETBUFFER_CFG(txBufferId).bufPos],
                        *SOAD_PDUROTDEST_CFG(PduRouteDestId).SoAd_TxPduHeaderId,
                        SoAdSrcPduInfoPtr->SduLength);
                    SoAd_SoTxBufMan[txBufferId].txPduTotalLength = (SoAdSrcPduInfoPtr->SduLength + SOAD_HEADER_SIZE);
                    SoAd_SoTxBufMan[txBufferId].upCopyLength = SOAD_HEADER_SIZE;
                    SoAd_SoConManager[SoConId].TpPduTransmitting = TRUE;
                    ret = E_OK;
                }
            }
        }
        else
        #endif/*STD_ON == SOAD_SUPPORT_HEADERID*/
        {
            SoAd_SoTxBufMan[txBufferId].txPduTotalLength = SoAdSrcPduInfoPtr->SduLength;
            SoAd_SoTxBufMan[txBufferId].upCopyLength = 0u;
            SoAd_SoConManager[SoConId].TpPduTransmitting = TRUE;
            ret = E_OK;
        }
    }
    return ret;
}
#endif/*0u < SOAD_TXBUFFER_NUM*/

/*Called by SoAd_TcpIpEvent.
 *Close the Udp SoCon Group*/
#if (STD_ON == SOAD_SUPPORT_UDP)
static FUNC(void, SOAD_CODE)
SoAd_CloseUdpSoConGroup(
        VAR(TcpIp_EventType, AUTOMATIC) Event,
        VAR(SoAd_uintx, AUTOMATIC) SoConGroupId)
{
    SoAd_SoConIdType soConCnt,soConId;
    /*SWS_SoAd_00643:TCPIP_UDP_CLOSED for a udp socket connection group*/
    if(TCPIP_UDP_CLOSED == Event)
    {
        for(soConCnt = 0u; soConCnt < SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConNum;soConCnt++)
        {
            soConId = SOAD_SOCONGROUP_CFG(SoConGroupId).SoAd_SoConId[soConCnt];
            SoAd_SoConManager[soConId].TcpIpSocketValid = FALSE;
            if(SOAD_SOCON_ONLINE == SoAd_SoConManager[soConId].SoAd_SoConMode)
            {
                SoAd_SoConCloseHandle(soConId,SoConGroupId,FALSE);
                /*Notification UP Module SoCon changed*/
                SoAd_SoConManager[soConId].SoAd_SoConMode = SOAD_SOCON_RECONNECT;
                SoAd_SoConModeChgNotificationAndTpStartOfReception(soConId, SoConGroupId);
            }
        }
        SoAd_SoConGroupManager[SoConGroupId].TcpIpSocketValid = FALSE;
        SoAd_SoConGroupManager[SoConGroupId].SoConSocketValidNum = 0u;
    }
    #if (STD_ON == SOAD_DEV_ERROR_DETECT)
    /*SWS_SoAd_00278*/
    else
    {
        SOAD_DET_REPORT(SOAD_SID_TCPIPEVENT,  SOAD_E_INV_ARG);
    }
    #endif /* STD_ON == SOAD_DEV_ERROR_DETECT */
    return;
}
#endif/*(STD_ON == SOAD_SUPPORT_UDP)*/

/*Called by SoAd_TxConfirmation.
 *Tcp TxConfirmation handle*/
#if (STD_ON == SOAD_SUPPORT_TCP)
static FUNC(void, SOAD_CODE)
SoAd_TxConfirmationHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) soConId,
        VAR(uint16, AUTOMATIC) Length,
        VAR(SoAd_uintx, AUTOMATIC) pduRouteDestId)
{
    PduIdType pduRouteId;
    if(SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength <= Length)
    {
        SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength = 0u;
        pduRouteId = SOAD_PDUROTDEST_CFG(pduRouteDestId).SoAd_PduRouteIndex;
        #if (0u < SOAD_TXBUFFER_NUM)
        if(SOAD_UPPER_LAYER_TP == SOAD_PDUROUT_CFG(pduRouteId).SoAd_TxUpperLayerType)
        {
            if(FALSE == SoAd_SoConManager[soConId].TpPduTransmitting)
            {
                /*here is the really TxConfirmation for SoAdSocketTcpImmediateTpTxConfirmation*/
                SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId,TCPIP_IPPROTO_TCP);
            }
            else
            {
                /*Tp Pdu TxConfirmation handle*/
                SoAd_uintx txBufferId;
                txBufferId = *SOAD_SOCKET_TXBUF_INDEX_CFG(soConId);
                if(SoAd_SoTxBufMan[txBufferId].upCopyLength == SoAd_SoTxBufMan[txBufferId].txPduTotalLength)
                {
                    SoAd_UpLayerModuleType upModule;
                    #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
                    if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr)
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).Tp_TxConfirmation_FuncPtr(
                                 SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId,
                                 E_OK);
                    }
                    #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
                    SoAd_SoTxBufMan[txBufferId].txPduTotalLength = 0u;
                    SoAd_SoTxBufMan[txBufferId].upCopyLength = 0u;
                    SoAd_SoConManager[soConId].TpPduTransmitting = FALSE;
                    SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId,TCPIP_IPPROTO_TCP);
                }
            }
        }
        /*If Pdu TxConfirmation handle*/
        else
        #endif/*0u < SOAD_TXBUFFER_NUM*/
        {
            if(SoAd_PduRoutManager[pduRouteId].TxPendingNum > 0u)
            {
                SoAd_PduRoutManager[pduRouteId].TxPendingNum -= 1u;
                if(0u == SoAd_PduRoutManager[pduRouteId].TxPendingNum)
                {
                    SoAd_UpLayerModuleType upModule;
                    #if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
                    upModule = SOAD_PDUROUT_CFG(pduRouteId).SoAd_RefModule;
                    if(NULL_PTR != SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr)
                    {
                        SOAD_UPPERLAY_PCCFG(upModule).If_TxConfirmation_FuncPtr(
                                 SOAD_PDUROUT_CFG(pduRouteId).SoAd_UpPduId);
                    }
                    #endif/* (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u) */
                    SoAd_PduRouteManagerInit(pduRouteId);
                    SoAd_SocnTransmitAutomaticSoConSetupHandle(soConId,TCPIP_IPPROTO_TCP);
                }
            }
        }
    }
    else
    {
        SoAd_PduRouteDestManager[pduRouteDestId].TxPendingLength -= Length;
    }
    return;
}
#endif


#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

