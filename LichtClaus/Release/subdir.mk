################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LichtClaus.cpp \
../TimerOne.cpp 

CPP_DEPS += \
./LichtClaus.cpp.d \
./TimerOne.cpp.d 

LINK_OBJ += \
./LichtClaus.cpp.o \
./TimerOne.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
LichtClaus.cpp.o: ../LichtClaus.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

TimerOne.cpp.o: ../TimerOne.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


