add_subdirectory(CommentParser)
add_subdirectory(Emitters)
add_subdirectory(Output)
add_subdirectory(ParsedData)
add_subdirectory(Utils)

set(
    SOURCE_FILES
        CommentParser
        Emitters
        Output
        ParsedData
        Utils

        main.cc
)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "-g --coverage")
endif()

add_library(jspp-docgen-lib ${SOURCE_FILES})

set(TARGET_NAME "docgen")
add_executable(${TARGET_NAME} main.cc)

target_link_libraries(${TARGET_NAME} ${DOCGEN_LIBS} ${DOCGEN_LIBS} pcrecpp sundown ${DEPENDS_LIB_LIST})

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_link_libraries(${TARGET_NAME} gcov)
endif()
