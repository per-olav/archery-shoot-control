################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\DisplayLED.ino 

CPP_SRCS += \
..\.ino.cpp 

LINK_OBJ += \
.\.ino.cpp.o 

INO_DEPS += \
.\DisplayLED.ino.d 

CPP_DEPS += \
.\.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ..\.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

DisplayLED.o: ..\DisplayLED.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

