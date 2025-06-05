################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/DIAG/CanTp/CanTp.c \
../Bsw/Bsw_Src/DIAG/CanTp/CanTp_Cbk.c \
../Bsw/Bsw_Src/DIAG/CanTp/CanTp_RX.c \
../Bsw/Bsw_Src/DIAG/CanTp/CanTp_TX.c 

OBJS += \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp.o \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp_Cbk.o \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp_RX.o \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp_TX.o 

C_DEPS += \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp.d \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp_Cbk.d \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp_RX.d \
./Bsw/Bsw_Src/DIAG/CanTp/CanTp_TX.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/DIAG/CanTp/%.o: ../Bsw/Bsw_Src/DIAG/CanTp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/DIAG/CanTp/CanTp.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


