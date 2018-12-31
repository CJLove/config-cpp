cmake_minimum_required(VERSION 3.1)

project(example)

include_directories( 
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/include
)

add_executable( yaml yaml.cpp yaml-config.cpp )

target_link_libraries( yaml config-cpp )

add_executable( json json.cpp json-config.cpp )

target_link_libraries( json config-cpp )


