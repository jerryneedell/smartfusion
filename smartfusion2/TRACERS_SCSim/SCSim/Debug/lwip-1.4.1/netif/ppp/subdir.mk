################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-1.4.1/netif/ppp/auth.c \
../lwip-1.4.1/netif/ppp/chap.c \
../lwip-1.4.1/netif/ppp/chpms.c \
../lwip-1.4.1/netif/ppp/fsm.c \
../lwip-1.4.1/netif/ppp/ipcp.c \
../lwip-1.4.1/netif/ppp/lcp.c \
../lwip-1.4.1/netif/ppp/magic.c \
../lwip-1.4.1/netif/ppp/md5.c \
../lwip-1.4.1/netif/ppp/pap.c \
../lwip-1.4.1/netif/ppp/ppp.c \
../lwip-1.4.1/netif/ppp/ppp_oe.c \
../lwip-1.4.1/netif/ppp/randm.c \
../lwip-1.4.1/netif/ppp/vj.c 

OBJS += \
./lwip-1.4.1/netif/ppp/auth.o \
./lwip-1.4.1/netif/ppp/chap.o \
./lwip-1.4.1/netif/ppp/chpms.o \
./lwip-1.4.1/netif/ppp/fsm.o \
./lwip-1.4.1/netif/ppp/ipcp.o \
./lwip-1.4.1/netif/ppp/lcp.o \
./lwip-1.4.1/netif/ppp/magic.o \
./lwip-1.4.1/netif/ppp/md5.o \
./lwip-1.4.1/netif/ppp/pap.o \
./lwip-1.4.1/netif/ppp/ppp.o \
./lwip-1.4.1/netif/ppp/ppp_oe.o \
./lwip-1.4.1/netif/ppp/randm.o \
./lwip-1.4.1/netif/ppp/vj.o 

C_DEPS += \
./lwip-1.4.1/netif/ppp/auth.d \
./lwip-1.4.1/netif/ppp/chap.d \
./lwip-1.4.1/netif/ppp/chpms.d \
./lwip-1.4.1/netif/ppp/fsm.d \
./lwip-1.4.1/netif/ppp/ipcp.d \
./lwip-1.4.1/netif/ppp/lcp.d \
./lwip-1.4.1/netif/ppp/magic.d \
./lwip-1.4.1/netif/ppp/md5.d \
./lwip-1.4.1/netif/ppp/pap.d \
./lwip-1.4.1/netif/ppp/ppp.d \
./lwip-1.4.1/netif/ppp/ppp_oe.d \
./lwip-1.4.1/netif/ppp/randm.d \
./lwip-1.4.1/netif/ppp/vj.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-1.4.1/netif/ppp/%.o: ../lwip-1.4.1/netif/ppp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DLWIP_COMPAT_MUTEX -DNET_USE_DHCP -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I".." -I"../CMSIS" -I"../CMSIS/startup_gcc" -I"../drivers" -I"../drivers/mss_gpio" -I"../drivers/mss_rtc" -I"../drivers/mss_spi" -I"../drivers/mss_timer" -I"../drivers/mss_uart" -I"../drivers_config" -I"../drivers_config/sys_config" -I"../FreeRTOS" -I"../FreeRTOS/include" -I"../FreeRTOS/portable" -I"../FreeRTOS/portable/GCC" -I"../FreeRTOS/portable/GCC/ARM_CM3" -I"../FreeRTOS/portable/MemMang" -I"../hal" -I"../hal/CortexM3" -I"../hal/CortexM3/GNU" -I"../lwip-1.4.1" -I"../lwip-1.4.1/api" -I"../lwip-1.4.1/core" -I"../lwip-1.4.1/core/ipv4" -I"../lwip-1.4.1/core/snmp" -I"../lwip-1.4.1/include" -I"../lwip-1.4.1/include/ipv4" -I"../lwip-1.4.1/include/ipv4/lwip" -I"../lwip-1.4.1/include/lwip" -I"../lwip-1.4.1/include/netif" -I"../lwip-1.4.1/include/posix" -I"../lwip-1.4.1/include/posix/sys" -I"../lwip-1.4.1/netif" -I"../lwip-1.4.1/netif/ppp" -I"../lwip-1.4.1/port" -I"../lwip-1.4.1/port/FreeRTOS" -I"../lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"../drivers/mss_ethernet_mac" -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


