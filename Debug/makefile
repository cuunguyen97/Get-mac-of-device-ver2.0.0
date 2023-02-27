################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include App/SYSTEM/sys/subdir.mk
-include App/SYSTEM/delay/subdir.mk
-include App/Middle/serial-uart/subdir.mk
-include App/Middle/rtos/subdir.mk
-include App/Middle/qr-code-to-lcd/subdir.mk
-include App/Middle/qr-code/subdir.mk
-include App/Middle/menu/subdir.mk
-include App/Middle/button/subdir.mk
-include App/Middle/Utilities/subdir.mk
-include App/Middle/SPI/subdir.mk
-include App/Middle/LCD/subdir.mk
-include App/Middle/GUI/subdir.mk
-include App/Driver/STM32F401RE_StdPeriph_Driver/src/subdir.mk
-include App/Driver/CMSIS/Source/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := get_mac_of_device_ver1.1
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
get_mac_of_device_ver1.1.elf \

MAP_FILES += \
get_mac_of_device_ver1.1.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
get_mac_of_device_ver1.1.list \


# All Target
all: main-build

# Main-build Target
main-build: get_mac_of_device_ver1.1.elf secondary-outputs

# Tool invocations
get_mac_of_device_ver1.1.elf get_mac_of_device_ver1.1.map: $(OBJS) $(USER_OBJS) C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\STM32F401RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "get_mac_of_device_ver1.1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="get_mac_of_device_ver1.1.map" -Wl,--gc-sections -static -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\Driver\CMSIS\Include" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\Driver\STM32F401RE_StdPeriph_Driver\inc" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\Middle\GUI" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\Middle\LCD" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\Middle\qr-code" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\Middle\SPI" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\SYSTEM\delay" -L"C:\Users\Trang\STM32CubeIDE\workspace_1.10.1\get_mac_of_device_ver1.1\App\SYSTEM\sys" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

get_mac_of_device_ver1.1.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "get_mac_of_device_ver1.1.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout get_mac_of_device_ver1.1.elf get_mac_of_device_ver1.1.list get_mac_of_device_ver1.1.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
