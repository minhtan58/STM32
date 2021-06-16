################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/ResourcesManager_SharedResources/CM7/Src/main.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/ResourcesManager_SharedResources/CM7/Src/stm32h7xx_it.c 

OBJS += \
./Application/User/CM7/main.o \
./Application/User/CM7/stm32h7xx_it.o 

C_DEPS += \
./Application/User/CM7/main.d \
./Application/User/CM7/stm32h7xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/CM7/main.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/ResourcesManager_SharedResources/CM7/Src/main.c Application/User/CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DUSE_STM32H747I_DISCOVERY -c -I../../../CM7/Inc -I../../../Common/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../../../../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32H747I-DISCO -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Utilities/Fonts -I../../../../../../../../Utilities/CPU -I../../../../../../../../Utilities/ResourcesManager -I../../../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../../../../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/CM7/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/CM7/stm32h7xx_it.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/ResourcesManager_SharedResources/CM7/Src/stm32h7xx_it.c Application/User/CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DUSE_STM32H747I_DISCOVERY -c -I../../../CM7/Inc -I../../../Common/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../../../../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32H747I-DISCO -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Utilities/Fonts -I../../../../../../../../Utilities/CPU -I../../../../../../../../Utilities/ResourcesManager -I../../../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../../../../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/CM7/stm32h7xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

