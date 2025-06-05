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
**  FILENAME    : TcpIp_TlsTypes.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : pengfei.zhu                                                 **
**  Vendor      : i-soft                                                      **
**  DESCRIPTION : Tls data type definition                                    **
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

#ifndef TCPIP_TLSTYPES_H_
#define TCPIP_TLSTYPES_H_


#include "Std_Types.h"
#include "TcpIp_Cfg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if (TCPIP_TCP_TLS_ENABLED == STD_ON)
/** Maximum length of any IV, in Bytes. */
#define TLS_MAX_IV_LENGTH      16u
/** Maximum block size of any cipher, in Bytes. */
#define TLS_MAX_BLOCK_LENGTH   16u

/* TLS state machine  */
typedef enum
{
    TLS_HELLO_REQUEST = 0u,
    TLS_CLIENT_HELLO,
    TLS_SERVER_HELLO,
    TLS_SERVER_CERTIFICATE,
    TLS_SERVER_KEY_EXCHANGE,
    TLS_CERTIFICATE_REQUEST,
    TLS_SERVER_HELLO_DONE,
    TLS_CLIENT_CERTIFICATE,
    TLS_CLIENT_KEY_EXCHANGE,
    TLS_CERTIFICATE_VERIFY,
    TLS_CLIENT_CHANGE_CIPHER_SPEC,
    TLS_CLIENT_FINISHED,
    TLS_SERVER_CHANGE_CIPHER_SPEC,
    TLS_SERVER_FINISHED,
    TLS_FLUSH_BUFFERS,
    TLS_HANDSHAKE_WRAPUP,
    TLS_HANDSHAKE_OVER,
    TLS_SERVER_NEW_SESSION_TICKET,
    TLS_SERVER_HELLO_VERIFY_REQUEST_SENT,
} TcpIp_TlsHandshakePhaseType;


typedef enum
{
    TLS_KEY_EXCHANGE_NONE = 0u,
    TLS_KEY_EXCHANGE_RSA,
    TLS_KEY_EXCHANGE_DHE_RSA,
    TLS_KEY_EXCHANGE_ECDHE_RSA,
    TLS_KEY_EXCHANGE_ECDHE_ECDSA,
    TLS_KEY_EXCHANGE_PSK,
    TLS_KEY_EXCHANGE_DHE_PSK,
    TLS_KEY_EXCHANGE_RSA_PSK,
    TLS_KEY_EXCHANGE_ECDHE_PSK,
    TLS_KEY_EXCHANGE_ECDH_RSA,
    TLS_KEY_EXCHANGE_ECDH_ECDSA,
    TLS_KEY_EXCHANGE_ECJPAKE,
} mbedtls_key_exchange_type_t;

typedef enum
{
    TLS_MD_NONE = 0u,  /**< None. */
    TLS_MD_MD2,       /**< The MD2 message digest. */
    TLS_MD_MD4,       /**< The MD4 message digest. */
    TLS_MD_MD5,       /**< The MD5 message digest. */
    TLS_MD_SHA1,      /**< The SHA-1 message digest. */
    TLS_MD_SHA224,    /**< The SHA-224 message digest. */
    TLS_MD_SHA256,    /**< The SHA-256 message digest. */
    TLS_MD_SHA384,    /**< The SHA-384 message digest. */
    TLS_MD_SHA512,    /**< The SHA-512 message digest. */
    TLS_MD_RIPEMD160, /**< The RIPEMD-160 message digest. */
} mbedtls_md_type_t;

/* Public key types */
typedef enum
{
    TLS_PK_NONE = 0u,
    TLS_PK_RSA,
    TLS_PK_ECKEY,
    TLS_PK_ECKEY_DH,
    TLS_PK_ECDSA,
    TLS_PK_RSA_ALT,
    TLS_PK_RSASSA_PSS,
} mbedtls_pk_type_t;

typedef enum
{
    TLS_CIPHER_NONE = 0u,             /**< Placeholder to mark the end of cipher-pair lists. */
    TLS_CIPHER_NULL,                 /**< The identity stream cipher. */
    TLS_CIPHER_AES_128_ECB,          /**< AES cipher with 128-bit ECB mode. */
    TLS_CIPHER_AES_192_ECB,          /**< AES cipher with 192-bit ECB mode. */
    TLS_CIPHER_AES_256_ECB,          /**< AES cipher with 256-bit ECB mode. */
    TLS_CIPHER_AES_128_CBC,          /**< AES cipher with 128-bit CBC mode. */
    TLS_CIPHER_AES_192_CBC,          /**< AES cipher with 192-bit CBC mode. */
    TLS_CIPHER_AES_256_CBC,          /**< AES cipher with 256-bit CBC mode. */
    TLS_CIPHER_AES_128_CFB128,       /**< AES cipher with 128-bit CFB128 mode. */
    TLS_CIPHER_AES_192_CFB128,       /**< AES cipher with 192-bit CFB128 mode. */
    TLS_CIPHER_AES_256_CFB128,       /**< AES cipher with 256-bit CFB128 mode. */
    TLS_CIPHER_AES_128_CTR,          /**< AES cipher with 128-bit CTR mode. */
    TLS_CIPHER_AES_192_CTR,          /**< AES cipher with 192-bit CTR mode. */
    TLS_CIPHER_AES_256_CTR,          /**< AES cipher with 256-bit CTR mode. */
    TLS_CIPHER_AES_128_GCM,          /**< AES cipher with 128-bit GCM mode. */
    TLS_CIPHER_AES_192_GCM,          /**< AES cipher with 192-bit GCM mode. */
    TLS_CIPHER_AES_256_GCM,          /**< AES cipher with 256-bit GCM mode. */
    TLS_CIPHER_CAMELLIA_128_ECB,     /**< Camellia cipher with 128-bit ECB mode. */
    TLS_CIPHER_CAMELLIA_192_ECB,     /**< Camellia cipher with 192-bit ECB mode. */
    TLS_CIPHER_CAMELLIA_256_ECB,     /**< Camellia cipher with 256-bit ECB mode. */
    TLS_CIPHER_CAMELLIA_128_CBC,     /**< Camellia cipher with 128-bit CBC mode. */
    TLS_CIPHER_CAMELLIA_192_CBC,     /**< Camellia cipher with 192-bit CBC mode. */
    TLS_CIPHER_CAMELLIA_256_CBC,     /**< Camellia cipher with 256-bit CBC mode. */
    TLS_CIPHER_CAMELLIA_128_CFB128,  /**< Camellia cipher with 128-bit CFB128 mode. */
    TLS_CIPHER_CAMELLIA_192_CFB128,  /**< Camellia cipher with 192-bit CFB128 mode. */
    TLS_CIPHER_CAMELLIA_256_CFB128,  /**< Camellia cipher with 256-bit CFB128 mode. */
    TLS_CIPHER_CAMELLIA_128_CTR,     /**< Camellia cipher with 128-bit CTR mode. */
    TLS_CIPHER_CAMELLIA_192_CTR,     /**< Camellia cipher with 192-bit CTR mode. */
    TLS_CIPHER_CAMELLIA_256_CTR,     /**< Camellia cipher with 256-bit CTR mode. */
    TLS_CIPHER_CAMELLIA_128_GCM,     /**< Camellia cipher with 128-bit GCM mode. */
    TLS_CIPHER_CAMELLIA_192_GCM,     /**< Camellia cipher with 192-bit GCM mode. */
    TLS_CIPHER_CAMELLIA_256_GCM,     /**< Camellia cipher with 256-bit GCM mode. */
    TLS_CIPHER_DES_ECB,              /**< DES cipher with ECB mode. */
    TLS_CIPHER_DES_CBC,              /**< DES cipher with CBC mode. */
    TLS_CIPHER_DES_EDE_ECB,          /**< DES cipher with EDE ECB mode. */
    TLS_CIPHER_DES_EDE_CBC,          /**< DES cipher with EDE CBC mode. */
    TLS_CIPHER_DES_EDE3_ECB,         /**< DES cipher with EDE3 ECB mode. */
    TLS_CIPHER_DES_EDE3_CBC,         /**< DES cipher with EDE3 CBC mode. */
    TLS_CIPHER_BLOWFISH_ECB,         /**< Blowfish cipher with ECB mode. */
    TLS_CIPHER_BLOWFISH_CBC,         /**< Blowfish cipher with CBC mode. */
    TLS_CIPHER_BLOWFISH_CFB64,       /**< Blowfish cipher with CFB64 mode. */
    TLS_CIPHER_BLOWFISH_CTR,         /**< Blowfish cipher with CTR mode. */
    TLS_CIPHER_ARC4_128,             /**< RC4 cipher with 128-bit mode. */
    TLS_CIPHER_AES_128_CCM,          /**< AES cipher with 128-bit CCM mode. */
    TLS_CIPHER_AES_192_CCM,          /**< AES cipher with 192-bit CCM mode. */
    TLS_CIPHER_AES_256_CCM,          /**< AES cipher with 256-bit CCM mode. */
    TLS_CIPHER_CAMELLIA_128_CCM,     /**< Camellia cipher with 128-bit CCM mode. */
    TLS_CIPHER_CAMELLIA_192_CCM,     /**< Camellia cipher with 192-bit CCM mode. */
    TLS_CIPHER_CAMELLIA_256_CCM,     /**< Camellia cipher with 256-bit CCM mode. */
    TLS_CIPHER_ARIA_128_ECB,         /**< Aria cipher with 128-bit key and ECB mode. */
    TLS_CIPHER_ARIA_192_ECB,         /**< Aria cipher with 192-bit key and ECB mode. */
    TLS_CIPHER_ARIA_256_ECB,         /**< Aria cipher with 256-bit key and ECB mode. */
    TLS_CIPHER_ARIA_128_CBC,         /**< Aria cipher with 128-bit key and CBC mode. */
    TLS_CIPHER_ARIA_192_CBC,         /**< Aria cipher with 192-bit key and CBC mode. */
    TLS_CIPHER_ARIA_256_CBC,         /**< Aria cipher with 256-bit key and CBC mode. */
    TLS_CIPHER_ARIA_128_CFB128,      /**< Aria cipher with 128-bit key and CFB-128 mode. */
    TLS_CIPHER_ARIA_192_CFB128,      /**< Aria cipher with 192-bit key and CFB-128 mode. */
    TLS_CIPHER_ARIA_256_CFB128,      /**< Aria cipher with 256-bit key and CFB-128 mode. */
    TLS_CIPHER_ARIA_128_CTR,         /**< Aria cipher with 128-bit key and CTR mode. */
    TLS_CIPHER_ARIA_192_CTR,         /**< Aria cipher with 192-bit key and CTR mode. */
    TLS_CIPHER_ARIA_256_CTR,         /**< Aria cipher with 256-bit key and CTR mode. */
    TLS_CIPHER_ARIA_128_GCM,         /**< Aria cipher with 128-bit key and GCM mode. */
    TLS_CIPHER_ARIA_192_GCM,         /**< Aria cipher with 192-bit key and GCM mode. */
    TLS_CIPHER_ARIA_256_GCM,         /**< Aria cipher with 256-bit key and GCM mode. */
    TLS_CIPHER_ARIA_128_CCM,         /**< Aria cipher with 128-bit key and CCM mode. */
    TLS_CIPHER_ARIA_192_CCM,         /**< Aria cipher with 192-bit key and CCM mode. */
    TLS_CIPHER_ARIA_256_CCM,         /**< Aria cipher with 256-bit key and CCM mode. */
    TLS_CIPHER_AES_128_OFB,          /**< AES 128-bit cipher in OFB mode. */
    TLS_CIPHER_AES_192_OFB,          /**< AES 192-bit cipher in OFB mode. */
    TLS_CIPHER_AES_256_OFB,          /**< AES 256-bit cipher in OFB mode. */
    TLS_CIPHER_AES_128_XTS,          /**< AES 128-bit cipher in XTS block mode. */
    TLS_CIPHER_AES_256_XTS,          /**< AES 256-bit cipher in XTS block mode. */
    TLS_CIPHER_CHACHA20,             /**< ChaCha20 stream cipher. */
    TLS_CIPHER_CHACHA20_POLY1305,    /**< ChaCha20-Poly1305 AEAD cipher. */
} mbedtls_cipher_type_t;

/** Supported cipher modes. */
typedef enum
{
    TLS_MODE_NONE = 0u,               /**< None. */
    TLS_MODE_ECB,                    /**< The ECB cipher mode. */
    TLS_MODE_CBC,                    /**< The CBC cipher mode. */
    TLS_MODE_CFB,                    /**< The CFB cipher mode. */
    TLS_MODE_OFB,                    /**< The OFB cipher mode. */
    TLS_MODE_CTR,                    /**< The CTR cipher mode. */
    TLS_MODE_GCM,                    /**< The GCM cipher mode. */
    TLS_MODE_STREAM,                 /**< The stream cipher mode. */
    TLS_MODE_CCM,                    /**< The CCM cipher mode. */
    TLS_MODE_XTS,                    /**< The XTS cipher mode. */
    TLS_MODE_CHACHAPOLY,             /**< The ChaCha-Poly cipher mode. */
} mbedtls_cipher_mode_t;

typedef struct mbedtls_x509_crt_profile
{
    uint32 allowed_mds;       /**< MDs for signatures         */
    uint32 allowed_pks;       /**< PK algs for signatures     */
    uint32 allowed_curves;    /**< Elliptic curves for ECDSA  */
    uint32 rsa_min_bitlen;    /**< Minimum size for RSA keys  */
}
mbedtls_x509_crt_profile;

/**
 * Type-length-value structure that allows for ASN1 using DER.
 */
typedef struct mbedtls_asn1_buf
{
    int tag;                /**< ASN1 type, e.g. TLS_ASN1_UTF8_STRING. */
    uint32 len;             /**< ASN1 length, in octets. */
    uint8* p;       /**< ASN1 data, e.g. in ASCII. */
}
mbedtls_asn1_buf;

/**
 * Container for a sequence or list of 'named' ASN.1 data items
 */
typedef struct mbedtls_asn1_named_data
{
    mbedtls_asn1_buf oid;                   /**< The object identifier. */
    mbedtls_asn1_buf val;                   /**< The named value. */
    struct mbedtls_asn1_named_data* next;  /**< The next entry in the sequence. */
    uint8 next_merged;      /**< Merge next item into the current one? */
}
mbedtls_asn1_named_data;

/** Container for date and time (precision in seconds). */
typedef struct mbedtls_x509_time
{
    int year, mon, day;         /**< Date. */
    int hour, min, sec;         /**< Time. */
}
mbedtls_x509_time;

/**
 * \brief           Public key container
 */
typedef struct mbedtls_pk_context
{
    const uint8*    pk_info; /**< Public key information         */
    void*           pk_ctx;  /**< Underlying public key context  */
} mbedtls_pk_context;

/**
 * Container for a sequence of ASN.1 items
 */
typedef struct mbedtls_asn1_sequence
{
    mbedtls_asn1_buf buf;                   /**< Buffer containing the given ASN.1 item. */
    struct mbedtls_asn1_sequence* next;    /**< The next entry in the sequence. */
}
mbedtls_asn1_sequence;

/**
 * Cipher information. Allows calling cipher functions
 * in a generic way.
 */
typedef struct mbedtls_cipher_info_t
{
    /** Full cipher identifier. For example,
     * TLS_CIPHER_AES_256_CBC.
     */
    mbedtls_cipher_type_t type;

    /** The cipher mode. For example, TLS_MODE_CBC. */
    mbedtls_cipher_mode_t mode;

    /** The cipher key length, in bits. This is the
     * default length for variable sized ciphers.
     * Includes parity bits for ciphers like DES.
     */
    unsigned int key_bitlen;

    /** Name of the cipher. */
    const char* name;

    /** IV or nonce size, in Bytes.
     * For ciphers that accept variable IV sizes,
     * this is the recommended size.
     */
    unsigned int iv_size;

    /** Bitflag comprised of TLS_CIPHER_VARIABLE_IV_LEN and
     *  TLS_CIPHER_VARIABLE_KEY_LEN indicating whether the
     *  cipher supports variable IV or variable key sizes, respectively.
     */
    int flags;

    /** The block size, in Bytes. */
    unsigned int block_size;

    /** Struct for base cipher information and functions. */
    const uint8* base;

} mbedtls_cipher_info_t;

typedef struct mbedtls_ssl_sig_hash_set_t mbedtls_ssl_sig_hash_set_t;
typedef struct mbedtls_ssl_key_cert mbedtls_ssl_key_cert;
typedef mbedtls_asn1_sequence mbedtls_x509_sequence;
typedef mbedtls_asn1_named_data mbedtls_x509_name;
typedef mbedtls_asn1_buf mbedtls_x509_buf;
typedef struct mbedtls_ssl_session mbedtls_ssl_session;
typedef struct mbedtls_ssl_config  mbedtls_ssl_config;
typedef struct mbedtls_ssl_transform mbedtls_ssl_transform;
typedef struct mbedtls_ssl_handshake_params mbedtls_ssl_handshake_params;
typedef struct mbedtls_ssl_ciphersuite_t mbedtls_ssl_ciphersuite_t;
typedef struct mbedtls_md_info_t mbedtls_md_info_t;

typedef int mbedtls_ssl_send_t(void* ctx, const uint8* buf, uint32 len);
typedef int mbedtls_ssl_recv_t(void* ctx, uint8* buf, uint32 len);
typedef int mbedtls_ssl_recv_timeout_t(void* ctx, uint8* buf, uint32 len, uint32 timeout);
typedef void mbedtls_ssl_set_timer_t(void* ctx, uint32 int_ms, uint32 fin_ms);
typedef int mbedtls_ssl_get_timer_t(void* ctx);


/* Container for an X.509 certificate. The certificate may be chained. */
typedef struct mbedtls_x509_crt
{
    mbedtls_x509_buf raw;               /**< The raw certificate data (DER). */
    mbedtls_x509_buf tbs;               /**< The raw certificate body (DER). The part that is To Be Signed. */

    int version;                        /**< The X.509 version. (1=v1, 2=v2, 3=v3) */
    mbedtls_x509_buf serial;            /**< Unique id for certificate issued by a specific CA. */
    mbedtls_x509_buf sig_oid;           /**< Signature algorithm, e.g. sha1RSA */

    mbedtls_x509_buf issuer_raw;        /**< The raw issuer data (DER). Used for quick comparison. */
    mbedtls_x509_buf subject_raw;       /**< The raw subject data (DER). Used for quick comparison. */

    mbedtls_x509_name issuer;           /**< The parsed issuer data (named information object). */
    mbedtls_x509_name subject;          /**< The parsed subject data (named information object). */

    mbedtls_x509_time valid_from;       /**< Start time of certificate validity. */
    mbedtls_x509_time valid_to;         /**< End time of certificate validity. */

    mbedtls_pk_context pk;              /**< Container for the public key context. */

    mbedtls_x509_buf issuer_id;         /**< Optional X.509 v2/v3 issuer unique identifier. */
    mbedtls_x509_buf subject_id;        /**< Optional X.509 v2/v3 subject unique identifier. */
    mbedtls_x509_buf v3_ext;            /**< Optional X.509 v3 extensions.  */
    mbedtls_x509_sequence subject_alt_names; /**< Optional list of Subject Alternative Names (Only dNSName supported).*/

    int ext_types;                      /**< Bit string containing detected and parsed extensions */
    /**< Optional Basic Constraint extension value: 1 if this certificate belongs to a CA, 0 otherwise. */
    int ca_istrue;
    /**< Optional Basic Constraint extension value: The maximum path length to the root certificate.
     * Path length is 1 higher than RFC 5280 'meaning', so 1+ */
    int max_pathlen;

    unsigned int key_usage;             /**< Optional key usage extension value: See the values in x509.h */

    mbedtls_x509_sequence ext_key_usage;/**< Optional list of extended key usage OIDs. */

    uint8 ns_cert_type;         /**< Optional Netscape certificate type extension value: See the values in x509.h */

    mbedtls_x509_buf sig;               /**< Signature: hash of the tbs part signed with the private key. */
    mbedtls_md_type_t
    sig_md;           /**< Internal representation of the MD algorithm of the signature algorithm, e.g. TLS_MD_SHA256 */
    mbedtls_pk_type_t
    sig_pk;      /**< Internal representation of the Public Key algorithm of the signature algorithm, e.g. TLS_PK_RSA */
    void* sig_opts;               /**< Signature options to be passed to mbedtls_pk_verify_ext(), e.g. for RSASSA-PSS */

    struct mbedtls_x509_crt* next;      /**< Next certificate in the CA-chain. */
} mbedtls_x509_crt;


/* The generic message-digest context. */
typedef struct mbedtls_md_context_t
{
    const mbedtls_md_info_t* md_info;           /* Information about the associated message digest. */
    void* md_ctx;                               /* The digest-specific context. */
    void* hmac_ctx;                             /* The HMAC part of the context. */
} mbedtls_md_context_t;

typedef struct
{
    const mbedtls_ssl_config* conf;             /*!< configuration information*/

    /* Miscellaneous */
    TcpIp_TlsHandshakePhaseType state;                      /*!< SSL handshake: current state*/
    uint16 major_ver;                           /*!< equal to  TLS_MAJOR_VERSION_3*/
    uint16 minor_ver;                           /*!< either 0 (SSL3) or 1 (TLS1.0)*/
    mbedtls_ssl_send_t* f_send;                 /*!< Callback for network send */
    mbedtls_ssl_recv_t* f_recv;                 /*!< Callback for network receive */
    mbedtls_ssl_recv_timeout_t* f_recv_timeout; /*!< Callback for network receive with timeout */
    void* p_bio;                                /*!< context for I/O operations*/

    /* Session layer */
    mbedtls_ssl_session* session_in;            /*!<  current session data (in)   */
    mbedtls_ssl_session* session_out;           /*!<  current session data (out)  */
    mbedtls_ssl_session* session;               /*!<  negotiated session data     */
    mbedtls_ssl_session* session_negotiate;     /*!<  session data in negotiation */
    mbedtls_ssl_handshake_params* handshake;    /*!<  params required only during the handshake process*/

    /* Record layer transformations */
    mbedtls_ssl_transform* transform_in;        /*!<  current transform params (in)   */
    mbedtls_ssl_transform* transform_out;       /*!<  current transform params (in)   */
    mbedtls_ssl_transform* transform;           /*!<  negotiated transform params     */
    mbedtls_ssl_transform* transform_negotiate; /*!<  transform params in negotiation */

    /* Timers */
    void* p_timer;                              /*!< context for the timer callbacks */
    mbedtls_ssl_set_timer_t* f_set_timer;       /*!< set timer callback */
    mbedtls_ssl_get_timer_t* f_get_timer;       /*!< get timer callback */

    /* Record layer (incoming data) */
    uint8* in_buf;                  /*!< input buffer                     */
    uint8* in_ctr;                  /*!< 64-bit incoming message counter TLS: maintained by us */
    uint8* in_hdr;                  /*!< start of record header           */
    uint8* in_len;                  /*!< two-bytes message length field   */
    uint8* in_iv;                   /*!< ivlen-byte IV                    */
    uint8* in_msg;                  /*!< message contents (in_iv+ivlen)   */
    uint8* in_offt;                 /*!< read offset in application data  */
    uint32 in_msgtype;                  /*!< record header: message type      */
    uint32 in_msglen;               /*!< record header: message length    */
    uint32 in_left;                 /*!< amount of data read so far       */
    uint32 in_hslen;                /*!< current handshake message length, including the handshake header */
    int nb_zero;                    /*!< # of 0-length encrypted messages */
    int keep_current_message;       /*!< drop or reuse current message on next call to record layer? */

    /* Record layer (outgoing data) */
    uint8* out_buf;                 /*!< output buffer                    */
    uint8* out_ctr;                 /*!< 64-bit outgoing message counter  */
    uint8* out_hdr;                 /*!< start of record header           */
    uint8* out_len;                 /*!< two-bytes message length field   */
    uint8* out_iv;                  /*!< ivlen-byte IV                    */
    uint8* out_msg;                 /*!< message contents (out_iv+ivlen)  */
    uint32 out_msgtype;                 /*!< record header: message type      */
    uint32 out_msglen;              /*!< record header: message length    */
    uint32 out_left;                /*!< amount of data not yet written   */
    uint8 cur_out_ctr[8];           /*!<  Outgoing record sequence number. */

    /* PKI layer */
    uint32 client_auth;                     /*!<  flag for client auth.   */

    /* User settings */
#if(STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)
    const uint8* hostname;                    /*!< expected peer CN for verification (and SNI if available) */
#endif /* TLS_X509_CRT_PARSE_C */

    /* Secure renegotiation */
    /* needed to know when to send extension on server */
    int secure_renegotiation;           /*!<  does peer support legacy or secure renegotiation */

} mbedtls_ssl_context;

/*
 * List of certificate + private key pairs
 */
struct mbedtls_ssl_key_cert
{
    mbedtls_x509_crt* cert;                 /*!< cert                       */
    mbedtls_pk_context* key;                /*!< private key                */
    mbedtls_ssl_key_cert* next;             /*!< next key/cert pair         */
};

/**
 * Certificate revocation list entry.
 * Contains the CA-specific serial numbers and revocation dates.
 */
typedef struct mbedtls_x509_crl_entry
{
    mbedtls_x509_buf raw;

    mbedtls_x509_buf serial;

    mbedtls_x509_time revocation_date;

    mbedtls_x509_buf entry_ext;

    struct mbedtls_x509_crl_entry* next;
}
mbedtls_x509_crl_entry;

/**
 * Certificate revocation list structure.
 * Every CRL may have multiple entries.
 */
typedef struct mbedtls_x509_crl
{
    mbedtls_x509_buf raw;           /**< The raw certificate data (DER). */
    mbedtls_x509_buf tbs;           /**< The raw certificate body (DER). The part that is To Be Signed. */

    int version;                    /**< CRL version (1=v1, 2=v2) */
    mbedtls_x509_buf sig_oid;       /**< CRL signature type identifier */

    mbedtls_x509_buf issuer_raw;    /**< The raw issuer data (DER). */

    mbedtls_x509_name issuer;       /**< The parsed issuer data (named information object). */

    mbedtls_x509_time this_update;
    mbedtls_x509_time next_update;

    mbedtls_x509_crl_entry entry;   /**< The CRL entries containing the certificate revocation times for this CA. */

    mbedtls_x509_buf crl_ext;

    mbedtls_x509_buf sig_oid2;
    mbedtls_x509_buf sig;
    mbedtls_md_type_t
    sig_md;           /**< Internal representation of the MD algorithm of the signature algorithm, e.g. TLS_MD_SHA256 */
    mbedtls_pk_type_t
    sig_pk;           /**< Internal representation of the Public Key algorithm of the signature algorithm, e.g. TLS_PK_RSA */
    void* sig_opts;                     /**< Signature options to be passed to mbedtls_pk_verify_ext(), e.g. for RSASSA-PSS */

    struct mbedtls_x509_crl* next;
} mbedtls_x509_crl;

/* TLS configuration to be shared between mbedtls_ssl_context structures. */
struct mbedtls_ssl_config
{
    /* Group items by size (largest first) to minimize padding overhead */

    /* Pointers */
    const int* ciphersuite_list[4];                                          /*!< allowed ciphersuites per version   */
    void (*f_dbg)(void*, int, const char*, int, const char*);            /* Callback for printing debug output */
    void* p_dbg;                                                             /*!< context for the debug function */
    int (*f_rng)(void*, uint8*, uint32);                                 /* Callback for getting (pseudo-)random numbers*/
    void* p_rng;                                                             /*!< context for the RNG function */
    int (*f_get_cache)(void*, mbedtls_ssl_session*);                     /* Callback to retrieve a session from the cache*/
    int (*f_set_cache)(void*, const mbedtls_ssl_session*);                   /* Callback to store a session into the cache*/
    void* p_cache;                                                           /*!< context for cache callbacks        */

#if(STD_ON == TCPIP_TLS_SERVER_NAME_INDICATION)
    int (*f_sni)(void*, mbedtls_ssl_context*, const uint8*,
                 uint32);     /* Callback for setting cert according to SNI extension*/
    void* p_sni;                                                             /*!< context for SNI callback */
#endif

    int (*f_vrfy)(void*, mbedtls_x509_crt*, int,
                  uint32*);               /* Callback to customize X.509 certificate chain verification*/
    void* p_vrfy;                                                            /*!< context for X.509 verify calllback */

#if defined(TLS_EXPORT_KEYS)
    /** Callback to export key block and master secret */
    int (*f_export_keys)(void*, const uint8*,
                         const uint8*, uint32, uint32, uint32);
    void* p_export_keys;            /*!< context for key export callback */
#endif

    const mbedtls_x509_crt_profile* cert_profile; /*!< verification profile */
    mbedtls_ssl_key_cert* key_cert;               /*!< own certificate/key pair(s) */
    mbedtls_x509_crt* ca_chain;                   /*!< trusted CAs */
    mbedtls_x509_crl* ca_crl;                     /*!< trusted CAs CRLs */
    const int* sig_hashes;                        /*!< allowed signature hashes */

    /* Numerical settings (int then char) */
    uint32 read_timeout;    /*!< timeout for TcpIp_TlsReadData (ms)  */

    uint8 max_major_ver;    /*!< max. major version used */
    uint8 max_minor_ver;    /*!< max. minor version used */
    uint8 min_major_ver;    /*!< min. major version used */
    uint8 min_minor_ver;    /*!< min. minor version used */

    /* Flags (bitfields) */
    int endpoint : 1;      /*!< 0: client, 1: server               */
    int transport : 1;     /*!< stream (TLS) or datagram (DTLS).No DTLS in AutoSAR  */
    int authmode : 2;      /*!< TLS_VERIFY_XXX             */
    /* needed even with renego disabled for LEGACY_BREAK_HANDSHAKE */
    int allow_legacy_renegotiation : 2 ; /*!< TLS_LEGACY_XXX   */

#if(STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)
    int mfl_code : 3;      /*!< desired fragment length            */
#endif
#if(STD_ON == TLS_ENCRYPT_THEN_MAC)
    int encrypt_then_mac : 1 ; /*!< negotiate encrypt-then-mac?    */
#endif

    int cert_req_ca_list : 1;  /*!< enable sending CA list in Certificate Request messages? */
    const uint8* hostname;
};

/* This structure is used for storing current session data. */
struct mbedtls_ssl_session
{
    int ciphersuite;            /*!< chosen ciphersuite */
    uint32 compression;            /*!< chosen compression */
    uint32 id_len;                 /*!< session id length  */
    uint8 id[32];                  /*!< session identifier */
    uint8 master[48];              /*!< the master secret  */
    mbedtls_x509_crt* peer_cert;   /*!< peer X.509 cert chain */
    uint32 verify_result;          /*!< verification result */

#if(STD_ON == TLS_MAX_FRAGMENT_LENGTH_ENABLED)
    uint8 mfl_code;     /*!< MaxFragmentLength negotiated by peer */
#endif /* TLS_MAX_FRAGMENT_LENGTH_ENABLED */

#if(STD_ON == TLS_ENCRYPT_THEN_MAC)
    uint32 encrypt_then_mac;       /*!< flag for EtM activation */
#endif
};

/* Generic cipher context. */
typedef struct mbedtls_cipher_context_t
{
    /** Information about the associated cipher. */
    const mbedtls_cipher_info_t* cipher_info;

    /** Key length to use. */
    int key_bitlen;

    /** Operation that the key of the context has been
     * initialized for.
     */
    uint8 operation;

    /** Buffer for input that has not been processed yet. */
    uint8 unprocessed_data[TLS_MAX_BLOCK_LENGTH];

    /** Number of Bytes that have not been processed yet. */
    uint32 unprocessed_len;

    /** Current IV or NONCE_COUNTER for CTR-mode, data unit (or sector) number
     * for XTS-mode. */
    uint8 iv[TLS_MAX_IV_LENGTH];

    /** IV size in Bytes, for ciphers with variable-length IVs. */
    uint32 iv_size;

    /** The cipher-specific context. */
    void* cipher_ctx;

} mbedtls_cipher_context_t;


/* This structure contains a full set of runtime transform parameters
 * either in negotiation or active. */
struct mbedtls_ssl_transform
{
    /* Session specific crypto layer */
    const mbedtls_ssl_ciphersuite_t* ciphersuite_info;      /*!<  Chosen cipersuite_info  */
    int keylen;                                             /*!<  symmetric key length (bytes)*/
    uint32 minlen;                                          /*!<  min. ciphertext length  */
    uint32 ivlen;                                           /*!<  IV length               */
    uint32 fixed_ivlen;                                     /*!<  Fixed part of IV (AEAD) */
    uint32 maclen;                                          /*!<  MAC length              */
    uint8 iv_enc[16];                                       /*!<  IV (encryption)         */
    uint8 iv_dec[16];                                       /*!<  IV (decryption)         */
    mbedtls_md_context_t md_ctx_enc;                        /*!<  MAC (encryption)        */
    mbedtls_md_context_t md_ctx_dec;                        /*!<  MAC (decryption)        */
    mbedtls_cipher_context_t cipher_ctx_enc;                /*!<  encryption context      */
    mbedtls_cipher_context_t cipher_ctx_dec;                /*!<  decryption context      */
};

typedef struct mbedtls_sha256_context
{
    uint32 total[2];          /*!< The number of Bytes processed.  */
    uint32 state[8];          /*!< The intermediate digest state.  */
    uint8 buffer[64];           /*!< The data block being processed. */
    int is224;                  /*!< Determines which function to use:
                                     0: Use SHA-256, or 1: Use SHA-224. */
}
mbedtls_sha256_context;

/*
 * Abstraction for a grid of allowed signature-hash-algorithm pairs.
 */
struct mbedtls_ssl_sig_hash_set_t
{
    /* At the moment, we only need to remember a single suitable
     * hash algorithm per signature algorithm. As long as that's
     * the case - and we don't need a general lookup function -
     * we can implement the sig-hash-set as a map from signatures
     * to hash algorithms. */
    mbedtls_md_type_t rsa;
    mbedtls_md_type_t ecdsa;
};

/* This structure contains the parameters only needed during handshake. */
struct mbedtls_ssl_handshake_params
{
    /* Handshake specific crypto variables */
    mbedtls_ssl_sig_hash_set_t hash_algs;             /*!<  Set of suitable sig-hash pairs */
    mbedtls_ssl_key_cert* key_cert;                 /*!< chosen key/cert pair (server)  */
#if defined(TCPIP_TLS_SERVER_NAME_INDICATION)
    uint32 sni_authmode;                            /*!< authmode from SNI callback     */
    mbedtls_ssl_key_cert* sni_key_cert;             /*!< key/cert list from SNI         */
    mbedtls_x509_crt* sni_ca_chain;                 /*!< trusted CAs from SNI callback  */
    mbedtls_x509_crl* sni_ca_crl;                   /*!< trusted CAs CRLs from SNI      */
#endif /* TCPIP_TLS_SERVER_NAME_INDICATION */

    /* Checksum contexts */
    mbedtls_sha256_context fin_sha256;  /* TLS V1.2 */
    void (*update_checksum)(mbedtls_ssl_context*, const uint8*, uint32);
    void (*calc_verify)(mbedtls_ssl_context*, uint8*);
    void (*calc_finished)(mbedtls_ssl_context*, uint8*, int);
    int (*tls_prf)(const uint8*, uint32, const char*, const uint8*, uint32, uint8*, uint32);
    uint32 pmslen;                         /*!< premaster length */
    uint8 randbytes[64u];                  /*!< random bytes */
    uint8 premaster[48u];                  /*!< premaster secret */
    int resume;                         /*!< session resume indicator*/
    uint32 max_major_ver;                  /*!< max. major version client*/
    uint32 max_minor_ver;                  /*!< max. minor version client*/
    int cli_exts;                       /*!< client extension presence*/
};

/* This structure is used for storing ciphersuite information */
struct mbedtls_ssl_ciphersuite_t
{
    int id;
    const uint8* name;
    mbedtls_cipher_type_t cipher;
    mbedtls_md_type_t mac;
    mbedtls_key_exchange_type_t key_exchange;
    int min_major_ver;
    int min_minor_ver;
    int max_major_ver;
    int max_minor_ver;
    uint8 flags;
};

#endif/* (TCPIP_TCP_TLS_ENABLED == STD_ON) */





#endif /* TCPIP_TLSTYPES_H_ */
