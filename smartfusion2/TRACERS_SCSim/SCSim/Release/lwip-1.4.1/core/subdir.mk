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
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DLWIP_COMPAT_MUTEX -DNET_USE_DHCP -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/CMSIS" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/CMSIS/startup_gcc" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_gpio" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_rtc" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_spi" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_timer" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_uart" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers_config" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers_config/sys_config" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/include" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable/GCC" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable/GCC/ARM_CM3" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable/MemMang" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/hal" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/hal/CortexM3" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/hal/CortexM3/GNU" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/api" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/core" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/core/ipv4" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/core/snmp" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/ipv4" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/ipv4/lwip" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/lwip" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/netif" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/posix" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/posix/sys" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/netif" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/netif/ppp" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/port" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/port/FreeRTOS" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_ethernet_mac" -std=gnu11 --specs=cmsis.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


