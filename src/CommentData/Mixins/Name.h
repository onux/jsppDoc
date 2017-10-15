//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_MIXINS_NAME_H
#define JSPP_DOCGEN_COMMENTDATA_MIXINS_NAME_H

#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `NameMixin` mixin provides functions for handling AST nodes with
     * names (identifiers).
     */
    class NameMixin
    {
    public:
        /**
         * Get the name of the documented node. This is usually the identifier
         * for the AST node but may be the class name for constructors.
         */
        std::string getName() const;
        /**
         * Get the fully-qualified name (FQN) of the documented node.
         */
        std::string getFQN() const;

    protected:
        /**
         * Constructs a `NameMixin`.
         *
         * @param name The name (usually identifier) for the documented node.
         * @param fqn The FQN (fully-qualified name) for the documented node.
         */
        NameMixin(const std::string& name, const std::string& fqn);

    private:
        std::string name;
        std::string fqn;
    };
}
}

#endif