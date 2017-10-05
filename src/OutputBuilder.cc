#include "OutputBuilder.h"
#include "DocCommentTags.h"
#include "Utils.h"

using namespace jspp::docgen;
using namespace jspp::parser;

std::string jspp::docgen::OutputBuilder::getOutput() const {
	return this->output.str();
}

void jspp::docgen::OutputBuilder::buildModule(const CommentData& comment) {
	std::shared_ptr<jspp::docgen::DocCommentTags> tags = comment.getTags();

	auto node = dynamic_cast<ModuleDeclaration *>(comment.getNode().get());

	const bool isPrefixModule = node->isSplit;
	if (isPrefixModule) {
		return;
	}

	std::string description = comment.getBodyText();

	this->output << "<module>";
	this->addTitle(node);
	this->addSummary(tags->summary != "" ? tags->summary : description);
	this->addDescription(description);
	this->output << "<examples>";
	for(auto example : tags->examples) {
		this->addExample(example->title, example->code);
	}
	this->output << "</examples>";
	this->output << "<see>";
	for (auto& see : tags->see_also) {
		this->addSeeAlso(see->title, see->path);
	}
	this->output << "</see>";
	this->output << "</module>";
}

std::string jspp::docgen::OutputBuilder::cdata(const std::string& s) const {
	return "<![CDATA[" + s + "]]>";
}

void jspp::docgen::OutputBuilder::addTitle(ModuleDeclaration* const node) {
	this->output << "<title>";
	this->output << cdata(node->id->name);
	this->output << "</title>";
}

void jspp::docgen::OutputBuilder::addTitle(ClassDeclaration* const node) {
	this->output << "<title>";
	this->output << cdata(node->id->name);
	this->output << "</title>";
}

void jspp::docgen::OutputBuilder::addSummary(const std::string& text) {
	this->output << "<summary>";
	this->output << cdata(text);
	this->output << "</summary>";
}

void jspp::docgen::OutputBuilder::addDescription(const std::string& text) {
	this->output << "<description>";
	this->output << cdata(text);
	this->output << "</description>";
}

void jspp::docgen::OutputBuilder::addExample(const std::string& title, const std::string& code) {
	this->output << "<example name=\"" << title << "\">";
	this->output << cdata(code);
	this->output << "</example>";
}

void jspp::docgen::OutputBuilder::addSeeAlso(const std::string& title, const std::string& page) {
	this->output << "<ref to=\"" << page << "\">";
	this->output << cdata(title);
	this->output << "</ref>";
}