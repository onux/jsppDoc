//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_CLASSCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_CLASSCOMMENTDATA_H

#include <bitset>
#include <string>
#include "CommentData.h"
#include "Mixins/includes.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `ClassCommentData` class represents all data associated with a 
     * class documentation comment *and* the class AST node it documents.
     */
    struct ClassCommentData : public CommentData,
                              public NameMixin,
                              public ModifiersMixin,
                              public DocumentationTextMixin
    {
        ClassCommentData(const std::string& name,
                         const std::string& fqn,
                         const std::string& docComment,
                         const std::bitset<10>& modifiers);
    };
}
}

#endif