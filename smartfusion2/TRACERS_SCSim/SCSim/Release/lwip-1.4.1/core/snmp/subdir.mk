################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-1.4.1/core/snmp/asn1_dec.c \
../lwip-1.4.1/core/snmp/asn1_enc.c \
../lwip-1.4.1/core/snmp/mib2.c \
../lwip-1.4.1/core/snmp/mib_structs.c \
../lwip-1.4.1/core/snmp/msg_in.c \
../lwip-1.4.1/core/snmp/msg_out.c 

OBJS += \
./lwip-1.4.1/core/snmp/asn1_dec.o \
./lwip-1.4.1/core/snmp/asn1_enc.o \
./lwip-1.4.1/core/snmp/mib2.o \
./lwip-1.4.1/core/snmp/mib_structs.o \
./lwip-1.4.1/core/snmp/msg_in.o \
./lwip-1.4.1/core/snmp/msg_out.o 

C_DEPS += \
./lwip-1.4.1/core/snmp/asn1_dec.d \
./lwip-1.4.1/core/snmp/asn1_enc.d \
./lwip-1.4.1/core/snmp/mib2.d \
./lwip-1.4.1/core/snmp/mib_structs.d \
./lwip-1.4.1/core/snmp/msg_in.d \
./lwip-1.4.1/core/snmp/msg_out.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-1.4.1/core/snmp/%.o: ../lwip-1.4.1/core/snmp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fsigned-char -fmessage-length=0 -ffunction-sections -fdata-sections  -g  -DLWIP_COMPAT_MUTEX   -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I".." -I"../CMSIS" -I"../CMSIS/startup_gcc" -I"../drivers" -I"../drivers/mss_gpio" -I"../drivers/mss_rtc" -I"../drivers/mss_spi" -I"../drivers/mss_timer" -I"../drivers/mss_uart" -I"../drivers_config" -I"../drivers_config/sys_config" -I"../FreeRTOS" -I"../FreeRTOS/include" -I"../FreeRTOS/portable" -I"../FreeRTOS/portable/GCC" -I"../FreeRTOS/portable/GCC/ARM_CM3" -I"../FreeRTOS/portable/MemMang"   -I"../lwip-1.4.1" -I"../lwip-1.4.1/api" -I"../lwip-1.4.1/core" -I"../lwip-1.4.1/core/ipv4" -I"../lwip-1.4.1/core/snmp" -I"../lwip-1.4.1/include" -I"../lwip-1.4.1/include/ipv4" -I"../lwip-1.4.1/include/ipv4/lwip" -I"../lwip-1.4.1/include/lwip" -I"../lwip-1.4.1/include/netif" -I"../lwip-1.4.1/include/posix" -I"../lwip-1.4.1/include/posix/sys" -I"../lwip-1.4.1/netif" -I"../lwip-1.4.1/netif/ppp" -I"../lwip-1.4.1/port" -I"../lwip-1.4.1/port/FreeRTOS" -I"../lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"../drivers/mss_ethernet_mac" -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


