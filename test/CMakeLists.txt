cmake_minimum_required (VERSION 3.1)

project (ConfigCppTests)

enable_testing()

include_directories( 
    .
    ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

add_executable ( ConfigCppTests 
    test_util.cpp 
    test_default.cpp
    test_yamlHandler.cpp 
    test_jsonHandler.cpp
)

target_link_libraries( ConfigCppTests gtest gtest_main )

target_link_libraries( ConfigCppTests config-cpp )

add_test (ConfigCppTests ConfigCppTests )