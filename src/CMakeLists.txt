set(
    SOURCE_FILES
    Application.cc
    Application.h
    CommentData.cc
    CommentData.h
    CommentParser.cc
    CommentParser.h
    DocCommentTags.h
    DocVisitor.cc
    DocVisitor.h
    FileEmitter.cc
    FileEmitter.h
    Modifiers.h
    OutputBuilder.cc
    OutputBuilder.h
    OutputEmitter.h
    Utils.cc
    Utils.h
)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "-g --coverage")
endif()

add_library(jspp-docgen-lib ${SOURCE_FILES})

set(TARGET_NAME "docgen")
add_executable(${TARGET_NAME} main.cc)

target_link_libraries (${TARGET_NAME} jspp-docgen-lib pcrecpp ${DEPENDS_LIB_LIST})

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_link_libraries (${TARGET_NAME} gcov)
endif()
