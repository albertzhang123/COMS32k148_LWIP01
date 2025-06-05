################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/RTE/Rte.c \
../Bsw/Bsw_Src/RTE/Rte_AR_HeadLampSWC.c \
../Bsw/Bsw_Src/RTE/Rte_COMCbk.c \
../Bsw/Bsw_Src/RTE/Rte_Cbk.c \
../Bsw/Bsw_Src/RTE/Rte_Data.c \
../Bsw/Bsw_Src/RTE/Rte_IOAbstractionSWC.c 

OBJS += \
./Bsw/Bsw_Src/RTE/Rte.o \
./Bsw/Bsw_Src/RTE/Rte_AR_HeadLampSWC.o \
./Bsw/Bsw_Src/RTE/Rte_COMCbk.o \
./Bsw/Bsw_Src/RTE/Rte_Cbk.o \
./Bsw/Bsw_Src/RTE/Rte_Data.o \
./Bsw/Bsw_Src/RTE/Rte_IOAbstractionSWC.o 

C_DEPS += \
./Bsw/Bsw_Src/RTE/Rte.d \
./Bsw/Bsw_Src/RTE/Rte_AR_HeadLampSWC.d \
./Bsw/Bsw_Src/RTE/Rte_COMCbk.d \
./Bsw/Bsw_Src/RTE/Rte_Cbk.d \
./Bsw/Bsw_Src/RTE/Rte_Data.d \
./Bsw/Bsw_Src/RTE/Rte_IOAbstractionSWC.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/RTE/%.o: ../Bsw/Bsw_Src/RTE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/RTE/Rte.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


