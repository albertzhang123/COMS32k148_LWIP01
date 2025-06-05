################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_CommManage.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_MsgManage.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_PendingManage.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_ProtocolManage.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_SecurityManage.c \
../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_SessionManage.c 

OBJS += \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_CommManage.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_MsgManage.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_PendingManage.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_ProtocolManage.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_SecurityManage.o \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_SessionManage.o 

C_DEPS += \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_CommManage.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_MsgManage.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_PendingManage.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_ProtocolManage.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_SecurityManage.d \
./Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_SessionManage.d 


# Each subdirectory must supply rules for building sources it contributes
Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/%.o: ../Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Bsw/Bsw_Src/DIAG/Dcm/DcmDsl/DcmDsl_CommManage.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


