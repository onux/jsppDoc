//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "Utils.h"
#include "CommentData/includes.h"

using namespace jspp::docgen;
using namespace jspp::parser;

void jspp::docgen::DocVisitor::saveOverload(jspp::parser::DocComment* node) {
    auto comment = std::unique_ptr<OverloadTagCommentData>(
        new OverloadTagCommentData(this->currentDocComment->text)
    );

    this->overloadComments.insert(
        std::make_pair(comment->getTags()->overload_name, std::move(comment))
    );
}

void jspp::docgen::DocVisitor::buildDocument(ModuleDeclaration* node) {
    const bool isDocumented = this->currentDocComment &&
                              this->currentDocComment->isBefore(node);
    if (!isDocumented) {
        return;
    }

    auto comment = std::unique_ptr<ModuleCommentData>(
        new ModuleCommentData(
            this->getIdentifier(node),
            this->getFQN(node),
            this->currentDocComment->text,
            this->modifiers
        )
    );
    this->documented.insert(
        std::make_pair(this->getFQN(node), std::move(comment))
    );
}

void jspp::docgen::DocVisitor::buildDocument(ClassDeclaration* node) {
    const bool isDocumented = this->currentDocComment &&
                              this->currentDocComment->isBefore(node);
    if (!isDocumented) {
        return;
    }

    auto comment = std::unique_ptr<ClassCommentData>(
        new ClassCommentData(
            this->getIdentifier(node),
            this->getFQN(node),
            this->currentDocComment->text,
            this->modifiers
        )
    );
    this->documented.insert(
        std::make_pair(this->getFQN(node), std::move(comment))
    );
}

void jspp::docgen::DocVisitor::buildDocument(VariableDeclarator* node) {
    const bool isDocumented = this->currentDocComment &&
                              this->currentDocComment->isBefore(node);
    if (!isDocumented) {
        return;
    }

    auto comment = std::unique_ptr<FieldCommentData>(
        new FieldCommentData(
            this->getIdentifier(node),
            this->getFQN(node),
            this->lastDatatype,
            this->currentDocComment->text,
            this->modifiers
        )
    );
    this->documented.insert(
        std::make_pair(this->getFQN(node), std::move(comment))
    );
}


void jspp::docgen::DocVisitor::buildDocument(ConstructorDeclaration* node) {
    ;;;
}

void jspp::docgen::DocVisitor::buildDocument(FunctionDeclaration* node) {
    // const std::string funcName = this->getIdentifier(node);
    // const std::string returnType = this->lastDatatype;

    // auto comment = std::unique_ptr<MethodCommentData>(
    //     this->getIdentifier(node),
    //     this->getFQN(node),
    //     this->params,
    //     returnType,
    //     this->currentDocComment->text,
    //     this->modifiers
    // );
    // this->documented.insert({ this->getFQN(node), comment });
}

std::vector<std::unique_ptr<CommentData>> jspp::docgen::DocVisitor::getDocuments() {
    std::vector<doc_comment_t> result;
    std::unordered_set<std::string> processedKeys;

    for (auto& pair : this->documented) {
        const std::string& key = pair.first;
        if (processedKeys.find(key) != processedKeys.end()) {
            continue;
        }

        const size_t count = this->documented.count(key);
        const bool isOverloaded = count > 1;
        if (!isOverloaded) {
            result.push_back(std::move(pair.second));
        }
        else {
            // TODO: combine this->documented into std::queue, including merging
            // SingleMethodCommentData into OverloadedMethodCommentData and
            // SingleConstructorCommentData into OverloadedConstructorCommentData
            ;;;
        }

        processedKeys.insert(key);
    }

    return result;
}

void jspp::docgen::DocVisitor::clearDocComment() {
    this->currentDocComment = nullptr;
}

void jspp::docgen::DocVisitor::clearModifiers() {
    this->modifiers.reset();
}

void jspp::docgen::DocVisitor::clearParameters() {
    this->params.clear();
}

std::string jspp::docgen::DocVisitor::getIdentifier(Node* node) const {
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