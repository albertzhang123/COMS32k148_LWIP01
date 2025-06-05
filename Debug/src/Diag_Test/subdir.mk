################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Diag_Test/Dcm_Callout.c \
../src/Diag_Test/Rte_Dcm.c \
../src/Diag_Test/Rte_Dem.c 

OBJS += \
./src/Diag_Test/Dcm_Callout.o \
./src/Diag_Test/Rte_Dcm.o \
./src/Diag_Test/Rte_Dem.o 

C_DEPS += \
./src/Diag_Test/Dcm_Callout.d \
./src/Diag_Test/Rte_Dcm.d \
./src/Diag_Test/Rte_Dem.d 


# Each subdirectory must supply rules for building sources it contributes
src/Diag_Test/%.o: ../src/Diag_Test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Diag_Test/Dcm_Callout.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


