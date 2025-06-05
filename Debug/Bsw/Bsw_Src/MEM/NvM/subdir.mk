################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/MEM/NvM/NvM.c \
../Bsw/Bsw_Src/MEM/NvM/NvM_Inter.c 

OBJS += \
./Bsw/Bsw_Src/MEM/NvM/NvM.o \
./Bsw/Bsw_Src/MEM/NvM/NvM_Inter.o 

C_DEPS += \
./Bsw/Bsw_Src/MEM/NvM/NvM.d \
./Bsw/Bsw_Src/MEM/NvM/NvM_Inter.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/MEM/NvM/%.o: ../Bsw/Bsw_Src/MEM/NvM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/MEM/NvM/NvM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


