#include "OverloadedMethodCommentData.h"

using namespace jspp::docgen;

OverloadedMethodCommentData::OverloadedMethodCommentData(const std::string& name,
                                                         const std::string& fqn,
                                                         methods_t methods) :
    NameMixin(name, fqn),
    OverloadMixin(std::move(methods))
{
}

OverloadedMethodCommentData::OverloadedMethodCommentData(const std::string& name,
                                                         const std::string& fqn,
                                                         methods_t methods,
                                                         overload_tag_t overload_tag) :
    NameMixin(name, fqn),
    OverloadMixin(std::move(methods), overload_tag)
{
}