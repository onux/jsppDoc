#include "DocCommentMixin.h"

using namespace jspp::parser;

bool jspp::docgen::DocCommentMixin::isDocumented(const Node& node) {
    const bool commentIsBeforeNode = this->currentDocComment &&
                                     this->currentDocComment->isBefore(node);
    if (!commentIsBeforeNode) {
        this->clearDocComment();
        return false;
    }

    return true;
}

bool jspp::docgen::DocCommentMixin::isDocumented(const Node& node,
                                                 const std::string& name,
                                                 const overload_tags_t& overloadTags)
{
    const bool isOverloadable = node.is<FunctionDeclaration>() ||
                                node.is<ConstructorDeclaration>();
    const bool overloadTagExists = overloadTags.find(name) !=
                                   overloadTags.end();
    if (isOverloadable && overloadTagExists) {
        return true;
    }

    const bool commentIsBeforeNode = this->currentDocComment &&
                                     this->currentDocComment->isBefore(node);
    if (!commentIsBeforeNode) {
        this->clearDocComment();
        return false;
    }

    return true;
}

void jspp::docgen::DocCommentMixin::clearDocComment() {
    this->currentDocComment = nullptr;
}