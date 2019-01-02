cmake_minimum_required(VERSION 3.1)

project(config-cpp)

#---------------------------------------------------------------------------------------
# Project options
#---------------------------------------------------------------------------------------
option(YAML_SUPPORT "Support YAML config files" ON)
option(JSON_SUPPORT "Support JSON config files" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build unit tests." ON)
option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
option(BUILD_STATIC_LIBS "Build Static Libraries" ON)

option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)


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

#---------------------------------------------------------------------------------------
# Dependencies
#---------------------------------------------------------------------------------------

if (YAML_SUPPORT)
    # yaml-cpp library dependency
    find_package(yaml-cpp CONFIG REQUIRED)

    add_definitions(-DYAML_SUPPORT)
endif(YAML_SUPPORT)

if (JSON_SUPPORT)
    # nlohman_json library dependency
    find_package(nlohmann_json 3.2.0 REQUIRED)

    add_definitions(-DJSON_SUPPORT)
endif(JSON_SUPPORT)

# Threads
find_package(Threads)

# Google Test
find_package(GTest)

#---------------------------------------------------------------------------------------
# Sources, headers, directories and libs
#---------------------------------------------------------------------------------------
# From http://www.cmake.org/pipermail/cmake/2010-March/035992.html:
# function to collect all the sources from sub-directories
# into a single list
function(add_sources)
  get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY SRCS_LIST
      BRIEF_DOCS "List of source files"
      FULL_DOCS "List of all source files in the entire project")
  endif()
  # make absolute paths
  set(SRCS)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND SRCS "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY SRCS_LIST "${SRCS}")
endfunction(add_sources)

file(GLOB sources "src/[a-zA-Z]*.cpp")
file(GLOB_RECURSE public_headers "include/config-cpp/[a-zA-Z]*.h")
file(GLOB private_headers "src/[a-zA-Z]*.h")
file(GLOB examples "examples/[a-zA-Z]*.[ch]*")
file(GLOB tests "test/[a-zA-Z]*.cpp")

set(library_sources
    ${sourcse}
    ${public_headers}
    ${private_headers}
    ${examples}
    ${tests}
)
add_sources(${library_sources})

if(VERBOSE)
    message(STATUS "sources: ${sources}")
    message(STATUS "public_headers: ${public_headers}")
    message(STATUS "private_headers: ${private_headers}")
    message(STATUS "examples: ${examples}")
    message(STATUS "tests: ${tests}")
endif(VERBOSE)

add_subdirectory(src)
if (BUILD_EXAMPLES)
    add_subdirectory(example)
endif(BUILD_EXAMPLES)
if (BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)

#---------------------------------------------------------------------------------------
# Addons
#---------------------------------------------------------------------------------------
# clang-tidy
if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
  else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
  endif()
endif()

# cppcheck
if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=syntaxError --enable=all --inconclusive)
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
endif()

# Formatting
get_property(all_sources GLOBAL PROPERTY SRCS_LIST)
add_custom_target(format
    COMMAND clang-format --style=file -i ${all_sources}
    COMMENT "Running clang-format"
    VERBATIM)





