################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/autoip.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/dhcp.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/etharp.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/icmp.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/igmp.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4_addr.c \
../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4_frag.c 

OBJS += \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/autoip.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/dhcp.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/etharp.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/icmp.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/igmp.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4_addr.o \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/autoip.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/dhcp.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/etharp.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/icmp.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/igmp.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4_addr.d \
./Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/%.o: ../Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/ETH/TcpIp/lwipSource/src/core/ipv4/autoip.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


