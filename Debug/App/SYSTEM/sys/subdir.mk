################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/SYSTEM/sys/sys.c 

OBJS += \
./App/SYSTEM/sys/sys.o 

C_DEPS += \
./App/SYSTEM/sys/sys.d 


# Each subdirectory must supply rules for building sources it contributes
App/SYSTEM/sys/%.o App/SYSTEM/sys/%.su: ../App/SYSTEM/sys/%.c App/SYSTEM/sys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/qr-code-to-lcd" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/menu" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/rtos" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/button" -I../Inc -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/Utilities" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/serial-uart" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/SYSTEM/delay" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/SYSTEM/sys" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Driver/CMSIS/Include" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Driver/STM32F401RE_StdPeriph_Driver/inc" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/GUI" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/LCD" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/qr-code" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/SPI" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-SYSTEM-2f-sys

clean-App-2f-SYSTEM-2f-sys:
	-$(RM) ./App/SYSTEM/sys/sys.d ./App/SYSTEM/sys/sys.o ./App/SYSTEM/sys/sys.su

.PHONY: clean-App-2f-SYSTEM-2f-sys

