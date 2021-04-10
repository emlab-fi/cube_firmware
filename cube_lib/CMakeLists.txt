cmake_minimum_required(VERSION 3.12)

# Explicitly enable colors in GCC/Clang – this is mainly important for the Ninja
# builds (with makefiles the compiler is able to autodetect if it runs on
# terminal)
set(COLORS "")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(COLORS "-fdiagnostics-color=always")
endif()

project(CUBE_LIB
    LANGUAGES CXX)

add_library(CUBE_LIB STATIC)
set_target_properties(CUBE_LIB PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_include_directories(CUBE_LIB PUBLIC include)

target_sources(CUBE_LIB
    PRIVATE
    source/common/coordinate.cpp
    source/planner.cpp
)

target_compile_options(CUBE_LIB PRIVATE -Wall -Wextra -pedantic  ${COLORS})