#include "NameBuilderMixin.h"
#include "Utils.h"

using namespace jspp::docgen;
using namespace jspp::parser;

const std::string NameBuilderMixin::getIdentifier(Node* node) const {
    if (node->is<ModuleDeclaration>()) {
        return node->as<ModuleDeclaration>()->id->name;
    }
    if (node->is<ClassDeclaration>()) {
        return node->as<ClassDeclaration>()->id->name;
    }
    if (node->is<ConstructorDeclaration>()) {
        assert(this->classes.size() != 0);
        return this->classes.back();
    }
    if (node->is<FunctionDeclaration>()) {
        return node->as<FunctionDeclaration>()->id->name;
    }
    if (node->is<VariableDeclarator>()) {
        return node->as<VariableDeclarator>()->id->name;
    }

    return "";
}

const std::string NameBuilderMixin::getFQN(Node* node) const {
    if (node->is<ModuleDeclaration>()) {
        return utils::join(this->modules, ".");
    }
    if (node->is<ClassDeclaration>()) {
        std::string result = utils::join(this->modules, ".");

        if (this->classes.size() != 0) {
            result += ".";
            result += utils::join(this->classes, ".");
        }

        return result;
    }
    if (node->is<ConstructorDeclaration>()) {
        const bool isClassMember = this->classes.size() != 0;
        assert(isClassMember);

        std::string result;
        if (this->modules.size() != 0) {
            result = utils::join(this->modules, ".");
            result += ".";
        }
        if (this->classes.size() != 0) {
            result += utils::join(this->classes, ".");
            result += ".";
        }
        result += this->classes.back();

        return result;
    }
    if (node->is<VariableDeclarator>() || node->is<FunctionDeclaration>()) {
        const bool isClassMember = this->classes.size() != 0;
        if (node->is<VariableDeclarator>()) {
            assert(isClassMember);
        }

        std::string result;
        if (this->modules.size() != 0) {
            result = utils::join(this->modules, ".");
            result += ".";
        }
        if (this->classes.size() != 0) {
            result += utils::join(this->classes, ".");
            result += ".";
        }
        result += node->is<VariableDeclarator>() ?
                    dynamic_cast<VariableDeclarator *>(node)->id->name
                    :
                    dynamic_cast<FunctionDeclaration *>(node)->id->name;

        return result;
    }

    return "";
}