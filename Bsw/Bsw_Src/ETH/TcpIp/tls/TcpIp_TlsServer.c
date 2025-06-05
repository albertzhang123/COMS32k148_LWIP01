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
**  DESCRIPTION : Implementation for TLS server                               **
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

#if (TCPIP_TCP_TLS_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientHello(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsProcessSignatureVerify(
    mbedtls_ssl_context* ssl,
    uint32 msg_len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckHashAndSignatureType(
    mbedtls_ssl_context* ssl,
    uint32 msg_len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerNameExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckClientHello(
    mbedtls_ssl_context* ssl,
    uint32 msg_len,
    uint32 ciph_len,
    uint32 ext_len,
    uint32 ciph_offset,
    uint8* bufPtr,
    uint8* extPtr
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsMatchLocalCipherSuite(
    mbedtls_ssl_context* ssl,
    uint32 ciph_len,
    uint32 ciph_offset,
    uint8* msgPtr,
    uint8* bufPtr
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckClientHelloExtendOffset(
    mbedtls_ssl_context* ssl,
    uint32 msg_len, uint32
    ciph_len,
    uint32 ext_len,
    uint32 ciph_offset,
    uint8* bufPtr,
    uint8* extPtr
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckClientHelloMessageHeader(
    mbedtls_ssl_context* ssl,
    uint32 msg_len,
    uint8* bufPtr
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerHello(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerHelloDone(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerKeyExchange(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientKeyExchange(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerCertificateRequest(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientCertificateVerify(
    mbedtls_ssl_context* ssl
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientSignatureAlgorithmsExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientMaxFragmentLengthExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
);
static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientEncryptedPremasterSecret(
    mbedtls_ssl_context* ssl,
    const uint8* msgPtr,
    const uint8* end,
    uint32 pms_offset
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsServerWriteMaxFragmentLength(
    mbedtls_ssl_context* ssl,
    uint8* buf,
    uint32* olen
);
static FUNC(void, TCPIP_CODE)
TcpIp_TlsServerWriteEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    uint8* buf,
    uint32* olen
);

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

static FUNC(int, TCPIP_CODE)
TcpIp_TlsProcessSignatureVerify(
    mbedtls_ssl_context* ssl,
    uint32 msg_len
)
{
    int ret;
    uint16 sig_len;
    Crypto_VerifyResultType verify;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    TcpIp_CsmJobsIdType jobId = tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
                                TlsConnectionHandshakeRef->TlsCsmKeyExchangeSignatureVerifyJobRef;

    if ((msg_len + 2u) <= ssl->in_hslen)
    {
        sig_len = (ssl->in_msg[msg_len] << 8u) | ssl->in_msg[msg_len + 1u];
        msg_len += 2u;

        if ((msg_len + sig_len) == ssl->in_hslen)
        {
            Std_ReturnType Std_Return = Csm_SignatureVerify(jobId,
                                                            CRYPTO_OPERATIONMODE_START,
                                                            ssl->in_msg,
                                                            ssl->in_msglen,
                                                            ssl->in_msg + msg_len,
                                                            sig_len,
                                                            &verify);

            if (E_OK == Std_Return)
            {
                TcpIp_TlsUpdateHandshakeStatus(ssl);
            }
            else
            {
                ret = TLS_ERR_INTERNAL_ERROR;
            }
        }
        else
        {
            ret = TLS_ERR_BAD_HS_CERTIFICATE_VERIFY;
        }
    }
    else
    {
        ret = TLS_ERR_BAD_HS_CERTIFICATE_VERIFY;
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckHashAndSignatureType(
    mbedtls_ssl_context* ssl,
    uint32 msg_len
)
{
    int ret;
    mbedtls_pk_type_t pk_alg;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;

    if (TLS_VERSION_V12 == tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsCiphersuiteDefinitionRef->TlsVersion)
    {
        if ((msg_len + 2u) > ssl->in_hslen)
        {
            ret = TLS_ERR_BAD_HS_CERTIFICATE_VERIFY;
        }
        else
        {
            /* check hash type */
            if (TLS_HASH_SHA256 == ssl->in_msg[msg_len])
            {
                msg_len++;
                /* check if signature type and pk_alg match */
                pk_alg = TcpIp_TlsGetPkAlgorithmBySignatureType(ssl->in_msg[msg_len]);

                if (TLS_PK_NONE != pk_alg)
                {
                    msg_len++;
                    ret = TLS_RETURN_OK;
                }
                else
                {
                    ret = TLS_ERR_INTERNAL_ERROR;
                }
            }
            else
            {
                ret = TLS_ERR_INTERNAL_ERROR;
            }
        }
    }
    else
    {
        ret = TLS_ERR_INTERNAL_ERROR;
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
)
{
    int ret;

    if (len != 0u)
    {
        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
    }
    else
    {
        ((void) buf);

        if (ssl->conf->encrypt_then_mac == TLS_ETM_ENABLED)
        {
            ssl->session_negotiate->encrypt_then_mac = TLS_ETM_ENABLED;
            ret = TLS_RETURN_OK;
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientMaxFragmentLengthExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
)
{
    int ret;

    if ((len != 1u) || (buf[0] >= TLS_MAX_FRAG_LEN_INVALID))
    {
        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
    }
    else
    {
        ssl->session_negotiate->mfl_code = buf[0];
        ret = TLS_RETURN_OK;
    }

    return ret;
}

static FUNC(void, TCPIP_CODE)
TcpIp_TlsServerWriteMaxFragmentLength(
    mbedtls_ssl_context* ssl,
    uint8* buf,
    uint32* olen
)
{
    uint8* msgPtr = buf;
#if(STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)

    if (ssl->conf->mfl_code == TLS_MAX_FRAG_LEN_INVALID)
    {
        *olen = 0u;
    }
    else
#endif
    {
        *msgPtr++ = (uint8)((TLS_EXT_MAX_FRAGMENT_LENGTH >> 8u) & 0xFFu);
        *msgPtr++ = (uint8)((TLS_EXT_MAX_FRAGMENT_LENGTH) & 0xFFu);
        *msgPtr++ = 0x00u;
        *msgPtr++ = 1u;
        *msgPtr++ = ssl->conf->mfl_code;
        *olen = 5u;
    }
}

static FUNC(void, TCPIP_CODE)
TcpIp_TlsServerWriteEncryptThenMacExtension(
    mbedtls_ssl_context* ssl,
    uint8* buf,
    uint32* olen
)
{
    uint8* msgPtr = buf;
    const mbedtls_ssl_ciphersuite_t* suite = NULL_PTR;
    const mbedtls_cipher_info_t* cipher = NULL_PTR;

    if ((ssl->session_negotiate->encrypt_then_mac == TLS_ETM_DISABLED) ||
        (ssl->minor_ver == TLS_MINOR_VERSION_0))
    {
        *olen = 0;
    }
    else
    {
        suite = TcpIp_TlsGetCiphersuiteFromId(ssl->session_negotiate->ciphersuite);

        if ((suite == NULL_PTR) || (cipher == NULL_PTR) || (cipher->mode != TLS_MODE_CBC))
        {
            /* todo : how to get CBC mode from crypto driver */
            *olen = 0u;
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

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientSignatureAlgorithmsExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
)
{
    uint32 sig_alg_list_size;
    int ret;
    const uint8* msgPtr;
    mbedtls_md_type_t md_cur;
    mbedtls_pk_type_t sig_cur;

    if (len < 2u)
    {
        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
    }
    else
    {
        sig_alg_list_size = ((buf[0] << 8u) | (buf[1]));

        if (((sig_alg_list_size + 2u) != len) || ((sig_alg_list_size % 2u) != 0u))
        {
            ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
        }
        else
        {
            msgPtr = buf + 2u;
            sig_cur = TcpIp_TlsGetPkAlgorithmBySignatureType(msgPtr[1]);

            if (sig_cur == TLS_PK_RSA)
            {
                /* Check if we support the hash the user proposes */
                md_cur = TcpIp_TlsGetMdAlgorithmByHashType(msgPtr[0]);

                if ((md_cur == TLS_MD_SHA256) && (*(ssl->conf->sig_hashes) == md_cur))
                {
                    TcpIp_TlsAddSignatureHash(&ssl->handshake->hash_algs, sig_cur, md_cur);
                }
                else
                {
                    ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                }
            }
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientEncryptedPremasterSecret(
    mbedtls_ssl_context* ssl,
    const uint8* msgPtr,
    const uint8* end,
    uint32 pms_offset
)
{
    int ret;
    uint8* result = NULL_PTR;
    Std_ReturnType Std_Return = E_NOT_OK;
    uint32 resultLength = 48u;
    uint8* pms = ssl->handshake->premaster + pms_offset;
    uint8 ver[2];
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    TcpIp_CsmJobsIdType  keyExchangeJobId = tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
                                            TlsConnectionHandshakeRef->TlsCsmKeyExchangeEncryptJobRef;
    ssl->handshake->pmslen = 48u;
    Std_Return = Csm_Decrypt(keyExchangeJobId,
                             CRYPTO_OPERATIONMODE_START,
                             pms,
                             ssl->handshake->pmslen,
                             result,
                             &resultLength);

    if (E_OK == Std_Return)
    {
        ret = TLS_RETURN_OK;
        pms = result;
        TcpIp_TlsWriteVersion(ssl, ver);
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientCertificateVerify(
    mbedtls_ssl_context* ssl
)
{
    uint32 msg_len;
    int ret = TLS_ERR_FEATURE_UNAVAILABLE;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info = ssl->transform_negotiate->ciphersuite_info;

    if ((ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECDHE_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_DHE_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECJPAKE) ||
        (ssl->session_negotiate->peer_cert == NULL_PTR))
    {
        ssl->state++;
        ret = TLS_RETURN_OK;
    }
    else
    {
        /* Read the message without adding it to the checksum */
        ret = TcpIp_TlsReadRecord(ssl, 0u); /* no checksum update */

        if (ret == TLS_RETURN_OK)
        {
            ssl->state++;

            if ((ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE) || (ssl->in_msg[0] != TLS_HS_TYPE_CERTIFICATE_VERIFY))
            {
                ret = TLS_ERR_BAD_HS_CERTIFICATE_VERIFY;
            }

            if (ret == TLS_RETURN_OK)
            {
                msg_len = TLS_HANDSHAKE_HEADER_LENGTH;
                /* struct {
                 *          SignatureAndHashAlgorithm algorithm; -- TLS 1.2 only
                 *          opaque signature<0..2^16-1>;
                 *        } DigitallySigned; */
                ret = TcpIp_TlsCheckHashAndSignatureType(ssl, msg_len);

                if (ret == TLS_RETURN_OK)
                {
                    ret = TcpIp_TlsProcessSignatureVerify(ssl, msg_len);
                }
            }
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerHelloDone(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_RETURN_OK;
    ssl->out_msglen  = 4;
    ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
    ssl->out_msg[0]  = TLS_HS_TYPE_SERVER_HELLO_DONE;
    ssl->state++;
    ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientKeyExchange(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info;
    uint8* msgPtr, *end;
    ciphersuite_info = ssl->transform_negotiate->ciphersuite_info;
    ret = TcpIp_TlsReadRecord(ssl, 1u);

    if (ret == TLS_RETURN_OK)
    {
        msgPtr = ssl->in_msg + TLS_HANDSHAKE_HEADER_LENGTH;
        end = ssl->in_msg + ssl->in_hslen;

        if (ssl->in_msgtype != TLS_MSG_TYPE_HANDSHAKE)
        {
            ret = TLS_ERR_BAD_HS_CLIENT_KEY_EXCHANGE;
        }
        else
        {
            if (ssl->in_msg[0] != TLS_HS_TYPE_CLIENT_KEY_EXCHANGE)
            {
                ret = TLS_ERR_BAD_HS_CLIENT_KEY_EXCHANGE;
            }

            if (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA)
            {
                ret = TcpIp_TlsParseClientEncryptedPremasterSecret(ssl, msgPtr, end, 0u);
            }
            else
            {
                ret = TLS_ERR_INTERNAL_ERROR;
            }

            if (ret == TLS_RETURN_OK)
            {
                /* derive master secret by pre-master secret */
                ret = (int)TcpIp_TlsKeyDerive(ssl);
                ssl->state++;
            }
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerCertificateRequest(
    mbedtls_ssl_context* ssl
)
{
    int ret = TLS_ERR_FEATURE_UNAVAILABLE;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info = ssl->transform_negotiate->ciphersuite_info;
    uint32 dn_size, total_dn_size; /* excluding length bytes */
    uint32 ct_len, sa_len; /* including length bytes */
    uint8* bufPtr, *msgPtr;
    const uint8* const end = ssl->out_msg + TLS_OUT_CONTENT_LEN;
    const mbedtls_x509_crt* crt = NULL_PTR;
    uint16 authmode;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    KeyM_CertificateIdType CertId = tlsConnCfgPtr->TlsCertificateIdentityRef->TlsCipherKeyMLocalCertificateId;
    KeyM_CertDataType CertificateData;
    Std_ReturnType Std_Return = E_NOT_OK;

    if ((ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_DHE_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECDHE_PSK) ||
        (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_ECJPAKE) ||
        (FALSE == tlsConnCfgPtr->TlsUseClientAuthenticationRequest))
    {
        ssl->state++;
        ret = TLS_RETURN_OK;
    }
    else
    {
        ssl->state++;
#if(STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)

        if (ssl->handshake->sni_authmode != TLS_VERIFY_UNSET)
        {
            authmode = ssl->handshake->sni_authmode;
        }
        else
#endif
        {
            authmode = ssl->conf->authmode;
        }

        /*
         *   0  .   0   handshake type
         *   1  .   3   handshake length
         *   4  .   4   cert type count
         *   5  .. m-1  cert types
         *   m  .. m+1  sig alg length (TLS 1.2 only)
         *   m+1 .. n-1  SignatureAndHashAlgorithms (TLS 1.2 only)
         *   n  .. n+1  length of all DNs
         *   n+2 .. n+3  length of DN 1
         *   n+4 .. ...  Distinguished Name #1
         *   ... .. ...  length of DN 2, etc.
         */
        bufPtr = ssl->out_msg;
        msgPtr = bufPtr + 4u;
        /*
         * Supported certificate types
         *
         *     ClientCertificateType certificate_types<1..2^8-1>;
         *     enum { (255) } ClientCertificateType;
         */
        ct_len = 0u;
        msgPtr[1u + ct_len++] = TLS_CERT_TYPE_RSA_SIGN;
        msgPtr[0] = (uint8) ct_len++;
        msgPtr += ct_len;
        sa_len = 0u;

        /*
         * Add signature_algorithms for verify (TLS 1.2)
         *
         *     SignatureAndHashAlgorithm supported_signature_algorithms<2..2^16-2>;
         *
         *     struct {
         *           HashAlgorithm hash;
         *           SignatureAlgorithm signature;
         *     } SignatureAndHashAlgorithm;
         *
         *     enum { (255) } HashAlgorithm;
         *     enum { (255) } SignatureAlgorithm;
         */
        if (TLS_VERSION_V12 ==
            tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->TlsCiphersuiteDefinitionRef->TlsVersion)
        {
            /* Supported signature algorithms */
            msgPtr[2u + sa_len++] = TLS_HASH_SHA256;
            msgPtr[2u + sa_len++] = TLS_SIG_RSA;
            msgPtr[0] = (uint8)(sa_len >> 8u);
            msgPtr[1] = (uint8)(sa_len);
            sa_len += 2u;
            msgPtr += sa_len;
        }

        /* DistinguishedName certificate_authorities<0..2^16-1>;
         * opaque DistinguishedName<1..2^16-1>; */
        msgPtr += 2u;
        total_dn_size = 0u;

        if (ssl->conf->cert_req_ca_list ==  TLS_CERT_REQ_CA_LIST_ENABLED)
        {
#if(STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)

            if (ssl->handshake->sni_ca_chain != NULL_PTR)
            {
                crt = ssl->handshake->sni_ca_chain;
            }
            else
#endif
            {
                crt = ssl->conf->ca_chain;
            }

            Std_Return = KeyM_GetCertificate(CertId, &CertificateData);

            if (E_OK == Std_Return)
            {
                *msgPtr++ = (uint8)(CertificateData.certDataLength >> 8u);
                *msgPtr++ = (uint8)(CertificateData.certDataLength);
                (void)memcpy(msgPtr, CertificateData.certData, CertificateData.certDataLength);
                msgPtr += CertificateData.certDataLength;
                total_dn_size += 2u + CertificateData.certDataLength;
            }
            else
            {
                ret = TLS_ERR_BAD_HS_CERTIFICATE;
            }
        }

        if (ret == TLS_RETURN_OK)
        {
            ssl->out_msglen  = msgPtr - bufPtr;
            ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
            ssl->out_msg[0]  = TLS_HS_TYPE_CERTIFICATE_REQUEST;      /* 0->0   handshake type */
            ssl->out_msg[4u + ct_len + sa_len] = (uint8)(total_dn_size  >> 8u);
            ssl->out_msg[5u + ct_len + sa_len] = (uint8)(total_dn_size);
        }
    }

    if (ret == TLS_RETURN_OK)
    {
        ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerKeyExchange(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info = ssl->transform_negotiate->ciphersuite_info;

    if (ciphersuite_info->key_exchange == TLS_KEY_EXCHANGE_RSA)
    {
        /* Add header and send. */
        ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
        ssl->out_msg[0u]  = TLS_HS_TYPE_SERVER_KEY_EXCHANGE;
        ssl->state++;
        ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteServerHello(
    mbedtls_ssl_context* ssl
)
{
    int ret;
    uint32 olen, ext_len = 0u;
    uint8* bufPtr, *msgPtr;
    Std_ReturnType Std_Return = E_NOT_OK;
    TcpIp_SocketHandleType* socketMngPtr = ssl->p_bio;
    const TcpIp_TlsConnectionConfigType* tlsConnCfgPtr = socketMngPtr->tlsPtr->tlsCnnCfgPtr;
    /*     0  .   0   handshake type
     *     1  .   3   handshake length
     *     4  .   5   protocol version
     *     6  .   9   UNIX time()
     *    10  .  37   random bytes
     */
    bufPtr = ssl->out_msg;
    msgPtr = bufPtr + 4u;
    TcpIp_TlsWriteVersion(ssl, msgPtr);
    msgPtr += 2u;
    Std_Return = TcpIp_TlsGenerateRandomNumber(ssl);
    msgPtr += 32u;
    (void)memcpy(ssl->handshake->randbytes + 32u, bufPtr + 6u, 32u);

    if ((ssl->handshake->resume == 0u) &&
        (ssl->session_negotiate->id_len != 0u) &&
        (ssl->conf->f_get_cache != NULL_PTR) &&
        (ssl->conf->f_get_cache(ssl->conf->p_cache, ssl->session_negotiate) == 0u))
    {
        ssl->handshake->resume = 1;
    }

    if (ssl->handshake->resume == 0)
    {
        /* New session, create a new session id, unless we're about to issue a session ticket */
        ssl->state++;
        Std_Return = TcpIp_TlsCsmRandomGenerate(ssl->session_negotiate->id, &(ssl->session_negotiate->id_len));

        if (E_OK != Std_Return)
        {
            ret = TLS_ERR_INTERNAL_ERROR;
        }
    }
    else
    {
        /* Resuming a session */
        ssl->state = TLS_SERVER_CHANGE_CIPHER_SPEC;
        ret = (int)TcpIp_TlsKeyDerive(ssl);
    }

    if (ret == TLS_RETURN_OK)
    {
        /*   38  .  38     session id length
         *   39  . 38+n    session id
         *  39+n . 40+n    chosen ciphersuite
         *  41+n . 41+n    chosen compression alg.
         *  42+n . 43+n    extensions length
         *  44+n . 43+n+m  extensions */
        *msgPtr++ = (uint8) ssl->session_negotiate->id_len;
        (void)memcpy(msgPtr, ssl->session_negotiate->id, ssl->session_negotiate->id_len);
        msgPtr += ssl->session_negotiate->id_len;
        *msgPtr++ = (uint8)(ssl->session_negotiate->ciphersuite >> 8u);
        *msgPtr++ = (uint8)(ssl->session_negotiate->ciphersuite);
        *msgPtr++ = (uint8)(ssl->session_negotiate->compression);

        /* First write extensions, then the total length */

        if ((0u < tlsConnCfgPtr->TlsMaxFragmentLength) && (0x4000UL >= tlsConnCfgPtr->TlsMaxFragmentLength))
        {
            TcpIp_TlsServerWriteMaxFragmentLength(ssl, msgPtr + 2u + ext_len, &olen);
            ext_len += olen;
        }

        if (TRUE == tlsConnCfgPtr->TlsConnectionCiphersuiteWorkerRef->
            TlsCiphersuiteDefinitionRef->TlsUseSecurityExtensionForceEncryptThenMac)
        {
            TcpIp_TlsServerWriteEncryptThenMacExtension(ssl, msgPtr + 2u + ext_len, &olen);
            ext_len += olen;
        }

        if (ext_len > 0u)
        {
            *msgPtr++ = (uint8)((ext_len >> 8u) & 0xFFUL);
            *msgPtr++ = (uint8)((ext_len) & 0xFFUL);
            msgPtr += ext_len;
        }

        ssl->out_msglen  = msgPtr - bufPtr;
        ssl->out_msgtype = TLS_MSG_TYPE_HANDSHAKE;
        ssl->out_msg[0]  = TLS_HS_TYPE_SERVER_HELLO;
        ret = TcpIp_TlsWriteHandshakeMessage(ssl);
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckClientHelloMessageHeader(
    mbedtls_ssl_context* ssl,
    uint32 msg_len,
    uint8* bufPtr
)
{
    int ret = TLS_RETURN_OK;
    uint16 major, minor;

    /* Record layer */
    if (bufPtr[0] != TLS_MSG_TYPE_HANDSHAKE)                 /* message type */
    {
        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
    }
    else
    {
        TcpIp_TlsReadVersion(&major, &minor, bufPtr + 1u);       /* protocol version */

        if (major != TLS_MAJOR_VERSION_3)
        {
            ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
        }
        else
        {
            msg_len = (ssl->in_len[0] << 8u) | ssl->in_len[1];       /* message length */

            if (msg_len > TLS_IN_CONTENT_LEN)
            {
                ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
            }

            if (ret == TLS_RETURN_OK)
            {
                ret = TcpIp_TlsFetchInput(ssl, TLS_HEADER_LENGTH + msg_len);

                if (ret == TLS_RETURN_OK)
                {
                    ssl->in_left = 0;
                    bufPtr = ssl->in_msg;
                    ssl->handshake->update_checksum(ssl, bufPtr, msg_len);

                    /* Handshake layer:
                     *     0  .   0   handshake type
                     *     1  .   3   handshake length
                     */
                    if (msg_len < TLS_HANDSHAKE_HEADER_LENGTH)
                    {
                        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                    }
                    else
                    {
                        if (bufPtr[0] != TLS_HS_TYPE_CLIENT_HELLO)
                        {
                            ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                        }
                        else
                        {
                            /* We don't support fragmentation of ClientHello (yet?) */
                            if ((bufPtr[1] != 0u) ||
                                (msg_len != (TLS_HANDSHAKE_HEADER_LENGTH + ((bufPtr[2] << 8u) | bufPtr[3]))))
                            {
                                ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                            }

                            if (ret == TLS_RETURN_OK)
                            {
                                bufPtr += TLS_HANDSHAKE_HEADER_LENGTH;
                                msg_len -= TLS_HANDSHAKE_HEADER_LENGTH;
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
TcpIp_TlsCheckClientHelloExtendOffset(
    mbedtls_ssl_context* ssl,
    uint32 msg_len,
    uint32 ciph_len,
    uint32 ext_len,
    uint32 ciph_offset,
    uint8* bufPtr,
    uint8* extPtr
)
{
    int ret;
    uint32 comp_offset, ext_offset;
    uint32 comp_len;
    ciph_offset = 35u + ssl->session_negotiate->id_len;
    ciph_len = (bufPtr[ciph_offset + 0u] << 8u) | (bufPtr[ciph_offset + 1u]);

    if ((ciph_len < 2u) || ((ciph_len + 2u + ciph_offset + 1u) > msg_len) || ((ciph_len % 2u) != 0u))
    {
        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
    }
    else
    {
        /* Check the compression algorithms length and pick one */
        comp_offset = ciph_offset + 2u + ciph_len;
        comp_len = bufPtr[comp_offset];

        if ((comp_len < 1u) || (comp_len > 16u) || ((comp_len + comp_offset + 1u) > msg_len))
        {
            ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
        }
        else
        {
            ssl->session_negotiate->compression = TLS_COMPRESS_NULL;
            /* Check the extension length */
            ext_offset = comp_offset + 1u + comp_len;

            if (msg_len > ext_offset)
            {
                if (msg_len < (ext_offset + 2u))
                {
                    ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                }
                else
                {
                    ext_len = (bufPtr[ext_offset + 0u] << 8u) | (bufPtr[ext_offset + 1u]);

                    if (((ext_len > 0u) && (ext_len < 4u)) || (msg_len != (ext_offset + 2u + ext_len)))
                    {
                        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                    }
                }
            }
            else
            {
                ext_len = 0u;
            }

            if (ret == TLS_RETURN_OK)
            {
                extPtr = bufPtr + ext_offset + 2u;
            }
        }
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsCheckClientHello(
    mbedtls_ssl_context* ssl,
    uint32 msg_len,
    uint32 ciph_len,
    uint32 ext_len,
    uint32 ciph_offset,
    uint8* bufPtr,
    uint8* extPtr
)
{
    int ret = 0;
    uint32 sess_len = 0;
    ret = TcpIp_TlsFetchInput(ssl, 5u);

    if (ret == TLS_RETURN_OK)
    {
        bufPtr = ssl->in_hdr;
        ret = TcpIp_TlsCheckClientHelloMessageHeader(ssl, msg_len, bufPtr);

        /*  ClientHello layer:
         *     0  .   1   protocol version
         *     2  .  33   random bytes (starting with 4 bytes of Unix time)
         *    34  .  35   session id length (1 byte)
         *    35  . 34+x  session id
         *    ..  .  ..   ciphersuite list length (2 bytes)
         *    ..  .  ..   ciphersuite list
         *    ..  .  ..   compression alg. list length (1 byte)
         *    ..  .  ..   compression alg. list
         *    ..  .  ..   extensions length (2 bytes, optional)
         *    ..  .  ..   extensions (optional)
         */

        if (ret == TLS_RETURN_OK)
        {
            /*  Minimal length is 38 bytes. */
            if (msg_len < 38u)
            {
                ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
            }
            else
            {
                /* Check and save the protocol version, 0->1   protocol version */
                TcpIp_TlsReadVersion(&ssl->major_ver, &ssl->minor_ver, bufPtr);
                ssl->handshake->max_major_ver = ssl->major_ver;
                ssl->handshake->max_minor_ver = ssl->minor_ver;
                /* store client random (include Unix time), 2->33   random bytes(starting with 4 bytes of Unix time) */
                (void)memcpy(ssl->handshake->randbytes, bufPtr + 2, 32);
                /* Check the session ID length and save session ID,34->35   session id length (1 byte) */
                sess_len = bufPtr[34];

                if ((sess_len > sizeof(ssl->session_negotiate->id)) || ((sess_len + 34u + 2u) > msg_len))
                {
                    ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                }
                else
                {
                    ssl->session_negotiate->id_len = sess_len;
                    (void)memset(ssl->session_negotiate->id, 0, sizeof(ssl->session_negotiate->id));
                    (void)memcpy(ssl->session_negotiate->id, bufPtr + 35, ssl->session_negotiate->id_len);
                    ret = TcpIp_TlsCheckClientHelloExtendOffset(ssl, msg_len, ciph_len, ext_len, ciph_offset, bufPtr, extPtr);
                }
            }
        }
    }

    return ret;
}


/* Search for a matching ciphersuite */
static FUNC(int, TCPIP_CODE)
TcpIp_TlsMatchLocalCipherSuite(
    mbedtls_ssl_context* ssl,
    uint32 ciph_len,
    uint32 ciph_offset,
    uint8* msgPtr,
    uint8* bufPtr
)
{
    int ret;
    const int* ciphersuites;
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info;
    ciphersuites = ssl->conf->ciphersuite_list[ssl->minor_ver];
    ciphersuite_info = NULL_PTR;

    if ((msgPtr[0] == ((ciphersuites[0] >> 8)) & 0xFFu) || (msgPtr[1] == (ciphersuites[0]  & 0xFFu)))
    {
        ciphersuite_info = TcpIp_TlsGetCiphersuiteFromId(ciphersuites[0]);

        if (ciphersuite_info != NULL_PTR)
        {
            ssl->session_negotiate->ciphersuite = ciphersuites[0];
            ssl->transform_negotiate->ciphersuite_info = ciphersuite_info;
            ssl->state++;
            ret = TLS_RETURN_OK;
        }
    }
    else
    {
        ret = TLS_ERR_NO_USABLE_CIPHERSUITE;
    }

    return ret;
}


static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseServerNameExtension(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
)
{
    int ret = TLS_RETURN_OK;
    uint32 servername_list_size, hostname_len;
    const uint8* p;

    if (len < 2u)
    {
        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
    }

    if (ret == TLS_RETURN_OK)
    {
        servername_list_size = ((buf[0] << 8u) | (buf[1]));

        if ((servername_list_size + 2u) != len)
        {
            ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
        }

        if (ret == TLS_RETURN_OK)
        {
            p = buf + 2u;

            while (servername_list_size > 2u)
            {
                hostname_len = ((p[1] << 8u) | p[2]);

                if ((hostname_len + 3u) > servername_list_size)
                {
                    return (TLS_ERR_BAD_HS_CLIENT_HELLO);
                }

                if (p[0] == TLS_EXT_SERVERNAME_HOSTNAME)
                {
                    ret = ssl->conf->f_sni(ssl->conf->p_sni, ssl, p + 3u, hostname_len);

                    if (ret != TLS_RETURN_OK)
                    {
                        return (TLS_ERR_BAD_HS_CLIENT_HELLO);
                    }

                    return (0);
                }

                servername_list_size -= hostname_len + 3u;
                p += hostname_len + 3u;
            }

            if (servername_list_size != 0u)
            {
                return (TLS_ERR_BAD_HS_CLIENT_HELLO);
            }

            return (0);
        }
    }

    return ret;
}

static FUNC(int, TCPIP_CODE)
TcpIp_TlsParseClientHello(
    mbedtls_ssl_context* ssl
)
{
    int ret = 0;
    uint32 ciph_offset = 0;
    uint32 msg_len = 0;
    uint32 ciph_len = 0;
    uint32 ext_len = 0;
    uint8* bufPtr = NULL_PTR;
    uint8* msgPtr = NULL_PTR;
    uint8* extPtr = NULL_PTR;
    ret = TcpIp_TlsCheckClientHello(ssl, msg_len, ciph_len, ext_len, ciph_offset, bufPtr, extPtr);

    if (ret == TLS_RETURN_OK)
    {
        while (ext_len != 0u)
        {
            uint16 ext_id;
            uint16 ext_size;

            if (ext_len < 4u)
            {
                ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
            }

            if (ret == TLS_RETURN_OK)
            {
                ext_id   = ((extPtr[0] << 8u) | (extPtr[1]));
                ext_size = ((extPtr[2] << 8u) | (extPtr[3]));

                if ((ext_size + 4u) > ext_len)
                {
                    ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                }

                if (ret == TLS_RETURN_OK)
                {
                    switch (ext_id)
                    {
#if (STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)

                        case TLS_EXT_SERVERNAME:
                            if (ssl->conf->f_sni == NULL_PTR)
                            {
                                break;
                            }

                            ret = TcpIp_TlsParseServerNameExtension(ssl, extPtr + 4u, ext_size);
                            break;
#endif /* TCPIP_TLS_SERVER_NAME_INDICATION */

                        case TLS_EXT_RENEGOTIATION_INFO:
                            break;

                        case TLS_EXT_SIG_ALG:
                            ret = TcpIp_TlsParseClientSignatureAlgorithmsExtension(ssl, extPtr + 4u, ext_size);
                            break;
#if (STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)

                        case TLS_EXT_MAX_FRAGMENT_LENGTH:
                            ret = TcpIp_TlsParseClientMaxFragmentLengthExtension(ssl, extPtr + 4u, ext_size);
                            break;
#endif /* TLS_MAX_FRAGMENT_LENGTH_ENABLED */
#if (STD_ON == TLS_ENCRYPT_THEN_MAC)

                        case TLS_EXT_ENCRYPT_THEN_MAC:
                            ret = TcpIp_TlsParseClientEncryptThenMacExtension(ssl, extPtr + 4u, ext_size);
                            break;
#endif /* TLS_ENCRYPT_THEN_MAC */

                        default:
                            break;
                    }

                    ext_len -= 4u + ext_size;
                    extPtr += 4u + ext_size;

                    if ((ext_len > 0u) && (ext_len < 4u))
                    {
                        ret = TLS_ERR_BAD_HS_CLIENT_HELLO;
                    }
                }
            }
        }

        if (ret == TLS_RETURN_OK)
        {
            ret = TcpIp_TlsMatchLocalCipherSuite(ssl, ciph_len, ciph_offset, msgPtr, bufPtr);
        }
    }

    return ret;
}

FUNC(int, TCPIP_CODE)
TcpIp_TlsServerHandshakeStep(
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

        if (ret == TLS_RETURN_OK)
        {
            switch (ssl->state)
            {
                case TLS_HELLO_REQUEST:
                    ssl->state = TLS_CLIENT_HELLO;
                    break;

                /*
                 *  <==   ClientHello
                 */
                case TLS_CLIENT_HELLO:
                    ret = TcpIp_TlsParseClientHello(ssl);
                    break;

                /*
                 *  ==>   ServerHello
                 *        Certificate
                 *      ( ServerKeyExchange  )
                 *      ( CertificateRequest )
                 *        ServerHelloDone
                 */
                case TLS_SERVER_HELLO:
                    ret = TcpIp_TlsWriteServerHello(ssl);
                    break;

                case TLS_SERVER_CERTIFICATE:
                    ret = TcpIp_TlsWriteCertificate(ssl);
                    break;

                case TLS_SERVER_KEY_EXCHANGE:
                    ret = TcpIp_TlsWriteServerKeyExchange(ssl);
                    break;

                case TLS_CERTIFICATE_REQUEST:
                    ret = TcpIp_TlsWriteServerCertificateRequest(ssl);
                    break;

                case TLS_SERVER_HELLO_DONE:
                    ret = TcpIp_TlsWriteServerHelloDone(ssl);
                    break;

                /*
                 *  <== ( Certificate/Alert  )
                 *        ClientKeyExchange
                 *      ( CertificateVerify  )
                 *        ChangeCipherSpec
                 *        Finished
                 */
                case TLS_CLIENT_CERTIFICATE:
                    ret = TcpIp_TlsParseCertificate(ssl);
                    break;

                case TLS_CLIENT_KEY_EXCHANGE:
                    ret = TcpIp_TlsParseClientKeyExchange(ssl);
                    break;

                case TLS_CERTIFICATE_VERIFY:
                    ret = TcpIp_TlsParseClientCertificateVerify(ssl);
                    break;

                case TLS_CLIENT_CHANGE_CIPHER_SPEC:
                    ret = TcpIp_TlsParseChangeCipherSpec(ssl);
                    break;

                case TLS_CLIENT_FINISHED:
                    ret = TcpIp_TlsParseHandshakeFinished(ssl);
                    break;

                /*
                 *  ==> ( NewSessionTicket )
                 *        ChangeCipherSpec
                 *        Finished
                 */
                case TLS_SERVER_CHANGE_CIPHER_SPEC:
                    ret = TcpIp_TLsWriteChangeCipherSpec(ssl);
                    break;

                case TLS_SERVER_FINISHED:
                    ret = TcpIp_TlsWriteHandshakeFinished(ssl);
                    break;

                case TLS_FLUSH_BUFFERS:
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

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#endif/* (TCPIP_TCP_TLS_ENABLED == STD_ON) */
