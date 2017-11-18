#include "OverloadableCommentData.h"

using namespace jspp::docgen;

OverloadableCommentData::OverloadableCommentData(const std::vector<std::string>& param_types,
                                                 const std::string& docComment,
                                                 const std::bitset<10>& modifiers)
    :
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment),

    param_types(param_types)
{
}

std::string OverloadableCommentData::getParameterType(size_t index) const {
    return this->param_types.at(index);
}