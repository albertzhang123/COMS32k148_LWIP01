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
**  FILENAME    : TcpIp_Tls.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : pengfei.zhu                                                 **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Implementation for TLS                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* <VERSION>    <DATE>       <AUTHOR>        <REVISION LOG>
 *  V2.0.0    [20200828]  [pengfei.zhu]   Add Tcp_Tls function.
 *  V2.0.1    [20200928]  [pengfei.zhu]   QAC test.
 ******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "TcpIp_Tls.h"
#include "lwip/mem.h"
#include "TcpIp_Lcfg.h"
#include "TcpIp_Internal.h"
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#if (TCPIP_TCP_TLS_ENABLED == STD_ON)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"

static FUNC(int, TCPIP_CODE)
TcpIp_TlsConsumeCurrentMessage(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseCertificateChain(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckRecordIsInProcess(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsHandshakeInit(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsReadDataCheck(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckProcessMessageFinish(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckReceiveDataType(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsReadDataLoop(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteReal(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckReceiveData(
    mbedtls_ssl_context* ssl,
    uint32 nb_want
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckTimeOut(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsPrepareRecordContent(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsGetNextRecord(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseRecordHeader(
    mbedtls_ssl_context* ssl
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsUpdateOutputPointers(
    mbedtls_ssl_context* ssl,
    mbedtls_ssl_transform* transform
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsUpdateInputPointers(
    mbedtls_ssl_context* ssl,
    mbedtls_ssl_transform* transform
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsResetInOutPointers(
    mbedtls_ssl_context* ssl
);
static FUNC(uint32, TCPIP_CODE)
TcpIp_TlsGetHandshakeTotalLen(
    mbedtls_ssl_context const* ssl
);
static FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsMessageEncrypt(
    mbedtls_ssl_context* ssl
);

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Functions                                     **
*******************************************************************************/

static FUNC(int, TCPIP_CODE)
TcpIp_TlsReadDataCheck(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_ERR_INIT_VAL;

    if ((ssl == NULL_PTR) || (ssl->conf == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }
    else
    {
        if (ssl->state != TLS_HANDSHAKE_OVER)
        {
            ret = TcpIp_TlsHandshakeStart(ssl);
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckProcessMessageFinish(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    ret = TcpIp_TlsReadRecord(ssl, 1);

    if (ret == TLS_ERR_CONN_EOF)
    {
        ret = TLS_RETURN_OK;
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckReceiveDataType(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_ERR_INIT_VAL;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;

    if ((tlsConnCfgPtr->TlsConnectionInitStateType == TLS_CLIENT) &&
        ((ssl->in_msg[0] != TLS_HS_TYPE_HELLO_REQUEST) || (ssl->in_hslen  != TLS_HANDSHAKE_HEADER_LENGTH)))
    {
        ret = TLS_ERR_UNEXPECTED_MESSAGE;           /*handshake received (not HelloRequest)*/
    }

    if ((tlsConnCfgPtr->TlsConnectionInitStateType == TLS_SERVER) &&
        (ssl->in_msg[0] != TLS_HS_TYPE_CLIENT_HELLO))
    {
        ret = TLS_ERR_UNEXPECTED_MESSAGE;           /*handshake received (not ClientHello)*/
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsReadDataLoop(
    mbedtls_ssl_context* ssl
)
{
    int ret;

    while (ssl->in_offt == NULL_PTR)
    {
        if ((ssl->f_get_timer != NULL_PTR) &&
            (ssl->f_get_timer(ssl->p_timer) == -1))     /* Start timer if not already running */
        {
            //          ssl_set_timer( ssl, ssl->conf->read_timeout );
        }

        ret = TcpIp_TlsCheckProcessMessageFinish(ssl);

        if (ret >= 0)
        {
            if ((ssl->in_msglen == 0u) && (ssl->in_msgtype == TLS_MSG_TYPE_APPLICATION_DATA))
            {
                ret = TcpIp_TlsCheckProcessMessageFinish(ssl);
            }

            if (ret >= 0)
            {
                if (ssl->in_msgtype == TLS_MSG_TYPE_HANDSHAKE)
                {
                    ret = TcpIp_TlsCheckReceiveDataType(ssl);

                    if (ret >= 0)
                    {
                        continue;
                    }
                }

                if (ssl->in_msgtype != TLS_MSG_TYPE_APPLICATION_DATA)
                {
                    ret = TLS_ERR_UNEXPECTED_MESSAGE;
                }

                if (ret >= 0)
                {
                    ssl->in_offt = ssl->in_msg;

                    if (ssl->state == TLS_HANDSHAKE_OVER)
                    {
                        // ssl_set_timer( ssl, 0u );
                    }
                }
            }
        }

        if (ret >= 0)
        {
            break;
        }
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsHandshakeInit(
    mbedtls_ssl_context* ssl
)
{
    int ret;

    /* Clear old handshake information if present */
    if (ssl->transform_negotiate != NULL_PTR)
    {
        (void)memset(ssl->transform_negotiate, 0, sizeof(mbedtls_ssl_transform));
    }

    if (ssl->session_negotiate != NULL_PTR)
    {
        (void)memset(ssl->session_negotiate, 0, sizeof(mbedtls_ssl_session));
    }

    if (ssl->handshake != NULL_PTR)
    {
        (void)memset(ssl->handshake, 0, sizeof(mbedtls_ssl_handshake_params));
    }

    if (ssl->transform_negotiate == NULL_PTR)
    {
        ssl->transform_negotiate = TcpIp_TlsMemCalloc(1u, sizeof(mbedtls_ssl_transform));
    }

    if (ssl->session_negotiate == NULL_PTR)
    {
        ssl->session_negotiate = TcpIp_TlsMemCalloc(1u, sizeof(mbedtls_ssl_session));
    }

    if (ssl->handshake == NULL_PTR)
    {
        ssl->handshake = TcpIp_TlsMemCalloc(1u, sizeof(mbedtls_ssl_handshake_params));
    }

    /* All pointers should exist and can be directly freed without issue */
    if ((ssl->handshake == NULL_PTR) ||
        (ssl->transform_negotiate == NULL_PTR) ||
        (ssl->session_negotiate == NULL_PTR))
    {
        (void)TcpIp_TlsMemFree(ssl->handshake);
        (void)TcpIp_TlsMemFree(ssl->transform_negotiate);
        (void)TcpIp_TlsMemFree(ssl->session_negotiate);
        ssl->handshake = NULL_PTR;
        ssl->transform_negotiate = NULL_PTR;
        ssl->session_negotiate = NULL_PTR;
        ret = TLS_ERR_ALLOC_FAILED;
    }
    else
    {
        /* Initialize structures */
        (void)memset(ssl->session_negotiate, 0, sizeof(mbedtls_ssl_session));
        (void)memset(ssl->transform_negotiate, 0, sizeof(mbedtls_ssl_transform));
        (void)memset(ssl->handshake, 0, sizeof(mbedtls_ssl_handshake_params));
        ret = TLS_RETURN_OK;
    }

    return ret;
}

static FUNC(void, TCPIP_CODE)
TcpIp_TlsResetInOutPointers(
    mbedtls_ssl_context* ssl
)
{
    /* Set the incoming and outgoing record pointers. */
    ssl->out_hdr = ssl->out_buf + 8u;
    ssl->in_hdr  = ssl->in_buf  + 8u;
    /* Derive other internal pointers. */
    TcpIp_TlsUpdateOutputPointers(ssl, NULL_PTR /* no transform enabled */);
    TcpIp_TlsUpdateInputPointers(ssl, NULL_PTR /* no transform enabled */);
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteReal(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
)
{
    int ret = TcpIp_TlsGetMaxOutRecordPayload(ssl);
    const uint32 max_len = (uint32) ret;

    if (ret >= 0)
    {
        if (len > max_len)
        {
            len = max_len;
        }

        if (ssl->out_left != 0u)
        {
            ret = (int)TcpIp_TlsFlushOutput(ssl);
        }
        else
        {
            ssl->out_msglen  = len;
            ssl->out_msgtype = TLS_MSG_TYPE_APPLICATION_DATA;
            (void)memcpy(ssl->out_msg, buf, len);
            ret = TcpIp_TlsWriteRecord(ssl, TLS_FORCE_FLUSH);
        }

        if (ret == TLS_RETURN_OK)
        {
            ret = (int) len;
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckReceiveData(
    mbedtls_ssl_context* ssl,
    uint32 nb_want
)
{
    int ret = TLS_RETURN_OK;

    if ((ssl->f_recv == NULL_PTR) && (ssl->f_recv_timeout == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }

    if (ret == TLS_RETURN_OK)
    {
        if (nb_want > (TLS_IN_BUFFER_LEN - (uint32)(ssl->in_hdr - ssl->in_buf)))
        {
            ret = TLS_ERR_BAD_INPUT_DATA;
        }
    }

    return ret;
}

/*
 * Return -1 is timer is expired, 0 if it isn't.
 */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckTimeOut(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;

    if (ssl->f_get_timer == NULL_PTR)
    {
        ret = TLS_RETURN_OK;
    }

    if (ssl->f_get_timer(ssl->p_timer) == 2)
    {
        ret = -1;
    }

    return ret;
}


static FUNC(uint32, TCPIP_CODE)
TcpIp_TlsGetHandshakeTotalLen(
    mbedtls_ssl_context const* ssl
)
{
    return ((ssl->in_msg[1] << 16) |
            (ssl->in_msg[2] <<  8) |
            ssl->in_msg[3]);
}

static FUNC(void, TCPIP_CODE)
TcpIp_TlsUpdateInputPointers(
    mbedtls_ssl_context* ssl,
    mbedtls_ssl_transform* transform
)
{
    ssl->in_ctr = ssl->in_hdr - 8u;
    ssl->in_len = ssl->in_hdr + 3u;
    ssl->in_iv  = ssl->in_hdr + 5u;

    /* Offset in_msg from in_iv to allow space for explicit IV, if used. */
    if ((transform != NULL_PTR) &&
        (ssl->minor_ver >= TLS_MINOR_VERSION_2))
    {
        ssl->in_msg = ssl->in_iv + transform->ivlen - transform->fixed_ivlen;
    }
    else
    {
        ssl->in_msg = ssl->in_iv;
    }
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckRecordIsInProcess(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;

    if (ssl->in_msglen > 0u)
    {
        ret = 1;
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsConsumeCurrentMessage(
    mbedtls_ssl_context* ssl
)
{
    int ret;

    /*  Handshake messages */
    if (ssl->in_hslen != 0u)
    {
        if (ssl->in_offt != NULL_PTR)
        {
            ret = TLS_ERR_INTERNAL_ERROR;
        }
        else
        {
            /* Get next Handshake message in the current record */
            if (ssl->in_hslen < ssl->in_msglen)
            {
                ssl->in_msglen -= ssl->in_hslen;
                (void)memmove(ssl->in_msg, ssl->in_msg + ssl->in_hslen, ssl->in_msglen);
            }
            else
            {
                ssl->in_msglen = 0u;
            }

            ssl->in_hslen   = 0u;
        }
    }
    /* Application data */
    else if (ssl->in_offt != NULL_PTR)
    {
        ret = TLS_RETURN_OK;
    }
    /* Everything else (CCS & Alerts) */
    else
    {
        ssl->in_msglen = 0u;
        ret = TLS_RETURN_OK;
    }

    return ret ;
}

/* Once the certificate message is read, parse it into a cert chain and
 * perform basic checks, but leave actual verification to the caller */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseCertificateChain(
    mbedtls_ssl_context* ssl
)
{
    int ret, cmpVal;
    uint32 i, msgLen;
    Std_ReturnType Std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    KeyM_CertDataType remoteCertInfo;

    if ((tlsConnCfgPtr->TlsConnectionInitStateType  == TLS_SERVER) &&
        (ssl->minor_ver != TLS_MINOR_VERSION_0))
    {
        cmpVal = memcmp(ssl->in_msg + TLS_HANDSHAKE_HEADER_LENGTH, "\0\0\0", 3u);

        if ((ssl->in_hslen   == (3u + TLS_HANDSHAKE_HEADER_LENGTH)) &&
            (ssl->in_msgtype == TLS_MSG_TYPE_HANDSHAKE)    &&
            (ssl->in_msg[0]  == TLS_HS_TYPE_CERTIFICATE)  &&
            (cmpVal == 0))
        {
            /* The client was asked for a certificate but didn't send
               one. The client should know what's going on, so we
               don't send an alert. */
            ssl->session_negotiate->verify_result = TLS_X509_BADCERT_MISSING;
            ret = TLS_ERR_NO_CLIENT_CERTIFICATE;
        }
    }

    if (ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE)
    {
        ret = TLS_ERR_UNEXPECTED_MESSAGE;
    }

    if ((ssl->in_msg[0] != TLS_HS_TYPE_CERTIFICATE) ||
        (ssl->in_hslen < (TLS_HANDSHAKE_HEADER_LENGTH + 3u + 3u)))
    {
        ret = TLS_ERR_BAD_HS_CERTIFICATE;
    }

    i = TLS_HANDSHAKE_HEADER_LENGTH;
    /*  Same message structure as in TcpIp_TlsWriteCertificate() */
    msgLen = (ssl->in_msg[i + 1u] << 8u) | ssl->in_msg[i + 2u];

    if ((ssl->in_msg[i] != 0u) || (ssl->in_hslen != (msgLen + 3u + TLS_HANDSHAKE_HEADER_LENGTH)))
    {
        ret = TLS_ERR_BAD_HS_CERTIFICATE;
    }

    i += 3u;

    while (i < ssl->in_hslen)
    {
        if ((i + 3u) > ssl->in_hslen)
        {
            ret = TLS_ERR_BAD_HS_CERTIFICATE;
        }

        if (ssl->in_msg[i] != 0u)
        {
            ret = TLS_ERR_BAD_HS_CERTIFICATE;
        }

        msgLen = (ssl->in_msg[i + 1u] << 8u) | ssl->in_msg[i + 2u];
        i += 3u;

        if ((msgLen < 128u) || ((i + msgLen) > ssl->in_hslen))
        {
            ret = TLS_ERR_BAD_HS_CERTIFICATE;
        }

        if (ret == TLS_RETURN_OK)
        {
            remoteCertInfo.certData = (uint8*)(ssl->session_negotiate->peer_cert);
            remoteCertInfo.certDataLength = msgLen;
            /* SWS_TCPIP_00338 : set certificate with the reference TcpIpTlsCipherKeyMRemoteCertificate */
            Std_Return = KeyM_SetCertificate(tlsConnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMRemoteCertificateId,
                                             &remoteCertInfo);

            if (E_OK == Std_Return)
            {
                /*Std_Return = Csm_CertificateParse(
                               tlsConnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMRemoteCertificateId);*/
            }

            if (E_OK != Std_Return)
            {
                ret = TLS_ERR_BAD_HS_CERTIFICATE;
            }

            i += msgLen;
        }
    }

    return ret;
}


/* Once ssl->out_hdr as the address of the beginning of the
 * next outgoing record is set, deduce the other pointers.
 *
 * Note: For TLS, we save the implicit record sequence number
 *       (entering MAC computation) in the 8 bytes before ssl->out_hdr,
 *       and the caller has to make sure there's space for this.
 */
static FUNC(void, TCPIP_CODE)
TcpIp_TlsUpdateOutputPointers(
    mbedtls_ssl_context* ssl,
    mbedtls_ssl_transform* transform
)
{
    ssl->out_ctr = ssl->out_hdr - 8;
    ssl->out_len = ssl->out_hdr + 3;
    ssl->out_iv  = ssl->out_hdr + 5;

    /* Adjust out_msg to make space for explicit IV, if used. */
    if ((transform != NULL_PTR) &&
        (ssl->minor_ver >= TLS_MINOR_VERSION_2))
    {
        ssl->out_msg = ssl->out_iv + transform->ivlen - transform->fixed_ivlen;
    }
    else
    {
        ssl->out_msg = ssl->out_iv;
    }
}

static FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsMessageEncrypt(
    mbedtls_ssl_context* ssl
)
{
    uint8* mac = NULL_PTR;
    uint8* enResult = NULL_PTR;
    uint32 macLen, enResultLen;
    Std_ReturnType Std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;

    if ((ssl->session_out == NULL_PTR) || (ssl->transform_out == NULL_PTR))
    {
        Std_Return = E_NOT_OK;
    }
    else
    {
        if (TRUE == tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsCiphersuiteDefinitionRef->TlsUseAEADCiper)
        {
            /* use AEAD encrypt */
        }
        else
        {
            /* Add MAC  */
            Std_Return = Csm_MacGenerate(tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
                                         TlsCipherCsmMacGenerateJobRef,
                                         CRYPTO_OPERATIONMODE_START,
                                         ssl->out_msg,
                                         ssl->out_msglen,
                                         mac,
                                         &macLen);
            ssl->out_msg = mac;
            ssl->out_msglen = macLen;
        }

        if (E_OK == Std_Return)
        {
            /* encrypt message */
            Std_Return = Csm_Encrypt(tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
                                     TlsCipherCsmEncryptJobRef,
                                     CRYPTO_OPERATIONMODE_START,
                                     ssl->out_msg,
                                     ssl->out_msglen,
                                     enResult,
                                     &enResultLen);
            ssl->out_msg = enResult;
            ssl->out_msglen = enResultLen;
        }
    }

    return Std_Return;
}


FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteVersion(
    mbedtls_ssl_context* ssl,
    uint8 ver[2]
)
{
    ssl->major_ver = ssl->conf->min_major_ver;
    ssl->minor_ver = ssl->conf->min_minor_ver;
    ver[0] = (uint8) ssl->major_ver;
    ver[1] = (uint8) ssl->major_ver;
}


/*
 * ContentType type;
 * ProtocolVersion version;
 * uint16 length;
 *
 * Return 0 if header looks sane (and, for DTLS, the record is expected)
 * TLS_ERR_INVALID_RECORD if the header looks bad,
 * TLS_ERR_UNEXPECTED_RECORD (DTLS only) if sane but unexpected.
 *
 * With DTLS, TcpIp_TlsReadRecord() will:
 * 1. proceed with the record if this function returns 0
 * 2. drop only the current record if this function returns UNEXPECTED_RECORD
 * 3. return CLIENT_RECONNECT if this function return that value
 * 4. drop the whole datagram if this function returns anything else.
 * Point 2 is needed when the peer is resending, and we have already received
 * the first record from a datagram but are still waiting for the others.
 */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseRecordHeader(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint16 major_ver, minor_ver;
    ssl->in_msgtype = (uint32)ssl->in_hdr[0];
    ssl->in_msglen = (ssl->in_len[0] << 8) | ssl->in_len[1];
    TcpIp_TlsReadVersion(&major_ver, &minor_ver, ssl->in_hdr + 1);

    /* Check record type */
    if ((ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE) &&
        (ssl->in_msgtype != TLS_MSG_TYPE_ALERT) &&
        (ssl->in_msgtype != TLS_MSG_TYPE_CHANGE_CIPHER_SPEC) &&
        (ssl->in_msgtype != TLS_MSG_TYPE_APPLICATION_DATA))
    {
        ret = TLS_ERR_INVALID_RECORD;
    }
    else
    {
        /* Check version */
        if ((major_ver != ssl->major_ver) || (minor_ver > ssl->conf->max_minor_ver))
        {
            ret = TLS_ERR_INVALID_RECORD;
        }
        else
        {
            /* Check length against the size of our buffer */
            if (ssl->in_msglen <= (TLS_IN_BUFFER_LEN - (uint32)(ssl->in_msg - ssl->in_buf)))
            {
                /* Check length against bounds of the current transform and version */
                if (ssl->transform_in != NULL_PTR)
                {
                    if (ssl->in_msglen >= ssl->transform_in->minlen)
                    {
                        /* TLS encrypted messages can have up to 256 bytes of padding */
                        if ((ssl->minor_ver >= TLS_MINOR_VERSION_1) &&
                            (ssl->in_msglen > (ssl->transform_in->minlen + TLS_IN_CONTENT_LEN + 256u)))
                        {
                            ret = TLS_ERR_INVALID_RECORD;
                        }
                    }
                    else
                    {
                        ret = TLS_ERR_INVALID_RECORD;
                    }
                }
                else
                {
                    if ((ssl->in_msglen < 1u) || (ssl->in_msglen > TLS_IN_CONTENT_LEN))
                    {
                        ret = TLS_ERR_INVALID_RECORD;
                    }
                }
            }
            else
            {
                ret = TLS_ERR_INVALID_RECORD;
            }
        }
    }

    return ret;
}

/* If applicable, decrypt (and decompress) record content */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsPrepareRecordContent(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;

    if (ssl->transform_in != NULL_PTR)
    {
        ret = (int)TcpIp_TlsMessageEncrypt(ssl);

        if (ret == TLS_RETURN_OK)
        {
            if (ssl->in_msglen > TLS_IN_CONTENT_LEN)
            {
                ret = TLS_ERR_INVALID_RECORD;
            }
        }
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsGetNextRecord(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    ret = TcpIp_TlsFetchInput(ssl, TLS_HEADER_LENGTH);

    if (0 == ret)
    {
        ret = TcpIp_TlsParseRecordHeader(ssl);

        if (ret == TLS_RETURN_OK)
        {
            ret = TcpIp_TlsFetchInput(ssl, TLS_HEADER_LENGTH + ssl->in_msglen);

            if (ret == TLS_RETURN_OK)
            {
                ssl->in_left = 0;
                ret = TcpIp_TlsPrepareRecordContent(ssl);
            }
        }
    }

    return ret;
}


/*
 * Inputs:
 *  - ssl->out_msglen: 4 + actual handshake message len
 *      (4 is the size of handshake headers for TLS)
 *  - ssl->out_msg[0]: the handshake type (ClientHello, ServerHello, etc)
 *  - ssl->out_msg + 4: the handshake message body
 *
 * Outputs, ie state before passing to flight_append() or write_record():
 *   - ssl->out_msglen: the length of the record contents
 *      (including handshake headers but excluding record headers)
 *   - ssl->out_msg: the record contents (handshake headers + content)
 */
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteHandshakeMessage(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    Std_ReturnType Std_Return = E_NOT_OK;
    const uint32 hs_len = ssl->out_msglen - 4u;
    const uint8 hs_type = ssl->out_msg[0u];
    uint8* result = NULL_PTR;
    uint32 resultLength;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    TcpIp_CsmJobsIdType hashJobId = tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
                                    TlsConnectionHandshakeRef->TlsCsmHashVerifyJobRef;

    if ((ssl->out_msgtype != TLS_MSG_TYPE_HANDSHAKE) &&
        (ssl->out_msgtype != TLS_MSG_TYPE_CHANGE_CIPHER_SPEC))
    {
        /* should never happen */
        ret = TLS_ERR_INTERNAL_ERROR;
    }
    else
    {
        /* Whenever we send anything different from a
             * HelloRequest we should be in a handshake - double check. */
        if (!((ssl->out_msgtype == TLS_MSG_TYPE_HANDSHAKE) &&
              (hs_type == TLS_HS_TYPE_HELLO_REQUEST)) &&
            (ssl->handshake == NULL_PTR))
        {
            /* should never happen */
            ret = TLS_ERR_INTERNAL_ERROR;
        }
        else
        {
            if (ssl->out_msglen > TLS_OUT_CONTENT_LEN)
            {
                /* should never happen */
                ret = TLS_ERR_INTERNAL_ERROR;
            }
            else
            {
                /* Fill handshake headers */
                if (ssl->out_msgtype == TLS_MSG_TYPE_HANDSHAKE)
                {
                    ssl->out_msg[1] = (uint8)(hs_len >> 16u);
                    ssl->out_msg[2] = (uint8)(hs_len >>  8u);
                    ssl->out_msg[3] = (uint8)(hs_len);

                    /* Update running hashes of handshake messages seen */
                    if (hs_type != TLS_HS_TYPE_HELLO_REQUEST)
                    {
                        Std_Return = Csm_Hash(hashJobId,
                                              CRYPTO_OPERATIONMODE_UPDATE,
                                              ssl->out_msg,
                                              ssl->out_msglen,
                                              result,
                                              &resultLength
                                             );
                    }
                }

                if (E_OK == Std_Return)
                {
                    ssl->out_msg = result;
                    ssl->out_msglen = resultLength;
                    /* Either send now, or just save to be sent (and resent) later */
                    ret = TcpIp_TlsWriteRecord(ssl, TLS_FORCE_FLUSH);
                }
                else
                {
                    ret = TLS_ERR_CSM_HASH_ERROR;
                }
            }
        }
    }

    return ret;
}

/*  Write current record */
FUNC(int, TCPIP_CODE) TcpIp_TlsWriteRecord(
    mbedtls_ssl_context* ssl,
    uint8_t force_flush
)
{
    int ret;
    uint32 len = ssl->out_msglen;
    uint8_t flush = force_flush;
    uint16 i;
    uint32 protected_record_size;
    ssl->out_hdr[0u] = (uint8) ssl->out_msgtype;
    TcpIp_TlsWriteVersion(ssl, ssl->out_hdr + 1u);
    (void)memcpy(ssl->out_ctr, ssl->cur_out_ctr, 8u);
    ssl->out_len[0u] = (uint8)(len >> 8u);
    ssl->out_len[1u] = (uint8)(len);

    if (ssl->transform_out != NULL_PTR)
    {
        ret = (int)TcpIp_TlsMessageEncrypt(ssl);     /* encrypt message */

        if (ret == TLS_RETURN_OK)
        {
            len = ssl->out_msglen;
            ssl->out_len[0u] = (uint8)(len >> 8u);
            ssl->out_len[1u] = (uint8)(len);
        }
        else
        {
            ret = TLS_ERR_ENCRYPT_ERROR;
        }
    }

    protected_record_size = len + TLS_HEADER_LENGTH;
    /* output record sent to network */
    ssl->out_left += protected_record_size;
    ssl->out_hdr  += protected_record_size;
    TcpIp_TlsUpdateOutputPointers(ssl, ssl->transform_out);

    for (i = 8u; i > 0u; i--)
    {
        if (++ssl->cur_out_ctr[i - 1u] != 0u)
        {
            break;
        }
    }

    /* The loop goes to its end if the counter is wrapping */
    if (i == 0u)
    {
        /* outgoing message counter would wrap */
        ret = TLS_ERR_INTERNAL_ERROR;
    }

    if (flush == TLS_FORCE_FLUSH)
    {
        ret = (int)TcpIp_TlsFlushOutput(ssl);
    }

    return ret;
}

/* Flush any data not yet written */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsFlushOutput(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint8* buf;
    Std_ReturnType Std_Return = E_NOT_OK;

    if ((ssl->f_send == NULL_PTR) || (ssl->out_left == 0u))
    {
        Std_Return = E_NOT_OK;
    }
    else
    {
        while (ssl->out_left > 0u)
        {
            buf = ssl->out_hdr - ssl->out_left;
            ret = ssl->f_send(ssl->p_bio, buf, ssl->out_left);

            if (ret <= 0)        /* send failed */
            {
                ret = TLS_ERR_SEND_FAILED;
                break;
            }

            if ((uint32)ret > ssl->out_left)
            {
                ret = TLS_ERR_INTERNAL_ERROR;
                break;
            }

            ssl->out_left -= (uint32)ret;
        }

        Std_Return = E_OK;
        ssl->out_hdr = ssl->out_buf + 8u;
        TcpIp_TlsUpdateOutputPointers(ssl, ssl->transform_out);
    }

    return Std_Return;
}


static const mbedtls_ssl_ciphersuite_t ciphersuite_definitions[1] =
{
    0/* todo:pengfei.zhu */
};

const mbedtls_ssl_ciphersuite_t*
TcpIp_TlsGetCiphersuiteFromId(
    int ciphersuite
)
{
    const mbedtls_ssl_ciphersuite_t* cur = ciphersuite_definitions;

    while (cur->id != 0)
    {
        if (cur->id == ciphersuite)
        {
            break;
        }
        else
        {
            cur++;
        }
    }

    if (cur->id == 0)
    {
        cur = NULL_PTR;
    }

    return cur;
}


/* Generate random bytes */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsGenerateRandomNumber(
    mbedtls_ssl_context* ssl
)
{
    uint32 p2RandLength = 28u;
    uint32 currentTimeUtc = 0;
    Std_ReturnType Std_Return = E_NOT_OK;
    uint8* randResultPtr = ssl->handshake->randbytes;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    TcpIp_Up_TlsGetCurrentTimeStampType getCurrentTimeStampFuncPtr = tlsConnCfgPtr->Up_TlsGetCurrentTimeStampFuncPtr;

    if (NULL_PTR != getCurrentTimeStampFuncPtr)
    {
        Std_Return = getCurrentTimeStampFuncPtr(&currentTimeUtc);

        if (E_OK == Std_Return)
        {
            *randResultPtr++ = (uint8)(currentTimeUtc >> 24);
            *randResultPtr++ = (uint8)(currentTimeUtc >> 16);
            *randResultPtr++ = (uint8)(currentTimeUtc >>  8);
            *randResultPtr++ = (uint8)(currentTimeUtc);
        }
    }

    if (E_NOT_OK == Std_Return)
    {
        *randResultPtr++ = 0;
        *randResultPtr++ = 0;
        *randResultPtr++ = 0;
        *randResultPtr++ = 0;
    }

    /* 28 bytes of random number */
    Std_Return = TcpIp_TlsCsmRandomGenerate(randResultPtr, &p2RandLength);
    return Std_Return;
}

/* Perform the TLS handshake */
FUNC(int, TCPIP_CODE)
TcpIp_TlsHandshakeStart(
    mbedtls_ssl_context* ssl
)
{
    int ret;

    if ((ssl == NULL_PTR) || (ssl->conf == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }

    while (ssl->state != TLS_HANDSHAKE_OVER)
    {
        ret = TcpIp_TlsHandshakeStep(ssl);

        if (ret != TLS_RETURN_OK)
        {
            break;
        }
    }

    return ret ;
}

/* Perform a single step of the SSL handshake */
FUNC(int, TCPIP_CODE)
TcpIp_TlsHandshakeStep(
    mbedtls_ssl_context* ssl
)
{
    int ret = 0;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;

    if ((ssl == NULL_PTR) || (ssl->conf == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }

    if (ret == TLS_RETURN_OK)
    {
        if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_CLIENT)
        {
            ret = TcpIp_TlsClientHandshakeStep(ssl);
        }

        if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_SERVER)
        {
            ret = TcpIp_TlsServerHandshakeStep(ssl);
        }
    }

    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsParseCertificate(
    mbedtls_ssl_context* ssl
)
{
    int ret = 0;
    Std_ReturnType Std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    const mbedtls_ssl_ciphersuite_t* const ciphersuite_info = ssl->transform_negotiate->ciphersuite_info;
#if (STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)
    const uint32 authmode = ssl->handshake->sni_authmode != TLS_VERIFY_UNSET
                            ? ssl->handshake->sni_authmode
                            : ssl->conf->authmode;
#else
    const uint32 authmode = ssl->conf->authmode;
#endif

    /*parse certificate*/
    if (((ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_PSK) ||
         (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_DHE_PSK) ||
         (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECDHE_PSK)) ||
        ((tlsConnCfgPtr->TlsConnectionInitStateType == TLS_SERVER) &&
         (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA_PSK)))
    {
        /* <= skip parse certificate */
        ssl->state++;
    }
    else
    {
        if ((tlsConnCfgPtr->TlsConnectionInitStateType == TLS_SERVER) && (authmode == TLS_VERIFY_NONE))
        {
            /* <= skip parse certificate */
            ssl->session_negotiate->verify_result = TLS_X509_BADCERT_SKIP_VERIFY;
            ssl->state++;
        }
        else
        {
            ret = TcpIp_TlsReadRecord(ssl, 1);

            if (ret != TLS_RETURN_OK)
            {
                ret = TLS_ERR_READ_ERR;
            }
            else
            {
                ret = TcpIp_TlsParseCertificateChain(ssl);

                if (ret != TLS_RETURN_OK)
                {
                    if ((ret == TLS_ERR_NO_CLIENT_CERTIFICATE) && (authmode == TLS_VERIFY_OPTIONAL))
                    {
                        ret = TLS_RETURN_OK;
                        ssl->state++;
                    }
                    else
                    {
                        if (authmode != TLS_VERIFY_NONE)
                        {
                            if (Std_Return == E_OK)
                            {
                                Std_Return = KeyM_VerifyCertificate(
                                                 tlsConnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMRemoteCertificateId);
                            }

                            if (Std_Return != E_OK)
                            {
                                ret = TLS_ERR_CERTIFICATE_VERIFY_FAILED;
                            }

                            if ((authmode == TLS_VERIFY_OPTIONAL) &&
                                ((ret == TLS_ERR_CERTIFICATE_VERIFY_FAILED) ||
                                 (ret == TLS_ERR_BAD_HS_CERTIFICATE)))
                            {
                                ret = TLS_RETURN_OK;
                            }

                            if (authmode == TLS_VERIFY_REQUIRED)
                            {
                                ret = TLS_ERR_CA_CHAIN_REQUIRED;
                            }
                        }
                    }
                }

                ssl->state++;
            }
        }
    }

    return ret ;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsPrepareHandshakeRecord(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;

    if (ssl->in_msglen < TLS_HANDSHAKE_HEADER_LENGTH)
    {
        ret = TLS_ERR_INVALID_RECORD;
    }
    else
    {
        ssl->in_hslen = TLS_HANDSHAKE_HEADER_LENGTH + TcpIp_TlsGetHandshakeTotalLen(ssl);

        /* With TLS we don't handle fragmentation (for now) */
        if (ssl->in_msglen < ssl->in_hslen)
        {
            ret = TLS_ERR_FEATURE_UNAVAILABLE;
        }
    }

    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsHandleMessageType(
    mbedtls_ssl_context* ssl
)
{
    int ret;

    /* Handle particular types of records */
    if (ssl->in_msgtype == TLS_MSG_TYPE_HANDSHAKE)
    {
        ret = TcpIp_TlsPrepareHandshakeRecord(ssl);
    }

    if (ssl->in_msgtype == TLS_MSG_TYPE_CHANGE_CIPHER_SPEC)
    {
        if ((ssl->in_msglen != 1u) || (ssl->in_msg[0] != 1u))
        {
            ret = TLS_ERR_INVALID_RECORD;
        }
    }

    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsReadRecord(
    mbedtls_ssl_context* ssl,
    uint8 update_hs_digest
)
{
    int ret = TLS_RETURN_OK;

    if (ssl->keep_current_message == 0)
    {
        do
        {
            ret = TcpIp_TlsConsumeCurrentMessage(ssl);

            if (ret == TLS_RETURN_OK)
            {
                if (TcpIp_TlsCheckRecordIsInProcess(ssl) == 0)
                {
                    ret = TcpIp_TlsGetNextRecord(ssl);

                    if (ret == TLS_ERR_CONTINUE_PROCESSING)
                    {
                        continue;
                    }

                    if (ret != TLS_RETURN_OK)
                    {
                        ret = TLS_ERR_READ_ERR;
                    }
                }

                if (ret == TLS_RETURN_OK)
                {
                    ret = TcpIp_TlsHandleMessageType(ssl);
                }
            }
        }
        while ((TLS_ERR_NON_FATAL == ret) || (TLS_ERR_CONTINUE_PROCESSING == ret));

        if (ret == TLS_RETURN_OK)
        {
            if ((ssl->in_msgtype == TLS_MSG_TYPE_HANDSHAKE) && (update_hs_digest == 1u))
            {
                TcpIp_TlsUpdateHandshakeStatus(ssl);
            }
        }
    }
    else
    {
        ssl->keep_current_message = 0;
    }

    return ret;
}


FUNC(void, TCPIP_CODE)
TcpIp_TlsUpdateHandshakeStatus(
    mbedtls_ssl_context* ssl
)
{
    uint8* resultVal = NULL_PTR;
    uint32 resultLength;
    Std_ReturnType Std_Return = E_NOT_OK;
    mbedtls_ssl_handshake_params* const hs = ssl->handshake;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    TcpIp_CsmJobsIdType hashJobId =
        tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsConnectionHandshakeRef->TlsCsmHashVerifyJobRef;

    if ((ssl->state != TLS_HANDSHAKE_OVER) && (hs != NULL_PTR))
    {
        Std_Return =  Csm_Hash(hashJobId,
                               CRYPTO_OPERATIONMODE_UPDATE,
                               ssl->in_msg,
                               ssl->in_hslen,
                               resultVal,
                               &resultLength
                              );

        if (E_OK == Std_Return)
        {
            ssl->in_msg = resultVal;
            ssl->in_hslen = resultLength;
        }
    }
}


FUNC(int, TCPIP_CODE)
TcpIp_TlsSendErrorAlertMessage(
    mbedtls_ssl_context* ssl,
    uint8 level,
    uint8 message
)
{
    int ret = TLS_RETURN_OK;

    if ((ssl == NULL_PTR) || (ssl->conf == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }

    ssl->out_msgtype = TLS_MSG_TYPE_ALERT;
    ssl->out_msglen = 2u;
    ssl->out_msg[0] = level;
    ssl->out_msg[1] = message;
    ret = TcpIp_TlsWriteRecord(ssl, TLS_FORCE_FLUSH);

    if (ret != TLS_RETURN_OK)
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }

    return ret;
}

/*
 * Fill the input message buffer by appending data to it.
 * The amount of data already fetched is in ssl->in_left.
 *
 * If we return 0, is it guaranteed that (at least) nb_want bytes are
 * available (from this read and/or a previous one). Otherwise, an error code
 * is returned (possibly EOF or WANT_READ).
 *
 * With stream transport (TLS) on success ssl->in_left == nb_want
 */
FUNC(int, TCPIP_CODE)
TcpIp_TlsFetchInput(
    mbedtls_ssl_context* ssl,
    uint32 nb_want
)
{
    int ret = TLS_RETURN_OK;
    uint32 len;
    ret = TcpIp_TlsCheckReceiveData(ssl, nb_want);

    if (ret == TLS_RETURN_OK)
    {
        while (ssl->in_left < nb_want)
        {
            len = nb_want - ssl->in_left;
            ret = TcpIp_TlsCheckTimeOut(ssl);

            if (ret != TLS_RETURN_OK)
            {
                ret = TLS_ERR_TIMEOUT;
            }
            else
            {
                if (ssl->f_recv_timeout != NULL_PTR)
                {
                    ret = ssl->f_recv_timeout(ssl->p_bio, ssl->in_hdr + ssl->in_left, len, ssl->conf->read_timeout);
                }
                else
                {
                    ret = ssl->f_recv(ssl->p_bio, ssl->in_hdr + ssl->in_left, len);
                }
            }

            if (ret == TLS_RETURN_OK)
            {
                ret = TLS_ERR_CONN_EOF;
            }
            else if (ret < 0)
            {
                /* return ret */
            }
            else
            {
                if ((uint32)ret > len)
                {
                    ret = TLS_ERR_INTERNAL_ERROR;
                }
                else
                {
                    ssl->in_left += (uint32)ret;
                    ret = TLS_RETURN_OK;
                }
            }
        }
    }

    return ret;
}


/*Get local/remote certificate from keyM*/
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_GetCertificate(
    VAR(TcpIp_KeyMCertificateIdType, AUTOMATIC) CertId,
    P2VAR(KeyM_CertDataType, AUTOMATIC, TCPIP_APPL_CONST) CertificateDataPtr
)
{
    Std_ReturnType std_Return = E_NOT_OK;
    std_Return = KeyM_GetCertificate(CertId, CertificateDataPtr);
    return std_Return;
}

FUNC(void, TCPIP_CODE)
TcpIp_TlsReadVersion(
    uint16* major,
    uint16* minor,
    const uint8 ver[2]
)
{
    *major = ver[0];
    *minor = ver[1];
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteCertificate(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_ERR_FEATURE_UNAVAILABLE;
    uint32 msgLen;
    KeyM_CertDataType certificateData;
    Std_ReturnType Std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;

    if (TLS_CLIENT == tlsConnCfgPtr->TlsConnectionInitStateType)
    {
        if (FALSE == tlsConnCfgPtr->TlsUseClientAuthenticationRequest)
        {
            ssl->state++;
            ret = TLS_RETURN_OK;
        }
    }

    if (TLS_SERVER == tlsConnCfgPtr->TlsConnectionInitStateType)
    {
        if (FALSE == tlsConnCfgPtr->TlsUseClientAuthenticationRequest)
        {
            ssl->state++;
            ret = TLS_RETURN_OK;
        }
        else
        {
            Std_Return = TcpIp_GetCertificate(
                             tlsConnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMLocalCertificateId,
                             &certificateData);

            if (E_NOT_OK == Std_Return)
            {
                ret = TLS_ERR_CERTIFICATE_REQUIRED;
            }
        }
    }

    if (ret == TLS_RETURN_OK)
    {
        /* 0->0: handshake type; 1->3: handshake length; 4->6: length of all certs; 7->9: length of cert1
         * 10->msgLen-1: peer certificate; msgLen->msgLen+2: length of cert. 2; msgLen+3...: upper level cert, etc. */
        msgLen = TLS_WRITE_CERTIFICATE_HEADER_LENGTH;
        ssl->handshake->key_cert = (mbedtls_ssl_key_cert*)(certificateData.certData);

        if (certificateData.certDataLength > (TLS_OUT_CONTENT_LEN - 3u - TLS_WRITE_CERTIFICATE_HEADER_LENGTH))
        {
            ret = TLS_ERR_CERTIFICATE_TOO_LARGE;
        }
        else
        {
            ssl->out_msg[msgLen     ] = (uint8)(certificateData.certDataLength >> 16u);
            ssl->out_msg[msgLen + 1u] = (uint8)(certificateData.certDataLength >>  8u);
            ssl->out_msg[msgLen + 2u] = (uint8)(certificateData.certDataLength);
            msgLen += 3u;
            (void)memcpy(ssl->out_msg + msgLen, certificateData.certData, certificateData.certDataLength);
            msgLen += certificateData.certDataLength;
            ssl->out_msg[4]  = (uint8)((msgLen - TLS_WRITE_CERTIFICATE_HEADER_LENGTH) >> 16u);
            ssl->out_msg[5]  = (uint8)((msgLen - TLS_WRITE_CERTIFICATE_HEADER_LENGTH) >>  8u);
            ssl->out_msg[6]  = (uint8)((msgLen - TLS_WRITE_CERTIFICATE_HEADER_LENGTH));
            ssl->out_msglen  = msgLen;
            ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
            ssl->out_msg[0] = TLS_HS_TYPE_CERTIFICATE;
            ssl->state++;
            ret = TcpIp_TlsWriteHandshakeMessage(ssl);
        }
    }

    return ret;
}

/* Derive and store session secret to TlsCsmMasterSecretKeyRef */
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsKeyDerive(
    mbedtls_ssl_context* ssl
)
{
    Std_ReturnType Std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    uint32 keyId = tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsConnectionHandshakeRef->TlsCsmPrfMacKeyRef;
    uint32 masterKeyId =
        tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsConnectionHandshakeRef->TlsCsmMasterSecretKeyRef;
    uint32 jobId = tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsConnectionHandshakeRef->TlsCsmPrfMacJobRef;

    if (TLS_PRF_CSM_FULL_SUPPORT ==
        tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsConnectionHandshakeRef->TlsCsmPRFSupportType)
    {
        Std_Return = Csm_KeyElementSet(
                         keyId,
                         TLS_KEY_ELEMENT_RANDOM_NUMBER,
                         ssl->handshake->randbytes,
                         64u
                     );
        Std_Return = Csm_KeyElementSet(
                         keyId,
                         TLS_KEY_ELEMENT_PREMASTER_SECRET,
                         ssl->handshake->premaster,
                         48u
                     );

        if (E_OK == Std_Return)
        {
            /* derive master secret */
            Std_Return = Csm_JobKeyDerive(jobId, keyId, masterKeyId);
        }

        if (E_OK == Std_Return)
        {
#if 0
            /* seed keyGenerate and generate key material */
            Std_Return = Csm_RandomSeed();
            Std_Return = Csm_KeyGenerate(masterKeyId);
#endif
        }
    }

    return Std_Return;
}

FUNC(int, TCPIP_CODE)
TcpIp_TLsWriteChangeCipherSpec(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;
    ssl->out_msgtype = TLS_MSG_TYPE_CHANGE_CIPHER_SPEC;
    ssl->out_msglen  = 1;
    ssl->out_msg[0]  = 1;
    ssl->state++;
    ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    return ret;
}


FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteHandshakeFinished(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;
    uint32 hash_len;
    TcpIp_TlsUpdateOutputPointers(ssl, ssl->transform_negotiate);
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    /*
     * RFC 5246 7.4.9 (Page 63) says 12 is the default length and ciphersuites
     * may define some other value. Currently (early 2016), no defined
     * moved to TLS 1.3 now) so we can keep the hardcoded 12 here.
     */
    hash_len = 12u;
    ssl->out_msglen  = 4u + hash_len;
    ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
    ssl->out_msg[0]  = TLS_HS_TYPE_FINISHED;

    /* In case of session resuming, invert the client and server ChangeCipherSpec messages order. */
    if (ssl->handshake->resume != 0)
    {
        if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_CLIENT)
        {
            ssl->state = TLS_HANDSHAKE_WRAPUP;
        }

        if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_SERVER)
        {
            ssl->state = TLS_CLIENT_CHANGE_CIPHER_SPEC;
        }
    }
    else
    {
        ssl->state++;
    }

    /* Switch to our negotiated transform and session parameters for outbound data. */
    (void)memset(ssl->cur_out_ctr, 0, 8);
    ssl->transform_out = ssl->transform_negotiate;
    ssl->session_out = ssl->session_negotiate;
    ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsParseChangeCipherSpec(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;
    ret = TcpIp_TlsReadRecord(ssl, 1u);

    if (ret == TLS_RETURN_OK)
    {
        if (ssl->in_msgtype != TLS_MSG_TYPE_CHANGE_CIPHER_SPEC)
        {
            ret = TLS_ERR_UNEXPECTED_MESSAGE;
        }
        else
        {
            ssl->transform_in = ssl->transform_negotiate;
            ssl->session_in = ssl->session_negotiate;
            (void)memset(ssl->in_ctr, 0, 8);
            TcpIp_TlsUpdateInputPointers(ssl, ssl->transform_negotiate);
            ssl->state++;
        }
    }

    return ret;
}


FUNC(int, TCPIP_CODE)
TcpIp_TlsParseHandshakeFinished(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint32 hash_len;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    ret = TcpIp_TlsReadRecord(ssl, 1u);

    if (ret == TLS_RETURN_OK)
    {
        if (ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE)
        {
            ret = TLS_ERR_UNEXPECTED_MESSAGE;
        }
        else
        {
            /* There is currently no ciphersuite using another length with TLS 1.2 */
            hash_len = 12u;

            if ((ssl->in_msg[0] != TLS_HS_TYPE_FINISHED) ||
                (ssl->in_hslen  != (TLS_HANDSHAKE_HEADER_LENGTH + hash_len)))
            {
                ret = TLS_ERR_BAD_HS_FINISHED;
            }
            else
            {
                if (ssl->handshake->resume != 0)
                {
                    if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_CLIENT)
                    {
                        ssl->state = TLS_CLIENT_CHANGE_CIPHER_SPEC;
                    }

                    if (tlsConnCfgPtr->TlsConnectionInitStateType == TLS_SERVER)
                    {
                        ssl->state = TLS_HANDSHAKE_WRAPUP;
                    }
                }
                else
                {
                    ssl->state++;
                }
            }
        }
    }

    return ret;
}

FUNC(void, TCPIP_CODE)
TcpIp_TlsHandshakeWrapUp(
    mbedtls_ssl_context* ssl
)
{
    /* Free the previous session and switch in the current one */
    if (ssl->session != NULL_PTR)
    {
        (void)memset(ssl->session, 0, sizeof(mbedtls_ssl_session));
        (void)TcpIp_TlsMemFree(ssl->session);
    }

    ssl->session = ssl->session_negotiate;
    ssl->session_negotiate = NULL_PTR;
    (void)memset(ssl, 0, sizeof(mbedtls_ssl_context));
    ssl->state++;
}

FUNC(uint8, TCPIP_CODE)
TcpIp_TlsGetSignatureTypeByPkAlgorithm(
    mbedtls_pk_type_t type
)
{
    uint8 sigType;

    switch (type)
    {
        case TLS_PK_RSA:
            sigType = TLS_SIG_RSA;
            break;

        case TLS_PK_ECDSA:
        case TLS_PK_ECKEY:
            sigType = TLS_SIG_ECDSA;
            break;

        default:
            sigType = TLS_SIG_ANON;
            break;
    }

    return sigType;
}

FUNC(mbedtls_pk_type_t, TCPIP_CODE)
TcpIp_TlsGetPkAlgorithmBySignatureType(
    uint8 sig
)
{
    mbedtls_pk_type_t pkType;

    switch (sig)
    {
        case TLS_SIG_RSA:
            pkType = TLS_PK_RSA;
            break;

        case TLS_SIG_ECDSA:
            pkType = TLS_PK_ECDSA;
            break;

        default:
            pkType = TLS_PK_NONE;
            break;
    }

    return pkType;
}

FUNC(mbedtls_md_type_t, TCPIP_CODE)
TcpIp_TlsGetMdAlgorithmByHashType(
    uint8 hash
)
{
    mbedtls_md_type_t mdType;

    switch (hash)
    {
        case TLS_HASH_MD5:
            mdType = TLS_MD_MD5;
            break;

        case TLS_HASH_SHA1:
            mdType = TLS_MD_SHA1;
            break;

        case TLS_HASH_SHA224:
            mdType = TLS_MD_SHA224;
            break;

        case TLS_HASH_SHA256:
            mdType = TLS_MD_SHA256;
            break;

        case TLS_HASH_SHA384:
            mdType = TLS_MD_SHA384;
            break;

        case TLS_HASH_SHA512:
            mdType = TLS_MD_SHA512;
            break;

        default:
            mdType = TLS_MD_NONE;
            break;
    }

    return mdType;
}

/* Add a signature-hash-pair to a signature-hash set */
FUNC(void, TCPIP_CODE)
TcpIp_TlsAddSignatureHash(
    mbedtls_ssl_sig_hash_set_t* set,
    mbedtls_pk_type_t sig_alg,
    mbedtls_md_type_t md_alg
)
{
    switch (sig_alg)
    {
        case TLS_PK_RSA:
            if (set->rsa == TLS_MD_NONE)
            {
                set->rsa = md_alg;
            }

            break;

        case TLS_PK_ECDSA:
            if (set->ecdsa == TLS_MD_NONE)
            {
                set->ecdsa = md_alg;
            }

            break;

        default:
            break;
    }
}


/* Receive application data decrypted from the SSL layer */
FUNC(int, TCPIP_CODE)
TcpIp_TlsReadData(
    mbedtls_ssl_context* ssl,
    uint8* buf,
    uint32 len
)
{
    int ret;
    uint32 msgLen;
    ret = TcpIp_TlsReadDataCheck(ssl);

    if (ret == TLS_RETURN_OK)
    {
        ret = TcpIp_TlsReadDataLoop(ssl);
        msgLen = (len < ssl->in_msglen) ? len : ssl->in_msglen;
        (void)memcpy(buf, ssl->in_offt, msgLen);
        ssl->in_msglen -= msgLen;

        if (ssl->in_msglen == 0u)        /* all data consumed */
        {
            ssl->in_offt = NULL_PTR;
            ssl->keep_current_message = 0;
        }
        else
        {
            ssl->in_offt += msgLen;     /* more data */
        }

        ret = (int)msgLen;
    }

    return ret;
}

/* SSL get accessors */
FUNC(int, TCPIP_CODE)
TcpIp_TlsGetBytesAvail(
    const mbedtls_ssl_context* ssl
)
{
    return ((ssl->in_offt == NULL_PTR) ? 0 : (int)ssl->in_msglen);
}

/* Write application data (public-facing wrapper) */
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteData(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
)
{
    int ret = TLS_RETURN_OK;

    if ((ssl == NULL_PTR) || (ssl->conf == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }

    if (ret == TLS_RETURN_OK)
    {
        if (ssl->state != TLS_HANDSHAKE_OVER)
        {
            ret = TcpIp_TlsHandshakeStart(ssl);
        }

        if (ret == TLS_RETURN_OK)
        {
            ret = TcpIp_TlsWriteReal(ssl, buf, len);
        }
    }

    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsGetMaxOutRecordPayload(
    const mbedtls_ssl_context* ssl
)
{
    uint32 max_len = TLS_OUT_CONTENT_LEN;
#if defined(TLS_MAX_FRAGMENT_LENGTH_ENABLED)
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    const uint32 mfl = tlsConnCfgPtr->TlsMaxFragmentLength;

    if (max_len > mfl)
    {
        max_len = mfl;
    }

#endif
    return ((int) max_len);
}


FUNC(int, TCPIP_CODE)
TcpIp_TlsSslSetup(
    mbedtls_ssl_context* ssl,
    const mbedtls_ssl_config* conf
)
{
    int ret;
    ssl->conf = conf;
    /* Set to NULL_PTR in case of an error condition */
    ssl->out_buf = NULL_PTR;
    ssl->in_buf = TcpIp_TlsMemCalloc(1u, TLS_IN_BUFFER_LEN);

    if (ssl->in_buf == NULL_PTR)
    {
        ret = TLS_ERR_ALLOC_FAILED;
    }
    else
    {
        ssl->out_buf = TcpIp_TlsMemCalloc(1u, TLS_OUT_BUFFER_LEN);

        if (ssl->out_buf == NULL_PTR)
        {
            ret = TLS_ERR_ALLOC_FAILED;
        }
        else
        {
            TcpIp_TlsResetInOutPointers(ssl);
            ret = TcpIp_TlsHandshakeInit(ssl);

            if (ret != TLS_RETURN_OK)
            {
                ret = TLS_ERR_HS_INIT;
            }
        }
    }

    if (ret != TLS_RETURN_OK)
    {
        (void)TcpIp_TlsMemFree(ssl->in_buf);
        (void)TcpIp_TlsMemFree(ssl->out_buf);
        ssl->conf = NULL_PTR;
        ssl->in_buf = NULL_PTR;
        ssl->out_buf = NULL_PTR;
        ssl->in_hdr = NULL_PTR;
        ssl->in_ctr = NULL_PTR;
        ssl->in_len = NULL_PTR;
        ssl->in_iv = NULL_PTR;
        ssl->in_msg = NULL_PTR;
        ssl->out_hdr = NULL_PTR;
        ssl->out_ctr = NULL_PTR;
        ssl->out_len = NULL_PTR;
        ssl->out_iv = NULL_PTR;
        ssl->out_msg = NULL_PTR;
    }

    return ret;
}

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#endif/* (TCPIP_TCP_TLS_ENABLED == STD_ON) */
