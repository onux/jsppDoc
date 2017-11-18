#include "EnumMemberCommentData.h"

using namespace jspp::docgen;

EnumMemberCommentData::EnumMemberCommentData(const std::string& name,
                                             const std::string& docComment)
    :
    DocumentationTextMixin(docComment),

    name(name)
{
}

std::string jspp::docgen::EnumMemberCommentData::getName() const {
    return this->name;
}