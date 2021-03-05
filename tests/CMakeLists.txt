cmake_minimum_required(VERSION 3.12)

project(cube_firmware_tests
    VERSION 1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# get Catch2
Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.4)
FetchContent_GetProperties(Catch2)
if(NOT catch2_POPULATED)
	FetchContent_Populate(Catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
endif()


# Explicitly enable colors in GCC/Clang – this is mainly important for the Ninja
# builds (with makefiles the compiler is able to autodetect if it runs on
# terminal)
set(COLORS "")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(COLORS "-fdiagnostics-color=always")
endif()


set(CUBE_LIB_DIR ../cube_lib)

include_directories(${CUBE_LIB_DIR}/include)
add_library(cube_lib ${CUBE_LIB_DIR}/source/planner.cpp ${CUBE_LIB_DIR}/source/common/coordinate.cpp)

add_executable(tests test_main.cpp test_coordinate.cpp test_planner.cpp)
target_compile_options(tests PRIVATE -Wall -Wextra -pedantic  ${COLORS})
target_link_libraries(tests Catch2::Catch2 cube_lib)


# run the tests automatically, just for convinience
add_custom_target(run-tests ALL ./tests --use-colour yes
                    DEPENDS tests
                    COMMENT "Running tests"
                    WORKING_DIRECTORY ${CMAKE_BUILD_DIR}
                    VERBATIM)
