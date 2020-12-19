################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-1.4.1/core/def.c \
../lwip-1.4.1/core/dhcp.c \
../lwip-1.4.1/core/dns.c \
../lwip-1.4.1/core/init.c \
../lwip-1.4.1/core/lwip_timers.c \
../lwip-1.4.1/core/mem.c \
../lwip-1.4.1/core/memp.c \
../lwip-1.4.1/core/netif.c \
../lwip-1.4.1/core/pbuf.c \
../lwip-1.4.1/core/raw.c \
../lwip-1.4.1/core/stats.c \
../lwip-1.4.1/core/sys.c \
../lwip-1.4.1/core/tcp.c \
../lwip-1.4.1/core/tcp_in.c \
../lwip-1.4.1/core/tcp_out.c \
../lwip-1.4.1/core/udp.c 

OBJS += \
./lwip-1.4.1/core/def.o \
./lwip-1.4.1/core/dhcp.o \
./lwip-1.4.1/core/dns.o \
./lwip-1.4.1/core/init.o \
./lwip-1.4.1/core/lwip_timers.o \
./lwip-1.4.1/core/mem.o \
./lwip-1.4.1/core/memp.o \
./lwip-1.4.1/core/netif.o \
./lwip-1.4.1/core/pbuf.o \
./lwip-1.4.1/core/raw.o \
./lwip-1.4.1/core/stats.o \
./lwip-1.4.1/core/sys.o \
./lwip-1.4.1/core/tcp.o \
./lwip-1.4.1/core/tcp_in.o \
./lwip-1.4.1/core/tcp_out.o \
./lwip-1.4.1/core/udp.o 

C_DEPS += \
./lwip-1.4.1/core/def.d \
./lwip-1.4.1/core/dhcp.d \
./lwip-1.4.1/core/dns.d \
./lwip-1.4.1/core/init.d \
./lwip-1.4.1/core/lwip_timers.d \
./lwip-1.4.1/core/mem.d \
./lwip-1.4.1/core/memp.d \
./lwip-1.4.1/core/netif.d \
./lwip-1.4.1/core/pbuf.d \
./lwip-1.4.1/core/raw.d \
./lwip-1.4.1/core/stats.d \
./lwip-1.4.1/core/sys.d \
./lwip-1.4.1/core/tcp.d \
./lwip-1.4.1/core/tcp_in.d \
./lwip-1.4.1/core/tcp_out.d \
./lwip-1.4.1/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-1.4.1/core/%.o: ../lwip-1.4.1/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fsigned-char -fmessage-length=0 -ffunction-sections -fdata-sections  -g  -DLWIP_COMPAT_MUTEX   -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I".." -I"../CMSIS" -I"../CMSIS/startup_gcc" -I"../drivers" -I"../drivers/mss_gpio" -I"../drivers/mss_rtc" -I"../drivers/mss_spi" -I"../drivers/mss_timer" -I"../drivers/mss_uart" -I"../drivers_config" -I"../drivers_config/sys_config" -I"../FreeRTOS" -I"../FreeRTOS/include" -I"../FreeRTOS/portable" -I"../FreeRTOS/portable/GCC" -I"../FreeRTOS/portable/GCC/ARM_CM3" -I"../FreeRTOS/portable/MemMang"   -I"../lwip-1.4.1" -I"../lwip-1.4.1/api" -I"../lwip-1.4.1/core" -I"../lwip-1.4.1/core/ipv4" -I"../lwip-1.4.1/core/snmp" -I"../lwip-1.4.1/include" -I"../lwip-1.4.1/include/ipv4" -I"../lwip-1.4.1/include/ipv4/lwip" -I"../lwip-1.4.1/include/lwip" -I"../lwip-1.4.1/include/netif" -I"../lwip-1.4.1/include/posix" -I"../lwip-1.4.1/include/posix/sys" -I"../lwip-1.4.1/netif" -I"../lwip-1.4.1/netif/ppp" -I"../lwip-1.4.1/port" -I"../lwip-1.4.1/port/FreeRTOS" -I"../lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"../drivers/mss_ethernet_mac" -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


