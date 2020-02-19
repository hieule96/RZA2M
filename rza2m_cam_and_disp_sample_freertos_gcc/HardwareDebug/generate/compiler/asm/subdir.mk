################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../generate/compiler/asm/cpu.asm \
../generate/compiler/asm/initsect.asm \
../generate/compiler/asm/irq.asm \
../generate/compiler/asm/irqfiq_handler.asm \
../generate/compiler/asm/mmu_operation.asm \
../generate/compiler/asm/r_cache_l1_rza2.asm \
../generate/compiler/asm/reset_handler.asm \
../generate/compiler/asm/vector_mirrortable.asm \
../generate/compiler/asm/vector_table.asm \
../generate/compiler/asm/vfp_init.asm 

OBJS += \
./generate/compiler/asm/cpu.o \
./generate/compiler/asm/initsect.o \
./generate/compiler/asm/irq.o \
./generate/compiler/asm/irqfiq_handler.o \
./generate/compiler/asm/mmu_operation.o \
./generate/compiler/asm/r_cache_l1_rza2.o \
./generate/compiler/asm/reset_handler.o \
./generate/compiler/asm/vector_mirrortable.o \
./generate/compiler/asm/vector_table.o \
./generate/compiler/asm/vfp_init.o 

ASM_DEPS += \
./generate/compiler/asm/cpu.d \
./generate/compiler/asm/initsect.d \
./generate/compiler/asm/irq.d \
./generate/compiler/asm/irqfiq_handler.d \
./generate/compiler/asm/mmu_operation.d \
./generate/compiler/asm/r_cache_l1_rza2.d \
./generate/compiler/asm/reset_handler.d \
./generate/compiler/asm/vector_mirrortable.d \
./generate/compiler/asm/vector_table.d \
./generate/compiler/asm/vfp_init.d 


# Each subdirectory must supply rules for building sources it contributes
generate/compiler/asm/%.o: ../generate/compiler/asm/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -Og -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -Wmaybe-uninitialized -g3 -Wstack-usage=100 -x assembler-with-cpp -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/os_abstraction/inc" -I"C:/Users/tle/e2_studio/workspace/rza2m_cam_and_disp_sample_freertos_gcc/generate/sc_drivers/r_drp/drp_lib" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


