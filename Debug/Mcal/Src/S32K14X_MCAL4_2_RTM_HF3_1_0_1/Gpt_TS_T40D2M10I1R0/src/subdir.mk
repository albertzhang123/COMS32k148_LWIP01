################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Ftm.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Ipw.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_LPit.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Lptmr.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_SRtc.c 

OBJS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Ftm.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Ipw.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_LPit.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Lptmr.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_SRtc.o 

C_DEPS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Ftm.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Ipw.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_LPit.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_Lptmr.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt_SRtc.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/%.o: ../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Gpt_TS_T40D2M10I1R0/src/Gpt.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


