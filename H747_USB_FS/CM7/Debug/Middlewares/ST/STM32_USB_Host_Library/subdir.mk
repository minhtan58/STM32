################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/usbh_core.o \
./Middlewares/ST/STM32_USB_Host_Library/usbh_ctlreq.o \
./Middlewares/ST/STM32_USB_Host_Library/usbh_ioreq.o \
./Middlewares/ST/STM32_USB_Host_Library/usbh_msc.o \
./Middlewares/ST/STM32_USB_Host_Library/usbh_msc_bot.o \
./Middlewares/ST/STM32_USB_Host_Library/usbh_msc_scsi.o \
./Middlewares/ST/STM32_USB_Host_Library/usbh_pipes.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/usbh_core.d \
./Middlewares/ST/STM32_USB_Host_Library/usbh_ctlreq.d \
./Middlewares/ST/STM32_USB_Host_Library/usbh_ioreq.d \
./Middlewares/ST/STM32_USB_Host_Library/usbh_msc.d \
./Middlewares/ST/STM32_USB_Host_Library/usbh_msc_bot.d \
./Middlewares/ST/STM32_USB_Host_Library/usbh_msc_scsi.d \
./Middlewares/ST/STM32_USB_Host_Library/usbh_pipes.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/usbh_core.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_core.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Host_Library/usbh_ctlreq.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_ctlreq.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Host_Library/usbh_ioreq.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_ioreq.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Host_Library/usbh_msc.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_msc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Host_Library/usbh_msc_bot.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_msc_bot.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Host_Library/usbh_msc_scsi.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_msc_scsi.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Host_Library/usbh_pipes.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/H747_USB_FS/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H747xx -DCORE_CM7 -DDEBUG -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../../Middlewares/Third_Party/FatFs/src -I../../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/usbh_pipes.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

