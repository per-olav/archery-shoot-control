################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\DisplayLED.ino 

CPP_SRCS += \
..\.ino.cpp \
..\sloeber.ino.cpp 

LINK_OBJ += \
.\.ino.cpp.o \
.\sloeber.ino.cpp.o 

INO_DEPS += \
.\DisplayLED.ino.d 

CPP_DEPS += \
.\.ino.cpp.d \
.\sloeber.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ..\.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\LEDMatrix\LEDMatrix-1.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

DisplayLED.o: ..\DisplayLED.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\LEDMatrix\LEDMatrix-1.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

sloeber.ino.cpp.o: ..\sloeber.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\LEDMatrix\LEDMatrix-1.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '


