//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_INTERFACECOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_INTERFACECOMMENTDATA_H

#include <bitset>
#include <string>
#include "CommentData.h"
#include "Mixins/includes.h"

namespace jspp
{
namespace docgen
{
    /**
     * The `InterfaceCommentData` class represents all data associated with an
     * interface documentation comment *and* the interface AST node it documents
     */
    struct InterfaceCommentData : public CommentData,
                                  public NameMixin,
                                  public ModifiersMixin,
                                  public DocumentationTextMixin
    {
        InterfaceCommentData(const std::string& name,
                             const std::string& fqn,
                             const std::string& docComment,
                             const std::bitset<10>& modifiers);
    };
}
}

#endif