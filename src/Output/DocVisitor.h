//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_DOCVISITOR_H
#define JSPP_DOCGEN_DOCVISITOR_H

#include <jspp-parser/jspp-parser.h>
#include <jspp-parser/VisitorVisitChildrenByDefault.h>
#include <vector>
#include <string>
#include <memory>
#include <bitset>
#include <map>
#include <unordered_map>
#include "OutputBuilder.h"
#include "../Emitters/OutputEmitter.h"
#include "../CommentData/includes.h"
#include "Mixins/DocCommentMixin.h"
#include "Mixins/NameBuilderMixin.h"

namespace jspp
{
namespace docgen
{
    class DocVisitor : public jspp::parser::VisitorVisitChildrenByDefault,
                       protected DocCommentMixin,
                       protected NameBuilderMixin
    {
    public:
        std::vector<std::unique_ptr<CommentData>> getDocuments();

        void visit(jspp::parser::DocComment* node) override;
        void visit(jspp::parser::ModuleDeclaration* node) override;
        void visit(jspp::parser::ClassDeclaration* node) override;
        void visit(jspp::parser::InterfaceDeclaration* node) override;
        void visit(jspp::parser::ConstructorDeclaration* node) override;
        void visit(jspp::parser::FunctionDeclaration* node) override;
        void visit(jspp::parser::VariableDeclaration* node) override;
        void visit(jspp::parser::EnumDeclaration* node) override;
        void visit(jspp::parser::Enumerator* node) override;
        void visit(jspp::parser::StatementModifier* node) override;

        void visit(jspp::parser::IdentifierType* node) override;
        void visit(jspp::parser::MemberedType* node) override;
        void visit(jspp::parser::ArrayType* node) override;
        void visit(jspp::parser::CallbackType* node) override;
        void visit(jspp::parser::GenericInstantiationType* node) override;
    private:
        typedef std::unique_ptr<CommentData> doc_comment_t;
        typedef std::unique_ptr<OverloadTagCommentData> overload_doc_comment_t;

        std::multimap<std::string, doc_comment_t> documented;
        std::unordered_map<std::string, overload_doc_comment_t> overloadTags;

        std::string lastDatatype;
        std::vector<std::unique_ptr<EnumMemberCommentData>> lastEnumMembers;

        void saveOverload();
        void saveDocument(const jspp::parser::ModuleDeclaration& node);
        void saveDocument(const jspp::parser::ClassDeclaration& node);
        void saveDocument(const jspp::parser::InterfaceDeclaration& node);
        void saveDocument(const jspp::parser::ConstructorDeclaration& node);
        void saveDocument(const jspp::parser::FunctionDeclaration& node);
        void saveDocument(const jspp::parser::VariableDeclarator& node);
        void saveDocument(const jspp::parser::EnumDeclaration& node);

        template<typename T>
        void combineMethodDocs(std::vector<doc_comment_t>& results,
                               const std::string& name,
                               const std::string& fqn);

        std::vector<std::string> params;
        std::bitset<10> modifiers;
        void clearModifiers();
        void clearParameters();

        std::string getDocCommentText() const;

        std::string getParameterType(const std::unique_ptr<jspp::parser::Parameter>& node) const;
    };
}
}

#endif