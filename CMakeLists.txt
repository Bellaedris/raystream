include(CMakePrintHelpers)
#cmake_print_variables(${SOURCES_FILES})
cmake_minimum_required(VERSION 3.28)
project(RaytracingStream)

set(CMAKE_CXX_STANDARD 20)

find_package(glm CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB_RECURSE SOURCE_FILES
        "Source/*.h"
        "Source/*.cpp"
)

file(GLOB_RECURSE TEST_FILES
    "Tests/**/*.cpp"
    "Source/*.cpp"
)
list(REMOVE_ITEM TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Source/main.cpp")
cmake_print_variables(${TEST_FILES})

#GTEST
add_executable(
        RayTest
        ${TEST_FILES}
)
target_link_libraries(
        RayTest
        glm::glm GTest::gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(RayTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(RaytracingStream ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR}/Include)
target_link_libraries(RaytracingStream PRIVATE glm::glm)
