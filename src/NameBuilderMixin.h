//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_NAMEBUILDERMIXIN_H
#define JSPP_DOCGEN_NAMEBUILDERMIXIN_H

#include <string>
#include <jspp-parser/jspp-parser.h>

namespace jspp
{
namespace docgen
{
    /**
     * The `NameBuilderMixin` class provides methods to build fully-qualified
     * names (FQNs) and decode identifiers for visitors that visit over
     * documented JS++ AST nodes.
     */
    class NameBuilderMixin
    {
    protected:
        /**
         * Returns the string representation for the identifier of an AST node.
         */
        const std::string getIdentifier(jspp::parser::Node* node) const;
        /**
         * Returns the fully-qualified name (FQN) of the specified AST node.
         */
        const std::string getFQN(jspp::parser::Node* node) const;

        std::vector<std::string> modules;
        std::vector<std::string> classes;
    };
}
}

#endif