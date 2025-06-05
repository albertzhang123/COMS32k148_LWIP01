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
**  FILENAME    : TcpIp_TlsBase.c                                                                                     **
**                                                                                                                    **
**  Created on  : 18/02/21                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : internal type definition for TcpIp                                                                  **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

/***********************************************************************************************************************
**                                          REVISION   HISTORY                                                        **
***********************************************************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20200828]  [pengfei.zhu]   Update to R19-11.
 *  V2.0.1    [20200928]  [pengfei.zhu]   QAC test.
 *  V2.1.0    [20210123]  [darren.zhang]  Code refactoring
 **********************************************************************************************************************/


#include "TcpIp_Cfg.h"
#include "TcpIp_Types.h"
#include "TcpIp_Lcfg.h"
#include "SchM_TcpIp.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp_Internal.h"
#include "Std_Types.h"
#include "TcpIp_Tls.h"

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

/* inner function condtion check level,per level consume memory or cycles is different */
#if !defined(TCPIP_TLS_CHECK_TAG)
#define TCPIP_TLS_CHECK
#define TCPIP_TLS_CHECK_OUTPUT(...)   TCPIP_CHECK_OUTPUT(__VA_ARGS__)
#endif /* defined(TCPIP_INNER_CHECK_TAG) */
/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
***********************************************************************************************************************/
#define U32FLAG_INDEX(id)               ((id) >> 5u)
#define U32FLAG_MASK(index)             (((uint32)0x1u) << ((index) & 0x1fu))
#define TLS_CONNGROUP_ARRY_SIZE         ((TCPIP_TCP_TLS_CONNECTION_GROUP_NUM + 31u) >> 5u) /* (x+31)/32 */
#define TLS_CONNGROUP_IS_USED(conIndex) (\
                                         (TcpIp_TlsConnectionGroupState[U32FLAG_INDEX(conIndex)] \
                                          & (U32FLAG_MASK(conIndex))) == U32FLAG_MASK(conIndex))
#define TLS_CONNGROUP_SET_USED(conIndex) do{\
        TcpIp_TlsConnectionGroupState[U32FLAG_INDEX(conIndex)] |= U32FLAG_MASK(conIndex);\
    }while(0);
#define TLS_CONNGROUP_SET_UNUSED(conIndex) do{\
        TcpIp_TlsConnectionGroupState[U32FLAG_INDEX(conIndex)] &=(uint32)(~U32FLAG_MASK(conIndex));\
    }while(0);

#define TCP_TLS_WRITE_FLAG_COPY     (0x01u)

#if defined(TCPIP_FASTTX_TCP)
#define TCPIP_TLS_FASTTX_TCP
#endif/* defined(TCPIP_FASTTX_TCP) */

/***********************************************************************************************************************
*  LOCAL DATA
************************************************************************************ **********************************/
#if (TCPIP_TCP_TLS_CONNECTION_GROUP_NUM > 0u)

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(uint32, TCPIP_VAR) TcpIp_TlsConnectionGroupState[TLS_CONNGROUP_ARRY_SIZE];
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#endif/* (TCPIP_TCP_TLS_CONNECTION_GROUP_NUM > 0u) */

#if (TCPIP_TLS_MAX_CONNECTIONS > 0u)

#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL VAR(TcpIp_TlsSocketDataType, TCPIP_VAR) TcpIp_TlsSocketDataTable[TCPIP_TLS_MAX_CONNECTIONS] = {0};          /* PRQA S 3218 */   /* MISRA Rule 8.9 */
#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_VAR_NO_INIT_PTR
#include "TcpIp_MemMap.h"
TCPIP_LOCAL P2VAR(TcpIp_TlsSocketDataType, AUTOMATIC, TCPIP_VAR) TcpIp_TlsSocketIdleListPtr = NULL_PTR;                 /* PRQA S 3432 */ /* MISRA Rule 20.7 */
#define TCPIP_STOP_SEC_VAR_NO_INIT_PTR
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"
TCPIP_LOCAL P2CONST(TcpIp_TlsConfigType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_tlsCfgPtr = NULL_PTR;
#define TCPIP_STOP_SEC_VAR_CLEARED_PTR
#include "TcpIp_MemMap.h"
#endif/* (TCPIP_TLS_MAX_CONNECTIONS > 0u) */

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS PROTOTYPES
***********************************************************************************************************************/
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE)altcp_mbedtls_handle_rx_appldata(
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(TcpIp_TlsSocketDataType, AUTOMATIC, TCPIP_APPL_VAR) tlsSocketPtr);                                            /* PRQA S 3432 */ /* MISRA Rule 20.7 */

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) altcp_mbedtls_lower_recv_process(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr);                                             /* PRQA S 3432 */ /* MISRA Rule 20.7 */
/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
***********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
TCPIP_LOCAL FUNC(int, TCPIP_CODE) TcpIp_TlsBioSendCallBack(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) ctx,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_VAR)dataptr,
    VAR(uint32, AUTOMATIC) size)
{
    int written = 0;
    const TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)ctx;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->pcbPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        uint32 localLen = size;

        while (localLen > 0u)
        {
            uint16 wrLen = (localLen > 0xffffu) ? 0xffffu : (uint16)localLen;
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            err_t err = tcp_write((struct tcp_pcb*)socketMngPtr->pcbPtr,
                                  dataptr, wrLen, TCP_TLS_WRITE_FLAG_COPY);
#if defined(TCPIP_TLS_FASTTX_TCP)
            err = tcp_output((struct tcp_pcb*)socketMngPtr->pcbPtr);
#endif/* defiend(TCPIP_FAST_TCP) */
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);

            if ((err_t)ERR_OK == err)
            {
                written += (int)wrLen;
                localLen -= wrLen;
            }
            else
            {
                TCPIP_TLS_CHECK_OUTPUT("tls_write, tcp_write: err =%d\n", err);
                written = TLS_ERR_NET_SEND_FAILED;
                break;
            }
        }
    }

#if defined(TCPIP_TLS_CHECK)
    else
    {
        TCPIP_TLS_CHECK_OUTPUT("ctx is error!\n");
    }

#endif/* defined(TCPIP_TLS_CHECK) */
    return written;
}

TCPIP_LOCAL FUNC(int, TCPIP_CODE) TcpIp_TlsBioRecvCallBack(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) ctx,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR)buf,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(uint32, AUTOMATIC) len)
{
    uint16 bufRet = 0u;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)ctx;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->tlsPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;
        struct pbuf* p = tlsSocketPtr->rx;

        if ((p == NULL) || ((p->len == 0u) && (p->next == NULL)))
        {
            if (p != NULL_PTR)
            {
                (void)pbuf_free(p);
            }

            tlsSocketPtr->rx = NULL;

            if ((tlsSocketPtr->flags & (ALTCP_MBEDTLS_FLAGS_RX_CLOSE_QUEUED | ALTCP_MBEDTLS_FLAGS_RX_CLOSED)) ==
                ALTCP_MBEDTLS_FLAGS_RX_CLOSE_QUEUED)
            {
                /* close queued but not passed up yet */
                return 0;
            }

            return TLS_ERR_SSL_WANT_READ;
        }

        /* limit number of bytes again to copy from first pbuf in a chain only */
        uint16 copyLen = (len > (uint32)(p->len)) ? p->len : (uint16)len;
        /* copy the data */
        bufRet = pbuf_copy_partial(p, buf, copyLen, 0);
#if defined(TCPIP_TLS_CHECK)

        if (bufRet == copyLen)
#endif/* defined(TCPIP_TLS_CHECK) */
        {
            /* hide the copied bytes from the pbuf */
#if defined(TCPIP_TLS_CHECK)
            if (pbuf_remove_header(p, bufRet) != 0u)
            {
                TCPIP_TLS_CHECK_OUTPUT("pbuf_remove_header happend error\n");
            }

#else /* !defined(TCPIP_TLS_CHECK) */
            (void)pbuf_remove_header(p, bufRet);
#endif/* defined(TCPIP_TLS_CHECK) */

            if (p->len == 0u)
            {
                /* the first pbuf has been fully read, free it */
                tlsSocketPtr->rx = p->next;
                p->next = NULL;
                (void)pbuf_free(p);
            }

            tlsSocketPtr->bio_bytes_read += (int)bufRet;
        }

#if defined(TCPIP_TLS_CHECK)
        else
        {
            TCPIP_TLS_CHECK_OUTPUT("pbuf_copy_partial happend error:%d,need copy length:%d\n", bufRet, copyLen);
        }

#endif/* defined(TCPIP_TLS_CHECK) */
    }

#if defined(TCPIP_TLS_CHECK)
    else
    {
        TCPIP_TLS_CHECK_OUTPUT("ctx is error!\n");
    }

#endif/* defined(TCPIP_TLS_CHECK) */
    return (int)bufRet;
}

/* Pass queued decoded rx data to application */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) altcp_mbedtls_pass_rx_data(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    err_t ret = (err_t)ERR_VAL;
    TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;
#if defined(TCPIP_TLS_CHECK)

    if (NULL_PTR != tlsSocketPtr)
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        struct pbuf* buf = tlsSocketPtr->rx_app;

        if (NULL_PTR != buf)
        {
            tlsSocketPtr->rx_app = NULL_PTR;
            uint16 totalLen = buf->tot_len;
            /* this needs to be increased first because the 'recved' call may come nested */
            tlsSocketPtr->rx_passed_unrecved += (int)totalLen;
            tlsSocketPtr->flags |= ALTCP_MBEDTLS_FLAGS_UPPER_CALLED;
            /* shall call upper layer */
            ret = TcpIp_TcpRxIndNotifyUpLayer(socketMngPtr, buf);
        }
        else if ((tlsSocketPtr->flags & (ALTCP_MBEDTLS_FLAGS_RX_CLOSE_QUEUED | ALTCP_MBEDTLS_FLAGS_RX_CLOSED)) ==
                 ALTCP_MBEDTLS_FLAGS_RX_CLOSE_QUEUED)
        {
            tlsSocketPtr->flags |= ALTCP_MBEDTLS_FLAGS_RX_CLOSED;

            /* shall notify upper layer receiver fin */
            if (!TCPIP_TCP_IS_PENDINGEVENT(socketMngPtr))
            {
                TcpIp_EventNotifyUpLayer(socketMngPtr, TCPIP_TCP_FIN_RECEIVED);
            }
        }
        else
        {
            /* nothing */
        }
    }

    return ret;
}
/* Call recved for possibly more than an u16_t */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) altcp_mbedtls_lower_recved(
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(int, AUTOMATIC) recvdLen)
{
    int pollLen = recvdLen;

    while (0 < pollLen)
    {
        uint16 rePartLen = (pollLen > 65535) ? 0xffffu : (uint16)pollLen;
        tcp_recved(tpcb, rePartLen);
        pollLen -= (int)rePartLen;
    }
}


TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) TcpIp_TlsRecvCallBack(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(struct pbuf, AUTOMATIC, TCPIP_APPL_VAR) p,                                                                    /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(err_t, AUTOMATIC) err)
{
    TCPIP_UNUSED_ARG(err);
    err_t ret = (err_t)ERR_VAL;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->tlsPtr) && (NULL_PTR != socketMngPtr->pcbPtr)
        && (socketMngPtr->pcbPtr == tpcb))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;

        if (NULL_PTR != p)
        {
            /* If we come here, the connection is in good state (handshake phase or application data phase).
            Queue up the pbuf for processing as handshake data or application data. */
            if (tlsSocketPtr->rx == NULL)
            {
                tlsSocketPtr->rx = p;
            }
            else
            {
                pbuf_cat(tlsSocketPtr->rx, p);
            }

            ret = altcp_mbedtls_lower_recv_process(socketMngPtr);
        }
        else
        {
            /* handle NULL pbuf (inner connection closed) */
            /* remote host sent FIN, remember this (SSL state is destroyed when both sides are closed only!) */
            if ((tlsSocketPtr->flags & (ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE | ALTCP_MBEDTLS_FLAGS_UPPER_CALLED)) ==
                (ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE | ALTCP_MBEDTLS_FLAGS_UPPER_CALLED))
            {
                if ((tlsSocketPtr->rx != NULL) || (tlsSocketPtr->rx_app != NULL))
                {
                    tlsSocketPtr->flags |= ALTCP_MBEDTLS_FLAGS_RX_CLOSE_QUEUED;
                    /* this is a normal close (FIN) but we have unprocessed data, so delay the FIN */
                    (void)altcp_mbedtls_handle_rx_appldata(tpcb, tlsSocketPtr);
                    return (err_t)ERR_OK;
                }

                tlsSocketPtr->flags |= ALTCP_MBEDTLS_FLAGS_RX_CLOSED;
                /* shall notify upper fin receiver */
            }
            else
            {
                /* pending event notify */
                TCPIP_TCP_SET_PENDINGEVENT(socketMngPtr);
                /* before connection setup is done: call 'err' */
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                ret = tcp_close(tpcb);
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            }
        }
    }

#if defined(TCPIP_TLS_CHECK)
    else
    {
        TCPIP_TLS_CHECK_OUTPUT("TcpIp_TlsRecvCallBack pre check is error!\n");
    }

#endif/* defined(TCPIP_TLS_CHECK) */
    return ret;
}


#if (TCPIP_TCP_TLS_CONNECTION_NUM > 0u)


TCPIP_LOCAL FUNC(void, TCPIP_CODE) altcp_mbedtls_free_config(void* item)
{
    if (item != NULL)
    {
        mem_free(item);
    }
}

TCPIP_LOCAL FUNC_P2VAR(void, AUTOMATIC, TCPIP_CODE) altcp_mbedtls_alloc_config(size_t size)                             /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    void* ret;
    size_t checked_size = (mem_size_t)size;

    if (size != checked_size)
    {
        /* allocation too big (mem_size_t overflow) */
        return NULL;
    }

    ret = (TcpIp_TlsSocketDataType*)mem_calloc(1, (mem_size_t)size);
    return ret;
}

TCPIP_LOCAL FUNC(void, TCPIP_CODE) mbedtls_ssl_conf_authmode(mbedtls_ssl_config* conf, int authmode)
{
    conf->authmode = (uint8)authmode;
}

/* Load default in mbedtls_ssl_config */
TCPIP_LOCAL FUNC(int, TCPIP_CODE) mbedtls_ssl_config_defaults(mbedtls_ssl_config* conf, int transport)
{
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = TcpIp_tlsCfgPtr->TlsConnectionPtr;
    conf->transport = (uint8)transport;

    if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_CLIENT)
    {
        conf->authmode = TLS_VERIFY_REQUIRED;
    }

    conf->cert_req_ca_list = TLS_CERT_REQ_CA_LIST_ENABLED;/*for server*/
    const TcpIp_TlsCiphersuiteDefinitionConfigType* ciphersuiteCfgPtr =
        tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsCiphersuiteDefinitionRef;

    if (TLS_VERSION_V12 == ciphersuiteCfgPtr->TlsVersion)
    {
        conf->min_major_ver = TLS_MAJOR_VERSION_3;
        conf->min_minor_ver = TLS_MINOR_VERSION_3;          /* TLS 1.2 */
        conf->max_major_ver = TLS_MAX_MAJOR_VERSION;
        conf->max_minor_ver = TLS_MAX_MINOR_VERSION;
        const int* ciphersuiteIdPtr = (const int*)&ciphersuiteCfgPtr->TlsCiphersuiteId;
        conf->ciphersuite_list[TLS_MINOR_VERSION_0] = ciphersuiteIdPtr;
        conf->ciphersuite_list[TLS_MINOR_VERSION_1] = ciphersuiteIdPtr;
        conf->ciphersuite_list[TLS_MINOR_VERSION_2] = ciphersuiteIdPtr;
        conf->ciphersuite_list[TLS_MINOR_VERSION_3] = ciphersuiteIdPtr;

        if ((boolean)TRUE == ciphersuiteCfgPtr->TlsUseSecurityExtensionForceEncryptThenMac)
        {
            conf->encrypt_then_mac = TLS_ETM_ENABLED;
        }
        else
        {
            conf->encrypt_then_mac = TLS_ETM_DISABLED;
        }

        conf->mfl_code = tlsConnCfgPtr->TlsMaxFragmentLength;
        conf->hostname = (const uint8*)tlsConnCfgPtr->TlsCertificateIdentityRef->TlsServerNameIdentification;
    }
    else
    {
        /* don't support TLS V1.3 */
    }

    return 0;
}

/** Create new TLS configuration
 * ATTENTION: Server certificate and private key have to be added outside this function!
 */
// *INDENT-OFF*
TCPIP_LOCAL FUNC_P2VAR(struct altcp_tls_config, AUTOMATIC, TCPIP_CODE) altcp_tls_create_config(                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    int is_server, int have_cert, int have_pkey, int have_ca)
// *INDENT-ON*
{
    TCPIP_UNUSED_ARG(is_server);
    size_t sz;
    int ret;
    struct altcp_tls_config* conf;
    mbedtls_x509_crt* mem;
#if defined(TCPIP_TLS_CHECK)
#if (TCP_WND < TLS_SSL_MAX_CONTENT_LEN)
#error "tls config is error"
#endif/* (TCP_WND < TLS_SSL_MAX_CONTENT_LEN) */
#endif/* defined(TCPIP_TLS_CHECK) */
    sz = sizeof(struct altcp_tls_config);

    if (have_cert > 0)
    {
        sz += sizeof(mbedtls_x509_crt);
    }

    if (have_ca > 0)
    {
        sz += sizeof(mbedtls_x509_crt);
    }

    if (have_pkey > 0)
    {
        sz += sizeof(mbedtls_pk_context);
    }

    conf = (struct altcp_tls_config*)altcp_mbedtls_alloc_config(sz);

    if (conf == NULL)
    {
        return NULL;
    }

    mem = (mbedtls_x509_crt*)(conf + 1);

    if (have_cert > 0)
    {
        conf->cert = mem;
        mem++;
    }

    if (have_ca > 0)
    {
        conf->ca = mem;
        mem++;
    }

    if (have_pkey > 0)
    {
        conf->pkey = (mbedtls_pk_context*)mem;
    }

    (void)memset(conf, 0, sizeof(mbedtls_ssl_config));
    /* Setup ssl context (@todo: what's different for a client here? -> might better be done on listen/connect) */
    ret = mbedtls_ssl_config_defaults(&conf->conf, TLS_SSL_TRANSPORT_STREAM);

    if (ret != 0)
    {
        altcp_mbedtls_free_config(conf);
        return NULL;
    }

    mbedtls_ssl_conf_authmode(&conf->conf, TLS_SSL_VERIFY_OPTIONAL);
    return conf;
}
TCPIP_LOCAL struct altcp_tls_config*
TcpIp_TlsCreateCommonClient(const u8_t* ca, size_t ca_len, int is_2wayauth)
{
    TCPIP_UNUSED_ARG(ca_len);
    struct altcp_tls_config* conf = altcp_tls_create_config(0, is_2wayauth, is_2wayauth, (ca != NULL) ? 1 : 0);

    if (conf == NULL)
    {
        return NULL;
    }

    return conf;
}

TCPIP_LOCAL struct altcp_tls_config*
TcpIp_TlsCreateClient(const u8_t* ca, size_t ca_len)
{
    return TcpIp_TlsCreateCommonClient(ca, ca_len, 0);
}

TCPIP_LOCAL FUNC(struct altcp_tls_config*, TCPIP_CODE) TcpIp_GetTlsCreateHandleCfg(
    P2CONST(TcpIp_TlsConnectionConfigType, AUTOMATIC, TCPIP_APPL_CONST) tlsCnnCfgPtr)
{
    struct altcp_tls_config* tlsHandlePtr = NULL_PTR;
    TcpIp_KeyMCertificateIdType certId = tlsCnnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMLocalCertificateId;
    KeyM_CertDataType certificateData = {0};
    Std_ReturnType ret = TcpIp_GetCertificate(certId, &certificateData); /*SWS_TCPIP_00316*/

    if ((Std_ReturnType)E_OK == ret)
    {
        if (TLS_CLIENT == tlsCnnCfgPtr->TlsConnectionInitStateType)
        {
            if ((boolean)FALSE == tlsCnnCfgPtr->TlsUseClientAuthenticationRequest)
            {
                tlsHandlePtr = TcpIp_TlsCreateClient((const uint8*)(certificateData.certData),
                                                     certificateData.certDataLength);
            }
            /* require local certificate and private key */
            else
            {
                /* tlsHandlePtr = TcpIp_TlsCreateTwoWayAuthClient(); */
            }
        }
        else if (TLS_SERVER == tlsCnnCfgPtr->TlsConnectionInitStateType)
        {
            /* tlsHandlePtr = TcpIp_TlsCreateServer(); */
        }
        else
        {
#if defined(TCPIP_TLS_CHECK)
            TCPIP_TLS_CHECK_OUTPUT("tls connection states=%d is error!\n", tlsCnnCfgPtr->TlsConnectionInitStateType);
#endif/* defined(TCPIP_TLS_CHECK) */
        }
    }

    return tlsHandlePtr;
}

/* Function prototype for tcp error callback functions. Called when the pcb receives a RST or is unexpectedly closed
for any other reason */
/* typedef void  (*tcp_err_fn)(void *arg, err_t err) */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) altcp_mbedtls_lower_err(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(err_t, AUTOMATIC) err)
{
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->ownerCfgPtr) && (NULL_PTR != socketMngPtr->tlsPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_EventNotifyUpLayer(socketMngPtr, (err != (err_t)ERR_CLSD) ? TCPIP_TCP_RESET : TCPIP_TCP_CLOSED);
    }
}

/** Sent callback from lower connection (i.e. TCP)
 * This only informs the upper layer to try to send more, not about
 * the number of ACKed bytes.
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) altcp_mbedtls_lower_sent(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(uint16, AUTOMATIC) len)
{
    TCPIP_UNUSED_ARG(len);
    err_t ret = (err_t)ERR_OK;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->tlsPtr) && (socketMngPtr->pcbPtr == tpcb))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;

        if ((tlsSocketPtr->flags & ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE) != 0u)
        {
            /* try to send more if we failed before */
            (void)TcpIp_TlsFlushOutput(&tlsSocketPtr->ssl_context);

            /* call upper sent with len==0 if the application already sent data */
            if ((tlsSocketPtr->flags & ALTCP_MBEDTLS_FLAGS_APPLDATA_SENT) != 0u)
            {
                TcpIp_TcpTxConfNotifyUpLayer(socketMngPtr, 0u);
            }
        }
    }

    return ret;
}

TCPIP_LOCAL FUNC(void, TCPIP_CODE) mbedtls_ssl_set_bio(
    mbedtls_ssl_context* ssl,
    void* p_bio,
    mbedtls_ssl_send_t* f_send,
    mbedtls_ssl_recv_t* f_recv,
    mbedtls_ssl_recv_timeout_t* f_recv_timeout)
{
    ssl->p_bio          = p_bio;
    ssl->f_send         = f_send;
    ssl->f_recv         = f_recv;
    ssl->f_recv_timeout = f_recv_timeout;
}

/*
 * Initialize an SSL context
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) mbedtls_ssl_init(mbedtls_ssl_context* ssl)
{
    (void)memset(ssl, 0, sizeof(mbedtls_ssl_context));
}

TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsSocketInit(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr->pcbPtr) && (NULL_PTR != socketMngPtr->tlsPtr) &&
        (NULL_PTR != socketMngPtr->tlsPtr->tlsCnnCfgPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        const TcpIp_TlsConnectionConfigType* tlsCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
        const struct altcp_tls_config* tlsHandlePtr = TcpIp_GetTlsCreateHandleCfg(tlsCfgPtr);
#if defined(TCPIP_TLS_CHECK)

        if (NULL_PTR != tlsHandlePtr)
#endif/* defined(TCPIP_TLS_CHECK) */
        {
            struct tcp_pcb* tpcb = (struct tcp_pcb*)socketMngPtr->pcbPtr;
            mbedtls_ssl_context* sslConPtr = &(socketMngPtr->tlsPtr->ssl_context);
            mbedtls_ssl_init(sslConPtr);
            int mbRet = TcpIp_TlsSslSetup(sslConPtr, &tlsHandlePtr->conf);
#if defined(TCPIP_TLS_CHECK)

            if (0 == mbRet)
#endif/* defined(TCPIP_TLS_CHECK) */
            {
                /* tell mbedtls about our I/O functions */
                mbedtls_ssl_set_bio(sslConPtr,
                                    socketMngPtr,
                                    TcpIp_TlsBioSendCallBack,
                                    TcpIp_TlsBioRecvCallBack,
                                    (mbedtls_ssl_recv_timeout_t*)NULL);
                /* cover reg callback function */
                /* shall reg recv function */
                tcp_recv(tpcb, TcpIp_TlsRecvCallBack);
                /* reg connect error callback function */
                tcp_err(tpcb, altcp_mbedtls_lower_err);
                /* reg send call back function */
                tcp_sent(tpcb, altcp_mbedtls_lower_sent);
                ret = E_OK;
            }

#if defined(TCPIP_TLS_CHECK)
            else
            {
                TCPIP_TLS_CHECK_OUTPUT("ssl setup error:%d\n", ret);
            }

#endif/* defined(TCPIP_TLS_CHECK) */
        }

#if defined(TCPIP_TLS_CHECK)
        else
        {
            TCPIP_TLS_CHECK_OUTPUT("get tls config error\n");
        }

#endif/* defined(TCPIP_TLS_CHECK) */
    }

    return ret;
}
#endif/* (TCPIP_TCP_TLS_CONNECTION_NUM > 0u) */


TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_TlsHandshakeDoneNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    /* listenSocketId less than TCPIP_TCP_SOCKET_MAX,meas this tls connect is tls server */
    if (socketMngPtr->listenSocketId < TCPIP_TCP_SOCKET_MAX)
    {
        /* [SWS_TCPIP_00310] */
        /* <Up_TcpAccepted> shall be called after the TLS handshake has been finished successfully */
        Std_ReturnType ret = TcpIp_TcpAcceptNofiyUplayer(socketMngPtr->listenSocketId, socketMngPtr);

        if ((Std_ReturnType)E_OK != ret)
        {
            /* if accept is not ok ,not continue notify upper layer event */
            tcp_err((struct tcp_pcb*)socketMngPtr->pcbPtr, NULL_PTR);
            /* next shall close tcp pcb */
            (void)tcp_close((struct tcp_pcb*)socketMngPtr->pcbPtr);
        }
    }
    else
    {
        /* [SWS_TCPIP_00309] */
        /* <Up_TcpConnected> this function shall be called after the TLS handshake has been finished successfully */
        TcpIp_TcpConnectNofiyUplayer(socketMngPtr);
    }

    /* [SWS_TCPIP_00345] */
    /* For static TLS connection assignment the call to <Up_TcpIpEvent> and the event type TCPIP_TLS_HANDSHAKE_SUCCEEDED
    shall take place after the call to <Up_TcpAccepted>/<Up_TcpConnected>. */
    TcpIp_EventNotifyUpLayer(socketMngPtr, TCPIP_TLS_HANDSHAKE_SUCCEEDED);
}

/*
 * SSL get accessors
 */
TCPIP_LOCAL FUNC(size_t, TCPIP_CODE) mbedtls_ssl_get_bytes_avail(const mbedtls_ssl_context* ssl)
{
    return ((ssl->in_offt == NULL) ? 0u : (size_t)(ssl->in_msglen));
}

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE)altcp_mbedtls_handle_rx_appldata(
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(TcpIp_TlsSocketDataType, AUTOMATIC, TCPIP_APPL_VAR) tlsSocketPtr)                                             /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    err_t ret = (err_t)ERR_VAL;
    int mbRet = 0;

    if ((tlsSocketPtr->flags & ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE) != 0u)
    {
        do
        {
            struct pbuf* buf = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);                                      /* PRQA S 1840 */ /* MISRA Rule 10.4 */

            if (NULL_PTR != buf)
            {
                /* decrypt application data, this pulls encrypted RX data off state->rx pbuf chain */
                mbRet = TcpIp_TlsReadData(&tlsSocketPtr->ssl_context,
                                          (unsigned char*)buf->payload, PBUF_POOL_BUFSIZE);                             /* PRQA S 1840 */ /* MISRA Rule 10.4 */

                if (mbRet > 0)
                {
                    /* trim pool pbuf to actually decoded length */
                    pbuf_realloc(buf, (u16_t)mbRet);
                    tlsSocketPtr->bio_bytes_appl += mbRet;

                    if (0u == mbedtls_ssl_get_bytes_avail(&tlsSocketPtr->ssl_context))
                    {
                        /* Record is done, now we know the share between application and protocol bytes
                        and can adjust the RX window by the protocol bytes.
                        The rest is 'recved' by the application calling our 'recved' fn. */
                        int overhead_bytes = 0;
#if defined(TCPIP_TLS_CHECK)

                        if (tlsSocketPtr->bio_bytes_read > tlsSocketPtr->bio_bytes_appl)
#endif/* defined(TCPIP_TLS_CHECK) */
                        {
                            overhead_bytes = tlsSocketPtr->bio_bytes_read - tlsSocketPtr->bio_bytes_appl;
                        }

                        altcp_mbedtls_lower_recved(tpcb, overhead_bytes);
                        tlsSocketPtr->bio_bytes_read = 0;
                        tlsSocketPtr->bio_bytes_appl = 0;
                    }

                    if (NULL_PTR == tlsSocketPtr->rx_app)
                    {
                        tlsSocketPtr->rx_app = buf;
                    }
                    else
                    {
                        pbuf_cat(tlsSocketPtr->rx_app, buf);
                    }
                }
                else
                {
                    (void)pbuf_free(buf);
                    buf = NULL_PTR;
                }

                ret = altcp_mbedtls_pass_rx_data(tpcb->callback_arg);

                if ((err_t)ERR_ABRT != ret)
                {
                    /* we hide all other errors as we retry feeding the pbuf to the app later */
                    ret = (err_t)ERR_OK;
                }
            }

#if defined(TCPIP_TLS_CHECK)
            else
            {
                TCPIP_TLS_CHECK_OUTPUT("We're short on pbufs, try again later from 'poll' or 'recv' callbacks\n");
            }

#endif/* defined(TCPIP_TLS_CHECK) */
        }
        while ((mbRet > 0) && ((err_t)ERR_OK == ret));
    }

#if defined(TCPIP_TLS_CHECK)
    else
    {
        TCPIP_TLS_CHECK_OUTPUT("handshake not done yet!\n");
    }

#endif/* defined(TCPIP_TLS_CHECK) */
    return ret;
}

TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) altcp_mbedtls_lower_recv_process(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;
    err_t ret = (err_t)ERR_VAL;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != tlsSocketPtr) && (NULL_PTR != socketMngPtr->pcbPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        boolean skipFlg = FALSE;
        struct tcp_pcb* tpcb = socketMngPtr->pcbPtr;

        if (0u == (tlsSocketPtr->flags & ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE))
        {
            /* handle connection setup (handshake not done) */
            int mbRet = TcpIp_TlsHandshakeStart(&tlsSocketPtr->ssl_context);
            /* try to send data... */
            (void)tcp_output(tpcb);

            if (tlsSocketPtr->bio_bytes_read != 0)
            {
                /* acknowledge all bytes read */
                altcp_mbedtls_lower_recved(tpcb, tlsSocketPtr->bio_bytes_read);
                tlsSocketPtr->bio_bytes_read = 0;
            }

            if ((mbRet == TLS_ERR_SSL_WANT_READ) || (mbRet == TLS_ERR_SSL_WANT_WRITE))
            {
                /* handshake not done, wait for more recv calls */
                skipFlg = TRUE;
                ret = (err_t)ERR_OK;
            }
            else if (0 != mbRet)
            {
                /* shall pending notify upper layer */
                TCPIP_TCP_SET_PENDINGEVENT(socketMngPtr);

                /* handshake failed, connection has to be closed */
                if ((err_t)ERR_OK != tcp_close(tpcb))
                {
                    tcp_abort(tpcb);
                }

                skipFlg = TRUE;
                ret = (err_t)ERR_OK;
            }
            else
            {
                tlsSocketPtr->flags |= ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE;
                /* issue "connect" callback" to upper connection (this can only happen for active open) */
                TcpIp_TlsHandshakeDoneNotifyUpLayer(socketMngPtr);

                if (NULL_PTR == tlsSocketPtr->rx)
                {
                    ret = (err_t)ERR_OK;
                    skipFlg = TRUE;
                }
            }
        }

        if (skipFlg != (boolean)TRUE)
        {
            /* handle application data */
            ret = altcp_mbedtls_handle_rx_appldata(tpcb, tlsSocketPtr);
        }
    }

    return ret;
}

/** Connected callback from lower connection (i.e. TCP).
 * Not really implemented/tested yet...
 */
TCPIP_LOCAL FUNC(err_t, TCPIP_CODE) altcp_mbedtls_lower_connected(
    P2VAR(void, AUTOMATIC, TCPIP_APPL_VAR) arg,                                                                         /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(err_t, AUTOMATIC) err)
{
    err_t ret = (err_t)ERR_VAL;
    TcpIp_SocketHandleType* socketMngPtr = (TcpIp_SocketHandleType*)arg;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr) && (NULL_PTR != socketMngPtr->tlsPtr) && (socketMngPtr->pcbPtr == tpcb))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        if ((err_t)ERR_OK == err)
        {
            ret = altcp_mbedtls_lower_recv_process(socketMngPtr);
        }

        /* input parameter err not more condition，so not care other result */
    }

    return ret;
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TlsInit(P2CONST(TcpIp_TlsConfigType, AUTOMATIC, TCPIP_APPL_DATA) tlsCfgPtr)
{
#if defined(TCPIP_TLS_CHECK)

    if (NULL_PTR != tlsCfgPtr)
#endif/* defined(TCPIP_TLS_CHECK) */
    {
#if (TCPIP_TCP_TLS_CONNECTION_GROUP_NUM > 0u)

        for (uintx index = 0; index < TLS_CONNGROUP_ARRY_SIZE; index++)
        {
            TcpIp_TlsConnectionGroupState[index] = 0u;
        }

#endif/* (TCPIP_TCP_TLS_CONNECTION_GROUP_NUM > 0u) */
#if (TCPIP_TLS_MAX_CONNECTIONS > 0u)
        TcpIp_TlsSocketIdleListPtr = &TcpIp_TlsSocketDataTable[0];

        for (uintx index = 0u; index < (TCPIP_TLS_MAX_CONNECTIONS - 1u); index++)
        {
            TcpIp_TlsSocketDataTable[index].next = &TcpIp_TlsSocketDataTable[index + 1u];
        }

        TcpIp_TlsSocketDataTable[TCPIP_TLS_MAX_CONNECTIONS - 1u].next = NULL_PTR;
#endif/* (TCPIP_TLS_MAX_CONNECTIONS > 0u) */
        TcpIp_tlsCfgPtr = tlsCfgPtr;
    }

#if defined(TCPIP_TLS_CHECK)
    else
    {
        TCPIP_TLS_CHECK_OUTPUT("TcpIp_TlsInit input parameter is null!\n");
    }

#endif/* defined(TCPIP_TLS_CHECK) */
}

FUNC(void, TCPIP_CODE) TcpIp_TlsSocketDeInit(P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr)     /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr->pcbPtr) && (NULL_PTR != socketMngPtr->tlsPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;
        TcpIp_TlsMemFree(&(tlsSocketPtr->ssl_context));
        /* tls connnect group shall set not used */
        const TcpIp_TlsConnectionConfigType* tlsCnCfgPtr = tlsSocketPtr->tlsCnnCfgPtr;
        uintx tlsConnGrpCfgId = tlsCnCfgPtr->TlsConnectionGroupRef->TlsConnectionGroupId;
        TLS_CONNGROUP_SET_UNUSED(tlsConnGrpCfgId);
        /* tls connect idle list shall change */
        tlsSocketPtr->next = TcpIp_TlsSocketIdleListPtr;
        TcpIp_TlsSocketIdleListPtr = tlsSocketPtr;
        /* free tls socket link socket management */
        socketMngPtr->tlsPtr = NULL_PTR;
    }
}


FUNC(err_t, TCPIP_CODE) TcpIp_TlsWrite(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(void, AUTOMATIC, TCPIP_APPL_VAR) dataptr,
    VAR(uint16, AUTOMATIC)len)
{
    err_t ret = (err_t)ERR_VAL;
#if defined(TCPIP_TLS_CHECK)

    if ((NULL_PTR != socketMngPtr->tlsPtr) && (NULL_PTR != socketMngPtr->pcbPtr))
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;

        if ((tlsSocketPtr->flags & ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE) != 0u)
        {
            ret = (err_t)ERR_OK;

            /* HACK: if thre is something left to send, try to flush it and only
            allow sending more if this succeeded (this is a hack because neither
            returning 0 nor TLS_ERR_SSL_WANT_WRITE worked for me) */
            if (tlsSocketPtr->ssl_context.out_left != 0u)
            {
                (void)TcpIp_TlsFlushOutput(&tlsSocketPtr->ssl_context);

                if (tlsSocketPtr->ssl_context.out_left != 0u)
                {
                    ret = (err_t)ERR_MEM;
                }
            }

            if ((err_t)ERR_OK == ret)
            {
                int retLen = TcpIp_TlsWriteData(&tlsSocketPtr->ssl_context, (const unsigned char*)dataptr, len);
                /* try to send data... */
                SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
                ret = tcp_output((struct tcp_pcb*)(socketMngPtr->pcbPtr));
                SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);

                if ((retLen >= 0) && ((err_t)ERR_OK == ret))
                {
                    if (retLen == (int)len)
                    {
                        tlsSocketPtr->flags |= ALTCP_MBEDTLS_FLAGS_APPLDATA_SENT;
                    }
                    else
                    {
                        /* @todo/@fixme: assumption: either everything sent or error */
                        TCPIP_TLS_CHECK_OUTPUT("either everything sent or error");
                        ret = (err_t)ERR_MEM;
                    }
                }

#if defined(TCPIP_TLS_CHECK)
                else
                {
                    TCPIP_TLS_CHECK_OUTPUT("mbedtls_ssl_write or tcp_output error,retLen=%d,ret=%d\n", retLen, ret);
                }

#endif/* defined(TCPIP_TLS_CHECK) */
            }
        }

#if defined(TCPIP_TLS_CHECK)
        else
        {
            TCPIP_TLS_CHECK_OUTPUT("handshake not cpmplete!\n");
        }

#endif/*     defined(TCPIP_TLS_CHECK) */
    }

    return ret;
}


FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsRecved(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(uint16, AUTOMATIC) len)
{
    Std_ReturnType ret = E_NOT_OK;
    TCPIP_UNUSED_ARG(len);
#if defined(TCPIP_TLS_CHECK)

    if (NULL_PTR != socketMngPtr->tlsPtr)
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        TcpIp_TlsSocketDataType* tlsSocketPtr = socketMngPtr->tlsPtr;

        if ((tlsSocketPtr->flags & ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE) != 0u)
        {
            uint16 lwReceLen = len;

            if ((int)lwReceLen > tlsSocketPtr->rx_passed_unrecved)
            {
                lwReceLen = (uint16)tlsSocketPtr->rx_passed_unrecved;
            }

            tlsSocketPtr->rx_passed_unrecved -= (int)lwReceLen;
            SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            tcp_recved((struct tcp_pcb*)(socketMngPtr->pcbPtr), lwReceLen);
            SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
            ret = E_OK;
        }

#if defined(TCPIP_TLS_CHECK)
        else
        {
            TCPIP_TLS_CHECK_OUTPUT("handshake not cpmplete!\n");
        }

#endif/* defined(TCPIP_TLS_CHECK) */
    }

    return ret;
}

FUNC(err_t, TCPIP_CODE) TcpIp_TlsConnet(
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,                                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_APPL_VAR) ipaddr,
    VAR(uint16, AUTOMATIC) port)
{
    SchM_Enter_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
    err_t lw_ret = (err_t)tcp_connect(tpcb, ipaddr, port, altcp_mbedtls_lower_connected);
    SchM_Exit_TcpIp(TCPIP_INSTANCE, TCPIP_AREA_TX);
    return lw_ret;
}


FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsBaseDynamicConnect(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    VAR(uint8, AUTOMATIC) tlsConnetId)
{
    Std_ReturnType ret = E_NOT_OK;
#if defined(TCPIP_TLS_CHECK)

    if (TcpIp_tlsCfgPtr->tlsConnectNum > tlsConnetId)
#endif/* defined(TCPIP_TLS_CHECK) */
    {
        const TcpIp_TlsConnectionConfigType* tlsCnnCfgPtr = &TcpIp_tlsCfgPtr->TlsConnectionPtr[tlsConnetId];
        uintx tlsConnGrpCfgId = tlsCnnCfgPtr->TlsConnectionGroupRef->TlsConnectionGroupId;

        /* based on either static configuration  or done dynamically,so  dynamically is not set exist tls connection*/
        if ((NULL_PTR == socketMngPtr->tlsPtr) && (NULL_PTR != TcpIp_TlsSocketIdleListPtr) &&
            (!TLS_CONNGROUP_IS_USED(tlsConnGrpCfgId)))
        {
            /* init handle shall be add */
            socketMngPtr->tlsPtr = TcpIp_TlsSocketIdleListPtr;
            socketMngPtr->tlsPtr->tlsCnnCfgPtr = tlsCnnCfgPtr;
            ret = TcpIp_TlsSocketInit(socketMngPtr);

            if ((Std_ReturnType)E_OK == ret)
            {
                TcpIp_TlsSocketIdleListPtr = TcpIp_TlsSocketIdleListPtr->next;
                TLS_CONNGROUP_SET_USED(tlsConnGrpCfgId);
            }
            else
            {
                /* tls socket init error,shall release socket manage link tls data point ,
                this socket shall be change normal state is not tls */
                socketMngPtr->tlsPtr = NULL_PTR;
            }
        }
    }

#if defined(TCPIP_TLS_CHECK)
    else
    {
        TCPIP_TLS_CHECK_OUTPUT("tlsConnetId:%d more than tls post build configure numbers\n", tlsConnetId);
    }

#endif/* defined(TCPIP_TLS_CHECK) */
    return ret;
}

#if (TCPIP_TCP_TLS_CONNECTION_NUM > 0u)

FUNC(void, TCPIP_CODE) TcpIp_TlsBaseStaticConnect(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,                                              /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(TcpIp_TlsConnectionInitStateType, AUTOMATIC) connType)
{
    boolean findFlg = FALSE;
    const TcpIp_TlsConnectionConfigType* tlsCnCfgPtr = NULL_PTR;
    uintx tlsConnGrpCfgId = 0;

    if (NULL_PTR != TcpIp_TlsSocketIdleListPtr)
    {
        /* find idle tls connect */
        for (uintx index = 0; (index < TCPIP_TCP_TLS_CONNECTION_NUM) && ((boolean)TRUE != findFlg); index++)
        {
            tlsCnCfgPtr = &(TcpIp_tlsCfgPtr->TlsConnectionPtr[index]);
            tlsConnGrpCfgId = tlsCnCfgPtr->TlsConnectionGroupRef->TlsConnectionGroupId;

            if (!TLS_CONNGROUP_IS_USED(tlsConnGrpCfgId)
                && (connType == tlsCnCfgPtr->TlsConnectionInitStateType)
                && (NULL_PTR != tlsCnCfgPtr->TlsPortAssignmentPtr)
                && (*tlsCnCfgPtr->TlsPortAssignmentPtr == remoteAddrPtr->port))
            {
                if ((NULL_PTR == tlsCnCfgPtr->TlsIpAddressAssignmentPtr)
                    || (NULL_PTR == tlsCnCfgPtr->TlsIpAddressAssignmentPtr->StaticIpAddressPtr))
                {
                    findFlg = TRUE;
                }
                else if (
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
                    (tlsCnCfgPtr->TlsIpAddressAssignmentPtr->StaticIpAddressPtr[0] == remoteAddrPtr->addr[0])
#else/* (TCPIP_SC1 != TCPIP_SCALABILITY_CLASS) */
                    && (tlsCnCfgPtr->TlsIpAddressAssignmentPtr->StaticIpAddressPtr[0] == remoteAddrPtr->addr[0])
                    && (tlsCnCfgPtr->TlsIpAddressAssignmentPtr->StaticIpAddressPtr[1] == remoteAddrPtr->addr[1])
                    && (tlsCnCfgPtr->TlsIpAddressAssignmentPtr->StaticIpAddressPtr[2] == remoteAddrPtr->addr[2])
                    && (tlsCnCfgPtr->TlsIpAddressAssignmentPtr->StaticIpAddressPtr[3] == remoteAddrPtr->addr[3])
#endif/* (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) */
                )
                {
                    findFlg = TRUE;
                }
                else
                {
                    /* nothing */
                }
            }
        }
    }

    if ((boolean)TRUE == findFlg)
    {
        socketMngPtr->tlsPtr = TcpIp_TlsSocketIdleListPtr;
        socketMngPtr->tlsPtr->tlsCnnCfgPtr = tlsCnCfgPtr;
        Std_ReturnType ret = TcpIp_TlsSocketInit(socketMngPtr);

        if ((Std_ReturnType)E_OK == ret)
        {
            TcpIp_TlsSocketIdleListPtr = TcpIp_TlsSocketIdleListPtr->next;
            TLS_CONNGROUP_SET_USED(tlsConnGrpCfgId);
        }
        else
        {
            /* tls socket init error,shall release socket manage link tls data point ,
            this socket shall be change normal state is not tls */
            socketMngPtr->tlsPtr = NULL_PTR;
        }
    }
}

#endif/* (TCPIP_TCP_TLS_CONNECTION_NUM > 0u) */

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsCsmRandomGenerate(
    uint8* resultPtr,
    uint32* resultLengthPtr)
{
    Std_ReturnType  Std_Return = E_NOT_OK;
    TcpIp_CsmJobsIdType randJobId = TcpIp_tlsCfgPtr->TlsCsmRandomGenerateJobRefId;
    Std_Return = Csm_RandomGenerate(randJobId, resultPtr, resultLengthPtr);
    return Std_Return;
}

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */
