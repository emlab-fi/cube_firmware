cmake_minimum_required(VERSION 3.16)
set(STM32_TOOLCHAIN_PATH "/home/loady/opt/gcc-arm-none-eabi-10-2020-q4-major/")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(cube_firmware C CXX ASM)

stm32_fetch_cmsis(G4)
stm32_fetch_hal(G4)

find_package(CMSIS COMPONENTS STM32G474VE REQUIRED)
find_package(HAL COMPONENTS STM32G4 REQUIRED)

set(SOURCES
    source/main.cpp
    source/interrupts.cpp
    source/peripherals.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

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
    HAL::STM32::G4::USART
    HAL::STM32::G4::UARTEx
    CMSIS::STM32::G474VE
    STM32::NoSys
    )

stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_binary_file(${PROJECT_NAME})
