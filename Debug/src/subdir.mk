################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/repulse.cpp 

OBJS += \
./src/repulse.o 

CPP_DEPS += \
./src/repulse.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/lib/gcc/i486-slackware-linux/4.4.4/include -I/usr/include/c++/4.4.4 -O3 -g3 -pedantic -pedantic-errors -Wall -Werror -c -fmessage-length=0 -msse2 -fno-strict-aliasing -march=i486 -mtune=i686 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


