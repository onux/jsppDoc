add_subdirectory(CommentData)
add_subdirectory(CommentParser)
add_subdirectory(Emitters)

set(
    SOURCE_FILES
        CommentData
        CommentParser
        Emitters

        Application.cc
        Application.h
        DocCommentMixin.cc
        DocCommentMixin.h
        DocCommentTags.h
        DocVisitor.cc
        DocVisitor-utils.cc
        DocVisitor.h
        MenuBuilder.cc
        MenuBuilder.h
        MenuVisitor.cc
        MenuVisitor.h
        Modifiers.h
        NameBuilderMixin.cc
        NameBuilderMixin.h
        OutputBuilder.cc
        OutputBuilder.h
        Utils.cc
        Utils.h
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
