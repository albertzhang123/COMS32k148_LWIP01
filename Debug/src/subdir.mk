################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/EthComm.c \
../src/NXP_TJA1100_Functions.c \
../src/PhyTja110x.c \
../src/main.c \
../src/nvic.c \
../src/sys_init.c 

OBJS += \
./src/EthComm.o \
./src/NXP_TJA1100_Functions.o \
./src/PhyTja110x.o \
./src/main.o \
./src/nvic.o \
./src/sys_init.o 

C_DEPS += \
./src/EthComm.d \
./src/NXP_TJA1100_Functions.d \
./src/PhyTja110x.d \
./src/main.d \
./src/nvic.d \
./src/sys_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/EthComm.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


