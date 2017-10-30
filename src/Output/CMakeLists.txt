add_subdirectory(Mixins)

set(
    OUTPUT_SOURCE_FILES
        Mixins

        DocVisitor.cc
        DocVisitor.h
        DocVisitor-utils.cc
        MenuBuilder.cc
        MenuBuilder.h
        MenuVisitor.cc
        MenuVisitor.h
        OutputBuilder.cc
        OutputBuilder.h
)

add_library(Output ${OUTPUT_SOURCE_FILES})
