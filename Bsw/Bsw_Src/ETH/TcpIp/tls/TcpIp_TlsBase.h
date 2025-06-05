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
**  FILENAME    : TcpIp_TlsBase.h                                                                                     **
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
#ifndef TCPIP_TLSBASE_H_
#define TCPIP_TLSBASE_H_

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

#include "TcpIp_TlsTypes.h"

#define ALTCP_MBEDTLS_FLAGS_HANDSHAKE_DONE    (uint8)0x01
#define ALTCP_MBEDTLS_FLAGS_UPPER_CALLED      (uint8)0x02
#define ALTCP_MBEDTLS_FLAGS_RX_CLOSE_QUEUED   (uint8)0x04
#define ALTCP_MBEDTLS_FLAGS_RX_CLOSED         (uint8)0x08
#define ALTCP_MBEDTLS_FLAGS_APPLDATA_SENT     (uint8)0x10

#define TLS_SSL_MAX_CONTENT_LEN               2000
#define TLS_SSL_IS_CLIENT                        0
#define TLS_SSL_IS_SERVER                        1
#define TLS_SSL_TRANSPORT_STREAM                 0
#define TLS_SSL_PRESET_DEFAULT                   0
#define TLS_SSL_VERIFY_OPTIONAL                  1



struct altcp_tls_config
{
    struct mbedtls_ssl_config conf;
    mbedtls_x509_crt* cert;
    mbedtls_pk_context* pkey;
    mbedtls_x509_crt* ca;
#if defined(MBEDTLS_SSL_CACHE_C) && ALTCP_MBEDTLS_SESSION_CACHE_TIMEOUT_SECONDS
    /** Inter-connection cache for fast connection startup */
    struct mbedtls_ssl_cache_context cache;
#endif
};

typedef struct TcpIp_TlsSocketDataTag
{
    mbedtls_ssl_context ssl_context;
    /* chain of rx pbufs (before decryption) */
    struct pbuf* rx;
    struct pbuf* rx_app;
    uint8 flags;
    int rx_passed_unrecved;
    int bio_bytes_read;
    int bio_bytes_appl;
    struct TcpIp_TlsSocketDataTag* next;
    P2CONST(TcpIp_TlsConnectionConfigType, TYPEDEF, TCPIP_APPL_DATA) tlsCnnCfgPtr;
} TcpIp_TlsSocketDataType;

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
***********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TlsCsmRandomGenerate(
    uint8* resultPtr,
    uint32* resultLengthPtr);

#endif/* (STD_ON == TCPIP_TCP_TLS_ENABLED) */

#endif/* TCPIP_TLSBASE_H_ */
