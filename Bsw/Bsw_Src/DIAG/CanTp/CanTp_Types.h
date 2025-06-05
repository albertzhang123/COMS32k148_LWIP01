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
**  FILENAME    : CanTp_Types.h                                              **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Public type definitions for CanTp module                   **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef CANTP_TYPES_H
#define CANTP_TYPES_H

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Addressing mode is supported for RX or TX N-SDU */
typedef enum CanTp_AddressingFormat_E
{
    CANTP_STANDARD, /* Standard addressing format */
    CANTP_EXTENDED, /* Extended addressing format */
	CANTP_NORMALFIXED,
	CANTP_MIXED,
	CANTP_MIXED29BIT
} CanTp_AddressingFormatType;

/* The communication type of RX or TX N-SDU */
typedef enum CanTp_RxTaType_E
{
	CANTP_CANFD_FUNCTIONAL,
	CANTP_CANFD_PHYSICAL,
    CANTP_FUNCTIONAL_RX, /* Functional request type */
    CANTP_PHYSICAL_RX /* physical request type */
} CanTp_RxTaTypeType;

typedef enum CanTp_TxTaType_E
{
    CANTP_FUNCTIONAL_TX, /* Functional request type */
    CANTP_PHYSICAL_TX /* physical request type */
} CanTp_TxTaTypeType;

typedef enum CanTp_ChannelMode_E
{
	CANTP_MODE_FULL_DUPLEX,
	CANTP_MODE_HALF_DUPLEX
} CanTp_ChannelModeType;

typedef enum CanTp_CanFrame_E
{
	CAN20            = 0,
	CAN_FD           = 1
} CanTp_CanFrameType;

typedef enum CanTp_PaddingActivation_E
{
	CANTP_PADDING_OFF        = 0,
	CANTP_PADDING_ON         = 1
}CanTp_PaddingActivationType;

/* Parameters needs to be configured for each CAN N-SDU that the CanTp module shall receive. */
typedef struct CanTp_RxNSdu_T
{
    /* Maximum number of N=PDUs the CanTp receiver allows the sender to send,
     * before waiting for an authorization to continue transmission of the following N-PDUs. */
    P2CONST(uint8, TYPEDEF, CANTP_CONST) Bs;

    /* Value of the N-Ar timeout. Unit:the number of OS counter value. */
    P2CONST(uint32, TYPEDEF, CANTP_CONST) Nar;

    /* Value of the performance requirement for (N_Br+N_Ar).
     *  N_Br is the elapsed time between the receiving
     * indication of a FF or CF or the transmit confirmation of a FC,
     *  until the transmit request of the next FC. */
    uint32 Nbr;

    /* Value of the N_Cr timeout. N_Cr is the time until the reception
     *  of the next Consecutive Frame N-PDU. */
    P2CONST(uint32, TYPEDEF, CANTP_CONST) Ncr;

    /* communication addressing mode for this RX N-SDU */
    CanTp_AddressingFormatType AddressingFormat;

    /* I-PduId of PduR or Dcm, the referenced Rx N_SDU in COM stack. */
    PduIdType RxIPduId;

    /* CanTp rx N_SDU*/
    PduIdType RxNSduId;

    /* CanTp rx N_Pdu*/
    PduIdType RxNPduId;

    /* CanTp tx FC_N_Pdu*/
    PduIdType TxFcNPduId;

    /* L-PduId of CanIf, the referenced Tx FC_N_PDU in COM stack*/
    PduIdType TxFcLPduId;

    uint8 CanTpPduFlag;/*bit0-3:MetaDataLength,bit5:ADDRESS_EXTENSION_8,
    bit6:SOURCE_ADDRESS_16 and TARGET_ADDRESS_16,bit7:CAN_ID_32 */

    PduLengthType RxNPduDLC;

    /* Defines if the receive frame uses padding or not. */
    CanTp_PaddingActivationType RxPaddingActivation;

    /* Declares the communication type of this Rx N-SDU. */
    CanTp_RxTaTypeType RxTaType;

    /* Indicates how many Flow Control wait N-PDUs can be
     * consecutively transmitted by the receiver. */
    P2CONST(uint8, TYPEDEF, CANTP_CONST) RxWftMax;

    /* Duration of the minimum time the CanTp Sender shall wait
     * between the transmissions of two CF N-PDUs. */
    P2CONST(uint8, TYPEDEF, CANTP_CONST) STmin;

    P2CONST(uint8, TYPEDEF, CANTP_CONST) NAe;

    P2CONST(uint8, TYPEDEF, CANTP_CONST) NSa;

    P2CONST(uint8, TYPEDEF, CANTP_CONST) NTa;

    /* CanFD or Can2.0, referenced from Ecuc pdu*/
    CanTp_CanFrameType CanFrameType;
} CanTp_RxNSduType;

/* Parameters needs to be configured for each CAN N_SDU that the CanTp module shall transmit. */
typedef struct CanTp_TxNSdu_T
{
    /* Value of the N_As timeout. N_As is the time for transmission of
     * a CAN frame on the part of the sender. */
    uint32 Nas;

    /* Value of the N_Bs timeout. N_Bs is the time of transmission until
     * reception of the next Flow Control N_PDU. */
    P2CONST(uint32, TYPEDEF, CANTP_CONST) Nbs;

    /* Value of the performation requirement of (N_Cs + N_As). N_Cs is the time which elapses
     *  between the transmit request of a CF N_PDU until the
     *  transmit request of the next CF N-PDU. */
    P2CONST(uint32, TYPEDEF, CANTP_CONST) Ncs;

    /* Declares which communication addressing format is supported for this TXNSdu. */
    CanTp_AddressingFormatType AddressingFormat;

    /*Tx I_Pdu of PduR or DCM, the referenced Tx N_Sdu which referenced Rx FC_N_Pdu*/
    PduIdType TxIPduId;

    /*Tx N_Sdu of Cantp*/
    PduIdType TxNSduId;

    /*Tx N_Pdu of Cantp*/
    PduIdType TxNPduId;

    /*Rx Fc_N_Pdu of Cantp*/
    PduIdType RxFcNPduId;

    /*Tx L-PduId of CanIf, the referenced Tx N_PDU in COM stack*/
    PduIdType TxLPduId;

    uint8 CanTpPduFlag; /*bit0-3:MetaDataLength,bit 4:CanTpTc,
    bit5:ADDRESS_EXTENSION_8,bit6:SOURCE_ADDRESS_16 and TARGET_ADDRESS_16,bit7:CAN_ID_32 */

    PduLengthType TxNPduDLC;

	/* Defines if the transmit frame use padding or not. */
     CanTp_PaddingActivationType TxPaddingActivation;

    /* Communication type of this TxNSdu. */
    CanTp_TxTaTypeType TxTaType;

    P2CONST(uint8, TYPEDEF, CANTP_CONST) NAe;

    P2CONST(uint8, TYPEDEF, CANTP_CONST) NSa;

    P2CONST(uint8, TYPEDEF, CANTP_CONST) NTa;

    /* CanFD or Can2.0, referenced from Ecuc pdu*/
     CanTp_CanFrameType CanFrameType;
} CanTp_TxNSduType;

typedef struct CanTp_Channel_T
{
	CanTp_ChannelModeType ChannelMode;
	uint8 RxNSduNum;
    P2CONST(CanTp_RxNSduType, TYPEDEF, CANTP_CONST) RxNSdus;
    uint8 TxNsduNum;
    P2CONST(CanTp_TxNSduType, TYPEDEF, CANTP_CONST) TxNSdus; /* link to TxNSdus */
} CanTp_ChannelType;

/* Configuration of the CanTp module */
typedef struct CanTp_Config_T
{
	uint32   MainFunctionPeriod;
	uint8    MaxChannelCnt;
    P2CONST(CanTp_ChannelType, TYPEDEF, CANTP_CONST) CanTpChannel;
} CanTp_ConfigType;

/*Cancel Reason for CancelTransmitRequest*/
typedef enum CanTp_CancelReason_T
{
    CANTP_CNLDO, /*Cancel Transfer because data are outdated*/
    CANTP_CNLNB, /*Cancel Transfer because no further buffer can be provided*/
    CANTP_CNLOR  /*Cancel Transfer because of another reason*/
} CanTp_CancelReasonType;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* CANTP_TYPES_H */

/*=======[E N D   O F   F I L E]==============================================*/
