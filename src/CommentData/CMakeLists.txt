add_subdirectory(Mixins)

set(
    COMMENT_DATA_SOURCE_FILES
        Mixins

        ClassCommentData.cc
        ClassCommentData.h
        CommentData.h
        ConstructorCommentData.cc
        ConstructorCommentData.h
        FieldCommentData.cc
        FieldCommentData.h
        MethodCommentData.cc
        MethodCommentData.h
        ModuleCommentData.cc
        ModuleCommentData.h
        OverloadedConstructorCommentData.cc
        OverloadedConstructorCommentData.h
        OverloadedMethodCommentData.cc
        OverloadedMethodCommentData.h
        OverloadTagCommentData.cc
        OverloadTagCommentData.h
)

add_library(CommentData ${COMMENT_DATA_SOURCE_FILES})
