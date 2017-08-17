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
#include "OutputBuilder.h"

namespace jspp
{
namespace docgen
{
	class DocVisitor : public jspp::parser::VisitorVisitChildrenByDefault
	{
	public:
		DocVisitor(OutputBuilder* builder);

		void visit(jspp::parser::DocComment* node) override;
		void visit(jspp::parser::ModuleDeclaration* node) override;
		void visit(jspp::parser::ClassDeclaration* node) override;
		void visit(jspp::parser::FunctionDeclaration* node) override;
		void visit(jspp::parser::VariableStatement* node) override;
		void visit(jspp::parser::StatementModifier* node) override;
	private:
		std::vector<std::string> modules;
		std::vector<std::string> classes;
		std::bitset<10> modifiers;
		jspp::parser::DocComment* currentDocComment = nullptr;
		OutputBuilder* builder = nullptr;

		std::string getFQN(jspp::parser::Node *node) const;
		void buildDocument(jspp::parser::Node* node);
	};
}
}

#endif