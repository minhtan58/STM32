################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/diskio.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/ff.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/option/syscall.c 

OBJS += \
./Middlewares/Third_Party/FatFs/diskio.o \
./Middlewares/Third_Party/FatFs/ff.o \
./Middlewares/Third_Party/FatFs/ff_gen_drv.o \
./Middlewares/Third_Party/FatFs/syscall.o 

C_DEPS += \
./Middlewares/Third_Party/FatFs/diskio.d \
./Middlewares/Third_Party/FatFs/ff.d \
./Middlewares/Third_Party/FatFs/ff_gen_drv.d \
./Middlewares/Third_Party/FatFs/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FatFs/diskio.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/diskio.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FatFs/diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/ff.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/ff.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FatFs/ff.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/ff_gen_drv.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FatFs/ff_gen_drv.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/FatFs/syscall.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/GPIO/Middlewares/Third_Party/FatFs/src/option/syscall.c Middlewares/Third_Party/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FatFs/syscall.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

