################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Cfg/DIAG/Dem/Dem_Cfg.c 

OBJS += \
./Bsw/Bsw_Cfg/DIAG/Dem/Dem_Cfg.o 

C_DEPS += \
./Bsw/Bsw_Cfg/DIAG/Dem/Dem_Cfg.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Cfg/DIAG/Dem/%.o: ../Bsw/Bsw_Cfg/DIAG/Dem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Cfg/DIAG/Dem/Dem_Cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


