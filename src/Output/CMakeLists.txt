set(
    OUTPUT_SOURCE_FILES
        DocCommentMixin.cc
		DocCommentMixin.h
		DocVisitor.cc
		DocVisitor.h
		DocVisitor-utils.cc
		MenuBuilder.cc
		MenuBuilder.h
		MenuVisitor.cc
		MenuVisitor.h
		NameBuilderMixin.cc
		NameBuilderMixin.h
		OutputBuilder.cc
		OutputBuilder.h
)

add_library(Output ${OUTPUT_SOURCE_FILES})
