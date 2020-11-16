################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-1.4.1/api/api_lib.c \
../lwip-1.4.1/api/api_msg.c \
../lwip-1.4.1/api/err.c \
../lwip-1.4.1/api/netbuf.c \
../lwip-1.4.1/api/netdb.c \
../lwip-1.4.1/api/netifapi.c \
../lwip-1.4.1/api/sockets.c \
../lwip-1.4.1/api/tcpip.c 

OBJS += \
./lwip-1.4.1/api/api_lib.o \
./lwip-1.4.1/api/api_msg.o \
./lwip-1.4.1/api/err.o \
./lwip-1.4.1/api/netbuf.o \
./lwip-1.4.1/api/netdb.o \
./lwip-1.4.1/api/netifapi.o \
./lwip-1.4.1/api/sockets.o \
./lwip-1.4.1/api/tcpip.o 

C_DEPS += \
./lwip-1.4.1/api/api_lib.d \
./lwip-1.4.1/api/api_msg.d \
./lwip-1.4.1/api/err.d \
./lwip-1.4.1/api/netbuf.d \
./lwip-1.4.1/api/netdb.d \
./lwip-1.4.1/api/netifapi.d \
./lwip-1.4.1/api/sockets.d \
./lwip-1.4.1/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-1.4.1/api/%.o: ../lwip-1.4.1/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DLWIP_COMPAT_MUTEX -DNET_USE_DHCP -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I".." -I"../CMSIS" -I"../CMSIS/startup_gcc" -I"../drivers" -I"../drivers/mss_gpio" -I"../drivers/mss_rtc" -I"../drivers/mss_spi" -I"../drivers/mss_timer" -I"../drivers/mss_uart" -I"../drivers_config" -I"../drivers_config/sys_config" -I"../FreeRTOS" -I"../FreeRTOS/include" -I"../FreeRTOS/portable" -I"../FreeRTOS/portable/GCC" -I"../FreeRTOS/portable/GCC/ARM_CM3" -I"../FreeRTOS/portable/MemMang" -I"../hal" -I"../hal/CortexM3" -I"../hal/CortexM3/GNU" -I"../lwip-1.4.1" -I"../lwip-1.4.1/api" -I"../lwip-1.4.1/core" -I"../lwip-1.4.1/core/ipv4" -I"../lwip-1.4.1/core/snmp" -I"../lwip-1.4.1/include" -I"../lwip-1.4.1/include/ipv4" -I"../lwip-1.4.1/include/ipv4/lwip" -I"../lwip-1.4.1/include/lwip" -I"../lwip-1.4.1/include/netif" -I"../lwip-1.4.1/include/posix" -I"../lwip-1.4.1/include/posix/sys" -I"../lwip-1.4.1/netif" -I"../lwip-1.4.1/netif/ppp" -I"../lwip-1.4.1/port" -I"../lwip-1.4.1/port/FreeRTOS" -I"../lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"../drivers/mss_ethernet_mac" -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


