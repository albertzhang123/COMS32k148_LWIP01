################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Buffers.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Enet.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Ipw.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Irq.c 

OBJS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Buffers.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Enet.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Ipw.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Irq.o 

C_DEPS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Buffers.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Enet.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Ipw.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth_Irq.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/%.o: ../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Eth_TS_T40D2M10I1R0/src/Eth.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


