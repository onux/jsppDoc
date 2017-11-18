#include "Name.h"

using namespace jspp::docgen;

NameMixin::NameMixin(const std::string& name, const std::string& fqn)
{
    this->name = name;
    this->fqn  = fqn;
}

std::string NameMixin::getName() const {
    return this->name;
}
std::string NameMixin::getFQN() const {
    return this->fqn;
}