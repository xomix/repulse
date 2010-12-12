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
	g++ -I"../soundtouch/include" -I"../soundtouch/source/SoundTouch" -O3 -pedantic -Wall -c -fmessage-length=0  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


