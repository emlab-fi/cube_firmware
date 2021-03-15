cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(MCU "STM32G474xx")
set(MCU_ARCH "cortex-m4")
set(MCU_FLOAT_ABI "hard")
set(MCU_FAMILY "STM32G4xx")
set(MCU_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32G474VETx_FLASH.ld)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_TOOLCHAIN_FILE stm32_toolchain)

include(stm32_hal)

add_compile_options(
    -std=c++17
)

add_definitions(-D${MCU})

set(MEMORY_BLOCK_SIZE 1024)
set(MEMORY_BLOCK_COUNT 20)

project(cube_firmware C CXX ASM)

set(SRC main.c interrupts.c stm32g4xx_system.c)

add_executable(${PROJECT_NAME}.elf ${SRC})
target_compile_options(${PROJECT_NAME}.elf PRIVATE "-g")
target_link_options(${PROJECT_NAME}.elf PRIVATE "-Wl,-Map=${PROJECT_NAME}.map,--cref" "-Wl,--print-memory-usage" "-funwind-tables" "-fasynchronous-unwind-tables")
target_link_libraries(${PROJECT_NAME}.elf stm32_hal)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")
