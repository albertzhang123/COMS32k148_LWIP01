
#include "ComStack_Types.h"
#include "LdCom.h"

void Stub_LdComEthUdpTxConfirmation(void) {}

void Stub_LdComEthUdpRxIndication(const PduInfoType* PduInfoPtr) {}

uint8 ldComEthTx[1000] = {0};

void Stub_LdComTask(void) {
  ldComEthTx[10]++;
  PduInfoType PduInfo = {.MetaDataPtr = NULL_PTR,
                         .SduDataPtr = ldComEthTx,
                         .SduLength = sizeof(ldComEthTx)};
  LdCom_Transmit(LdComIPdu_EthUdpTx, &PduInfo);
}
