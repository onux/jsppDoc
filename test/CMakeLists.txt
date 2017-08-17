include_directories(../libs/libs-catch/include)

set(
    SOURCE_FILES
    main.cc
    modules.cc
    CommentParser.test.cc
    Utils.test.cc
)

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests jspp-docgen-lib pcrecpp gcov ${DEPENDS_LIB_LIST})