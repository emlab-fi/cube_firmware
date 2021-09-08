cmake_minimum_required(VERSION 3.16)
set(STM32_TOOLCHAIN_PATH "/home/loady/opt/gcc-arm-none-eabi-10-2020-q4-major/")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(cube_firmware C CXX ASM)

stm32_fetch_cmsis(G4)
stm32_fetch_hal(G4)

find_package(CMSIS COMPONENTS STM32G474VE REQUIRED)
find_package(HAL COMPONENTS STM32G4 REQUIRED)

set(SOURCES main.c interrupts.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    CMSIS::STM32::G474VE
    HAL::STM32::G4::LL_GPIO
    HAL::STM32::G4::LL_RCC
    HAL::STM32::G4::LL_UTILS
    HAL::STM32::G4::LL_PWR
    STM32::NoSys
    )

stm32_print_size_of_target(${PROJECT_NAME})
