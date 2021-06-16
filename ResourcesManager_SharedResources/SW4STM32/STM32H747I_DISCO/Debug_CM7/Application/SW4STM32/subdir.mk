################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/ResourcesManager_SharedResources/SW4STM32/startup_stm32h747xx.s 

OBJS += \
./Application/SW4STM32/startup_stm32h747xx.o 

S_DEPS += \
./Application/SW4STM32/startup_stm32h747xx.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32h747xx.o: C:/Users/Admin/STM32CubeIDE/workspace_1.5.0/ResourcesManager_SharedResources/SW4STM32/startup_stm32h747xx.s Application/SW4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -I../../../CM7/Inc -x assembler-with-cpp -MMD -MP -MF"Application/SW4STM32/startup_stm32h747xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

