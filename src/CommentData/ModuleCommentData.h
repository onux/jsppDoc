//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_MODULECOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_MODULECOMMENTDATA_H

#include <bitset>
#include <string>
#include "CommentData.h"
#include "Mixins/includes.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `ModuleCommentData` class represents all data associated with a 
     * module documentation comment *and* the module AST node it documents.
     */
    struct ModuleCommentData : public CommentData,
                               public NameMixin,
                               public ModifiersMixin,
                               public DocumentationTextMixin
    {
        ModuleCommentData(const std::string& name,
                          const std::string& fqn,
                          const std::string& docComment,
                          const std::bitset<10>& modifiers);
    };
}
}

#endif