################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/COM/Com/Com.c \
../Bsw/Bsw_Src/COM/Com/Com_GwInternal.c \
../Bsw/Bsw_Src/COM/Com/Com_Internal.c \
../Bsw/Bsw_Src/COM/Com/Com_RxInternal.c \
../Bsw/Bsw_Src/COM/Com/Com_TxInternal.c 

OBJS += \
./Bsw/Bsw_Src/COM/Com/Com.o \
./Bsw/Bsw_Src/COM/Com/Com_GwInternal.o \
./Bsw/Bsw_Src/COM/Com/Com_Internal.o \
./Bsw/Bsw_Src/COM/Com/Com_RxInternal.o \
./Bsw/Bsw_Src/COM/Com/Com_TxInternal.o 

C_DEPS += \
./Bsw/Bsw_Src/COM/Com/Com.d \
./Bsw/Bsw_Src/COM/Com/Com_GwInternal.d \
./Bsw/Bsw_Src/COM/Com/Com_Internal.d \
./Bsw/Bsw_Src/COM/Com/Com_RxInternal.d \
./Bsw/Bsw_Src/COM/Com/Com_TxInternal.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/COM/Com/%.o: ../Bsw/Bsw_Src/COM/Com/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/COM/Com/Com.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


