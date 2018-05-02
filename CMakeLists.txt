cmake_minimum_required (VERSION 2.8.10)
project (docgen CXX)

include(libs/common/common-settings.cmake)
set(PARSER_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

option(NO_TESTS OFF)
option(GPROF OFF)
option(COVERAGE ON)

set(PARSER_LIB_ONLY ON CACHE BOOL "Build only main parser lib, don't build tests and extra binaries" FORCE)
add_subdirectory(libs/jspp-parser "${CMAKE_CURRENT_BINARY_DIR}/jspp-parser")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/common/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/symbol-table/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/jspp-parser/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/pcre)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/sundown/cpp)

set(DEPENDS_LIB_LIST jspp-parser jspp-symbol-table jspp-common)
set(DOCGEN_LIBS Utils OutputMixins Output ParsedDataMixins ParsedData CommentParser Emitters jspp-docgen-lib)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(coverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()
endif()

add_subdirectory(src)
add_subdirectory(libs/pcre)
add_subdirectory(libs/sundown)

if (NOT NO_TESTS)
    add_subdirectory(test)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(
        COVERAGE_EXCLUDES
        '/usr/*'
        '${CMAKE_CURRENT_BINARY_DIR}/*'
        '${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc'
        '${CMAKE_CURRENT_SOURCE_DIR}/libs/*'
        '${CMAKE_CURRENT_SOURCE_DIR}/test/*'
    )
    SETUP_TARGET_FOR_COVERAGE(
        NAME docgen_coverage
        EXECUTABLE tests
        DEPENDENCIES 
    )
endif()
