//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_ENUMCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_ENUMCOMMENTDATA_H

#include "CommentData.h"
#include "EnumMemberCommentData.h"
#include "Mixins/includes.h"
#include <bitset>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `EnumCommentData` class represents all data associated with a 
     * documentation comment for an enum *and* the EnumDeclaration AST node
     * it documents.
     */
    class EnumCommentData : public CommentData,
                            public NameMixin,
                            public ModifiersMixin,
                            public DocumentationTextMixin
    {
    public:
        EnumCommentData(const std::string& name,
                        const std::string& fqn,
                        const std::string& datatype,
                        std::vector<std::unique_ptr<EnumMemberCommentData>>&& members,
                        const std::string& docComment,
                        const std::bitset<10>& modifiers);

        /**
         * Get the declared datatype for the documented field.
         */
        std::string getDatatype() const;

        /**
         * Gets the documentation comment data for the enumeration members.
         */
        const std::vector<std::unique_ptr<EnumMemberCommentData>>& getMembers() const;

    private:
        typedef std::vector<std::unique_ptr<EnumMemberCommentData>> members_t;

        std::string datatype;
        members_t members;
    };
}
}

#endif