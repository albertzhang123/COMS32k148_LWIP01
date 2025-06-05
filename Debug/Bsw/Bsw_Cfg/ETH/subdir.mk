################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Cfg/ETH/EthIf_Cfg.c \
../Bsw/Bsw_Cfg/ETH/EthSM_Cfg.c \
../Bsw/Bsw_Cfg/ETH/SoAd_Cfg.c \
../Bsw/Bsw_Cfg/ETH/SoAd_PBcfg.c \
../Bsw/Bsw_Cfg/ETH/TcpIp_Lcfg.c \
../Bsw/Bsw_Cfg/ETH/TcpIp_PBcfg.c 

OBJS += \
./Bsw/Bsw_Cfg/ETH/EthIf_Cfg.o \
./Bsw/Bsw_Cfg/ETH/EthSM_Cfg.o \
./Bsw/Bsw_Cfg/ETH/SoAd_Cfg.o \
./Bsw/Bsw_Cfg/ETH/SoAd_PBcfg.o \
./Bsw/Bsw_Cfg/ETH/TcpIp_Lcfg.o \
./Bsw/Bsw_Cfg/ETH/TcpIp_PBcfg.o 

C_DEPS += \
./Bsw/Bsw_Cfg/ETH/EthIf_Cfg.d \
./Bsw/Bsw_Cfg/ETH/EthSM_Cfg.d \
./Bsw/Bsw_Cfg/ETH/SoAd_Cfg.d \
./Bsw/Bsw_Cfg/ETH/SoAd_PBcfg.d \
./Bsw/Bsw_Cfg/ETH/TcpIp_Lcfg.d \
./Bsw/Bsw_Cfg/ETH/TcpIp_PBcfg.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Cfg/ETH/%.o: ../Bsw/Bsw_Cfg/ETH/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Cfg/ETH/EthIf_Cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


