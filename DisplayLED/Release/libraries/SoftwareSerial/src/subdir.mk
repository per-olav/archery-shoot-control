################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src\SoftwareSerial.cpp 

LINK_OBJ += \
.\libraries\SoftwareSerial\src\SoftwareSerial.cpp.o 

CPP_DEPS += \
.\libraries\SoftwareSerial\src\SoftwareSerial.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\SoftwareSerial\src\SoftwareSerial.cpp.o: C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src\SoftwareSerial.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\FastLED\FastLED-3.1.3" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial" -I"C:\eclipse-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\SoftwareSerial\src" -I"C:\eclipse-neon\eclipse\arduinoPlugin\libraries\LEDMatrix\LEDMatrix-1.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '


