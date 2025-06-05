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
**  FILENAME    : TcpIp_TlsClient.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : pengfei.zhu                                                 **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Implementation for TLS client                               **
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
#include "TcpIp_Lcfg.h"
#include "TcpIp_Internal.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"

#if (TCPIP_TCP_TLS_ENABLED == STD_ON)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckServerHelloMessageHeader(
    mbedtls_ssl_context* ssl, uint8* bufPtr
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseCertificateRequest(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHelloDone(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteEncryptedPremasterSecret(
    mbedtls_ssl_context* ssl,
    uint16 offset,
    uint16* olen,
    uint16 pms_offset
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteCertificateVerify(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerMaxFragmentLengthExtension(
    mbedtls_ssl_context* ssl,
    const uint8* bufPtr,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    const uint8* bufPtr,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteClientKeyExchange(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHello(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHelloExtension(
    mbedtls_ssl_context* ssl,
    uint8* ext,
    uint32 extLen
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHelloPayload(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32 extLen,
    uint32 sesIdLen
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerKeyExchange(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteClientHello(
    mbedtls_ssl_context* ssl
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteMaxFragmentLengthExtension(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteSignatureAlgorithmsExtension(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
);
static FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsBuildClientHelloMessage(
    mbedtls_ssl_context* ssl
);
static FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsWriteServerHostName(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
);

/*******************************************************************************
**                      Private Functions                                     **
*******************************************************************************/

static FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
)
{
    uint8* msgPtr = bufPtr;
    const uint8* end = ssl->out_msg + TLS_OUT_CONTENT_LEN;
    *olen = 0u;

    if ((ssl->conf->encrypt_then_mac == TLS_ETM_DISABLED) ||
        (ssl->conf->max_minor_ver == TLS_MINOR_VERSION_0))
    {
        /* do nothing */
    }
    else
    {
        if ((end < msgPtr) || ((uint32)(end - msgPtr) < 4u))
        {
            /* do nothing */
        }
        else
        {
            *msgPtr++ = (uint8)((TLS_EXT_ENCRYPT_THEN_MAC >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)((TLS_EXT_ENCRYPT_THEN_MAC) & 0xFFu);
            *msgPtr++ = 0x00u;
            *msgPtr++ = 0x00u;
            *olen = 4u;
        }
    }
}

#if (STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)
static FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteMaxFragmentLengthExtension(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
)
{
    uint8* msgPtr = bufPtr;

    if (ssl->conf->mfl_code == TLS_MAX_FRAG_LEN_INVALID)
    {
        *olen = 0u;
    }
    else
    {
        *msgPtr++ = (uint8)((TLS_EXT_MAX_FRAGMENT_LENGTH >> 8) & 0xFFu);
        *msgPtr++ = (uint8)((TLS_EXT_MAX_FRAGMENT_LENGTH) & 0xFFu);
        *msgPtr++ = 0x00u;
        *msgPtr++ = 1u;
        *msgPtr++ = ssl->conf->mfl_code;
        *olen = 5u;
    }
}
#endif  /*TLS_MAX_FRAGMENT_LENGTH_ENABLED*/

static FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteSignatureAlgorithmsExtension(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
)
{
    uint8* msgPtr = bufPtr;
    const uint8* end = ssl->out_msg + TLS_OUT_CONTENT_LEN;
    uint32 sig_alg_len = 0u;
    uint8* sig_alg_list = bufPtr + 6u;
    *olen = 0u;

    if (ssl->conf->max_minor_ver != TLS_MINOR_VERSION_3)
    {
        /* do nothing */
    }
    else
    {
        sig_alg_len += 2u;

        if ((end < msgPtr) || ((uint32)(end - msgPtr) < (sig_alg_len + 6u)))
        {
            /* do nothing */
        }
        else
        {
            /* Prepare signature_algorithms extension (TLS 1.2) */
            sig_alg_len = 0u;
            sig_alg_list[sig_alg_len++] = TLS_HASH_SHA256;
            sig_alg_list[sig_alg_len++] = TLS_SIG_RSA;
            *msgPtr++ = (uint8)((TLS_EXT_SIG_ALG >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)((TLS_EXT_SIG_ALG) & 0xFFu);
            *msgPtr++ = (uint8)(((sig_alg_len + 2u) >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)(((sig_alg_len + 2u)) & 0xFFu);
            *msgPtr++ = (uint8)((sig_alg_len >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)((sig_alg_len) & 0xFFu);
            *olen = 6u + sig_alg_len;
        }
    }
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    const uint8* bufPtr,
    uint32 len
)
{
    int ret;

    if ((ssl->conf->encrypt_then_mac == TLS_ETM_DISABLED) ||
        (ssl->minor_ver == TLS_MINOR_VERSION_0) ||
        (len != 0u))
    {
        ret = TLS_ERR_BAD_HS_SERVER_HELLO;
    }
    else
    {
        ssl->session_negotiate->encrypt_then_mac = TLS_ETM_ENABLED;
    }

    ((void)bufPtr);
    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerMaxFragmentLengthExtension(
    mbedtls_ssl_context* ssl,
    const uint8* bufPtr,
    uint32 len
)
{
    int ret;
#if(STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)

    if ((ssl->conf->mfl_code == TLS_MAX_FRAG_LEN_INVALID) || (len != 1u) ||
        (bufPtr[0] != ssl->conf->mfl_code))
    {
        ret = TLS_ERR_BAD_HS_SERVER_HELLO;
    }
    else
#endif
    {
        ret = TLS_RETURN_OK;
    }

    return ret ;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteCertificateVerify(
    mbedtls_ssl_context* ssl
)
{
    Std_ReturnType Std_Return = E_NOT_OK;
    int ret = TLS_ERR_FEATURE_UNAVAILABLE;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info = ssl->transform_negotiate->ciphersuite_info;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    KeyM_CertDataType localCert;
    TcpIp_KeyMCertificateIdType localCertId = tlsConnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMLocalCertificateId;
    TcpIp_CsmJobsIdType jobId = tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
                                TlsConnectionHandshakeRef->TlsCsmKeyExchangeSignatureGenerateJobRef;
    uint32 msgLen = 0u, offset = 0u;
    ret = (int)TcpIp_TlsKeyDerive(ssl);

    if (ret == TLS_RETURN_OK)
    {
        if ((ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_PSK) ||
            (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA_PSK) ||
            (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECDHE_PSK) ||
            (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_DHE_PSK) ||
            (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECJPAKE))
        {
            ssl->state++;
            ret = TLS_RETURN_OK;
        }
        else
        {
            Std_Return = TcpIp_GetCertificate(localCertId, &localCert);

            if ((tlsConnCfgPtr->TlsUseClientAuthenticationRequest == FALSE) || (&localCert == NULL_PTR))
            {
                ssl->state++;
                ret = TLS_RETURN_OK;
            }
            else
            {
                if (TLS_VERSION_V12 ==
                    tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsCiphersuiteDefinitionRef->TlsVersion)
                {
                    /* 2 bytes of signature algorithm type */
                    ssl->out_msg[4] = TLS_HASH_SHA256;
                    ssl->out_msg[5] = TLS_SIG_RSA;
                    offset = 2u;
                }
                else
                {
                    ret = TLS_ERR_INTERNAL_ERROR;
                }

                if (ret == TLS_RETURN_OK)
                {
                    /* digitally-signed */
                    Std_Return = Csm_SignatureGenerate(jobId,
                                                       CRYPTO_OPERATIONMODE_START,
                                                       localCert.certData,
                                                       localCert.certDataLength,
                                                       ssl->out_msg + 6u + offset,
                                                       &msgLen
                                                      );

                    if (E_OK == Std_Return)
                    {
                        ssl->out_msg[4u + offset] = (uint8)(msgLen >> 8u);
                        ssl->out_msg[5u + offset] = (uint8)(msgLen);
                        ssl->out_msglen  = 6u + msgLen + offset;
                        ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
                        ssl->out_msg[0]  = TLS_HS_TYPE_CERTIFICATE_VERIFY;
                        ssl->state++;
                        ret = TcpIp_TlsWriteHandshakeMessage(ssl);
                    }
                    else
                    {
                        ret = TLS_ERR_INTERNAL_ERROR;
                    }
                }
            }
        }
    }

    return ret;
}

/* Generate a pre-master secret and encrypt it with the server's RSA key */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteEncryptedPremasterSecret(
    mbedtls_ssl_context* ssl,
    uint16 offset,
    uint16* olen,
    uint16 pms_offset
)
{
    int ret;
    uint8* result = NULL_PTR;
    uint32 resultLength = 48u;
    uint32 randomLength = 46u;
    uint32 len_bytes = 2u;
    Std_ReturnType Std_Return = E_NOT_OK;
    uint8* pmsPtr = ssl->handshake->premaster + pms_offset;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    TcpIp_CsmJobsIdType  keyExchangeJobId =
        tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsConnectionHandshakeRef->TlsCsmKeyExchangeEncryptJobRef;

    if ((offset + len_bytes) > TLS_OUT_CONTENT_LEN)
    {
        ret = TLS_ERR_BUFFER_TOO_SMALL;
    }
    else
    {
        /*  Generate (part of) the pre-master as
         *  struct {
         *      ProtocolVersion client_version;
         *      opaque random[46];
         *  } PreMasterSecret;
         */
        TcpIp_TlsWriteVersion(ssl, pmsPtr);
        pmsPtr += 2u;   /* add 2 bytes version */
        Std_Return = TcpIp_TlsCsmRandomGenerate(pmsPtr, &randomLength);
        ssl->handshake->pmslen = 48u;

        if (ssl->session_negotiate->peer_cert == NULL_PTR)
        {
            ret = TLS_ERR_UNEXPECTED_MESSAGE;
        }
        else
        {
            /* Private key stored to TlsCsmKeyExchangeEncryptKeyRef after certificate parsed.
             * Use job referenced private key encrypt the pre-master secret */
            Std_Return = Csm_Encrypt(keyExchangeJobId,
                                     CRYPTO_OPERATIONMODE_START,
                                     pmsPtr,
                                     ssl->handshake->pmslen,
                                     result,
                                     &resultLength
                                    );

            if (E_OK == Std_Return)
            {
                pmsPtr = result;
                ssl->handshake->pmslen = resultLength;
                ssl->out_msg[offset + 0u] = (uint8)(*olen >> 8u);
                ssl->out_msg[offset + 1u] = (uint8)(*olen);
                *olen += 2u;
                ret = TLS_RETURN_OK;
            }
            else
            {
                ret = TLS_ERR_INTERNAL_ERROR;
            }
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteClientKeyExchange(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint16 msgLen = 0u;
    uint16 extLen = 0u;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info =
        ssl->transform_negotiate->ciphersuite_info;

    if (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA)
    {
        msgLen = 4u;
        ret = TcpIp_TlsWriteEncryptedPremasterSecret(ssl, msgLen, &extLen, 0u);
    }
    else
    {
        ret = TLS_ERR_INTERNAL_ERROR;
        /* todo : pengfei.zhu */
    }

    if (ret == TLS_RETURN_OK)
    {
        ssl->out_msglen  = msgLen + extLen;
        ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
        ssl->out_msg[0]  = TLS_HS_TYPE_CLIENT_KEY_EXCHANGE;
        ssl->state++;
        ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    }

    return ret ;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseCertificateRequest(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint8* bufPtr;
    uint32 n = 0u;
    uint32 cert_type_len = 0u, dn_len = 0u;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;

    if (FALSE == tlsConnCfgPtr->TlsUseClientAuthenticationRequest)
    {
        ssl->state++;
        ret = TLS_RETURN_OK;
    }
    else
    {
        ret = TcpIp_TlsReadRecord(ssl, 1u);

        if (ret != TLS_RETURN_OK)
        {
            ret = TLS_ERR_READ_ERR;
        }
        else
        {
            if (ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE)
            {
                ret = TLS_ERR_UNEXPECTED_MESSAGE;
            }
            else
            {
                ssl->state++;
                ssl->client_auth = (ssl->in_msg[0] == TLS_HS_TYPE_CERTIFICATE_REQUEST);

                if (ssl->client_auth == 0u)
                {
                    /* Current message is probably the ServerHelloDone */
                    ssl->keep_current_message = 1;
                }
                else
                {
                    bufPtr = ssl->in_msg;

                    /* certificate_types */
                    if (ssl->in_hslen <= TLS_HANDSHAKE_HEADER_LENGTH)
                    {
                        ret = TLS_ERR_BAD_HS_CERTIFICATE_REQUEST;
                    }

                    if (ret == TLS_RETURN_OK)
                    {
                        cert_type_len = bufPtr[TLS_HANDSHAKE_HEADER_LENGTH];
                        n = cert_type_len;

                        if (ssl->in_hslen <= (TLS_HANDSHAKE_HEADER_LENGTH + 2u + n))
                        {
                            ret = TLS_ERR_BAD_HS_CERTIFICATE_REQUEST;
                        }

                        if (ret == TLS_RETURN_OK)
                        {
                            /* supported_signature_algorithms */
                            if (ssl->minor_ver == TLS_MINOR_VERSION_3)
                            {
                                uint32 sig_alg_len = ((bufPtr[TLS_HANDSHAKE_HEADER_LENGTH + 1u + n] <<  8u)
                                                      | (bufPtr[TLS_HANDSHAKE_HEADER_LENGTH + 2u + n]));

                                if (ssl->in_hslen <= (TLS_HANDSHAKE_HEADER_LENGTH + 3u + n + sig_alg_len))
                                {
                                    ret = TLS_ERR_BAD_HS_CERTIFICATE_REQUEST;
                                }

                                n += 2u + sig_alg_len;
                            }

                            if (ret == TLS_RETURN_OK)
                            {
                                /* certificate_authorities */
                                dn_len = ((bufPtr[TLS_HANDSHAKE_HEADER_LENGTH + 1u + n] << 8u)
                                          | (bufPtr[TLS_HANDSHAKE_HEADER_LENGTH + 2u + n]));
                                n += dn_len;

                                if (ssl->in_hslen != (TLS_HANDSHAKE_HEADER_LENGTH + 3u + n))
                                {
                                    ret = TLS_ERR_BAD_HS_CERTIFICATE_REQUEST;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHelloDone(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    ret = TcpIp_TlsReadRecord(ssl, 1u);

    if (ret != TLS_RETURN_OK)
    {
        ret = TLS_ERR_READ_ERR;
    }

    if (ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE)
    {
        ret = TLS_ERR_BAD_MESSAGE_ERR;
    }
    else
    {
        if ((ssl->in_hslen  != TLS_HANDSHAKE_HEADER_LENGTH) ||
            (ssl->in_msg[0] != TLS_HS_TYPE_SERVER_HELLO_DONE))
        {
            ret = TLS_ERR_BAD_HS_SERVER_HELLO_DONE;
        }
    }

    ssl->state++;
    return ret;
}

#if(STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)
static FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsWriteServerHostName(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32* olen
)
{
    uint8* msgPtr = bufPtr;
    const uint8* end = ssl->out_msg + TLS_OUT_CONTENT_LEN;
    uint32 hostname_len;
    ssl->hostname = ssl->conf->hostname;
    Std_ReturnType Std_Return = E_NOT_OK;
    *olen = 0;

    if (ssl->hostname == NULL_PTR)
    {
        Std_Return = E_NOT_OK;
    }
    else
    {
        Std_Return = E_OK;
        hostname_len = strlen(ssl->hostname);

        if ((end < msgPtr) || ((end - msgPtr) < (hostname_len + 9u)))
        {
            /* buff overflow */
            Std_Return = E_NOT_OK;
        }
        else
        {
            /* Sect. 3, RFC 6066 (TLS Extensions Definitions)*/
            *msgPtr++ = (uint8)((TLS_EXT_SERVERNAME >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)((TLS_EXT_SERVERNAME) & 0xFFu);
            *msgPtr++ = (uint8)(((hostname_len + 5u) >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)(((hostname_len + 5u)) & 0xFFu);
            *msgPtr++ = (uint8)(((hostname_len + 3u) >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)(((hostname_len + 3u)) & 0xFFu);
            *msgPtr++ = (uint8)((TLS_EXT_SERVERNAME_HOSTNAME) & 0xFFu);
            *msgPtr++ = (uint8)((hostname_len >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)((hostname_len) & 0xFFu);
            (void)memcpy(msgPtr, ssl->hostname, hostname_len);
            *olen = hostname_len + 9u;
        }
    }

    return Std_Return;
}
#endif /* TCPIP_TLS_SERVER_NAME_INDICATION */

static FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsBuildClientHelloMessage(
    mbedtls_ssl_context* ssl
)
{
    uint8* bufPtr, *msgPtr, *csLenPtr;
    uint32 iLoop, csLen, olen, extLen = 0u;
    uint8 hsLenSize = 4u;
    uint8 hsVerSize = 2u;
    uint8 hsRandNumSize = 32u;
    uint8 csLenSize = 2u;
    Std_ReturnType std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    /* Write standard client hello message part:*/
    bufPtr = ssl->out_msg;
    ssl->out_msg[0]  = TLS_HS_TYPE_CLIENT_HELLO;                        /* Byte 0       handshake type */
    msgPtr = bufPtr + hsLenSize;                                        /* Byte 1->3    handshake length */
    TcpIp_TlsWriteVersion(ssl, msgPtr);                                 /* Byte 4->5    version supported */
    msgPtr += hsVerSize;
    std_Return = TcpIp_TlsGenerateRandomNumber(ssl);                   /* Byte 6->9    current GMT Unix time;
                                                                           Byte 10->37  random bytes */

    if (E_OK == std_Return)
    {
        (void)memcpy(msgPtr, ssl->handshake->randbytes, hsRandNumSize);
        msgPtr += hsRandNumSize;
        /* Write extended client hello part:*/
        csLen = ssl->session_negotiate->id_len;                         /* Byte 38         session id length */

        if ((csLen < 16u) || (csLen > 32u) ||
            (ssl->handshake->resume == 0))
        {
            csLen = 0u;
        }

        *msgPtr++ = (uint8) csLen;

        for (iLoop = 0u; iLoop < csLen; iLoop++)
        {
            *msgPtr++ = ssl->session_negotiate->id[iLoop];              /* Byte 39 + csLen     session id length */
        }

        /* CipherSuite list */
        /* todo  */
        //        ciphersuites = ssl->conf->ciphersuite_list[ssl->minor_ver];
        //
        //        /* Skip writing ciphersuite length for now */
        //        n = 0;
        //        q = msgPtr;
        //        msgPtrp += 2;
        csLenPtr = msgPtr;
        msgPtr += csLenSize;                                            /* cipher suite list length (2 bytes) */
        csLen++;
        *msgPtr++ = (uint8)(*(ssl->conf->ciphersuite_list[ssl->minor_ver]) >> 8u);
        *msgPtr++ = (uint8)(*(ssl->conf->ciphersuite_list[ssl->minor_ver]));             /* cipher suite list (2 bytes) */
        /* write cipher suite length */
        *csLenPtr++ = (uint8)(csLen >> 7u);
        *csLenPtr++ = (uint8)(csLen << 1u);
        *msgPtr++ = 1u;
        *msgPtr++ = TLS_COMPRESS_NULL;

        /*First write extensions, then the total length*/
        if (NULL_PTR != tlsConnCfgPtr->TlsCertificateIdentityRef->TlsServerNameIdentification)
        {
            std_Return = TcpIp_TlsWriteServerHostName(ssl, msgPtr + 2u + extLen, &olen);
            extLen += olen;
        }

        if (TLS_VERSION_V12 ==
            tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsCiphersuiteDefinitionRef->TlsVersion)
        {
            TcpIp_TlsWriteSignatureAlgorithmsExtension(ssl, msgPtr + 2u + extLen, &olen);
            extLen += olen;
        }

#if(STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)

        if ((0u < tlsConnCfgPtr->TlsMaxFragmentLength) && (0x4000u >= tlsConnCfgPtr->TlsMaxFragmentLength))
        {
            TcpIp_TlsWriteMaxFragmentLengthExtension(ssl, msgPtr + 2u + extLen, &olen);
            extLen += olen;
        }

#endif
#if(STD_ON == TLS_ENCRYPT_THEN_MAC)

        if (TRUE == tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
            TlsCiphersuiteDefinitionRef->TlsUseSecurityExtensionForceEncryptThenMac)
        {
            TcpIp_TlsWriteEncryptThenMacExtension(ssl, msgPtr + 2u + extLen, &olen);
            extLen += olen;
        }

#endif

        if (extLen > 0u)
        {
            *msgPtr++ = (uint8)((extLen >> 8u) & 0xFFu);
            *msgPtr++ = (uint8)((extLen) & 0xFFu);
            msgPtr += extLen;
        }

        ssl->out_msglen  = (uint32)(msgPtr - bufPtr);
        ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
        ssl->state++;
    }

    return std_Return;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteClientHello(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_ERR_INTERNAL_ERROR;
    Std_ReturnType std_Return = E_NOT_OK;
    std_Return = TcpIp_TlsBuildClientHelloMessage(ssl);

    if (E_OK == std_Return)
    {
        ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckServerHelloMessageHeader(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr
)
{
    int ret = TLS_RETURN_OK;

    if (ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE)
    {
        ret = TLS_ERR_UNEXPECTED_MESSAGE;
    }
    else
    {
        if ((ssl->in_hslen < (38u + TLS_HANDSHAKE_HEADER_LENGTH)) || (bufPtr[0] != TLS_HS_TYPE_SERVER_HELLO))
        {
            ret = TLS_ERR_BAD_HS_SERVER_HELLO;
        }
        else
        {
            bufPtr += TLS_HANDSHAKE_HEADER_LENGTH;
            TcpIp_TlsReadVersion(&ssl->major_ver, &ssl->minor_ver, bufPtr);

            if ((ssl->major_ver < ssl->conf->min_major_ver) || (ssl->minor_ver < ssl->conf->min_minor_ver) ||
                (ssl->major_ver > ssl->conf->max_major_ver) || (ssl->minor_ver > ssl->conf->max_minor_ver))
            {
                ret = TLS_ERR_BAD_HS_PROTOCOL_VERSION;
            }
        }
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHelloExtension(
    mbedtls_ssl_context* ssl,
    uint8* ext,
    uint32 extLen
)
{
    int ret;

    while (extLen)
    {
        uint32 ext_id   = ((ext[0] << 8u) | ext[1]);
        uint32 ext_size = ((ext[2] << 8u) | ext[3]);

        if ((ext_size + 4u) > extLen)
        {
            ret = TLS_ERR_BAD_HS_SERVER_HELLO;
        }

        if (ret == TLS_RETURN_OK)
        {
            /* parse extend message */
            switch (ext_id)
            {
                case TLS_EXT_MAX_FRAGMENT_LENGTH:
                    ret = TcpIp_TlsParseServerMaxFragmentLengthExtension(ssl, ext + 4u, ext_size);

                    if (ret != TLS_RETURN_OK)
                    {
                        ret = TLS_ERR_FRAGMENT_ERR;
                    }

                    break;

                case TLS_EXT_ENCRYPT_THEN_MAC:
                    ret = TcpIp_TlsParseServerEncryptThenMacExtension(ssl, ext + 4u, ext_size);

                    if (ret != TLS_RETURN_OK)
                    {
                        ret = TLS_ERR_ENCRYPT_THEN_MAC;
                    }

                    break;

                default:
                    break;
            }

            if (ret == TLS_RETURN_OK)
            {
                extLen -= 4u + ext_size;
                ext += 4u + ext_size;

                if ((extLen > 0u) && (extLen < 4u))
                {
                    ret = TLS_ERR_BAD_HS_SERVER_HELLO;
                }
            }
        }
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHelloPayload(
    mbedtls_ssl_context* ssl,
    uint8* bufPtr,
    uint32 extLen,
    uint32 sesIdLen
)
{
    int ret, i, cmpVal;
    uint32 comp;
    uint8* ext;
    const mbedtls_ssl_ciphersuite_t* suite_info;
    /* ciphersuite (used later) */
    i = (bufPtr[35u + sesIdLen] << 8u) | bufPtr[36u + sesIdLen];
    /* Read and check compression( AutoSAR dont support compression ) */
    comp = bufPtr[37u + sesIdLen];

    if (comp == TLS_COMPRESS_NULL)
    {
        /* Initialize update checksum functions */
        ssl->transform_negotiate->ciphersuite_info = TcpIp_TlsGetCiphersuiteFromId(i);

        if (ssl->transform_negotiate->ciphersuite_info != NULL_PTR)
        {
            cmpVal = memcmp(ssl->session_negotiate->id, bufPtr + 35u, sesIdLen);

            /* Check if the session can be resumed */
            if ((ssl->handshake->resume == 0) || (sesIdLen == 0u) ||
                (ssl->session_negotiate->ciphersuite != i) ||
                (ssl->session_negotiate->compression != comp) ||
                (ssl->session_negotiate->id_len != sesIdLen) ||
                (cmpVal != 0))
            {
                ssl->state++;
                ssl->handshake->resume = 0;
                ssl->session_negotiate->ciphersuite = i;
                ssl->session_negotiate->compression = comp;
                ssl->session_negotiate->id_len = sesIdLen;
                (void)memcpy(ssl->session_negotiate->id, bufPtr + 35u, sesIdLen);
            }
            else
            {
                ssl->state = TLS_SERVER_CHANGE_CIPHER_SPEC;
                ret = (int)TcpIp_TlsKeyDerive(ssl);     /* derive key */

                if (ret != TLS_RETURN_OK)
                {
                    ret = TLS_ERR_DERIVE_KEY;
                }
            }

            if (ret == TLS_RETURN_OK)
            {
                /* Perform cipher suite validation in same way as in TcpIp_TlsWriteClientHello. */
                i = 0;

                while (1u)
                {
                    if (ssl->conf->ciphersuite_list[ssl->minor_ver][i] == 0)
                    {
                        ret = TLS_ERR_BAD_HS_SERVER_HELLO;
                    }

                    if ((ret == TLS_ERR_BAD_HS_SERVER_HELLO) ||
                        (ssl->conf->ciphersuite_list[ssl->minor_ver][i++] == ssl->session_negotiate->ciphersuite))
                    {
                        break;
                    }
                }

                if (ret == TLS_RETURN_OK)
                {
                    suite_info = TcpIp_TlsGetCiphersuiteFromId(ssl->session_negotiate->ciphersuite);

                    // ret = ssl_validate_ciphersuite( suite_info, ssl, ssl->minor_ver, ssl->minor_ver );

                    if (ret != TLS_RETURN_OK)
                    {
                        ret = TLS_ERR_BAD_HS_SERVER_HELLO;
                    }

                    if (ret == TLS_RETURN_OK)
                    {
                        ssl->session_negotiate->compression = comp;
                        ext = bufPtr + 40u + sesIdLen;
                        ret = TcpIp_TlsParseServerHelloExtension(ssl, ext, extLen);
                    }
                }
            }
        }
        else
        {
            /* cipher suite info not found */
            ret = TLS_ERR_BAD_INPUT_DATA;
        }
    }
    else
    {
        ret = TLS_ERR_FEATURE_UNAVAILABLE;
    }

    return ret;
}


/* parse server hello message :
 * 0->1:server_version;
 * 2->33:random (maybe including 4 bytes of Unix time);
 * 34->34:session_id length = n;
 * 35->34+n  session_id;
 * 35+n->36+n:cipher_suite;
 * 37+n->37+n:compression_method;
 * 38+n->39+n:extensions length (optional);
 * 40+n->.. : other extensions */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerHello(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint32 sesIdLen;    /* session id length */
    uint32 extLen;      /* extended message length */
    uint8* bufPtr;
    bufPtr = ssl->in_msg;
    ret = TcpIp_TlsReadRecord(ssl, 1u);

    if (ret == TLS_RETURN_OK)
    {
        ret = TcpIp_TlsCheckServerHelloMessageHeader(ssl, bufPtr);

        if (ret == TLS_RETURN_OK)
        {
            (void)memcpy(ssl->handshake->randbytes + 32u, bufPtr + 2u, 32u);
            sesIdLen = bufPtr[34u];

            if (sesIdLen <= 32u)
            {
                if (ssl->in_hslen > (TLS_HANDSHAKE_HEADER_LENGTH + 39u + sesIdLen))
                {
                    extLen = ((bufPtr[38u + sesIdLen] << 8u) | (bufPtr[39u + sesIdLen]));

                    if (((extLen > 0u) && (extLen < 4u)) || ((ssl->in_hslen) != (4u + 40u + sesIdLen + extLen)))
                    {
                        ret = TLS_ERR_BAD_HS_SERVER_HELLO;
                    }
                }
                else if (ssl->in_hslen == (TLS_HANDSHAKE_HEADER_LENGTH + 38u + sesIdLen))
                {
                    extLen = 0u;
                }
                else
                {
                    ret = TLS_ERR_BAD_HS_SERVER_HELLO;
                }

                if (ret == TLS_RETURN_OK)
                {
                    ret = TcpIp_TlsParseServerHelloPayload(ssl, bufPtr, extLen, sesIdLen);
                }
            }
            else
            {
                ret = TLS_ERR_BAD_HS_SERVER_HELLO;
            }
        }
    }

    return ret;
}

/* The ServerKeyExchange message is intended to carry additional data for key exchange.
 * If RSA/DH/ECDH, then do nothing.If DHE/ECDHE ,need read ECC parameters for process  */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerKeyExchange(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info =
        ssl->transform_negotiate->ciphersuite_info;

    if (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA)
    {
        ssl->state++;
        ret = TLS_RETURN_OK;
    }

    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsClientHandshakeStep(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;

    if ((ssl->state == TLS_HANDSHAKE_OVER) || (ssl->handshake == NULL_PTR))
    {
        ret = TLS_ERR_BAD_INPUT_DATA;
    }
    else
    {
        ret = (int)TcpIp_TlsFlushOutput(ssl);

        if (ret != TLS_RETURN_OK)
        {
            ret = TLS_ERR_BAD_INPUT_DATA;
        }
        else
        {
            switch (ssl->state)
            {
                case TLS_HELLO_REQUEST:
                    ssl->state = TLS_CLIENT_HELLO;
                    break;

                /*
                *  ==> ClientHello
                */
                case TLS_CLIENT_HELLO:
                    ret = TcpIp_TlsWriteClientHello(ssl);
                    break;

                /*
                *  <==   ServerHello
                *        Certificate
                *      ( ServerKeyExchange  )
                *      ( CertificateRequest )
                *        ServerHelloDone
                */
                case TLS_SERVER_HELLO:
                    ret = TcpIp_TlsParseServerHello(ssl);
                    break;

                case TLS_SERVER_CERTIFICATE:
                    ret = TcpIp_TlsParseCertificate(ssl);
                    break;

                case TLS_SERVER_KEY_EXCHANGE:  /* if RSA key exchange then do nothing */
                    ret = TcpIp_TlsParseServerKeyExchange(ssl);
                    break;

                case TLS_CERTIFICATE_REQUEST:  /* server request client certificate for bidirectional authentication */
                    ret = TcpIp_TlsParseCertificateRequest(ssl);
                    break;

                case TLS_SERVER_HELLO_DONE:
                    ret = TcpIp_TlsParseServerHelloDone(ssl);
                    break;

                /* ==> ( Certificate/Alert  )
                 *        ClientKeyExchange
                 *      ( CertificateVerify  )
                 *        ChangeCipherSpec
                 *        Finished           */
                case TLS_CLIENT_CERTIFICATE:
                    ret = TcpIp_TlsWriteCertificate(ssl);
                    break;

                case TLS_CLIENT_KEY_EXCHANGE:
                    ret = TcpIp_TlsWriteClientKeyExchange(ssl);
                    break;

                case TLS_CERTIFICATE_VERIFY:
                    ret = TcpIp_TlsWriteCertificateVerify(ssl);
                    break;

                case TLS_CLIENT_CHANGE_CIPHER_SPEC:
                    ret = TcpIp_TLsWriteChangeCipherSpec(ssl);
                    break;

                case TLS_CLIENT_FINISHED:
                    ret = TcpIp_TlsWriteHandshakeFinished(ssl);
                    break;

                /* <==   ( NewSessionTicket )
                 *        ChangeCipherSpec
                 *        Finished          */
                case TLS_SERVER_CHANGE_CIPHER_SPEC:
                    ret = TcpIp_TlsParseChangeCipherSpec(ssl);
                    break;

                case TLS_SERVER_FINISHED:
                    ret = TcpIp_TlsParseHandshakeFinished(ssl);
                    break;

                case TLS_FLUSH_BUFFERS:  /* handshake done */
                    ssl->state = TLS_HANDSHAKE_WRAPUP;
                    break;

                case TLS_HANDSHAKE_WRAPUP:
                    TcpIp_TlsHandshakeWrapUp(ssl);
                    break;

                default:
                    ret = TLS_ERR_BAD_INPUT_DATA;
                    break;
            }
        }
    }

    return ret;
}
#endif/* (TCPIP_TCP_TLS_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
