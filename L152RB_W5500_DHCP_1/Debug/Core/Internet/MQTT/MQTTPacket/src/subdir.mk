################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTFormat.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTPacket.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c \
../Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c 

OBJS += \
./Core/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTFormat.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTPacket.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o \
./Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o 

C_DEPS += \
./Core/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTFormat.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTPacket.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d \
./Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTFormat.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTFormat.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTFormat.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTPacket.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTPacket.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTPacket.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o: ../Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L152xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../Ethernet/W5500 -I../Core/Ethernet -I../Core/Internet -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

