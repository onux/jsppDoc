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
#include <queue>
#include "OutputBuilder.h"
#include "OutputEmitter.h"

namespace jspp
{
namespace docgen
{
    class DocVisitor : public jspp::parser::VisitorVisitChildrenByDefault
    {
    public:
        std::queue<std::shared_ptr<CommentData>> getDocuments() const;

        void visit(jspp::parser::DocComment* node) override;
        void visit(jspp::parser::ModuleDeclaration* node) override;
        void visit(jspp::parser::ClassDeclaration* node) override;
        void visit(jspp::parser::FunctionDeclaration* node) override;
        void visit(jspp::parser::VariableStatement* node) override;
        void visit(jspp::parser::StatementModifier* node) override;
    private:
        typedef std::shared_ptr<jspp::parser::FunctionDeclaration> method_t;
        typedef std::shared_ptr<CommentData> doc_comment_t;

        std::vector<std::string> modules;
        std::vector<std::string> classes;
        std::bitset<10> modifiers;
        std::multimap<std::string, method_t> overloads;
        std::queue<doc_comment_t> documented;
        jspp::parser::DocComment* currentDocComment = nullptr;

        std::string getFQN(jspp::parser::Node* node) const;
        void buildDocument(jspp::parser::Node* node);
        
        void clearDocComment();
        void clearModifiers();
    };
}
}

#endif