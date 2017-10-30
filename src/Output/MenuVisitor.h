//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_MENUVISITOR_H
#define JSPP_DOCGEN_MENUVISITOR_H

#include "MenuBuilder.h"
#include "Mixins/DocCommentMixin.h"
#include "Mixins/NameBuilderMixin.h"
#include <jspp-parser/jspp-parser.h>
#include <jspp-parser/VisitorVisitChildrenByDefault.h>

namespace jspp
{
namespace docgen
{
    /**
     * The `MenuVisitor` class visits over AST nodes where producing
     * documentation menu XML would be relevant.
     */
    class MenuVisitor : public jspp::parser::VisitorVisitChildrenByDefault,
                        public MenuBuilder,
                        protected DocCommentMixin,
                        protected NameBuilderMixin
    {
    public:
        void visit(jspp::parser::DocComment* node) override;
        void visit(jspp::parser::ModuleDeclaration* node) override;
        void visit(jspp::parser::ClassDeclaration* node) override;
        void visit(jspp::parser::InterfaceDeclaration* node) override;
        void visit(jspp::parser::ConstructorDeclaration* node) override;
        void visit(jspp::parser::FunctionDeclaration* node) override;
        void visit(jspp::parser::VariableDeclaration* node) override;
        void visit(jspp::parser::EnumDeclaration* node) override;

    private:
        bool inModule = false;
        bool hasConstructor = false;
        void saveOverload(jspp::parser::DocComment* node);
    };
}
}

#endif