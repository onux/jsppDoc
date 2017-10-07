//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "CommentData.h"
#include "Utils.h"

using namespace jspp::parser;

void jspp::docgen::DocVisitor::visit(jspp::parser::DocComment* node) {
    this->currentDocComment = node;
}

void jspp::docgen::DocVisitor::visit(ModuleDeclaration* node) {
    this->modules.push_back(node->id->name);

    this->buildDocument(node);
    this->clearModifiers();

    visitChildren(node);
    
    this->modules.pop_back();
}

void jspp::docgen::DocVisitor::visit(ClassDeclaration* node) {
    this->classes.push_back(node->id->name);

    this->buildDocument(node);
    this->clearModifiers();
    this->clearDocComment();

    visitChildren(node);

    this->classes.pop_back();
}

void jspp::docgen::DocVisitor::visit(FunctionDeclaration* node) {
}

void jspp::docgen::DocVisitor::visit(VariableStatement* node) {
}

void jspp::docgen::DocVisitor::visit(StatementModifier* node) {
    StatementModifier* modifier = node;

    do {
        this->modifiers.set(static_cast<int>(modifier->modifier));

        Statement* modified = modifier->modified.get();
        if (nullptr == modified) break;
        modifier = dynamic_cast<StatementModifier*>(modified);
    }
    while (nullptr != modifier);

    visitChildren(node);
}

std::string jspp::docgen::DocVisitor::getFQN(Node* node) const {
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

    return "";
}

void jspp::docgen::DocVisitor::buildDocument(Node* node) {
    const bool isDocumented = this->currentDocComment &&
                              this->currentDocComment->isBefore(node);
    if (!isDocumented) {
        return;
    }

    auto comment = std::make_shared<CommentData>(
        node,
        this->getFQN(node),
        this->currentDocComment->text,
        this->modifiers
    );

    this->documented.push(comment);
}

auto jspp::docgen::DocVisitor::getDocuments() const ->
    std::queue<std::shared_ptr<jspp::docgen::CommentData>>
{
    return this->documented;
}

void jspp::docgen::DocVisitor::clearDocComment() {
    this->currentDocComment = nullptr;
}

void jspp::docgen::DocVisitor::clearModifiers() {
    this->modifiers.reset();
}