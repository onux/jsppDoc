//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_OVERLOADABLECOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_OVERLOADABLECOMMENTDATA_H

#include "CommentData.h"
#include "Mixins/Modifiers.h"
#include "Mixins/DocumentationText.h"
#include <bitset>
#include <vector>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `OverloadableCommentData` class represents the data associated with
     * the documentation comment for an overloadable method or constructor and
     * the AST node it documents.
     */
    class OverloadableCommentData : public CommentData,
                                    public ModifiersMixin,
                                    public DocumentationTextMixin
    {
    public:
        OverloadableCommentData(const std::vector<std::string>& param_types,
                                const std::string& docComment,
                                const std::bitset<10>& modifiers);

        /**
         * Get the declared parameter type at the specified index.
         *
         * @param index The zero-based index for the parameter.
         */
        std::string getParameterType(size_t index) const;

    protected:
        std::vector<std::string> param_types;
    };
}
}

#endif