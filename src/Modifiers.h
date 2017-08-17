//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_MODIFIERS_H
#define JSPP_DOCGEN_MODIFIERS_H

namespace jspp
{
namespace docgen
{
	/**
	 * Used for representing the modifiers applied to a documented AST node.
	 */
	struct Modifiers
	{
		bool isPublic = false;
		bool isProtected = false;
		bool isPrivate = false;
		bool isStatic = false;
		bool isFinal = false;
		bool isInline = false;
		bool isProperty = false;
		bool isAbstract = false;
		bool isVirtual = false;
		bool isOverride = false;
	};
}
}

#endif