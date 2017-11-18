//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "ParsedData/includes.h"
#include "DocCommentData/DocCommentTags.h"
#include <jspp-parser/Utils.h>

using namespace jspp::docgen;
using namespace jspp::parser;

void jspp::docgen::DocVisitor::saveOverload() {
    const std::string docText = this->getDocCommentText();
    auto comment = std::unique_ptr<OverloadTagCommentData>(
        new OverloadTagCommentData(docText)
    );
    const DocCommentTags& tags = comment->tags();

    this->overloadTags.insert(
        std::make_pair(tags.overload, std::move(comment))
    );
}

void jspp::docgen::DocVisitor::saveDocument(const ModuleDeclaration& node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<ModuleCommentData>(
        new ModuleCommentData(
            name,
            fqn,
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(const ClassDeclaration& node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name    = this->getIdentifier(node);
    const std::string title   = this->getGenericTitle<ClassDeclaration>(node);
    const std::string fqn     = this->getFQN(node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<ClassCommentData>(
        new ClassCommentData(
            name,
            title,
            fqn,
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(const InterfaceDeclaration& node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name    = this->getIdentifier(node);
    const std::string title   = this->getGenericTitle<InterfaceDeclaration>(node);
    const std::string fqn     = this->getFQN(node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<InterfaceCommentData>(
        new InterfaceCommentData(
            name,
            title,
            fqn,
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(const VariableDeclarator& node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<FieldCommentData>(
        new FieldCommentData(
            name,
            fqn,
            this->lastDatatype,
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(const ConstructorDeclaration& node) {
    const std::string name = this->getIdentifier(node);
    const bool overloadTagExists = this->overloadTags.find(name) !=
                                   this->overloadTags.end();
    if (!overloadTagExists && !isDocumented(node)) {
        return;
    }

    const std::string fqn = this->getFQN(node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<ConstructorCommentData>(
        new ConstructorCommentData(
            name,
            fqn,
            this->params,
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(const FunctionDeclaration& node) {
    const std::string name = this->getIdentifier(node);
    const bool overloadTagExists = this->overloadTags.find(name) !=
                                   this->overloadTags.end();
    if (!overloadTagExists && !isDocumented(node)) {
        return;
    }

    const std::string fqn = this->getFQN(node);
    const std::string returnType = this->lastDatatype;
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<MethodCommentData>(
        new MethodCommentData(
            name,
            fqn,
            this->params,
            returnType,
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
}

void jspp::docgen::DocVisitor::saveDocument(const EnumDeclaration& node) {
    if (!isDocumented(node)) {
        return;
    }

    const std::string name = this->getIdentifier(node);
    const std::string fqn = this->getFQN(node);
    const std::string docText = this->getDocCommentText();

    auto comment = std::unique_ptr<EnumCommentData>(
        new EnumCommentData(
            name,
            fqn,
            this->lastDatatype,
            std::move(this->lastEnumMembers),
            docText,
            this->modifiers
        )
    );
    this->documented.insert(std::make_pair(fqn, std::move(comment)));
    this->lastEnumMembers.clear();
}

auto jspp::docgen::DocVisitor::getDocuments() ->
    std::vector<std::unique_ptr<CommentData>>
{
    std::vector<doc_comment_t> result;
    std::unordered_set<std::string> processedKeys;

    for (auto& pair : this->documented) {
        const std::string& key = pair.first;
        std::unique_ptr<CommentData>& document = pair.second;

        if (processedKeys.find(key) != processedKeys.end()) {
            continue;
        }
        processedKeys.insert(key);

        const bool isOverloaded = this->documented.count(key) > 1;
        if (!isOverloaded) {
            result.push_back(std::move(pair.second));
            continue;
        }

        if (document->is<MethodCommentData>()) {
            auto method_doc = dynamic_cast<MethodCommentData*>(document.get());
            const std::string methodName = method_doc->getName();
            const std::string methodFQN  = method_doc->getFQN();

            this->combineMethodDocs<OverloadedMethodCommentData>(
                result,
                methodName,
                methodFQN
            );
            continue;
        }
        if (document->is<ConstructorCommentData>()) {
            auto ctor_doc = dynamic_cast<ConstructorCommentData*>(document.get());
            const std::string constructorName = ctor_doc->getClassName();
            const std::string constructorFQN  = ctor_doc->getFQN();

            this->combineMethodDocs<OverloadedConstructorCommentData>(
                result,
                constructorName,
                constructorFQN
            );
            continue;
        }
    }
    this->documented.clear();

    return result;
}

std::string jspp::docgen::DocVisitor::getDocCommentText() const {
    return this->currentDocComment ? this->currentDocComment->text : "";
}

template<typename T>
void jspp::docgen::DocVisitor::combineMethodDocs(std::vector<doc_comment_t>& results,
                                                 const std::string& name,
                                                 const std::string& fqn)
{
    std::vector<std::unique_ptr<OverloadableCommentData>> overloads;
    auto iterators = this->documented.equal_range(fqn);
    for (auto it = iterators.first, end = iterators.second; it != end; ++it) {
        auto method_doc = CommentData::dynamic_unique_ptr_cast<OverloadableCommentData>(
            std::move(it->second)
        );
        overloads.push_back(std::move(method_doc));
    }

    std::unique_ptr<T> combinedDoc;
    const bool overloadTagExists = overloadTags.find(name) !=
                                   overloadTags.end();
    if (overloadTagExists) {
        combinedDoc = std::unique_ptr<T>(
            new T(
                name,
                fqn,
                std::move(overloads),
                overloadTags.at(name).get()
            )
        );
    }
    else {
        combinedDoc = std::unique_ptr<T>(
            new T(
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

std::string jspp::docgen::DocVisitor::getParameterType(const std::unique_ptr<jspp::parser::Parameter>& node) const {    
    std::string result = parser::Utils::parameterTypeToString(node);

    const bool isVariadicParam = node->is<FormalParameter>() &&
                                 node->as<FormalParameter>()->isVariadic;
    if (isVariadicParam) {
        result.insert(0, "...");
    }

    return result;
}