################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/RTE/staticFlie/Rte_COMCbk.c \
../Bsw/Bsw_Src/RTE/staticFlie/Rte_SchM.c 

OBJS += \
./Bsw/Bsw_Src/RTE/staticFlie/Rte_COMCbk.o \
./Bsw/Bsw_Src/RTE/staticFlie/Rte_SchM.o 

C_DEPS += \
./Bsw/Bsw_Src/RTE/staticFlie/Rte_COMCbk.d \
./Bsw/Bsw_Src/RTE/staticFlie/Rte_SchM.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/RTE/staticFlie/%.o: ../Bsw/Bsw_Src/RTE/staticFlie/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/RTE/staticFlie/Rte_COMCbk.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


