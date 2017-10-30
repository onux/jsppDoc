set(
    OUTPUT_MIXINS_SOURCE_FILES
        DocCommentMixin.cc
        DocCommentMixin.h
        NameBuilderMixin.cc
        NameBuilderMixin.h
)

add_library(OutputMixins ${OUTPUT_MIXINS_SOURCE_FILES})
