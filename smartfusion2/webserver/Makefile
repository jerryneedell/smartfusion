
RM := rm -rf

# All of the sources participating in the build are defined here

ELF_SRCS := 
C_UPPER_SRCS := 
CXX_SRCS := 
C++_SRCS := 
OBJ_SRCS := 
CC_SRCS := 
ASM_SRCS := 
C_SRCS := 
CPP_SRCS := 
S_UPPER_SRCS := 
O_SRCS := 
CC_DEPS := 
C++_DEPS := 
OBJS := 
C_UPPER_DEPS := 
CXX_DEPS := 
SECONDARY_FLASH := 
SECONDARY_SIZE := 
ASM_DEPS := 
S_UPPER_DEPS := 
C_DEPS := 
CPP_DEPS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./drivers/mss_uart/mss_uart.c \
./drivers/mss_ethernet_mac/mss_ethernet_mac.c \
./drivers/mss_ethernet_mac/m88e1340_phy.c \
./drivers_config/sys_config/sys_config_SERDESIF_0.c \
./drivers_config/sys_config/sys_config.c \
./CMSIS/startup_gcc/newlib_stubs.c \
./CMSIS/core_cm3.c \
./CMSIS/system_m2sxxx.c \
./lwip-1.4.1/core/def.c \
./lwip-1.4.1/core/dhcp.c \
./lwip-1.4.1/core/dns.c \
./lwip-1.4.1/core/init.c \
./lwip-1.4.1/core/lwip_timers.c \
./lwip-1.4.1/core/mem.c \
./lwip-1.4.1/core/memp.c \
./lwip-1.4.1/core/netif.c \
./lwip-1.4.1/core/pbuf.c \
./lwip-1.4.1/core/raw.c \
./lwip-1.4.1/core/stats.c \
./lwip-1.4.1/core/sys.c \
./lwip-1.4.1/core/tcp.c \
./lwip-1.4.1/core/tcp_in.c \
./lwip-1.4.1/core/tcp_out.c \
./lwip-1.4.1/core/udp.c \
./lwip-1.4.1/core/ipv4/autoip.c \
./lwip-1.4.1/core/ipv4/icmp.c \
./lwip-1.4.1/core/ipv4/igmp.c \
./lwip-1.4.1/core/ipv4/inet.c \
./lwip-1.4.1/core/ipv4/inet_chksum.c \
./lwip-1.4.1/core/ipv4/ip.c \
./lwip-1.4.1/core/ipv4/ip_addr.c \
./lwip-1.4.1/core/ipv4/ip_frag.c \
./lwip-1.4.1/api/api_lib.c \
./lwip-1.4.1/api/api_msg.c \
./lwip-1.4.1/api/err.c \
./lwip-1.4.1/api/netbuf.c \
./lwip-1.4.1/api/netdb.c \
./lwip-1.4.1/api/netifapi.c \
./lwip-1.4.1/api/sockets.c \
./lwip-1.4.1/api/tcpip.c \
./lwip-1.4.1/netif/etharp.c \
./lwip-1.4.1/netif/ethernetif.c \
./lwip-1.4.1/netif/m2sxxx_ethernetif.c \
./lwip-1.4.1/netif/slipif.c \
./lwip-1.4.1/port/FreeRTOS/M2SXXX/sys_arch.c \
./main.c 


S_UPPER_SRCS += \
./CMSIS/startup_gcc/startup_m2sxxx.S 

OBJS += \
./drivers/mss_uart/mss_uart.o \
./drivers/mss_ethernet_mac/mss_ethernet_mac.o \
./drivers/mss_ethernet_mac/m88e1340_phy.o \
./drivers_config/sys_config/sys_config_SERDESIF_0.o \
./drivers_config/sys_config/sys_config.o \
./CMSIS/startup_gcc/newlib_stubs.o \
./CMSIS/startup_gcc/startup_m2sxxx.o \
./CMSIS/core_cm3.o \
./CMSIS/system_m2sxxx.o\
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
./lwip-1.4.1/core/udp.o \
./lwip-1.4.1/core/ipv4/autoip.o \
./lwip-1.4.1/core/ipv4/icmp.o \
./lwip-1.4.1/core/ipv4/igmp.o \
./lwip-1.4.1/core/ipv4/inet.o \
./lwip-1.4.1/core/ipv4/inet_chksum.o \
./lwip-1.4.1/core/ipv4/ip.o \
./lwip-1.4.1/core/ipv4/ip_addr.o\
./lwip-1.4.1/core/ipv4/ip_frag.o \
./lwip-1.4.1/api/api_lib.o \
./lwip-1.4.1/api/api_msg.o \
./lwip-1.4.1/api/err.o \
./lwip-1.4.1/api/netbuf.o \
./lwip-1.4.1/api/netdb.o \
./lwip-1.4.1/api/netifapi.o \
./lwip-1.4.1/api/sockets.o \
./lwip-1.4.1/api/tcpip.o \
./lwip-1.4.1/netif/etharp.o \
./lwip-1.4.1/netif/ethernetif.o \
./lwip-1.4.1/netif/m2sxxx_ethernetif.o \
./lwip-1.4.1/netif/slipif.o \
./lwip-1.4.1/port/FreeRTOS/M2SXXX/sys_arch.o \
./main.o 


S_UPPER_DEPS += \
./CMSIS/startup_gcc/startup_m2sxxx.d 


C_DEPS += \
./drivers/mss_uart/mss_uart.d \
./drivers/mss_ethernet_mac/mss_ethernet_mac.d \
./drivers/mss_ethernet_mac/m88e1340_phy.d \
./drivers_config/sys_config/sys_config_SERDESIF_0.d \
./drivers_config/sys_config/sys_config.d \
./CMSIS/startup_gcc/newlib_stubs.d \
./CMSIS/core_cm3.d \
./CMSIS/system_m2sxxx.d \
./lwip-1.4.1/core/def.d \
./lwip-1.4.1/core/dhcpd.d \
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
./lwip-1.4.1/core/udp.d \
./lwip-1.4.1/core/ipv4/autoip.d \
./lwip-1.4.1/core/ipv4/icmp.d \
./lwip-1.4.1/core/ipv4/igmp.d \
./lwip-1.4.1/core/ipv4/inet.d \
./lwip-1.4.1/core/ipv4/inet_chksum.d \
./lwip-1.4.1/core/ipv4/ip.d \
./lwip-1.4.1/core/ipv4/ip_addr.d \
./lwip-1.4.1/core/ipv4/ip_frag.d \
./lwip-1.4.1/api/api_lib.d \
./lwip-1.4.1/api/api_msg.d \
./lwip-1.4.1/api/err.d \
./lwip-1.4.1/api/netbuf.d \
./lwip-1.4.1/api/netdb.d \
./lwip-1.4.1/api/netifapi.d \
./lwip-1.4.1/api/sockets.d \
./lwip-1.4.1/api/tcpip.d \
./lwip-1.4.1/netif/etharp.d \
./lwip-1.4.1/netif/ethernetif.d \
./lwip-1.4.1/netif/m2sxxx_ethernetif.d \
./lwip-1.4.1/netif/slipif.d \
./lwip-1.4.1/port/FreeRTOS/M2SXXX/sys_arch.d \
./main.d



# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	#arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DGCC_ARMCM3 -DLWIP_COMPAT_MUTEX -g3  -I./ -I./CMSIS -I./CMSIS/startup_gcc -I./drivers/mss_ethernet_mac -I./drivers/mss_uart -I./drivers/mss_rtc -I./drivers/mss_timer -I./drivers/mss_uart -I./FreeRTOS/include -I./lwip-1.4.1 -I./lwip-1.4.1/include -I./lwip-1.4.1/include/lwip -I./lwip-1.4.1/include/ipv4 -I./lwip-1.4.1/port/FreeRTOS/M2SXXX/ -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DLWIP_COMPAT_MUTEX -DNET_USE_DHCP -DLWIP_PROVIDE_ERRNO -DMICROSEMI_STDIO_THRU_MMUART0 -I"./" -I"./CMSIS" -I"./CMSIS/startup_gcc" -I"./drivers" -I"./drivers/mss_gpio" -I"./drivers/mss_rtc" -I"./drivers/mss_spi" -I"./drivers/mss_timer" -I"./drivers/mss_uart" -I"./drivers_config" -I"./drivers_config/sys_config" -I"./FreeRTOS" -I"./FreeRTOS/include" -I"./FreeRTOS/portable" -I"./FreeRTOS/portable/GCC" -I"./FreeRTOS/portable/GCC/ARM_CM3" -I"./FreeRTOS/portable/MemMang" -I"./hal" -I"./hal/CortexM3" -I"./hal/CortexM3/GNU" -I"./lwip-1.4.1" -I"./lwip-1.4.1/api" -I"./lwip-1.4.1/core" -I"./lwip-1.4.1/core/ipv4" -I"./lwip-1.4.1/core/snmp" -I"./lwip-1.4.1/include" -I"./lwip-1.4.1/include/ipv4" -I"./lwip-1.4.1/include/ipv4/lwip" -I"./lwip-1.4.1/include/lwip" -I"./lwip-1.4.1/include/netif" -I"./lwip-1.4.1/include/posix" -I"./lwip-1.4.1/include/posix/sys" -I"./lwip-1.4.1/netif" -I"./lwip-1.4.1/netif/ppp" -I"./lwip-1.4.1/port" -I"./lwip-1.4.1/port/FreeRTOS" -I"./lwip-1.4.1/port/FreeRTOS/M2SXXX" -I"./drivers/mss_ethernet_mac" -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

	@echo 'Finished building: $<'
	@echo ' '



%.o: ./%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '




USER_OBJS :=

LIBS :=



# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
webserver.hex \

SECONDARY_SIZE += \
webserver.siz \


# All Target
all: webserver.elf secondary-outputs

# Tool invocations
webserver.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3  -T ./CMSIS/startup_gcc/debug-in-microsemi-smartfusion2-envm.ld -Xlinker --gc-sections -Wl,-Map,"webserver.map" --specs=nano.specs -o "webserver.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

webserver.hex: webserver.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "webserver.elf"  "webserver.hex"
	@echo 'Finished building: $@'
	@echo ' '

webserver.siz: webserver.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "webserver.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) webserver.elf webserver.map webserver_release.elf webserver_release.map
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

