################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Project1.cpp \
../src/control.cpp \
../src/guidance.cpp \
../src/navigator.cpp \
../src/pid.cpp \
../src/util.cpp 

OBJS += \
./src/Project1.o \
./src/control.o \
./src/guidance.o \
./src/navigator.o \
./src/pid.o \
./src/util.o 

CPP_DEPS += \
./src/Project1.d \
./src/control.d \
./src/guidance.d \
./src/navigator.d \
./src/pid.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-linux-gnueabi-g++ -mcpu=arm9 -marm -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -I/usr/local/EV3-API/API -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


