#include "DocumentationText.h"
#include "../../CommentParser.h"

using namespace jspp::docgen;

DocumentationTextMixin::DocumentationTextMixin(const std::string& docComment)
{
    CommentParser parser;
    this->text                =   parser.parseDocCommentText(docComment);
    this->documentation_tags  =   parser.parseDocCommentTags(this->text);
    this->body_text           =   parser.parseDocCommentBodyText(this->text);
}

std::string DocumentationTextMixin::getText() const {
    return this->text;
}
std::string DocumentationTextMixin::getBodyText() const {
    return this->body_text;
}
void DocumentationTextMixin::setBodyText(const std::string& text) {
    this->body_text = text;
}
const jspp::docgen::DocCommentTags& DocumentationTextMixin::tags() const {
    return *this->documentation_tags;
}
jspp::docgen::DocCommentTags& DocumentationTextMixin::writableTags() const {
    return *this->documentation_tags;
}