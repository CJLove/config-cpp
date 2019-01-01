cmake_minimum_required(VERSION 3.1)

project(example)

include_directories( 
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/include
)

if (YAML_SUPPORT)
    add_executable( yaml yaml.cpp yaml-config.cpp )

    target_link_libraries( yaml config-cpp )
endif(YAML_SUPPORT)

if (JSON_SUPPORT)
    add_executable( json json.cpp json-config.cpp )

    target_link_libraries( json config-cpp )

    add_executable( watch watch.cpp )

    target_link_libraries( watch config-cpp )

    install(TARGETS watch RUNTIME DESTINATION bin)
endif(JSON_SUPPORT)


