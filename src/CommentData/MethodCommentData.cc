#include "MethodCommentData.h"

using namespace jspp::docgen;

MethodCommentData::MethodCommentData(const std::string& name,
                                     const std::string& fqn,
                                     std::vector<std::string> param_types,
                                     const std::string return_type,
                                     const std::string& docComment,
                                     const std::bitset<10>& modifiers)
    :
    NameMixin(name, fqn),
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment),

    param_types(param_types),
    return_type(return_type)
{
}

std::string MethodCommentData::getReturnType() const {
    return this->return_type;
}

std::string MethodCommentData::getParameterType(size_t index) const {
    return this->param_types.at(index);
}