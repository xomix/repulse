################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tinyxml/tinystr.cpp \
../tinyxml/tinyxml.cpp \
../tinyxml/tinyxmlerror.cpp \
../tinyxml/tinyxmlparser.cpp 

OBJS += \
./tinyxml/tinystr.o \
./tinyxml/tinyxml.o \
./tinyxml/tinyxmlerror.o \
./tinyxml/tinyxmlparser.o 

CPP_DEPS += \
./tinyxml/tinystr.d \
./tinyxml/tinyxml.d \
./tinyxml/tinyxmlerror.d \
./tinyxml/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
tinyxml/%.o: ../tinyxml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jrodrigo/workspace/repulse/soundtouch/source/SoundTouch" -I"/home/jrodrigo/workspace/repulse/soundtouch/include" -O3 -g3 -pedantic -pedantic-errors -Wall -Werror -c -fmessage-length=0 -pedantic -msse2 -fno-strict-aliasing -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


