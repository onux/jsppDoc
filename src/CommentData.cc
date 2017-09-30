#include "CommentData.h"
#include "CommentParser.h"
#include "Utils.h"

jspp::docgen::CommentData::CommentData(	jspp::parser::Node const* node,
										const std::string& fqn,
										const std::string& docComment,
										const std::bitset<10>& modifiers)
{
	this->node 	= std::shared_ptr<jspp::parser::Node>(node->clone());
	this->fqn 	= fqn;

	CommentParser parser;
	this->text 			=	parser.parseDocCommentText(docComment);
	this->modifiers 	= 	std::move(parser.parseModifiers(modifiers));
	this->tags 			=	parser.parseDocCommentTags(this->text);
	this->description 	= 	parser.parseDocCommentDescription(this->text);
}

std::string jspp::docgen::CommentData::getText() const {
	return this->text;
}
std::string jspp::docgen::CommentData::getFQN() const {
	return this->fqn;
}
std::shared_ptr<jspp::docgen::Modifiers> jspp::docgen::CommentData::getModifiers() const {
	return this->modifiers;
}
std::shared_ptr<jspp::parser::Node> jspp::docgen::CommentData::getNode() const {
	return this->node;
}
std::shared_ptr<jspp::docgen::DocCommentTags> jspp::docgen::CommentData::getTags() const {
	return this->tags;
}
std::string jspp::docgen::CommentData::getBodyText() const {
	return this->description;
}