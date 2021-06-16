################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
L152RB_SPI_Master.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
L152RB_SPI_Master.list \

OBJCOPY_BIN += \
L152RB_SPI_Master.bin \


# All Target
all: L152RB_SPI_Master.elf secondary-outputs

# Tool invocations
L152RB_SPI_Master.elf: $(OBJS) $(USER_OBJS) D:\My_Project\ARM\L152RB_SPI_Master\STM32L152RBTX_FLASH.ld
	arm-none-eabi-gcc -o "L152RB_SPI_Master.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\My_Project\ARM\L152RB_SPI_Master\STM32L152RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="L152RB_SPI_Master.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

L152RB_SPI_Master.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "L152RB_SPI_Master.list"
	@echo 'Finished building: $@'
	@echo ' '

L152RB_SPI_Master.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "L152RB_SPI_Master.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
