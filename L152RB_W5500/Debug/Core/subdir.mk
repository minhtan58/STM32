################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/socket.c \
../Core/wizchip_conf.c 

OBJS += \
./Core/socket.o \
./Core/wizchip_conf.o 

C_DEPS += \
./Core/socket.d \
./Core/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/socket.o: ../Core/socket.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Core -I../Core/W5500 -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/socket.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/wizchip_conf.o: ../Core/wizchip_conf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Core -I../Core/W5500 -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/wizchip_conf.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

