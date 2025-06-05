################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x01.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x02.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x03.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x04.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x05.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x06.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x07.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x08.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x09.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x0A.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBDInternal.c 

OBJS += \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x01.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x02.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x03.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x04.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x05.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x06.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x07.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x08.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x09.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x0A.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBDInternal.o 

C_DEPS += \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x01.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x02.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x03.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x04.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x05.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x06.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x07.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x08.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x09.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x0A.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBDInternal.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/%.o: ../Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/DIAG/Dcm/DcmDsp/OBD/Dcm_OBD0x01.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


