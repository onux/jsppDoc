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
		 * Adds a <summary> tag to the output XML document.
		 */
		void addSummary(const std::string& text);
	};
}
}

#endif