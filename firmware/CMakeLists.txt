cmake_minimum_required(VERSION 3.16)
set(STM32_TOOLCHAIN_PATH "/home/loady/opt/gcc-arm-none-eabi-10-2020-q4-major/")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

#global compile flags, to force some arm specific flags into the linked libraries
#not a nice solution
add_compile_options(-g -mthumb -fno-exceptions -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nosys.specs)

#get tiny_printf
Include(FetchContent)

FetchContent_Declare(
    tinyprintf
    GIT_REPOSITORY https://github.com/mpaland/printf
    GIT_TAG        v4.0.0)
FetchContent_GetProperties(tinyprintf)
if(NOT tinyprintf_POPULATED)
	FetchContent_Populate(tinyprintf)
endif()

add_library(tiny_printf STATIC
    ${tinyprintf_SOURCE_DIR}/printf.c
)

project(cube_firmware C CXX ASM)

stm32_fetch_cmsis(G4)
stm32_fetch_hal(G4)

find_package(CMSIS COMPONENTS STM32G474VE REQUIRED)
find_package(HAL COMPONENTS STM32G4 REQUIRED)

add_subdirectory(../cube_lib ${PROJECT_BINARY_DIR}/cube_lib)

set(SOURCES
    source/main.cpp
    source/interrupts.cpp
    source/peripherals.cpp
    source/cube_hw.cpp
    source/logging.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 99
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${tinyprintf_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    HAL::STM32::G4::GPIO
    HAL::STM32::G4::CORTEX
    HAL::STM32::G4::DMA
    HAL::STM32::G4::RCC
    HAL::STM32::G4::RCCEx
    HAL::STM32::G4::PWR
    HAL::STM32::G4::PWREx
    HAL::STM32::G4::I2C
    HAL::STM32::G4::I2CEx
    HAL::STM32::G4::UART
    HAL::STM32::G4::UARTEx
    CMSIS::STM32::G474VE
    STM32::NoSys
    cube_lib
    tiny_printf
    )

target_compile_options(${PROJECT_NAME} PRIVATE -fno-exceptions) #-Wall -Wextra -pedantic

stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_binary_file(${PROJECT_NAME})
