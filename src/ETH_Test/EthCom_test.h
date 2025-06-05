#ifndef ETHCOM_TEST_H
#define ETHCOM_TEST_H

typedef enum
{
    ETHCOMTEST_NONE = 0u,
    ETHCOMTEST_COMTX,
    ETHCOMTEST_COMRX,
    ETHCOMTEST_LDCOMTX,
    ETHCOMTEST_LDCOMRX
}EthComTestCmdType;
extern uint8 LdComTxPduData[1000];
extern uint16 LdComTxPduLen;
void EthCom_Test(uint16 RxPduLen,uint8 * RxData);

#endif

