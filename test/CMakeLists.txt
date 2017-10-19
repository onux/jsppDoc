include(build-pugixml.cmake)  

include_directories(../libs/libs-catch/include)
include_directories(../libs/pugixml/src)

set(
    SOURCE_FILES
    class.cc
    fields.cc
    main.cc
    methods.cc
    modules.cc
    overload.cc
    test.cc
    CommentParser.test.cc
    Utils.test.cc
)

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests ${DOCGEN_LIBS} ${DOCGEN_LIBS} pcrecpp gcov pugixml ${DEPENDS_LIB_LIST})