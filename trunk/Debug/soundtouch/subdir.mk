################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../soundtouch/3dnow_win.cpp \
../soundtouch/AAFilter.cpp \
../soundtouch/BPMDetect.cpp \
../soundtouch/FIFOSampleBuffer.cpp \
../soundtouch/FIRFilter.cpp \
../soundtouch/PeakFinder.cpp \
../soundtouch/RateTransposer.cpp \
../soundtouch/SoundTouch.cpp \
../soundtouch/TDStretch.cpp \
../soundtouch/cpu_detect_x86_gcc.cpp \
../soundtouch/cpu_detect_x86_win.cpp \
../soundtouch/mmx_optimized.cpp \
../soundtouch/sse_optimized.cpp 

OBJS += \
./soundtouch/3dnow_win.o \
./soundtouch/AAFilter.o \
./soundtouch/BPMDetect.o \
./soundtouch/FIFOSampleBuffer.o \
./soundtouch/FIRFilter.o \
./soundtouch/PeakFinder.o \
./soundtouch/RateTransposer.o \
./soundtouch/SoundTouch.o \
./soundtouch/TDStretch.o \
./soundtouch/cpu_detect_x86_gcc.o \
./soundtouch/cpu_detect_x86_win.o \
./soundtouch/mmx_optimized.o \
./soundtouch/sse_optimized.o 

CPP_DEPS += \
./soundtouch/3dnow_win.d \
./soundtouch/AAFilter.d \
./soundtouch/BPMDetect.d \
./soundtouch/FIFOSampleBuffer.d \
./soundtouch/FIRFilter.d \
./soundtouch/PeakFinder.d \
./soundtouch/RateTransposer.d \
./soundtouch/SoundTouch.d \
./soundtouch/TDStretch.d \
./soundtouch/cpu_detect_x86_gcc.d \
./soundtouch/cpu_detect_x86_win.d \
./soundtouch/mmx_optimized.d \
./soundtouch/sse_optimized.d 


# Each subdirectory must supply rules for building sources it contributes
soundtouch/%.o: ../soundtouch/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/lib/gcc/i486-slackware-linux/4.4.4/include -I/usr/include/c++/4.4.4 -O3 -g3 -p -pg -pedantic -Wall -c -fmessage-length=0 -ansi  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


