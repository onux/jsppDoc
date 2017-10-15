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
#include "OutputBuilder.h"
#include "OutputEmitter.h"
#include "CommentData/includes.h"

namespace jspp
{
namespace docgen
{
    class DocVisitor : public jspp::parser::VisitorVisitChildrenByDefault
    {
    public:
        std::vector<std::unique_ptr<CommentData>> getDocuments();

        void visit(jspp::parser::DocComment* node) override;
        void visit(jspp::parser::ModuleDeclaration* node) override;
        void visit(jspp::parser::ClassDeclaration* node) override;
        void visit(jspp::parser::FunctionDeclaration* node) override;
        void visit(jspp::parser::VariableDeclaration* node) override;
        void visit(jspp::parser::StatementModifier* node) override;

        void visit(jspp::parser::IdentifierType* node) override;
        void visit(jspp::parser::MemberedType* node) override;
        void visit(jspp::parser::ArrayType* node) override;
        void visit(jspp::parser::CallbackType* node) override;
        void visit(jspp::parser::GenericInstantiationType* node) override;
    private:
        typedef std::unique_ptr<CommentData> doc_comment_t;

        std::multimap<std::string, doc_comment_t> documented;

        jspp::parser::DocComment* currentDocComment = nullptr;

        std::vector<std::string> modules;
        std::vector<std::string> classes;
        std::vector<std::string> params;
        std::bitset<10> modifiers;
        std::string lastDatatype;

        std::string getIdentifier(jspp::parser::Node* node) const;
        std::string getFQN(jspp::parser::Node* node) const;

        void saveOverload(jspp::parser::DocComment* node);
        void buildDocument(jspp::parser::ModuleDeclaration* node);
        void buildDocument(jspp::parser::ClassDeclaration* node);
        void buildDocument(jspp::parser::VariableDeclarator* node);
        void buildDocument(jspp::parser::ConstructorDeclaration* node);
        void buildDocument(jspp::parser::FunctionDeclaration* node);

        void clearDocComment();
        void clearModifiers();
        void clearParameters();
    };
}
}

#endif