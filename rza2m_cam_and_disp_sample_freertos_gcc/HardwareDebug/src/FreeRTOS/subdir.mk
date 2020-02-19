################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOS/event_groups.c \
../src/FreeRTOS/list.c \
../src/FreeRTOS/queue.c \
../src/FreeRTOS/stream_buffer.c \
../src/FreeRTOS/tasks.c \
../src/FreeRTOS/timers.c 

C_DEPS += \
./src/FreeRTOS/event_groups.d \
./src/FreeRTOS/list.d \
./src/FreeRTOS/queue.d \
./src/FreeRTOS/stream_buffer.d \
./src/FreeRTOS/tasks.d \
./src/FreeRTOS/timers.d 

OBJS += \
./src/FreeRTOS/event_groups.o \
./src/FreeRTOS/list.o \
./src/FreeRTOS/queue.o \
./src/FreeRTOS/stream_buffer.o \
./src/FreeRTOS/tasks.o \
./src/FreeRTOS/timers.o 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS/%.o: ../src/FreeRTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Og -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=100 -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\FreeRTOS\include" -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\FreeRTOS\include\private" -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\FreeRTOS\portable\GCC\ARM_CA9_RZA2M" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/compiler/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/configuration" -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\config_files" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/system/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/system/iodefine" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/src/renesas/application/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_scifa/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_riic/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_ostm/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_ceu/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_mipi/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_rvapi/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_vdc/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_cbuffer/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_cache/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_cpg/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_gpio/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_intc/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_mmu/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_stb/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/os_abstraction/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_jcu/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/src" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/drp_lib" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_adc/inc" -fabi-version=0 -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

