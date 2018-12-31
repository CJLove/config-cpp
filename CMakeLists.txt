cmake_minimum_required(VERSION 3.1)

project(cpp-yaml-playground)

#---------------------------------------------------------------------------------------
# compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wconversion")
    add_compile_options("-pedantic")
    add_compile_options("-Wfatal-errors")

endif()


# yaml-cpp library dependency
find_package(yaml-cpp CONFIG REQUIRED)

# nlohman_json library dependency
find_package(nlohmann_json 3.2.0 REQUIRED)

# Threads
find_package(Threads)

# Google Test
find_package(GTest)
if (GTEST_FOUND)
    option (BUILD_TESTS "Build unit tests." ON)
endif (GTEST_FOUND)

add_subdirectory(src)

add_subdirectory(example)

if (BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
