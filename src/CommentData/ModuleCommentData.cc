#include "ModuleCommentData.h"

using namespace jspp::docgen;

ModuleCommentData::ModuleCommentData(const std::string& name,
                                     const std::string& fqn,
                                     const std::string& docComment,
                                     const std::bitset<10>& modifiers)
    :
    NameMixin(name, fqn),
    ModifiersMixin(modifiers),
    DocumentationTextMixin(docComment)
{
}