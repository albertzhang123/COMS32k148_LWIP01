################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio_Gpio.c 

OBJS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio_Gpio.o 

C_DEPS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio_Gpio.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/%.o: ../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Dio_TS_T40D2M10I1R0/src/Dio.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


