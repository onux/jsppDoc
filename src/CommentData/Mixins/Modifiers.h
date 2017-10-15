//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_MIXINS_MODIFIERS_H
#define JSPP_DOCGEN_COMMENTDATA_MIXINS_MODIFIERS_H

#include <bitset>
#include <memory>
#include "../../Modifiers.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `ModifiersMixin` mixin provides functions for handling AST nodes with
     * modifiers.
     */
    class ModifiersMixin
    {
    public:
        /**
         * Get the modifiers associated with the documented node.
         */
        std::shared_ptr<jspp::docgen::Modifiers> getModifiers() const;

    protected:
        /**
         * Constructs a `ModifiersMixin`.
         *
         * @param modifiers The modifiers to parse.
         */
        ModifiersMixin(const std::bitset<10>& modifiers);

    private:
        std::shared_ptr<jspp::docgen::Modifiers> modifiers;
    };
}
}

#endif