################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/COM/PDUR/PduR.c \
../Bsw/Bsw_Src/COM/PDUR/PduR_Internal.c 

OBJS += \
./Bsw/Bsw_Src/COM/PDUR/PduR.o \
./Bsw/Bsw_Src/COM/PDUR/PduR_Internal.o 

C_DEPS += \
./Bsw/Bsw_Src/COM/PDUR/PduR.d \
./Bsw/Bsw_Src/COM/PDUR/PduR_Internal.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/COM/PDUR/%.o: ../Bsw/Bsw_Src/COM/PDUR/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/COM/PDUR/PduR.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


