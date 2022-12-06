################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
aic23.obj: ../aic23.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Client/Desktop/Vysoka_skola/Ing/1. Semestr/Signalove_procesory/Cvika/doma/xtomes07/Simulator" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6416/CCStudio/c6000/dsk6416/include" --define=c6416 --define=CHIP_6416 --define=SIMULATOR --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="aic23.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hello.obj: ../hello.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Client/Desktop/Vysoka_skola/Ing/1. Semestr/Signalove_procesory/Cvika/doma/xtomes07/Simulator" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6416/CCStudio/c6000/dsk6416/include" --define=c6416 --define=CHIP_6416 --define=SIMULATOR --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="hello.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hellocfg.cmd: ../hello.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hellocfg.s??: | hellocfg.cmd
hellocfg_c.c: | hellocfg.cmd
hellocfg.h: | hellocfg.cmd
hellocfg.h??: | hellocfg.cmd
hello.cdb: | hellocfg.cmd

hellocfg.obj: ./hellocfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Client/Desktop/Vysoka_skola/Ing/1. Semestr/Signalove_procesory/Cvika/doma/xtomes07/Simulator" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6416/CCStudio/c6000/dsk6416/include" --define=c6416 --define=CHIP_6416 --define=SIMULATOR --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="hellocfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hellocfg_c.obj: ./hellocfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Client/Desktop/Vysoka_skola/Ing/1. Semestr/Signalove_procesory/Cvika/doma/xtomes07/Simulator" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6416/CCStudio/c6000/dsk6416/include" --define=c6416 --define=CHIP_6416 --define=SIMULATOR --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="hellocfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pseudoRandomNumber.obj: ../pseudoRandomNumber.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6400 --abi=coffabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="C:/Users/Client/Desktop/Vysoka_skola/Ing/1. Semestr/Signalove_procesory/Cvika/doma/xtomes07/Simulator" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6416/CCStudio/c6000/dsk6416/include" --define=c6416 --define=CHIP_6416 --define=SIMULATOR --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="pseudoRandomNumber.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


