//
// Copyright (C) 2017 Onux Corporation. All rights reserved.
//

#ifndef JSPP_DOCGEN_COMMENTDATA_ENUMMEMBERCOMMENTDATA_H
#define JSPP_DOCGEN_COMMENTDATA_ENUMMEMBERCOMMENTDATA_H

#include "CommentData.h"
#include "Mixins/includes.h"
#include <bitset>
#include <string>

namespace jspp
{
namespace docgen
{
    /**
     * The `EnumMemberCommentData` class represents all data associated with a 
     * documentation comment for an enum *and* the Enumerator AST node it
     * documents.
     */
    class EnumMemberCommentData : public CommentData,
                                  public DocumentationTextMixin
    {
    public:
        EnumMemberCommentData(const std::string& name,
                              const std::string& docComment);

        /**
         * Get the identifier for the documented enumeration member.
         */
        std::string getName() const;

    private:
        std::string name;
    };
}
}

#endif