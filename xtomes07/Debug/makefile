################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c6000_7.4.4

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./interp.obj" \
"./hellocfg_c.obj" \
"./hellocfg.obj" \
"./hello.obj" \
"./generate.obj" \
"./fir_filter.obj" \
"./aic23.obj" \
-l"libc.a" \
-lcsl6416.lib \
-ldsk6416bsl.lib \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: generate.out

# Tool invocations
generate.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6400 --abi=coffabi -g --define=c6416 --define=CHIP_6416 --display_error_number --diag_warning=225 --diag_wrap=off -z --stack_size=0x800 -m"generate.map" --heap_size=0x8000 -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/lib" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" -i"C:/ti/bios_5_42_01_09/packages/ti/rtdx/lib/c6000" -i"C:/ti/bios_5_42_01_09/packages/ti/bios/lib" -i"C:/ti/C6xCSL/lib_3x" -i"C:/ti/DSK6416/CCStudio/c6000/dsk6416/lib" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="generate_linkInfo.xml" --rom_model -o "generate.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_HDRS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(C6000_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(GEN_CMDS__QUOTED) "generate.out"
	-$(RM) "aic23.pp" "fir_filter.pp" "generate.pp" "hello.pp" "hellocfg_c.pp" "interp.pp" 
	-$(RM) "hellocfg.pp" 
	-$(RM) "aic23.obj" "fir_filter.obj" "generate.obj" "hello.obj" "hellocfg.obj" "hellocfg_c.obj" "interp.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

