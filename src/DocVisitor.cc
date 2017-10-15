//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "CommentData/CommentData.h"
#include "Utils.h"
#include <jspp-parser/Utils.h>

using namespace jspp::parser;

void jspp::docgen::DocVisitor::visit(jspp::parser::DocComment* node) {
    this->currentDocComment = node;

    const bool isOverloadComment = node->text.find("@overload") !=
                                   std::string::npos;
    if (isOverloadComment) {
        this->saveOverload(node);
    }
}

void jspp::docgen::DocVisitor::visit(ModuleDeclaration* node) {
    this->modules.push_back(node->id->name);

    const bool isPrefixModule = node->isSplit;

    if (!isPrefixModule) {
        this->buildDocument(node);
        this->clearModifiers();
    }

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

void jspp::docgen::DocVisitor::visit(VariableDeclaration* node) {
    if (node->declarations.size() != 1) {
        // TODO: output warning for multiple var declarations on the same line
        return;
    }

    std::unique_ptr<VariableDeclarator>& decl = node->declarations[0];

    this->buildDocument(decl.get());
    this->clearModifiers();
    this->clearDocComment();

    visitChildren(node);
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

void jspp::docgen::DocVisitor::visit(IdentifierType* node) {
    this->lastDatatype = parser::Utils::annotationTypeToString(node);
}

void jspp::docgen::DocVisitor::visit(MemberedType* node) {
    this->lastDatatype = parser::Utils::annotationTypeToString(node);
}

void jspp::docgen::DocVisitor::visit(ArrayType* node) {
    this->lastDatatype = parser::Utils::annotationTypeToString(node);
}

void jspp::docgen::DocVisitor::visit(CallbackType* node) {
    this->lastDatatype = parser::Utils::annotationTypeToString(node);
}

void jspp::docgen::DocVisitor::visit(GenericInstantiationType* node) {
    this->lastDatatype = parser::Utils::annotationTypeToString(node);
}