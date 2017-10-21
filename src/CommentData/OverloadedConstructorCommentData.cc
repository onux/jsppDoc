#include "OverloadedConstructorCommentData.h"

using namespace jspp::docgen;

OverloadedConstructorCommentData::OverloadedConstructorCommentData(
    const std::string& className,
    const std::string& fqn,
    std::vector<std::unique_ptr<OverloadableCommentData>>&& constructors)
    :
    OverloadMixin(std::move(constructors)),
    
    className(className),
    fqn(fqn)
{
}

OverloadedConstructorCommentData::OverloadedConstructorCommentData(
    const std::string& className,
    const std::string& fqn,
    std::vector<std::unique_ptr<OverloadableCommentData>>&& constructors,
    const OverloadTagCommentData* const overload_tag)
    :
    OverloadMixin(std::move(constructors), overload_tag),
    
    className(className),
    fqn(fqn)
{
}

std::string OverloadedConstructorCommentData::getClassName() const {
    return this->className;
}

std::string OverloadedConstructorCommentData::getFQN() const {
    return this->fqn;
}