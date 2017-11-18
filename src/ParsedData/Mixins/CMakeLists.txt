set(
    COMMENT_DATA_MIXINS
    DocumentationText.cc
    DocumentationText.h
    Modifiers.cc
    Modifiers.h
    Name.cc
    Name.h
    Overload.cc
    Overload.h
)

add_library(ParsedDataMixins ${COMMENT_DATA_MIXINS})