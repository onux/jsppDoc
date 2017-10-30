#include "MenuBuilder.h"

using namespace jspp::docgen;

const std::string MenuBuilder::getOutput() const {
    return this->output.str();
}