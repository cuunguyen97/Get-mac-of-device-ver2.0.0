################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Middle/GUI/GUI.c 

OBJS += \
./App/Middle/GUI/GUI.o 

C_DEPS += \
./App/Middle/GUI/GUI.d 


# Each subdirectory must supply rules for building sources it contributes
App/Middle/GUI/%.o App/Middle/GUI/%.su: ../App/Middle/GUI/%.c App/Middle/GUI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/qr-code-to-lcd" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/menu" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/rtos" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/button" -I../Inc -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/Utilities" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/serial-uart" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/SYSTEM/delay" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/SYSTEM/sys" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Driver/CMSIS/Include" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Driver/STM32F401RE_StdPeriph_Driver/inc" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/GUI" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/LCD" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/qr-code" -I"C:/Users/Trang/STM32CubeIDE/workspace_1.10.1/get_mac_of_device_ver1.1/App/Middle/SPI" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Middle-2f-GUI

clean-App-2f-Middle-2f-GUI:
	-$(RM) ./App/Middle/GUI/GUI.d ./App/Middle/GUI/GUI.o ./App/Middle/GUI/GUI.su

.PHONY: clean-App-2f-Middle-2f-GUI

