
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

# Every subdirectory with source files must be described here
#SUBDIRS := \
#CMSIS \
#CMSIS/startup_gcc \
#drivers/mss_uart \
#drivers/mss_ethernet_mac \
#. \



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
./main.d


# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3  -I./ -I./CMSIS -I./CMSIS/startup_gcc -I./drivers/mss_ethernet_mac  -I./drivers/mss_uart -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
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
ethernet_test.hex \

SECONDARY_SIZE += \
ethernet_test.siz \


# All Target
all: ethernet_test.elf secondary-outputs

# Tool invocations
ethernet_test.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T .//CMSIS/startup_gcc/debug-in-microsemi-smartfusion2-envm.ld -Xlinker --gc-sections -Wl,-Map,"ethernet_test.map" --specs=nano.specs -o "ethernet_test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ethernet_test.hex: ethernet_test.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ethernet_test.elf"  "ethernet_test.hex"
	@echo 'Finished building: $@'
	@echo ' '

ethernet_test.siz: ethernet_test.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "ethernet_test.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) ethernet_test.elf ethernet_test.map ethernet_test_release.elf ethernet_test_release.map
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

