//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "DocCommentTags.h"
#include "Utils.h"
#include "CommentData/includes.h"

using namespace jspp::docgen;
using namespace jspp::parser;

void jspp::docgen::DocVisitor::saveOverload(jspp::parser::DocComment* node) {
    auto comment = std::unique_ptr<OverloadTagCommentData>(
        new OverloadTagCommentData(this->currentDocComment->text)
    );
    const DocCommentTags& tags = comment->tags();

    this->overloadTags.insert(
        std::make_pair(tags.overload, std::move(comment))
    );
}

void jspp::docgen::DocVisitor::saveDocument(ModuleDeclaration* node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);

    auto comment = std::unique_ptr<ModuleCommentData>(
        new ModuleCommentData(
            name,
            fqn,
            this->currentDocComment->text,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(ClassDeclaration* node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);

    auto comment = std::unique_ptr<ClassCommentData>(
        new ClassCommentData(
            name,
            fqn,
            this->currentDocComment->text,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(VariableDeclarator* node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);

    auto comment = std::unique_ptr<FieldCommentData>(
        new FieldCommentData(
            name,
            fqn,
            this->lastDatatype,
            this->currentDocComment->text,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(ConstructorDeclaration* node) {
    ;;;
}

void jspp::docgen::DocVisitor::saveDocument(FunctionDeclaration* node) {
    const std::string name = this->getIdentifier(node);
    const bool overloadTagExists = this->overloadTags.find(name) !=
                                   this->overloadTags.end();
    if (!overloadTagExists && !isDocumented(node)) {
        return;
    }

    const std::string fqn = this->getFQN(node);
    const std::string returnType = this->lastDatatype;

    auto comment = std::unique_ptr<MethodCommentData>(
        new MethodCommentData(
            name,
            fqn,
            this->params,
            returnType,
            this->currentDocComment ? this->currentDocComment->text : "",
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

auto jspp::docgen::DocVisitor::getDocuments() ->
    std::vector<std::unique_ptr<CommentData>>
{
    std::vector<doc_comment_t> result;
    std::unordered_set<std::string> processedKeys;

    for (auto& pair : this->documented) {
        const std::string& key = pair.first;
        if (processedKeys.find(key) != processedKeys.end()) {
            continue;
        }
        processedKeys.insert(key);

        const bool isOverloaded = this->documented.count(key) > 1;
        if (!isOverloaded) {
            result.push_back(std::move(pair.second));
            continue;
        }

        std::unique_ptr<CommentData>& document = pair.second;
        if (document->is<MethodCommentData>()) {
            auto method_doc = dynamic_cast<MethodCommentData*>(document.get());
            const std::string methodName = method_doc->getName();
            const std::string methodFQN  = method_doc->getFQN();

            this->combineMethodDocs(result, methodName, methodFQN);
            continue;
        }
        if (document->is<ConstructorCommentData>()) {
            // TODO
            ;;;
        }
    }

    return result;
}

void jspp::docgen::DocVisitor::combineMethodDocs(std::vector<doc_comment_t>& results,
                                                 const std::string& name,
                                                 const std::string& fqn)
{
    std::vector<std::unique_ptr<MethodCommentData>> overloads;
    std::unique_ptr<OverloadedMethodCommentData> combinedDoc;

    auto iterators = this->documented.equal_range(fqn);
    for (auto it = iterators.first, end = iterators.second; it != end; ++it) {
        auto method_doc = CommentData::dynamic_unique_ptr_cast<MethodCommentData>(
            std::move(it->second)
        );
        overloads.push_back(std::move(method_doc));
    }

    const bool overloadTagExists = overloadTags.find(name) !=
                                   overloadTags.end();
    if (overloadTagExists) {
        combinedDoc = std::unique_ptr<OverloadedMethodCommentData>(
            new OverloadedMethodCommentData(
                name,
                fqn,
                std::move(overloads),
                overloadTags.at(name).get()
            )
        );
    }
    else {
        combinedDoc = std::unique_ptr<OverloadedMethodCommentData>(
            new OverloadedMethodCommentData(
                name,
                fqn,
                std::move(overloads)
            )
        );
    }

    results.push_back(std::move(combinedDoc));
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