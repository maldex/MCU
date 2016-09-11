################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/CDC.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/HID.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/IPAddress.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Print.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Stream.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Tone.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/USBCore.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/WMath.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/WString.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/main.cpp \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/new.cpp 

C_SRCS += \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/WInterrupts.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/hooks.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/malloc.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_analog.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_digital.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_pulse.c \
C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_shift.c 

C_DEPS += \
./arduino/WInterrupts.c.d \
./arduino/hooks.c.d \
./arduino/malloc.c.d \
./arduino/wiring.c.d \
./arduino/wiring_analog.c.d \
./arduino/wiring_digital.c.d \
./arduino/wiring_pulse.c.d \
./arduino/wiring_shift.c.d 

AR_OBJ += \
./arduino/CDC.cpp.o \
./arduino/HID.cpp.o \
./arduino/HardwareSerial.cpp.o \
./arduino/IPAddress.cpp.o \
./arduino/Print.cpp.o \
./arduino/Stream.cpp.o \
./arduino/Tone.cpp.o \
./arduino/USBCore.cpp.o \
./arduino/WInterrupts.c.o \
./arduino/WMath.cpp.o \
./arduino/WString.cpp.o \
./arduino/hooks.c.o \
./arduino/main.cpp.o \
./arduino/malloc.c.o \
./arduino/new.cpp.o \
./arduino/wiring.c.o \
./arduino/wiring_analog.c.o \
./arduino/wiring_digital.c.o \
./arduino/wiring_pulse.c.o \
./arduino/wiring_shift.c.o 

CPP_DEPS += \
./arduino/CDC.cpp.d \
./arduino/HID.cpp.d \
./arduino/HardwareSerial.cpp.d \
./arduino/IPAddress.cpp.d \
./arduino/Print.cpp.d \
./arduino/Stream.cpp.d \
./arduino/Tone.cpp.d \
./arduino/USBCore.cpp.d \
./arduino/WMath.cpp.d \
./arduino/WString.cpp.d \
./arduino/main.cpp.d \
./arduino/new.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
arduino/CDC.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/CDC.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/HID.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/HID.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/HardwareSerial.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/IPAddress.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/IPAddress.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/Print.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Print.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/Stream.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Stream.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/Tone.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Tone.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/USBCore.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/USBCore.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/WInterrupts.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/WInterrupts.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/WMath.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/WMath.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/WString.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/WString.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/hooks.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/hooks.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/main.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/main.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/malloc.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/malloc.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/new.cpp.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/new.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_analog.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_analog.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_digital.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_digital.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_pulse.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_pulse.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_shift.c.o: C:/Programme/arduino-1.5.2/hardware/arduino/avr/cores/arduino/wiring_shift.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Programme/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=152    -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\cores\arduino" -I"C:\Programme\arduino-1.5.2\hardware\arduino\avr\variants\standard" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


