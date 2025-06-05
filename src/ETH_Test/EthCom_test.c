#include "Com.h"
#include "LdCom.h"
#include "EthCom_test.h"
#include "LdCom_Cbk.h"

uint8 LdComTxPduData[1000];
uint16 LdComTxPduLen = 0u;
EthComTestCmdType EthComTestCmd = ETHCOMTEST_NONE;

static void EthCom_ComTxSignal(uint8 * RxData)
{
    uint8 SignalId;
    uint16 SignalLen;
    SignalId = RxData[2];
    SignalLen = (((uint16)RxData[3] << 8) | ((uint16)RxData[4]));
    if((TxComSignal_Eth == SignalId) && (SignalLen == 8))
    {
        Com_SendSignal(TxComSignal_Eth,&RxData[5]);
    }
    else if((TxTpSignal_0 == SignalId) && (0 < SignalLen) && (SignalLen <= 100))
    {
        Com_SendDynSignal(TxTpSignal_0,&RxData[5],SignalLen);
    }
    else
    {

    }
}

static void EthCom_ComRxSignal(uint8 * RxData)
{
    uint8 SignalId;
    uint16 SignalLen;
    SignalId = RxData[2];
    SignalLen = (((uint16)RxData[3] << 8) | ((uint16)RxData[4]));
    if((RxComSignal_Eth == SignalId) && (SignalLen == 8u))
    {
        PduInfoType pduInfo;
        pduInfo.MetaDataPtr = NULL_PTR;
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 8u;
        LdComTxPduLen = 8u;
        Com_ReceiveSignal(RxComSignal_Eth,LdComTxPduData);
        LdCom_Transmit(LdComIPdu_TxTP_Udp,&pduInfo);
    }
    else if(RxTpSignal_0 == SignalId)
    {
        PduInfoType pduInfo;
        pduInfo.MetaDataPtr = NULL_PTR;
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = 1000u;
        Com_ReceiveDynSignal(RxTpSignal_0,LdComTxPduData,&(pduInfo.SduLength));
        LdComTxPduLen = pduInfo.SduLength;
        LdCom_Transmit(LdComIPdu_TxTP_Udp,&pduInfo);
    }
    else
    {

    }
}

static void EthCom_LdComTxPdu(uint8 * RxData)
{
    uint8 PduId;
    PduInfoType pduInfo;
    uint16 cnt;
    PduId = RxData[2];
    if(PduId == LdComIPdu_TxTP_Udp)
    {
        pduInfo.MetaDataPtr = NULL_PTR;
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = (((uint16)RxData[3] << 8) | ((uint16)RxData[4]));
        for(cnt = 0u;cnt < pduInfo.SduLength;cnt++)
        {
            LdComTxPduData[cnt] = RxData[5 + cnt];
        }
        LdComTxPduLen = pduInfo.SduLength;
        LdCom_Transmit(LdComIPdu_TxTP_Udp,&pduInfo);
    }
}

static void EthCom_LdComRxPdu(uint16 RxPduLen,uint8 * RxData)
{
    uint8 PduId;
    PduInfoType pduInfo;
    uint16 cnt;
    PduId = RxData[2];
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduDataPtr = NULL_PTR;
    if(PduId == LdComIPdu_RxTP_Tcp)
    {
        pduInfo.SduLength = RxPduLen;
        for(cnt = 0u;cnt < RxPduLen;cnt++)
        {
            LdComTxPduData[cnt] = RxData[cnt];
        }
        LdComTxPduLen = RxPduLen;
        LdCom_Transmit(LdComIPdu_TxTP_Udp,&pduInfo);
    }
}

void EthCom_Test(uint16 RxPduLen,uint8 * RxData)
{
    if((RxData[0] == 1u) && (RxData[1] == 1u))
    {
        EthComTestCmd = ETHCOMTEST_COMTX;
    }
    else if((RxData[0] == 1u) && (RxData[1] == 2u))
    {
        EthComTestCmd = ETHCOMTEST_LDCOMTX;
    }
    else if((RxData[0] == 2u) && (RxData[1] == 2u))
    {
        EthComTestCmd = ETHCOMTEST_LDCOMRX;
    }
    else if((RxData[0] == 2u) && (RxData[1] == 1u))
    {
        EthComTestCmd = ETHCOMTEST_COMRX;
    }
    else
    {
        EthComTestCmd = ETHCOMTEST_NONE;
    }
    switch(EthComTestCmd)
    {
        case ETHCOMTEST_COMTX:
            EthCom_ComTxSignal(RxData);
            break;
        case ETHCOMTEST_COMRX:
            EthCom_ComRxSignal(RxData);
            break;
        case ETHCOMTEST_LDCOMTX:
            EthCom_LdComTxPdu(RxData);
            break;
        case ETHCOMTEST_LDCOMRX:
            EthCom_LdComRxPdu(RxPduLen,RxData);
            break;
        default:
            break;
    }
    EthComTestCmd = ETHCOMTEST_NONE;
}
