################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/loopback/loopback.c 

OBJS += \
./Core/loopback/loopback.o 

C_DEPS += \
./Core/loopback/loopback.d 


# Each subdirectory must supply rules for building sources it contributes
Core/loopback/loopback.o: ../Core/loopback/loopback.c Core/loopback/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I../Core -I../Core/loopback -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/loopback/loopback.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

