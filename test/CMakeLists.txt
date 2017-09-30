include(build-pugixml.cmake)  

include_directories(../libs/libs-catch/include)
include_directories(../libs/pugixml/src)

set(
    SOURCE_FILES
    main.cc
    modules.cc
    CommentParser.test.cc
    Utils.test.cc
)

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests jspp-docgen-lib pcrecpp gcov pugixml ${DEPENDS_LIB_LIST})
