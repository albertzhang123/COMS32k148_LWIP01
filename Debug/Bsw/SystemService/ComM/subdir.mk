################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/SystemService/ComM/ComM.c \
../Bsw/SystemService/ComM/ComM_Dcm.c \
../Bsw/SystemService/ComM/ComM_Internal.c 

OBJS += \
./Bsw/SystemService/ComM/ComM.o \
./Bsw/SystemService/ComM/ComM_Dcm.o \
./Bsw/SystemService/ComM/ComM_Internal.o 

C_DEPS += \
./Bsw/SystemService/ComM/ComM.d \
./Bsw/SystemService/ComM/ComM_Dcm.d \
./Bsw/SystemService/ComM/ComM_Internal.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/SystemService/ComM/%.o: ../Bsw/SystemService/ComM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/SystemService/ComM/ComM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


