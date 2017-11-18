#include "InterfaceCommentData.h"

using namespace jspp::docgen;

InterfaceCommentData::InterfaceCommentData(const std::string& name,
                                           const std::string& title,
                                           const std::string& fqn,
                                           const std::string& docComment,
                                           const std::bitset<10>& modifiers)
    :
    NameMixin(name, fqn),
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment),

    title(title)
{
}

std::string InterfaceCommentData::getTitle() const {
    return this->title;
}