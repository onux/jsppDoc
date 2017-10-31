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

        /**
         * Returns the string representation for the identifier of an AST node
         * including generic parameters.
         */
        template<typename T>
        const std::string getGenericTitle(jspp::parser::Node* node) const
        {
            constexpr const bool isUserDefinedType =
                std::is_base_of<jspp::parser::ClassDeclaration, T>::value ||
                std::is_base_of<jspp::parser::InterfaceDeclaration, T>::value;

            static_assert(
                isUserDefinedType,
                "'getGenericTitle' expects a 'ClassDeclaration' or 'InterfaceDeclaration' node"
            );

            auto node_cast = node->as<T>();
            std::string result = node_cast->id->name;

            const bool isGeneric = node_cast->genericParams != nullptr &&
                                   node_cast->genericParams->size() != 0;
            if (isGeneric) {
                result += "<";
                bool first = true;
                auto& genericParams = *node_cast->genericParams;
                for (auto& param : genericParams) {
                    if (!first) {
                        result += ", ";
                    }

                    result += param->id->name;

                    first = false;
                }
                result += ">";
            }

            return result;
        }

        std::vector<std::string> modules;
        std::vector<std::string> userDefinedTypes;
    };
}
}

#endif