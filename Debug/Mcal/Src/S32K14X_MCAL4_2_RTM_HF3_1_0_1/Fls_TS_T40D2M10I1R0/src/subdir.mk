################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Ac.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Flash.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Flash_Const.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_IPW.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Qspi.c \
../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Qspi_Irq.c 

OBJS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Ac.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Flash.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Flash_Const.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_IPW.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Qspi.o \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Qspi_Irq.o 

C_DEPS += \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Ac.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Flash.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Flash_Const.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_IPW.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Qspi.d \
./Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls_Qspi_Irq.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/%.o: ../Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Src/S32K14X_MCAL4_2_RTM_HF3_1_0_1/Fls_TS_T40D2M10I1R0/src/Fls.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


