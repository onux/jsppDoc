#include "OutputBuilder.h"
#include "DocCommentTags.h"

std::string jspp::docgen::OutputBuilder::getOutput() const {
	return this->output.str();
}

void jspp::docgen::OutputBuilder::buildModule(const CommentData& comment) {
	std::shared_ptr<jspp::docgen::DocCommentTags> tags = comment.getTags();

	this->output << "<module>";
	this->addSummary(tags->summary);
	this->output << "</module>";
}

std::string jspp::docgen::OutputBuilder::cdata(const std::string& s) const {
	return "<![CDATA[" + s + "]]>";
}

void jspp::docgen::OutputBuilder::addSummary(const std::string& text) {
	this->output << "<summary>";
	this->output << cdata(text);
	this->output << "</summary>";
}