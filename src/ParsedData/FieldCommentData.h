//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_FIELDCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_FIELDCOMMENTDATA_H

#include "CommentData.h"
#include "Mixins/includes.h"
#include <bitset>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `FieldCommentData` class represents all data associated with a 
     * documentation comment for a field *and* the VariableDeclarator AST node
     * it documents.
     */
    class FieldCommentData : public CommentData,
                             public NameMixin,
                             public ModifiersMixin,
                             public DocumentationTextMixin
    {
    public:
        FieldCommentData(const std::string& name,
                         const std::string& fqn,
                         const std::string& datatype,
                         const std::string& docComment,
                         const std::bitset<10>& modifiers);

        /**
         * Get the declared datatype for the documented field.
         */
        std::string getDatatype() const;

    private:
        std::string datatype;
    };
}
}

#endif