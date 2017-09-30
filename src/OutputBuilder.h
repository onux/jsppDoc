//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_OUTPUTBUILDER_H
#define JSPP_DOCGEN_OUTPUTBUILDER_H

#include "CommentData.h"
#include <sstream>

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
		void buildModule(const CommentData& comment);
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
	};
}
}

#endif