################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/netif/ethernet.c 

OBJS += \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/netif/ethernet.o 

C_DEPS += \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/netif/ethernet.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/netif/%.o: ../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/netif/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/netif/ethernet.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


