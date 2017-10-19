//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_METHODCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_METHODCOMMENTDATA_H

#include "CommentData.h"
#include "Mixins/includes.h"
#include <bitset>
#include <vector>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `MethodCommentData` class represents the data associated with the
     * documentation comment for a method *and* the FunctionDeclaration AST
     * node it documents.
     *
     * This class only represents a single method. Objects of this class are
     * eventually combined into `OverloadedMethodCommentData` to represent all
     * overloads of the method.
     */
    class MethodCommentData : public CommentData,
                              public NameMixin,
                              public ModifiersMixin,
                              public DocumentationTextMixin
    {
    public:
        MethodCommentData(const std::string& name,
                          const std::string& fqn,
                          std::vector<std::string> param_types,
                          const std::string return_type,
                          const std::string& docComment,
                          const std::bitset<10>& modifiers);

        /**
         * Get the declared return type for the documented method.
         */
        std::string getReturnType() const;

        /**
         * Get the declared parameter type at the specified index.
         *
         * @param index The zero-based index for the parameter.
         */
        std::string getParameterType(size_t index) const;

    private:
        std::vector<std::string> param_types;
        std::string return_type;
    };
}
}

#endif