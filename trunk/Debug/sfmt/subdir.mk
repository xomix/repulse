################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sfmt/sfmt.cpp 

OBJS += \
./sfmt/sfmt.o 

CPP_DEPS += \
./sfmt/sfmt.d 


# Each subdirectory must supply rules for building sources it contributes
sfmt/%.o: ../sfmt/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g3 -pedantic -pedantic-errors -Wall -Werror -c -fmessage-length=0 -msse2 -fno-strict-aliasing -march=i486 -mtune=i686 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


