cmake_minimum_required(VERSION 3.10)
project(RampingVisualizer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
set(SOURCES
    ./main.cpp
    ./mock/mock.cpp
    ../../firmware/source/stepper_generator.cpp
    ../../firmware/source/config.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 99
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ../../firmware/include
    ../../cube_lib/include
    ./mock
)

target_compile_options(${PROJECT_NAME} PRIVATE -fno-exceptions -Wall -Wextra -pedantic )
