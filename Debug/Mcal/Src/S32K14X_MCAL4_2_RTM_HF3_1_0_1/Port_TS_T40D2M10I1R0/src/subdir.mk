################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port_Ipw.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port_Port_Ci.c 

OBJS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port_Ipw.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port_Port_Ci.o 

C_DEPS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port_Ipw.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port_Port_Ci.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/%.o: ../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Port_TS_T40D2M10I1R0/src/Port.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


