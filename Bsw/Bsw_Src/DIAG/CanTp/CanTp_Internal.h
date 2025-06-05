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
**  FILENAME    : CanTp_Internal.h                                           **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Internal header file of CanTp module.                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */

#ifndef CANTP_INTERNAL_H
#define CANTP_INTERNAL_H

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Cfg.h"
#include "CanTp_Types.h"
#include "Det.h"
#include "PduR_CanTp.h"
#include "CanIf.h"
#include "SchM_CanTp.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/
#define CANTP_MODULE_ID                         35u
#define CANTP_VENDOR_ID                         62u
#define CANTP_INSTANCE_ID                       0x0u

#define CANTP_CAN20_FRAME_LEN_MAX (PduLengthType)8
#define CANTP_CANFD_FRAME_LEN_MAX (PduLengthType)64

/*****************************Development error values ***********************/
#define CANTP_E_PARAM_CONFIG                 (0x01u)
#define CANTP_E_PARAM_ID                     (0x02u)
#define CANTP_E_PARAM_POINTER                (0x03u)
#define CANTP_E_INIT_FAILED             	 (0x04u)
#define CANTP_E_UNINIT		                 (0x20u)
#define CANTP_E_INVALID_TX_ID                (0x30u)
#define CANTP_E_INVALID_RX_ID                (0x40u)
/*****************************Runtime error values ***********************/
#define CANTP_E_PADDING                      (0x70u)
#define CANTP_E_INVALID_TATYPE               (0x90u)
#define CANTP_E_OPER_NOT_SUPPORTED           (0xA0u)
#define CANTP_E_COM                          (0xB0u)
#define CANTP_E_RX_COM                       (0xC0u)
#define CANTP_E_TX_COM                       (0xD0u)

/*******************************CanTp Module API ID*****************************/
#define CANTP_SERVICEID_RXINDICATION          (0x42u)
#define CANTP_SERVICEID_TXCONFIRMATION        (0x40u)
#define CANTP_SERVICEID_MAINFUNCTION          (0x06u)
#define CANTP_SERVICEID_TRANSMIT              (0x03u)
#define CANTP_SERVICEID_INIT                  (0x01u)
#define CANTP_SERVICEID_CANCELTRANSMITREQUEST (0x08u)
#define CANTP_SERVICEID_CANCELRECEIVEREQUEST  (0x09u)
#define CANTP_SERVICEID_CHANGEPARAMETER       (0x0Au)
#define CANTP_SERVICEID_READPARAMETER         (0x0Bu)
#define CANTP_SERVICEID_GETVERSIONINFO        (0x07u)


#define CANTP_DET_REPORTRUNTIMEERROR(ApiId,ErrorId)     (\
                                           (void)Det_ReportRuntimeError(\
                                                                 CANTP_MODULE_ID,\
                                                                 CANTP_INSTANCE_ID,\
                                                                 (ApiId),\
                                                                 (ErrorId)))
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
#define CANTP_DET_REPORTERROR(ApiId,ErrorId)     (\
                                           (void)Det_ReportError(\
                                        		                 CANTP_MODULE_ID,\
																 CANTP_INSTANCE_ID,\
                                                                 (ApiId),\
                                                                 (ErrorId)))
#endif/* STD_ON == CANTP_DEV_ERROR_DETECT */

/*type of frame types*/
typedef uint8 CanTp_FramePCIType;

#define CANTP_FTYPE_MASK (uint8)0xF0

#define CANTP_FTYPE_SF 0x00u

#define CANTP_FTYPE_FF (uint8)0x10

#define CANTP_FTYPE_CF (uint8)0x20

#define CANTP_FTYPE_FC (uint8)0x30

#define CANTP_FTYPE_RESEVED (uint8)0xF0

/*mask of the SF_DL*/
#define CANTP_SF_DL_MASK (uint8)0x0F

/*mask of the upper nibble for FF DL*/
#define CANTP_FF_DL_MASK (uint8)0x0F

/*bit offset of the length information in the low part of first PCI byte*/
#define CANTP_OFFSET_BYTE_PART 8u

/*Offset of FF_DL low nibble for standard addressing*/
#define CANTP_STD_FF_FFDL_LOW_OFFSET_LT4095 1u
#define CANTP_EX_FF_FFDL_LOW_OFFSET_LT4095  2u
#define CANTP_NF_FF_FFDL_LOW_OFFSET_LT4095 1u
#define CANTP_MIX11_FF_FFDL_LOW_OFFSET_LT4095 2u
#define CANTP_MIX29_FF_FFDL_LOW_OFFSET_LT4095 2u

#define CANTP_STD_FF_FFDL_LOW_OFFSET_BT4095 5u
#define CANTP_EX_FF_FFDL_LOW_OFFSET_BT4095  6u
#define CANTP_NF_FF_FFDL_LOW_OFFSET_BT4095 5u
#define CANTP_11MIX_FF_FFDL_LOW_OFFSET_BT4095 6u
#define CANTP_29MIX_FF_FFDL_LOW_OFFSET_BT4095 6u

/*mask of the SN for CF*/
#define CANTP_CF_SN_MASK (uint8)0x0F

/*mask of the FS for FC*/
#define CANTP_FC_FS_MASK (uint8)0x0F

/*CONST values for FS of FC*/
/*continue to send*/
#define CANTP_FC_FS_CTS  0x00u

/*FC_WAIT*/
#define CANTP_FC_FS_WT (uint8)0x01

/*overflow*/
#define CANTP_FC_FS_OVFLW (uint8)0x02

/*offset of FC_BS for standard addressing*/
#define CANTP_FC_OFFSET_BS_STD  1u

/*offset of FC_BS for extended addressing*/
#define CANTP_FC_OFFSET_BS_EX  2u

/*offset of FC_BS for normal fixed addressing*/
#define CANTP_FC_OFFSET_BS_NF  1u

/*offset of FC_BS for mix11 addressing*/
#define CANTP_FC_OFFSET_BS_MIX11  2u

/*offset of FC_BS for mix29 addressing*/
#define CANTP_FC_OFFSET_BS_MIX29  2u

/*FC STmin limits*/
/*ms upper limit*/
#define CANTP_FC_STMIN_MS_LIMIT (uint8)0x7F

/*us lower limit*/
#define CANTP_FC_STMIN_US_LOW (uint8)0xF1

/*us upper limit*/
#define CANTP_FC_STMIN_US_UPPER (uint8)0xF9

/*unit for STmin from 0-7f in us*/
#define CANTP_FC_STMIN_MS_UNIT (uint32)1000

/*unit for STmin from F1-F9 in us*/
#define CANTP_FC_STMIN_US_UNIT (uint32)100

/*offset of FC_STMin for standard addressing*/
#define CANTP_FC_OFFSET_STMIN_STD  2u

/*offset of FC_STmin for extended addressing*/
#define CANTP_FC_OFFSET_STMIN_EX  3u

/*offset of FC_STmin for normal fixed addressing*/
#define CANTP_FC_OFFSET_STMIN_NF  2u

/*offset of FC_STmin for mix11 addressing*/
#define CANTP_FC_OFFSET_STMIN_MIX11  3u

/*offset of FC_STmin for mix29 addressing*/
#define CANTP_FC_OFFSET_STMIN_MIX29  3u

/*frame length of FC for standard addressing*/
#define CANTP_FC_LEN_STD  (PduLengthType)0x03

/*frame length of FC for extended addressing*/
#define CANTP_FC_LEN_EX  (PduLengthType)0x04

/*frame length of FC for normal fixed addressing*/
#define CANTP_FC_LEN_NF  (PduLengthType)0x03

/*frame length of FC for mix11 addressing*/
#define CANTP_FC_LEN_MIX11  (PduLengthType)0x04

/*frame length of FC for mix29 addressing*/
#define CANTP_FC_LEN_MIX29  (PduLengthType)0x04

/*FFDL have 12 bits, its data length*/
#define CANTP_FF_DL_12bit_LENGTH (PduLengthType)4095

/*maximum length of single frame data, if the addressing format is standard*/
#define CANTP_SF_LEN_MAX_STD (PduLengthType)0x07

/*maximum length of single frame data, if the addressing format is extended*/
#define CANTP_SF_LEN_MAX_EX (PduLengthType)0x06

/*maximum length of single frame data, if the addressing format is normal fixed*/
#define CANTP_SF_LEN_MAX_NF (PduLengthType)0x07

/*maximum length of single frame data, if the addressing format is mixed11*/
#define CANTP_SF_LEN_MAX_MIX11 (PduLengthType)0x06

/*maximum length of single frame data, if the addressing format is mixed29*/
#define CANTP_SF_LEN_MAX_MIX29 (PduLengthType)0x06

/*maximum length of the CF data for standard SDU*/
#define CANTP_CF_DATALEN_STD (PduLengthType)0x07

/*maximum length of the CF data for extended SDU*/
#define CANTP_CF_DATALEN_EX (PduLengthType)0x06

#define CANTP_CF_DATALEN_NF (PduLengthType)0x07

#define CANTP_CF_DATALEN_MIX11 (PduLengthType)0x06

#define CANTP_CF_DATALEN_MIX29 (PduLengthType)0x06

/*CF data offset for CF of standard addressing*/
#define CANTP_CF_DATA_OFFSET_STD (PduLengthType)0x01
#define CANTP_CF_DATA_OFFSET_EX (PduLengthType)0x02
#define CANTP_CF_DATA_OFFSET_NF (PduLengthType)0x01
#define CANTP_CF_DATA_OFFSET_MIX11 (PduLengthType)0x02
#define CANTP_CF_DATA_OFFSET_MIX29 (PduLengthType)0x02

/*data offset of FF*/
#define CANTP_FF_DATA_OFFSET_LT4095_STD (PduLengthType)0x02
#define CANTP_FF_DATA_OFFSET_LT4095_EX (PduLengthType)0x03
#define CANTP_FF_DATA_OFFSET_LT4095_NF (PduLengthType)0x02
#define CANTP_FF_DATA_OFFSET_LT4095_MIX11 (PduLengthType)0x03
#define CANTP_MIX29_FF_DATA_OFFSET_LT4095 (PduLengthType)0x03

#define CANTP_FF_DATA_OFFSET_BT4095_STD (PduLengthType)0x06
#define CANTP_FF_DATA_OFFSET_BT4095_EX (PduLengthType)0x07
#define CANTP_FF_DATA_OFFSET_BT4095_NF (PduLengthType)0x06
#define CANTP_FF_DATA_OFFSET_BT4095_MIX11 (PduLengthType)0x07
#define CANTP_MIX29_FF_DATA_OFFSET_BT4095 (PduLengthType)0x07

/*data offset of SF for standard addressing*/
#define CANTP_SF_DATA_OFFSET_STD (PduLengthType)0x01
#define CANTP_SF_DATA_OFFSET_CANFD_STD (PduLengthType)0x02

/*data offset of SF for extended addressing*/
#define CANTP_SF_DATA_OFFSET_EX (PduLengthType)0x02
#define CANTP_SF_DATA_OFFSET_CANFD_EX (PduLengthType)0x03

/*data offset of SF for normal fix addressing*/
#define CANTP_SF_DATA_OFFSET_NF (PduLengthType)0x01
#define CANTP_SF_DATA_OFFSET_CANFD_NF (PduLengthType)0x02

/*data offset of SF for mix11 addressing*/
#define CANTP_SF_DATA_OFFSET_MIX11 (PduLengthType)0x02
#define CANTP_SF_DATA_OFFSET_CANFD_MIX11 (PduLengthType)0x03

/*data offset of SF for mix29 addressing*/
#define CANTP_SF_DATA_OFFSET_MIX29 (PduLengthType)0x02
#define CANTP_SF_DATA_OFFSET_CANFD_MIX29 (PduLengthType)0x03

/*offset of PCI for standard addressing*/
#define CANTP_PCI_OFFSET_STD (PduLengthType)0x00

/*offset of PCI for extended addressing*/
#define CANTP_PCI_OFFSET_EX (PduLengthType)0x01

/*offset of PCI for normal fixed addressing*/
#define CANTP_PCI_OFFSET_NF (PduLengthType)0x00

/*offset of PCI for mixed11 addressing*/
#define CANTP_PCI_OFFSET_MIX11 (PduLengthType)0x01

/*offset of PCI for mixed29 addressing*/
#define CANTP_PCI_OFFSET_MIX29 (PduLengthType)0x01

/*MetaData information*/
/*extended meta data TA*/
#define CANTP_TA_OFFSET_META_EX   0u
/*mix11 meta data AE*/
#define CANTP_AE_OFFSET_META_MIX  0u
/*mix29 meta data AE*/
#define CANTP_AE_OFFSET_META_MIX29   0u
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
/*normal fix meta data SA and TA*/
#define CANTP_SA_OFFSET_META_NF_MSB   3u
#define CANTP_TA_OFFSET_META_NF_MSB   2u
/*mix29 meta data SA and TA*/
#define CANTP_SA_OFFSET_META_MIX29_MSB   3u
#define CANTP_TA_OFFSET_META_MIX29_MSB   2u

#define CANTP_SA_FOR_DCM_MSB   1u
#define CANTP_TA_FOR_DCM_MSB   3u
#else
/*normal fix meta data SA and TA*/
#define CANTP_SA_OFFSET_META_NF_LSB   0u
#define CANTP_TA_OFFSET_META_NF_LSB   1u
/*mix29 meta data SA and TA*/
#define CANTP_SA_OFFSET_META_MIX29_LSB   0u
#define CANTP_TA_OFFSET_META_MIX29_LSB   1u

#define CANTP_SA_FOR_DCM_LSB   0u
#define CANTP_TA_FOR_DCM_LSB   2u
#endif
#define CANTP_AE_FOR_DCM   4u

/*Event flags*/
#define CANTP_CHANNEL_EVENT_SF_RECEIVED (uint16)0x01

#define CANTP_CHANNEL_EVENT_FF_RECEIVED (uint16)0x02

#define CANTP_CHANNEL_EVENT_CF_RECEIVED (uint16)0x04

#define CANTP_CHANNEL_EVENT_FC_RECEIVED (uint16)0x08

#define CANTP_CHANNEL_EVENT_RECEIVED_MASK (uint16)0x03/*only SF and FF*/

#define CANTP_CHANNEL_EVENT_TX_CONFIRMED (uint16)0x10

#define CANTP_CHANNEL_EVENT_TXFC_CONFIRMED (uint16)0x20

#define CANTP_CHANNEL_EVENT_TXCANCELLED (uint16)0x40

#define CANTP_CHANNEL_EVENT_TRANSMITREQUEST (uint16)0x80

#define CANTP_CHANNEL_EVENT_RXCANCELLED (uint16)0x100

#define CANTP_CHANNEL_EVENT_RXBSCHANGED (uint16)0x200

#define CANTP_CHANNEL_EVENT_RXSTMINCHANGED (uint16)0x400

/*CanTpPduFlag*/
#define CANTP_METADATA_LENGTH_MASK       (uint8)0x0F

#define CANTP_ADDRESS_EXTENSION_8_MASK   (uint8)0x10

#define CANTP_TC_MASK                    (uint8)0x20

#define CANTP_CAN_ID_32_MASK             (uint8)0x80

#define CANTP_SA16_AND_TA16_MASK         (uint8)0x40


/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* State of the CanTp module. Initialized or not. */
typedef enum CanTp_State_E
{
    CANTP_OFF, /* CanTp module not initialized */
    CANTP_ON   /* CanTp module initialized */
} CanTp_StateType;

/* current enabled ABC timer for receiver or sender  */
typedef enum CanTp_ABCTimer_E
{
    CANTP_TIMER_NONE, /* No timer started */
    CANTP_NA,   /* Nar or Nas started */
    CANTP_NB,   /* Nbs or Nbr started */
    CANTP_NC    /* Ncs or Ncr started */
} CanTp_ABCTimerType;

/* running control of the ABC timer for receiver and sender */
typedef struct CanTp_ChannelTimer_T
{
    CanTp_ABCTimerType EnabledTimer;
    uint32 StartTime;
    uint32 RemainTime;
} CanTp_ChannelTimerType;

/* running control for STmin for sender */
typedef struct CanTp_STmin_T
{
    boolean Started;
    uint32 RemainTime;
    uint32 StartTime;
    uint32 FcSTMin;
} CanTp_STminType;

/*root states of communication channel*/
typedef enum CanTp_ChannelRootState_E
{
    CANTP_IDLE, /*the channel is idle*/
    CANTP_SF_RECEIVING, /*the channel is handling SF receiving*/
    CANTP_SF_TRANSIMITTING, /*the channel is handling SF transmitting*/
    CANTP_LARGE_RECEIVING, /*the channel is handling large SDU receiving*/
    CANTP_LARGE_TRANSMITTING /*the channel is handling large SDU transmitting*/
} CanTp_ChannelRootStateType;

/*Sub-states of communication channel*/
typedef enum CanTp_ChannelSubstate_E
{
    CANTP_SUBSTATE_NONE,

    CANTP_RX_SF_START, /*SF Receiving*/

	CANTP_RX_FF_START,
    CANTP_RX_LARGE_START, /*Large receiving start*/
    CANTP_RX_LARGE_V4FCWT_CFM, /*wait for confirmation of FC_WAIT*/
    CANTP_RX_LARGE_V4FCCTS_CFM, /*wait for confirmation of FC_CTS*/
    CANTP_RX_LARGE_V4CF, /*wait for the following CFs*/
    CANTP_RX_LARGE_RXCF, /*receiving received CF*/

    CANTP_TX_SF_START, /*SF transmitting start*/
    CANTP_TX_SF_V4SF_CFM, /*wait for confirmation of SF transmitting*/

    CANTP_TX_LARGE_START, /*large transmitting start*/
    CANTP_TX_LARGE_V4DATA_CFM, /*wait for confirmation of transmitted frame*/
    CANTP_TX_LARGE_V4FC /*wait for FC*/
} CanTp_ChannelSubstateType;

typedef struct CanTp_Connection_T
{
    /*runtime timer for channel*/
    CanTp_ChannelTimerType ChannelTimer;

    /*STmin timer runtime information*/
    CanTp_STminType STminTimer;

    /*root state of channel*/
    CanTp_ChannelRootStateType RootState;

    /*sub-state of channel*/
    CanTp_ChannelSubstateType Substate;

    /*handled frame type in current phase*/
    CanTp_FramePCIType HandleType;

    /*identifier of N-SDU which is using this channel*/
    PduIdType NSduId;

    /*newly received N-PDU identifier*/
    PduIdType RcvdNPduId;

    /*newly received N-SDU identifier*/
    PduIdType RcvdNSduId;

    PduLengthType CanFrameLength;

    PduLengthType UpperBufRemaining;

    /*data in bytes need to deal with of this N-SDU*/
    PduLengthType SduDataRemaining;

    /*total count of N-SDU data in bytes*/
    PduLengthType SduDataTotalCount;

    /*length of receive data, include pci and metaData*/
    PduLengthType DataLen;

    /*length of valid data in local buffer,exclude pci and metaData*/
    PduLengthType LocalDataLen;

    /*offset of the valid data in local buffer*/
    PduLengthType LocalDataPosition;

#if (STD_ON == CANTP_FD)
    /*local buffer of channel*/
    uint8 LocalBuf[CANTP_CANFD_FRAME_LEN_MAX];

    /*FC content received or transmit by this channel*/
    uint8 FcBuf[CANTP_CANFD_FRAME_LEN_MAX];
#else
    /*local buffer of channel*/
    uint8 LocalBuf[CANTP_CAN20_FRAME_LEN_MAX];

    /*FC content received or transmit by this channel*/
    uint8 FcBuf[CANTP_CAN20_FRAME_LEN_MAX];
#endif

    /*current BS value*/
    uint8 CurrentBs;

#if (STD_ON == CANTP_FD)
    PduLengthType FDDlc;
#endif

    uint8 ChangedBs;

    uint8 ChangedSTmin;

    /*count of handled CF in a block*/
    uint8 HandledCfCount;

    /*current CF serial number*/
    uint8 CurrentCfSn;

    /*number of already sent FCwait consecutively*/
    uint8 SentWftCount;

    /*Event Flags*/
    uint16 EventFlags;

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    /*The addressing information in the MedataData depends on the addressing format*/
    uint8 MetaDataN_TA;

    /*The addressing information in the MedataData depends on the addressing format*/
    uint8 MetaDataN_SA;

    /*The addressing information in the MedataData depends on the addressing format*/
    uint8 MetaDataN_AE;

    uint8 MetaDataLength;
#endif
}CanTp_ConnectionType;

/* Channel parameters during CanTp module running. Channel is a logic
 * connection between sender and receiver. */
typedef struct CanTp_ConnectionChannel_T
{
	CanTp_ChannelModeType ChannelMode;
	CanTp_ConnectionType RxConnection;
	CanTp_ConnectionType TxConnection;
} CanTp_ConnectionChannelType;

/*structure for FC frame information*/
typedef struct CanTp_FCInfo_T
{
    uint8 FcFs;
    uint8 FcBS;
    uint32 FcSTMin; /*STmin in tick unit*/
} CanTp_FCInfoType;

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANTP_START_SEC_VAR_POWER_ON_INIT_8
#include "CanTp_MemMap.h"
/*CANTP global variable for module state:CANTP_ON, CANTP_OFF*/
extern volatile VAR(CanTp_StateType, CANTP_VAR_POWER_ON_INIT) CanTp_ModuleState;
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_8
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTp_MemMap.h"
/*CANTP global variable for runtime channel controlling information*/
extern VAR(CanTp_ConnectionChannelType, CANTP_VAR_NOINIT) CanTp_Channels[CANTP_CHANNEL_NUMBER];
#define CANTP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "CanTp_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

/*Get TX-SDU configuration by TX-NSduId.*/
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxSduId(PduIdType TxSduId, uint8 *channelId);

/*Get RX-SDU configuration by received NPDU information.*/
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2VAR(CanTp_FramePCIType, AUTOMATIC, AUTOMATIC) SduFrameType,
	P2VAR(uint8, AUTOMATIC, AUTOMATIC) channelIdx
    );

/*Get TX-SDU configuration by received FC NPDU information.*/
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByFCNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
	P2VAR(uint8, AUTOMATIC, AUTOMATIC) channelIdx
    );

/*Get RxSDU configuration by N-SDU ID.*/
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByNSduId(PduIdType CanTpRxSduId, uint8* channelId);

/*Get Tx-SDU configuration by Tx NPDUID.*/
extern FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetTxSduCfgByTxNPdu(PduIdType CanTpTxPduId, uint8 *channelId);

/*Get Rx-SDU configuration by Tx-FC NPDUID.*/
extern FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
CanTp_GetRxSduCfgByTxFCNPdu(PduIdType CanTpTxPduId, uint8 *channelId);

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*check RxFc meta data*/
extern FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckRxFCMetaData(
	uint8 channelIdx,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxInfoPtr
	);

/*store Tx meta data info*/
extern FUNC(void, CANTP_CODE)CanTp_SaveTxMetaDataInfo(
	uint8 channelId,
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr
	);

/*check RxCF meta data*/
extern FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckRxCFMetaData(
    uint8 channelIdx,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxInfoPtr
    );

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
extern FUNC(void, CANTP_CODE)CanTp_ConstructForwardRxMetaData(
    uint8 channelId,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_VAR) CanTpRxInfoPtr
    );
#endif

/*store Rx meta data info*/
extern FUNC(Std_ReturnType, CANTP_CODE)CanTp_SaveRxMetaDataInfo(
    uint8 channelId,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxInfoPtr
    );
#endif
/*Handle received CF.*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleCFReception(uint8 ChannelIdx);

/*Handle received FF.*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleFFReception(uint8 ChannelIdx);

/*Handle received SF.*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleSFReception(uint8 ChannelIdx);

/*Handle confirmation for FC.*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleFCCfm(uint8 ChannelIdx);

/*Back ground handling for receiving SF,in this state the channel
 * is trying to get buffer to store received SF.*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleSFStart(uint8 ChannelIdx);

/*Back ground handling for large SDU receiving, trying to get RX buffer.*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleLargeStart(uint8 ChannelIdx);

/*Get FF_DL information from the data content of FF.*/
extern FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetFFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr);

/*Get SFF_DL information from the data content of SF.*/
extern FUNC(PduLengthType, CANTP_CODE)
CanTp_RxGetSFDl(
        P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
        P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr
#if (STD_ON == CANTP_FD)
		,PduLengthType CanTpDLC
#endif
		);

/*Handle received FC.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleFCReception(uint8 ChannelIdx);

/*Handle transmit request.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitReq(uint8 ChannelIdx);

/*Handle transmit cancel request event.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCancel(uint8 ChannelIdx);

/*Handle confirmation for data transmit.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleTransmitCfm(uint8 ChannelIdx);

/*Back ground handling for SF transmitting.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleSFStart(uint8 ChannelIdx);

/*Back ground handling for large SDU transmitting, trying to get TX buffer.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeStart(uint8 ChannelIdx);

/*Back ground handling for sending CFs.*/
extern FUNC(void, CANTP_CODE)
CanTp_TxHandleLargeSendCF(uint8 ChannelIdx);

/*Set memory with specific value.*/
extern FUNC(void, CANTP_CODE)
CanTp_MemorySet(
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Address,
    uint8 Value,
    uint16 Length);

/*check Tx length in address mode*/
extern FUNC(Std_ReturnType, CANTP_CODE)
CanTp_CheckTxLengthInAddressMode(
	P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
	P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpTxInfoPtr);

/*FF come, just execute one time of PduR_CanTpStartOfReception*/
extern FUNC(void, CANTP_CODE)
CanTp_RxHandleFFStart(uint8 ChannelIdx);

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/*check Rx ChangeParameter*/
extern FUNC(boolean, CANTP_CODE)
	CanTP_CheckRxChangeParameter(
	TPParameterType canTpParam,
	uint16 value);
#endif

/*Cancel Receive*/
extern FUNC(void, CANTP_CODE)
	CanTp_RxHandleReceiveCancel(uint8 ChannelIdx);

#if (STD_ON == CANTP_FD)
/*match CanFd Data length*/
extern FUNC(PduLengthType, CANTP_CODE)
CanTp_MatchDLC(PduLengthType frameDL);
#endif

extern FUNC(void, CANTP_CODE)
CanTp_ResetTime(
P2VAR(uint32, AUTOMATIC, CANTP_APPL_DATA) TimerPtr
);

#if (STD_ON == CANTP_FD)
extern FUNC(boolean, CANTP_CODE) CanTp_checkSF(
        PduLengthType SF_DL,PduLengthType CAN_DL,PduLengthType Offset);
extern FUNC(boolean, CANTP_CODE) CanTp_checkLastCF(
        PduLengthType CF_DL,PduLengthType CAN_DL,PduLengthType Offset);
#endif

extern FUNC(void, CANTP_CODE)
CanTp_InitRxChannel(uint8 ChannelIdx);

extern FUNC(void, CANTP_CODE)
CanTp_ReleaseRxChannel(uint8 ChannelIdx);

extern FUNC(void, CANTP_CODE)
CanTp_ReleaseTxChannel(uint8 ChannelIdx);
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_INTERNAL_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
