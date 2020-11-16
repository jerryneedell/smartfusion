################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/startup_gcc/newlib_stubs.c 

S_UPPER_SRCS += \
../CMSIS/startup_gcc/startup_m2sxxx.S 

OBJS += \
./CMSIS/startup_gcc/newlib_stubs.o \
./CMSIS/startup_gcc/startup_m2sxxx.o 

S_UPPER_DEPS += \
./CMSIS/startup_gcc/startup_m2sxxx.d 

C_DEPS += \
./CMSIS/startup_gcc/newlib_stubs.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/startup_gcc/%.o: ../CMSIS/startup_gcc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DLWIP_COMPAT_MUTEX -DNET_USE_DHCP -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/CMSIS" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/CMSIS/startup_gcc" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_gpio" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_rtc" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_spi" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_timer" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_uart" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers_config" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers_config/sys_config" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/include" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable/GCC" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable/GCC/ARM_CM3" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/FreeRTOS/portable/MemMang" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/hal" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/hal/CortexM3" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/hal/CortexM3/GNU" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/api" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/core" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/core/ipv4" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/core/snmp" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/ipv4" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/ipv4/lwip" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/lwip" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/netif" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/posix" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/include/posix/sys" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/netif" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/netif/ppp" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/port" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/port/FreeRTOS" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"/home/jerryneedell/projects/TRACERS/SF2_webserver_softConsole/SoftConsole_tcp_test/Webserver/drivers/mss_ethernet_mac" -std=gnu11 --specs=cmsis.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/startup_gcc/%.o: ../CMSIS/startup_gcc/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


