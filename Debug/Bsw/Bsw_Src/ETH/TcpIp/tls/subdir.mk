################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_Externals.c \
../Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsBase.c \
../Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsClient.c \
../Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsGeneral.c \
../Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsServer.c 

OBJS += \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_Externals.o \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsBase.o \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsClient.o \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsGeneral.o \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsServer.o 

C_DEPS += \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_Externals.d \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsBase.d \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsClient.d \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsGeneral.d \
./Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_TlsServer.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/ETH/TcpIp/tls/%.o: ../Bsw/Bsw_Src/ETH/TcpIp/tls/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/ETH/TcpIp/tls/TcpIp_Externals.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


