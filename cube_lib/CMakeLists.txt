cmake_minimum_required(VERSION 3.12)

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

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    ${nanopb_SOURCE_DIR}
    protocols
)

target_link_libraries(${PROJECT_NAME} nanopb_lib)

target_sources(${PROJECT_NAME}
    PRIVATE
    source/common/coordinate.cpp
    source/common/message.cpp
    source/planner.cpp
    source/controller.cpp
    source/instr_move.cpp
    source/instr_comms.cpp
    protocols/cube.pb.c
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -fno-exceptions ${COLORS})