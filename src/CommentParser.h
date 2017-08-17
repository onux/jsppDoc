//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTPARSER_H
#define JSPP_DOCGEN_COMMENTPARSER_H

#include "Modifiers.h"
#include "DocCommentTags.h"

#include <bitset>
#include <string>
#include <jspp-parser/jspp-parser.h>

namespace jspp
{
namespace docgen
{
	/**
	 * `CommentParser` is responsible for parsing JS++ documentation comments
	 * to process documentation tags, text, and so forth.
	 */
	struct CommentParser
	{
		/**
		 * Get the modifiers for the documented node.
		 */
		std::unique_ptr<Modifiers> parseModifiers(const std::bitset<10>& modifiers) const;
		/**
		 * Parses out JS++ documentation comment delimiters such as /\htmlonly\endhtmlonly** and *\htmlonly\endhtmlonly/
		 *
		 * @param docComment The raw text from the AST node for the documentation comment.
		 */
		std::string parseDocCommentText(const std::string& docComment) const;
		/**
		 * Parses the @tag format for JS++ documentation comments.
		 *
		 * @param text The text with comment delimiters stripped. See `parseDocCommentText()`.
		 */
		std::unique_ptr<DocCommentTags> parseDocCommentTags(const std::string& text) const;
		/**
		 * Parses the description (body) text for a JS++ documentation comment.
		 *
		 * @param text The text with comment delimiters stripped. See `parseDocCommentText()`.
		 */
		std::string parseDocCommentDescription(const std::string& text) const;
		/**
		 * Returns the Markdown-formatted text for a given string.
		 */
		std::string markdown(const std::string& text) const;
	};
}
}

#endif