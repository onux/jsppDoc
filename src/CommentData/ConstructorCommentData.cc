#include "ConstructorCommentData.h"

using namespace jspp::docgen;

ConstructorCommentData::ConstructorCommentData(const std::string& className,
                                               const std::string& fqn,
                                               const std::vector<std::string>& param_types,
                                               const std::string& docComment,
                                               const std::bitset<10>& modifiers)
    :
    OverloadableCommentData(param_types, docComment, modifiers),

    className(className),
    fqn(fqn)
{
}

std::string ConstructorCommentData::getClassName() const {
    return this->className;
}

std::string ConstructorCommentData::getFQN() const {
    return this->fqn;
}