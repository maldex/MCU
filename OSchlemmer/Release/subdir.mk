################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../OSchlemmer.cpp 

CPP_DEPS += \
./OSchlemmer.cpp.d 

LINK_OBJ += \
./OSchlemmer.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
OSchlemmer.cpp.o: ../OSchlemmer.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


