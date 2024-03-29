cmake_minimum_required(VERSION 3.12)

project(cube_lib_tests
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

add_subdirectory(../cube_lib ${PROJECT_BINARY_DIR}/cube_lib)

add_executable(${PROJECT_NAME}
    test_main.cpp
    test_coordinate.cpp
    test_planner.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic  ${COLORS})
target_link_libraries(${PROJECT_NAME} Catch2::Catch2 cube_lib)


# run the tests automatically, just for convinience
add_custom_target(run-${PROJECT_NAME} ALL ./${PROJECT_NAME} --use-colour yes
                    DEPENDS ${PROJECT_NAME}
                    COMMENT "Running tests"
                    WORKING_DIRECTORY ${CMAKE_BUILD_DIR}
                    VERBATIM)
