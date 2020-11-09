
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
SUBDIRS := \
CMSIS \
CMSIS/startup_gcc \
drivers/mss_uart \
. \



# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./drivers/mss_uart/mss_uart.c 

OBJS += \
./drivers/mss_uart/mss_uart.o 

C_DEPS += \
./drivers/mss_uart/mss_uart.d 


# Each subdirectory must supply rules for building sources it contributes
./drivers/mss_uart/%.o: ./drivers/mss_uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DACTEL_STDIO_THRU_UART -I./ -I./CMSIS -I./CMSIS/startup_gcc -I./drivers/mss_watchdog -I./drivers/mss_uart -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./CMSIS/startup_gcc/newlib_stubs.c 

S_UPPER_SRCS += \
./CMSIS/startup_gcc/startup_a2fxxxm3.S 

OBJS += \
./CMSIS/startup_gcc/newlib_stubs.o \
./CMSIS/startup_gcc/startup_a2fxxxm3.o 

S_UPPER_DEPS += \
./CMSIS/startup_gcc/startup_a2fxxxm3.d 

C_DEPS += \
./CMSIS/startup_gcc/newlib_stubs.d 


# Each subdirectory must supply rules for building sources it contributes
./CMSIS/startup_gcc/%.o: ./CMSIS/startup_gcc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DACTEL_STDIO_THRU_UART -I./ -I./CMSIS -I./CMSIS/startup_gcc  -I./drivers/mss_watchdog -I./drivers/mss_uart -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

./CMSIS/startup_gcc/%.o: ./CMSIS/startup_gcc/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./CMSIS/brownout_isr.c \
./CMSIS/core_cm3.c \
./CMSIS/system_a2fxxxm3.c 

OBJS += \
./CMSIS/brownout_isr.o \
./CMSIS/core_cm3.o \
./CMSIS/system_a2fxxxm3.o 

C_DEPS += \
./CMSIS/brownout_isr.d \
./CMSIS/core_cm3.d \
./CMSIS/system_a2fxxxm3.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ./CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DACTEL_STDIO_THRU_UART -I./ -I./CMSIS -I./CMSIS/startup_gcc -I./drivers/mss_watchdog -I./drivers/mss_uart -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DACTEL_STDIO_THRU_UART -I./ -I./CMSIS -I./CMSIS/startup_gcc  -I./drivers/mss_watchdog -I./drivers/mss_uart -std=gnu11 --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '




USER_OBJS :=

LIBS :=



# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
networktest.hex \

SECONDARY_SIZE += \
networktest.siz \


# All Target
all: networktest.elf secondary-outputs

# Tool invocations
networktest.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T .//CMSIS/startup_gcc/debug-in-actel-smartfusion-envm.ld -Xlinker --gc-sections -Wl,-Map,"networktest.map" --specs=nano.specs -o "networktest.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

networktest.hex: networktest.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "networktest.elf"  "networktest.hex"
	@echo 'Finished building: $@'
	@echo ' '

networktest.siz: networktest.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "networktest.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) networktest.elf networktest.map networktest_release.elf networktest_release.map
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

