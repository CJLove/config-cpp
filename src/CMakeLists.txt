cmake_minimum_required(VERSION 3.17)

project(config-cpp)

include_directories( 
    ${CMAKE_SOURCE_DIR}/include 
    /usr/local/include
    .
)

if (BUILD_COVERAGE)
    add_definitions( --coverage )
endif (BUILD_COVERAGE)

set (config-cpp_SRC
    config-cpp.cpp
    util.cpp
    value.cpp
    event.cpp
    notification.cpp
    filesystemEvent.cpp
    inotify.cpp
)

if (YAML_SUPPORT)
    set (config-cpp_SRC ${config-cpp_SRC}
        yamlHandler.cpp
    )
endif(YAML_SUPPORT)

if (JSON_SUPPORT)
    set (config-cpp_SRC ${config-cpp_SRC}
        jsonHandler.cpp
    )
endif (JSON_SUPPORT)

if (TOML_SUPPORT)
    set (config-cpp_SRC ${config-cpp_SRC}
        tomlHandler.cpp
    )
endif (TOML_SUPPORT)

if(BUILD_SHARED_LIBS)
    add_library( config-cpp SHARED ${config-cpp_SRC} )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    add_library( config-cpp ${config-cpp_SRC} )
endif(BUILD_STATIC_LIBS)

target_link_libraries( config-cpp pthread )

if (YAML_SUPPORT)
    target_link_libraries( config-cpp yaml-cpp )
endif(YAML_SUPPORT)

if (JSON_SUPPORT)
    target_link_libraries( config-cpp nlohmann_json::nlohmann_json )
endif(JSON_SUPPORT)

    target_link_libraries( config-cpp cxxopts::cxxopts )

if (BUILD_COVERAGE)
    target_link_libraries( config-cpp gcov )
endif (BUILD_COVERAGE)

if (BUILD_SHARED_LIBS)
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
endif(BUILD_SHARED_LIBS)
