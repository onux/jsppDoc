//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_H

#include "Modifiers.h"
#include "DocCommentTags.h"
#include <bitset>
#include <memory>
#include <jspp-parser/jspp-parser.h>

namespace jspp
{
namespace docgen
{
	/**
	 * The `CommentData` class represents all data associated with a 
	 * documentation comment *and* the AST node it documents.
	 */
	class CommentData
	{
	public:
		CommentData(jspp::parser::Node const* node,
					const std::string& fqn,
					const std::string& docComment,
					const std::bitset<10>& modifiers);

		/**
		 * Return the raw, unformatted documentation text.
		 */
		std::string getText() const;
		/**
		 * Get the fully-qualified name (FQN) of the documented node.
		 */
		std::string getFQN() const;
		/**
		 * Get the modifiers associated with the documented node.
		 */
		std::shared_ptr<jspp::docgen::Modifiers> getModifiers() const;
		/**
		 * Get the documented node.
		 */
		std::shared_ptr<jspp::parser::Node> getNode() const;
		/**
		 * Get the documentation tags and their parsed associated data.
		 */
		std::shared_ptr<jspp::docgen::DocCommentTags> getTags() const;
		/**
		 * Get the documentation body text.
		 */
		std::string getBodyText() const;
	private:
		std::string text;
		std::string fqn;
		std::string description;
		std::shared_ptr<jspp::docgen::Modifiers> modifiers;
		std::shared_ptr<jspp::parser::Node> node;
		std::shared_ptr<jspp::docgen::DocCommentTags> tags;
	};
}
}

#endif