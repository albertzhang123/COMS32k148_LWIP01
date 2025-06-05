################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsd/DcmDsd.c 

OBJS += \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsd/DcmDsd.o 

C_DEPS += \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsd/DcmDsd.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/DIAG/Dcm/DcmDsd/%.o: ../Bsw/Bsw_Src/DIAG/Dcm/DcmDsd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/DIAG/Dcm/DcmDsd/DcmDsd.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


