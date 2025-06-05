################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Libraries/Crc/Crc.c 

OBJS += \
./Bsw/Libraries/Crc/Crc.o 

C_DEPS += \
./Bsw/Libraries/Crc/Crc.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Libraries/Crc/%.o: ../Bsw/Libraries/Crc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Libraries/Crc/Crc.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


