################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/arduino.ar \

ELF += \
LichtClaus.elf \

EEP += \
LichtClaus.eep \

FLASH_IMAGE += \
LichtClaus.hex \


# All Target
all: LichtClaus

# Tool invocations
LichtClaus: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-size" -A "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/LichtClaus.hex"
	@echo 'Finished building target: $@'
	@echo ' '

C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-ar" rcs "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

LichtClaus.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega328p -o "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/LichtClaus.elf"   $(LINK_OBJ) $(AR)  "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/arduino.ar" "-LC:/Users/Administrator/Desktop/Projects/LichtClaus/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

LichtClaus.eep: $(ELF)
	@echo 'Create eeprom image'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/LichtClaus.elf" "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/LichtClaus.eep"
	@echo 'Finished building: $@'
	@echo ' '

LichtClaus.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/LichtClaus.elf" "C:/Users/Administrator/Desktop/Projects/LichtClaus/Release/LichtClaus.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF)$(C++_DEPS)$(AR)$(C_DEPS)$(CC_DEPS)$(FLASH_IMAGE)$(AR_OBJ)$(CPP_DEPS)$(LINK_OBJ)$(EEP)$(CXX_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) LichtClaus
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
