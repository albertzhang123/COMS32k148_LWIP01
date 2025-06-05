################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/ETH/TcpIp/TcpIp.c \
../Bsw/Bsw_Src/ETH/TcpIp/TcpIp_Internal.c 

OBJS += \
./Bsw/Bsw_Src/ETH/TcpIp/TcpIp.o \
./Bsw/Bsw_Src/ETH/TcpIp/TcpIp_Internal.o 

C_DEPS += \
./Bsw/Bsw_Src/ETH/TcpIp/TcpIp.d \
./Bsw/Bsw_Src/ETH/TcpIp/TcpIp_Internal.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/ETH/TcpIp/%.o: ../Bsw/Bsw_Src/ETH/TcpIp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/ETH/TcpIp/TcpIp.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


