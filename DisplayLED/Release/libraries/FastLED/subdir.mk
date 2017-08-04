################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\FastLED.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\bitswap.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\colorpalettes.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\colorutils.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\hsv2rgb.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\lib8tion.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\noise.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\power_mgt.cpp \
C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\wiring.cpp 

LINK_OBJ += \
.\libraries\FastLED\FastLED.cpp.o \
.\libraries\FastLED\bitswap.cpp.o \
.\libraries\FastLED\colorpalettes.cpp.o \
.\libraries\FastLED\colorutils.cpp.o \
.\libraries\FastLED\hsv2rgb.cpp.o \
.\libraries\FastLED\lib8tion.cpp.o \
.\libraries\FastLED\noise.cpp.o \
.\libraries\FastLED\power_mgt.cpp.o \
.\libraries\FastLED\wiring.cpp.o 

CPP_DEPS += \
.\libraries\FastLED\FastLED.cpp.d \
.\libraries\FastLED\bitswap.cpp.d \
.\libraries\FastLED\colorpalettes.cpp.d \
.\libraries\FastLED\colorutils.cpp.d \
.\libraries\FastLED\hsv2rgb.cpp.d \
.\libraries\FastLED\lib8tion.cpp.d \
.\libraries\FastLED\noise.cpp.d \
.\libraries\FastLED\power_mgt.cpp.d \
.\libraries\FastLED\wiring.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\FastLED\FastLED.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\FastLED.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\bitswap.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\bitswap.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\colorpalettes.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\colorpalettes.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\colorutils.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\colorutils.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\hsv2rgb.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\hsv2rgb.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\lib8tion.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\lib8tion.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\noise.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\noise.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\power_mgt.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\power_mgt.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\FastLED\wiring.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3\wiring.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\eclipse-neon\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


