//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_OUTPUTBUILDER_H
#define JSPP_DOCGEN_OUTPUTBUILDER_H

#include "CommentData.h"
#include <sstream>
#include <memory>

namespace jspp
{
namespace docgen
{
	/**
	 * The `OutputBuilder` class is responsible for building the XML output.
	 */
	class OutputBuilder
	{
	public:
		/**
		 * Gets the output XML.
		 */
		std::string getOutput() const;

		/**
		 * Builds the XML for a JS++ `module`.
		 *
		 * @param comment The processed comment data. See `CommentData`.
		 */
		void buildModule(const std::shared_ptr<CommentData> comment);
		/**
		 * Builds the XML for a JS++ `class`.
		 *
		 * @param comment The processed comment data. See `CommentData`.
		 */
		void buildClass(const std::shared_ptr<CommentData> comment);
	private:
		std::ostringstream output;

		/**
		 * Wraps the string in an XML CDATA section.
		 */
		std::string cdata(const std::string& s) const;
		/**
		 * Adds a <title> tag to the output XML document.
		 *
		 * @param node The AST node to extract the identifier from for the <title> tag.
		 */
		void addTitle(jspp::parser::ModuleDeclaration* const node);
		void addTitle(jspp::parser::ClassDeclaration* const node);
		/**
		 * Adds a <summary> tag to the output XML document.
		 */
		void addSummary(const std::string& text);
		/**
		 * Adds a <description> tag to the output XML document.
		 */
		void addDescription(const std::string& text);
		/**
		 * Adds a <example> tag to the output XML document.
		 *
		 * @param The title of the example.
		 * @param The code for the example.
		 */
		void addExample(const std::string& title, const std::string& code);
		/**
		 * Adds a <deprecated> tag with the specified reason to the output XML
		 * document.
		 *
		 * @param reason The reason the documented node is deprecated.
		 */
		void addDeprecated(const std::string& reason);
		/**
		 * Adds a <see> ("See Also") tag with the specified page to the output
		 * XML document.
		 *
		 * @param title The link text for the suggested page.
		 * @param page The fully-qualified name (FQN) or URL to suggest.
		 */
		void addSeeAlso(const std::string& title, const std::string& page);
	};
}
}

#endif