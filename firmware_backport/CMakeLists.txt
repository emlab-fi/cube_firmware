include(CMakeForceCompiler)

# CROSS COMPILER SETTING
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TOOLCHAIN_FILE armgcc.cmake)
CMAKE_MINIMUM_REQUIRED (VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(ProjectName)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

# CURRENT DIRECTORY
set(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
-DDEBUG -D__STARTUP_CLEAR_BSS \
-g -Wall -Wextra -std=gnu99 \
-fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin \
-mthumb -mapcs -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
-DDEBUG -D__NEWLIB__ -DCPU_MK64FN1M0VLL12 -DPRINTF_FLOAT_ENABLE=0 -DSCANF_FLOAT_ENABLE=0 \
-DPRINTF_ADVANCED_ENABLE=0 -DSCANF_ADVANCED_ENABLE=0 -DFRDM_K64F -DFREEDOM \
-g -O0 -Wall -std=gnu99 \
-fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin \
-mthumb -mapcs -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -MMD -MP")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-DDEBUG -D__NEWLIB__ -DCPU_MK64FN1M0VLL12 \
-g -O0 -Wall -Wextra -std=c++20 \
-fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin \
-mthumb -mapcs -fno-rtti -fno-exceptions -mcpu=cortex-m4 -mfloat-abi=hard \
-mfpu=fpv4-sp-d16 -MMD -MP")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-g --specs=nano.specs --specs=nosys.specs -Wall -Wextra -fno-common -ffunction-sections \
-fdata-sections -ffreestanding -fno-builtin -mthumb -mapcs \
-Xlinker --gc-sections -Xlinker -static -Xlinker -z -Xlinker muldefs -Xlinker \
-Map=output.map -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
-T${ProjDirPath}/sdk/MK64FN1M0xxx12_flash.ld -static")


add_subdirectory(../cube_lib ${PROJECT_BINARY_DIR}/cube_lib)


include_directories(${ProjDirPath}/sdk/src)
include_directories(${ProjDirPath}/sdk/device)
include_directories(${ProjDirPath}/sdk/CMSIS)
include_directories(${ProjDirPath}/sdk/component/lists)
include_directories(${ProjDirPath}/sdk/component/uart)
include_directories(${ProjDirPath}/sdk/component/serial_manager)
include_directories(${ProjDirPath}/sdk/startup)
include_directories(${ProjDirPath}/sdk/utilities)
include_directories(${ProjDirPath}/sdk/board)
include_directories(${ProjDirPath}/sdk/drivers)
include_directories(${ProjDirPath}/source)
include_directories(${ProjDirPath}/source/tiny_printf)
include_directories(${ProjDirPath}/source/TMC429)
include_directories(${ProjDirPath}/source/mcp23008)

add_executable(Cube_FW.elf
"${ProjDirPath}/sdk/board/board.c"
"${ProjDirPath}/sdk/board/peripherals.c"
"${ProjDirPath}/sdk/board/clock_config.c"
"${ProjDirPath}/sdk/board/pin_mux.c"
"${ProjDirPath}/sdk/device/system_MK64F12.c"
"${ProjDirPath}/sdk/drivers/fsl_clock.c"
"${ProjDirPath}/sdk/drivers/fsl_common.c"
"${ProjDirPath}/sdk/drivers/fsl_ftfx_cache.c"
"${ProjDirPath}/sdk/drivers/fsl_ftfx_controller.c"
"${ProjDirPath}/sdk/drivers/fsl_ftfx_flash.c"
"${ProjDirPath}/sdk/drivers/fsl_ftfx_flexnvm.c"
"${ProjDirPath}/sdk/drivers/fsl_gpio.c"
"${ProjDirPath}/sdk/drivers/fsl_smc.c"
"${ProjDirPath}/sdk/drivers/fsl_uart.c"
"${ProjDirPath}/sdk/drivers/fsl_i2c.c"
"${ProjDirPath}/sdk/drivers/fsl_dspi.c"
"${ProjDirPath}/sdk/drivers/fsl_pit.c"
"${ProjDirPath}/sdk/src/MK64FN1M0xxx12_flash.ld"
"${ProjDirPath}/sdk/startup/startup_MK64F12.S"
#"${ProjDirPath}/sdk/utilities/fsl_assert.c"
"${ProjDirPath}/source/main.cpp"
"${ProjDirPath}/source/tiny_printf/printf.c"
"${ProjDirPath}/source/TMC429/TMC429.cpp"
"${ProjDirPath}/source/mcp23008/mcp23008.cpp"
"${ProjDirPath}/source/reactor.cpp"
)

target_link_libraries(Cube_FW.elf cube_lib)
target_link_libraries(Cube_FW.elf -Wl,--start-group)
target_link_libraries(Cube_FW.elf debug m)
target_link_libraries(Cube_FW.elf debug c)
target_link_libraries(Cube_FW.elf debug gcc)
target_link_libraries(Cube_FW.elf debug nosys)
target_link_libraries(Cube_FW.elf optimized m)
target_link_libraries(Cube_FW.elf optimized c)
target_link_libraries(Cube_FW.elf optimized gcc)
target_link_libraries(Cube_FW.elf optimized nosys)
target_link_libraries(Cube_FW.elf -Wl,--end-group)
