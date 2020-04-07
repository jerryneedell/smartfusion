################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/brownout_isr.c \
../CMSIS/core_cm3.c \
../CMSIS/system_a2fxxxm3.c 

OBJS += \
./CMSIS/brownout_isr.o \
./CMSIS/core_cm3.o \
./CMSIS/system_a2fxxxm3.o 

C_DEPS += \
./CMSIS/brownout_isr.d \
./CMSIS/core_cm3.d \
./CMSIS/system_a2fxxxm3.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DACTEL_STDIO_THRU_UART -I../ -I../CMSIS -I../CMSIS/startup_gcc -I../drivers/mss_gpio -I../drivers/mss_watchdog -I../drivers/mss_uart -std=gnu11 --specs=cmsis.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


