//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "CommentData/CommentData.h"
#include "Utils/Utils.h"
#include <jspp-parser/Utils.h>

using namespace jspp::parser;

void jspp::docgen::DocVisitor::visit(jspp::parser::DocComment* node) {
    this->currentDocComment = node;

    const bool isOverloadComment = node->text.find("@overload") !=
                                   std::string::npos;
    if (isOverloadComment) {
        this->saveOverload();
        this->clearDocComment();
    }
}

void jspp::docgen::DocVisitor::visit(ModuleDeclaration* node) {
    this->modules.push_back(node->id->name);

    const bool isPrefixModule = node->isSplit;
    if (!isPrefixModule) {
        this->saveDocument(*node);
        this->clearModifiers();
    }

    visitChildren(node);

    this->modules.pop_back();
}

void jspp::docgen::DocVisitor::visit(ClassDeclaration* node) {
    this->userDefinedTypes.push_back(node->id->name);

    this->saveDocument(*node);
    this->clearModifiers();
    this->clearDocComment();

    visitChildren(node);

    this->userDefinedTypes.pop_back();
}

void jspp::docgen::DocVisitor::visit(InterfaceDeclaration* node) {
    this->userDefinedTypes.push_back(node->id->name);

    this->saveDocument(*node);
    this->clearModifiers();
    this->clearDocComment();

    visitChildren(node);

    this->userDefinedTypes.pop_back();
}

void jspp::docgen::DocVisitor::visit(ConstructorDeclaration* node) {
    assert(this->userDefinedTypes.size() != 0);

    for (auto& param : node->params) {
        this->params.push_back(this->getParameterType(param));
    }

    this->saveDocument(*node);
    this->clearParameters();
    this->clearModifiers();
    this->clearDocComment();

    visitChildren(node);
}

void jspp::docgen::DocVisitor::visit(FunctionDeclaration* node) {
    const bool isFreeFunction = this->modules.size() == 0 &&
                                this->userDefinedTypes.size() == 0;
    if (isFreeFunction) {
        // TODO: output warning for free functions
        return;
    }

    this->lastDatatype = parser::Utils::annotationTypeToString(node->kind.get());
    for (auto& param : node->params) {
        this->params.push_back(this->getParameterType(param));
    }

    this->saveDocument(*node);
    this->clearParameters();
    this->clearModifiers();
    this->clearDocComment();
}

void jspp::docgen::DocVisitor::visit(VariableDeclaration* node) {
    const bool multipleVarsDeclared = node->declarations.size() != 1;
    if (multipleVarsDeclared) {
        // TODO: output warning for multiple var declarations on the same line
        return;
    }

    const VariableDeclarator& decl = *node->declarations[0];

    this->saveDocument(decl);
    this->clearModifiers();
    this->clearDocComment();
}

void jspp::docgen::DocVisitor::visit(jspp::parser::EnumDeclaration* node) {
    DocComment* const saveDocComment = this->currentDocComment;
    this->clearDocComment();
    visitChildren(node);
    this->currentDocComment = saveDocComment;
    
    this->lastDatatype = parser::Utils::annotationTypeToString(node->kind.get());

    this->saveDocument(*node);
    this->clearModifiers();
    this->clearDocComment();
}

void jspp::docgen::DocVisitor::visit(jspp::parser::Enumerator* node) {
    const std::string name = this->getIdentifier(*node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<EnumMemberCommentData>(
        new EnumMemberCommentData(name, docText)
    );
    this->lastEnumMembers.push_back(std::move(comment));
    this->clearDocComment();
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
    this->clearModifiers();
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