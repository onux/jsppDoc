#include "MenuVisitor.h"
#include "../Utils/Utils.h"

using namespace jspp::docgen;
using namespace jspp::parser;

void MenuVisitor::visit(DocComment* node) {
    this->currentDocComment = node;

    const bool isOverloadComment = node->text.find("@overload") !=
                                   std::string::npos;
    if (isOverloadComment) {
        this->saveOverload(node);
    }
}

void MenuVisitor::visit(ModuleDeclaration* node) {
    if (!isDocumented(node)) {
        visitChildren(node);
        return;
    }

    this->modules.push_back(node->id->name);

    const bool isPrefixModule = node->isSplit;
    if (isPrefixModule) {
        visitChildren(node);
    }
    else {
        this->inModule = true;

        const std::string id = this->getIdentifier(node);
        this->output << "<menu name=\"" << id << "\" slug=\"" << id << "\">";
        visitChildren(node);
        for (auto& pair : this->members) {
            const std::string itemXml = pair.second;
            this->output << itemXml;
        }
        this->output << "</menu>";

        this->inModule = false;
    }

    this->modules.pop_back();
}

void MenuVisitor::visit(ClassDeclaration* node) {
    if (!isDocumented(node)) {
        visitChildren(node);
        return;
    }

    this->userDefinedTypes.push_back(node->id->name);

    const std::string id    = this->getIdentifier(node);
    const std::string title = this->getGenericTitle<ClassDeclaration>(node);
    const std::string tag   = this->inModule ? "item" : "menu";

    this->output
        << "<"
        << tag
        << " name=\""
        << utils::escapeXML(title)
        << "\" slug=\""
        << id
        << "\">";

    this->clearDocComment();
    visitChildren(node);

    if (this->hasConstructor) {
        this->output << "<item name=\"(Constructor)\" slug=\"constructor\" />";
    }

    for (auto& pair : this->members) {
        const std::string itemXml = pair.second;
        this->output << itemXml;
    }
    this->output << "</" << tag << ">";

    this->hasConstructor = false;
    this->userDefinedTypes.pop_back();
}

void MenuVisitor::visit(InterfaceDeclaration* node) {
    if (!isDocumented(node)) {
        visitChildren(node);
        return;
    }

    this->userDefinedTypes.push_back(node->id->name);

    const std::string id    = this->getIdentifier(node);
    const std::string title = this->getGenericTitle<InterfaceDeclaration>(node);
    const std::string tag   = this->inModule ? "item" : "menu";

    this->output
        << "<"
        << tag
        << " name=\""
        << utils::escapeXML(title)
        << "\" slug=\""
        << id
        << "\">";

    this->clearDocComment();
    visitChildren(node);

    for (auto& pair : this->members) {
        const std::string itemXml = pair.second;
        this->output << itemXml;
    }
    this->output << "</" << tag << ">";

    this->userDefinedTypes.pop_back();
}

void MenuVisitor::visit(ConstructorDeclaration* node) {
    const std::string id = this->getIdentifier(node);

    if (!isDocumented(node, id, this->overloadTags)) {
        return;
    }

    this->hasConstructor = true;

    this->clearDocComment();
}

void MenuVisitor::visit(FunctionDeclaration* node) {
    const std::string id = this->getIdentifier(node);

    if (!isDocumented(node, id, this->overloadTags)) {
        return;
    }

    const std::string fqn = this->getFQN(node);
    const std::string xml = "<item name=\"" + id + "\" slug=\"" + id + "\" />";
    this->members.insert({ fqn, xml });

    this->clearDocComment();
}

void MenuVisitor::visit(VariableDeclaration* node) {
    if (!isDocumented(node)) {
        return;
    }

    const bool multipleVarsDeclared = node->declarations.size() != 1;
    if (multipleVarsDeclared) {
        return;
    }

    VariableDeclarator decl = *node->declarations[0];
    const std::string id  = this->getIdentifier(&decl);
    const std::string fqn = this->getFQN(&decl);
    const std::string xml = "<item name=\"" + id + "\" slug=\"" + id + "\" />";
    this->members.insert({ fqn, xml });

    this->clearDocComment();
}

void MenuVisitor::visit(EnumDeclaration* node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string id = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);
    const std::string xml = "<item name=\"" + id + "\" slug=\"" + id + "\" />";
    this->members.insert({ fqn, xml });

    this->clearDocComment();
}

void MenuVisitor::saveOverload(jspp::parser::DocComment* node) {
    OverloadTagCommentData comment{ this->currentDocComment->text };
    const DocCommentTags& tags = comment.tags();

    this->overloadTags.insert(tags.overload);
}