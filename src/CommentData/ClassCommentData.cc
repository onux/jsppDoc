#include "ClassCommentData.h"

using namespace jspp::docgen;

ClassCommentData::ClassCommentData(const std::string& name,
                                   const std::string& fqn,
                                   const std::string& docComment,
                                   const std::bitset<10>& modifiers)
    :
    NameMixin(name, fqn),
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment)
{
}