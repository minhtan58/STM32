################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Internet/httpServer/httpParser.c \
../Core/Internet/httpServer/httpServer.c \
../Core/Internet/httpServer/httpUtil.c 

OBJS += \
./Core/Internet/httpServer/httpParser.o \
./Core/Internet/httpServer/httpServer.o \
./Core/Internet/httpServer/httpUtil.o 

C_DEPS += \
./Core/Internet/httpServer/httpParser.d \
./Core/Internet/httpServer/httpServer.d \
./Core/Internet/httpServer/httpUtil.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Internet/httpServer/httpParser.o: ../Core/Internet/httpServer/httpParser.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/httpServer/httpParser.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/httpServer/httpServer.o: ../Core/Internet/httpServer/httpServer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/httpServer/httpServer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/httpServer/httpUtil.o: ../Core/Internet/httpServer/httpUtil.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/httpServer/httpUtil.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

