################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/MEM/MemIf/MemIf.c 

OBJS += \
./Bsw/Bsw_Src/MEM/MemIf/MemIf.o 

C_DEPS += \
./Bsw/Bsw_Src/MEM/MemIf/MemIf.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/MEM/MemIf/%.o: ../Bsw/Bsw_Src/MEM/MemIf/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/MEM/MemIf/MemIf.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


