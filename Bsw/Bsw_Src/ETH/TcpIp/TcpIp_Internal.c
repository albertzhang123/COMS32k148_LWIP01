/***********************************************************************************************************************
**                                                                                                                    **
** Copyright (C) iSOFT   (2020)                                                                                       **
**                                                                                                                    **
** All rights reserved.                                                                                               **
**                                                                                                                    **
** This document contains proprietary information belonging to iSOFT                                                  **
** Passing on and copying of this document, and communication                                                         **
** of its contents is not permitted without prior written authorization.                                              **
**                                                                                                                    **
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : TcpIp_Internal.c                                                                                    **
**                                                                                                                    **
**  Created on  : 02/03/21                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : internal function for TcpIp                                                                         **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.2    [20210817]  [darren.zhang]  first create
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
***********************************************************************************************************************/
#include "TcpIp_Cfg.h"
#include "TcpIp_Types.h"
#include "TcpIp_Lcfg.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp.h"
#include "EthIf.h"
#include "SchM_TcpIp.h"
#include "TcpIp_Internal.h"
#if (STD_ON == TCPIP_ETHSM_SUPPORT)
#include "EthSM_TcpIp.h"
#endif/* (STD_ON == TCPIP_ETHSM_SUPPORT) */

/***********************************************************************************************************************
 *  VERSION
***********************************************************************************************************************/
#define TCPIP_INTERNAL_C_AR_MAJOR_VERSION                            19U
#define TCPIP_INTERNAL_C_AR_MINOR_VERSION                            11U
#define TCPIP_INTERNAL_C_AR_PATCH_VERSION                            0U
#define TCPIP_INTERNAL_C_SW_MAJOR_VERSION                            2U
#define TCPIP_INTERNAL_C_SW_MINOR_VERSION                            0U
#define TCPIP_INTERNAL_C_SW_PATCH_VERSION                            2U

/***********************************************************************************************************************
 *  VERSION CHECK
***********************************************************************************************************************/
/* check the version of TcpIp Internal header files */
#if (  (TCPIP_INTERNAL_C_AR_MAJOR_VERSION != TCPIP_INTERNAL_H_AR_MAJOR_VERSION) \
    || (TCPIP_INTERNAL_C_AR_MINOR_VERSION != TCPIP_INTERNAL_H_AR_MINOR_VERSION) \
    || (TCPIP_INTERNAL_C_AR_PATCH_VERSION != TCPIP_INTERNAL_H_AR_PATCH_VERSION) \
    || (TCPIP_INTERNAL_C_SW_MAJOR_VERSION != TCPIP_INTERNAL_H_SW_MAJOR_VERSION) \
    || (TCPIP_INTERNAL_C_SW_MINOR_VERSION != TCPIP_INTERNAL_H_SW_MINOR_VERSION) \
    )
#error "version numbers of TcpIp_Internal.h and TcpIp_Internal.c are inconsistent"
#endif

/* check the version of the configuration header file */
#if (  (TCPIP_INTERNAL_C_AR_MAJOR_VERSION != TCPIP_CFG_H_AR_MAJOR_VERSION) \
    || (TCPIP_INTERNAL_C_AR_MINOR_VERSION != TCPIP_CFG_H_AR_MINOR_VERSION) \
    || (TCPIP_INTERNAL_C_AR_PATCH_VERSION != TCPIP_CFG_H_AR_PATCH_VERSION) \
    || (TCPIP_INTERNAL_C_SW_MAJOR_VERSION != TCPIP_CFG_H_SW_MAJOR_VERSION) \
    || (TCPIP_INTERNAL_C_SW_MINOR_VERSION != TCPIP_CFG_H_SW_MINOR_VERSION) \
    )
#error "version numbers of TcpIp_Cfg.h and TcpIp_Internal.c are inconsistent"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
***********************************************************************************************************************/

#define TCPIP_SOCKET_INVALIDVLU                     TCPIP_SOCKET_NUM

/* inner function condtion check level,per level consume memory or cycles is different */
#if !defined(TCPIP_INNER_CHECK_TAG)
#define TCPIP_INNER_CHECK
#define TCPIP_INNER_CHECK_OUTPUT(...)               TCPIP_CHECK_OUTPUT(__VA_ARGS__)
#endif /* defined(TCPIP_INNER_CHECK_TAG) */

#if (TCPIP_CONTROLLER_NUM > 0u)
#define TCPIP_REG_NETIF_ETHIF_CTRLINDEX(netif,index)    \
    do{\
        (netif)->name[0] = (char)'I';\
        (netif)->name[1] = (char)(index);\
    }while(0)\

#define TCPIP_CHECK_NETIF_ETHIF_CTRLINDEX(netif)    \
    (((netif)->name[0] == (char)'I')&&((netif)->name[1] < (char)(TCPIP_CONTROLLER_NUM)))
#define TCPIP_GET_NETIF_ETHIF_CTRLINDEX(netif)      ((uint8)((netif)->name[1]))

#define TCPIP_GET_NETIF_CTRLINDEX(netif)            ((uint8)((netif)->num))

#define TCPIP_CTRL_STATE_MASK(state)                (1u << (state))

#define TCPIP_CTRL_SET_STATE_PENDING(ctrl,state)    ((ctrl)->stateFlag |= (uint8)TCPIP_CTRL_STATE_MASK(state))
#define TCPIP_CTRL_CLR_STATE_PENDING(ctrl,state)    ((ctrl)->stateFlag &= ~(uint8)TCPIP_CTRL_STATE_MASK(state))
#define TCPIP_CTRL_IS_STATE_PENDING(ctrl,state)     (((ctrl)->stateFlag & (uint8)TCPIP_CTRL_STATE_MASK(state)) != 0u)

#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)                                                                          /* PRQA S 3332 */
#define TCPIP_CTRL_MASK_AUTOIP_PENDING_MASK         ((uint8)0x01)
#define TCPIP_CTRL_SET_AUTOIP_PENDING(ctrl)         ((ctrl)->maskFlg |= TCPIP_CTRL_MASK_AUTOIP_PENDING_MASK)
#define TCPIP_CTRL_CLR_AUTOIP_PENDING(ctrl)         ((ctrl)->maskFlg &=~ TCPIP_CTRL_MASK_AUTOIP_PENDING_MASK)
#define TCPIP_CTRL_IS_AUTOIP_PENDING(ctrl)         (((ctrl)->maskFlg & TCPIP_CTRL_MASK_AUTOIP_PENDING_MASK) != 0u)
#endif/* (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER) */
#endif/* (TCPIP_CONTROLLER_NUM > 0u) */

#define TCPIP_NETIF_NSC_IPCHANGED                   (netif_nsc_reason_t)(\
        (netif_nsc_reason_t)LWIP_NSC_IPV4_ADDRESS_CHANGED | \
        (netif_nsc_reason_t)LWIP_NSC_IPV4_GATEWAY_CHANGED |\
        (netif_nsc_reason_t) LWIP_NSC_IPV4_NETMASK_CHANGED |\
        (netif_nsc_reason_t) LWIP_NSC_IPV4_SETTINGS_CHANGED |\
        (netif_nsc_reason_t) LWIP_NSC_IPV6_SET |\
        (netif_nsc_reason_t) LWIP_NSC_IPV6_ADDR_STATE_CHANGED)
#if (TCPIP_LOCAL_ADR_NUM > 0)
#define TCPIP_LOCALADR_IS_REQUESTIP(lcd)            (((lcd)->flag & LOCALADR_FLAG_REQUEST_IP) != 0u)
#define TCPIP_LOCALADR_SET_REQUESTIP(lcd)           (((lcd)->flag = (lcd)->flag |LOCALADR_FLAG_REQUEST_IP))
#define TCPIP_LOCALADR_CLR_REQUESTIP(lcd)           (((lcd)->flag = \
                                                      (lcd)->flag & (uint32)(~(LOCALADR_FLAG_REQUEST_IP))))

#define TCPIP_LOCALADR_IS_IPVALID(lcd)              (((lcd)->flag & LOCALADR_FLAG_IP_VAID) != 0u)
#define TCPIP_LOCALADR_SET_IPVALID(lcd)             (((lcd)->flag = (lcd)->flag | LOCALADR_FLAG_IP_VAID))
#define TCPIP_LOCALADR_CLR_IPVALID(lcd)             (((lcd)->flag = (lcd)->flag & (uint32)(~(LOCALADR_FLAG_IP_VAID))))

#endif /* TCPIP_LOCAL_ADR_NUM > 0 */

#define TCPIP_CALCTMR(x)                            (((uint32)(x))/(uint32)TCPIP_MAINFUNCTION_PERIOD)

#if (STD_ON == TCPIP_TCP_ENABLED)
/* call lwip api tcp_write fill last parameter apiflags value */
#if TCPIP_SUPPORT_WRITE_MORE
/* apiflags is set TCP_WRITE_FLAG_MORE,tcp header not set PUSH flag */
#define TCPIP_WRITE_FILL_FLAG                       (uint8)(0x3u)
#else
#define TCPIP_WRITE_FILL_FLAG                       (uint8)(0x1u)
#endif/* TCPIP_SUPPORT_WRITE_MORE */
#define TCP_FLAG_FORCERETRIEVE                      (uint8)(0x01u)
#define TCP_FLAG_SERVERSOCKET                       (uint8)(0x02u)


#define TCPIP_TCP_IS_FORCERETRIEVE(sockPtr)         ((((sockPtr)->socketFlag) & TCP_FLAG_FORCERETRIEVE) != 0u)
#define TCPIP_TCP_SET_ZFORCERETRIEVE(sockPtr,sw)    do{\
        if ((boolean)TRUE == (sw))\
        {\
            (sockPtr)->socketFlag |= TCP_FLAG_FORCERETRIEVE;\
        }\
        else\
        {\
            (sockPtr)->socketFlag &= (uint8)(~TCP_FLAG_FORCERETRIEVE);\
        }\
    }while(0)
/* mark this socket is listen socket */
#define TCPIP_TCP_IS_SERVERSOCKET(sockPtr)          ((((sockPtr)->socketFlag) & TCP_FLAG_SERVERSOCKET) != 0u)
#define TCPIP_TCP_SET_SERVERSOCKET(sockPtr)         (((sockPtr)->socketFlag) |= TCP_FLAG_SERVERSOCKET)
#define TCPIP_TCP_CLR_SERVERSOCKET(sockPtr)         (((sockPtr)->socketFlag) &=(uint8)(~ TCP_FLAG_SERVERSOCKET))



#endif/* STD_ON == TCPIP_TCP_ENABLED */

#if (TCPIP_SOCKET_NUM > 0u)
/* by socket management data point calculate socket index */
#define TCPIP_SOCKID_CAL_ARYADR(ptr)                ((TcpIp_SocketIdType)\
                                                     (((uintx)(ptr) - (uintx)(TcpIp_SocketTable)) / \
                                                      sizeof(TcpIp_SocketTable[0])))
#endif /* (TCPIP_SOCKET_NUM > 0u) */

#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
/* multiple ip handle */
#define TCPIP_CALC_U32BITMASK_BYTES_POS(x)          ((x)/ 32u)
#define TCPIP_CALC_U32BITMASK_BIT_POS(x)            ((x) & 0x1f)
#define TCPIP_CALC_U32BITMASK_BIT_MASK(x)           ((uint32)0x1u<<((x) & 0x1f))

#define TCPIP_SET_CTRL_LOCAL_MASK(ctrlPtr, ctrlLocalIndex)      (\
        (ctrlPtr)->localUsedMask[TCPIP_CALC_U32BITMASK_BYTES_POS(ctrlLocalIndex)] |= \
                TCPIP_CALC_U32BITMASK_BIT_MASK(ctrlLocalIndex))

#define TCPIP_CLR_CTRL_LOCAL_MASK(ctrlPtr, ctrlLocalIndex)      (\
        (ctrlPtr)->localUsedMask[TCPIP_CALC_U32BITMASK_BYTES_POS(ctrlLocalIndex)] &= \
                (uint32)(~TCPIP_CALC_U32BITMASK_BIT_MASK(ctrlLocalIndex)))

#define TCPIP_IS_CTRL_LOCAL_MASK(ctrlPtr, ctrlLocalIndex)      (\
        ((ctrlPtr)->localUsedMask[TCPIP_CALC_U32BITMASK_BYTES_POS(ctrlLocalIndex)] & \
         TCPIP_CALC_U32BITMASK_BIT_MASK(ctrlLocalIndex)) != 0u)

/* return true is this control link all local addr released */
#if (1u < TCPIP_CTRL_LOCAL_NUMBER)
#define TCPIP_CTRL_ALL_LOCAL_IS_RELEASE(ctrlPtr)        (TcpIp_CtrlLinkAllLocalIsRelease(ctrlPtr) == TRUE)
#else
#define TCPIP_CTRL_ALL_LOCAL_IS_RELEASE(ctrlPtr)        ((ctrlPtr)->localUsedMask[0] == 0u)
#endif/* (1u < TCPIP_CTRL_LOCAL_NUMBER) */

#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */

/***********************************************************************************************************************
 *  LOCAL DATA
***********************************************************************************************************************/
#if (TCPIP_CONTROLLER_NUM > 0)

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_ControllerStatus, TCPIP_VAR) TcpIp_ControllerVar[TCPIP_CONTROLLER_NUM];
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"
TCPIP_LOCAL P2CONST(TcpIp_CtrlType, AUTOMATIC, TCPIP_VAR) ControlCfgPtr;
#define TCPIP_STOP_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"

#endif /* (TCPIP_CONTROLLER_NUM > 0) */

#if (TCPIP_SOCKET_NUM > 0)

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_SocketHandleType, TCPIP_VAR) TcpIp_SocketTable[TCPIP_SOCKET_NUM];
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#endif /* (TCPIP_SOCKET_NUM > 0) */

#if (TCPIP_LOCAL_ADR_NUM > 0)

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_LocalAdrHandleType, TCPIP_VAR) TcpIp_LocalAdrTable[TCPIP_LOCAL_ADR_NUM];
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_ICMP_ENABLED)
#define TCPIP_START_SEC_VAR_NO_INIT_PTR
#include "TcpIp_MemMap.h"
TCPIP_LOCAL P2VAR(struct raw_pcb, AUTOMATIC, TCPIP_APPL_VAR) TcpIp_IcmpPcbPtr;                                          /* PRQA S 3218,3432 */
#define TCPIP_STOP_SEC_VAR_NO_INIT_PTR
#include "TcpIp_MemMap.h"
#endif/* (STD_ON == TCPIP_ICMP_ENABLED) */

#define TCPIP_START_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"
TCPIP_LOCAL P2CONST(TcpIp_LocalAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalAdrCfgPtr;
#define TCPIP_STOP_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"

#endif /* TCPIP_LOCAL_ADR_NUM > 0 */

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIP_PeriodTmrType, TCPIP_VAR) TcpIp_PeriodTmr;
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#if (TCPIP_TCP_SOCKET_MAX > 0)
#define TCPIP_START_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_SocketIdType, TCPIP_VAR) Tcp_SocketUsedNum;
#define TCPIP_STOP_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */

#if (TCPIP_UDP_SOCKET_MAX > 0)
#define TCPIP_START_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_SocketIdType, TCPIP_VAR) Udp_SocketUsedNum;
#define TCPIP_STOP_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */

#if (TCPIP_SOCKET_NUM > 0)
#define TCPIP_START_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_SocketIdType, TCPIP_VAR) TcpIp_SocketLastId;
#define TCPIP_STOP_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"
#endif /* (TCPIP_SOCKET_NUM > 0) */

/* tcp pcb extend register callback */
#if (STD_ON == TCPIP_TCP_ENABLED)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TcpExtargCallbackPcbDestroyed(
    VAR(u8_t, AUTOMATIC) id,
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) data);                                                                       /* PRQA S 3432 */
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL const struct tcp_ext_arg_callbacks TcpIp_tcpExtArgCallbacksTable[1] =
{
    {
        TcpIp_TcpExtargCallbackPcbDestroyed,
        NULL_PTR,
    }
};
#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
#endif /* STD_ON == TCPIP_TCP_ENABLED */

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
NETIF_DECLARE_EXT_CALLBACK(netifExtCallBackStu);                                                                        /* PRQA S 0605,3218 */ /* MISRA Rule 1.1 8.9 */
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_TCP_SYNISN_RECALC)                                                                                 /* PRQA S 3332 */ /* MISRA Rule 20.9 */
#define TCPIP_START_SEC_VAR_NO_INIT_32
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(uint32, TCPIP_VAR) TcpIp_TcpIsnCount;
#define TCPIP_STOP_SEC_VAR_NO_INIT_32
#include "TcpIp_MemMap.h"
#endif/* (STD_ON == TCPIP_TCP_SYNISN_RECALC) */
/***********************************************************************************************************************
 *  LOCAL FUNCTIONS PROTOTYPES
***********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultCastIpAssigmentHandle(
    VAR(uintx, AUTOMATIC)  localAdrIndex,
    VAR(uint8, AUTOMATIC)  ethIfIndex,
    VAR(boolean, AUTOMATIC) isAdd);

TCPIP_LOCAL FUNC(uintx, TCPIP_CODE) TcpIp_CloseAllSocket(VAR(uint8, AUTOMATIC) ctrlIdx);

#if (STD_ON == TCPIP_TCP_ENABLED)

TCPIP_LOCAL FUNC_P2VAR(TcpIp_SocketHandleType,AUTOMATIC,TCPIP_CODE) TcpIp_MallocTcpSocketIdByPcb(
    P2VAR(TcpIp_SocketHandleType,AUTOMATIC,TCPIP_APPL_DATA) socketMngPtr,
    P2CONST(struct tcp_pcb,AUTOMATIC,TCPIP_APPL_DATA) pcbPtr);

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_FreeTcpSocketId(
    VAR(TcpIp_SocketIdType, TCPIP_VAR) socketId);

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_CopyTcpMainHandle(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_CONST) socketMngPtr);                                           /* PRQA S 3432 */

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TcpSocketFreeAndEventNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    VAR(TcpIp_EventType, AUTOMATIC) event);
#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if (TCPIP_CONTROLLER_NUM > 0)

TCPIP_LOCAL FUNC(void, TCPIP_CODE) netIfExtStuChangeCallBack(
    P2VAR(struct netif, AUTOMATIC, TCPIP_APPL_VAR) netif,                                                               /* PRQA S 3432 */
    VAR(netif_nsc_reason_t, AUTOMATIC) reason,
    P2CONST(netif_ext_callback_args_t, AUTOMATIC, TCPIP_APPL_VAR) args);

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) ethernetif_init(
    P2VAR(struct netif, AUTOMATIC, TCPIP_APPL_VAR) netif);                                                              /* PRQA S 3432 */

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_ControlStateNotifyEthSm(
    VAR(uintx, AUTOMATIC) ctrlIdx,
    VAR(TcpIp_StateType, AUTOMATIC) notifyState);

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_UnasignedCtrlIpAssigemt(VAR(uintx, AUTOMATIC) ctrlIndex);
#endif /* (TCPIP_CONTROLLER_NUM > 0) */

#if (STD_ON == TCPIP_TCP_ENABLED)

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TcpErrCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err);

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpSentCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */
    VAR(uint16, AUTOMATIC) len);

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpConnnectCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err);

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpAcceptCallcback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) newpcb,                                                            /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err);

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpRecvCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */
    P2VAR(struct pbuf, AUTOMATIC, TCPIP_APPL_VAR) p,                                                                    /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err);

#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if (STD_ON == TCPIP_UDP_ENABLED)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_UdpRecvCallcback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct udp_pcb, AUTOMATIC, TCPIP_APPL_VAR) pcb,                                                               /* PRQA S 3432 */
    P2VAR(struct pbuf, AUTOMATIC, TCPIP_APPL_VAR) p,                                                                    /* PRQA S 3432 */
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_APPL_VAR) addr,
    VAR(uint16, AUTOMATIC) port);

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_FreeUdpSocketId(VAR(TcpIp_SocketIdType, TCPIP_VAR) socketId);

TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_UdpSocketFreeAndEventNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    VAR(TcpIp_EventType, AUTOMATIC) event);
#endif/* (STD_ON == TCPIP_UDP_ENABLED) */

#if (TCPIP_LOCAL_ADR_NUM > 0)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_CtrlLinkLocalAddrChagneHandle(
    VAR(uintx, AUTOMATIC) ctrlIndex,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) ipAdrState,
    VAR(boolean, AUTOMATIC) forceNotifyFlg);
#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
***********************************************************************************************************************/

/**
 * @ingroup tcpip_internal
 * multicast ip assignment shall call EthIf_UpdatePhysAddrFilter
 * @param localAdrIndex inner local address index
 * @param ethIfIndex EthIf controller index
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultCastIpAssigmentHandle(
    VAR(uintx, AUTOMATIC)  localAdrIndex,
    VAR(uint8, AUTOMATIC)  ethIfIndex,
    VAR(boolean, AUTOMATIC) isAdd)
{
    TcpIp_LocalAdrHandleType* localAdtPtr = &TcpIp_LocalAdrTable[localAdrIndex];
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
    uint8 macAddr[6];
    ip4_addr_t ipaddr;
    ipaddr.addr = localAdtPtr->ipAdrSrc.addr;
    macAddr[0] = LL_IP4_MULTICAST_ADDR_0;
    macAddr[1] = LL_IP4_MULTICAST_ADDR_1;
    macAddr[2] = LL_IP4_MULTICAST_ADDR_2;
    macAddr[3] = (uint8)(ip4_addr2(&ipaddr)) & 0x7fu;
    macAddr[4] = ip4_addr3(&ipaddr);
    macAddr[5] = ip4_addr4(&ipaddr);
    (void)EthIf_UpdatePhysAddrFilter(ethIfIndex, macAddr,
                                     ((isAdd != (boolean)TRUE) ?
                                      (Eth_FilterActionType)ETH_REMOVE_FROM_FILTER :
                                      (Eth_FilterActionType)ETH_ADD_TO_FILTER));
#endif/* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) */

    /* entry this line,sure local is request */
    if (TRUE == isAdd)
    {
        const TcpIp_LocalAddrType* localCfgPtr = &LocalAdrCfgPtr[localAdrIndex];
        const struct netif* netifPtr = &TcpIp_ControllerVar[localCfgPtr->ctrlIndex].netifVar;

        /* multcast need to make sure that the netif has a valid ip */
        if (!ip4_addr_isany_val(*netif_ip4_addr(netifPtr)))
        {
            TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
            TCPIP_LOCALADR_SET_IPVALID(localAdtPtr);
            /* The current design of multicast IP allows sockets to be associated, With the upper module SOAD */
            TcpIp_NotfiyUpperLayerIpAdrChange((TcpIp_LocalAddrIdType)localAdrIndex,
                                              TCPIP_IPADDR_STATE_ASSIGNED);
        }
    }
    else
    {
        /* clear flag */
        TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
        TCPIP_LOCALADR_CLR_IPVALID(localAdtPtr);
        /* The current design of multicast IP allows sockets to be unassociated, With the upper module SOAD */
        TcpIp_NotfiyUpperLayerIpAdrChange((TcpIp_LocalAddrIdType)localAdrIndex,
                                          TCPIP_IPADDR_STATE_UNASSIGNED);
    }
}

/**
 * @ingroup tcpip_internal
 * when tcpip management controller entry off_line shall close base on this controller all sockets
 * @param ctrlIdx inner controller index
 * @return pending handle tcp socket close number
 */
TCPIP_LOCAL FUNC(uintx, TCPIP_CODE) TcpIp_CloseAllSocket(VAR(uint8, AUTOMATIC) ctrlIdx)
{
#if TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET
    uintx pendCnt = 0;

    for (uintx index = 0; index < TCPIP_SOCKET_NUM; index++)
    {
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[index];

        if ((NULL_PTR != socketMngPtr->pcbPtr)
            && (ctrlIdx == socketMngPtr->ctrlIndex))
        {
#if (STD_ON == TCPIP_TCP_ENABLED)

            if (TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
            {
                /* first shall notify event */
                TCPIP_TCP_SET_PENDINGEVENT(socketMngPtr);
                /* next close socket */
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                err_t err = tcp_close((struct tcp_pcb*)socketMngPtr->pcbPtr);

                /* A close failure occurs while a close task is already executing,this not consider half close */
                if (err != (err_t)ERR_OK)
                {
                    tcp_abort((struct tcp_pcb*)socketMngPtr->pcbPtr);
                }

                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                /* tcp socket close four times to shake hands,
                so will pending control state,not direct switch to shutdown */
                pendCnt++;
            }

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */
#if (STD_ON == TCPIP_UDP_ENABLED)

            if (TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
            {
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                udp_disconnect((struct udp_pcb*)socketMngPtr->pcbPtr);
                udp_remove((struct udp_pcb*)socketMngPtr->pcbPtr);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                /* first shall notify event */
                TcpIp_UdpSocketFreeAndEventNotifyUpLayer(socketMngPtr, TCPIP_UDP_CLOSED);
            }

#endif/* (STD_ON == TCPIP_UDP_ENABLED) */
        }
    }

    return pendCnt;
#else /* TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET == 0*/
    return TcpIp_ControllerVar[ctrlIdx].socketNum;
#endif /* TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET */
}

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @ingroup tcpip_internal
 * malloc tcp socket,call by TcpIp_GetSocket
 * @param socketMngPtr socket inner management data variable point
 * @param pcbPtr the new connection pcb
 * @return TcpIp_SocketHandleType *,is not null,meas malloc ok,another is failed
 */

// *INDENT-OFF*
TCPIP_LOCAL FUNC_P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_CODE) TcpIp_MallocTcpSocketIdByPcb(                     /* PRQA S 3432 *//* MISRA Rule 20.7 */
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_DATA) socketMngPtr,                                             /* PRQA S 3432 */
    P2CONST(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_DATA) pcbPtr)
// *INDENT-ON*
{
    TcpIp_SocketHandleType* newSocketMngPtr = NULL_PTR;

    if ((Tcp_SocketUsedNum < TCPIP_TCP_SOCKET_MAX) && (TcpIp_SocketLastId < TCPIP_SOCKET_NUM))
    {
        newSocketMngPtr = &TcpIp_SocketTable[TcpIp_SocketLastId];
        newSocketMngPtr->pcbPtr = (void*)pcbPtr;                                                                        /* PRQA S 0311 */ /* MISRA Rule 11.8 */
        struct tcp_pcb* tpcb = (struct tcp_pcb*)pcbPtr;                                                                 /* PRQA S 0311 */ /* MISRA Rule 11.8 */
        socketMngPtr->socketFlag = 0u;
        /* reg extend callback function */
        tcp_ext_arg_set_callbacks(tpcb, 0u, TcpIp_tcpExtArgCallbacksTable);
        tcp_ext_arg_set(tpcb, 0u, newSocketMngPtr);
        tcp_arg(tpcb, newSocketMngPtr);
        /* shall reg recv function */
        tcp_recv(tpcb, TcpIp_TcpRecvCallback);
        /* reg connect error callback function */
        tcp_err(tpcb, TcpIp_TcpErrCallback);
        /* reg send call back function */
        tcp_sent(tpcb, TcpIp_TcpSentCallback);
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
        /* state handle */
        Tcp_SocketUsedNum++;
        /* output parameter setting */
        newSocketMngPtr->protocol = TCPIP_IPPROTO_TCP;
        /* link list handle */
        TcpIp_SocketLastId = newSocketMngPtr->nextSocketId;
        newSocketMngPtr->nextSocketId = TCPIP_SOCKET_INVALIDVLU;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        newSocketMngPtr->listenSocketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                        /* PRQA S 0326,0306 */
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
        /* copy data from listen socket */
        newSocketMngPtr->ownerCfgPtr = socketMngPtr->ownerCfgPtr;
        newSocketMngPtr->ctrlIndex = socketMngPtr->ctrlIndex;
        TcpIp_ControllerVar[socketMngPtr->ctrlIndex].socketNum++;
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
    }

    return newSocketMngPtr;
}

/**
 * @ingroup tcpip_internal
 * free tcp socket,shall release inner socket management data
 * @param socketId inner socket index,from TcpIp_GetSocket
 * @return E_OK ok,another is failed
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_FreeTcpSocketId(VAR(TcpIp_SocketIdType, TCPIP_VAR) socketId)
{
#if defined(TCPIP_INNER_CHECK)

    if (socketId < TCPIP_SOCKET_NUM)
#endif /* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
        socketMngPtr->pcbPtr = NULL_PTR;
        socketMngPtr->ownerCfgPtr = NULL_PTR;
        socketMngPtr->socketFlag = 0u;
        socketMngPtr->txRemainLen = 0u;

        if (NULL_PTR != socketMngPtr->txBuf)
        {
            /* have not transmit data in local buffer */
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            (void)pbuf_free(socketMngPtr->txBuf);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        }

        socketMngPtr->txBuf = NULL_PTR;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
        socketMngPtr->listenSocketId = TCPIP_SOCKET_INVALIDVLU;
        socketMngPtr->tlsPtr = NULL_PTR;
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
#if (TCPIP_CONTROLLER_NUM > 0)
        TcpIp_ControllerStatus* ctrlMngPtr = &TcpIp_ControllerVar[socketMngPtr->ctrlIndex];

        if (0u < ctrlMngPtr->socketNum)
        {
            ctrlMngPtr->socketNum--;
        }

        /* [SWS_TCPIP_00087] */
        if ((TCPIP_CTRL_IS_STATE_PENDING(ctrlMngPtr, TCPIP_STATE_SHUTDOWN)) && (0u == ctrlMngPtr->socketNum))
        {
            TCPIP_CTRL_CLR_STATE_PENDING(ctrlMngPtr, TCPIP_STATE_SHUTDOWN);
            ctrlMngPtr->ctrlStatus = TCPIP_STATE_OFFLINE;
            /* call lwip close netif */
            netif_set_down(&ctrlMngPtr->netifVar);
            netif_set_link_down(&ctrlMngPtr->netifVar);
            /* all socket this control is free,shall be switch TCPIP_STATE_OFFLINE */
            TcpIp_ControlStateNotifyEthSm(socketMngPtr->ctrlIndex, TCPIP_STATE_OFFLINE);
        }

#endif /* (TCPIP_CONTROLLER_NUM > 0) */
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
        /* link list handle */
        socketMngPtr->nextSocketId = TcpIp_SocketLastId;
        TcpIp_SocketLastId = socketId;
        Tcp_SocketUsedNum--;
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("free socket is error! socketId=%d\n", socketId);
    }

#endif /* defined(TCPIP_INNER_CHECK) */
}

/**
 * @ingroup tcpip_internal
 * tcp socket data copy mainhandle,when TcpIp_TcpTransmit used null is dataptr, shall handle one or multiple
 * <Up>_CopyTxData() calls
 * @param socketMngPtr inner socket index,from TcpIp_GetSocket
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_CopyTcpMainHandle(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                                              /* PRQA S 3432 */
{
    struct tcp_pcb* tcpPcbPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
    uint16 copyLen = (socketMngPtr->txRemainLen > (uint32)(tcpPcbPtr->snd_buf)) ?
                     tcpPcbPtr->snd_buf : (uint16)(socketMngPtr->txRemainLen);
    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
    struct pbuf* tcpTxPbuf = pbuf_alloc(PBUF_RAW, copyLen, PBUF_RAM);
    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#if defined(TCPIP_INNER_CHECK)

    if (NULL_PTR != tcpTxPbuf)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        if (tcpTxPbuf->tot_len != tcpTxPbuf->len)
        {
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            tcpTxPbuf = pbuf_coalesce(tcpTxPbuf, PBUF_RAW);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        }

        TcpIp_Up_SocketOwnerCopyTxDataType copyFuncPtr = socketMngPtr->ownerCfgPtr->Up_CopyTxDataFuncPtr;
        BufReq_ReturnType bufRet = copyFuncPtr(TCPIP_SOCKID_CAL_ARYADR(socketMngPtr), (uint8*)tcpTxPbuf->payload,       /* PRQA S 0326,0306 */
                                               tcpTxPbuf->len);
        err_t lw_ret = (err_t) ERR_OK;

        if (BUFREQ_OK == bufRet)
        {
            /* link all pbuf */
            if (NULL_PTR != socketMngPtr->txBuf)
            {
                pbuf_cat(socketMngPtr->txBuf, tcpTxPbuf);
                tcpTxPbuf = socketMngPtr->txBuf;
            }
            else
            {
                socketMngPtr->txBuf = tcpTxPbuf;
            }

            socketMngPtr->txRemainLen -= copyLen;

            if ((socketMngPtr->txRemainLen == 0u) || (!TCPIP_TCP_IS_FORCERETRIEVE(socketMngPtr)))
            {
                if (tcpTxPbuf->tot_len != tcpTxPbuf->len)
                {
                    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                    tcpTxPbuf = pbuf_coalesce(tcpTxPbuf, PBUF_RAW);
                    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                }

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

                if (NULL_PTR != socketMngPtr->tlsPtr)
                {
                    lw_ret = TcpIp_TlsWrite(socketMngPtr, tcpTxPbuf->payload, tcpTxPbuf->tot_len);
                }
                else
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
                {
                    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                    lw_ret = tcp_write(tcpPcbPtr, tcpTxPbuf->payload, tcpTxPbuf->tot_len, TCPIP_WRITE_FILL_FLAG);
#if TCPIP_FASTTX_TCP
                    lw_ret = tcp_output(tcpPcbPtr);
#endif/* TCPIP_FASTTX_TCP */
                    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                }

                /* call low layer transmit, shall realse all buffer ,whether it's successful or not */
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                (void)pbuf_free(tcpTxPbuf);
                socketMngPtr->txBuf = NULL_PTR;
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            }
        }
        else
        {
            /* upper layer copy tx data return BUFREQ_E_NOT_OK,shall release local buff */
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            (void)pbuf_free(tcpTxPbuf);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            lw_ret = (err_t)ERR_ARG;
#if defined(TCPIP_INNER_CHECK)
            TCPIP_INNER_CHECK_OUTPUT("CopyTxData return error=%d\n", bufRet);
#endif/* defined(TCPIP_INNER_CHECK) */
        }

        if ((err_t)ERR_OK != lw_ret)
        {
            /* free history buf,not copy next mainfunction */
            if (NULL_PTR != socketMngPtr->txBuf)
            {
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                (void)pbuf_free(socketMngPtr->txBuf);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            }

            socketMngPtr->txBuf = NULL_PTR;
            socketMngPtr->txRemainLen = 0u;
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("pbuf alloc error\n");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
}
/**
 * @ingroup tcpip_internal
 * tcp socket free and event notify socket owner
 * @param socketMngPtr local socket management data struct
 * @param event @ref TcpIp_EventType
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TcpSocketFreeAndEventNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    VAR(TcpIp_EventType, AUTOMATIC) event)
{
    TcpIp_Up_TcpIpEventType eventFuncPtr = socketMngPtr->ownerCfgPtr->Up_TcpIpEventFuncPtr;
    TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                                /* PRQA S 0326,0306 */
    uintx needNotifyFlg = (uintx)TCPIP_TCP_IS_PENDINGEVENT(socketMngPtr);                                               /* PRQA S 4304 */   /* MISRA Rule 10.5 */
    TcpIp_FreeTcpSocketId(socketId);

    if ((needNotifyFlg != 0u) && (NULL_PTR != eventFuncPtr))
    {
        /* report event to upper layer */
        eventFuncPtr(socketId, event);
    }
}
#endif/* (STD_ON == TCPIP_TCP_ENABLED) */

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @ingroup tcpip_internal
 * event notify socket owner
 * @param socketMngPtr local socket management data struct
 * @param event @ref TcpIp_EventType
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_UdpSocketFreeAndEventNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    VAR(TcpIp_EventType, AUTOMATIC) event)
{
    TcpIp_Up_TcpIpEventType eventFuncPtr = socketMngPtr->ownerCfgPtr->Up_TcpIpEventFuncPtr;
    TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                                /* PRQA S 0326,0306 */
    TcpIp_FreeUdpSocketId(socketId);

    if (NULL_PTR != eventFuncPtr)
    {
        /* report FIN to upper layer */
        eventFuncPtr(socketId, event);
    }
}
#endif/* (STD_ON == TCPIP_UDP_ENABLED) */

/***********************************************************************************************************************
 *  LWIP HOOKS FUNCTIONS
***********************************************************************************************************************/
#if TCPIP_LWIP_HOOKS
#if (STD_ON == TCPIP_TCP_SYNISN_RECALC)
FUNC(uint32, TCPIP_CODE) TcpIp_HookTcpIsn(const ip_addr_t* local_ip, u16_t local_port, const ip_addr_t* remote_ip,
                                          u16_t remote_port)
{
    TCPIP_UNUSED_ARG(local_ip);
    TCPIP_UNUSED_ARG(local_port);
    TCPIP_UNUSED_ARG(remote_ip);
    TCPIP_UNUSED_ARG(remote_port);
    return TcpIp_TcpIsnCount;
}
#endif/* (STD_ON == TCPIP_TCP_SYNISN_RECALC) */

#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
FUNC_P2VAR(struct netif, AUTOMATIC, TCPIP_CODE) TcpIp_HookIp4InputAccept(
    P2VAR(struct netif, AUTOMATIC, TCPIP_VAR) netifPtr, P2CONST(ip_addr_t, AUTOMATIC, TCPIP_VAR) destIpPtr)             /* PRQA S 3432 */
{
    struct netif* netifRtePtr = NULL_PTR;
    uintx ctrlIdx = TCPIP_GET_NETIF_CTRLINDEX(netifPtr);
#if defined(TCPIP_INNER_CHECK)

    if ((ctrlIdx < TCPIP_CONTROLLER_NUM) && (NULL_PTR != destIpPtr))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (TCPIP_LOCAL_ADR_NUM > 0)
        /* find this controller link all local address */
        uintx loop = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStartId;
        uintx loopEnd = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStopId;

        while (loop <= loopEnd)
        {
            TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[loop];
#if !TCPIP_ALLOW_RX_INVALID_LOCAL_IP

            /* compare ip address,compare before shall check local addr is valid */
            if ((TCPIP_LOCALADR_IS_IPVALID(lcaPtr)) && (ip_addr_cmp(&lcaPtr->ipAdrSrc, destIpPtr)))
#else/* TCPIP_ALLOW_RX_INVALID_LOCAL_IP */
            if ((!ip_addr_isany_val(lcaPtr->ipAdrSrc)) && (ip_addr_cmp(&lcaPtr->ipAdrSrc, destIpPtr)))
#endif/* !TCPIP_ALLOW_RX_INVALID_LOCAL_IP */
            {
                netifRtePtr = netifPtr;
                break;
            }

            loop++;
        }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input netif point parameter is not find ctrl index or dest ip is nullptr");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return netifRtePtr;
}

FUNC(uint8, TCPIP_CODE) TcpIp_HookEthArpInputForus(
    P2CONST(struct netif, AUTOMATIC, TCPIP_VAR) netifPtr,
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_VAR) destAddrPtr,
    P2VAR(ip_addr_t, AUTOMATIC, TCPIP_VAR) rltAddrPtr)                                                                  /* PRQA S 3432 */
{
    uintx ctrlIdx = TCPIP_GET_NETIF_CTRLINDEX(netifPtr);
    uint8 ret = FALSE;
#if defined(TCPIP_INNER_CHECK)

    if ((ctrlIdx < TCPIP_CONTROLLER_NUM) && (NULL_PTR != destAddrPtr))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        /* find this controller link all local address */
#if (TCPIP_LOCAL_ADR_NUM > 0)
        uintx loop = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStartId;
        uintx loopEnd = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStopId;

        while (loop <= loopEnd)
        {
            TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[loop];

            /* compare ip address, compare before shall check local addr is valid */
            if ((TCPIP_LOCALADR_IS_IPVALID(lcaPtr)) && (ip_addr_cmp(&lcaPtr->ipAdrSrc, destAddrPtr)))
            {
                *rltAddrPtr = *destAddrPtr;
                ret = TRUE;
                break;
            }

            loop++;
        }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input netif point parameter is not find ctrl index or dest ip is nullptr");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}

FUNC_P2CONST(ip4_addr_t, AUTOMATIC, TCPIP_CODE) TcpIp_HookEthArpGetGw(
    P2CONST(struct netif, AUTOMATIC, TCPIP_VAR) netifPtr,
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_VAR) destAddrPtr)
{
    uintx ctrlIdx = TCPIP_GET_NETIF_CTRLINDEX(netifPtr);
    const ip4_addr_t* retPtr = NULL_PTR;
#if defined(TCPIP_INNER_CHECK)

    if ((ctrlIdx < TCPIP_CONTROLLER_NUM) && (NULL_PTR != destAddrPtr))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (TCPIP_LOCAL_ADR_NUM > 0)
        uintx loop = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStartId;
        uintx loopEnd = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStopId;

        while (loop <= loopEnd)
        {
            const TcpIp_LocalAdrHandleType* localAdrVarPtr = &TcpIp_LocalAdrTable[loop];

            if ((TCPIP_LOCALADR_IS_IPVALID(localAdrVarPtr))
                && (!ip4_addr_isany_val(localAdrVarPtr->ipAdrSrc))
                && (!(ip4_addr_isany_val(localAdrVarPtr->netmask)))
                && ip4_addr_netcmp(destAddrPtr, &localAdrVarPtr->ipAdrSrc, &localAdrVarPtr->netmask))
            {
                retPtr = &localAdrVarPtr->defaultrouter;
                break;
            }

            loop++;
        }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input netif point parameter is not find ctrl index or dest ip is nullptr");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return retPtr;
}

FUNC(void, TCPIP_CODE) TcpIp_HookGratuitousArp(
    P2VAR(struct netif, AUTOMATIC, TCPIP_VAR) netifPtr)                                                                 /* PRQA S 3432 */
{
    uintx ctrlIdx = TCPIP_GET_NETIF_CTRLINDEX(netifPtr);
#if defined(TCPIP_INNER_CHECK)

    if (ctrlIdx < TCPIP_CONTROLLER_NUM)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (TCPIP_LOCAL_ADR_NUM > 0)
        uintx loop = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStartId;
        uintx loopEnd = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStopId;
        struct eth_addr local_ethbroadcast = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
        struct eth_addr local_ethzero = {{0, 0, 0, 0, 0, 0}};

        while (loop <= loopEnd)
        {
            TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[loop];
            ip_addr_t* ipvar = &lcaPtr->ipAdrSrc;

            if ((TCPIP_LOCALADR_IS_IPVALID(lcaPtr)) && (!ip4_addr_isany_val(*ipvar)))
            {
                (void)etharp_raw(netifPtr, (struct eth_addr*)netifPtr->hwaddr, &local_ethbroadcast,
                                 (struct eth_addr*)netifPtr->hwaddr, ipvar, &local_ethzero,
                                 ipvar, ARP_REQUEST);
            }

            loop++;
        }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input netif point parameter is not find ctrl index or dest ip is nullptr");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
}

#if (STD_ON == TCPIP_UDP_ENABLED)
FUNC(uint8, TCPIP_CODE) TcpIp_UdpTransmitLocalIpCompare(
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_VAR) pcbLocalAddrPtr,
    P2CONST(struct netif, AUTOMATIC, TCPIP_VAR) netifPtr)
{
    uintx ctrlIdx = TCPIP_GET_NETIF_CTRLINDEX(netifPtr);
    uint8 ret = FALSE;
#if defined(TCPIP_INNER_CHECK)

    if ((ctrlIdx < TCPIP_CONTROLLER_NUM) && (NULL_PTR != pcbLocalAddrPtr))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (TCPIP_LOCAL_ADR_NUM > 0)
        uintx loop = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStartId;
        uintx loopEnd = ControlCfgPtr[ctrlIdx].TcpIpLocalAddrStopId;

        while (loop <= loopEnd)
        {
            TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[loop];

            /* compare ip address, compare before shall check local addr is valid */
            if ((TCPIP_LOCALADR_IS_IPVALID(lcaPtr)) && (ip_addr_cmp(&lcaPtr->ipAdrSrc, pcbLocalAddrPtr)))
            {
                ret = TRUE;
                break;
            }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
            loop++;
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input netif point parameter is not find ctrl index or local pcb ip is nullptr");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */

#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */

#endif/* TCPIP_LWIP_HOOKS */

/***********************************************************************************************************************
 *  LOCAL CALLBACK FUNCTIONS
***********************************************************************************************************************/
#if (TCPIP_CONTROLLER_NUM > 0)
/**
 * @ingroup tcpip_internal
 * Function used for extended netif status callbacks
 * Note: When parsing reason argument, keep in mind that more reasons may be added in the future!
 * @param netif lwip netif that is affected by change
 * @param reason change reason
 * @param args depends on reason, see reason description
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) netIfExtStuChangeCallBack(
    P2VAR(struct netif, AUTOMATIC, TCPIP_APPL_VAR) netif,                                                               /* PRQA S 3432 */
    VAR(netif_nsc_reason_t, AUTOMATIC) reason,
    P2CONST(netif_ext_callback_args_t, AUTOMATIC, TCPIP_APPL_VAR) args)
{
    TCPIP_UNUSED_ARG(args);
#if defined(TCPIP_INNER_CHECK)

    if (TCPIP_CHECK_NETIF_ETHIF_CTRLINDEX(netif))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        uintx ctrlIdx = TCPIP_GET_NETIF_CTRLINDEX(netif);

        if (((TCPIP_NETIF_NSC_IPCHANGED) & reason) != 0u)
        {
            TcpIp_ControllerStatus* ctrlStuPtr = &TcpIp_ControllerVar[ctrlIdx];
            TcpIp_IpAddrStateType ipAdrState = TCPIP_IPADDR_STATE_ASSIGNED;
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)

            if (!ip4_addr_isany_val(*netif_ip4_addr(netif)))
            {
                /* SWS_TCPIP_00085 */
                /* if at least one IP address has been successfully assigned to an EthIf controller and the current
                state is TCPIP_STATE_STARTUP for that EthIf controller, the TcpIp module shall enter the state
                TCPIP_STATE_ONLINE for theEthIf controller */
                if (TCPIP_STATE_STARTUP == ctrlStuPtr->ctrlStatus)
                {
                    ctrlStuPtr->ctrlStatus = TCPIP_STATE_ONLINE;
                    TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_ONLINE);
                }
            }
            else
            {
                /* ip adr is invalid */
                ipAdrState =  TCPIP_IPADDR_STATE_UNASSIGNED;
            }

#endif/* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) */
            /* pool this control link all localaddress */
            TcpIp_CtrlLinkLocalAddrChagneHandle(ctrlIdx, ipAdrState, FALSE);
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("netif struct member is not find ethif control index");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
}

/**
 * @ingroup tcpip_internal
 * lwip netif get mac addr form ethif module
 * @param netif lwip netif struct
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_NetIfGetPhyAddrFormEthIf(
    P2VAR(struct netif, AUTOMATIC, TCPIP_APPL_VAR) netif)
{
    uint8 MacAddr[NETIF_MAX_HWADDR_LEN];
    EthIf_GetPhysAddr(TCPIP_GET_NETIF_CTRLINDEX(netif), MacAddr);
    /* set MAC hardware address length */
    netif->hwaddr_len = NETIF_MAX_HWADDR_LEN;

    /* set MAC hardware address */
    for (uintx i = 0; i < netif->hwaddr_len; i++)
    {
        netif->hwaddr[i] = MacAddr[i];
    }
}

/**
 * @ingroup tcpip_internal
 * lwip netif initial callback
 * @param netif lwip netif struct
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) ethernetif_init(
    P2VAR(struct netif, AUTOMATIC, TCPIP_APPL_VAR) netif)                                                               /* PRQA S 3432 */
{
    err_t lwRet = (err_t)ERR_OK;
#if LWIP_IPV4
    netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = NULL_PTR;
#if defined(TCPIP_INNER_CHECK)

    if (TCPIP_CHECK_NETIF_ETHIF_CTRLINDEX(netif))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_NetIfGetPhyAddrFormEthIf(netif);
        /* maximum transfer unit */
        netif->mtu = 1500; /* Ethernet max message length is 1500 */
        /* device capabilities */
        /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
        netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#if LWIP_NETIF_HOSTNAME
        netif->hostname = "DoIP-ISoft";
#endif
#if LWIP_IPV6 && LWIP_IPV6_MLD

        /*
         * For hardware/netifs that implement MAC filtering.
         * All-nodes link-local is handled by default, so we must let the hardware know
         * to allow multicast packets in.
         * Should set mld_mac_filter previously. */
        if (netif->mld_mac_filter != NULL_PTR)
        {
            ip6_addr_t ip6_allnodes_ll;
            ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
            netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
        }

#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("netif struct member is not find ethif control index");
        lwRet = (err_t)ERR_IF;
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return lwRet;
}
#endif /* (TCPIP_CONTROLLER_NUM > 0) */

#if (STD_ON == TCPIP_TCP_ENABLED)
/*
 * Function prototype for deallocation of arguments. Called *just before* the
 * pcb is freed, so don't expect to be able to do anything with this pcb!
 *
 * @param id ext arg id (allocated via @ref tcp_ext_arg_alloc_id)
 * @param data pointer to the data (set via @ref tcp_ext_arg_set before)
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TcpExtargCallbackPcbDestroyed(
    VAR(u8_t, AUTOMATIC) id,
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) data)                                                                        /* PRQA S 3432 */
{
    TCPIP_UNUSED_ARG(id);

    if (NULL_PTR != data)
    {
        TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(data);                                                    /* PRQA S 0326,0306 */
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
        struct tcp_pcb* pcbTmpPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
#if defined(TCPIP_INNER_CHECK)

        if (NULL_PTR != pcbTmpPtr)
#endif /* defined(TCPIP_INNER_CHECK) */
        {
            /* clear extend callback fucntion */
            tcp_ext_arg_set(pcbTmpPtr, 0, NULL_PTR);
            /* set callback is null shall trig lwip assert */
            /* tcp_ext_arg_set_callbacks(pcbTmpPtr, 0u, NULL_PTR); */
            /* clear normal callback function */
            tcp_arg(pcbTmpPtr, NULL);

            /* listen socket handle is difference */
            if (TCPIP_TCP_IS_SERVERSOCKET(socketMngPtr))
            {
                tcp_accept(pcbTmpPtr, NULL);
            }
            else
            {
                tcp_sent(pcbTmpPtr, NULL);
                tcp_recv(pcbTmpPtr, NULL);
                tcp_err(pcbTmpPtr, NULL);
            }

            /* clear socket server flag */
            TCPIP_TCP_CLR_SERVERSOCKET(socketMngPtr);
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

            if (NULL_PTR != socketMngPtr->tlsPtr)
            {
                TcpIp_TlsSocketDeInit(socketMngPtr);
            }

#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
            TcpIp_TcpSocketFreeAndEventNotifyUpLayer(socketMngPtr, TCPIP_TCP_CLOSED);
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("socket:%d pcb is null\n", socketId);
        }

#endif /* defined(TCPIP_INNER_CHECK) */
    }
}

/** Function prototype for tcp error callback functions. Called when the pcb  receives a RST or is unexpectedly closed
 * for any other reason.
 *
 * @note The corresponding pcb is already freed when this callback is called!
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param err Error code to indicate why the pcb has been closed
 *            ERR_ABRT: aborted through tcp_abort or by a TCP timer
 *            ERR_RST: the connection was reset by the remote host
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TcpErrCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err)
{
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->ownerCfgPtr))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(arg);                                                     /* PRQA S 0326,0306 */
        /* notify upper layer socket is RESET */
        TcpIp_Up_TcpIpEventType eventFuncPtr = socketMngPtr->ownerCfgPtr->Up_TcpIpEventFuncPtr;
        /* tcp socket error event only notify once ,sence: receiver remote FIN,local response ACK but remote send RST */
        TCPIP_TCP_CLR_PENDINGEVENT(socketMngPtr);

        if (NULL_PTR != eventFuncPtr)
        {
            /* ERR_RST,ERR_CLSD,ERR_ABRT */
            TcpIp_EventType event = (err != (err_t)ERR_CLSD) ? TCPIP_TCP_RESET : TCPIP_TCP_CLOSED;
            eventFuncPtr(socketId,  event);
        }
    }
}

/** Function prototype for tcp sent callback functions. Called when sent data has been acknowledged by the remote side.
 * Use it to free corresponding resources.This also means that the pcb has now space available to send new data.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb for which data has been acknowledged
 * @param len The amount of bytes acknowledged
 * @return ERR_OK: try to send some data by calling tcp_output
 *            Only return ERR_ABRT if you have called tcp_abort from within the callback function!
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpSentCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */
    VAR(uint16, AUTOMATIC) len)
{
    err_t err = (err_t)ERR_OK;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->pcbPtr) && (socketMngPtr->pcbPtr == tpcb))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_TcpTxConfNotifyUpLayer(socketMngPtr, len);
    }

    return err;
}

/** Function prototype for tcp connected callback functions. Called when a pcb is connected to the remote side after
 * initiating a connection attempt by calling tcp_connect().
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb which is connected
 * @param err An unused error code, always ERR_OK currently ;-) @todo!
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 *
 * @note When a connection attempt fails, the error callback is currently called!
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpConnnectCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err)
{
    err_t err_ret = (err_t)ERR_ABRT;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->pcbPtr) && (socketMngPtr->pcbPtr == tpcb))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        if ((err_t)ERR_OK == err)
        {
            err_ret = (err_t)ERR_OK;
            TcpIp_TcpConnectNofiyUplayer(socketMngPtr);
        }

        /* input parameter err not more condition,so not care other result */
    }

    return err_ret;
}

/** Function prototype for tcp accept callback functions. Called when a new connection can be accepted on a
 * listening pcb.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param newpcb The new connection pcb
 * @param err An error code if there has been an error accepting.
 *            Only return ERR_ABRT if you have called tcp_abort from within the callback function!
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpAcceptCallcback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) newpcb,                                                            /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err)
{
    err_t retErr = err;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != arg) && ((err_t)ERR_OK == err))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_SocketHandleType* socketMngPtr = TcpIp_MallocTcpSocketIdByPcb((TcpIp_SocketHandleType*)arg, newpcb);

        if (NULL_PTR != socketMngPtr)
        {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

            if (NULL_PTR == socketMngPtr->tlsPtr)
            {
                ip_addr_t reIp;
                TcpIp_SockAddrType remoteAdrInfo;
                /* get local port and ip */
                retErr = tcp_tcp_get_tcp_addrinfo(newpcb, 1, &reIp, &remoteAdrInfo.port);

                if ((err_t)ERR_OK == retErr)
                {
                    TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAdrInfo.addr, reIp.addr);
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
                    remoteAdrInfo.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
                    remoteAdrInfo.domain = TCPIP_AF_INET6;
#else /* TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */
                    remoteAdrInfo.domain = IP_IS_V4_VAL(reIp) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */
                    /* static tls connection assignment */
                    TcpIp_TlsBaseStaticConnect(socketMngPtr, &remoteAdrInfo, TLS_SERVER);
                }
            }

            /* if tls connect not assignment,this socket is normal */
            if (NULL_PTR == socketMngPtr->tlsPtr)
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
            {
                TcpIp_SocketIdType listenSocketId = TCPIP_SOCKID_CAL_ARYADR(arg);                                       /* PRQA S 0326,0306 */
                Std_ReturnType ret = E_NOT_OK;

                /* [SWS_TCPIP_00094]  The TcpIp module shall only accept new TCP connections if the related EthIf
                controller is in state TCPIP_STATE_ONLINE */
                if (TCPIP_STATE_ONLINE == TcpIp_ControllerVar[socketMngPtr->ctrlIndex].ctrlStatus)
                {
                    ret = TcpIp_TcpAcceptNofiyUplayer(listenSocketId, socketMngPtr);
                }

                if ((Std_ReturnType)E_OK == ret)
                {
                    retErr = (err_t)ERR_OK;
                }
                else
                {
                    /* if accept is not ok ,not continue notify upper layer event */
                    tcp_err(newpcb, NULL_PTR);
                    retErr = (err_t)ERR_VAL;
                }
            }
        }
    }

    return retErr;
}

/** Function prototype for tcp receive callback functions. Called when data has been received.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb which received data
 * @param p The received data (or NULL when the connection has been closed!)
 * @param err An error code if there has been an error receiving Only return ERR_ABRT if you have called tcp_abort
 *            from within the callback function!
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TcpRecvCallback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */
    P2VAR(struct pbuf, AUTOMATIC, TCPIP_APPL_VAR) p,                                                                    /* PRQA S 3432 */
    VAR(err_t, AUTOMATIC) err)
{
    err_t ret = err;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->pcbPtr) && (socketMngPtr->pcbPtr == tpcb))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(arg);                                                     /* PRQA S 0326,0306 */

        if ((err_t)ERR_OK == ret)
        {
            if (NULL_PTR != p)
            {
                ret = TcpIp_TcpRxIndNotifyUpLayer(socketMngPtr, p);
            }
            else /* receive FIN */
            {
                TcpIp_Up_TcpIpEventType eventFuncPtr = socketMngPtr->ownerCfgPtr->Up_TcpIpEventFuncPtr;

                if ((NULL_PTR != eventFuncPtr) && (!TCPIP_TCP_IS_PENDINGEVENT(socketMngPtr)))
                {
                    /* report FIN to upper layer */
                    eventFuncPtr(socketId, TCPIP_TCP_FIN_RECEIVED);
                }
            }
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        ret = (err_t)ERR_ABRT;
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */


#if (STD_ON == TCPIP_UDP_ENABLED)
/** Function prototype for udp pcb receive callback functions addr and port are in same byte order as in the pcb
 * The callback is responsible for freeing the pbuf if it's not used any more.
 *
 * ATTENTION: Be aware that 'addr' might point into the pbuf 'p' so freeing this pbuf
 *            can make 'addr' invalid, too.
 *
 * @param arg user supplied argument (udp_pcb.recv_arg)
 * @param pcb the udp_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_UdpRecvCallcback(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */
    P2VAR(struct udp_pcb, AUTOMATIC, TCPIP_APPL_VAR) pcb,                                                               /* PRQA S 3432 */
    P2VAR(struct pbuf, AUTOMATIC, TCPIP_APPL_VAR) p,                                                                    /* PRQA S 3432 */
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_APPL_VAR) addr,
    VAR(uint16, AUTOMATIC) port)
{
    struct pbuf* q = p;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != p) && (socketMngPtr->pcbPtr == pcb))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(arg);                                                     /* PRQA S 0326,0306 */
        /* rxindication function pointer */
        TcpIp_Up_RxIndicationType rxIndFuncPtr = socketMngPtr->ownerCfgPtr->Up_RxIndicationFuncPtr;

        if (NULL_PTR != rxIndFuncPtr)
        {
            TcpIp_SockAddrType remoteAdrInfo;
            ip_addr_t rxIpAdr;
            rxIpAdr.addr = addr->addr;
            TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAdrInfo.addr, rxIpAdr.addr);
            remoteAdrInfo.port = port;
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
            remoteAdrInfo.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
            remoteAdrInfo.domain = TCPIP_AF_INET6;
#else /* TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */
            remoteAdrInfo.domain = IP_IS_V4_VAL(reIp) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif/* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */

            if (NULL_PTR != p->next)
            {
                q = pbuf_coalesce(p, PBUF_RAW);
#if defined(TCPIP_INNER_CHECK)

                if (p == q)
                {
                    TCPIP_INNER_CHECK_OUTPUT("pbuf_coalesce handle pbuf error\n");
                }

#endif/* defined(TCPIP_INNER_CHECK) */
            }

            rxIndFuncPtr(socketId, &remoteAdrInfo, (uint8*)(q->payload), q->len);
        }
    }

    if (NULL_PTR != q)
    {
        (void)pbuf_free(q);
    }
}

/**
 * @ingroup tcpip_internal
 * free udp socket,shall release inner socket management data
 * @param socketId inner socket index,from TcpIp_GetSocket
 * @return E_OK ok,another is failed
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_FreeUdpSocketId(VAR(TcpIp_SocketIdType, TCPIP_VAR) socketId)
{
#if defined(TCPIP_INNER_CHECK)

    if (socketId < TCPIP_SOCKET_NUM)
#endif /* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
        Udp_SocketUsedNum--;
        socketMngPtr->pcbPtr = NULL_PTR;
#if (TCPIP_CONTROLLER_NUM > 0)
        TcpIp_ControllerStatus* ctrlMngPtr = &TcpIp_ControllerVar[socketMngPtr->ctrlIndex];

        if (0u < ctrlMngPtr->socketNum)
        {
            ctrlMngPtr->socketNum--;
        }

        /* [SWS_TCPIP_00077] */
        if (TCPIP_CTRL_IS_STATE_PENDING(ctrlMngPtr, TCPIP_STATE_SHUTDOWN) && (0u == ctrlMngPtr->socketNum))
        {
            TCPIP_CTRL_CLR_STATE_PENDING(ctrlMngPtr, TCPIP_STATE_SHUTDOWN);
            ctrlMngPtr->ctrlStatus = TCPIP_STATE_OFFLINE;
            /* call lwip close netif */
            netif_set_down(&ctrlMngPtr->netifVar);
            netif_set_link_down(&ctrlMngPtr->netifVar);
            /* all socket this control is free,shall be switch TCPIP_STATE_OFFLINE */
            TcpIp_ControlStateNotifyEthSm(socketMngPtr->ctrlIndex, TCPIP_STATE_OFFLINE);
        }

#endif /* (TCPIP_CONTROLLER_NUM > 0) */
        /* link list handle */
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
        socketMngPtr->nextSocketId = TcpIp_SocketLastId;
        TcpIp_SocketLastId = socketId;
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("free socket is error! socketId=%d\n", socketId);
    }

#endif /* defined(TCPIP_INNER_CHECK) */
}
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */

#if (TCPIP_CONTROLLER_NUM > 0)
/**
 * notify ethsm controller state
 *
 * @param ctrlIdx inner controller index
 * @param notifyState @ref TcpIp_StateType
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_ControlStateNotifyEthSm(
    VAR(uintx, AUTOMATIC) ctrlIdx,
    VAR(TcpIp_StateType, AUTOMATIC) notifyState)
{
    TCPIP_UNUSED_ARG(ctrlIdx);
    TCPIP_UNUSED_ARG(notifyState);
#if (STD_ON == TCPIP_ETHSM_SUPPORT)
    /* [SWS_TCPIP_00084] */
    /* After each transition the TcpIp module shall report the new state to EthSM via
    EthSM_TcpIpModeIndication() */
    EthSM_TcpIpModeIndication(ControlCfgPtr[ctrlIdx].EthIfCtrlRef, notifyState);
#endif/* (STD_ON == TCPIP_ETHSM_SUPPORT) */
}

#if (TCPIP_LOCAL_ADR_NUM > 0)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_CtrlLinkLocalAddrChagneHandle(
    VAR(uintx, AUTOMATIC) ctrlIndex,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) ipAdrState,
    VAR(boolean, AUTOMATIC) forceNotifyFlg)
{
    /* Info:In an ideal scenario localAddrId shall link used socket and corresponding socket owner ,
    but In most cases, LocalAddrid has not yet mapped the socket owner ,Therefore, we believe that all socket owners
    should be notified of LocalAddrid state changes */
#if defined(TCPIP_INNER_CHECK)
    if ((LocalAdrCfgPtr != NULL_PTR) && (ControlCfgPtr != NULL_PTR) && ((ctrlIndex < TCPIP_CONTROLLER_NUM)))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        uintx loop = ControlCfgPtr[ctrlIndex].TcpIpLocalAddrStartId;
        uintx loopEnd = ControlCfgPtr[ctrlIndex].TcpIpLocalAddrStopId;

        while (loop <= loopEnd)
        {
            TcpIp_LocalAdrHandleType* lcaPtr = &TcpIp_LocalAdrTable[loop];
            boolean notifyFlg = forceNotifyFlg;

            if (TCPIP_LOCALADR_IS_REQUESTIP(lcaPtr))
            {
                TCPIP_LOCALADR_CLR_REQUESTIP(lcaPtr);
                /* The address is written back to inner local address variable asynchronously */
                const struct netif* netifPtr = &TcpIp_ControllerVar[ctrlIndex].netifVar;
                const TcpIp_LocalAddrType* localCfgPtr = &LocalAdrCfgPtr[loop];

                if ((TCPIP_IPADDR_STATE_ASSIGNED == ipAdrState) && (localCfgPtr->AddrType == TCPIP_UNICAST))
                {
                    lcaPtr->ipAdrSrc = netifPtr->ip_addr;
                    lcaPtr->netmask = netifPtr->netmask;
                    lcaPtr->defaultrouter = netifPtr->gw;
                }

                if (TCPIP_IPADDR_STATE_ASSIGNED == ipAdrState)
                {
                    TCPIP_LOCALADR_SET_IPVALID(lcaPtr);
                }
                else if (TCPIP_IPADDR_STATE_UNASSIGNED == ipAdrState)
                {
                    TCPIP_LOCALADR_CLR_IPVALID(lcaPtr);
                }
                else
                {
                    /* TCPIP_IPADDR_STATE_ONHOLD state not switch local addr ip valid flag */
                }

                /* notify all socket owner */
                notifyFlg = TRUE;
            }

            if ((boolean)TRUE == notifyFlg)
            {
                TcpIp_NotfiyUpperLayerIpAdrChange((TcpIp_LocalAddrIdType)loop, ipAdrState);
            }

            loop++;
        }
    }
}

#if ((STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) && (1u < TCPIP_CTRL_LOCAL_NUMBER))
TCPIP_LOCAL_INLINE FUNC(boolean, TCPIP_CODE) TcpIp_CtrlLinkAllLocalIsRelease(
    P2CONST(TcpIp_ControllerStatus, AUTOMATIC, TCPIP_VAR) ctrlPtr)
{
    boolean findFlg = TRUE;

    for (uintx loop = 0; loop < TCPIP_CTRL_LOCAL_NUMBER; loop++)
    {
        if (ctrlPtr->localUsedMask[loop] != 0u)
        {
            findFlg = FALSE;
            break;
        }
    }

    return findFlg;
}
#endif/* ((STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) && (1u < TCPIP_CTRL_LOCAL_NUMBER)) */

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */

/**
 * controller ip address assignment handle
 *
 * @param ctrlIndex inner controller index
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_RequestCtrlIpAssignment(VAR(uintx, AUTOMATIC) ctrlIndex)
{
#if (TCPIP_LOCAL_ADR_NUM > 0)
    uintx loop = ControlCfgPtr[ctrlIndex].TcpIpLocalAddrStartId;
    uintx loopEnd = ControlCfgPtr[ctrlIndex].TcpIpLocalAddrStopId;

    /* pool this controller link all local address */
    while (loop <= loopEnd)
    {
        const TcpIp_LocalAddrType* localAdrCfgPtr = &(LocalAdrCfgPtr[loop]);
#if defined(TCPIP_CHECK)

        if ((localAdrCfgPtr->AddrAssignmentCfgPtr != NULL_PTR) && (localAdrCfgPtr->AddrAssignmentNum > 0U))
#endif/* defined(TCPIP_CHECK) */
        {
#if (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON)

            for (uintx index = 0u; index < localAdrCfgPtr->AddrAssignmentNum; index++)
#else/* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT != STD_ON) */
            uintx index = 0u;

#endif/* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON) */
            {
                const TcpIp_AddrAssignmentType* addAssCfgPtr = &localAdrCfgPtr->AddrAssignmentCfgPtr[index];

                if (TCPIP_AUTOMATIC == addAssCfgPtr->AssignmentTrigger)
                {
                    (void)TcpIp_AutoMaticAssignmentIpByLocal(loop, localAdrCfgPtr, addAssCfgPtr);
                }

                /* TCPIP_MANUAL:this localAdr assigenment by the upper layer */
            }
        }

#if defined(TCPIP_CHECK)
        else
        {
            TCPIP_CHECK_OUTPUT("this localAdrIndex:%d not have assignement config\n", loop);
        }

#endif/* defined(TCPIP_CHECK) */
        loop++;
    }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
}

/**
 * controller ip address unassignment handle
 *
 * @param ctrlIndex inner controller index
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_UnasignedCtrlIpAssigemt(VAR(uintx, AUTOMATIC) ctrlIndex)
{
#if (TCPIP_LOCAL_ADR_NUM > 0)
    uintx loop = ControlCfgPtr[ctrlIndex].TcpIpLocalAddrStartId;
    uintx loopEnd = ControlCfgPtr[ctrlIndex].TcpIpLocalAddrStopId;

    /* pool this controller link all local address */
    while (loop <= loopEnd)
    {
        const TcpIp_LocalAddrType* localAdrCfgPtr = &(LocalAdrCfgPtr[loop]);
#if defined(TCPIP_CHECK)

        if ((localAdrCfgPtr->AddrAssignmentCfgPtr != NULL_PTR) && (localAdrCfgPtr->AddrAssignmentNum > 0U))
#endif/* defined(TCPIP_CHECK) */
        {
#if (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON)

            for (uintx index = 0u; index < localAdrCfgPtr->AddrAssignmentNum; index++)
#else/* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT != STD_ON) */
            uintx index = 0u;

#endif/* (TCPIP_LOCALADDR_MUILT_ASSIGNMENT == STD_ON) */
            {
                const TcpIp_AddrAssignmentType* addAssCfgPtr = &localAdrCfgPtr->AddrAssignmentCfgPtr[index];
                (void)TcpIp_ClearLocalAdrIpVar(loop, ctrlIndex, addAssCfgPtr->AssignmentMethod);
            }
        }

#if defined(TCPIP_CHECK)
        else
        {
            TCPIP_CHECK_OUTPUT("this localAdrIndex:%d not have assignement config\n", loop);
        }

#endif/* defined(TCPIP_CHECK) */
        loop++;
    }

#endif/* (TCPIP_LOCAL_ADR_NUM > 0) */
}

#endif /* (TCPIP_CONTROLLER_NUM > 0) */


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#if (TCPIP_CONTROLLER_NUM > 0)
/**
 * @ingroup tcpip_internal
 * controller init,call by TcpIp_Init
 * @param ctrlCnt controller number
 * @param ctrlCfgPtr post build controller config data point @TcpIp_CtrlType
 */
FUNC(void, TCPIP_CODE) TcpIp_ControllerInit(
    VAR(uint32, AUTOMATIC) ctrlCnt,
    P2CONST(TcpIp_CtrlType, AUTOMATIC, TCPIP_APPL_DATA) ctrlCfgPtr)
{
    TCPIP_UNUSED_ARG(ctrlCnt);
#if (1u < TCPIP_CONTROLLER_NUM)
    uintx ctrlPollCnt = (ctrlCnt > TCPIP_CONTROLLER_NUM) ? TCPIP_CONTROLLER_NUM : ctrlCnt;

    for (uintx index = 0; index < ctrlPollCnt; index++)
#else/* (1u >= TCPIP_CONTROLLER_NUM) */
    uintx index = 0u;

#endif/* (1u < TCPIP_CONTROLLER_NUM) */
    {
        TCPIP_REG_NETIF_ETHIF_CTRLINDEX(&TcpIp_ControllerVar[index].netifVar, ctrlCfgPtr[index].EthIfCtrlRef);
        struct netif* netifPtr =
            netif_add_noaddr(&TcpIp_ControllerVar[index].netifVar, (void*)NULL_PTR, ethernetif_init, NULL_PTR);
#if defined(TCPIP_INNER_CHECK)

        if ((void*)NULL_PTR != netifPtr)
#endif /* defined(TCPIP_INNER_CHECK) */
        {
#if LWIP_IPV6
            netif_create_ip6_linklocal_address(netifPtr, 1);
#endif /* LWIP_IPV6 */
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
            autoip_set_struct(netifPtr, &TcpIp_ControllerVar[index].netif_autoip);
#endif /* (STD_ON == TCPIP_AUTOIP_ENABLED) */
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
            dhcp_set_struct(netifPtr, &TcpIp_ControllerVar[index].netif_dhcp);
#endif /* (STD_ON == TCPIP_DHCP_CLIENT_ENABLED) */
            netif_set_down(netifPtr);
            netif_set_link_down(netifPtr);
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("netif_add return nullptr");
        }

#endif /* defined(TCPIP_INNER_CHECK) */
        ControlCfgPtr = ctrlCfgPtr;
        /* inner val init */
        TcpIp_ControllerVar[index].maskFlg = 0u;
        TcpIp_ControllerVar[index].stateFlag = 0u;
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
        TcpIp_ControllerVar[index].autoipTimer = 0u;
#endif/* (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER) */
        /* [SWS_TCPIP_00136] */
        TcpIp_ControllerVar[index].ctrlStatus = TCPIP_STATE_OFFLINE;
        /* clear all local mask in this control */
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
#if (1u < TCPIP_CTRL_LOCAL_NUMBER)

        for (uintx loop = 0; loop < TCPIP_CTRL_LOCAL_NUMBER; loop++)
#else
        uintx loop = 0;

#endif/* (0u < TCPIP_CTRL_LOCAL_NUMBER) */
        {
            TcpIp_ControllerVar[index].localUsedMask[loop] = 0u;
        }
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
    }
    netif_add_ext_callback(&netifExtCallBackStu, netIfExtStuChangeCallBack);
    netif_set_default(&TcpIp_ControllerVar[0].netifVar);
}
/**
 * @ingroup tcpip_internal
 * get control state
 * @param ctrlIndex inner control index
 */
FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_GetControlState(VAR(uintx, AUTOMATIC) ctrlIndex)
{
    TcpIp_StateType state =  TCPIP_STATE_OFFLINE;
#if defined(TCPIP_INNER_CHECK)

    if (ctrlIndex < TCPIP_CONTROLLER_NUM)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        state = TcpIp_ControllerVar[ctrlIndex].ctrlStatus;
    }

    return state;
}

#endif /* (TCPIP_CONTROLLER_NUM > 0) */

#if (TCPIP_SOCKET_NUM > 0u)
/**
 * @ingroup tcpip_internal
 * socket init,call by TcpIp_Init
 * @param socketNum socket number
 */
FUNC(void, TCPIP_CODE) TcpIp_SocketInit(VAR(uint8, AUTOMATIC) socketNum)
{
    uintx num = (socketNum < TCPIP_SOCKET_NUM) ? socketNum : TCPIP_SOCKET_NUM;
    TcpIp_SocketLastId = 0u;

    for (uintx index = 0u; index < num; index++)
    {
        TcpIp_SocketTable[index].ownerCfgPtr = NULL_PTR;
        TcpIp_SocketTable[index].nextSocketId = (TcpIp_SocketIdType)(index + 1u);
    }

    TcpIp_SocketTable[num - 1u].nextSocketId = TCPIP_SOCKET_NUM;
#if (TCPIP_UDP_SOCKET_MAX > 0)
    Udp_SocketUsedNum = 0u;
#endif /* (TCPIP_UDP_SOCKET_MAX > 0) */
#if (TCPIP_TCP_SOCKET_MAX > 0)
    Tcp_SocketUsedNum = 0u;
#endif /* (TCPIP_TCP_SOCKET_MAX > 0) */
}
#endif /* (TCPIP_SOCKET_NUM > 0u) */

#if (TCPIP_LOCAL_ADR_NUM > 0u)
/**
 * @ingroup tcpip_internal
 * local address init,call by TcpIp_Init
 * @param localAdrNum local address number
 */
FUNC(void, TCPIP_CODE) TcpIp_LocalAdrInit(VAR(uint32, AUTOMATIC) localAdrNum,
                                          P2CONST(TcpIp_LocalAddrType, AUTOMATIC, TCPIP_APPL_DATA) localAdrCfgPtr)
{
    uintx pollCnt = (localAdrNum > TCPIP_LOCAL_ADR_NUM) ? TCPIP_LOCAL_ADR_NUM : localAdrNum;
#if (TCPIP_LOCAL_ADR_NUM > 1u)

    for (uintx index = 0u; index < pollCnt; index++)
#else/* (TCPIP_LOCAL_ADR_NUM == 1u) */
    uintx index = 0u;

    if (index < pollCnt)
#endif/* (TCPIP_LOCAL_ADR_NUM > 1u) */
    {
        TcpIp_LocalAdrTable[index].ipAdrSrc.addr = TCPIP_IPADDR_ANY;
        TcpIp_LocalAdrTable[index].defaultrouter.addr = TCPIP_IPADDR_ANY;
        TcpIp_LocalAdrTable[index].netmask.addr = TCPIP_IPADDR_ANY;
#if LWIP_NETIF_HOSTNAME
        TcpIp_LocalAdrTable[index].hostNameLen = 10u;
#endif /* LWIP_NETIF_HOSTNAME */
    }

    LocalAdrCfgPtr = localAdrCfgPtr;
}
#endif /* (TCPIP_LOCAL_ADR_NUM > 0u) */

#if (TCPIP_TCP_SOCKET_MAX > 0u)
/**
 * @ingroup tcpip_internal
 * close socket,support tcp and udp protocal
 * @param socketId inner socket index,from TcpIp_GetSocket
 * @param isAbort true,shall call tcp_abort;false shall be call tcp_close
 * @return E_OK close is execute ok,another is error
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CloseSocketHandle(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(boolean, AUTOMATIC) isAbort)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

    /* if not support inner check, pcb shull not equal nullpoint */
    if (NULL_PTR != socketMngPtr->pcbPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (STD_ON == TCPIP_TCP_ENABLED)

        if (TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
        {
            struct tcp_pcb* localTcpPcbPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
            err_t errRes;
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);

            /* tcp socket close not nodify upper module
             listen socket close shall used close,not call isAbort */
            if (localTcpPcbPtr->state == LISTEN)
            {
                TCPIP_TCP_SET_PENDINGEVENT(socketMngPtr);
                errRes = (err_t)tcp_close(localTcpPcbPtr);
            }
            else if ((boolean)TRUE != isAbort)
            {
                TCPIP_TCP_SET_PENDINGEVENT(socketMngPtr);
#if (TCPIP_TCP_HALFCLOSE)
                /* TCP supports half - close function, then this end is not allowed to send, but allow receiving */
                errRes = (err_t)tcp_shutdown(localTcpPcbPtr, 0, 1);
#else/* !(TCPIP_TCP_HALFCLOSE) */
                errRes = (err_t)tcp_close(localTcpPcbPtr);
#endif/* (TCPIP_TCP_HALFCLOSE) */
            }
            else
            {
                tcp_abort(localTcpPcbPtr);
                errRes = (err_t)ERR_OK;
            }

            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);

            if ((err_t)ERR_OK == errRes)
            {
                ret = E_OK;
            }
        }

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */
#if (STD_ON == TCPIP_UDP_ENABLED)

        if (TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
        {
            struct udp_pcb* localUdpPcbPtr = (struct udp_pcb*)socketMngPtr->pcbPtr;
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            udp_disconnect(localUdpPcbPtr);
            udp_remove(localUdpPcbPtr);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            /* shall notify upper module this udp socket is close */
            TcpIp_UdpSocketFreeAndEventNotifyUpLayer(socketMngPtr, TCPIP_UDP_CLOSED);
            ret = E_OK;
        }

#endif/* (STD_ON == TCPIP_UDP_ENABLED) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        /* maybe pcb is free */
        ret = E_OK;
        TCPIP_INNER_CHECK_OUTPUT("repeat close socket:%d\n", socketId);
    }

#endif /* defined(TCPIP_INNER_CHECK) */
    return ret;
}

#endif /* (TCPIP_TCP_SOCKET_MAX > 0u) */

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @ingroup tcpip_internal
 * malloc tcp socket,call by tcp listen accept
 * @param domain TCPIP_AF_INET ipv4,TCPIP_AF_INET6 ipv6
 * @param socketOwnerCfgPtr socket owner configuration data,normal is soad socket owner
 * @param socketIdPtr output malloc socket index result
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_MallocTcpSocketId(
    VAR(TcpIp_DomainType, AUTOMATIC) domain,
    P2CONST(TcpIp_SocketOwnerLCfgType, AUTOMATIC, TCPIP_APPL_DATA) socketOwnerCfgPtr,
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr)                                                  /* PRQA S 3432 */
{
    TCPIP_UNUSED_ARG(domain);
    Std_ReturnType ret = E_NOT_OK;
    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);

    if ((Tcp_SocketUsedNum < TCPIP_TCP_SOCKET_MAX) && (TcpIp_SocketLastId < TCPIP_SOCKET_NUM))
    {
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[TcpIp_SocketLastId];
        struct tcp_pcb* tpcb;
#if (TCPIP_SC3 != TCPIP_SCALABILITY_CLASS)
        tpcb = tcp_new();
#else  /* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */

        if (domain == TCPIP_AF_INET)
        {
            tpcb = tcp_new_ip_type(IPADDR_TYPE_V4);
        }
        else
        {
            tpcb = tcp_new_ip_type(IPADDR_TYPE_V6);
        }

#endif /* (TCPIP_SC3 != TCPIP_SCALABILITY_CLASS) */

        if ((tpcb != NULL_PTR) && (TCPIP_SOCKET_NUM > socketMngPtr->nextSocketId))
        {
            /* reg extend callback function */
            tcp_ext_arg_set_callbacks(tpcb, 0u, TcpIp_tcpExtArgCallbacksTable);
            tcp_ext_arg_set(tpcb, 0u, socketMngPtr);
            tcp_arg(tpcb, socketMngPtr);
            /* shall reg recv function */
            tcp_recv(tpcb, TcpIp_TcpRecvCallback);
            /* reg connect error callback function */
            tcp_err(tpcb, TcpIp_TcpErrCallback);
            /* reg send call back function */
            tcp_sent(tpcb, TcpIp_TcpSentCallback);
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
            socketMngPtr->socketFlag = 0u;
            /* state handle */
            Tcp_SocketUsedNum++;
            /* output parameter setting */
            *socketIdPtr = TcpIp_SocketLastId;
            socketMngPtr->protocol = TCPIP_IPPROTO_TCP;
            socketMngPtr->pcbPtr = tpcb;
            /* link list handle */
            TcpIp_SocketLastId = socketMngPtr->nextSocketId;
            socketMngPtr->nextSocketId = TCPIP_SOCKET_INVALIDVLU;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
            socketMngPtr->listenSocketId = TCPIP_SOCKET_INVALIDVLU;
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
            /* comm unit fill data */
            socketMngPtr->ownerCfgPtr = socketOwnerCfgPtr;
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
            ret = E_OK;
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("not have enough tcp socket");
    }

#endif /* (TCPIP_TCP_SOCKET_MAX > 0u) */
    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
    return ret;
}

#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @ingroup tcpip_internal
 * malloc udp socket,call by tcp listen accept
 * @param domain TCPIP_AF_INET ipv4,TCPIP_AF_INET6 ipv6
 * @param socketOwnerCfgPtr socket owner configuration data,normal is soad socket owner
 * @param socketIdPtr output malloc socket index result
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_MallocUdpSocketId(
    VAR(TcpIp_DomainType, AUTOMATIC) domain,
    P2CONST(TcpIp_SocketOwnerLCfgType, AUTOMATIC, TCPIP_APPL_DATA) socketOwnerCfgPtr,
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr)                                                  /* PRQA S 3432 */
{
    TCPIP_UNUSED_ARG(domain);
    Std_ReturnType ret = E_NOT_OK;

    if ((Udp_SocketUsedNum < TCPIP_UDP_SOCKET_MAX) && (TcpIp_SocketLastId < TCPIP_SOCKET_NUM))
    {
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[TcpIp_SocketLastId];
#if (TCPIP_SC3 != TCPIP_SCALABILITY_CLASS)
        socketMngPtr->pcbPtr = udp_new();
#else  /* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */

        if (domain == TCPIP_AF_INET)
        {
            socketMngPtr->pcbPtr = udp_new_ip_type(IPADDR_TYPE_V4);
        }
        else
        {
            /* domain == TCPIP_AF_INET6 */
            socketMngPtr->pcbPtr = udp_new_ip_type(IPADDR_TYPE_V6);
        }

#endif /* (TCPIP_SC3 != TCPIP_SCALABILITY_CLASS) */

        if (socketMngPtr->pcbPtr != NULL_PTR)
        {
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
            Udp_SocketUsedNum++;
            socketMngPtr->protocol = TCPIP_IPPROTO_UDP;
            /* output parameter setting */
            *socketIdPtr = TcpIp_SocketLastId;
            /* link list handle */
            TcpIp_SocketLastId = socketMngPtr->nextSocketId;
            socketMngPtr->nextSocketId = TCPIP_SOCKET_INVALIDVLU;
            /* comm unit fill data */
            socketMngPtr->ownerCfgPtr = socketOwnerCfgPtr;
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_ALL);
            ret = E_OK;
        }
    }

    return ret;
}

#endif /* (STD_ON == TCPIP_UDP_ENABLED) */

#if (TCPIP_CONTROLLER_NUM > 0)
/**
 * @ingroup tcpip_internal
 * controller request state handle,call by TcpIp_RequestComMode
 * @param ctrlIdx inner controller index
 * @param reqState @ref TcpIp_StateType
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerRequestComMode(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    VAR(TcpIp_StateType, AUTOMATIC) reqState)
{
    Std_ReturnType ret = E_OK;
    TcpIp_StateType* ctrlState = &TcpIp_ControllerVar[ctrlIdx].ctrlStatus;
    struct netif* netifPtr = &TcpIp_ControllerVar[ctrlIdx].netifVar;

    switch (*ctrlState)
    {
        case TCPIP_STATE_ONLINE:

            /* [SWS_TCPIP_00076] */
            if (reqState == TCPIP_STATE_ONHOLD)
            {
                /* (a) notify the upper layer via Up_LocalIpAddrAssignmentChg() with State TCPIP_IPADDR_STATE_ONHOLD
                for all assigned IP addresses of the related EthIf controller, and */
                TcpIp_CtrlLinkLocalAddrChagneHandle(ctrlIdx, TCPIP_IPADDR_STATE_ONHOLD, TRUE);
                /* (b) deactivate the communication within the TcpIp module for the related EthIf controller, and */
                /* communication shall check EthIf controller state is ONLINE,so this require not handled this here */
                /* (c) enter the state TCPIP_STATE_ONHOLD for the EthIf controller. */
                *ctrlState = TCPIP_STATE_ONHOLD;
                TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_ONHOLD);
            }
            /* [SWS_TCPIP_00077] */
            else if (TCPIP_STATE_OFFLINE == reqState)
            {
                /* (a) call Up_LocalIpAddrAssignmentChg() with State TCPIP_IPADDR_STATE_UNASSIGNED for all assigned IP
                addresses of the related EthIf controller */
                /* TcpIp_UnasignedCtrlIpAssigemt shall notify TCPIP_IPADDR_STATE_UNASSIGNED */
                /* (b) deactivate the communication within the TcpIp module for the related EthIf controller */
                /* communication shall check EthIf controller state is ONLINE,so this require not handled this here */
                /* (c) release related resources, i.e. any socket using the EthIf controller shall be closed and
                thereafter any IP address assigned to the EthIf controller shall be unassigned, */
                uintx pendCnt = TcpIp_CloseAllSocket(ctrlIdx);
                TcpIp_UnasignedCtrlIpAssigemt(ctrlIdx);
                /* (d) in case the no EthIf controller is assigned any more, all unbound sockets shall be released as
                well, and */
                /* (e) enter the state TCPIP_STATE_SHUTDOWN for the EthIf controller. */
                *ctrlState = TCPIP_STATE_SHUTDOWN;
                TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_SHUTDOWN);

                if (0u == pendCnt)
                {
                    /* [SWS_TCPIP_00087] */
                    *ctrlState = TCPIP_STATE_OFFLINE;
                    /* call lwip netif api */
                    netif_set_down(netifPtr);
                    netif_set_link_down(netifPtr);
                    TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_OFFLINE);
                }
                else
                {
                    TCPIP_CTRL_SET_STATE_PENDING(&TcpIp_ControllerVar[ctrlIdx], TCPIP_STATE_SHUTDOWN);
                }
            }
            else
            {
                ret = E_NOT_OK;
            }

            break;

        case TCPIP_STATE_ONHOLD:

            /* [SWS_TCPIP_00086] */
            if (TCPIP_STATE_ONLINE == reqState)
            {
                /* (a) reactivate the communication within the TcpIp module for the related EthIf controller, */
                /* communication shall check EthIf controller state is ONLINE,so this require not handled this here */
                /* (b) call Up_LocalIpAddrAssignmentChg() with State TCPIP_IPADDR_STATE_ASSIGNED for all assigned IP
                addresses of the related EthIf controller, and */
                TcpIp_CtrlLinkLocalAddrChagneHandle(ctrlIdx, TCPIP_IPADDR_STATE_ASSIGNED, TRUE);
                /* (c) enter the state TCPIP_STATE_ONLINE for the EthIf controller. */
                *ctrlState = TCPIP_STATE_ONLINE;
                TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_ONLINE);
            }
            /* [SWS_TCPIP_00077] */
            else if (TCPIP_STATE_OFFLINE == reqState)
            {
                /* (a) call Up_LocalIpAddrAssignmentChg() with State TCPIP_IPADDR_STATE_UNASSIGNED for all assigned IP
                addresses of the related EthIf controller */
                /* TcpIp_UnasignedCtrlIpAssigemt shall notify TCPIP_IPADDR_STATE_UNASSIGNED */
                /* (b) deactivate the communication within the TcpIp module for the related EthIf controller */
                /* communication shall check EthIf controller state is ONLINE,so this require not handled this here */
                /* (c) release related resources, i.e. any socket using the EthIf controller shall be closed and
                thereafter any IP address assigned to the EthIf controller shall be unassigned, */
                uintx pendCnt = TcpIp_CloseAllSocket(ctrlIdx);
                TcpIp_UnasignedCtrlIpAssigemt(ctrlIdx);
                /* (d) in case the no EthIf controller is assigned any more, all unbound sockets shall be released as
                well, and */
                /* (e) enter the state TCPIP_STATE_SHUTDOWN for the EthIf controller. */
                *ctrlState = TCPIP_STATE_SHUTDOWN;
                TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_SHUTDOWN);

                if (0u == pendCnt)
                {
                    /* [SWS_TCPIP_00087] */
                    *ctrlState = TCPIP_STATE_OFFLINE;
                    /* call lwip netif api */
                    netif_set_down(netifPtr);
                    netif_set_link_down(netifPtr);
                    /* notify ethsm */
                    TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_OFFLINE);
                }
                else
                {
                    TCPIP_CTRL_SET_STATE_PENDING(&TcpIp_ControllerVar[ctrlIdx], TCPIP_STATE_SHUTDOWN);
                }
            }
            else
            {
                ret = E_NOT_OK;
            }

            break;

        case TCPIP_STATE_OFFLINE:

            /* [SWS_TCPIP_00075] */
            if (reqState == TCPIP_STATE_ONLINE)
            {
#if TCPIP_REPEAT_GET_NETIF_MAC
                TcpIp_NetIfGetPhyAddrFormEthIf(netifPtr);
#endif/* TCPIP_REPEAT_GET_NETIF_MAC */
                /* set lwip netif is used by upper layer */
                netif_set_up(netifPtr);
                /* (b) enter the state TCPIP_STATE_STARTUP for the EthIf controller */
                *ctrlState = TCPIP_STATE_STARTUP;
                TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_STARTUP);
                /* (a) enable all IP address assignments according to the configured assignment methods
                (TcpIpAssignmentMethod) and triggers (TcpIpAssignmentTrigger) for that EthIf controller */
                /* only assignment trigger is configured to TCPIP_AUTOMATIC,shall be handle ip assigement */
                TcpIp_RequestCtrlIpAssignment(ctrlIdx);
                /* open netif link ,info:netif up and link up shall trg arp send*/
                netif_set_link_up(netifPtr);
                ret = E_OK;
            }
            else
            {
                ret = E_NOT_OK;
            }

            break;

        case TCPIP_STATE_STARTUP:

            /* [SWS_TCPIP_00088] */
            if (reqState == TCPIP_STATE_OFFLINE)
            {
                /* a:isAbort all ongoing IP address assignment actions appropriate and */
                TcpIp_UnasignedCtrlIpAssigemt(ctrlIdx);
                /* b:enter the state TCPIP_STATE_OFFLINE for the EthIf controller */
                *ctrlState = TCPIP_STATE_OFFLINE;
                /* call lwip netif api */
                netif_set_down(netifPtr);
                netif_set_link_down(netifPtr);
                TcpIp_ControlStateNotifyEthSm(ctrlIdx, TCPIP_STATE_OFFLINE);
            }
            else
            {
                ret = E_NOT_OK;
            }

            break;

        default:
            ret = E_NOT_OK;
            /* TCPIP_STATE_SHUTDOWN */
            break;
    }

    return ret;
}
#endif /* (TCPIP_CONTROLLER_NUM > 0) */

#if (TCPIP_LOCAL_ADR_NUM > 0u)
/**
 * @ingroup tcpip_internal
 * local address inner data ip address setting,call by TcpIp_RequestIpAddrAssignment
 * @param localAdrIndx local address index
 * @param localAdrCfgPtr local address post build configuration data @ref TcpIp_LocalAddrType
 * @param ipAdrPtr the address information to be write
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SetLocalAdrIpVar(
    VAR(uint8, AUTOMATIC) localAdrIndx,
    P2CONST(TcpIp_LocalAddrType, AUTOMATIC, TCPIP_APPL_DATA) localAdrCfgPtr,
    P2CONST(TcpIp_StaticIpAddressConfigType, AUTOMATIC, TCPIP_APPL_DATA) ipAdrPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != ipAdrPtr) && (NULL_PTR != ipAdrPtr->StaticIpAddressPtr))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_AddressType adrType = localAdrCfgPtr->AddrType;
        TcpIp_LocalAdrHandleType* localAdtPtr = &TcpIp_LocalAdrTable[localAdrIndx];
        TCPIP_LOCAL_TO_LWIP_IPADDR(localAdtPtr->ipAdrSrc.addr, ipAdrPtr->StaticIpAddressPtr);

        if (NULL_PTR != ipAdrPtr->DefaultRouterPtr)
        {
            TCPIP_LOCAL_TO_LWIP_IPADDR(localAdtPtr->defaultrouter.addr, ipAdrPtr->DefaultRouterPtr);
        }

        if (NULL_PTR != ipAdrPtr->NetmaskPtr)
        {
            TCPIP_LOCAL_TO_LWIP_IPADDR(localAdtPtr->netmask.addr, ipAdrPtr->NetmaskPtr);
        }

        if (TCPIP_UNICAST == adrType)
        {
            TcpIp_ControllerStatus* ctrlVarPtr = &TcpIp_ControllerVar[localAdrCfgPtr->ctrlIndex];
            TCPIP_LOCALADR_SET_REQUESTIP(localAdtPtr);
            struct netif* netif = &(ctrlVarPtr->netifVar);
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
            const TcpIp_CtrlType* ctrlCfgPtr = &ControlCfgPtr[localAdrCfgPtr->ctrlIndex];

            if (TCPIP_CTRL_ALL_LOCAL_IS_RELEASE(ctrlVarPtr))
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
            {
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
                TCPIP_SET_CTRL_LOCAL_MASK(ctrlVarPtr, localAdrIndx - ctrlCfgPtr->TcpIpLocalAddrStartId);
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
                netif_set_addr(netif, &localAdtPtr->ipAdrSrc, &localAdtPtr->netmask, &localAdtPtr->defaultrouter);
            }

#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
            else
            {
                TCPIP_SET_CTRL_LOCAL_MASK(ctrlVarPtr, localAdrIndx - ctrlCfgPtr->TcpIpLocalAddrStartId);
                TCPIP_LOCALADR_SET_IPVALID(localAdtPtr);
                TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
                TcpIp_NotfiyUpperLayerIpAdrChange((TcpIp_LocalAddrIdType)localAdrIndx, TCPIP_IPADDR_STATE_ASSIGNED);
            }

#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
            ret = E_OK;
        }
        else if (TCPIP_MULTICAST == adrType)
        {
            TCPIP_LOCALADR_SET_REQUESTIP(localAdtPtr);
            TcpIp_MultCastIpAssigmentHandle(localAdrIndx, localAdrCfgPtr->CtrlRef, TRUE);
            ret = E_OK;
        }
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("only support unicast and multcast,\
                                        other config is error localAdrIndx=%d\n", localAdrIndx);
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input parameters is error");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}

/**
 * @ingroup tcpip_internal
 * local address inner data ip address clear,call by TcpIp_ReleaseIpAddrAssignment
 * @param localAdrIndx local address index
 * @param ctrlIdex local controller index
 * @param assignmentType @ref TcpIp_IpAddrAssignmentType
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ClearLocalAdrIpVar(
    VAR(uintx, AUTOMATIC) localAdrIndx,
    VAR(uintx, AUTOMATIC) ctrlIdex,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) assignmentType)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_LocalAdrHandleType* localAdtPtr = &TcpIp_LocalAdrTable[localAdrIndx];
    TCPIP_LOCALADR_CLR_IPVALID(localAdtPtr);
    TcpIp_AddressType addrType = LocalAdrCfgPtr[localAdrIndx].AddrType;
    TCPIP_LOCALADR_SET_REQUESTIP(localAdtPtr);

    /* shall clear netif ipaddress */
    switch (assignmentType)
    {
        case TCPIP_IPADDR_ASSIGNMENT_STATIC:
#if !TCPIP_ALLOW_RX_INVALID_LOCAL_IP
            localAdtPtr->ipAdrSrc.addr = TCPIP_IPADDR_ANY;
            localAdtPtr->defaultrouter.addr = TCPIP_IPADDR_ANY;
            localAdtPtr->netmask.addr = TCPIP_IPADDR_ANY;
#endif/* !TCPIP_ALLOW_RX_INVALID_LOCAL_IP */

            if (TCPIP_UNICAST == addrType)
            {
                TcpIp_ControllerStatus* ctrlVarPtr = &TcpIp_ControllerVar[ctrlIdex];
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
                const TcpIp_CtrlType* ctrlCfgPtr = &ControlCfgPtr[ctrlIdex];
                TCPIP_CLR_CTRL_LOCAL_MASK(ctrlVarPtr, localAdrIndx - ctrlCfgPtr->TcpIpLocalAddrStartId);

                /* all local address is release shall be set netif addr is null in this control */
                if (TCPIP_CTRL_ALL_LOCAL_IS_RELEASE(ctrlVarPtr))
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
                {
                    netif_set_addr(&(ctrlVarPtr->netifVar), NULL_PTR, NULL_PTR, NULL_PTR);
                }
            }
            else if (TCPIP_MULTICAST == addrType)
            {
                TcpIp_MultCastIpAssigmentHandle(localAdrIndx,
                                                LocalAdrCfgPtr[localAdrIndx].CtrlRef,
                                                FALSE);
            }
            else
            {
                TCPIP_INNER_CHECK_OUTPUT("only support unicast and multcast,\
                                        other config is error localAdrIndex=%d\n", localAdrIndx);
                ret = E_NOT_OK;
            }

            break;

        case TCPIP_IPADDR_ASSIGNMENT_DHCP:
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
            ret = TcpIp_StopDhcp(ctrlIdex);
#endif/* (STD_ON == TCPIP_DHCP_CLIENT_ENABLED) */
            break;

        case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP:
        case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL:
#if(STD_ON == TCPIP_AUTOIP_ENABLED)
            ret = TcpIp_StopAutoIp(ctrlIdex);
#endif/* (STD_ON == TCPIP_AUTOIP_ENABLED) */
            break;

        default:
            ret = E_NOT_OK;
            break;
    }

    if (E_OK != ret)
    {
        TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
    }

    return ret;
}

/**
 * @ingroup tcpip_internal
 * automatic assignment ip address handle
 * @param localAdrIndx local address index
 * @param localAdrCfgPtr local address post build configuration data point
 * @param addAssCfgPtr ip address assignment post build configuration data point
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_AutoMaticAssignmentIpByLocal(
    VAR(uintx, AUTOMATIC)  localAdrIndex,
    P2CONST(TcpIp_LocalAddrType, AUTOMATIC, TCPIP_APPL_DATA) localAdrCfgPtr,
    P2CONST(TcpIp_AddrAssignmentType, AUTOMATIC, TCPIP_APPL_DATA) addAssCfgPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_IpAddrAssignmentType assignmentMethod = addAssCfgPtr->AssignmentMethod;
    const TcpIp_StaticIpAddressConfigType* ipAdrPtr = localAdrCfgPtr->StaticIpAddressCfgPtr;
    TcpIp_LocalAdrHandleType* localAdtPtr = &TcpIp_LocalAdrTable[localAdrIndex];
    TCPIP_LOCALADR_SET_REQUESTIP(localAdtPtr);

    switch (assignmentMethod)
    {
        case TCPIP_IPADDR_ASSIGNMENT_STATIC:
            if (NULL_PTR != ipAdrPtr)
            {
                /* only support ipv4 */
                /* inner local address shall copy from configure datas */
                TCPIP_LOCAL_TO_LWIP_IPADDR(localAdtPtr->ipAdrSrc.addr, ipAdrPtr->StaticIpAddressPtr);

                if (NULL_PTR != ipAdrPtr->DefaultRouterPtr)
                {
                    TCPIP_LOCAL_TO_LWIP_IPADDR(localAdtPtr->defaultrouter.addr, ipAdrPtr->DefaultRouterPtr);
                }

                if (NULL_PTR != ipAdrPtr->NetmaskPtr)
                {
                    TCPIP_LOCAL_TO_LWIP_IPADDR(localAdtPtr->netmask.addr, ipAdrPtr->NetmaskPtr);
                }

                TcpIp_ControllerStatus* ctrlVarPtr = &TcpIp_ControllerVar[localAdrCfgPtr->ctrlIndex];
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
                const TcpIp_CtrlType* ctrlCfgPtr = &ControlCfgPtr[localAdrCfgPtr->ctrlIndex];
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
                struct netif* netif = &(ctrlVarPtr->netifVar);
                ret = E_OK;

                /* unicast shall wirte netif */
                if (TCPIP_UNICAST == localAdrCfgPtr->AddrType)
                {
                    /* multiple ip only first local addr write to netif,other local addr direct notify upper layer */
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
                    if (TCPIP_CTRL_ALL_LOCAL_IS_RELEASE(ctrlVarPtr))
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
                    {
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
                        TCPIP_SET_CTRL_LOCAL_MASK(ctrlVarPtr, localAdrIndex - ctrlCfgPtr->TcpIpLocalAddrStartId);
#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
                        netif_set_addr(netif, &localAdtPtr->ipAdrSrc,
                                       &localAdtPtr->netmask, &localAdtPtr->defaultrouter);
                    }

#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
                    else
                    {
                        TCPIP_SET_CTRL_LOCAL_MASK(ctrlVarPtr, localAdrIndex - ctrlCfgPtr->TcpIpLocalAddrStartId);
                        TCPIP_LOCALADR_SET_IPVALID(localAdtPtr);
                        TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
                        TcpIp_NotfiyUpperLayerIpAdrChange((TcpIp_LocalAddrIdType)localAdrIndex,
                                                          TCPIP_IPADDR_STATE_ASSIGNED);
                    }

#endif/* (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL) */
                }
                else if (TCPIP_MULTICAST == localAdrCfgPtr->AddrType)
                {
                    /* multicast shall set ip valid ? */
                    TcpIp_MultCastIpAssigmentHandle(localAdrIndex, localAdrCfgPtr->CtrlRef, TRUE);
                }
                else
                {
                    TCPIP_INNER_CHECK_OUTPUT("only support unicast and multcast,\
                                        other config is error localAdrIndex=%d\n", localAdrIndex);
                    ret = E_NOT_OK;
                }
            }
            else
            {
                TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
            }

            break;

        case TCPIP_IPADDR_ASSIGNMENT_DHCP:
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
            ret = TcpIp_StartDhcp(localAdrCfgPtr->ctrlIndex);
#endif/* (STD_ON == TCPIP_DHCP_CLIENT_ENABLED) */
            break;

        case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP:
        case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL:
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
            ret = TcpIp_StartAutoIp(localAdrCfgPtr->ctrlIndex);
#endif/* (STD_ON == TCPIP_AUTOIP_ENABLED) */
            break;

        default:                                                                                                        /* PQSA S 2016 */
            break;
    }

    if ((Std_ReturnType)E_OK != ret)
    {
        TCPIP_LOCALADR_CLR_REQUESTIP(localAdtPtr);
    }

    return ret;
}

#endif/* TCPIP_LOCAL_ADR_NUM > 0u */


#if (TCPIP_LOCAL_ADR_NUM > 0u)
/**
 * @ingroup tcpip_internal
 * get local address inner ip infomation,call by TcpIp_GetIpAddr
 * @param localAdrIndx local address index
 * @param ipAddrPtr Pointer to a struct where the IP address
 * @param netmaskPtr Network mask
 * @param defaultRouterPtr default router(gateway)
 * @return E_OK
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerGetIpAddr(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,                                                    /* PRQA S 3432 */
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) netmaskPtr,                                                   /* PRQA S 3432 */
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) defaultRouterPtr)                                             /* PRQA S 3432 */
{
    Std_ReturnType ret = E_OK;
    const TcpIp_LocalAdrHandleType* localAdtPtr = &TcpIp_LocalAdrTable[localAdrIndx];
    TCPIP_LWIP_TO_LOCAL_IPADDR(ipAddrPtr->addr, localAdtPtr->ipAdrSrc.addr);
    TCPIP_LWIP_TO_LOCAL_IPADDR(netmaskPtr->addr, localAdtPtr->netmask.addr);
    TCPIP_LWIP_TO_LOCAL_IPADDR(defaultRouterPtr->addr, localAdtPtr->defaultrouter.addr);
    return ret;
}
#endif/* (TCPIP_LOCAL_ADR_NUM > 0u) */

#if ((TCPIP_LOCAL_ADR_NUM > 0u) && (TCPIP_SOCKET_NUM > 0u))
/**
 * @ingroup tcpip_internal
 * get local address inner ip infomation,call by TcpIp_Bind
 * @param ctrlIdx local controller index
 * @param socketId socket index
 * @param localAdrIndx local address index
 * @param portPtr socket local port,if TCPIP_PORT_ANY ,shall get automatically assignment port
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerBind(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) portPtr)                                                                  /* PRQA S 3432 */
{
    Std_ReturnType ret = E_NOT_OK;
    /* TCPIP_LOCALADDRID_ANY */
    TcpIp_LocalAdrHandleType* localAdtPtr = NULL_PTR ;

    if (localAdrIndx < TCPIP_LOCAL_ADR_NUM)
    {
        localAdtPtr = &TcpIp_LocalAdrTable[localAdrIndx];
    }

    TcpIp_ControllerStatus* ctrlMngPtr = &TcpIp_ControllerVar[ctrlIdx];

    /*[SWS_TCPIP_00147] first check local address is valid */
    if ((TCPIP_STATE_ONLINE == ctrlMngPtr->ctrlStatus)
        && ((localAdtPtr == NULL_PTR) || (TCPIP_LOCALADR_IS_IPVALID(localAdtPtr))))
    {
        err_t lw_ret = (err_t)ERR_ARG;
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

        if (NULL_PTR != socketMngPtr->pcbPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
        {
            ip_addr_t* bindIpSrcPtr = NULL_PTR;

            if (localAdtPtr != NULL_PTR)
            {
                bindIpSrcPtr = &localAdtPtr->ipAdrSrc;
            }

#if (STD_ON == TCPIP_TCP_ENABLED)

            if (TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
            {
                struct tcp_pcb* tpcb = (struct tcp_pcb*)socketMngPtr->pcbPtr;
                lw_ret = (err_t)tcp_bind(tpcb, bindIpSrcPtr, *portPtr);

                if (((err_t)ERR_OK == lw_ret) && (TCPIP_PORT_ANY == *portPtr))
                {
                    *portPtr = tpcb->local_port;
                }

                /* Preprocess to avoid calling ip_route */
                tcp_bind_netif(tpcb, &ctrlMngPtr->netifVar);
            }

#endif /* STD_ON == TCPIP_TCP_ENABLED */
#if (STD_ON == TCPIP_UDP_ENABLED)

            if (TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
            {
                struct udp_pcb* upcb = (struct udp_pcb*)socketMngPtr->pcbPtr;
                lw_ret = (err_t)udp_bind(upcb, bindIpSrcPtr, *portPtr);

                if (((err_t)ERR_OK == lw_ret) && (TCPIP_PORT_ANY == *portPtr))
                {
                    *portPtr = upcb->local_port;
                }

                /* Preprocess to avoid calling ip_route */
                udp_bind_netif(upcb, &ctrlMngPtr->netifVar);
                /* register udp rx data callback function and callback input arg data point */
                udp_recv((struct udp_pcb*)socketMngPtr->pcbPtr,
                         TcpIp_UdpRecvCallcback,
                         socketMngPtr);
            }

#endif/* (STD_ON == TCPIP_UDP_ENABLED) */

            if ((err_t)ERR_OK == lw_ret)
            {
                socketMngPtr->ctrlIndex = ctrlIdx;
                ctrlMngPtr->socketNum++;
                ret = E_OK;
            }

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
            else if ((err_t)ERR_USE == lw_ret)
            {
                /* [SWS_TCPIP_00147]
                another socket already bound to the same port, protocol and local address  */
                TCPIP_DET(TCPIP_SID_BIND, TCPIP_E_ADDRINUSE);
                TCPIP_INNER_CHECK_OUTPUT("another socket already bound to the same port, protocol and local address\n");
            }

#endif /* (STD_ON == TCPIP_DEV_ERROR_DETECT) */
            else
            {
#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
                TCPIP_DET(TCPIP_SID_BIND, TCPIP_E_ADDRNOTAVAIL);
#endif /*STD_ON == TCPIP_DEV_ERROR_DETECT*/
                TCPIP_INNER_CHECK_OUTPUT("bind is error:%d\n", lw_ret);
            }
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("pcb data point is null");
        }

#endif/* defined(TCPIP_INNER_CHECK) */
    }

    return ret;
}
#endif/* ((TCPIP_LOCAL_ADR_NUM > 0u)&&(TCPIP_SOCKET_NUM > 0u)) */

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @ingroup tcpip_internal
 * upd transmit data,dataPtr is not nullptr call by TcpIp_UdpTransmit
 * @param socketId socket index
 * @param dataPtr  Pointer to a linear buffer of TotalLength bytes containing the data to be transmitted.
 * @param remoteAddrPtr ip address and port of the remote host to transmit to
 * @param TotalLength indicates the payload size of the UDP datagram
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerUdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(uint16, AUTOMATIC) totalLength)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];

    if ((TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
        && (NULL_PTR != socketMngPtr->pcbPtr))
    {
        ip_addr_t dst_ip;
        u16_t dst_port = remoteAddrPtr->port;
        TCPIP_LOCAL_TO_LWIP_IPADDR(dst_ip.addr, remoteAddrPtr->addr);
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        struct pbuf* udpTxPbuf = pbuf_alloc(PBUF_TRANSPORT, totalLength, PBUF_REF);
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#if defined(TCPIP_INNER_CHECK)

        if (NULL_PTR != udpTxPbuf)
#endif/* defined(TCPIP_INNER_CHECK) */
        {
            udpTxPbuf->payload = (void*)dataPtr;                                                                        /* PRQA S 0311 */
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            err_t lw_ret = (err_t)udp_sendto((struct udp_pcb*)socketMngPtr->pcbPtr, udpTxPbuf, &dst_ip, dst_port);
            (void)pbuf_free(udpTxPbuf);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);

            if ((err_t)ERR_OK == lw_ret)
            {
                ret = E_OK;
            }

#if defined(TCPIP_INNER_CHECK)
            else
            {
                TCPIP_INNER_CHECK_OUTPUT("call udp_sendto return error:%d\n", lw_ret);
            }

#endif/* defined(TCPIP_INNER_CHECK) */
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("socket id:%d error\n", socketId);
        }

#endif/* defined(TCPIP_INNER_CHECK) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("pbuf alloc error\n");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}

/**
 * @ingroup tcpip_internal
 * upd transmit data,retrieve data from upper layer via callback <Up>_CopyTxData(),dataPtr is
 * nullptr call by TcpIp_UdpTransmit
 * @param socketId socket index
 * @param remoteAddrPtr ip address and port of the remote host to transmit to
 * @param totalLength indicates the payload size of the UDP datagram
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerCopyUdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(uint16, AUTOMATIC) totalLength)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];

    if ((TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
        && (NULL_PTR != socketMngPtr->pcbPtr)
#if defined(TCPIP_INNER_CHECK)
        && (NULL_PTR != socketMngPtr->ownerCfgPtr)
        && (NULL_PTR != socketMngPtr->ownerCfgPtr->Up_CopyTxDataFuncPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
       )
    {
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        struct pbuf* udpTxPbuf = pbuf_alloc(PBUF_TRANSPORT, totalLength, PBUF_RAM);
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#if defined(TCPIP_INNER_CHECK)

        if ((NULL_PTR != udpTxPbuf) && (udpTxPbuf->len == udpTxPbuf->tot_len))
#endif/* defined(TCPIP_INNER_CHECK) */
        {
            TcpIp_Up_SocketOwnerCopyTxDataType copyFuncPtr = socketMngPtr->ownerCfgPtr->Up_CopyTxDataFuncPtr;
            BufReq_ReturnType bufRet = copyFuncPtr(socketId, (uint8*)udpTxPbuf->payload, udpTxPbuf->len);

            if (BUFREQ_OK == bufRet)
            {
                ip_addr_t dst_ip;
                u16_t dst_port = remoteAddrPtr->port;
                TCPIP_LOCAL_TO_LWIP_IPADDR(dst_ip.addr, remoteAddrPtr->addr);
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                err_t lw_ret = (err_t)udp_sendto((struct udp_pcb*)socketMngPtr->pcbPtr, udpTxPbuf, &dst_ip, dst_port);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);

                if ((err_t)ERR_OK == lw_ret)
                {
                    ret = E_OK;
                }

#if defined(TCPIP_INNER_CHECK)
                else
                {
                    TCPIP_INNER_CHECK_OUTPUT("call udp_sendto return error:%d\n", lw_ret);
                }

#endif/* defined(TCPIP_INNER_CHECK) */
            }

#if defined(TCPIP_INNER_CHECK)
            else
            {
                TCPIP_INNER_CHECK_OUTPUT("CopyTxData return error=%d\n", bufRet);
            }

#endif/* defined(TCPIP_INNER_CHECK) */
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("pbuf_alloc error,length=%d\n", totalLength);
        }

#endif/* defined(TCPIP_INNER_CHECK) */
        /* free buffer */
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        (void)pbuf_free(udpTxPbuf);
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("socket id:%d is error!\n", socketId);
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */


#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @ingroup tcpip_internal
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpTransmit
 * @param socketId socket index
 * @param dataPtr  Pointer to a linear buffer of TotalLength bytes containing the data to be transmitted.
 * @param totalLength indicates the payload size of the TCP datagram
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerTcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(uint16, AUTOMATIC) totalLength)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];

    if ((TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
        && (NULL_PTR != socketMngPtr->pcbPtr))
    {
        err_t lw_ret;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

        if (NULL_PTR != socketMngPtr->tlsPtr)
        {
            lw_ret = (err_t)TcpIp_TlsWrite(socketMngPtr, dataPtr, totalLength);
        }
        else
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
        {
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            /* waring:input parameters 'totalLength' shall less than tcp_pcb->snd_buf */
            lw_ret = (err_t)tcp_write(
                         (struct tcp_pcb*)socketMngPtr->pcbPtr,
                         dataPtr, (u16_t)totalLength, TCPIP_WRITE_FILL_FLAG);
#if TCPIP_FASTTX_TCP
            lw_ret = tcp_output((struct tcp_pcb*)socketMngPtr->pcbPtr);
#endif/* TCPIP_FASTTX_TCP */
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        }

        if ((err_t)ERR_OK == lw_ret)
        {
            ret = E_OK;
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("call tcp_write return error:%d", lw_ret);
        }

#endif/* defined(TCPIP_INNER_CHECK) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("socket id:%d is error!\n", socketId);
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}
/**
 * @ingroup tcpip_internal
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpTransmit
 * @param socketId socket index
 * @param totalLength indicates the payload size of the TCP datagram
 * @param forceRetrieve  Indicates how the TCP/IP stack retrieves data from upper layer.
 *          TRUE: the whole data indicated by availableLength shall be retrieved from the upper layer via
 *                      one or multiple <Up>_CopyTxData() calls within the context of this transmit function.
 *          FALSE: the TCP/IP stack may retrieve up to availableLength data from the upper layer. It is
 *                      allowed to retrieve less than availableLength bytes. Note: Not retrieved data will be provided
 *                      by upper layer with the next call to TcpIp_TcpTransmit (along with new data if available).
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerCopyTcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(uint16, AUTOMATIC) totalLength,
    VAR(boolean, AUTOMATIC) forceRetrieve)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];

    if ((TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
        && (NULL_PTR != socketMngPtr->pcbPtr)
#if defined(TCPIP_INNER_CHECK)
        && (NULL_PTR != socketMngPtr->ownerCfgPtr)
        && (NULL_PTR != socketMngPtr->ownerCfgPtr->Up_CopyTxDataFuncPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
       )
    {
        struct tcp_pcb* tcpPcbPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
        uint16 copyLen = (totalLength > tcpPcbPtr->snd_buf) ? tcpPcbPtr->snd_buf : totalLength;
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        struct pbuf* tcpTxPbuf = pbuf_alloc(PBUF_RAW, copyLen, PBUF_RAM);
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#if defined(TCPIP_INNER_CHECK)

        if (NULL_PTR != tcpTxPbuf)
#endif/* defined(TCPIP_INNER_CHECK) */
        {
            if (tcpTxPbuf->tot_len != tcpTxPbuf->len)
            {
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                tcpTxPbuf = pbuf_coalesce(tcpTxPbuf, PBUF_RAW);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            }

            TcpIp_Up_SocketOwnerCopyTxDataType copyFuncPtr = socketMngPtr->ownerCfgPtr->Up_CopyTxDataFuncPtr;
            BufReq_ReturnType bufRet = copyFuncPtr(socketId, (uint8*)tcpTxPbuf->payload, tcpTxPbuf->len);
            err_t lw_ret = (err_t) ERR_OK;

            if (BUFREQ_OK == bufRet)
            {
                if (((boolean)FALSE == forceRetrieve) || (((boolean)TRUE == forceRetrieve) && (copyLen == totalLength)))
                {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

                    if (NULL_PTR != socketMngPtr->tlsPtr)
                    {
                        lw_ret = TcpIp_TlsWrite(socketMngPtr, tcpTxPbuf->payload, tcpTxPbuf->tot_len);
                    }
                    else
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
                    {
                        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                        lw_ret = tcp_write(tcpPcbPtr, tcpTxPbuf->payload, tcpTxPbuf->tot_len, TCPIP_WRITE_FILL_FLAG);
#if TCPIP_FASTTX_TCP
                        lw_ret = tcp_output(tcpPcbPtr);
#endif/* TCPIP_FASTTX_TCP */
                        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                    }

                    /* call low layer transmit, shall realse pbuf ,whether it's successful or not */
                    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                    (void)pbuf_free(tcpTxPbuf);
                    tcpTxPbuf = NULL_PTR;
                    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                }
            }
            else
            {
                /* upper layer copy tx data return BUFREQ_E_NOT_OK,shall release local buff */
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                (void)pbuf_free(tcpTxPbuf);
                tcpTxPbuf = NULL_PTR;
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                lw_ret = (err_t)ERR_ARG;
#if defined(TCPIP_INNER_CHECK)
                TCPIP_INNER_CHECK_OUTPUT("CopyTxData return error=%d\n", bufRet);
#endif/* defined(TCPIP_INNER_CHECK) */
            }

            if ((err_t)ERR_OK == lw_ret)
            {
                socketMngPtr->txBuf = tcpTxPbuf;
                socketMngPtr->txRemainLen = (uint32)totalLength - (uint32)copyLen;
                TCPIP_TCP_SET_ZFORCERETRIEVE(socketMngPtr, forceRetrieve);
                ret = E_OK;
            }
        }

#if defined(TCPIP_INNER_CHECK)
        else
        {
            TCPIP_INNER_CHECK_OUTPUT("pbuf_alloc return null,length=%d!\n", totalLength);
        }

#endif/* defined(TCPIP_INNER_CHECK) */
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("socket id:%d is error!\n", socketId);
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}
/**
 * @ingroup tcpip_internal
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpConnect
 * @param socketId socket index
 * @param remoteAddrPtr IP address and port of the remote host to connect to
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerTcpConnect(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
    struct tcp_pcb* tcpPcbPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != tcpPcbPtr) && (TCPIP_IPPROTO_TCP == socketMngPtr->protocol))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        /* [SWS_TCPIP_00094]  The TcpIp module shall only accept new TCP connections if the related EthIf controller is
        in state TCPIP_STATE_ONLINE */
        if (TCPIP_STATE_ONLINE == TcpIp_ControllerVar[socketMngPtr->ctrlIndex].ctrlStatus)
        {
            ip_addr_t reIp = {0};
            TCPIP_LOCAL_TO_LWIP_IPADDR(reIp.addr, remoteAddrPtr->addr);
            u16_t port = remoteAddrPtr->port;
            tcp_arg(tcpPcbPtr, socketMngPtr);
            /* reg connect error callback function */
            tcp_err(tcpPcbPtr, TcpIp_TcpErrCallback);
            err_t lw_ret;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

            if (NULL_PTR == socketMngPtr->tlsPtr)
            {
                /* static tls connection assignment */
                TcpIp_TlsBaseStaticConnect(socketMngPtr, remoteAddrPtr, TLS_CLIENT);
            }

            /* shall support static and dynamic tls connect */
            if (NULL_PTR != socketMngPtr->tlsPtr)
            {
                lw_ret = TcpIp_TlsConnet(tcpPcbPtr, &reIp, port);
            }
            else
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
                /* connect handle shell protect context */
            {
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                lw_ret = (err_t)tcp_connect(tcpPcbPtr, &reIp, port, TcpIp_TcpConnnectCallback);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            }

            if ((err_t)ERR_OK == lw_ret)
            {
                ret = E_OK;
            }

#if defined(TCPIP_INNER_CHECK)
            else
            {
                TCPIP_INNER_CHECK_OUTPUT("call tcp_connect return error:%d\n", lw_ret);
            }

#endif/* defined(TCPIP_INNER_CHECK) */
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("pcb data point is null or is not tcp protocol");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}
/**
 * @ingroup tcpip_internal
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpListen
 * @param socketId socket index
 * @param maxCh Maximum number of new parallel connections established on this listen connection
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerTcpListen(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(uint8, AUTOMATIC) maxCh)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
    struct tcp_pcb* tcpPcbPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != tcpPcbPtr) && (TCPIP_IPPROTO_TCP == socketMngPtr->protocol))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        /* [SWS_TCPIP_00094]  The TcpIp module shall only accept new TCP connections if the related EthIf controller is
        in state TCPIP_STATE_ONLINE */
        if (TCPIP_STATE_ONLINE == TcpIp_ControllerVar[socketMngPtr->ctrlIndex].ctrlStatus)
        {
            /* listen pcb shall be remalloc,so lwip pcb free handle not release local val */
            tcp_ext_arg_set(tcpPcbPtr, 0u, NULL_PTR);
#if !defined(TCPIP_INNER_CHECK)
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            socketMngPtr->pcbPtr = tcp_listen_with_backlog(tcpPcbPtr, &err);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#else/* defined(TCPIP_INNER_CHECK) */
            err_t err;
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            socketMngPtr->pcbPtr = tcp_listen_with_backlog_and_err(tcpPcbPtr, maxCh, &err);
            tcpPcbPtr = (struct tcp_pcb*)socketMngPtr->pcbPtr;
            /* is server socket */
            TCPIP_TCP_SET_SERVERSOCKET(socketMngPtr);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            /* register other callback function */
            tcp_ext_arg_set(tcpPcbPtr, 0u, socketMngPtr);

            if (err == (err_t)ERR_OK)
#endif/* defined(TCPIP_INNER_CHECK) */
            {
                /* set accept callback function */
                tcp_accept(tcpPcbPtr, TcpIp_TcpAcceptCallcback);
                ret = E_OK;
            }

#if defined(TCPIP_INNER_CHECK)
            else
            {
                TCPIP_INNER_CHECK_OUTPUT("listen pcb create is error:%d\n", err);
            }

#endif/* defined(TCPIP_INNER_CHECK) */
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("pcb data point is null or is not tcp protocol");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}

/**
 * @ingroup tcpip_internal
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_TcpListen
 * @param socketId socket index
 * @param revcLen Length Number of bytes finally consumed by the upper layer
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerTcpReceived(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(uint16, AUTOMATIC) revcLen)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr->pcbPtr) && (TCPIP_IPPROTO_TCP == socketMngPtr->protocol))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

        if (NULL_PTR != socketMngPtr->tlsPtr)
        {
            ret = TcpIp_TlsRecved(socketMngPtr, revcLen);
        }
        else
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
        {
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            tcp_recved((struct tcp_pcb*)socketMngPtr->pcbPtr, revcLen);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            ret = E_OK;
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("pcb data point is null or is not tcp protocol\n");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
    return ret;
}
#endif/* (STD_ON == TCPIP_TCP_ENABLED) */

/**
 * @ingroup tcpip_internal
 * tcp transmit data,dataPtr is not nullptr call by TcpIp_RxIndication
 * @param ctrlIdx controller index
 * @param frameType frame type of received Ethernet frame
 * @param flag lwip buf stateflag
 * @param dataPtr Pointer to payload of the received Ethernet frame
 * @param lenByte Length of received data
 */
FUNC(void, TCPIP_CODE) TcpIp_InnerRxIndication(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    VAR(Eth_FrameType, AUTOMATIC) frameType,
    VAR(uint8, AUTOMATIC) flag,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
    VAR(uint16, AUTOMATIC) lenByte)
{
    struct pbuf* p = pbuf_alloc(PBUF_RAW, lenByte, PBUF_POOL);
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != p) && (NULL_PTR != dataPtr) && (lenByte > 0u))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if defined(TCPIP_INNER_CHECK)
        err_t lwRet = pbuf_take(p, dataPtr, lenByte);

        if ((err_t)ERR_OK == lwRet)
#else/* !defined(TCPIP_INNER_CHECK) */
        (void)pbuf_take(p, dataPtr, lenByte);

#endif/* defined(TCPIP_INNER_CHECK) */
        {
            struct netif* netifPtr = &TcpIp_ControllerVar[ctrlIdx].netifVar;
            p->if_idx = netif_get_index(netifPtr);                                                                      /* PRQA S 1860 */   /* MISRA Rule 10.4 */
            p->flags |= flag;

            switch (frameType)
            {
#if (STD_ON == TCPIP_IPV4_ENABLED)&&(STD_ON == TCPIP_ARP_ENABLED)

                case ((uint16)ETHTYPE_IP):
                    if ((netifPtr->flags & (uint8)NETIF_FLAG_ETHARP) != 0u)
                    {
                        /* pass to IP layer */
                        (void)ip4_input(p, netifPtr);
                    }
                    else
                    {
                        (void)pbuf_free(p);
                    }

                    break;

                case ((uint16)ETHTYPE_ARP):
                    if ((netifPtr->flags & (uint8)NETIF_FLAG_ETHARP) != 0u)
                    {
                        /* pass p to ARP module */
                        etharp_input(p, netifPtr);
                    }
                    else
                    {
                        (void)pbuf_free(p);
                    }

                    break;
#endif/* (STD_ON == TCPIP_IPV4_ENABLED)&&(STD_ON == TCPIP_ARP_ENABLED) */
#if (STD_ON == TCPIP_IPV6_ENABLED)

                case ((uint16)ETHTYPE_IPV6):
                    /* pass to IPv6 layer */
                    ip6_input(p, netif);
                    break;
#endif/* (STD_ON == TCPIP_IPV6_ENABLED) */

                default:
#if defined(TCPIP_INNER_CHECK)
                    TCPIP_INNER_CHECK_OUTPUT("input frameType not find handle!!\n");
#endif/* defined(TCPIP_INNER_CHECK) */
                    break;
            }
        }
    }

#if defined(TCPIP_INNER_CHECK)
    else
    {
        TCPIP_INNER_CHECK_OUTPUT("input parameters check error\n");
    }

#endif/* defined(TCPIP_INNER_CHECK) */
}

/**
 * @ingroup tcpip_internal
 * change specifies the frame priority for outgoing frames on the socket. call by TcpIp_ChangeParameter
 * @param socketId socket index
 * @param parameterId not used
 * @param paraPtr [uint8]frame priority
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ChanegeSocketParaFramePrio(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr)
{
    TCPIP_UNUSED_ARG(parameterId);
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

    if (NULL_PTR != socketMngPtr->pcbPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        uint8 vluTos = (uint8)(((*((const uint8*)paraPtr)) & 0x07u) << 5u);
#if (STD_ON == TCPIP_TCP_ENABLED)

        if (TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
        {
            ((struct tcp_pcb*)(socketMngPtr->pcbPtr))->tos &= 0x1fu;
            ((struct tcp_pcb*)(socketMngPtr->pcbPtr))->tos |= vluTos;
            ret = E_OK;
        }

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */
#if (STD_ON == TCPIP_UDP_ENABLED)

        if (TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
        {
            ((struct udp_pcb*)(socketMngPtr->pcbPtr))->tos &= 0x1fu;
            ((struct udp_pcb*)(socketMngPtr->pcbPtr))->tos |= vluTos;
            ret = E_OK;
        }

#endif/* (STD_ON == TCPIP_UDP_ENABLED) */
    }

    return ret;
}

/**
 * @ingroup tcpip_internal
 * change specifies the time to live value for outgoing frames on the socket. call by TcpIp_ChangeParameter
 * @param socketId socket index
 * @param parameterId not used
 * @param paraPtr [uint8]time to live value
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ChanegeSocketParaTtl(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr)
{
    TCPIP_UNUSED_ARG(parameterId);
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

    if (NULL_PTR != socketMngPtr->pcbPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
#if (STD_ON == TCPIP_TCP_ENABLED)

        if (TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
        {
            ((struct tcp_pcb*)(socketMngPtr->pcbPtr))->ttl = *((const uint8*)paraPtr);
            ret = E_OK;
        }

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */
#if (STD_ON == TCPIP_UDP_ENABLED)

        if (TCPIP_IPPROTO_UDP == socketMngPtr->protocol)
        {
            ((struct udp_pcb*)(socketMngPtr->pcbPtr))->ttl = *((const uint8*)paraPtr);
            ret = E_OK;
        }

#endif/* (STD_ON == TCPIP_UDP_ENABLED) */
    }

    return ret;
}

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @ingroup tcpip_internal
 * change specifies if the Nagle Algorithm  on the tcp socket. call by TcpIp_ChangeParameter
 * @param socketId socket index
 * @param parameterId not used
 * @param paraPtr [uint8]Nagle switch state
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ChanegeSocketTcpNagle(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr)
{
    TCPIP_UNUSED_ARG(parameterId);
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr->pcbPtr) && (TCPIP_IPPROTO_TCP == socketMngPtr->protocol))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        struct tcp_pcb* tpcb = (struct tcp_pcb*)(socketMngPtr->pcbPtr);

        /* listen pcb not suuport nagle switch */
        if (LISTEN != tpcb->state)
        {
            /* input parameter is function switch */
            if (0x0u != *((const uint8*)paraPtr))
            {
                tcp_nagle_enable(tpcb);
            }
            else
            {
                tcp_nagle_disable(tpcb);
            }

            ret = E_OK;
        }
    }

    return ret;
}
/**
 * @ingroup tcpip_internal
 * change specifies if the Nagle Algorithm  on the tcp socket. call by TcpIp_ChangeParameter
 * @param socketId socket index
 * @param parameterId Identifier of the parameter to be changed [limit keep alive]
 * @param paraPtr Pointer to memory containing the new parameter value
 * @return E_OK,ok;another failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ChanegeSocketTcpKeepAlive(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
    struct tcp_pcb* tpcb = (struct tcp_pcb*)(socketMngPtr->pcbPtr);
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != tpcb) && (TCPIP_IPPROTO_TCP == socketMngPtr->protocol))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        switch (parameterId)
        {
            case TCPIP_PARAMID_TCP_KEEPALIVE:
                if((*(const boolean *)paraPtr) == TRUE)
                {
                    ip_set_option(tpcb, SOF_KEEPALIVE);
                }
                else
                {
                    ip_reset_option(tpcb, SOF_KEEPALIVE);
                }
                ret = E_OK;
                break;
            case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:
                tpcb->keep_idle = *((const uint32*)paraPtr);
                ret = E_OK;
                break;
#if LWIP_TCP_KEEPALIVE

            case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX:
                tpcb->keep_cnt = *((const uint32*)paraPtr);
                ret = E_OK;
                break;

            case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:
                tpcb->keep_intvl = *((const uint32*)paraPtr);
                ret = E_OK;
                break;
#endif /* LWIP_TCP_KEEPALIVE */

            default:                                                                                                    /* PRQA S 2016 */ /* MISRA Rule 16.4 */
                break;
        }
    }

    return ret;
}
#endif/* (STD_ON == TCPIP_TCP_ENABLED) */

/**
 * @ingroup tcpip_internal
 * convert network mask from outside u8 to inside
 * @param inNetMaskNum outside network mask
 * @param domainType @ref TcpIp_DomainType
 * @param outNteMaskArry output point to memory inside encoding network mask
 */
FUNC(void, TCPIP_CODE) TcpIp_NetMaskExtToInner(
    VAR(uint8, AUTOMATIC) inNetMaskNum,
    VAR(TcpIp_DomainType, AUTOMATIC) domainType,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_VAR) outNteMaskArry)                                                            /* PRQA S 3432 */
{
    TCPIP_UNUSED_ARG(domainType);
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)

    if (TCPIP_AF_INET == domainType)
#endif/* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        /* ipv4 32bit*/
        outNteMaskArry[0] = (uint32)(0xffffffffUL << (32u - inNetMaskNum));
#endif/* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }

#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    else
#endif/* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        /* ipv6 128bit*/
        /* (128u - inNetMaskNum)>96u?32-inNetMaskNum:0  */
        outNteMaskArry[3] = (uint32)(0xffffffffUL << ((inNetMaskNum < 32u) ? (32u - inNetMaskNum) : 0u));
        /* (128u - inNetMaskNum)>64u?(64-inNetMaskNum)%32:0  */
        outNteMaskArry[2] = (uint32)(0xffffffffUL << ((inNetMaskNum < 64u) ? (64u - inNetMaskNum) & 0x1f : 0u));
        /* (128u - inNetMaskNum)>96u?(96-inNetMaskNum)%32:0  */
        outNteMaskArry[1] = (uint32)(0xffffffffUL << ((inNetMaskNum < 64u) ? (64u - inNetMaskNum) & 0x1f : 0u));
        /* (128u - inNetMaskNum)%32  */
        outNteMaskArry[0] = (uint32)(0xffffffffUL << ((128u - inNetMaskNum) & 0x1f));
#endif/* (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }
}
/**
 * @ingroup tcpip_internal
 * convert network mask from inside u8 to outside
 * @param domainType @ref TcpIp_DomainType
 * @param inNteMaskArry point to memory outside encoding network mask
 * @param outNetMaskNumPtr output point to memory outside encoding network mask
 */
FUNC(void, TCPIP_CODE) TcpIp_NetMaskInnerToExt(
    VAR(TcpIp_DomainType, AUTOMATIC) domainType,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_VAR) inNteMaskArry,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) outNetMaskNumPtr)                                                           /* PRQA S 3432 */
{
    uint8 cnt = 0u;
    TCPIP_UNUSED_ARG(domainType);
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)

    if (TCPIP_AF_INET == domainType)
#endif/* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        uint32 vlu = inNteMaskArry[0];

        while (vlu > 0u)
        {
            cnt++;
            vlu <<= 1;
        }

#endif/* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }

#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
    else
#endif/* (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    {
#if (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        uint32 vlu = inNteMaskArry[0];

        for (uintx i = 0; (i < 4) && (vlu > 0u); i++)
        {
            vlu = inNteMaskArry[i];

            while (vlu > 0)
            {
                cnt++;
                vlu <<= 1;
            }
        }

#endif/* (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS) */
    }

    *outNetMaskNumPtr = cnt;
}
/**
 * @ingroup tcpip_internal
 * event notify socket owner
 * @param socketMngPtr local socket management data struct
 * @param event @ref TcpIp_EventType
 */
FUNC(void, TCPIP_CODE) TcpIp_EventNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    VAR(TcpIp_EventType, AUTOMATIC) event)
{
    TcpIp_Up_TcpIpEventType eventFuncPtr = socketMngPtr->ownerCfgPtr->Up_TcpIpEventFuncPtr;

    if (NULL_PTR != eventFuncPtr)
    {
        TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                            /* PRQA S 0326,0306 */
        /* report event to upper layer */
        eventFuncPtr(socketId, event);
    }
}

#if (STD_ON == TCPIP_TCP_ENABLED)

/**
 * @ingroup tcpip_internal
 * tcp notify socket owner tx confirmation
 * @param socketMngPtr local socket management data struct
 * @param len Number of transmitted data bytes
 */
FUNC(void, TCPIP_CODE) TcpIp_TcpTxConfNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    VAR(uint16, AUTOMATIC) len)
{
    TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                                /* PRQA S 0326,0306 */
    TcpIp_Up_TxConfirmationType txConfFuncPtr = socketMngPtr->ownerCfgPtr->Up_TxConfirmationFuncPtr;

    if ((NULL_PTR != txConfFuncPtr) && (0u < len))
    {
        txConfFuncPtr(socketId, len);
    }
}
/**
 * @ingroup tcpip_internal
 * tcp client socket notify connect owner
 * @param socketMngPtr local socket management data struct
 */
FUNC(void, TCPIP_CODE)
TcpIp_TcpConnectNofiyUplayer(P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                     /* PRQA S 3432 */
{
    TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                                /* PRQA S 0326,0306 */
    TcpIp_Up_TcpConnectedType connectPtr = socketMngPtr->ownerCfgPtr->Up_TcpConnectedFuncPtr;

    if (NULL_PTR != connectPtr)
    {
        /* notify upper layer */
        connectPtr(socketId);
    }
}

/**
 * @ingroup tcpip_internal
 * tcp server socket notify accept owner
 * @param listenSocketId local listen socket id
 * @param socketMngPtr local socket management data struct
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpAcceptNofiyUplayer(
    VAR(TcpIp_SocketIdType, AUTOMATIC) listenSocketId,
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                                              /* PRQA S 3432 */
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_Up_TcpAcceptedType acceptFunc = socketMngPtr->ownerCfgPtr->Up_TcpAcceptedFuncPtr;

    if (NULL_PTR != acceptFunc)
    {
        struct tcp_pcb* newpcb = (struct tcp_pcb*)socketMngPtr->pcbPtr;
        TcpIp_SockAddrType remoteAdrInfo;
        TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAdrInfo.addr, newpcb->remote_ip.addr);
        remoteAdrInfo.port = newpcb->remote_port;
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
        remoteAdrInfo.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
        remoteAdrInfo.domain = TCPIP_AF_INET6;
#else /* TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */
        remoteAdrInfo.domain = IP_IS_V4_VAL(reIp) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif/* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */
        TcpIp_SocketIdType newAccSckId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                         /* PRQA S 0326,0306 */
        ret = acceptFunc(listenSocketId, newAccSckId, &remoteAdrInfo);
    }

    return ret;
}

/**
 * @ingroup tcpip_internal
 * tcp receiver data notify socket owner
 * @param socketMngPtr local socket management data struct
 * @param buf receiver data point struct
 */
FUNC(err_t, TCPIP_CODE) TcpIp_TcpRxIndNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */
    P2VAR(struct pbuf, AUTOMATIC, TCPIP_APPL_VAR) buf)                                                                  /* PRQA S 3432 */
{
    err_t ret = (err_t)ERR_ARG;
#if defined(TCPIP_INNER_CHECK)

    if (NULL_PTR != socketMngPtr->pcbPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        TcpIp_Up_RxIndicationType rxIndFuncPtr = socketMngPtr->ownerCfgPtr->Up_RxIndicationFuncPtr;

        if (NULL_PTR != rxIndFuncPtr)
        {
            TcpIp_SockAddrType remoteAdrInfo;
            ip_addr_t reIp;
            ret = tcp_tcp_get_tcp_addrinfo((struct tcp_pcb*)socketMngPtr->pcbPtr,
                                           0, &reIp, &remoteAdrInfo.port);

            if ((err_t)ERR_OK == ret)
            {
                TCPIP_LWIP_TO_LOCAL_IPADDR(remoteAdrInfo.addr, reIp.addr);
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
                remoteAdrInfo.domain = TCPIP_AF_INET;
#elif (TCPIP_SC2 == TCPIP_SCALABILITY_CLASS)
                remoteAdrInfo.domain = TCPIP_AF_INET6;
#else /* TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */
                remoteAdrInfo.domain = IP_IS_V4_VAL(reIp) ? TCPIP_AF_INET : TCPIP_AF_INET6;
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */
                struct pbuf* q = buf;

                if (NULL_PTR != buf->next)
                {
                    q = pbuf_coalesce(buf, PBUF_RAW);
#if defined(TCPIP_INNER_CHECK)

                    if (buf == q)
                    {
                        TCPIP_INNER_CHECK_OUTPUT("pbuf_coalesce handle pbuf error\n");
                        ret = (err_t)ERR_VAL;
                    }

#endif/* defined(TCPIP_INNER_CHECK) */
                }

                if ((err_t)ERR_OK == ret)
                {
                    TcpIp_SocketIdType socketId = TCPIP_SOCKID_CAL_ARYADR(socketMngPtr);                                /* PRQA S 0326,0306 */
                    rxIndFuncPtr(socketId, &remoteAdrInfo, (uint8*)(q->payload), q->len);
                    (void)pbuf_free(q);
                }
            }
        }
    }

    if ((err_t)ERR_OK != ret)
    {
        (void)pbuf_free(buf);
    }

    return ret;
}

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */


#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
/**
 * @ingroup tcpip_internal
 * start dhcp
 * @param ctrlIndex local controller index
  * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StartDhcp(VAR(uintx, AUTOMATIC) ctrlIndex)
{
    err_t err = (err_t)dhcp_start(&TcpIp_ControllerVar[ctrlIndex].netifVar);
    return ((err_t)ERR_OK == err) ? E_OK : E_NOT_OK;
}
/**
 * @ingroup tcpip_internal
 * stop dhcp
 * @param ctrlIndex local controller index
  * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StopDhcp(VAR(uintx, AUTOMATIC) ctrlIndex)
{
    dhcp_stop(&TcpIp_ControllerVar[ctrlIndex].netifVar);
    return E_OK;
}
#endif/* (STD_ON == TCPIP_DHCP_CLIENT_ENABLED) */

#if (STD_ON == TCPIP_AUTOIP_ENABLED)
/**
 * @ingroup tcpip_internal
 * start autoip
 * @param ctrlIndex local controller index
  * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StartAutoIp(VAR(uintx, AUTOMATIC) ctrlIndex)
{
    Std_ReturnType ret = E_NOT_OK;
#if defined(TCPIP_INNER_CHECK)

    if (NULL_PTR != ControlCfgPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        const TcpIp_AutoIpConfigType* autoIpCfgPtr = ControlCfgPtr[ctrlIndex].AutoIpCfgPtr;
        TcpIp_ControllerStatus* ctrlPtr = &TcpIp_ControllerVar[ctrlIndex];

        if (NULL_PTR != autoIpCfgPtr)
        {
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)

            if ((0u < autoIpCfgPtr->AutoIpInitTimeout) && (!TCPIP_CTRL_IS_AUTOIP_PENDING(ctrlPtr)))
            {
                ctrlPtr->autoipTimer = autoIpCfgPtr->AutoIpInitTimeout;
                TCPIP_CTRL_SET_AUTOIP_PENDING(ctrlPtr);
                ret = E_OK;
            }
            else
#endif/* (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER) */
            {
                err_t err = (err_t)autoip_start(&ctrlPtr->netifVar);
                ret = (Std_ReturnType)(((err_t)ERR_OK == err) ? E_OK : E_NOT_OK);
            }
        }
    }

    return ret;
}
/**
 * @ingroup tcpip_internal
 * stop autoip
 * @param ctrlIndex local controller index
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StopAutoIp(VAR(uintx, AUTOMATIC) ctrlIndex)
{
    Std_ReturnType ret = E_NOT_OK;
#if defined(TCPIP_INNER_CHECK)

    if (NULL_PTR != ControlCfgPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        const TcpIp_AutoIpConfigType* autoIpCfgPtr = ControlCfgPtr[ctrlIndex].AutoIpCfgPtr;
        TcpIp_ControllerStatus* ctrlPtr = &TcpIp_ControllerVar[ctrlIndex];

        if (NULL_PTR != autoIpCfgPtr)
        {
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)

            if (TCPIP_CTRL_IS_AUTOIP_PENDING(ctrlPtr))
            {
                TCPIP_CTRL_CLR_AUTOIP_PENDING(ctrlPtr);
                ctrlPtr->autoipTimer = 0u;
                ret = E_OK;
            }
            else
#endif/* (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER) */
            {
                err_t err = (err_t)autoip_stop(&ctrlPtr->netifVar);
                ret = (Std_ReturnType)(((err_t)ERR_OK == err) ? E_OK : E_NOT_OK);
            }
        }
    }

    return ret;
}
#endif/* (STD_ON == TCPIP_AUTOIP_ENABLED) */

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
/**
 * @ingroup tcpip_internal
 * start dynamic tls
 * @param socketId socket inner index
 * @param parameterId not used
 * @param tlsConnetId tls connect index
 * @return E_OK ok,another is failed
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsDynamicConnect(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) paraPtr)
{
    TCPIP_UNUSED_ARG(parameterId);
    uint8 tlsConnetId = *(const uint8*)paraPtr;
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[socketId];
#if defined(TCPIP_INNER_CHECK)

    if ((NULL_PTR != socketMngPtr->pcbPtr) && (TCPIP_IPPROTO_TCP == socketMngPtr->protocol))
#endif/* defined(TCPIP_INNER_CHECK) */
    {
        ret = TcpIp_TlsBaseDynamicConnect(socketMngPtr, tlsConnetId);
    }

    return ret;
}
#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */

/**
 * @ingroup tcpip_internal
 * all period timer init,call by TcpIp_Tnit
 */
FUNC(void, TCPIP_CODE) TcpIp_PeriodTimerInit(void)
{
#if (STD_ON == TCPIP_SUPPORT_TMR_4ms)
    TcpIp_PeriodTmr.tmr_4ms = 0u;
#endif/* (STD_ON == TCPIP_SUPPORT_TMR_4ms) */
#if (STD_ON == TCPIP_SUPPORT_TMR_100ms)
    TcpIp_PeriodTmr.tmr_100ms = 0u;
#endif /*STD_ON == TCPIP_SUPPORT_TMR_100ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_250ms)
    TcpIp_PeriodTmr.tmr_250ms = 0u;
#endif /*STD_ON == TCPIP_SUPPORT_TMR_250ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_500ms)
    TcpIp_PeriodTmr.tmr_500ms = 0u;
#endif /*STD_ON == TCPIP_SUPPORT_TMR_500ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_1000ms)
    TcpIp_PeriodTmr.tmr_1000ms = 0u;
#endif /*STD_ON == TCPIP_SUPPORT_TMR_1000ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_60000ms)
    TcpIp_PeriodTmr.tmr_60000ms = 0u;
#endif /*STD_ON == TCPIP_SUPPORT_TMR_60000ms */
}

#if (TCPIP_CONTROLLER_NUM > 0u)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @ingroup tcpip_internal
 * get remote ip link MAC address by arp protocal
 * @param ctrlIndex local controller index
 * @param ipAddrPtr specifies the IP address for which the physical address shall be retrieved
 * @param physAddrPtr Pointer to the memory where the physical address (MAC address) related to the specified IP address
 *                   is stored in network byte order
 * @param initRes specifies if the address resolution shall be initiated (TRUE) or not (FALSE) in case the
 *                physical address related to the specified IP address is currently unknown
 *
 * @return TCPIP_OK:specified IP address resolved
 *         TCPIP_E_PHYS_ADDR_MISS;physical address currently unknown
 *         TCPIP_E_NOT_OK:error handle
 */
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_GetRemotePhysAddrByArp(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) ipAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,                                                               /* PRQA S 3432 */
    VAR(boolean, AUTOMATIC) initRes)
{
    TcpIp_ReturnType ret = TCPIP_E_PHYS_ADDR_MISS;
    struct netif* netifPtr = &TcpIp_ControllerVar[ctrlIdx].netifVar;
    ip4_addr_t ipaddr = {ipAddrPtr->addr[0]};
    struct eth_addr* ethAdrPtr = NULL_PTR;
    const ip4_addr_t* ipPtr = NULL_PTR;

    if (-1 != etharp_find_addr(netifPtr, &ipaddr, &ethAdrPtr, &ipPtr))
    {
        for (uintx i = 0u; i < (uintx)ETH_HWADDR_LEN; i++)
        {
            physAddrPtr[i] = ethAdrPtr->addr[i];
        }

        ret = TCPIP_OK;
    }
    else if ((boolean)TRUE == initRes)
    {
        if ((err_t)ERR_OK != etharp_request(netifPtr, &ipaddr))
        {
            ret = TCPIP_E_NOT_OK;
        }
    }
    else
    {
        ret = TCPIP_E_NOT_OK;
    }

    return ret;
}
#endif/* ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)) */

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_InnerIcmpTransmit(
    VAR(uintx, AUTOMATIC) ipAdr,
    VAR(uintx, AUTOMATIC) ctrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    P2CONST(TcpIp_IcmpTransDataType, AUTOMATIC, TCPIP_APPL_VAR) icmpParaPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    TcpIp_ControllerStatus* ctrlMngPtr = &TcpIp_ControllerVar[ctrlIdx];

    if (TCPIP_STATE_ONLINE == ctrlMngPtr->ctrlStatus)
    {
        err_t lwRet = (err_t)ERR_ARG;

        if (TcpIp_IcmpPcbPtr == NULL_PTR)
        {
            TcpIp_IcmpPcbPtr = raw_new(IP_PROTO_ICMP);
        }

#if defined(TCPIP_INNER_CHECK)

        if (TcpIp_IcmpPcbPtr != NULL_PTR)
#endif/* defined(TCPIP_INNER_CHECK) */
        {
#if defined(TCPIP_INNER_CHECK)
            lwRet = raw_bind(TcpIp_IcmpPcbPtr, &TcpIp_LocalAdrTable[ipAdr].ipAdrSrc);
            raw_bind_netif(TcpIp_IcmpPcbPtr, &ctrlMngPtr->netifVar);

            if ((err_t)ERR_OK == lwRet)
#else/* !defined(TCPIP_INNER_CHECK) */
            (void)raw_bind(TcpIp_IcmpPcbPtr, &TcpIp_LocalAdrTable[ipAdr].ipAdrSrc);

#endif/* defined(TCPIP_INNER_CHECK) */
            {
                uint16 len = (uint16)sizeof(struct icmp_echo_hdr) + icmpParaPtr->dataLength;
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                struct pbuf* q = pbuf_alloc(PBUF_IP, len, PBUF_RAM);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#if defined(TCPIP_INNER_CHECK)

                if (NULL_PTR != q)
#endif/* defined(TCPIP_INNER_CHECK) */
                {
                    struct icmp_echo_hdr* icmphdr = q->payload;
                    icmphdr->code = icmpParaPtr->code;
                    icmphdr->type = icmpParaPtr->type;
                    icmphdr->id = 0u;
                    icmphdr->seqno = 0u;
                    TcpIp_IcmpPcbPtr->ttl = icmpParaPtr->ttl;
                    icmphdr->chksum = 0u;                                                                               /* PRQA S 2982 */ /* MISRA Rule 2.2 */
                    /* copy data to pbuf */
#if defined(TCPIP_INNER_CHECK)
                    lwRet = pbuf_take_at(q,
                                         icmpParaPtr->dataPtr,
                                         icmpParaPtr->dataLength,
                                         (uint16)sizeof(struct icmp_echo_hdr));
#endif/* defined(TCPIP_INNER_CHECK) */
#if CHECKSUM_GEN_ICMP
                    IF__NETIF_CHECKSUM_ENABLED(netif, NETIF_CHECKSUM_GEN_ICMP)
                    {
                        icmphdr->chksum = inet_chksum(icmphdr, q->len);
                    }
#endif/* CHECKSUM_GEN_ICMP */
                }

#if defined(TCPIP_INNER_CHECK)
                else
                {
                    lwRet = (err_t)ERR_ARG;
                }

#endif/* defined(TCPIP_INNER_CHECK) */

                if ((err_t)ERR_OK == lwRet)
                {
                    ip_addr_t rometeIp;
                    TCPIP_LOCAL_TO_LWIP_IPADDR(rometeIp.addr, remoteAddrPtr->addr);
                    lwRet = raw_sendto(TcpIp_IcmpPcbPtr, q, &rometeIp);
                }

                /* free buff */
                if (NULL_PTR != q)
                {
                    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                    (void)pbuf_free(q);
                    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                }
            }
        }

        if ((err_t)ERR_OK == lwRet)
        {
            ret = E_OK;
        }
    }

    return ret;
}
#endif/* ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)) */
#endif/* (STD_ON == TCPIP_ICMP_ENABLED) */
#endif/* (TCPIP_CONTROLLER_NUM > 0u) */

#if (TCPIP_SOCKET_NUM > 0u)
/**
 * @ingroup tcpip_internal
 * poll all socket
 *
 *  */
FUNC(void, TCPIP_CODE) TcpIp_SocketPollMainHandle(void)
{
    for (uintx index = 0; index < TCPIP_SOCKET_NUM; index++)
    {
        TcpIp_SocketHandleType* socketMngPtr = &TcpIp_SocketTable[index];
#if (STD_ON == TCPIP_TCP_ENABLED)

        if ((TCPIP_IPPROTO_TCP == socketMngPtr->protocol)
            && (NULL_PTR != socketMngPtr->pcbPtr)
            && (0u < socketMngPtr->txRemainLen)
#if defined(TCPIP_INNER_CHECK)
            && (NULL_PTR != socketMngPtr->ownerCfgPtr)
            && (NULL_PTR != socketMngPtr->ownerCfgPtr->Up_CopyTxDataFuncPtr)
#endif/* defined(TCPIP_INNER_CHECK) */
           )
        {
            TcpIp_CopyTcpMainHandle(socketMngPtr);
        }

#endif/* (STD_ON == TCPIP_TCP_ENABLED) */
    }
}
#endif /* (TCPIP_TCP_SOCKET_MAX > 0u) */

#if defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
/**
 * @ingroup tcpip_internal
 * poll all control
 *
 *  */
FUNC(void, TCPIP_CODE) TcpIp_ControlPollMainHandle(void)
{
#if (1u < TCPIP_CONTROLLER_NUM)

    for (uintx index = 0u; index < TCPIP_CONTROLLER_NUM; index++)
#else/* (1u >= TCPIP_CONTROLLER_NUM) */
    uintx index = 0u;

#endif/* (1u < TCPIP_CONTROLLER_NUM) */
    {
        TcpIp_ControllerStatus* ctrlPtr = &TcpIp_ControllerVar[index];
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)

        if (TCPIP_CTRL_IS_AUTOIP_PENDING(ctrlPtr))
        {
            TCPIP_CTRL_CLR_AUTOIP_PENDING(ctrlPtr);

            if (0u < ctrlPtr->autoipTimer)
            {
                ctrlPtr->autoipTimer--;
            }

            if (0u == ctrlPtr->autoipTimer)
            {
                (void)autoip_start(&ctrlPtr->netifVar);
            }
        }

#endif/* (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER) */
    }
}
#endif/* defined(TCPIP_SUUPORT_CTRL_MAINHANDLE) */

/**
 * @ingroup tcpip_internal
 * all period timer poll,call by TcpIp_MainFunction
 */
FUNC(void, TCPIP_CODE) TcpIp_PeriodTimerMainHandle(void)
{
#if (STD_ON == TCPIP_TCP_SYNISN_RECALC)                                                                                 /* PRQA S 3332 */
#if (TCPIP_MAINFUNCTION_PERIOD >= 4u)
    TcpIp_TcpIsnCount += (TCPIP_MAINFUNCTION_PERIOD / 4u);
#else/* (TCPIP_MAINFUNCTION_PERIOD >= 4u) */

    if (0u == (TcpIp_PeriodTmr.tmr_4ms--))                                                                              /* PRQA S 3440 */
    {
        TcpIp_PeriodTmr.tmr_4ms = TCPIP_CALCTMR(4);
        TcpIp_TcpIsnCount++;
    }

#endif/* (TCPIP_MAINFUNCTION_PERIOD >= 4u) */
#endif/* (STD_ON == TCPIP_TCP_SYNISN_RECALC) */
#if (STD_ON == TCPIP_SUPPORT_TMR_100ms)

    /* 100ms period handle */
    if (0u == (TcpIp_PeriodTmr.tmr_100ms--))                                                                            /* PRQA S 3440 */
    {
        TcpIp_PeriodTmr.tmr_100ms = TCPIP_CALCTMR(100u);
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
        autoip_tmr();
#endif/* STD_ON == TCPIP_AUTOIP_ENABLED */
#if LWIP_IPV6_MLD
        mld6_tmr();
#endif /* LWIP_IPV6_MLD */
    }

#endif/*STD_ON == TCPIP_SUPPORT_TMR_100ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_250ms)

    /* 250ms period handle */
    if (0u == (TcpIp_PeriodTmr.tmr_250ms--))                                                                            /* PRQA S 3440 */
    {
        TcpIp_PeriodTmr.tmr_250ms = TCPIP_CALCTMR(250u);
#if (STD_ON == TCPIP_TCP_ENABLED)
        SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
        tcp_tmr();
        SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
#endif/* STD_ON == TCPIP_TCP_ENABLED */
    }

#endif/*STD_ON == TCPIP_SUPPORT_TMR_250ms */
    /* 500ms period handle */
#if (STD_ON == TCPIP_SUPPORT_TMR_500ms)

    if (0u == (TcpIp_PeriodTmr.tmr_500ms--))                                                                            /* PRQA S 3440 */
    {
        TcpIp_PeriodTmr.tmr_500ms = TCPIP_CALCTMR(500u);
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
        dhcp_fine_tmr();
#endif/* STD_ON == TCPIP_DHCP_CLIENT_ENABLED */
    }

#endif/*STD_ON == TCPIP_SUPPORT_TMR_500ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_1000ms)
    /* 1000ms period handle */

    if (0u == (TcpIp_PeriodTmr.tmr_1000ms--))                                                                           /* PRQA S 3440 */
    {
        TcpIp_PeriodTmr.tmr_1000ms = TCPIP_CALCTMR(1000u);
#if (STD_ON == TCPIP_ARP_ENABLED)
        etharp_tmr();
#endif/* (STD_ON == TCPIP_ARP_ENABLED) */
#if IP_REASSEMBLY
        ip_reass_tmr();
#endif /* IP_REASSEMBLY */
#if (STD_ON == TCPIP_IPV6_ENABLED)
        nd6_tmr();
#endif/* STD_ON == TCPIP_IPV6_ENABLED */
#if LWIP_IPV6_REASS
        ip6_reass_tmr();
#endif /* LWIP_IPV6_MLD */
    }

#endif/*STD_ON == TCPIP_SUPPORT_TMR_1000ms */
#if (STD_ON == TCPIP_SUPPORT_TMR_60000ms)
    /* 60000ms period handle */

    if (0u == (TcpIp_PeriodTmr.tmr_60000ms--))                                                                          /* PRQA S 3440 */
    {
        TcpIp_PeriodTmr.tmr_60000ms = TCPIP_CALCTMR(60000u);
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
        dhcp_coarse_tmr();
#endif/* STD_ON == TCPIP_DHCP_CLIENT_ENABLED */
    }

#endif/*STD_ON == TCPIP_SUPPORT_TMR_60000ms */
}

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
