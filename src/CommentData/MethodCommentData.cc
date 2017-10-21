#include "MethodCommentData.h"

using namespace jspp::docgen;

MethodCommentData::MethodCommentData(const std::string& name,
                                     const std::string& fqn,
                                     const std::vector<std::string>& param_types,
                                     const std::string& return_type,
                                     const std::string& docComment,
                                     const std::bitset<10>& modifiers)
    :
    OverloadableCommentData(param_types, docComment, modifiers),
    NameMixin(name, fqn),

    return_type(return_type)
{
}

std::string MethodCommentData::getReturnType() const {
    return this->return_type;
}