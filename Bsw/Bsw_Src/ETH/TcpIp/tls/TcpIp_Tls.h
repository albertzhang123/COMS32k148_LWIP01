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
**  FILENAME    : TcpIp_Tls.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : pengfei.zhu                                                 **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : function declaration of tcp_tls                             **
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
#include "TcpIp.h"
#include "TcpIp_TlsTypes.h"

#ifndef TCPIP_TLS_H_
#define TCPIP_TLS_H_

#define TcpIp_TlsMemFree      mem_free
#define TcpIp_TlsMemCalloc    mem_calloc

BEGIN_C_DECLS

#if (TCPIP_TCP_TLS_ENABLED == STD_ON)
/*************************Function Declaration***********************************/
FUNC(int, TCPIP_CODE)
TcpIp_TlsServerHandshakeStep(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsSendErrorAlertMessage(
    mbedtls_ssl_context* ssl,
    uint8 level, uint8 message
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteHandshakeMessage(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteRecord(
    mbedtls_ssl_context* ssl,
    uint8 force_flush
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsReadRecord(
    mbedtls_ssl_context* ssl,
    uint8 update_hs_digest
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsFetchInput(
    mbedtls_ssl_context* ssl,
    uint32 nb_want
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteCertificate(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TLsWriteChangeCipherSpec(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsHandshakeStart(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsHandshakeStep(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsClientHandshakeStep(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsParseCertificate(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsParseChangeCipherSpec(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteHandshakeFinished(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsParseHandshakeFinished(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsHandleMessageType(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsPrepareHandshakeRecord(
    mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsReadData(
    mbedtls_ssl_context* ssl,
    uint8* buf,
    uint32 len
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsGetBytesAvail(
    const mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsWriteData(
    mbedtls_ssl_context* ssl,
    const uint8* buf,
    uint32 len
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsGetMaxOutRecordPayload(
    const mbedtls_ssl_context* ssl
);
FUNC(int, TCPIP_CODE)
TcpIp_TlsSslSetup(
    mbedtls_ssl_context* ssl,
    const mbedtls_ssl_config* conf
);
FUNC(void, TCPIP_CODE)
TcpIp_TlsUpdateHandshakeStatus(
    mbedtls_ssl_context* ssl
);
FUNC(void, TCPIP_CODE)
TcpIp_TlsAddSignatureHash(
    mbedtls_ssl_sig_hash_set_t* set,
    mbedtls_pk_type_t sig_alg,
    mbedtls_md_type_t md_alg
);
FUNC(void, TCPIP_CODE)
TcpIp_TlsHandshakeWrapUp(
    mbedtls_ssl_context* ssl
);
FUNC(void, TCPIP_CODE)
TcpIp_TlsReadVersion(
    uint16* major,
    uint16* minor,
    const uint8 ver[2]
);
FUNC(void, TCPIP_CODE)
TcpIp_TlsWriteVersion(
    mbedtls_ssl_context* ssl,
    uint8 ver[2]
);
FUNC(uint8, TCPIP_CODE)
TcpIp_TlsGetSignatureTypeByPkAlgorithm(
    mbedtls_pk_type_t type
);
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsKeyDerive(
    mbedtls_ssl_context* ssl
);
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsFlushOutput(
    mbedtls_ssl_context* ssl
);
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsGenerateRandomNumber(
    mbedtls_ssl_context* ssl
);
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_GetCertificate(
    VAR(TcpIp_KeyMCertificateIdType, AUTOMATIC) CertId,
    P2VAR(KeyM_CertDataType, AUTOMATIC, TCPIP_APPL_CONST) CertificateDataPtr
);
FUNC(mbedtls_pk_type_t, TCPIP_CODE)
TcpIp_TlsGetPkAlgorithmBySignatureType(
    uint8 sig
);
FUNC(mbedtls_md_type_t, TCPIP_CODE)
TcpIp_TlsGetMdAlgorithmByHashType(
    uint8 hash
);
extern const mbedtls_ssl_ciphersuite_t*
TcpIp_TlsGetCiphersuiteFromId(
    int ciphersuite
);

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsCsmRandomGenerate(
    uint8* resultPtr,
    uint32* resultLengthPtr);


/********************************************************************************************/
#define TLS_EXT_MAX_FRAGMENT_LENGTH         1u  /* fragment flag */
#define TLS_MAX_FRAG_LEN_INVALID            0u  /* don't support fragment */

#define TLS_ETM_DISABLED                    0u
#define TLS_ETM_ENABLED                     1u

#define TLS_EXT_SIG_ALG                     13u

#define TLS_EXT_SERVERNAME                  0u
#define TLS_EXT_SERVERNAME_HOSTNAME         0u

#define TLS_EXT_ENCRYPT_THEN_MAC            22u /* 0x16 */
#define TLS_EXT_EXTENDED_MASTER_SECRET      0x17 /* 23 */

#define TLS_VERIFY_NONE                     0u
#define TLS_VERIFY_OPTIONAL                 1u
#define TLS_VERIFY_REQUIRED                 2u
#define TLS_VERIFY_UNSET                    3u /* Used only for sni_authmode */

#define TLS_COMPRESS_NULL                   0u

#define TLS_CERT_TYPE_RSA_SIGN              1u
#define TLS_CERT_TYPE_ECDSA_SIGN            64u

#define TLS_CERT_REQ_CA_LIST_ENABLED        1u
#define TLS_CERT_REQ_CA_LIST_DISABLED       0u

#define TLS_EXT_RENEGOTIATION_INFO          0xFF01

/************************************** NET ERROR ******************************************/
/**< Return ok */
#define TLS_RETURN_OK                                  0x0000
/**< Failed to open a socket. */
#define TLS_ERR_NET_SOCKET_FAILED                     -0x0042
/**< The connection to the given server / port failed. */
#define TLS_ERR_NET_CONNECT_FAILED                    -0x0044
/**< Binding of the socket failed. */
#define TLS_ERR_NET_BIND_FAILED                       -0x0046
/**< Could not listen on the socket. */
#define TLS_ERR_NET_LISTEN_FAILED                     -0x0048
/**< Could not accept the incoming connection. */
#define TLS_ERR_NET_ACCEPT_FAILED                     -0x004A
/**< Reading information from the socket failed. */
#define TLS_ERR_NET_RECV_FAILED                       -0x004C
/**< Sending information through the socket failed. */
#define TLS_ERR_NET_SEND_FAILED                       -0x004E
/**< Connection was reset by peer. */
#define TLS_ERR_NET_CONN_RESET                        -0x0050
/**< Failed to get an IP address for the given hostname. */
#define TLS_ERR_NET_UNKNOWN_HOST                      -0x0052
/**< Buffer is too small to hold the data. */
#define TLS_ERR_NET_BUFFER_TOO_SMALL                  -0x0043
/**< The context is invalid, eg because it was free()ed. */
#define TLS_ERR_NET_INVALID_CONTEXT                   -0x0045
/**< Polling the net context failed. */
#define TLS_ERR_NET_POLL_FAILED                       -0x0047
/**< Input invalid. */
#define TLS_ERR_NET_BAD_INPUT_DATA                    -0x0049
/**< Read error */
#define TLS_ERR_SSL_WANT_READ                         -0x0051
/**< Write error */
#define TLS_ERR_SSL_WANT_WRITE                        -0x0052

/*************************** MESSAGE LENGTH DEFINITION **************************************/
#define TLS_HEADER_LEN              13u
#define TLS_MAX_CONTENT_LEN         2000u
#define TLS_MAC_ADD                 32u
#define TLS_IN_CONTENT_LEN          TLS_MAX_CONTENT_LEN
#define TLS_OUT_CONTENT_LEN         TLS_MAX_CONTENT_LEN

#define TLS_OUT_PAYLOAD_LEN         TLS_PAYLOAD_OVERHEAD + TLS_OUT_CONTENT_LEN
#define TLS_PAYLOAD_OVERHEAD        TLS_MAX_IV_LENGTH + TLS_MAC_ADD
#define TLS_IN_PAYLOAD_LEN          TLS_PAYLOAD_OVERHEAD + TLS_IN_CONTENT_LEN
#define TLS_IN_BUFFER_LEN           TLS_HEADER_LEN + TLS_IN_PAYLOAD_LEN
#define TLS_OUT_BUFFER_LEN          TLS_HEADER_LEN + TLS_OUT_PAYLOAD_LEN

/*********************************** CERT ERROR DEFINITION *******************************************/
/**< The certificate validity has expired. */
#define TLS_X509_BADCERT_EXPIRED             0x01
/**< The certificate has been revoked (is on a CRL). */
#define TLS_X509_BADCERT_REVOKED             0x02
/**< The certificate Common Name (CN) does not match with the expected CN. */
#define TLS_X509_BADCERT_CN_MISMATCH         0x04
/**< The certificate is not correctly signed by the trusted CA. */
#define TLS_X509_BADCERT_NOT_TRUSTED         0x08
/**< The CRL is not correctly signed by the trusted CA. */
#define TLS_X509_BADCRL_NOT_TRUSTED          0x10
/**< The CRL is expired. */
#define TLS_X509_BADCRL_EXPIRED              0x20
/**< Certificate was missing. */
#define TLS_X509_BADCERT_MISSING             0x40
/**< Certificate verification was skipped. */
#define TLS_X509_BADCERT_SKIP_VERIFY         0x80
/**< Other reason (can be used by verify callback) */
#define TLS_X509_BADCERT_OTHER             0x0100
/**< The certificate validity starts in the future. */
#define TLS_X509_BADCERT_FUTURE            0x0200
/**< The CRL is from the future */
#define TLS_X509_BADCRL_FUTURE             0x0400
/**< Usage does not match the keyUsage extension. */
#define TLS_X509_BADCERT_KEY_USAGE         0x0800
/**< Usage does not match the extendedKeyUsage extension. */
#define TLS_X509_BADCERT_EXT_KEY_USAGE     0x1000
/**< Usage does not match the nsCertType extension. */
#define TLS_X509_BADCERT_NS_CERT_TYPE      0x2000
/**< The certificate is signed with an unacceptable hash. */
#define TLS_X509_BADCERT_BAD_MD            0x4000
/**< The certificate is signed with an unacceptable PK alg (eg RSA vs ECDSA). */
#define TLS_X509_BADCERT_BAD_PK            0x8000
/**< The certificate is signed with an unacceptable key (eg bad curve, RSA too short). */
#define TLS_X509_BADCERT_BAD_KEY         0x010000
/**< The CRL is signed with an unacceptable hash. */
#define TLS_X509_BADCRL_BAD_MD           0x020000
/**< The CRL is signed with an unacceptable PK alg (eg RSA vs ECDSA). */
#define TLS_X509_BADCRL_BAD_PK           0x040000
/**< The CRL is signed with an unacceptable key (eg bad curve, RSA too short). */
#define TLS_X509_BADCRL_BAD_KEY          0x080000

/* Supported Signature and Hash algorithms (For TLS 1.2) RFC 5246 section 7.4.1.4.1 */
#define TLS_SIG_ANON                 0u
#define TLS_SIG_RSA                  1u
#define TLS_SIG_ECDSA                3u

#define TLS_HASH_NONE                0u
#define TLS_HASH_MD5                 1u
#define TLS_HASH_SHA1                2u
#define TLS_HASH_SHA224              3u
#define TLS_HASH_SHA256              4u
#define TLS_HASH_SHA384              5u
#define TLS_HASH_SHA512              6u

/********************************** KEY ELEMENT ID **************************************/
#define TLS_KEY_ELEMENT_RANDOM_NUMBER           0x11u       /*todo : pengfei.zhu;fix this*/
#define TLS_KEY_ELEMENT_PREMASTER_SECRET        0x33u

/********************************** MESSAGE TYPE ******************************************/
#define TLS_MSG_TYPE_CHANGE_CIPHER_SPEC         20u
#define TLS_MSG_TYPE_ALERT                      21u
#define TLS_MSG_TYPE_HANDSHAKE                  22u
#define TLS_MSG_TYPE_APPLICATION_DATA           23u

#define TLS_FORCE_FLUSH                         1u
#define TLS_EMPTY_RENEGOTIATION_INFO            0xFF   /**< renegotiation info ext */

/******************************** HANDSHAKE HEADER TYPE ***********************************/
#define TLS_HS_TYPE_HELLO_REQUEST                0u
#define TLS_HS_TYPE_CLIENT_HELLO                 1u
#define TLS_HS_TYPE_SERVER_HELLO                 2u
#define TLS_HS_TYPE_HELLO_VERIFY_REQUEST         3u
#define TLS_HS_TYPE_NEW_SESSION_TICKET           4u
#define TLS_HS_TYPE_CERTIFICATE                  11u
#define TLS_HS_TYPE_SERVER_KEY_EXCHANGE          12u
#define TLS_HS_TYPE_CERTIFICATE_REQUEST          13u
#define TLS_HS_TYPE_SERVER_HELLO_DONE            14u
#define TLS_HS_TYPE_CERTIFICATE_VERIFY           15u
#define TLS_HS_TYPE_CLIENT_KEY_EXCHANGE          16u
#define TLS_HS_TYPE_FINISHED                     20u

/*********************************** HEADER LENGTH ***************************************/
#define TLS_HANDSHAKE_HEADER_LENGTH             4u
#define TLS_HEADER_LENGTH                       5u
#define TLS_WRITE_CERTIFICATE_HEADER_LENGTH     7u

/******************* SUPPORT CIPHERSUITES (Official IANA names) *************************/
#define TLS_RSA_WITH_NULL_SHA256                 0x3B
#define TLS_RSA_WITH_AES_128_CBC_SHA256          0x3C
#define TLS_RSA_WITH_AES_256_CBC_SHA256          0x3D

#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA256      0x67
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA256      0x6B

#define TLS_RSA_WITH_AES_128_GCM_SHA256          0x9C
#define TLS_RSA_WITH_AES_256_GCM_SHA384          0x9D
#define TLS_DHE_RSA_WITH_AES_128_GCM_SHA256      0x9E
#define TLS_DHE_RSA_WITH_AES_256_GCM_SHA384      0x9F

#define TLS_PSK_WITH_AES_128_GCM_SHA256          0xA8
#define TLS_PSK_WITH_AES_256_GCM_SHA384          0xA9
#define TLS_DHE_PSK_WITH_AES_128_GCM_SHA256      0xAA
#define TLS_DHE_PSK_WITH_AES_256_GCM_SHA384      0xAB
#define TLS_RSA_PSK_WITH_AES_128_GCM_SHA256      0xAC
#define TLS_RSA_PSK_WITH_AES_256_GCM_SHA384      0xAD

#define TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256     0xBA
#define TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256 0xBE

#define TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256     0xC0
#define TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256 0xC4

#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256  0xC023
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  0xC024
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256   0xC025
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384   0xC026
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256    0xC027
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384    0xC028
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256     0xC029
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384     0xC02A

#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256  0xC02B
#define TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384  0xC02C
#define TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256   0xC02D
#define TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384   0xC02E
#define TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256    0xC02F
#define TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384    0xC030
#define TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256     0xC031
#define TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384     0xC032

#define TLS_RSA_WITH_ARIA_128_CBC_SHA256         0xC03C
#define TLS_RSA_WITH_ARIA_256_CBC_SHA384         0xC03D
#define TLS_DHE_RSA_WITH_ARIA_128_CBC_SHA256     0xC044
#define TLS_DHE_RSA_WITH_ARIA_256_CBC_SHA384     0xC045
#define TLS_ECDHE_ECDSA_WITH_ARIA_128_CBC_SHA256 0xC048
#define TLS_ECDHE_ECDSA_WITH_ARIA_256_CBC_SHA384 0xC049
#define TLS_ECDH_ECDSA_WITH_ARIA_128_CBC_SHA256  0xC04A
#define TLS_ECDH_ECDSA_WITH_ARIA_256_CBC_SHA384  0xC04B
#define TLS_ECDHE_RSA_WITH_ARIA_128_CBC_SHA256   0xC04C
#define TLS_ECDHE_RSA_WITH_ARIA_256_CBC_SHA384   0xC04D
#define TLS_ECDH_RSA_WITH_ARIA_128_CBC_SHA256    0xC04E
#define TLS_ECDH_RSA_WITH_ARIA_256_CBC_SHA384    0xC04F
#define TLS_RSA_WITH_ARIA_128_GCM_SHA256         0xC050
#define TLS_RSA_WITH_ARIA_256_GCM_SHA384         0xC051
#define TLS_DHE_RSA_WITH_ARIA_128_GCM_SHA256     0xC052
#define TLS_DHE_RSA_WITH_ARIA_256_GCM_SHA384     0xC053
#define TLS_ECDHE_ECDSA_WITH_ARIA_128_GCM_SHA256 0xC05C
#define TLS_ECDHE_ECDSA_WITH_ARIA_256_GCM_SHA384 0xC05D
#define TLS_ECDH_ECDSA_WITH_ARIA_128_GCM_SHA256  0xC05E
#define TLS_ECDH_ECDSA_WITH_ARIA_256_GCM_SHA384  0xC05F
#define TLS_ECDHE_RSA_WITH_ARIA_128_GCM_SHA256   0xC060
#define TLS_ECDHE_RSA_WITH_ARIA_256_GCM_SHA384   0xC061
#define TLS_ECDH_RSA_WITH_ARIA_128_GCM_SHA256    0xC062
#define TLS_ECDH_RSA_WITH_ARIA_256_GCM_SHA384    0xC063
#define TLS_PSK_WITH_ARIA_128_CBC_SHA256         0xC064
#define TLS_PSK_WITH_ARIA_256_CBC_SHA384         0xC065
#define TLS_DHE_PSK_WITH_ARIA_128_CBC_SHA256     0xC066
#define TLS_DHE_PSK_WITH_ARIA_256_CBC_SHA384     0xC067
#define TLS_RSA_PSK_WITH_ARIA_128_CBC_SHA256     0xC068
#define TLS_RSA_PSK_WITH_ARIA_256_CBC_SHA384     0xC069
#define TLS_PSK_WITH_ARIA_128_GCM_SHA256         0xC06A
#define TLS_PSK_WITH_ARIA_256_GCM_SHA384         0xC06B
#define TLS_DHE_PSK_WITH_ARIA_128_GCM_SHA256     0xC06C
#define TLS_DHE_PSK_WITH_ARIA_256_GCM_SHA384     0xC06D
#define TLS_RSA_PSK_WITH_ARIA_128_GCM_SHA256     0xC06E
#define TLS_RSA_PSK_WITH_ARIA_256_GCM_SHA384     0xC06F
#define TLS_ECDHE_PSK_WITH_ARIA_128_CBC_SHA256   0xC070
#define TLS_ECDHE_PSK_WITH_ARIA_256_CBC_SHA384   0xC071

#define TLS_RSA_WITH_CAMELLIA_128_GCM_SHA256         0xC07A
#define TLS_RSA_WITH_CAMELLIA_256_GCM_SHA384         0xC07B
#define TLS_DHE_RSA_WITH_CAMELLIA_128_GCM_SHA256     0xC07C
#define TLS_DHE_RSA_WITH_CAMELLIA_256_GCM_SHA384     0xC07D
#define TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_GCM_SHA256 0xC086
#define TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_GCM_SHA384 0xC087
#define TLS_ECDH_ECDSA_WITH_CAMELLIA_128_GCM_SHA256  0xC088
#define TLS_ECDH_ECDSA_WITH_CAMELLIA_256_GCM_SHA384  0xC089
#define TLS_ECDHE_RSA_WITH_CAMELLIA_128_GCM_SHA256   0xC08A
#define TLS_ECDHE_RSA_WITH_CAMELLIA_256_GCM_SHA384   0xC08B
#define TLS_ECDH_RSA_WITH_CAMELLIA_128_GCM_SHA256    0xC08C
#define TLS_ECDH_RSA_WITH_CAMELLIA_256_GCM_SHA384    0xC08D

#define TLS_PSK_WITH_CAMELLIA_128_GCM_SHA256       0xC08E
#define TLS_PSK_WITH_CAMELLIA_256_GCM_SHA384       0xC08F
#define TLS_DHE_PSK_WITH_CAMELLIA_128_GCM_SHA256   0xC090
#define TLS_DHE_PSK_WITH_CAMELLIA_256_GCM_SHA384   0xC091
#define TLS_RSA_PSK_WITH_CAMELLIA_128_GCM_SHA256   0xC092
#define TLS_RSA_PSK_WITH_CAMELLIA_256_GCM_SHA384   0xC093

#define TLS_RSA_WITH_AES_128_CCM                0xC09C
#define TLS_RSA_WITH_AES_256_CCM                0xC09D
#define TLS_DHE_RSA_WITH_AES_128_CCM            0xC09E
#define TLS_DHE_RSA_WITH_AES_256_CCM            0xC09F
#define TLS_RSA_WITH_AES_128_CCM_8              0xC0A0
#define TLS_RSA_WITH_AES_256_CCM_8              0xC0A1
#define TLS_DHE_RSA_WITH_AES_128_CCM_8          0xC0A2
#define TLS_DHE_RSA_WITH_AES_256_CCM_8          0xC0A3
#define TLS_PSK_WITH_AES_128_CCM                0xC0A4
#define TLS_PSK_WITH_AES_256_CCM                0xC0A5
#define TLS_DHE_PSK_WITH_AES_128_CCM            0xC0A6
#define TLS_DHE_PSK_WITH_AES_256_CCM            0xC0A7
#define TLS_PSK_WITH_AES_128_CCM_8              0xC0A8
#define TLS_PSK_WITH_AES_256_CCM_8              0xC0A9
#define TLS_DHE_PSK_WITH_AES_128_CCM_8          0xC0AA
#define TLS_DHE_PSK_WITH_AES_256_CCM_8          0xC0AB

#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM        0xC0AC
#define TLS_ECDHE_ECDSA_WITH_AES_256_CCM        0xC0AD
#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8      0xC0AE
#define TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8      0xC0AF

/* RFC 7905 */
#define TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   0xCCA8
#define TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 0xCCA9
#define TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256     0xCCAA
#define TLS_PSK_WITH_CHACHA20_POLY1305_SHA256         0xCCAB
#define TLS_ECDHE_PSK_WITH_CHACHA20_POLY1305_SHA256   0xCCAC
#define TLS_DHE_PSK_WITH_CHACHA20_POLY1305_SHA256     0xCCAD
#define TLS_RSA_PSK_WITH_CHACHA20_POLY1305_SHA256     0xCCAE

/*********************************** ERROR CODE *****************************************/
#define TLS_ERR_INIT_VAL                          -0x1000
#define TLS_ERR_READ_ERR                          -0x1001
#define TLS_ERR_BAD_MESSAGE_ERR                   -0x1002
#define TLS_ERR_DERIVE_KEY                        -0x1003
#define TLS_ERR_FRAGMENT_ERR                      -0x1004
#define TLS_ERR_ENCRYPT_THEN_MAC                  -0x1005
#define TLS_ERR_ENCRYPT_ERROR                     -0x1006
#define TLS_ERR_HS_TYPE_ERR                       -0x1007
#define TLS_ERR_SEND_FAILED                       -0x1008
#define TLS_ERR_MEM_OVERFLOW                      -0x1009
#define TLS_ERR_CSM_HASH_ERROR                    -0x1010
#define TLS_ERR_HS_INIT                           -0x1011
#define TLS_ERR_CERTIFICATE_VERIFY_FAILED         -0x1012

/**< The requested feature is not available. */
#define TLS_ERR_FEATURE_UNAVAILABLE               -0x7080
/**< Bad input parameters to function. */
#define TLS_ERR_BAD_INPUT_DATA                    -0x7100
#define TLS_ERR_INVALID_MAC                       -0x7180
/**< Verification of the message MAC failed. */
#define TLS_ERR_INVALID_RECORD                    -0x7200
/**< An invalid SSL record was received. */
#define TLS_ERR_CONN_EOF                          -0x7280
/**< The connection indicated an EOF. */
#define TLS_ERR_UNKNOWN_CIPHER                    -0x7300
/**< An unknown cipher was received. */
/**< The server has no ciphersuites in common with the client. */
#define TLS_ERR_NO_CIPHER_CHOSEN                  -0x7380
/**< No RNG was provided to the SSL module. */
#define TLS_ERR_NO_RNG                            -0x7400
/**< No client certification received from the client, but required by the authentication mode. */
#define TLS_ERR_NO_CLIENT_CERTIFICATE             -0x7480
/**< Our own certificate(s) is/are too large to send in an SSL message. */
#define TLS_ERR_CERTIFICATE_TOO_LARGE             -0x7500
/**< The own certificate is not set, but needed by the server. */
#define TLS_ERR_CERTIFICATE_REQUIRED              -0x7580
/**< The own private key or pre-shared key is not set, but needed. */
#define TLS_ERR_PRIVATE_KEY_REQUIRED              -0x7600
/**< No CA Chain is set, but required to operate. */
#define TLS_ERR_CA_CHAIN_REQUIRED                 -0x7680
/**< An unexpected message was received from our peer. */
#define TLS_ERR_UNEXPECTED_MESSAGE                -0x7700
/**< A fatal alert message was received from our peer. */
#define TLS_ERR_FATAL_ALERT_MESSAGE               -0x7780
/**< Verification of our peer failed. */
#define TLS_ERR_PEER_VERIFY_FAILED                -0x7800
/**< The peer notified us that the connection is going to be closed. */
#define TLS_ERR_PEER_CLOSE_NOTIFY                 -0x7880
/**< Processing of the ClientHello handshake message failed. */
#define TLS_ERR_BAD_HS_CLIENT_HELLO               -0x7900
/**< Processing of the ServerHello handshake message failed. */
#define TLS_ERR_BAD_HS_SERVER_HELLO               -0x7980
/**< Processing of the Certificate handshake message failed. */
#define TLS_ERR_BAD_HS_CERTIFICATE                -0x7A00
/**< Processing of the CertificateRequest handshake message failed. */
#define TLS_ERR_BAD_HS_CERTIFICATE_REQUEST        -0x7A80
/**< Processing of the ServerKeyExchange handshake message failed. */
#define TLS_ERR_BAD_HS_SERVER_KEY_EXCHANGE        -0x7B00
/**< Processing of the ServerHelloDone handshake message failed. */
#define TLS_ERR_BAD_HS_SERVER_HELLO_DONE          -0x7B80
/**< Processing of the ClientKeyExchange handshake message failed. */
#define TLS_ERR_BAD_HS_CLIENT_KEY_EXCHANGE        -0x7C00
/**< Processing of the ClientKeyExchange handshake message failed in DHM / ECDH Read Public. */
#define TLS_ERR_BAD_HS_CLIENT_KEY_EXCHANGE_RP     -0x7C80
/**< Processing of the ClientKeyExchange handshake message failed in DHM / ECDH Calculate Secret. */
#define TLS_ERR_BAD_HS_CLIENT_KEY_EXCHANGE_CS     -0x7D00
/**< Processing of the CertificateVerify handshake message failed. */
#define TLS_ERR_BAD_HS_CERTIFICATE_VERIFY         -0x7D80
/**< Processing of the ChangeCipherSpec handshake message failed. */
#define TLS_ERR_BAD_HS_CHANGE_CIPHER_SPEC         -0x7E00
/**< Processing of the Finished handshake message failed. */
#define TLS_ERR_BAD_HS_FINISHED                   -0x7E80
/**< Memory allocation failed */
#define TLS_ERR_ALLOC_FAILED                      -0x7F00
/**< Hardware acceleration function returned with error */
#define TLS_ERR_HW_ACCEL_FAILED                   -0x7F80
/**< Hardware acceleration function skipped / left alone data */
#define TLS_ERR_HW_ACCEL_FALLTHROUGH              -0x6F80
/**< Processing of the compression / decompression failed */
#define TLS_ERR_COMPRESSION_FAILED                -0x6F00
/**< Handshake protocol not within min/max boundaries */
#define TLS_ERR_BAD_HS_PROTOCOL_VERSION           -0x6E80
/**< Processing of the NewSessionTicket handshake message failed. */
#define TLS_ERR_BAD_HS_NEW_SESSION_TICKET         -0x6E00
/**< Session ticket has expired. */
#define TLS_ERR_SESSION_TICKET_EXPIRED            -0x6D80
/**< Public key type mismatch (eg, asked for RSA key exchange and presented EC key) */
#define TLS_ERR_PK_TYPE_MISMATCH                  -0x6D00
/**< Unknown identity received (eg, PSK identity) */
#define TLS_ERR_UNKNOWN_IDENTITY                  -0x6C80
/**< Internal error (eg, unexpected failure in lower-level module) */
#define TLS_ERR_INTERNAL_ERROR                    -0x6C00
/**< A counter would wrap (eg, too many messages exchanged). */
#define TLS_ERR_COUNTER_WRAPPING                  -0x6B80
/**< Unexpected message at ServerHello in renegotiation. */
#define TLS_ERR_WAITING_SERVER_HELLO_RENEGO       -0x6B00
/**< DTLS client must retry for hello verification */
#define TLS_ERR_HELLO_VERIFY_REQUIRED             -0x6A80
/**< A buffer is too small to receive or write a message */
#define TLS_ERR_BUFFER_TOO_SMALL                  -0x6A00
/**< None of the common ciphersuites is usable (eg, no suitable certificate, see debug messages). */
#define TLS_ERR_NO_USABLE_CIPHERSUITE             -0x6980
/**< No data of requested type currently available on underlying transport. */
#define TLS_ERR_WANT_READ                         -0x6900
/**< Connection requires a write call. */
#define TLS_ERR_WANT_WRITE                        -0x6880
/**< The operation timed out. */
#define TLS_ERR_TIMEOUT                           -0x6800
/**< The client initiated a reconnect from the same port. */
#define TLS_ERR_CLIENT_RECONNECT                  -0x6780
/**< Record header looks valid but is not expected. */
#define TLS_ERR_UNEXPECTED_RECORD                 -0x6700
/**< The alert message received indicates a non-fatal error. */
#define TLS_ERR_NON_FATAL                         -0x6680
/**< Couldn't set the hash for verifying CertificateVerify */
#define TLS_ERR_INVALID_VERIFY_HASH               -0x6600
/**< Internal-only message signaling that further message-processing should be done */
#define TLS_ERR_CONTINUE_PROCESSING               -0x6580
/**< The asynchronous operation is not completed yet. */
#define TLS_ERR_ASYNC_IN_PROGRESS                 -0x6500
/**< Internal-only message signaling that a message arrived early. */
#define TLS_ERR_EARLY_MESSAGE                     -0x6480
/**< A cryptographic operation is in progress. Try again later. */
#define TLS_ERR_CRYPTO_IN_PROGRESS                -0x7000

/********************************** TLS VERSION ***************************************/
#define TLS_MAJOR_VERSION_3             3u
#define TLS_MINOR_VERSION_0             0u
#define TLS_MINOR_VERSION_1             1u
#define TLS_MINOR_VERSION_2             2u
#define TLS_MINOR_VERSION_3             3u   /*!< TLS v1.2 */
#define TLS_MAX_MAJOR_VERSION           TLS_MAJOR_VERSION_3
#define TLS_MAX_MINOR_VERSION           TLS_MINOR_VERSION_3

/********************************* TLS PROTOCOL **************************************/
#define SSL_TRANSPORT_STREAM            0u   /*!< TLS      */
#define SSL_TRANSPORT_DATAGRAM          1u   /*!< DTLS     */

#endif/* (TCPIP_TCP_TLS_ENABLED == STD_ON) */

END_C_DECLS

#endif/*TCPIP_TLS_H_*/





