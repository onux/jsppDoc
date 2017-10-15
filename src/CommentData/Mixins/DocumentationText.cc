#include "DocumentationText.h"
#include "../../CommentParser.h"

using namespace jspp::docgen;

DocumentationTextMixin::DocumentationTextMixin(const std::string& docComment)
{
    CommentParser parser;
    this->text          =   parser.parseDocCommentText(docComment);
    this->tags          =   parser.parseDocCommentTags(this->text);
    this->body_text     =   parser.parseDocCommentBodyText(this->text);
}

std::string DocumentationTextMixin::getText() const {
    return this->text;
}
std::string DocumentationTextMixin::getBodyText() const {
    return this->body_text;
}
std::shared_ptr<DocCommentTags> DocumentationTextMixin::getTags() const {
    return this->tags;
}