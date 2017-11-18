//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_DOCCOMMENTMIXIN_H
#define JSPP_DOCGEN_DOCCOMMENTMIXIN_H

#include <string>
#include <memory>
#include <unordered_set>
#include <jspp-parser/jspp-parser.h>
#include "ParsedData/OverloadTagCommentData.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `DocCommentMixin` class provides methods for visitors that visit
     * over documented JS++ AST nodes.
     */
    class DocCommentMixin
    {
    private:
        typedef std::unordered_set<std::string> overload_tags_t;

    protected:
        /**
         * Checks if the current documentation comment precedes the specified
         * abstract syntax tree (AST) node.
         *
         * @param node The AST node to check.
         */
        bool isDocumented(const jspp::parser::Node& node);
        /**
         * Checks if the current documentation comment precedes the specified
         * abstract syntax tree (AST) node. For undocumented methods that have
         * been documented with an \@overload tag, this function will still
         * return `true`.
         *
         * @param node The AST node to check.
         */
        bool isDocumented(const jspp::parser::Node& node,
                          const std::string& name,
                          const overload_tags_t& overloadTags);
        void clearDocComment();

        jspp::parser::DocComment* currentDocComment = nullptr;
    };
}
}

#endif