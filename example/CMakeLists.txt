cmake_minimum_required(VERSION 3.17)

project(example)

include_directories( 
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/include
)

if (YAML_SUPPORT)
    add_executable( yaml yaml.cpp yaml-config.cpp )

    target_link_libraries( yaml config-cpp )
endif(YAML_SUPPORT)

if (TOML_SUPPORT)
    add_executable( toml toml.cpp toml-config.cpp )

    target_link_libraries( toml config-cpp )
endif(TOML_SUPPORT)

if (JSON_SUPPORT)
    add_executable( json json.cpp json-config.cpp )

    target_link_libraries( json config-cpp )

    add_executable( watcher watcher.cpp )

    target_link_libraries( watcher config-cpp )

    install(TARGETS watcher RUNTIME DESTINATION bin)
endif(JSON_SUPPORT)


