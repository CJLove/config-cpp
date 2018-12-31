cmake_minimum_required(VERSION 3.1)

project(config-cpp)

include_directories( 
    ${CMAKE_SOURCE_DIR}/include 
    /usr/local/include
    .
)

set (config-cpp_SRC
    config-cpp.cpp
    jsonHandler.cpp
    yamlHandler.cpp
    util.cpp
)

add_library( config-cpp ${config-cpp_SRC} )

target_link_libraries( config-cpp pthread )
target_link_libraries( config-cpp yaml-cpp )
target_link_libraries( config-cpp nlohmann_json::nlohmann_json )


