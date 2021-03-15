cmake_minimum_required(VERSION 3.11)

include(FetchContent)

project(stm32_hal)
enable_language(C CXX ASM)

# STM32 HAL and CMSIS
string(TOLOWER ${MCU} MCU_LOWER)
string(TOLOWER ${MCU_FAMILY} MCU_FAMILY_LOWER)

FetchContent_Declare(
        hal
        GIT_REPOSITORY https://github.com/STMicroelectronics/STM32CubeG4.git
        GIT_TAG        v1.3.0
	GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(hal)

if(NOT hal_POPULATED)
  FetchContent_Populate(hal)
  # Turn config template into config
  file(GLOB_RECURSE cfgFile "${hal_SOURCE_DIR}/Drivers/*_conf_template.h")
  foreach(f ${cfgFile})
        string(REGEX REPLACE "_template\.h$" ".h" newF ${f})
        file(RENAME ${f} ${newF})
    endforeach(f)
endif()

set(STM_LIB ${hal_SOURCE_DIR}/Drivers)
set(HAL_PATH ${STM_LIB}/${MCU_FAMILY}_HAL_Driver)

add_library(startup OBJECT
    ${STM_LIB}/CMSIS/Device/ST/${MCU_FAMILY}/Source/Templates/gcc/startup_${MCU_LOWER}.s)

add_library(CMSIS STATIC
    ${STM_LIB}/CMSIS/Device/ST/${MCU_FAMILY}/Source/Templates/system_${MCU_FAMILY_LOWER}.c)
target_include_directories(CMSIS
    SYSTEM PUBLIC ${STM_LIB}/CMSIS/Include
    SYSTEM PUBLIC ${STM_LIB}/CMSIS/Device/ST/${MCU_FAMILY}/Include)

function(add_hallib libname)
    set(sources "")
    foreach(f ${ARGN})
        list(APPEND sources "${HAL_PATH}/Src/${MCU_FAMILY_LOWER}_${f}")
    endforeach(f)
    add_library(${libname} STATIC ${sources})
    target_include_directories(${libname} SYSTEM PUBLIC ${HAL_PATH}/Inc)
    target_link_libraries(${libname} PUBLIC CMSIS)
    target_compile_options(${libname} PUBLIC -DUSE_FULL_LL_DRIVER -D${MCU_FAMILY} -Os)
endfunction()

add_hallib(HAL ll_rcc.c ll_gpio.c)


# There is a pooled memory functionality in the library, if no values are set,
# define one
if(NOT DEFINED MEMORY_BLOCK_SIZE)
    set(MEMORY_BLOCK_SIZE 1024)
endif()

if(NOT DEFINED MEMORY_BLOCK_COUNT)
    set(MEMORY_BLOCK_COUNT 10)
endif()

add_library( stm32_hal STATIC $<TARGET_OBJECTS:startup>)
target_compile_options( stm32_hal PUBLIC -DUSE_FULL_LL_DRIVER -D${MCU_FAMILY} -DMEMORY_BLOCK_SIZE=${MEMORY_BLOCK_SIZE} -DMEMORY_BLOCK_COUNT=${MEMORY_BLOCK_COUNT} -Os)
target_link_libraries(stm32_hal PUBLIC HAL)

