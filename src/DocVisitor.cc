//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#include "DocVisitor.h"
#include "CommentData.h"
#include "Utils.h"

using namespace jspp::parser;

jspp::docgen::DocVisitor::DocVisitor(	const std::string& outputDir,
										OutputBuilder* const builder,
										OutputEmitter* const emitter)
									:
										outputDir(outputDir),
										builder(builder),
										emitter(emitter)
{
}

void jspp::docgen::DocVisitor::visit(jspp::parser::DocComment* node) {
	this->currentDocComment = node;
}

void jspp::docgen::DocVisitor::visit(ModuleDeclaration* node) {
	this->modules.push_back(node->id->name);

	const bool isDocumented = 	this->currentDocComment &&
								this->currentDocComment->isBefore(node);
	if (isDocumented) {
		this->buildDocument(node);
	}

	visitChildren(node);
	this->modules.pop_back();
}

void jspp::docgen::DocVisitor::visit(ClassDeclaration* node) {
	this->classes.push_back(node->id->name);
	visitChildren(node);
	this->classes.pop_back();
}

void jspp::docgen::DocVisitor::visit(FunctionDeclaration* node) {
}

void jspp::docgen::DocVisitor::visit(VariableStatement* node) {
}

void jspp::docgen::DocVisitor::visit(StatementModifier* node) {
	this->modifiers.set(static_cast<int>(node->modifier));
	visitChildren(node);
	this->modifiers.reset();
}

std::string jspp::docgen::DocVisitor::getFQN(Node* node) const {
	if (node->is<ModuleDeclaration>()) {
		return utils::join(this->modules, ".");
	}
	return "";
}

void jspp::docgen::DocVisitor::buildDocument(Node* node) {
	CommentData comment(
		node,
		this->getFQN(node),
		this->currentDocComment->text,
		this->modifiers
	);

	std::string xml, identifier;
	if (node->is<ModuleDeclaration>()) {
		this->builder->buildModule(comment);
		xml = this->builder->getOutput();
		identifier = dynamic_cast<ModuleDeclaration *>(node)->id->name;
	}

	if (xml != "") {
		this->emitter->write(xml, this->outputPath);
	}
}