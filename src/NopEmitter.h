//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_OUTPUTEMITTER_NOP_H
#define JSPP_DOCGEN_OUTPUTEMITTER_NOP_H

#include "OutputEmitter.h"

namespace jspp
{
namespace docgen
{
	/**
	 * This class will perform a NOP (no operation) when attempting to output
	 * data. This class is useful for testing.
	 */
	struct NopEmitter : OutputEmitter
	{
		/**
		 * Performs a NOP (no operation) when attempting to write to output.
		 */
		void write(const std::string&, const std::string&) override;
	};
}
}

#endif