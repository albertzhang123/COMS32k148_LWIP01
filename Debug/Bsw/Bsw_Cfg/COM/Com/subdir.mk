################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Cfg/COM/Com/Com_Cfg.c 

OBJS += \
./Bsw/Bsw_Cfg/COM/Com/Com_Cfg.o 

C_DEPS += \
./Bsw/Bsw_Cfg/COM/Com/Com_Cfg.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Cfg/COM/Com/%.o: ../Bsw/Bsw_Cfg/COM/Com/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Cfg/COM/Com/Com_Cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


