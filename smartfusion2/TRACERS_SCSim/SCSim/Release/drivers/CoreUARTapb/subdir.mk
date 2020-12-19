################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/CoreUARTapb/core_uart_apb.c 

OBJS += \
./drivers/CoreUARTapb/core_uart_apb.o 

C_DEPS += \
./drivers/CoreUARTapb/core_uart_apb.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/CoreUARTapb/%.o: ../drivers/CoreUARTapb/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fsigned-char -fmessage-length=0 -ffunction-sections -fdata-sections  -g  -DLWIP_COMPAT_MUTEX   -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I".." -I"../CMSIS" -I"../CMSIS/startup_gcc" -I"../hal/" -I"../hal/CortexM3" -I"../hal/CortexM3/GNU"   -I"../drivers/CoreUARTapb" -I"../drivers_config" -I"../drivers_config/sys_config"  -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


