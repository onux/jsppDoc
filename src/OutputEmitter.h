//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_OUTPUTEMITTER_H
#define JSPP_DOCGEN_OUTPUTEMITTER_H

#include <string>

namespace jspp
{
namespace docgen
{
	/**
	 * An abstract class representing classes that write to output.
	 */
	struct OutputEmitter
	{
		virtual void write(const std::string& text, const std::string& path) = 0;
	};
}
}

#endif