################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can_Flexcan.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can_Irq.c 

OBJS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can_Flexcan.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can_Irq.o 

C_DEPS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can_Flexcan.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can_Irq.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/%.o: ../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Can_TS_T40D2M10I1R0/src/Can.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


