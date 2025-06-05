################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/APP/AR_HeadLampSWC.c \
../src/APP/IOAbstractionSWC.c 

OBJS += \
./src/APP/AR_HeadLampSWC.o \
./src/APP/IOAbstractionSWC.o 

C_DEPS += \
./src/APP/AR_HeadLampSWC.d \
./src/APP/IOAbstractionSWC.d 


# Each subdirectory must supply rules for building sources it contributes
src/APP/%.o: ../src/APP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/APP/AR_HeadLampSWC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


