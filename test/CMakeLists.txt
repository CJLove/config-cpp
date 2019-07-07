cmake_minimum_required (VERSION 3.1)

project (ConfigCppTests)

enable_testing()

# Disable clang-tidy checks for unit test code
set(CMAKE_CXX_CLANG_TIDY "")

include_directories( 
    .
    ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set (ConfigCppTests_SRC 
    test_util.cpp 
    test_value.cpp
    test_event.cpp
    test_notification.cpp
    test_inotify.cpp
    test_configcpp.cpp
)

if (YAML_SUPPORT)
    set (ConfigCppTests_SRC ${ConfigCppTests_SRC}
        test_yamlHandler.cpp
    )
endif(YAML_SUPPORT)

if (JSON_SUPPORT)
    set (ConfigCppTests_SRC ${ConfigCppTests_SRC}
        test_jsonHandler.cpp
    )
endif(JSON_SUPPORT)

add_executable ( ConfigCppTests ${ConfigCppTests_SRC} )


target_link_libraries( ConfigCppTests gtest gtest_main )

target_link_libraries( ConfigCppTests config-cpp )

add_test (ConfigCppTests ConfigCppTests )