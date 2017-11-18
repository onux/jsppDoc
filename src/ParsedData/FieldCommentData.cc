#include "FieldCommentData.h"

using namespace jspp::docgen;

FieldCommentData::FieldCommentData(const std::string& name,
                                   const std::string& fqn,
                                   const std::string& datatype,
                                   const std::string& docComment,
                                   const std::bitset<10>& modifiers)
    :
    NameMixin(name, fqn),
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment),

    datatype(datatype)
{
}

std::string jspp::docgen::FieldCommentData::getDatatype() const {
    return this->datatype;
}