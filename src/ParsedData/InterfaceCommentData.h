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
    class InterfaceCommentData : public CommentData,
                                 public NameMixin,
                                 public ModifiersMixin,
                                 public DocumentationTextMixin
    {
    public:
        InterfaceCommentData(const std::string& name,
                             const std::string& title,
                             const std::string& fqn,
                             const std::string& docComment,
                             const std::bitset<10>& modifiers);

        /**
         * Returns the title for the documentation page. Interfaces differ
         * from most other AST nodes because they can be generic, and a custom
         * title allows the documentation generator to include generic
         * parameters and constraints.
         *
         * @return The identifier for the InterfaceDeclaration AST node
         *         including generic parameters and constraints.
         */
        std::string getTitle() const;

    private:
        std::string title;
    };
}
}

#endif