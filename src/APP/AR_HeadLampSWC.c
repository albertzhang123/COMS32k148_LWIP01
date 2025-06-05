#include "AR_HeadLampSWC.h"
#include "AR_HeadLampSWC_private.h"

void Runnable_Step(Rte_Instance self)
{
  uint8 HMIHighBeam;
  uint8 HMILowBeam;
  UInt8 DE_LedState1;
  boolean LogicalOperator_o;
  HMIHighBeam = Rte_IRead_Runnable_Step_HMIHighBeam_HMIHighBeam(self);
  HMILowBeam = Rte_IRead_Runnable_Step_HMILowBeam_HMILowBeam(self);
  LogicalOperator_o = ((HMIHighBeam != 0) &&
                       (Rte_IRead_Runnable_Step_HMIPositionLamp_HMIPositionLamp
                        (self) != 0));
  LogicalOperator_o = ((Rte_IRead_Runnable_Step_HMILowBeam_HMILowBeam(self) != 0)
                       || LogicalOperator_o);
  if (HMIHighBeam == 1)
  {
	  DE_LedState1 = 1;
  }
  else
  {
	  DE_LedState1 = 0;
  }
  
  Rte_Call_CS_IF_SetLedState_OP_SetLedState(self,&DE_LedState1);

  Rte_IWrite_Runnable_Step_CMDHighBeam_CMDHighBeam(self, HMIHighBeam);
  Rte_IWrite_Runnable_Step_CMDLowBeam_CMDLowBeam(self, HMILowBeam);
}

void Runnable_Init(Rte_Instance self)
{
	(void)self;
}
