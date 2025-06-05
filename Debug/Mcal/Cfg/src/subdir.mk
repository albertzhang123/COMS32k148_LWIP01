################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mcal/Cfg/src/CDD_Mcl_Cfg.c \
../Mcal/Cfg/src/CDD_Mcl_PBcfg.c \
../Mcal/Cfg/src/Can_Cfg.c \
../Mcal/Cfg/src/Can_PBcfg.c \
../Mcal/Cfg/src/Dio_Cfg.c \
../Mcal/Cfg/src/Eth_Cfg.c \
../Mcal/Cfg/src/Eth_PBcfg.c \
../Mcal/Cfg/src/Fee_Cfg.c \
../Mcal/Cfg/src/Fls_Cfg.c \
../Mcal/Cfg/src/Fls_PBcfg.c \
../Mcal/Cfg/src/Gpt_PBcfg.c \
../Mcal/Cfg/src/Mcu_Cfg.c \
../Mcal/Cfg/src/Mcu_PBcfg.c \
../Mcal/Cfg/src/Port_Cfg.c \
../Mcal/Cfg/src/Port_PBcfg.c 

OBJS += \
./Mcal/Cfg/src/CDD_Mcl_Cfg.o \
./Mcal/Cfg/src/CDD_Mcl_PBcfg.o \
./Mcal/Cfg/src/Can_Cfg.o \
./Mcal/Cfg/src/Can_PBcfg.o \
./Mcal/Cfg/src/Dio_Cfg.o \
./Mcal/Cfg/src/Eth_Cfg.o \
./Mcal/Cfg/src/Eth_PBcfg.o \
./Mcal/Cfg/src/Fee_Cfg.o \
./Mcal/Cfg/src/Fls_Cfg.o \
./Mcal/Cfg/src/Fls_PBcfg.o \
./Mcal/Cfg/src/Gpt_PBcfg.o \
./Mcal/Cfg/src/Mcu_Cfg.o \
./Mcal/Cfg/src/Mcu_PBcfg.o \
./Mcal/Cfg/src/Port_Cfg.o \
./Mcal/Cfg/src/Port_PBcfg.o 

C_DEPS += \
./Mcal/Cfg/src/CDD_Mcl_Cfg.d \
./Mcal/Cfg/src/CDD_Mcl_PBcfg.d \
./Mcal/Cfg/src/Can_Cfg.d \
./Mcal/Cfg/src/Can_PBcfg.d \
./Mcal/Cfg/src/Dio_Cfg.d \
./Mcal/Cfg/src/Eth_Cfg.d \
./Mcal/Cfg/src/Eth_PBcfg.d \
./Mcal/Cfg/src/Fee_Cfg.d \
./Mcal/Cfg/src/Fls_Cfg.d \
./Mcal/Cfg/src/Fls_PBcfg.d \
./Mcal/Cfg/src/Gpt_PBcfg.d \
./Mcal/Cfg/src/Mcu_Cfg.d \
./Mcal/Cfg/src/Mcu_PBcfg.d \
./Mcal/Cfg/src/Port_Cfg.d \
./Mcal/Cfg/src/Port_PBcfg.d 


# Each subdirectory must supply rules for building sources it contributes
Mcal/Cfg/src/%.o: ../Mcal/Cfg/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Mcal/Cfg/src/CDD_Mcl_Cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


