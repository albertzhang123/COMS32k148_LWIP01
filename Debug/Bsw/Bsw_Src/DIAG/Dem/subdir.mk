################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/DIAG/Dem/Dem.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_Dcm.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_EventDebounce.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_EventMemory.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_EventQueue.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_ExtendedData.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_FreezeFrame.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_Internal.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_J1939.c \
../Bsw/Bsw_Src/DIAG/Dem/Dem_OBD.c 

OBJS += \
./Bsw/Bsw_Src/DIAG/Dem/Dem.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_Dcm.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_EventDebounce.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_EventMemory.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_EventQueue.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_ExtendedData.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_FreezeFrame.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_Internal.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_J1939.o \
./Bsw/Bsw_Src/DIAG/Dem/Dem_OBD.o 

C_DEPS += \
./Bsw/Bsw_Src/DIAG/Dem/Dem.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_Dcm.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_EventDebounce.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_EventMemory.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_EventQueue.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_ExtendedData.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_FreezeFrame.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_Internal.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_J1939.d \
./Bsw/Bsw_Src/DIAG/Dem/Dem_OBD.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/DIAG/Dem/%.o: ../Bsw/Bsw_Src/DIAG/Dem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/DIAG/Dem/Dem.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


