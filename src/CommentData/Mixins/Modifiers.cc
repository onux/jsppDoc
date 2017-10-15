#include "Modifiers.h"
#include "../../CommentParser.h"

using namespace jspp::docgen;

ModifiersMixin::ModifiersMixin(const std::bitset<10>& modifiers)
{
    CommentParser parser;
    this->modifiers = std::move(parser.parseModifiers(modifiers));
}

std::shared_ptr<Modifiers> ModifiersMixin::getModifiers() const {
    return this->modifiers;
}