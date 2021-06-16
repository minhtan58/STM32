################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Internet/DHCP/dhcp.c 

OBJS += \
./Core/Internet/DHCP/dhcp.o 

C_DEPS += \
./Core/Internet/DHCP/dhcp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Internet/DHCP/dhcp.o: ../Core/Internet/DHCP/dhcp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/DHCP/dhcp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

