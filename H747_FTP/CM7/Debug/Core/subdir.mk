################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ftpc.c \
../Core/socket.c \
../Core/uart.c \
../Core/w5500.c \
../Core/wizchip_conf.c 

OBJS += \
./Core/ftpc.o \
./Core/socket.o \
./Core/uart.o \
./Core/w5500.o \
./Core/wizchip_conf.o 

C_DEPS += \
./Core/ftpc.d \
./Core/socket.d \
./Core/uart.d \
./Core/w5500.d \
./Core/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ftpc.o: ../Core/ftpc.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../Core -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/ftpc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/socket.o: ../Core/socket.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../Core -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/socket.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/uart.o: ../Core/uart.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../Core -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/w5500.o: ../Core/w5500.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../Core -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/w5500.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/wizchip_conf.o: ../Core/wizchip_conf.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../Core -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/wizchip_conf.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

