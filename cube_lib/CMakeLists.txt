cmake_minimum_required(VERSION 3.12)

# Explicitly enable colors in GCC/Clang – this is mainly important for the Ninja
# builds (with makefiles the compiler is able to autodetect if it runs on
# terminal)
set(COLORS "")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(COLORS "-fdiagnostics-color=always")
endif()


Include(FetchContent)

FetchContent_Declare(
    nanopb
    GIT_REPOSITORY https://github.com/nanopb/nanopb.git
    GIT_TAG        0.4.5)
FetchContent_GetProperties(nanopb)
if(NOT nanopb_POPULATED)
	FetchContent_Populate(nanopb)
endif()

add_library(nanopb_lib STATIC
    ${nanopb_SOURCE_DIR}/pb_common.c
    ${nanopb_SOURCE_DIR}/pb_encode.c
    ${nanopb_SOURCE_DIR}/pb_decode.c
)

project(cube_lib
    LANGUAGES CXX C)

add_library(${PROJECT_NAME} STATIC)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PUBLIC include ${nanopb_SOURCE_DIR})

target_sources(${PROJECT_NAME}
    PRIVATE
    source/common/coordinate.cpp
    source/planner.cpp
    protocols/cube.pb.c
)

target_link_libraries(${PROJECT_NAME} nanopb_lib)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic  ${COLORS})