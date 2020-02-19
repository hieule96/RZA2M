################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/renesas/application/dat.asm 

C_SRCS += \
../src/renesas/application/r_bcd_camera.c \
../src/renesas/application/r_bcd_lcd.c \
../src/renesas/application/r_bcd_main.c \
../src/renesas/application/r_cui_image_adjust.c 

C_DEPS += \
./src/renesas/application/r_bcd_camera.d \
./src/renesas/application/r_bcd_lcd.d \
./src/renesas/application/r_bcd_main.d \
./src/renesas/application/r_cui_image_adjust.d 

OBJS += \
./src/renesas/application/dat.o \
./src/renesas/application/r_bcd_camera.o \
./src/renesas/application/r_bcd_lcd.o \
./src/renesas/application/r_bcd_main.o \
./src/renesas/application/r_cui_image_adjust.o 

ASM_DEPS += \
./src/renesas/application/dat.d 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/application/%.o: ../src/renesas/application/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Og -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=100 -x assembler-with-cpp -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/os_abstraction/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/drp_lib" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/renesas/application/%.o: ../src/renesas/application/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Og -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=100 -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\FreeRTOS\include" -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\FreeRTOS\include\private" -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\FreeRTOS\portable\GCC\ARM_CA9_RZA2M" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/compiler/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/configuration" -I"C:\Users\tle\e2_studio\workspace\rza2m_cam_and_disp_sample_freertos_gcc\src\config_files" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/system/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/system/iodefine" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/src/renesas/application/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_scifa/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_riic/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_ostm/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_ceu/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_mipi/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_rvapi/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_vdc/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_cbuffer/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_cache/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_cpg/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_gpio/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_intc/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_mmu/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/drivers/r_stb/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/os_abstraction/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_jcu/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/src" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/drp_lib" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_adc/inc" -fabi-version=0 -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


